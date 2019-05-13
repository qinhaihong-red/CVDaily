#include "cv_helper.h"
#include <vector>

static cv::Mat img;
static int thresh_val = 100, max_val = 255;
static cv::RNG rng(0xFFFF);
static void threash_callback(int, void *)
{
	cv::Mat edges;
	cv::Canny(img, edges, thresh_val, thresh_val * 2);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);


	std::vector<cv::Moments> vm(contours.size());
	for (int i=0;i<contours.size();++i)
	{
		vm[i] = cv::moments(contours[i]);
	}

	std::vector<cv::Point2f> vp(contours.size());
	for (int i=0;i<contours.size();++i)
	{
		vp[i] = cv::Point2f(float(vm[i].m10/ (vm[i].m00+1e-5)),float(vm[i].m01/ (vm[i].m00 + 1e-5)));
	}

	cv::Mat dst = cv::Mat::zeros(edges.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); ++i)
	{
		auto color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		cv::drawContours(dst, contours, i, color);

		cv::circle(dst,vp[i], 4, color, 2);

	}

	cv::imshow(DST_WIND, dst);

}


static void image_moments(const cv::String &img_path)
{
	img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img.empty());
	cv::blur(img, img, cv::Size(3, 3));
	cv::namedWindow(DST_WIND);
	cv::createTrackbar("canny thresh:", DST_WIND, &thresh_val, max_val, threash_callback);

	cv::imshow("src", img);
	threash_callback(0, 0);

	cv::waitKey();

}


int main_moments(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(image_moments(img_path));
	return 0;
}