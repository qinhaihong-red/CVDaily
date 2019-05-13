#include "cv_helper.h"
#include <algorithm>
#include <math.h>
void func_alpha_beta_correction()
{
	cv::String img_path;
	if(!cv_helper::get_imgPathEx("jordan.jpg", img_path))_EXIT_FAILURE;
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	cv::Mat out_img1=cv::Mat::zeros(img.size(),img.type());
	cv::Mat out_img2 = out_img1.clone();

	auto apply_correction_with_self_def = [&](double alpha, int beta,cv::Mat &outimg)
	{
		auto src_bgein = img.begin<cv::Vec3b>(),dst_begin=outimg.begin< cv::Vec3b >();
		auto src_end = img.end<cv::Vec3b>(),  dst_end=outimg.end<cv::Vec3b>();

		std::transform(src_bgein, src_end, dst_begin,[&](cv::Vec3b &src)
		{
			cv::Vec3b dst;
			dst.val[0] = cv::saturate_cast<uchar>(alpha*src.val[0] + beta);
			dst.val[1] = cv::saturate_cast<uchar>(alpha*src.val[1] + beta);
			dst.val[2] = cv::saturate_cast<uchar>(alpha*src.val[2] + beta);

			return dst;
		}
		);	
	};

	auto apply_correction_with_buit_in = [&](double alpha, int beta, cv::Mat &outimg)
	{
		img.convertTo(outimg, -1, alpha, beta);
	};



	try
	{
		apply_correction_with_self_def(2.2, 50, out_img1);
		apply_correction_with_buit_in(2.2, 50, out_img2);

		cv::imwrite("alpha_beta_correction_1.png", out_img1);
		cv::imwrite("alpha_beta_correction_2.png", out_img2);

	}
	catch (const cv::Exception& e)
	{
		EXCEPTION_INFO(e.what());
		_EXIT_FAILURE;
	}

	cv::namedWindow("src_wind", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("out_wind1", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("out_wind2", cv::WINDOW_AUTOSIZE);

	cv::imshow("src_wind", img);
	cv::imshow("out_wind1",out_img1);
	cv::imshow("out_wind2", out_img2);
	cv::waitKey();

}

void func_gamma_correction()
{
	cv::String img_path;
	if (!cv_helper::get_imgPathEx("jordan.jpg", img_path))_EXIT_FAILURE;
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	cv::Mat out_img1 = cv::Mat::zeros(img.size(), img.type());

	try
	{
		double gamma = 0.4;
		cv::Mat table(1, 256, CV_8U);
		uchar *p = table.ptr<uchar>();
		for (int i = 0; i < 256; ++i)
		{
			p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
		}

		cv::LUT(img, table, out_img1);
		cv::imwrite("gamma_correction.png", out_img1);
	}
	catch (const cv::Exception& e)
	{
		EXCEPTION_INFO(e.what());
		_EXIT_FAILURE;
	}


	cv::namedWindow("out_wind2", cv::WINDOW_AUTOSIZE);
	cv::imshow("out_wind1", out_img1);
	cv::waitKey();
}


int main_alpha_beta_gamma()
{
	//test ok
	//func_alpha_beta_correction();
	
	func_gamma_correction();

	return 0;
}