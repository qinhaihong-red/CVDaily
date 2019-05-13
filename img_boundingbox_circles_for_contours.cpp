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

	decltype(contours) contours_poly(contours.size());
	std::vector<cv::Rect> rects(contours.size());
	std::vector<cv::Point2f> pts(contours.size());
	std::vector<float> radius(contours.size());

	for (int i=0;i<contours.size();++i)
	{
		cv::approxPolyDP(contours[i], contours_poly[i], 3, true);
		rects[i] = cv::boundingRect(contours_poly[i]);
		cv::minEnclosingCircle(contours_poly[i], pts[i], radius[i]);
	}

	cv::Mat dst = cv::Mat::zeros(edges.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); ++i)
	{
		auto color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		cv::drawContours(dst, contours_poly, i, color);
		cv::rectangle(dst, rects[i], color, 2);
		cv::circle(dst, pts[i], radius[i], color, 2);

	}

	cv::imshow(DST_WIND, dst);

}


static void draw_boundingbox_and_circles_for_contours(const cv::String &img_path)
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


int main_boundingbox_and_circles(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(draw_boundingbox_and_circles_for_contours(img_path));
	return 0;
}