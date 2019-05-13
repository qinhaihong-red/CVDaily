#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <iostream>

void func_index_slice()
{
	std::string img_folder(getenv("IMG_FOLDER"));
	if (img_folder.empty())
	{
		std::cout << "folder not exists.\n";
		exit(EXIT_FAILURE);
	}

	cv::String img_path = img_folder + "L3.png";
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	cv::imwrite("./original.png", img);

	std::cout << "rows and cols:" << img.rows << ", " << img.cols << std::endl;

	//cv::Rect roi(img.rows -1- 100,img.cols-1-100, 100-1,100-1);

	int left = img.cols - 100 ;
	int top = img.rows - 100 ;
	
	std::cout << "left,top is:" << left << "," << top << std::endl;
	
	/*
	roi.x + roi.width <= m.cols  &&
	roi.y + roi.height <= m.rows
	
	宽、高表示从left,top坐标起（包含在内）的长度

	*/


	cv::Rect roi(left,top,100,100);
	printf("roi[%d,%d,%d,%d]",roi.x,roi.y,roi.width,roi.height);


	try
	{
		cv::Mat sub_img = img(cv::Range(0, std::min(100, img.rows)), cv::Range(0, std::min(100, img.cols)));
		cv::imwrite("./leftop_sub_img.png", sub_img);

		cv::Mat sub_img2 = img(roi);
		cv::imwrite("./rightbottom_sub_img.png",sub_img2);

	}
	catch (const cv::Exception& e)
	{
		std::cout << "exception occurs:\n" << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

}


int main_index_slice()
{
	func_index_slice();
	return 0;
}