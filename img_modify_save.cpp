#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>

void func_img_modify_save()
{
	std::string img_folder(getenv("IMG_FOLDER"));
	if (img_folder.empty())
	{
		std::cout << "folder not exists.\n";
		exit(EXIT_FAILURE);
	}


	int sz[] = {2,3,1};
	cv::Mat _m(3,sz,CV_8UC1,cv::Scalar::all(0));
	std::cout << _m << std::endl;


	cv::String img_path = img_folder + "L3.png";

	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	cv::Mat img_gray;
	cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);


	cv::namedWindow("source", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("modified", cv::WINDOW_AUTOSIZE);

	cv::imshow("source", img);
	cv::imshow("modified",img_gray);

	cv::waitKey(0);
}

int main_img_modify_save()
{
	func_img_modify_save();
	return 0;
}

