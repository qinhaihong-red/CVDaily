#include "cv_helper.h"

static void draw_line()
{
	cv::Mat img = cv::Mat::zeros(100, 100, CV_8U);
	std::cout << std::boolalpha << (CV_8UC1 == CV_8U) << std::endl;
	img.at<uchar>(0, 0) = 255;
	cv::Vec<uchar,1> pixel = img.at<uchar>(0, 0);
	//cv::Scalar
	int p = pixel.val[0];
	std::cout <<"pixel is:" << p<< std::endl;
	printf("%d", pixel.val[0]);
	cv::Point px(10, 10);
	cv::Point py(50, 10);
	//cv::Mat dst;
	cv::line(img, px, py, cv::Scalar(255), 5);
	cv::imshow("line demo", img);
	
	cv::waitKey();
}

int main_line()
{
	CV_TRY_CATCH(draw_line());
	return 0;
}