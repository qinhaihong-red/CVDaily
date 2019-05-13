#include "cv_helper.h"

static int radius = 1, max_radius=100;
static int nsr = 1, max_nsr = 100;//base number,will be multiplied by times.
static int times = 1, max_times = 3;//1x,10x,100x of nsr
static int multiplier = 10;
static cv::Mat img;
static void filter_impl();
static void radius_callback(int, void *)
{
	filter_impl();
}

static void nsr_callback(int, void *)
{
	filter_impl();
}

static void times_callback(int, void*)
{
	COND_BEGIN_ONCE
		COND_PRED_BREAK(times == 1, multiplier = 1);
		COND_PRED_BREAK(times == 2, multiplier = 10);
		COND_PRED_BREAK(times == 3, multiplier = 100);
	COND_END_ONCE
	filter_impl();
}

static void shift(cv::Mat &dft, cv::Mat &shift)
{
	shift = dft.clone();
	int cx = shift.cols / 2;
	int cy = shift.rows / 2;

	cv::Mat q0(shift, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	cv::Mat q1(shift, cv::Rect(cx, 0, cx, cy));  // Top-Right
	cv::Mat q2(shift, cv::Rect(0, cy, cx, cy));  // Bottom-Left
	cv::Mat q3(shift, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
	cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
}


static void get_PSF(cv::Size sz, int r, cv::Mat &out)//point spread function,the filter
{
	out = cv::Mat::zeros(sz,CV_32F);
	cv::Point center(out.cols / 2, out.rows / 2);
	cv::circle(out, center, r, 255, -1);//negative eqs filled
	cv::Scalar s = cv::sum(out);
	out = out / s[0];
}

static void get_WienerFilter(cv::Mat &psf,int nsr,cv::Mat &out)
{
	double snr = 1.0 / double(nsr);
	cv::Mat psf_shift;
	shift(psf, psf_shift);
	cv::Mat planes[] = {cv::Mat_<float>(psf_shift.clone()),cv::Mat::zeros(psf_shift.size(),CV_32F)};
	cv::Mat complexI;
	cv::merge(planes, 2, complexI);
	cv::dft(complexI, complexI);
	cv::split(complexI, planes);
	cv::Mat _norm;
	cv::pow(cv::abs(planes[0]), 2, _norm);
	_norm += snr;
	cv::divide(planes[0], _norm, out);

}


static void filter_impl()
{
	cv::Mat psf;
	get_PSF(cv::Size(img.size()), radius, psf);
	cv::Mat wnerFilter;
	get_WienerFilter(psf, nsr*multiplier, wnerFilter);

	cv::Mat planes[] = {cv::Mat_<float>(img.clone()),cv::Mat::zeros(img.size(),CV_32F)};
	cv::Mat complexI;
	cv::merge(planes, 2, complexI);
	cv::dft(complexI, complexI, cv::DFT_SCALE);

	cv::Mat winer_planes[] = {cv::Mat_<float>(wnerFilter.clone()),cv::Mat::zeros(wnerFilter.size(),CV_32F) };
	cv::Mat complexWiener;
	cv::merge(winer_planes,2,complexWiener);

	cv::Mat complexOut;
	cv::mulSpectrums(complexI, complexWiener, complexOut,0);
	cv::Mat dst;
	cv::idft(complexOut, dst, cv::DFT_REAL_OUTPUT);

	cv::normalize(dst, dst, 0, 1, cv::NORM_MINMAX);
	dst.convertTo(dst, CV_8U, 255);

	//cv::idft(complexOut, complexOut);
	//split(complexOut, planes);
	//dst = planes[0];
	//cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
	//dst.convertTo(dst, CV_8U);

	cv::imshow(DST_WIND, dst);
}


static void deblur_filter(cv::String &img_path)
{
	cv::Mat src = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!src.empty());

	int rows = cv::getOptimalDFTSize(src.rows);
	int cols = cv::getOptimalDFTSize(src.cols);

	cv::copyMakeBorder(src, img, 0, rows - src.rows, 0, cols - src.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::namedWindow(DST_WIND, cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("radius£º", DST_WIND, &radius, max_radius, radius_callback);
	cv::createTrackbar("nsr£º", DST_WIND, &nsr, max_nsr, nsr_callback);
	cv::createTrackbar("times:",DST_WIND,&times,max_times,times_callback);

	cv::imshow("src", src);

	cv::waitKey();

}

int main_wiener_deblur(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "out_of_focus";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(deblur_filter(img_path));
	return 0;
}