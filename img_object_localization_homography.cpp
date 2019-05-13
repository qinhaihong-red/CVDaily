#include "cv_helper.h"
#include <opencv2/calib3d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <vector>


static void homograhpy_localization(cv::String &obj_path, cv::String &scene_path)
{
	cv::Mat img_obj, img_scene;
	img_obj = cv::imread(obj_path, cv::IMREAD_GRAYSCALE);
	img_scene = cv::imread(scene_path,cv::IMREAD_GRAYSCALE);
	CV_Assert(!img_obj.empty() && !img_scene.empty());


	int hessian_threshold = 400;
	cv::Ptr<xf::SURF> p_detector = xf::SURF::create(hessian_threshold);
	CV_Assert(p_detector);
	std::vector<cv::KeyPoint> vkp_obj, vkp_scene;
	cv::Mat descriptor_obj, descriptor_scene;
	p_detector->detectAndCompute(img_obj, cv::Mat(), vkp_obj, descriptor_obj);
	p_detector->detectAndCompute(img_scene,cv::Mat(),vkp_scene,descriptor_scene);
	cv::Ptr<cv::DescriptorMatcher> p_matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::MatcherType::FLANNBASED);
	CV_Assert(p_matcher);
	std::vector<std::vector<cv::DMatch>> vv_matches;
	p_matcher->knnMatch(descriptor_obj, descriptor_scene, vv_matches,2);

	float ratio_thresh = 0.75f;
	std::vector<cv::DMatch> good_matches;
	std::vector<cv::Point2f> vpt_obj,vpt_scene;
	std::for_each(std::begin(vv_matches), std::end(vv_matches), [&](std::vector<cv::DMatch> &v_m)
	{

		if (v_m[0].distance<ratio_thresh*v_m[1].distance)
		{
			good_matches.push_back(v_m[0]);
			vpt_obj.push_back(vkp_obj[v_m[0].queryIdx].pt);
			vpt_scene.push_back(vkp_scene[v_m[0].queryIdx].pt);
		}

	});

	cv::Mat img_match;
	cv::drawMatches(img_obj, vkp_obj, img_scene, vkp_scene, good_matches, img_match,
		cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(),
		cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	cv::Mat H = cv::findHomography(vpt_obj, vpt_scene, cv::RANSAC);

	std::vector<cv::Point2f> vpt_objCorners(4), vpt_sceneCorners(4);
	vpt_objCorners[0] = cv::Point2f(0,0);
	vpt_objCorners[1] = cv::Point2f(img_obj.cols,0);
	vpt_objCorners[2] = cv::Point2f(img_obj.cols,img_obj.rows);
	vpt_objCorners[3] = cv::Point2f(0,img_obj.rows);

	cv::perspectiveTransform(vpt_objCorners, vpt_sceneCorners, H);

	auto green = cv::Scalar(0, 255, 0);
	int thickness = 4;
	auto shift = cv::Point2f(img_obj.cols, 0);
	cv::line(img_match, vpt_sceneCorners[0] + shift, vpt_sceneCorners[1] + shift, green, thickness);
	cv::line(img_match, vpt_sceneCorners[1] + shift, vpt_sceneCorners[2] + shift, green, thickness);
	cv::line(img_match, vpt_sceneCorners[2] + shift, vpt_sceneCorners[3] + shift, green, thickness);
	cv::line(img_match, vpt_sceneCorners[3] + shift, vpt_sceneCorners[0] + shift, green, thickness);


	cv::imshow("dst", img_match);
	cv::waitKey();
}


int main_homography(_MAIN_ARGS)
{
	char *obj_name = (argc == 3) ? argv[1] : "box";
	char *scene_name = (argc == 3) ? argv[2] : "box_in_scene";
	
	cv::String obj_path, scene_path;
	CV_Assert(cv_helper::get_imgPathEx(obj_name, obj_path, "OPENCV41_IMG_DIR"));
	CV_Assert(cv_helper::get_imgPathEx(scene_name, scene_path, "OPENCV41_IMG_DIR"));
	CV_TRY_CATCH(homograhpy_localization(obj_path, scene_path));

	return 0;
}