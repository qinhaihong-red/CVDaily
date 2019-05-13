#include "cv_helper.h"
#include <algorithm>

void func_img_blending()
{
	cv::String img_path1, img_path2;
	if (cv_helper::get_imgPathEx("L2.png",img_path1) && cv_helper::get_imgPathEx("L3.png",img_path2))
	{
		try
		{
			cv::Mat img1 = cv::imread(img_path1, cv::IMREAD_COLOR);
			cv::Mat img2 = cv::imread(img_path2, cv::IMREAD_COLOR);

			int rows = std::min(img1.rows, img2.rows);
			int cols = std::min(img1.cols, img2.cols);

			cv::Rect roi(0, 0, cols, rows);
			cv::Mat sub_img1 = img1(roi), sub_img2 = img2(roi);
			double alpha = 0.4, beta = 0, gamma = 0;

			cv::Mat out;
			beta = 1 - alpha;

			cv::addWeighted(sub_img1, alpha, sub_img2, beta, gamma, out);

			cv::namedWindow("dst_wind",cv::WINDOW_AUTOSIZE);
			cv::imshow("dst_wind", out);
			cv::waitKey();
		}
		catch (const cv::Exception& e)
		{
			EXCEPTION_INFO(e.what());
			_EXIT_FAILURE;
		}
	}
	else
	{
		_EXIT_FAILURE;
	}
}

int main_img_blending()
{
	func_img_blending();

	return 0;
}