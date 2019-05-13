#include "cv_helper.h"

void func_separable_kernels()
{
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx("lena", img_path));
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());

	cv::namedWindow(SRC_WIND, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(DST_WIND, cv::WINDOW_AUTOSIZE);

	cv::Mat dst;
	auto func_apply_kernel = [&](const cv::Mat &kernel,const std::string &kernel_name)
	{
		cv::filter2D(img, dst, img.depth(), kernel);

		cv::putText(dst, kernel_name, cv::Point(cvRound(0.75*dst.cols), cvRound(0.75*dst.rows)), cv::FONT_HERSHEY_COMPLEX,
			0.5, cv::Scalar(0, 0, 0));

		cv::imshow(SRC_WIND, img);
		cv::imshow(DST_WIND, dst);
		cv::waitKey();
	};

	cv::Mat kernel_v =(cv::Mat_<double>(3, 1) << 1, 2, 1);
	cv::Mat bilinear_kernel;
	cv::mulTransposed(kernel_v, bilinear_kernel, false);
	PRINTMAT_F(bilinear_kernel, NPF);
	bilinear_kernel = bilinear_kernel / 16.0f;

	func_apply_kernel(bilinear_kernel,"bilinear_kernel");
	cv::Mat sobel_kernel = (cv::Mat_<double>(3, 3) << \
		-1, 0, -1,
		-2, 0, 2,
		-1, 0, 1);
	sobel_kernel = sobel_kernel / 8.0f;
	func_apply_kernel(sobel_kernel,"sobel_kernel");

	kernel_v = (cv::Mat_<double>(5,1)<<1,4,6,4,1);
	cv::Mat gauss_kernel;
	cv::mulTransposed(kernel_v, gauss_kernel, false);
	gauss_kernel /= 256.0f;
	func_apply_kernel(gauss_kernel,"gauss_kernel");

}

int main_separable_kernels()
{
	CV_TRY_CATCH(func_separable_kernels());

	return 0;
}