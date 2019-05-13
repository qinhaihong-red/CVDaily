#include "cv_helper.h"
#include <vector>
#include <algorithm>
static void hough_circle_transform(const cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());
	//cv::Mat gray; cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
	CV_CVT2GRAY(img,gray);
	cv::medianBlur(gray, gray, 5);
	std::vector<cv::Vec3f> vec;
	cv::HoughCircles(gray, vec, cv::HOUGH_GRADIENT, 1, gray.rows/2, 100, 30, 1, 30);
	std::for_each(std::begin(vec), std::end(vec), [&img](const cv::Vec3f &v3) 
	{
		cv::Point center(v3[0],v3[1]);
		cv::circle(img, center, 1, cv::Scalar(0, 255, 0),3,cv::LINE_AA);
		cv::circle(img, center, v3[2], cv::Scalar(255, 0, 0),3,cv::LINE_AA);
	});
	std::cout << "circles found:" << vec.size() << std::endl;

	cv::imshow("hough circle transform", img);
	cv::waitKey();

}

int main_hough_circle_transform(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "dog";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(hough_circle_transform(img_path));
	return 0;
}