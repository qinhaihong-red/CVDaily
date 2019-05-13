#include "cv_helper.h"
#include <vector>
#include <algorithm>
static cv::Mat img, img_gray;
static int thresh = 20, max_thresh = 100;


static void thresh_callbck(int, void *)
{
	int max_corners = cv::max(1, thresh);
	std::vector<cv::Point> vp;
	cv::goodFeaturesToTrack(img_gray, vp, max_corners, 0.01, 10, cv::Mat(), 3, 3);

	cv::RNG rng(0xFFFF);

	cv::Mat copy = img.clone();
	std::for_each(std::begin(vp), std::end(vp), [&copy,&rng](cv::Point &pt)
	{
		int radius = 4;
		cv::circle(copy, pt, radius, cv::Scalar((int)rng.uniform(0, 255), (int)rng.uniform(0, 255), (int)rng.uniform(0, 255)),-1);
	});

	cv::imshow(DST_WIND, copy);

}


static void shi_tomasi_corner_detect(cv::String &img_path)
{
	img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());

	CV_CVT2GRAY(img, gray);
	img_gray = gray;

	cv::namedWindow(DST_WIND);
	cv::createTrackbar("Thresh:", DST_WIND, &thresh, max_thresh, thresh_callbck);

	thresh_callbck(0, 0);
	cv::imshow("src", img);
	cv::waitKey();
}

int main_shitomasi_cd(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "coco3";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(name, img_path));
	CV_TRY_CATCH(shi_tomasi_corner_detect(img_path));
	return 0;
}