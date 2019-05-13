#include "cv_helper.h"
#include <fstream>

void dft_ex(const cv::String &img_path)
{
	std::cerr.rdbuf(std::cout.rdbuf());

	cv::Mat img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);//must be grayscale...
	CV_Assert(!img.empty());

	cv::Mat padded;
	int m = cv::getOptimalDFTSize(img.rows);
	int n = cv::getOptimalDFTSize(img.cols);

	cv::copyMakeBorder(img, padded, 0, m - img.rows, 0, n - img.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::Mat _m(padded.size(), CV_32FC1);
	padded.convertTo(_m, CV_32FC1);
	cv::Mat planes[] = {cv::Mat_<float>(padded),cv::Mat::zeros(padded.size(),CV_32FC1)};
	cv::Mat complex;
	cv::merge(planes, 2, complex);

	std::cout << std::boolalpha <<(complex.type()== CV_32FC1) << std::endl;
	std::cout << complex.type() << std::endl;
	cv::dft(complex, complex);

	cv::split(complex, planes);

	auto func_write_file = [](const std::string &filename,cv::Mat &m)
	{
		std::ofstream of(filename);
		CV_Assert(of);
		CerrRdWrapper rd(of);
		PRINTMAT_F(m, NPF);
	};

	/*see the real part and the imagine part of dft*/
	//func_write_file("dft_real_part.txt", planes[0]);
	//func_write_file("dft_imagine_part.txt", planes[1]);


	cv::magnitude(planes[0], planes[1], planes[0]);
	cv::Mat mag = planes[0];

	cv::Mat mag_without_log = mag.clone();
	double _min=0, _max=0;
	cv::minMaxLoc(mag_without_log, &_min, &_max);
	CV_Assert(_max!=0 && _max>255.f);
	mag_without_log = 255.f*mag_without_log / _max;

	mag += cv::Scalar::all(1);
	log(mag,mag);
	//see lena-log-mag
	{
		std::ofstream of("lena-log-mag.txt");
		CerrRdWrapper wrapper(of);
		PRINTMAT_F(mag,NPF);
	}
	{
		std::ofstream of("lena-log-mag-norm.txt");
		CerrRdWrapper wrapper(of);
		cv::Mat out;
		cv::normalize(mag, out, 0, 1, cv::NORM_MINMAX);
		PRINTMAT_F(out, NPF);

	}

	cv::Mat mag_clone = mag.clone();

	auto func_quadrants_switch = [](cv::Mat &mag)
	{
		mag = cv::Mat(mag, cv::Rect(0, 0, mag.cols&(-2), mag.rows&(-2)));
		int cx = mag.cols / 2;
		int cy = mag.rows / 2;

		cv::Mat q0(mag, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
		cv::Mat q1(mag, cv::Rect(cx, 0, cx, cy));  // Top-Right
		cv::Mat q2(mag, cv::Rect(0, cy, cx, cy));  // Bottom-Left
		cv::Mat q3(mag, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
		cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);
		q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);
	};

	func_quadrants_switch(mag);
	func_quadrants_switch(mag_without_log);


	cv::normalize(mag, mag, 0, 1, cv::NORM_MINMAX);


	cv::normalize(mag_clone, mag_clone, 0, 1, cv::NORM_MINMAX);

	//std::ofstream of("without_log.txt");
	//std::cerr.rdbuf(of.rdbuf());
	//PRINTMAT_F(mag_without_log,NPF);
	cv::Mat mag_without_log_gray(mag_without_log.size(),CV_8UC1);
	mag_without_log.convertTo(mag_without_log_gray, CV_8UC1);
	cv::normalize(mag_without_log_gray, mag_without_log_gray, 0, 255, cv::NORM_MINMAX);

	cv::Mat mag_gray(mag.size(),CV_8UC1);
	mag.convertTo(mag_gray, CV_8UC1,255);

	cv::Mat mag_clone_gray(mag_clone.size(), CV_8UC1);
	mag_clone.convertTo(mag_clone_gray, CV_8UC1, 255);

	cv::imwrite("dft_outxxx.png", mag_gray);

	cv::imshow("src", img);
	cv::imshow("shifted without log", mag_without_log_gray);
	cv::imshow("no-shifted with log", mag_clone_gray);
	cv::imshow("shifted with log", mag_gray);
	cv::waitKey();
}

void mat_ex_1225()
{
	cv::Mat m = (cv::Mat_<float>(2, 3)<<1,2,3,4,5,6);
	m += 1;
	m *= 2;
	PRINTMAT_F(m,NPF);

	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx("lena", img_path));
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());

	cv::Mat img_float;
	img.convertTo(img_float, CV_32FC1);
	img_float /= 255.0;
	cv::normalize(img_float, img_float, 0, 1, cv::NORM_MINMAX);

	cv::Mat img2;
	img_float *= 255;
	cv::normalize(img_float, img_float, 0, 255, cv::NORM_MINMAX);
	img_float.convertTo(img2, CV_8UC1);
	cv::imshow("xxx", img2);
	cv::waitKey();


}

int main_dft(int argc,char **argv)
{
	char *img = (argc==2) ? argv[1] : "lena";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(img, img_path));
	//CV_TRY_CATCH(mat_ex_1225());
	CV_TRY_CATCH(dft_ex(img_path));
	return 0;
}