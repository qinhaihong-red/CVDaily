#include "cv_helper.h"
#include <float.h>

void fun_cv_rng_ex()
{
	cv::RNG rng(0xffffffff);
	std::cout << int(uchar(rng))<< std::endl;
	std::cout << int(schar(rng)) << std::endl;

	std::cout << int(rng) << std::endl;
	std::cout << float(rng) << std::endl;
	std::cout << double(rng) << std::endl;

	std::cout << rng()<< std::endl;

	cv::Mat M1(2,3,CV_32FC1);
	rng.fill(M1, cv::RNG::UNIFORM, 0.f, 0.5f);
	PRINTMAT_F(M1,NPF);
	rng.fill(M1,cv::RNG::NORMAL,0,1);
	PRINTMAT_F(M1,NPF);
}



int main_rng()
{
	//test ok
	CV_TRY_CATCH(fun_cv_rng_ex());
	
	return 0;
}