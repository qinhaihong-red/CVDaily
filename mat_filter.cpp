#include "cv_helper.h"
#include <opencv2/highgui.hpp>
#include <iomanip>

#define _RED cv::Scalar(0,0,255)
void apply_sharpen_filer(cv::Mat &IM,cv::Mat &OM)
{
	CV_Assert(IM.depth() == CV_8U);

	int nrows = IM.rows;
	int ncols = IM.cols;
	int nc = IM.channels();
	ncols *= nc;

	OM.create(IM.size(), IM.type());

	//for both the first/last row and the first/last col,
	//do not apply sharpen kernel
	for (int i=1;i<nrows-1;++i)
	{
		uchar *p_current  = IM.ptr<uchar>(i);
		uchar *p_previous = IM.ptr<uchar>(i-1);
		uchar *p_next = IM.ptr<uchar>(i+1);
		uchar *p_out = OM.ptr<uchar>(i);

		for (int j=nc;j<ncols-nc;++j)
		{
			*p_out++ = cv::saturate_cast<uchar>(5*p_current[j] - (p_previous[j]+p_next[j]+p_current[j-nc]+p_current[j+nc]));
		}
	}

	//set the first&last row and first&last col
	OM.row(0).setTo(_RED);
	OM.row(IM.rows - 1).setTo(_RED);
	OM.col(0).setTo(_RED);
	OM.col(IM.cols - 1).setTo(_RED);
}

void func_filter_on_mat()
{
	cv::String img_path;
	bool b = cv_helper::get_imgPathEx("jordan.jpg",img_path);
	if (!b)exit(EXIT_FAILURE);
	cv::Mat img_jordan = cv::imread(img_path, cv::IMREAD_COLOR);

	cv::namedWindow("src_wind", cv::WINDOW_AUTOSIZE);
	cv::namedWindow("dst_wind", cv::WINDOW_AUTOSIZE);


	cv::Mat img_sharpen_jordan;
	CV_TimeSpan ts;
	apply_sharpen_filer(img_jordan, img_sharpen_jordan);
	std::cout.precision(8);
	std::cout<<std::fixed<< "time elapsed with self defined kernel: "<<ts.stop()<<" s.\n";

	cv::imshow("src_wind", img_jordan);
	cv::imshow("dst_wind", img_sharpen_jordan);
	cv::waitKey();


	cv::Mat sharpen_filter = (cv::Mat_<char>(3, 3)<<0,-1,0,
													-1,5,-1,
													0,-1,0);

	cv::Mat img_sharpen_jordan2;
	ts.start();
	cv::filter2D(img_jordan, img_sharpen_jordan2, img_jordan.depth(),sharpen_filter );
	std::cout << std::fixed << "time elapsed with built-in kernel: " << ts.stop() << " s.\n";

	cv::imshow("dst_wind",img_sharpen_jordan2);
	cv::waitKey();
}


int main_filter()
{
	func_filter_on_mat();

	return 0;
}