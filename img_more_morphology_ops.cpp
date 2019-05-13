#include "cv_helper.h"

static int op_type = 0;
static int structing_element = 0;
static int kernel = 0;
static int max_op_type = 4;
static int max_structing_element = 2;
static int max_kernel = 21;
static cv::Mat img;

#define DEMO_WND "morphology demo"
static void morphologyOpsCallback(int, void*)
{
	cv::Mat filter = cv::getStructuringElement(structing_element, cv::Size(2*kernel+1,2*kernel+1));
	static cv::Mat out;
	cv::morphologyEx(img, out, op_type + 2, filter);
	cv::imshow(DEMO_WND, out);
}

static void more_morphology_ops(const cv::String &img_path)
{
	img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());

	auto func_create_track_bar = [](const std::string &bar_name, const std::string &wnd_name,
		int *morph_type, int max_ticks, cv_helper::bar_func pfunc)
	{
		cv::createTrackbar(bar_name, wnd_name, morph_type, max_ticks, pfunc);
	};

	cv::namedWindow(DEMO_WND, cv::WINDOW_AUTOSIZE);
	func_create_track_bar("op types:", DEMO_WND, &op_type, max_op_type, morphologyOpsCallback);
	func_create_track_bar("structing element type:",DEMO_WND,&structing_element,max_structing_element,morphologyOpsCallback);
	func_create_track_bar("kernel:",DEMO_WND,&kernel,max_kernel,morphologyOpsCallback);

	cv::imshow(DEMO_WND, img);
	morphologyOpsCallback(0, 0);
	cv::waitKey();

}

int main_more_morphology_ops(int argc, char **argv)
{
	char *param = (argc == 2) ? argv[1] : "81";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(param,img_path));
	CV_TRY_CATCH(more_morphology_ops(img_path));
	return 0;
}