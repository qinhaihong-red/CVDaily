#include "cv_helper.h"

static void hough_line_transform(const cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img.empty());
	cv::Mat edges;cv::Canny(img, edges, 50, 200);
	cv::Mat dst; cv::cvtColor(img,dst, cv::COLOR_GRAY2BGR);
	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50, 10);
	std::for_each(std::begin(lines), std::end(lines), [&dst] (const cv::Vec4i &v)
	{
		cv::line(dst, cv::Point(v[0], v[1]), cv::Point(v[2], v[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
	});

	cv::Point ptx(10,10);
	cv::Point pty(10 + 100, 10 + 50);
	cv::rectangle(img, ptx, pty, cv::Scalar(0,255,0),2);

	cv::imshow("src", img);
	cv::imshow("hough line transform", dst);
	cv::waitKey();
}

int main_hough_line_transform(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(hough_line_transform(img_path));
	return 0;
}