#include "cv_helper.h"
#include<opencv2/features2d.hpp>
#include<opencv2/xfeatures2d.hpp>


static void object_detection(cv::String &obj_path,cv::String &scene_path)
{
	cv::Mat img_obj, img_scene;
	img_obj = cv::imread(obj_path, cv::IMREAD_GRAYSCALE);
	img_scene = cv::imread(scene_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img_obj.empty() && !img_scene.empty());

	cv::Ptr<cv::Feature2D> p_detector = xf::SURF::create();
	CV_Assert(p_detector);
	std::vector<cv::KeyPoint> vkp_obj, vkp_scene;
	cv::Mat descriptor_obj, descriptor_scene;
	p_detector->detectAndCompute(img_obj, cv::Mat(), vkp_obj, descriptor_obj);
	p_detector->detectAndCompute(img_scene, cv::Mat(), vkp_scene, descriptor_scene);

	//to be continued...
}


int main_od_planar(_MAIN_ARGS)
{
	char *obj_name = (argc == 3) ? argv[1] : "box";
	char *scene_name = (argc == 3) ? argv[2] : "box_in_scene";

	cv::String obj_path, scene_path;
	CV_Assert(cv_helper::get_imgPathEx(obj_name, obj_path, "OPENCV41_IMG_DIR"));
	CV_Assert(cv_helper::get_imgPathEx(scene_name, scene_path, "OPENCV41_IMG_DIR"));
	CV_TRY_CATCH(object_detection(obj_path, scene_path));
	
	return 0;
}