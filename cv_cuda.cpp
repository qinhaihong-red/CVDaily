#include "cv_helper.h"
#include <opencv2/core/cuda.hpp>
#include <Eigen/Dense>
#include <math.h>
#include <algorithm>
using Eigen::MatrixXd;

using std::max;
using std::min;


static void ex()
{
	std::cout << cv::cuda::getCudaEnabledDeviceCount() << std::endl;;
	cv::cuda::DeviceInfo dev;
	std::cout << dev.name() << std::endl;


	MatrixXd m(2, 2);
	m(0, 0) = 3;
	m(1, 0) = 2.5;
	m(0, 1) = -1;
	m(1, 1) = m(1, 0) + m(0, 1);
	std::cout << m << std::endl;

	int c = ::max(1, 2);
	::asinf(10);
}

int main_cuda()
{
	CV_TRY_CATCH(ex());
	return 0;
}