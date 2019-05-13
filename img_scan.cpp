#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "cv_helper.h"



cv::Mat& scanImage_and_reduce_with_c_pointer(cv::Mat &M, const uchar *ptable)
{
	CV_Assert(M.depth() == CV_8U);

	int nc = M.channels();

	int nRows = M.rows;
	int nCols = M.cols * nc;

	if (M.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	uchar *p;
	for (int i=0;i<nRows;++i)
	{
		p = M.ptr<uchar>(i);
		for (int j=0;j<nCols;++j)
		{
			p[j] = ptable[p[j]];
		}
	}

	return M;
}

cv::Mat& scanImage_and_reduce_with_iterator(cv::Mat& M, const uchar *ptable)
{
	CV_Assert(M.depth() == CV_8U);

	int nc = M.channels();

	if (nc==1)
	{
		for (auto iter=M.begin<uchar>();iter!=M.end<uchar>();++iter)
		{
			*iter = ptable[*iter];
		}
	}
	else if (nc==3)
	{
		for (auto iter=M.begin<cv::Vec3b>();iter!=M.end<cv::Vec3b>();++iter)
		{
			(*iter)[0] = ptable[(*iter)[0]];
			(*iter)[1] = ptable[(*iter)[1]];
			(*iter)[2] = ptable[(*iter)[2]];

		}
	}
	else
	{
		std::cout << "not support\n";
	}

	return M;
}


void scanImage_and_reduce_with_LUT(cv::Mat &IM,const uchar *ptable, cv::Mat &OM)
{
	cv::Mat table(1,256,CV_8U);
	uchar *p = table.ptr();

	for (int i=0;i<256;++i)
	{
		p[i] = ptable[i];
	}

	cv::LUT(IM, table,OM);

}

void func_img_scan()
{
	//0.lookup table
	int width = 10;
	uchar table[256];
	for (int i=0;i<sizeof(table);++i)
	{
		table[i] = uchar((i / width)*width);
	}

	std::string img_folder(getenv("IMG_FOLDER"));
	if (img_folder.empty())
	{
		exit(EXIT_FAILURE);
	}

	cv::String img_path = img_folder + "L3.png";
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);

	std::cout.precision(8);
	std::cout << std::fixed;

	CV_TimeSpan ts;
	scanImage_and_reduce_with_c_pointer(img.clone(), table);
	std::cout << "scanImage_and_reduce_with_c_pointer time span:" << ts.stop() << " s.\n";

	ts.start();
	scanImage_and_reduce_with_iterator(img.clone(), table);
	std::cout << "scanImage_and_reduce_with_iterator time span:" << ts.stop() << " s.\n";

	ts.start();
	cv::Mat OM;
	scanImage_and_reduce_with_LUT(img.clone(),table,OM);
	std::cout << "scanImage_and_reduce_with_LUT time span:" << ts.stop() << " s.\n";
}


int main_scan()
{
	func_img_scan();

	return 0;
}