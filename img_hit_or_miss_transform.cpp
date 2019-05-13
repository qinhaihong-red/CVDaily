#include "cv_helper.h"
#include <opencv2/imgproc.hpp>

static void hit_miss()
{
	cv::Mat input = (cv::Mat_<uchar>(8, 8) <<
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 255, 255, 255, 0, 0, 0, 255,
		0, 255, 255, 255, 0, 0, 0, 0,
		0, 255, 255, 255, 0, 255, 0, 0,
		0, 0, 255, 0, 0, 0, 0, 0,
		0, 0, 255, 0, 0, 255, 255, 0,
		0, 255, 0, 255, 0, 0, 255, 0,
		0, 255, 255, 255, 0, 0, 0, 0);

	cv::Mat kernel = (cv::Mat_<int>(3,3) <<
		0, 1, 0,
		1,-1, 1,
		0, 1, 0);

	cv::Mat out;
	cv::morphologyEx(input, out, cv::MORPH_HITMISS, kernel);

	int rate = 50;
	kernel = (kernel + 1) * 127;
	kernel.convertTo(kernel, CV_8U);
	cv::resize(kernel, kernel,cv::Size() ,rate, rate, cv::INTER_NEAREST);
	cv::imshow("kernel", kernel);
	cv::moveWindow("kernel", 0, 0);

	cv::resize(input, input, cv::Size(), rate, rate, cv::INTER_NEAREST);
	cv::imshow("input", input);
	cv::moveWindow("input", 0, 200);

	cv::resize(out, out, cv::Size(), rate, rate, cv::INTER_NEAREST);
	cv::imshow("output", out);
	cv::moveWindow("output", 500, 200);

	cv::waitKey();

	
}

int main_hit_miss()
{
	CV_TRY_CATCH(hit_miss());
	return 0;
}