#include "cv_helper.h"

static cv::Mat src,patch,dst;
static int method_type = 2;
static int max_type = 5;
static const char *dst_wnd = "dst";

static void method_callback(int ,void *)
{
	int rows = src.rows - patch.rows + 1;
	int cols = src.cols - patch.cols + 1;
	dst.create(rows,cols,CV_32FC1);
	std::cout << src.size << std::endl;
	std::cout << patch.size << std::endl;
	std::cout << dst.size << std::endl;


	cv::matchTemplate(src, patch, dst, method_type);
	cv::normalize(dst, dst, 0, 1, cv::NORM_MINMAX);
	double min_val = 0, max_val = 0;
	cv::Point minLoc, maxLoc, matchLoc;
	cv::minMaxLoc(dst, &min_val, &max_val, &minLoc, &maxLoc);

	matchLoc = maxLoc;
	COND_PRED((method_type== cv::TM_SQDIFF)||(method_type==cv::TM_SQDIFF_NORMED),matchLoc=minLoc);

	cv::Mat display = src.clone();
	cv::Mat dst2;
	cv::convertScaleAbs(dst, dst2, 255);
	cv::rectangle(dst2, matchLoc, cv::Point(matchLoc.x + patch.cols, matchLoc.y + patch.rows), cv::Scalar(0, 0, 0), 2);
	cv::rectangle(display,matchLoc, cv::Point(matchLoc.x + patch.cols, matchLoc.y + patch.rows), cv::Scalar(0, 0, 0), 2);
	
	
	cv::imshow("src", display);
	cv::imshow(dst_wnd, dst2);
}

static void match_template()
{
	cv::String path_img, path_patch;
	CV_Assert(cv_helper::get_imgPathEx("dog", path_img));
	CV_Assert(cv_helper::get_imgPath("dog_eye", path_patch));
	src = cv::imread(path_img);
	CV_Assert(!src.empty());
	patch = cv::imread(path_patch);
	CV_Assert(!patch.empty());

	cv::namedWindow(dst_wnd);
	cv::createTrackbar("method", dst_wnd, &method_type, max_type, method_callback);
	method_callback(0, 0);

	cv::waitKey();

}


int main_tm(_MAIN_ARGS)
{

	CV_TRY_CATCH(match_template());
	return 0;
}