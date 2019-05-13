#include "cv_helper.h"

void func_img_access()
{
	cv::String img_path;
	bool b = cv_helper::get_imgPathEx("jordan.jpg", img_path);
	if (!b) exit(EXIT_FAILURE);

	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	std::cout << "the pixel coordinate(0,0) is:\n";
	
	//the Scalar_ is a 4-elements vector,
	//when Mat addressing with it,will always move additional element
	cv::Scalar_<uchar> pixel = img.at<cv::Scalar_<uchar>>(0,0);
	printf("BGR-pulus1(%d,%d,%d,%d)\n", pixel[0], pixel[1], pixel[2],pixel[3]);

	//the Vec3b is a 3-elements vector,
	//which matches the 3-channels img.
	cv::Vec3b pixel2 = img.at<cv::Vec3b>(0,0);
	printf("BGR(%d,%d,%d)\n", pixel2.val[0], pixel2.val[1], pixel2.val[2]);


	std::cout<< "the pixel coordinate(50,100) is(which is .at(100,50)):\n";
	//the addressing of Mat's .at() method,begins with 0-based rows and then 0-based columns.
	pixel = img.at<cv::Scalar_<uchar>>(100, 50);
	printf("BGR-pulus1(%d,%d,%d,%d)\n", pixel[0], pixel[1], pixel[2], pixel[3]);

	img.at<cv::Vec3b>(100, 50);
	printf("BGR(%d,%d,%d)\n", pixel2.val[0], pixel2.val[1], pixel2.val[2]);

	std::vector<cv::Point3i> vec;
	vec.push_back(cv::Point3i(1,2,3));
	vec.push_back(cv::Point3i(2,3,4));

	cv::Mat M2=cv::Mat(vec).reshape(1);
	PRINTMAT_F(M2, NPF);

	
}

int main_access()
{
	func_img_access();

	return 0;
}