#include "cv_helper.h"

static void img_pyramids(const cv::String& path_name)
{
	cv::Mat img = cv::imread(path_name, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());
	const char* wnd_name = "pyramids demo";
	cv::namedWindow(wnd_name, cv::WINDOW_AUTOSIZE);
	while (true)
	{
		cv::imshow(wnd_name,img);
		char key = (char)cv::waitKey();

		if (key == 27 ) break;
		if (key == 'i') cv::pyrUp(img, img/*, cv::Size(img.cols * 2, img.rows * 2)*/);
		if (key == 'o') cv::pyrDown(img, img/*, cv::Size(img.cols / 2, img.rows / 2)*/);
	}
}

int main_pyramids(int argc, char **argv)
{
	const char *name = (argc == 2) ? argv[1] : "lena";
	cv::String path_name;
	CV_Assert(cv_helper::get_imgPathEx(name, path_name));
	CV_TRY_CATCH(img_pyramids(path_name));

	return 0;
}