#include "cv_helper.h"

static void histogram_eq(const cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());
	CV_CVT2GRAY(img, gray);
	cv::Mat dst;
	cv::equalizeHist(gray, dst);
	cv::imshow("src", gray);
	cv::imshow("dst", dst);
	cv::waitKey();
}


int main_hist(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(histogram_eq(img_path));
	return 0;
}