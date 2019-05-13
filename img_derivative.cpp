#include "cv_helper.h"

#define STEP 6
#define ABS(X) ((X)>0? X:(-(X)))
#define PI 3.1415926f

static void ex()
{
	cv::Mat kx1, ky1;
	cv::getDerivKernels(kx1, ky1, 2, 2, 3);
	PRINTMAT_F(kx1,NPF);
	PRINTMAT_F(ky1, NPF);
	cv::Mat out;
	cv::mulTransposed(kx1, out, false);
	PRINTMAT_F(out, NPF);



	
}


static void hess(cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img.empty());

	int width = img.cols;
	int height = img.rows;

	cv::Mat dst(height, width, CV_8UC1, cv::Scalar::all(0));
	int W = 5;
	float sigma = 01;
	cv::Mat xxGauKernel(2 * W + 1, 2 * W + 1, CV_32FC1, cv::Scalar::all(0));
	cv::Mat xyGauKernel(2 * W + 1, 2 * W + 1, CV_32FC1, cv::Scalar::all(0));
	cv::Mat yyGauKernel(2 * W + 1, 2 * W + 1, CV_32FC1, cv::Scalar::all(0));

	//构建高斯二阶偏导数模板
	for (int i = -W; i <= W; i++)
	{
		for (int j = -W; j <= W; j++)
		{
			xxGauKernel.at<float>(i + W, j + W) = (1 - (i*i) / (sigma*sigma))*exp(-1 * (i*i + j*j) / (2 * sigma*sigma))*(-1 / (2 * PI*pow(sigma, 4)));
			yyGauKernel.at<float>(i + W, j + W) = (1 - (j*j) / (sigma*sigma))*exp(-1 * (i*i + j*j) / (2 * sigma*sigma))*(-1 / (2 * PI*pow(sigma, 4)));
			xyGauKernel.at<float>(i + W, j + W) = ((i*j))*exp(-1 * (i*i + j*j) / (2 * sigma*sigma))*(1 / (2 * PI*pow(sigma, 6)));
		}
	}


	for (int i = 0; i < (2 * W + 1); i++)
	{
		for (int j = 0; j < (2 * W + 1); j++)
		{
			std::cout << xxGauKernel.at<float>(i, j) << "  ";
		}
		std::cout << std::endl;
	}

	cv::Mat xxDerivae(height, width, CV_32FC1, cv::Scalar::all(0));
	cv::Mat yyDerivae(height, width, CV_32FC1, cv::Scalar::all(0));
	cv::Mat xyDerivae(height, width, CV_32FC1, cv::Scalar::all(0));
	//图像与高斯二阶偏导数模板进行卷积
	cv::filter2D(img, xxDerivae, xxDerivae.depth(), xxGauKernel);
	cv::filter2D(img, yyDerivae, yyDerivae.depth(), yyGauKernel);
	cv::filter2D(img, xyDerivae, xyDerivae.depth(), xyGauKernel);


	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{


			//map<int, float> best_step;

			/*	int HLx = h - STEP; if (HLx < 0){ HLx = 0; }
			int HUx = h + STEP; if (HUx >= height){ HUx = height - 1; }
			int WLy = w - STEP; if (WLy < 0){ WLy = 0; }
			int WUy = w + STEP; if (WUy >= width){ WUy = width - 1; }
			float fxx = img.at<uchar>(h, WUy) + img.at<uchar>(h, WLy) - 2 * img.at<uchar>(h, w);
			float fyy = img.at<uchar>(HLx, w) + img.at<uchar>(HUx, w) - 2 * img.at<uchar>(h, w);
			float fxy = 0.25*(img.at<uchar>(HUx, WUy) + img.at<uchar>(HLx, WLy) - img.at<uchar>(HUx, WLy) - img.at<uchar>(HLx, WUy));*/


			float fxx = xxDerivae.at<float>(h, w);
			float fyy = yyDerivae.at<float>(h, w);
			float fxy = xyDerivae.at<float>(h, w);


			float myArray[2][2] = { { fxx, fxy },{ fxy, fyy } };          //构建矩阵，求取特征值

			cv::Mat Array(2, 2, CV_32FC1, myArray);
			cv::Mat eValue;
			cv::Mat eVector;

			cv::eigen(Array, eValue, eVector);                               //矩阵是降序排列的
			float a1 = eValue.at<float>(0, 0);
			float a2 = eValue.at<float>(1, 0);

			if ((a1 > 0) && (ABS(a1) > (1 + ABS(a2))))             //根据特征向量判断线性结构
			{

				dst.at<uchar>(h, w) = cv::pow((ABS(a1) - ABS(a2)), 4);
				//dst.at<uchar>(h, w) = pow((ABS(a1) / ABS(a2))*(ABS(a1) - ABS(a2)), 1.5);
			}
		}
	}


	//----------做一个闭操作
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 2));
	cv::morphologyEx(dst, dst, cv::MORPH_CLOSE, element);

	cv::imwrite("temp.bmp", dst);

	cv::imshow("[原始图]", dst);
	cv::waitKey(0);
}

int main_hess(_MAIN_ARGS)
{
	CV_TRY_CATCH(ex());
	return 0;
	char *name = (argc == 2) ? argv[1] : "coco3";
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx(name, img_path));
	CV_TRY_CATCH(hess(img_path));
	return 0;

}