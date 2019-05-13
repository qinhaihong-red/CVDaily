#include "cv_helper.h"

static void remapping(const cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());
	cv::imshow("src", img);
	cv::waitKey();

	auto remapping_transform = [&img](cv::InputArray mapx,cv::InputArray mapy)->cv::Mat
	{
		cv::Mat out;
		cv::remap(img, out, mapx, mapy, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
		return out;
	};

	//1.shrink to half of the original size
	cv::Mat mapx(img.size(), CV_32FC1), mapy(img.size(), CV_32FC1);
	int rows = img.rows, cols = img.cols;
	for (int i=0;i<rows;++i)
	{
		for (int j=0;j<cols;++j)
		{
			if (j > 0.25*cols && j < 0.75*cols && i>0.25*rows && i<0.75*rows)
			{
				mapx.at<float>(i, j) = float (2 * j - (cols / 2) + 0.5);
				mapy.at<float>(i, j) = float (2 * i - (rows / 2) + 0.5);
			}
			else
			{
				mapx.at<float>(i, j) = 0;
				mapy.at<float>(i, j) = 0;
			}
		}
	}
	cv::Mat shrink = remapping_transform(mapx, mapy);

	//2.turn it upside down
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j< cols; ++j)
		{
			mapx.at<float>(i, j) = float(j);
			mapy.at<float>(i, j) = float(rows - i);
		}
	}
	cv::Mat upsidedown = remapping_transform(mapx,mapy);

	//3.reflecting it in x direction
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			mapx.at<float>(i, j) = float(cols-j);
			mapy.at<float>(i, j) = float(i);
		}
	}
	cv::Mat reflection_x = remapping_transform(mapx,mapy);

	
	cv::imshow("shrink", shrink);
	cv::imshow("upsidedown", upsidedown);
	cv::imshow("reflection_x", reflection_x);

	cv::waitKey();

}

int main_remapping(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(remapping(img_path));
	return 0;
}