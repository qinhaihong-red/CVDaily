#include "cv_helper.h"
#include <opencv2/xfeatures2d.hpp>
#include <vector>

#define xf cv::xfeatures2d 

static void feature_description(cv::String &img_path1,cv::String &img_path2)
{
	cv::Mat img1, img2;
	img1 = cv::imread(img_path1,cv::IMREAD_GRAYSCALE);
	img2 = cv::imread(img_path2, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img1.empty() && !img2.empty());

	int hessian_threshold = 400;
	cv::Ptr<xf::SURF> p_detector = xf::SURF::create(hessian_threshold);
	std::vector<cv::KeyPoint> v_kp1,v_kp2;
	cv::Mat descriptor1, descriptor2;
	p_detector->detectAndCompute(img1, cv::Mat(), v_kp1, descriptor1);
	p_detector->detectAndCompute(img2,cv::Mat(),v_kp2,descriptor2);

	cv::Ptr<cv::DescriptorMatcher> p_matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::BRUTEFORCE);
	std::vector<cv::DMatch> v_match;
	p_matcher->match(descriptor1, descriptor2, v_match);

	cv::Mat img_match;
	cv::drawMatches(img1, v_kp1, img2, v_kp2, v_match, img_match);

	cv::imshow("dst",img_match);

	cv::waitKey();
}


int main_feature_description(_MAIN_ARGS)
{
	char *name1 = (argc == 3) ? argv[1] : "L1";
	char *name2 = (argc == 3) ? argv[2] : "L2";

	cv::String img_path1,img_path2;
	CV_Assert(cv_helper::get_imgPathEx(name1, img_path1));
	CV_Assert(cv_helper::get_imgPathEx(name2, img_path2));
	CV_TRY_CATCH(feature_description(img_path1,img_path2));
	return 0;
}