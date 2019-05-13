#include "cv_helper.h"

static void laplacian_ex(const cv::String &img_path)
{
	cv::Mat src = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!src.empty());
	cv::GaussianBlur(src, src, cv::Size(3, 3),0);
	//cv::Mat img; cv::cvtColor(src, img, cv::COLOR_BGR2GRAY);
	cv::Mat img = src.clone();

	cv::Mat dst;
	cv::Laplacian(img, dst, CV_32F, 3);
	cv::Mat img2;
	img.convertTo(img2, CV_32F);
	cv::Mat temp = img2 - dst;
	cv::Mat lap, result;
	dst.convertTo(lap, CV_8UC3);
	temp.convertTo(result, CV_8UC3);

	//compare with filter2d
	cv::Mat filter = (cv::Mat_<float>(3, 3)<<1,1,1,
											 1,-8,1,
											 1,1,1);
	cv::Mat filterout;
	cv::filter2D(img, filterout, CV_32F, filter);
	cv::Mat temp2 = img2 - filterout;
	cv::Mat lap2, result2;
	filterout.convertTo(lap2, CV_8UC3);
	temp2.convertTo(result2, CV_8UC3);



	cv::imshow("src", img);
	cv::imshow("laplacian(buit-in)", lap);
	cv::imshow("result(built-in)", result);
	cv::imshow("laplacian(filter2d)", lap2);
	cv::imshow("result(filter2d)", result2);
	cv::waitKey();

}


int main_lap(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(laplacian_ex(img_path));
	return 0;
}