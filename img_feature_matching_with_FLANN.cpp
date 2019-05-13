#include "cv_helper.h"
#include <opencv2/xfeatures2d.hpp>
#include <vector>

static void feature_matching_with_FLANN(cv::String &img_path1, cv::String &img_path2)
{

	cv::Mat img1 = cv::imread(img_path1, cv::IMREAD_GRAYSCALE);
	cv::Mat img2 = cv::imread(img_path2, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img1.empty() && !img2.empty());

	//cv::imshow("src1", img1);
	//cv::imshow("src2", img2);

	int hessian_threshold = 400;
	cv::Ptr<xf::SURF> p_detector = xf::SURF::create(hessian_threshold);
	std::vector<cv::KeyPoint> v_kp1, v_kp2;
	cv::Mat descrpition1, description2;
	p_detector->detectAndCompute(img1, cv::Mat(), v_kp1, descrpition1);
	p_detector->detectAndCompute(img2, cv::Mat(), v_kp2, description2);

	cv::Ptr<cv::DescriptorMatcher> p_matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
	std::vector<std::vector<cv::DMatch>> v_matches;
	p_matcher->knnMatch(descrpition1, description2, v_matches,2);

	float ratio_thresh = 0.7f;
	std::vector<cv::DMatch> v_goodMatch;
	std::for_each(std::begin(v_matches), std::end(v_matches), [&v_goodMatch,&ratio_thresh](std::vector<cv::DMatch>& v_m) 
	{

		COND_PRED(v_m[0].distance < ratio_thresh*v_m[1].distance, v_goodMatch.push_back(v_m[0]));

	});

	cv::Mat img_match;
	cv::drawMatches(img1, v_kp1, img2, v_kp2, v_goodMatch, img_match,cv::Scalar::all(-1),cv::Scalar::all(-1),std::vector<char>(),cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	cv::imshow("dst", img_match);
	cv::waitKey();

}


int main_fann(_MAIN_ARGS)
{

	char *name1 = (argc == 3) ? argv[1] : "box";
	char *name2 = (argc == 3) ? argv[2] : "box_in_scene";

	cv::String img_path1, img_path2;
	CV_Assert(cv_helper::get_imgPathEx(name1, img_path1,"OPENCV41_IMG_DIR"));
	CV_Assert(cv_helper::get_imgPathEx(name2, img_path2,"OPENCV41_IMG_DIR"));
	CV_TRY_CATCH(feature_matching_with_FLANN(img_path1, img_path2));

	return 0;
}