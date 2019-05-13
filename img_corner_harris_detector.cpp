#include "cv_helper.h"

static cv::Mat img, img_gray;
static int thresh = 200, max_thresh = 255;


static void thresh_callbck(int, void *)
{
	
	cv::Mat dst;	
	cv::cornerHarris(img_gray, dst, 2, 3, 0.04);
	cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
	cv::Mat dst8u;
	dst.convertTo(dst8u, CV_8U);

	for (int i=0;i<dst8u.rows;++i)
	{
		for (int j=0;j<dst8u.cols;++j)
		{
			int pixel = int(dst8u.at<uchar>(i,j));
			if (pixel>thresh)
			{
				cv::Point center(j, i);
				cv::circle(dst8u, center, 5, cv::Scalar(0), 2);
			}
		}
	}

	cv::imshow(DST_WIND,dst8u);

}


static void harris_corner_detect(cv::String &img_path)
{
	img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());

	CV_CVT2GRAY(img, gray);
	img_gray = gray;

	cv::namedWindow(DST_WIND);
	cv::createTrackbar("Thresh:", DST_WIND, &thresh, max_thresh, thresh_callbck);

	thresh_callbck(0, 0);
	cv::imshow("src", img);
	cv::waitKey();
}

int main_harris_corner_detect(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "coco3";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(name, img_path));
	CV_TRY_CATCH(harris_corner_detect(img_path));
	return 0;
}