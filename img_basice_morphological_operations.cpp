#include "cv_helper.h"
#include <functional>

void func_morphological_operations()
{
	int erosion_size = 2;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1));


	PRINTMAT_F(element, NPF);
}

#define ER_WND "Erosion Demo"
#define DI_WND "Dilation Demo"



static int e_morph_type = 0, d_morph_type = 0;
static int max_ticks = 2;
static int e_kernel = 0, d_kernel = 0;
static int max_kernel = 21;
static cv::Mat img;


void morphOP_Erosion(int , void* )
{
	int type = 0;
	
	COND_BEGIN_ONCE
	COND_PRED_BREAK(e_morph_type ==0,type=cv::MORPH_RECT);
	COND_PRED_BREAK(e_morph_type ==1,type=cv::MORPH_CROSS);
	COND_PRED_BREAK(e_morph_type ==2, type = cv::MORPH_ELLIPSE);
	COND_END_ONCE

	cv::Mat filter = cv::getStructuringElement(type, cv::Size(2 * e_kernel + 1, 2 * e_kernel + 1),cv::Point(e_kernel,e_kernel));
	cv::Mat out;
	cv::dilate(img, out, filter);
	cv::imshow(ER_WND, out);

}


void morphOP_Dilation(int , void* )
{
	int type = 0;

	COND_BEGIN_ONCE
		COND_PRED_BREAK(d_morph_type == 0, type = cv::MORPH_RECT);
		COND_PRED_BREAK(d_morph_type == 1, type = cv::MORPH_CROSS);
		COND_PRED_BREAK(d_morph_type == 2, type = cv::MORPH_ELLIPSE);
	COND_END_ONCE

	cv::Mat filter = cv::getStructuringElement(type, cv::Size(2 * d_kernel + 1, 2 * d_kernel + 1),cv::Point(d_kernel,d_kernel));
	cv::Mat out;


	cv::erode(img, out, filter);
	cv::imshow(DI_WND, out);
}


void func_dilate_and_erode(const cv::String &img_path)
{
	img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());

	cv::namedWindow(ER_WND, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(DI_WND, cv::WINDOW_AUTOSIZE);
	cv::moveWindow(DI_WND, img.cols,0);


	auto func_create_track_bar = [](const std::string &bar_name,const std::string &wnd_name,
		int *morph_type,int max_ticks,cv_helper::bar_func pfunc)
	{
		cv::createTrackbar(bar_name, wnd_name, morph_type, max_ticks, pfunc);
	};



	func_create_track_bar("Erosion Type:\n 0:Rect \n 1: Cross \n 2: Ellipse", ER_WND, &e_morph_type, max_ticks, morphOP_Erosion);
	func_create_track_bar("Kernel Size:\n 2n+1",ER_WND,&e_kernel,max_kernel,morphOP_Erosion);

	func_create_track_bar("Dilation Type:\n 0:Rect \n 1: Cross \n 2: Ellipse", DI_WND, &d_morph_type, max_ticks, morphOP_Dilation);
	func_create_track_bar("Kernel Size:\n 2n+1", DI_WND, &d_kernel, max_kernel, morphOP_Dilation);
	
	morphOP_Erosion(0, 0);
	morphOP_Dilation(0,0);
	cv::waitKey();
	
}


int main_erosion_dilation(int argc,char **argv)
{
	//test
	//CV_TRY_CATCH(func_morphological_operations());
	
	char *pstr = (argc == 2) ? argv[1] : "81";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(pstr,img_path));
	CV_TRY_CATCH(func_dilate_and_erode(img_path));
	
	return 0;
}