#include "cv_helper.h"

void func_histogram_equalizaiton(cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());
	cv::Mat dst = img.clone();
	int npixels = img.cols*img.rows;
	int ncols = img.cols*img.channels();
	double blue[256] = { 0 };
	double green[256] = {0};
	double red[256] = { 0 };

	for (int i=0;i<img.rows;++i)
	{
		uchar *p = img.ptr<uchar>(i);
		for (int j=0;j<ncols;)
		{
			++blue[p[j++]];
			++green[p[j++]];
			++red[p[j++]];
		}
	}


	blue[0] = (blue[0] / npixels) * 255;
	green[0] = (green[0] / npixels) * 255;
	red[0] = (red[0] / npixels) * 255;
	for (int i = 1; i < 256; ++i)
	{
		blue[i] = (blue[i] / npixels) * 255 +blue[i-1];
		green[i] = (green[i]/ npixels) * 255 + green[i-1];
		red[i] = (red[i]/ npixels) * 255 + red[i-1];
	}

	//method 1:use .ptr
	for (int i = 0; i < dst.rows; ++i)
	{
		uchar *p = dst.ptr<uchar>(i);
		for (int j = 0; j < ncols;)
		{
			p[j++] = cv::saturate_cast<uchar>(blue[p[j]]);
			p[j++] = cv::saturate_cast<uchar>(green[p[j]]);
			p[j++] = cv::saturate_cast<uchar>(red[p[j]]);

		}
	}

	//method 2:use iterator
	//for (auto iter=dst.begin<cv::Vec3b>();iter!=dst.end<cv::Vec3b>();++iter)
	//{
	//	(*iter).val[0]= cv::saturate_cast<uchar>(blue[(*iter).val[0]]);
	//	(*iter).val[1] = cv::saturate_cast<uchar>(green[(*iter).val[1]]);
	//	(*iter).val[2] = cv::saturate_cast<uchar>(red[(*iter).val[2]]);
	//}



	cv::namedWindow(SRC_WIND, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(DST_WIND, cv::WINDOW_AUTOSIZE);
	cv::imshow(SRC_WIND, img);
	cv::imshow(DST_WIND, dst);
	cv::waitKey();
}


int main_histo_eq(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(name, img_path));
	CV_TRY_CATCH(func_histogram_equalizaiton(img_path));
	return 0;
}