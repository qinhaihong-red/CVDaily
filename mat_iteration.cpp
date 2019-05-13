#include "cv_helper.h"


static void ex()
{
	uchar x[] = {1,2,3,4};
	uchar y[] = {5,6,7,8};
	cv::Mat mx(2,2,CV_8U,x);
	cv::Mat my(2,2,CV_8U,y);
	PRINTMAT_F(mx,NPF);
	PRINTMAT_F(my, NPF);
	cv::Mat m;
	cv::Mat ms[] = {mx,my};
	cv::merge(ms,2,m);

	for (int i=0;i<m.rows;++i)
	{
		uchar *p = m.ptr<uchar>(i);
		for (int j=0;j<m.cols;++j)
		{
			//this is ok
			auto pixel = m.at<cv::Vec2b>(i,j);
			std::cout << int(pixel[0]) << " " << int(pixel[1]) << /*" " << pixel[2] <<*/ std::endl;

			//this is not correct
			std::cout << int(p[j]) << " " << int(p[j + 1]) << /*" " << int(p[j + 2]) <<*/ std::endl;
		}
	}
}

int main_mat_iteration()
{
	CV_TRY_CATCH(ex());
	return 0;
}