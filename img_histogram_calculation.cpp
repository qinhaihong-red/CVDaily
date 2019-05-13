#include "cv_helper.h"
#include <vector>

static void histogram_calc(const cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());

	std::vector<cv::Mat> vec;
	cv::split(img, vec);

	int bins = 256;
	float ranges[] = {0,256};
	const float *histRange = ranges;

	cv::Mat b_hist, g_hist, r_hist;
	cv::calcHist(&vec[0],1,0,cv::Mat(),b_hist,1,&bins,&histRange);
	cv::calcHist(&vec[1], 1, 0, cv::Mat(), g_hist, 1, &bins, &histRange);
	cv::calcHist(&vec[2], 1, 0, cv::Mat(), r_hist, 1, &bins, &histRange);


	cv::Mat histImage(400, 512, CV_8UC3);
	cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX);
	cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX);
	cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX);

	int bin_w = cvRound(float(histImage.cols) / bins);

	for (size_t i = 1; i < bins; i++)
	{
		cv::line(histImage, cv::Point(bin_w*(i - 1), histImage.rows - cvRound(b_hist.at<float>(i - 1))),
			cv::Point(bin_w*i, histImage.rows - cvRound(b_hist.at<float>(i))), cv::Scalar(255, 0, 0), 2);

		cv::line(histImage,cv::Point(bin_w*(i-1),histImage.rows-cvRound(g_hist.at<float>(i-1))), 
			cv::Point(bin_w*i,histImage.rows - cvRound(g_hist.at<float>(i))),cv::Scalar(0,255,0),2);


		cv::line(histImage,cv::Point(bin_w*(i-1),histImage.rows-cvRound(r_hist.at<float>(i-1))),
			cv::Point(bin_w*i,histImage.rows - cvRound(r_hist.at<float>(i))),cv::Scalar(0,0,255),2);
	}

	cv::imshow("hist", histImage);
	cv::waitKey();
	
}


int main_hist_calc(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(histogram_calc(img_path));
	return 0;
}