#include "cv_helper.h"

static void affine_transformation(const cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());

	cv::Point2f orgin_pts[3];
	orgin_pts[0].x = 0.0f; orgin_pts[0].y = 0.0f;
	orgin_pts[1].x = img.cols - 1; orgin_pts[1].y = 0.0f;
	orgin_pts[2].x = 0.0f; orgin_pts[2].y = img.rows - 1;

	cv::Point2f dst_pts[3];
	dst_pts[0].x = 0.0f; dst_pts[0].y = 0.25*img.rows;
	dst_pts[1].x = 0.75*img.cols; dst_pts[1].y = 0.2*img.rows;
	dst_pts[2].x = 0.2*img.cols; dst_pts[2].y = 0.75*img.rows;

	cv::Mat aff_transform;
	aff_transform = cv::getAffineTransform(orgin_pts, dst_pts);
	cv::Mat aff_dst=cv::Mat::zeros(img.size(),img.type());
	cv::warpAffine(img, aff_dst, aff_transform, aff_dst.size());
	
	
	cv::Mat rotate_transform = cv::getRotationMatrix2D(cv::Point2f(aff_dst.cols / 2.0f, aff_dst.rows / 2.0f), 90, 1);
	cv::Mat rotate_dst = cv::Mat::zeros(img.size(),img.type());
	PRINTMAT_F(rotate_transform,NPF);
	cv::warpAffine(aff_dst, rotate_dst, rotate_transform, rotate_dst.size());
	
	cv::imshow("aff result", aff_dst);
	cv::imshow("rotate result", rotate_dst);
	cv::waitKey();

}

int main_warp_affine_tranformation(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(affine_transformation(img_path));
	return 0;
}