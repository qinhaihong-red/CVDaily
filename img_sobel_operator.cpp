#include "cv_helper.h"


static void ex()
{
	cv::Mat m = (cv::Mat_<uchar>(3,3)<<1,2,5,
									 4,5,11,
									 7,8,9);

	cv::Mat dst ;
	PRINTMAT_F(m, NPF);

	cv::Sobel(m, dst, CV_16S, 1, 0);
	PRINTMAT_F(dst, NPF);
	cv::Sobel(dst, dst, CV_16S, 1, 0);
	PRINTMAT_F(dst, NPF);

	cv::Sobel(m, dst, CV_16S, 0,1);
	PRINTMAT_F(dst, NPF);
	cv::Sobel(dst,dst, CV_16S, 0, 1);
	PRINTMAT_F(dst, NPF);


	cv::Laplacian(m, dst, CV_16S,3);
	PRINTMAT_F(dst,NPF);
}

static void ex2()
{
	cv::Mat kx, ky;
	cv::getDerivKernels(kx, ky, 2, 0, 3);
	cv::getDerivKernels(kx, ky, 0, 2, 3);
	PRINTMAT_F(kx,NPF);
	PRINTMAT_F(ky,NPF);
	

}

static void sobel_opterator(const cv::String &img_path)
{
	cv::Mat src = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!src.empty());
	int kernel_sz = 3;
	COND_BEGIN_LOOP
		cv::Mat img;
		cv::GaussianBlur(src, img, cv::Size(3, 3), 0, 0);

		//laplacian
		cv::Mat laplacian_dst;
		cv::Laplacian(img, laplacian_dst, CV_16S, kernel_sz);
		cv::Mat lap_abs_dst;
		cv::convertScaleAbs(laplacian_dst, lap_abs_dst);

		//sobel
		cv::Mat grad_x, grad_y;
		cv::Sobel(img, grad_x, CV_16S, 1, 0, kernel_sz);
		cv::Sobel(img, grad_y, CV_16S, 0, 1, kernel_sz);
		cv::Mat grad_x_abs, grad_y_abs;
		cv::convertScaleAbs(grad_x, grad_x_abs);
		cv::convertScaleAbs(grad_y, grad_y_abs);
		cv::Mat sobel_dst;
		cv::addWeighted(grad_x_abs, 0.5, grad_y_abs, 0.5, 0, sobel_dst);

		//show
		cv::imshow("src", src);
		cv::imshow("sobel demo", sobel_dst);
		cv::imshow("laplacian demo", lap_abs_dst);
		char key = cv::waitKey(0);
		COND_PRED(key == 27, break);
		COND_PRED(key=='k',kernel_sz+=2);
	COND_END_LOOP
}

int main_sobel(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(sobel_opterator(img_path));

	return 0;
}