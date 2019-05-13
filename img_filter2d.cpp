#include "cv_helper.h"

static void filter2d_ex(const cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path);
	cv::Mat dst;
	int index = 0;
	while (true)
	{
		int kernel_sz = 3 + (index % 5);
		cv::Mat kernel = cv::Mat::ones(kernel_sz, kernel_sz, CV_32F) / float(kernel_sz*kernel_sz);
		cv::filter2D(img, dst, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
		cv::imshow("filter2d", dst);
		char key = (char)cv::waitKey(500);
		if (key == 27)break;
		++index;
	}
}

int main_filer2d(int argc, char **argv)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(name, img_path));
	CV_TRY_CATCH(filter2d_ex(img_path));
	return 0;
}