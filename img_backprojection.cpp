#include "cv_helper.h"
#include <vector>

static cv::Mat hue;
static int bins_val = 25;
static void bins_val_callback(int, void*)
{
	int histSz = cv::max(2, bins_val);
	float hue_range[] = {0,180};
	const float *histRange[] = {hue_range};
	cv::Mat hist;
	cv::calcHist(&hue, 1, 0, cv::Mat(), hist, 1,&histSz, histRange);
	cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);
	cv::Mat backproj;
	cv::calcBackProject(&hue, 1, 0, hist, backproj, histRange);
	cv::imshow("backprojection", backproj);
}

static void backprojection(const cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());

	cv::Mat hsv;
	cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
	hue.create(hsv.size(), hsv.depth());
	int ch[] = {0,0};
	cv::mixChannels(&hsv, 1, &hue, 1, ch, 1);
	cv::namedWindow("backprojection", cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("bin_val", "backprojection", &bins_val, 180, bins_val_callback);
	cv::imshow("src", img);
	bins_val_callback(0, 0);
	cv::waitKey();
}

static void dims_test()
{
	cv::Mat m = cv::Mat::ones(2, 3, CV_8UC3);
	std::cout << m.dims << std::endl;//2
	std::cout << m.channels() << std::endl;//3
	std::cout << m.type() << std::endl ;
	PRINTMAT_F(m,NPF);

	//using depth as second param to initialize mat makes it to be a single channel matrix
	cv::Mat m2(m.size(),m.depth());
	std::cout << m2.dims << std::endl;//2
	std::cout << m2.channels() << std::endl;//1
	std::cout << m2.type() << std::endl;
	PRINTMAT_F(m2, NPF);

}

int main_backprojection(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(backprojection(img_path));
	return 0;
}