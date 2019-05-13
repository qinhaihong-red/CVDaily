#include <opencv2/core.hpp>
#include <iostream>
#include "cv_helper.h"
using namespace cv;


void func_mat_ex()
{
	//0.regular construction
	cv::Mat M(2,2,CV_8UC3,Scalar(0,0,255));
	PRINTMAT_F(M,Formatter::FMT_NUMPY);
	//size return width and heights,which means cols and rows
	std::cout << M.size() << std::endl;

	int sz[3] = {2,2,2};
	try
	{
		//3 batch,2 channel and 2 x 2
		Mat M2(3, sz, CV_8UC(1), Scalar::all(0));
		//std::cout not good support for multi-dim MAT
		//PRINTMAT_F(M2,NPF);
		std::cout << "M2 size is:"<<M2.size() << std::endl;
		M2.create(2, 3, CV_8UC2);
		PRINTMAT_F(M2,NPF);
	}
	catch (const cv::Exception& e)
	{
		EXCEPTION_INFO(e.what());
	}

	//1.eyes,ones,zeros
	Mat E = Mat::eye(4, 4, CV_64F);
	PRINT_SZ(E);
	Mat E2 = Mat::eye(4, 4, CV_64FC1);
	PRINT_SZ(E2);

	Mat O = Mat::ones(2, 2, CV_32F);
	PRINTMAT_F(O,NPF);
	Mat Z = Mat::zeros(2, 3, CV_64F);
	PRINTMAT_F(Z,NPF);

	//2.construct from array and reshape
	Mat M3 = (Mat_<double>({ 2,3,4,5,6,7 })).reshape(1, std::vector<int>({2,3}));
	PRINTMAT_F(M3,NPF);

	M3 = M3.reshape(1, std::vector<int>({ 3,2 }));
	PRINTMAT_F(M3, NPF);


	Mat2b m2b (1,2,3);
	PRINTMAT_F(m2b,NPF);
	//SVD

}

void func_mat_ex2()
{
	int sz[] = {3,2,2};
	Mat M(3,sz,CV_8UC1,Scalar::all(0));

	Mat R(2,3,CV_8UC3);
	randu(R, Scalar::all(0), Scalar::all(255));
	PRINTMAT_F(R,NPF);

	Point2d p2(1, 2);
	std::cout << p2 << std::endl;

	Point3f p3(2,3,4);
	std::cout << p3 << std::endl;

	std::vector<float> v;
	v.push_back((float)CV_PI);
	v.push_back(2);
	v.push_back(3.01f);
	std::cout << "vector via matrix:\n" ;
	PRINTMAT_F(Mat(v), NPF);


}

int main()
{
	//func_mat_ex();
	func_mat_ex2();

	return 0;
}