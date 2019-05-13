#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdlib.h>
#include <iostream>

int main_img_read()
{
	char *p = NULL;
	p = getenv("IMG_FOLDER");
	if (!p)
	{
		exit(EXIT_FAILURE);
	}
	std::string img_folder(p);
	cv::String img_path = img_folder+"L3.png";

	std::cout << "image path is:"<<img_path.c_str() << std::endl;

	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	printf("img size[0]:%d,size[1]:%d,size[2]:%d\n",img.size[0],img.size[1],img.size[2]);
	std::cout << "img channels:" << img.channels() << std::endl;
	std::cout << "img size:" << img.size << std::endl;
	std::cout << "img size():" << img.size() << std::endl;

	std::cout << "img rows and cols:" << img.rows << "," << img.cols << std::endl;

	//std::cout << cv::format(img, cv::Formatter::FMT_NUMPY) << std::endl;


	std::cout << "\n\n";
	int sz[] = {12,3,4};
	cv::Mat M2(3,sz,CV_8UC3,cv::Scalar::all(0));

	printf("img size[0]:%d,size[1]:%d,size[2]:%d\n", M2.size[0], M2.size[1], M2.size[2]);
	std::cout << "M2 channels:" << M2.channels() << std::endl;
	std::cout << "M2 size:" << M2.size << std::endl;
	std::cout << "M2 size():" << M2.size() << std::endl;
	std::cout << "M2 dims:" << M2.dims << std::endl;
	std::cout << "M2 rows and cols:" << M2.rows << "," << M2.cols << std::endl;

	//std::cout << cv::format(M2, cv::Formatter::FMT_NUMPY) << std::endl;

	//cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Display Window", img);
	//cv::waitKey(0);

	return 0;
}