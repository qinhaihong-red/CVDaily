#include "cv_helper.h"

static int type = 0;
static int max_type = 4;
static int threshold_val = 0;
static int max_threshold_val = 255;
static int max_bin_val = 255;

static cv::Mat img;
static const char *wnd_name = "threshold demo";

static void threshold_callback(int, void *)
{
	cv::Mat dst;
	cv::threshold(img, dst, threshold_val, max_bin_val, type);
	cv::imshow(wnd_name, dst);
	//cv::THRESH_BINARY == 0
}

static void threshold_ex(const cv::String &img_path)
{
	img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img.empty());
	cv::namedWindow(wnd_name, cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("threshold type:", wnd_name, &type, max_type, threshold_callback);
	cv::createTrackbar("threshold value:", wnd_name, &threshold_val, max_threshold_val, threshold_callback);
	cv::waitKey();
}

int main_threshold(int argc, char **argv)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(name,img_path));
	CV_TRY_CATCH(threshold_ex(img_path));
	return 0;
}