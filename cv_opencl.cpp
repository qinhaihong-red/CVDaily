#include "cv_helper.h"
#include <opencv2/core/ocl.hpp>


static void test(cv::InputArray _src, cv::OutputArray _dst)
{
	std::cout << std::boolalpha << _src.isUMat() << std::endl;
	_dst.create(_src.size(),CV_8U);
}

static void ex()
{
	std::cout << std::boolalpha << cv::ocl::haveOpenCL() << std::endl;
	std::cout << std::boolalpha << cv::ocl::useOpenCL() << std::endl;

	cv::ocl::Context ctx;
	cv::ocl::Device dev;
	bool b = ctx.create(cv::ocl::Device::TYPE_ALL);
	if (b)
	{
		for (int i=0;i<ctx.ndevices();++i)
		{
			dev = ctx.device(i);
			std::cout << std::string(dev.name()) << std::endl;
		}
	}
	else
	{
		std::cout << "something wrong!" << std::endl;
	}

	cv::Mat m = cv::Mat::ones(2, 2, CV_8U);
	cv::Mat m2;
	test(m, m2);
}


int main_ocl()
{
	CV_TRY_CATCH(ex());
	return 0;
}
