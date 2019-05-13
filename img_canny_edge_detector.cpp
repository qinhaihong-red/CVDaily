#include "cv_helper.h"
#include <fstream>
static cv::Mat src, dst, gray,edges;
static int threshold_val = 0, threshold_max = 100;
static int kernel_sz = 3,ratio=3;


static void trackbar_callback(int, void*)
{
	cv::blur(gray, edges, cv::Size(3, 3));
	cv::imshow("gray after blur", edges);
	cv::Canny(edges, edges, threshold_val, threshold_val*ratio, kernel_sz);

	//if (threshold_val>50)
	//{
	//	std::ofstream of("edges.txt");
	//	CerrRdWrapper wrapper(of);
	//	PRINTMAT_F(edges, NPF)
	//}

	cv::imshow("edges", edges);
	dst = cv::Scalar::all(0);
	src.copyTo(dst, edges);
	cv::imshow("dst", dst);
}

static void canny_ex(const cv::String &img_path)
{
	src = cv::imread(img_path,cv::IMREAD_COLOR);
	CV_Assert(!src.empty());
	dst.create(src.size(),src.type());
	cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
	cv::namedWindow("canny demo", cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("threashold:", "canny demo", &threshold_val, threshold_max, trackbar_callback);
	cv::imshow("canny demo", src);
	trackbar_callback(0, 0);
	cv::waitKey();
	cv::Mat out(edges.size(),CV_8UC3,cv::Scalar(0,0,255));
	edges.copyTo(out, edges);
	cv::imwrite("thz.jpg", out);
}

int main_canny(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(canny_ex(img_path));
	return 0;
}