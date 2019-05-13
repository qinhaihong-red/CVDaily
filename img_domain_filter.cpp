#include "cv_helper.h"
#include <vector>

//test cv_helper::histEqualization
static void ex()
{
	cv::String img_path;
	cv_helper::get_imgPathEx("lena", img_path);
	cv::Mat lena_gray = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	cv::Mat lena= cv::imread(img_path, cv::IMREAD_COLOR);

	cv::Mat gray_heq;
	cv_helper::histEqualization(lena_gray, gray_heq);
	cv::Mat color_heq;
	cv_helper::histEqualization(lena, color_heq);

	cv::imshow("src_gray", lena_gray);
	cv::imshow("src_color", lena);
	cv::imshow("gray_heq", gray_heq);
	cv::imshow("color_heq", color_heq);

	cv::waitKey();

}

static void synthesizeFilterH(cv::Mat& inputOutput, int fill_val,int radius)
{
	int cx = inputOutput.cols / 2, cy = inputOutput.rows / 2;
	cv::circle(inputOutput, cv::Point(cx,cy), radius, fill_val, -1);
}

static void band_pass_filter(cv::String &img_path)
{
	cv::Mat temp = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!temp.empty());
	cv::Mat img;
	cv_helper::get_optimalImage(temp, img);

	cv::Mat lowpass = cv::Mat::zeros(img.size(), CV_32F);
	synthesizeFilterH(lowpass, 1, 20);

	cv::Mat highpass = cv::Mat::ones(img.size(),CV_32F);
	synthesizeFilterH(highpass, 0, 10);

	cv::Mat lp_out;
	cv_helper::filterDomain(img, lp_out, lowpass, true);
	cv::Mat hp_out;
	cv_helper::filterDomain(img, hp_out, highpass, true);

	cv::imshow("lowpass", lp_out);
	cv::imshow("highpass", hp_out);
	cv::imshow("src1", img);
	cv::waitKey();
}

int main_pass_filter(_MAIN_ARGS)
{
	//ex();
	//return 0;
	char *name = (argc == 2) ? argv[1] : "dog";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(band_pass_filter(img_path));
	return 0;
}