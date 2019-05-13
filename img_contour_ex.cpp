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

	cv::Mat dst = cv::Mat::zeros(edges.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); ++i)
	{
		auto color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		cv::drawContours(dst, contours, i, color);
	}
	std::cout << "contour sz:" << contours.size() << std::endl;
	cv::Mat dst2 = cv::Mat::zeros(edges.size(), CV_8UC3);
	for (int i=0;i<contours.size();++i)
	{
		auto color = cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		for (int j=0;j<contours[i].size();)
		{
			if (j+1<contours[i].size())
			{
				cv::line(dst2, contours[i][j++], contours[i][j++],color);
			}
			else
			{
				cv::line(dst2, contours[i][j], contours[i][j++], color);
			}
			
		}
	}

	cv::imshow("edges", edges);
	cv::imshow(DST_WIND, dst);
	cv::imshow("dst2", dst2);

}


static void find_coutour(const cv::String &img_path)
{
	img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img.empty());
	cv::blur(img, img, cv::Size(3,3));
	cv::namedWindow(DST_WIND);
	cv::createTrackbar("canny thresh:", DST_WIND, &thresh_val, max_val, threash_callback);

	cv::imshow("src", img);
	threash_callback(0, 0);

	cv::waitKey();

}


int main_contour_ex(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(find_coutour(img_path));
	return 0;
}