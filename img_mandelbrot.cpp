#include "cv_helper.h"
#include <complex>
#include <math.h>

int mandelbrot_process(const std::complex<float> &c, int maxiter)
{
	std::complex<float> z = c;
	int i = 0;
	for (;i<maxiter;++i)
	{
		if (z.real()*z.real()+z.imag()*z.imag()>4.0f) return i;
		z = z*z + c;
	}

	return maxiter;
}

int mandelbrot_gen_color(const std::complex<float> &c, int maxiter = 500)
{
	int iter = mandelbrot_process(c, maxiter);
	if (iter == maxiter) return 0;

	return cvRound(cv::sqrt(iter / float(maxiter)) * 255);
}

void sequentialMandelbrot(cv::Mat &img,float x1, float x2,float y1,float y2)
{
	int cols = img.cols;
	int rows = img.rows;
	float Xscale = cols / (x2 - x1);
	float Yscale = rows / (y2 - y1);

	for (int i=0;i<rows;++i)
	{
		for (int j=0;j<cols;++j)
		{
			float x = j / Xscale + x1;
			float y = i / Yscale + y1;
			std::complex<float> c(x, y);
			uchar color = cv::saturate_cast<uchar>(mandelbrot_gen_color(c));
			img.ptr<uchar>(i)[j] = color;
		}
	}
}

void sequential_gen()
{
	cv::Mat img=cv::Mat::zeros(4800, 5400, CV_8U);
	float x1 = -2.1f, x2 = 0.6f;
	float y1 = -1.2f, y2 = 1.2f;
	sequentialMandelbrot(img, x1, x2, y1, y2);

	cv::imwrite("sequential_mandelbrot_gen.png", img);
}

void parallel_gen()
{
	cv::Mat img = cv::Mat::zeros(4800, 5400, CV_8U);
	float x1 = -2.1f, x2 = 0.6f;
	float y1 = -1.2f, y2 = 1.2f;

	int cols = img.cols;
	int rows = img.rows;
	float Xscale = cols / (x2 - x1);
	float Yscale = rows / (y2 - y1);

	//this parallel mode looks like cuda's block and grid
	cv::parallel_for_(cv::Range(0, rows*cols), [&](const cv::Range &r)
	{
		for (int k=r.start;k<r.end;++k)
		{
			int i = k / cols ;
			int j = k % cols ;

			float x = j / Xscale + x1;
			float y = i / Yscale + y1;
			std::complex<float> c(x, y);

			uchar color = cv::saturate_cast<uchar>(mandelbrot_gen_color(c));
			img.ptr<uchar>(i)[j] = color;
		}
	});

	cv::imwrite("parallel_mandelbrot_gen.png", img);
}

int main_mandelbrot()
{
	CV_TimeSpan ts;
	CV_TRY_CATCH(sequential_gen());
	std::cout << "sequential_gen ok.cost time:"<<ts.stop()<<" s.\n";

	ts.start();
	parallel_gen();
	std::cout << "parallel_gen ok.cost time:" << ts.stop() << " s.";

	return 0;
}