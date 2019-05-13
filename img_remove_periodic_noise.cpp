#include "cv_helper.h"

static void synthesizeFilterH(cv::Mat& inputOutput_H, int radius)
{
	int cx = inputOutput_H.cols / 2, cy = inputOutput_H.rows / 2;

	cv::Point center(cx,cy);
	cv::Point p1(cx, cy / 2);
	cv::Point p2(int(0.75*cx),int(0.75*cy));
	cv::Point p3(cx/2,cy);
	cv::Point p4(int(0.75*cx), int(1.25*cy));
	cv::Point p5(cx, int(1.5*cy));
	cv::Point p6(int(1.25*cx), int(1.25*cy));
	cv::Point p7(int(1.5*cx),cy);
	cv::Point p8(int(1.25*cx),int(0.75*cy));


	cv::circle(inputOutput_H, center, radius, 0, -1);
	cv::circle(inputOutput_H, p1, radius, 0, -1);
	cv::circle(inputOutput_H, p2, radius, 0, -1);
	cv::circle(inputOutput_H, p3, radius, 0, -1);
	cv::circle(inputOutput_H, p4, radius, 0, -1);
	cv::circle(inputOutput_H, p5, radius, 0, -1);
	cv::circle(inputOutput_H, p6, radius, 0, -1);
	cv::circle(inputOutput_H, p7, radius, 0, -1);
	cv::circle(inputOutput_H, p8, radius, 0, -1);

}



static void remove_periodic_noise(cv::String &img_path)
{
	cv::Mat temp = cv::imread(img_path,cv::IMREAD_GRAYSCALE);
	CV_Assert(!temp.empty());

	cv::Mat img;
	cv_helper::get_optimalImage(temp,img);
	cv::Mat imgPSD;
	cv_helper::calc_dftPSD(img, imgPSD);
	cv::Mat imgPSD_shift;
	cv_helper::dftshift(imgPSD, imgPSD_shift);
	cv::normalize(imgPSD_shift,imgPSD_shift, 0, 255, cv::NORM_MINMAX);
	imgPSD_shift.convertTo(imgPSD_shift, CV_8U);
	cv::imshow("dftPSD", imgPSD_shift);



	cv::Mat H = cv::Mat(img.size(), CV_32F, cv::Scalar(1));
	synthesizeFilterH(H, 2);
	cv::Mat H8u;
	H.convertTo(H8u, CV_8U, 255);
	cv::imshow("filter", H8u);

	cv::Mat dst;
	cv_helper::filterDomain(img, dst, H, true);
	
	cv::imshow("dst", dst);

	cv::imshow("src", img);
	cv::waitKey();
}

int main_periodic_noise(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(name, img_path));
	CV_TRY_CATCH(remove_periodic_noise(img_path));
	return 0;
}