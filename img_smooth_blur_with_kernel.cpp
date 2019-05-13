#include "cv_helper.h"

#define DELAY_CAP 1500
#define DELAY_BLUR 100
#define MAX_KERNEL 31


template<typename F,typename Display,typename T,typename ...Args>
void blur_wrapper(F &&f,Display &&d ,T t,Args&& ...args)
{
	for (int i=1;i<MAX_KERNEL;i=i+2)
	{
		f(std::forward<Args>(args)...);
		d(t);
	}
	
}

void func_img_smooth()
{
	cv::Mat src, dst;
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx("lena.jpg",img_path));
	src = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!src.empty());

	const char *psz = "smoothing demo";
	
	
	auto func_display_dst = [&](int delay)
	{
		cv::imshow(psz, dst);
		cv::waitKey(delay);
	};

	auto func_display_caption = [&](const std::string &cap)
	{
		dst = cv::Mat::zeros(src.size(), src.type());
		cv::putText(dst, cap, cv::Point(dst.cols / 4, dst.rows / 2), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(255, 255, 255));
		func_display_dst(DELAY_CAP);
	};

	func_display_caption("Original Image");
	dst = src.clone();
	func_display_dst(DELAY_CAP);

	func_display_caption("Homogeneous Blur");
	for (int i=1;i<MAX_KERNEL;i+=2)
	{
		cv::blur(src, dst, cv::Size(i, i), cv::Point(-1, -1));
		func_display_dst(DELAY_BLUR);
	}

	func_display_caption("Gaussian Blur");
	for (int i = 1; i<MAX_KERNEL; i += 2)
	{
		cv::GaussianBlur(src, dst, cv::Size(i, i), 0, 0);
		func_display_dst(DELAY_BLUR);
	}

	func_display_caption("Median Blur");
	for (int i = 1; i < MAX_KERNEL; i += 2)
	{
		cv::medianBlur(src, dst,i);
		func_display_dst(DELAY_BLUR);
	}

	func_display_caption("Bilateral Blur");
	for (int i = 1; i < MAX_KERNEL; i += 2)
	{
		cv::bilateralFilter(src, dst,i,i*2,i/2);
		func_display_dst(DELAY_BLUR);
	}

}

int main_smooth_blur()
{
	CV_TRY_CATCH(func_img_smooth());
	return 0;
}