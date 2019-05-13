#include "cv_helper.h"
#include <vector>

static void histogram_comparison()
{
	cv::String path_base, path_t1, path_t2;
	cv_helper::get_imgPathEx("dog", path_base);
	cv_helper::get_imgPathEx("L1", path_t1);
	cv_helper::get_imgPathEx("L2", path_t2);
	cv::Mat base, t1, t2;
	base = cv::imread(path_base);
	t1 = cv::imread(path_t1);
	t2 = cv::imread(path_t2);

	cv::Mat hsv_base, hsv_t1, hsv_t2;
	cv::cvtColor(base, hsv_base, cv::COLOR_BGR2HSV);
	cv::cvtColor(t1, hsv_t1, cv::COLOR_BGR2HSV);
	cv::cvtColor(t2, hsv_t2, cv::COLOR_BGR2HSV);

	cv::Mat hsv_halfbase = hsv_base(cv::Range(0,hsv_base.rows/2),cv::Range(0,hsv_base.cols));
	int h_bins = 50, s_bins = 60;
	int histSz[] = {h_bins,s_bins};

	float h_range[] = {0,180};
	float s_range[] = {0,256};
	const float *histRange[] = {h_range,s_range};
	int channels[] = {0,1};

	cv::Mat hist_base, hist_halfbase, hist_t1, hist_t2;
	cv::calcHist(&hsv_base, 1, channels, cv::Mat(), hist_base, 2, histSz, histRange);
	cv::normalize(hsv_base, hsv_base, 0, 1,cv::NORM_MINMAX);

	cv::calcHist(&hsv_halfbase, 1, channels, cv::Mat(), hist_halfbase, 2, histSz, histRange);
	cv::normalize(hsv_halfbase, hsv_halfbase, 0, 1, cv::NORM_MINMAX);
	
	cv::calcHist(&hsv_t1,1,channels,cv::Mat(),hist_t1,2,histSz,histRange);
	cv::normalize(hsv_t1, hsv_t1, 0, 1, cv::NORM_MINMAX);

	cv::calcHist(&hsv_t2,1,channels,cv::Mat(),hist_t2,2,histSz,histRange);
	cv::normalize(hsv_t2, hsv_t2, 0, 1, cv::NORM_MINMAX);


	for (int method=0;method<4;++method)
	{
		double base_base = cv::compareHist(hist_base, hist_base, method);
		double base_half = cv::compareHist(hist_base,hist_halfbase,method);
		double base_t1 = cv::compareHist(hist_base,hist_t1,method);
		double base_t2 = cv::compareHist(hist_base,hist_t2,method);

		std::cout << "Method " << method << " Perfect, Base-Half, Base-Test(1), Base-Test(2) : "
			<< base_base << " / " << base_half << " / " << base_t1 << " / " << base_t2 << std::endl;
	}
	
}


int main_hist_comparison(_MAIN_ARGS)
{
	CV_TRY_CATCH(histogram_comparison());
	return 0;
}