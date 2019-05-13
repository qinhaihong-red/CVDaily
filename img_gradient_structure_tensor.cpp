#include "cv_helper.h"
#include <time.h>


static void ex()
{
	int seed = clock();
	
	cv::RNG rng(seed);
	cv::Mat m(2,3,CV_32F);
	rng.fill(m, cv::RNG::UNIFORM, 0, 1);
	PRINTMAT_F(m,NPF);
	cv::Mat rout;
	cv::inRange(m, cv::Scalar(0.4), cv::Scalar(0.8), rout);
	PRINTMAT_F(rout,NPF);

	cv::Mat _m = m > 0.5f;
	PRINTMAT_F(_m,NPF);

	cv::Mat m2 = (cv::Mat_<uchar>(3, 3) << 1, 2, 5,
		4, 5, 11,
		7, 8, 9);

	cv::Mat sobel_x,sobel_xx;
	cv::Sobel(m2, sobel_x, CV_32F, 1, 0);
	cv::multiply(sobel_x, sobel_x, sobel_xx);
	PRINTMAT_F(sobel_xx,NPF);
	cv::Mat sobel_x2;
	cv::Sobel(m2, sobel_x2, CV_32F, 2, 0);
	PRINTMAT_F(sobel_x2,NPF);


	double x1 = 1, y1 = 1;
	double x2 = -1, y2 = 1;
	double x3 = -1, y3 = -1;
	double x4 = 1, y4 = -1;

	cv::Scalar s1;
	cv::phase(cv::Scalar(x1), cv::Scalar(y1), s1, true);
	std::cout << "\ncalc phased:\n"<<s1[0] << std::endl;
	cv::phase(cv::Scalar( x2), cv::Scalar( y2), s1, true);
	std::cout << s1[0] << std::endl;
	cv::phase(cv::Scalar( x3), cv::Scalar( y3), s1, false);
	std::cout << s1[0] << std::endl;
	cv::phase(cv::Scalar (x4), cv::Scalar( y4), s1, false);
	std::cout << s1[0] << std::endl;
}

static void gst_impl(cv::Mat &input, cv::Mat &coherency, cv::Mat &orientation, int w);
static void calc_gradient_structure_tensor(cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img.empty());

	int low_thr = 35;
	int high_thr = 57;
	double coherency_thr = 0.43;
	int w = 52;

	cv::Mat coherency, oritentation;
	gst_impl(img, coherency, oritentation, w);
	cv::Mat coherency_bin = coherency > coherency_thr;
	cv::Mat oritentation_bin;
	cv::inRange(img, cv::Scalar(low_thr), cv::Scalar(high_thr), oritentation_bin);

	cv::normalize(coherency_bin, coherency_bin, 0, 255, cv::NORM_MINMAX);
	cv::normalize(oritentation_bin,oritentation_bin,0,255,cv::NORM_MINMAX);

	coherency_bin.convertTo(coherency_bin, CV_8U);
	oritentation_bin.convertTo(oritentation_bin, CV_8U);

	cv::Mat img_bin = coherency_bin&oritentation_bin;

	cv::imshow("coherency", coherency_bin);
	cv::imshow("orientaion", oritentation_bin);
	cv::imshow("img", 0.5*(img_bin + img));
	
	cv::waitKey();
}

static void gst_impl(cv::Mat &input, cv::Mat &coherency, cv::Mat &orientation, int w)
{
	cv::Mat img;
	input.convertTo(img, CV_32F);

	cv::Mat sobel_x, sobel_y;
	cv::Sobel(img, sobel_x, CV_32F, 1, 0);
	cv::Sobel(img,sobel_y,CV_32F,0,1);

	cv::Mat xy, x2, y2;
	xy = sobel_x*sobel_y;
	x2 = sobel_x*sobel_x;
	y2 = sobel_y*sobel_y;

	cv::boxFilter(xy, xy, CV_32F, cv::Size(w, w));
	cv::boxFilter(x2, x2, CV_32F, cv::Size(w,w));
	cv::boxFilter(y2, y2, CV_32F, cv::Size(w,w));

	cv::Mat temp1, temp2, temp3, temp4;
	temp1 = x2 + y2;
	temp2 = x2 - y2;
	temp2 = temp2*temp2;
	temp3 = xy*xy;
	cv::sqrt(temp2 + 4 * temp3,temp4);

	cv::Mat lambda1, lambda2;
	lambda1 = temp1 + temp4;
	lambda2 = temp1 - temp4;


	coherency = (lambda1 - lambda2) / (lambda1+lambda2);
	cv::phase(y2 - x2, 2.0*xy, orientation, true);
	orientation = 0.5*orientation;
}


int main_gst(_MAIN_ARGS)
{

	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(calc_gradient_structure_tensor(img_path));

	return 0;
}