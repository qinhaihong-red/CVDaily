#include "cv_helper.h"
#include <opencv2/features2d.hpp>
#ifdef HAVE_OPENCV_XFEATURES2D
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D


#include <vector>

#define xf cv::xfeatures2d 

static void feature_detectEx(cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path,cv::IMREAD_GRAYSCALE);
	CV_Assert(!img.empty());

	int hessian_threshold = 400;
	cv::Ptr<xf::SURF> p_detector = xf::SURF::create(hessian_threshold);
	std::vector<cv::KeyPoint> v_kp;
	p_detector->detect(img, v_kp);

	cv::Mat dst;
	cv::drawKeypoints(img, v_kp, dst);
	cv::imshow("dst", dst);
	cv::waitKey();

}


int main_feature_detection(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "coco3";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(name, img_path));
	CV_TRY_CATCH(feature_detectEx(img_path));
	return 0;
}