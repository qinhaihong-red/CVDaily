#include "cv_helper.h"

static void make_borders(const cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path),dst;
	CV_Assert(!img.empty());
	int border_type = cv::BORDER_CONSTANT;
	cv::RNG rng(0xFFFF);
	int top = int(0.05f*img.rows); int bottom = top;
	int left = int(0.05f*img.cols); int right = left;

	COND_BEGIN_LOOP
		cv::Scalar value(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
		cv::copyMakeBorder(img,dst,top,bottom,left,right,border_type,value);
		cv::imshow("padding border demo", dst);
		char key = cv::waitKey(500);
		COND_PRED(key == 27, break);
		COND_PRED(key == 'c',border_type=cv::BORDER_CONSTANT);
		COND_PRED(key == 'r',border_type=cv::BORDER_REPLICATE);
	COND_END_LOOP
}

int main_paddingborders(_MAIN_ARGS)
{
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(name,img_path));
	CV_TRY_CATCH(make_borders(img_path));
	return 0;
}