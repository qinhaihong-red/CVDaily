#include "cv_helper.h"
#include <fstream>

static void show_wait_destroy(const std::string &name,cv::Mat &img)
{
	cv::imshow(name, img);
	cv::moveWindow(name, 500, 0);
	cv::waitKey();
	cv::destroyWindow(name);

}

static void extract_horizontal_vertical_lines_with_morphological_ops()
{
	std::ofstream of("bin_image.txt");
	CerrRdWrapper wrapper(of);

	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx("sheet_music",img_path));
	cv::Mat img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img.empty());
	show_wait_destroy("original", img);
	cv::Mat img_bw_not;
	cv::adaptiveThreshold(~img, img_bw_not,255 ,cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, -2);
	show_wait_destroy("bitwise-not", img_bw_not);
	//PRINTMAT_F(img_bw_not,NPF);

	cv::Mat h = img_bw_not.clone();
	cv::Mat v = img_bw_not.clone();

	int h_size = h.cols / 30;
	cv::Mat hKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(h_size, 1));
	cv::erode(h, h, hKernel);
	cv::dilate(h, h, hKernel);
	show_wait_destroy("horizontal", h);

	int v_size = v.rows / 30;
	cv::Mat vKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1,v_size));
	cv::erode(v, v, vKernel);
	cv::dilate(v,v,vKernel);
	show_wait_destroy("vertical", v);
	cv::bitwise_not(v, v);
	show_wait_destroy("vertical bitwise-not", v);

	//this is mask
	cv::Mat edges;
	cv::adaptiveThreshold(v,edges,255,cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY,3,-2);
	show_wait_destroy("edges", edges);

	cv::Mat kernel = cv::Mat::ones(2, 2, CV_8U);
	cv::dilate(edges, edges, kernel);
	show_wait_destroy("dilate",edges);

	cv::Mat smooth;
	v.copyTo(smooth);

	cv::blur(smooth, smooth, cv::Size(2, 2));
	smooth.copyTo(v, edges);

	show_wait_destroy("final", v);
}

int main_extract_lines(int argc, char **argv)
{
	CV_TRY_CATCH(extract_horizontal_vertical_lines_with_morphological_ops());
	return 0;
}