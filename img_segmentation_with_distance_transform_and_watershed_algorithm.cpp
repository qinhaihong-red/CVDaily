#include "cv_helper.h"
#include <vector>


static void segmentation_with_distance_transform_and_watershed_algor(cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path);
	CV_Assert(!img.empty());

	cv::imshow("src", img);

	for (int i=0;i<img.rows;++i)
	{
		for (int j=0;j<img.cols;++j)
		{

			//notice .at returns reference of a pixel,
			//in order to change pixel intensity,a reference type should be used.
			cv::Vec3b &pixel = img.at<cv::Vec3b>(i, j);
			COND_PRED(pixel[0]==255&&pixel[1]==255&&pixel[2]==255,(pixel[0]=pixel[1]=pixel[2]=0));
		}
	}

	cv::imshow("after black background", img);

	cv::Mat kernel = (cv::Mat_<float>(3,3)<<1,1,1,	
											1,-8,1,
											1,1,1);
	cv::Mat laplacian;
	cv::filter2D(img, laplacian, CV_32F, kernel);
	cv::Mat img_32f;
	img.convertTo(img_32f, CV_32F);
	cv::Mat result = (img_32f - laplacian);
	cv::Mat sharpened;
	result.convertTo(sharpened, CV_8U);
	cv::Mat laplacian_8u;
	laplacian.convertTo(laplacian_8u, CV_8U);
	cv::imshow("laplacian", laplacian_8u);
	cv::imshow("sharpend", sharpened);
	
	cv::Mat img_bin;
	cv::cvtColor(sharpened, img_bin,cv::COLOR_BGR2GRAY);
	cv::threshold(img_bin, img_bin,40,255 ,cv::THRESH_BINARY | cv::THRESH_OTSU);
	cv::imshow("bin", img_bin);

	cv::Mat img_distance;
	cv::distanceTransform(img_bin, img_distance, cv::DIST_L2, 3);
	cv::normalize(img_distance, img_distance, 0, 1, cv::NORM_MINMAX);
	cv::Mat img_distance8u;
	img_distance.convertTo(img_distance8u, CV_8U, 255);
	cv::imshow("distance", img_distance8u);

	cv::threshold(img_distance, img_distance, 0.4, 1, cv::THRESH_BINARY);
	img_distance.convertTo(img_distance8u, CV_8U, 255);
	cv::Mat dilate_kernel = cv::Mat::ones(3, 3, CV_8U);
	cv::dilate(img_distance8u,img_distance8u,dilate_kernel);
	cv::imshow("peaks", img_distance8u);

	std::vector<std::vector<cv::Point>> contour;
	cv::findContours(img_distance8u, contour, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	cv::Mat markers = cv::Mat(img_distance.size(),CV_32S);
	for (int i=0;i<contour.size();++i)
	{
		cv::drawContours(markers, contour, i, cv::Scalar(i + 1));
	}
	cv::circle(markers, cv::Point(5, 5), 3, cv::Scalar(255));
	//cv::imshow("markers", markers * 10000);


	cv::watershed(sharpened, markers);
	/*	cv::Mat mark;
		markers.convertTo(mark, CV_8U)*/;

	std::vector<cv::Vec3b> colors(contour.size());
	cv::RNG rng(0XFFFF);
	for (int i=0;i<contour.size();++i)
	{
		colors[i][0] = rng.uniform(0, 255);
		colors[i][1] = rng.uniform(0, 255);
		colors[i][2] = rng.uniform(0, 255);

	}

	cv::Mat dst = cv::Mat(markers.size(),CV_8UC3);

	for (int i=0;i<markers.rows;++i)
	{
		for (int j=0;j<markers.cols;++j)
		{
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contour.size()))
			{
				dst.at<cv::Vec3b>(i, j) = colors[index - 1];
			}
		}
	}

	cv::imshow("dst", dst);


	cv::waitKey();

}





int main_seg_watershed(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "81";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(segmentation_with_distance_transform_and_watershed_algor(img_path));
	return 0;
}