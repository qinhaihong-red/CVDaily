#include "cv_helper.h"

static cv::Mat src, src_gray;
static cv::Mat harris_feature, harris_H,harris_dst;
static cv::Mat shitomasi_feature, shitomasi_dst;

static int harris_qualityLevel = 50;
static int shitomasi_qualityLevel = 50;
static int max_qualityLevel = 100;

static double harris_minVal, harris_maxVal;
static double shitomasi_minVal, shitomasi_maxVal;

static cv::RNG rng(0XFF);

#define HARISS_WND	  "harrisCorner"
#define SHITOMASI_WND "shitomasiCorner"

static void harris_callback(int, void *)
{
	harris_qualityLevel = cv::max(harris_qualityLevel, 1);
	harris_dst = src.clone();

	for (int i=0;i<harris_H.rows;++i)
	{
		for (int j=0;j<harris_H.cols;++j)
		{
			float val = harris_H.at<float>(i,j);
			float thresh = harris_qualityLevel*(harris_maxVal - harris_minVal) / max_qualityLevel;
			if (val>thresh)
			{
				cv::circle(harris_dst, cv::Point(j, i), 4, cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)), -1);
			}
		}
	}

	cv::imshow(HARISS_WND, harris_dst);
}

static void shitomasi_callback(int, void *)
{
	shitomasi_qualityLevel = cv::max(shitomasi_qualityLevel,1);
	shitomasi_dst = src.clone();

	for (int i = 0; i < shitomasi_feature.rows; ++i)
	{
		for (int j = 0; j < shitomasi_feature.cols; ++j)
		{
			float val = shitomasi_feature.at<float>(i,j);
			float thresh = shitomasi_qualityLevel*(shitomasi_maxVal-shitomasi_minVal) / max_qualityLevel;
			if (val>thresh)
			{
				cv::circle(shitomasi_dst, cv::Point(j,i),4,cv::Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)),-1);
			}
		}
	}

	cv::imshow(SHITOMASI_WND,shitomasi_dst);
}


static void corner_ex(cv::String &img_path)
{
	src = cv::imread(img_path);
	CV_Assert(!src.empty());
	CV_CVT2GRAY(src,gray);
	src_gray = gray;

	cv::cornerEigenValsAndVecs(src_gray, harris_feature, 3, 3);
	//calculate covariation matrix to approximate Hessian matrix
	harris_H = cv::Mat(harris_feature.size(),CV_32FC1);
	for (int i=0;i<harris_H.rows;++i)
	{
		for (int j=0;j<harris_H.cols;++j)
		{
			double lambda1 = harris_feature.at<cv::Vec6f>(i, j)[0];
			double lambda2 = harris_feature.at<cv::Vec6f>(i, j)[1];
			harris_H.at<float>(i, j) = lambda1*lambda2 - 0.04*cv::pow(lambda1+lambda2,2);
		}
	}

	cv::minMaxLoc(harris_H, &harris_minVal, &harris_maxVal);

	cv::namedWindow(HARISS_WND);
	cv::createTrackbar("harris", HARISS_WND, &harris_qualityLevel, max_qualityLevel, harris_callback);

	cv::cornerMinEigenVal(src_gray, shitomasi_feature, 3);
	cv::minMaxLoc(shitomasi_feature, &shitomasi_minVal, &shitomasi_maxVal);
	cv::namedWindow(SHITOMASI_WND);
	cv::createTrackbar("shitomasi", SHITOMASI_WND, &shitomasi_qualityLevel, max_qualityLevel, shitomasi_callback);

	harris_callback(0,0);
	shitomasi_callback(0, 0);

	cv::waitKey();

}

int main_corner(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "coco3";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(name, img_path));
	CV_TRY_CATCH(corner_ex(img_path));
	return 0;
}