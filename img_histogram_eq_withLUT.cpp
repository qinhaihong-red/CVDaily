#include "cv_helper.h"
#include <vector>
#include <time.h>
#include <algorithm>
#include <numeric>

//this is for test purpose
static void ex()
{

	std::vector<cv::Mat> M;
	CV_INIT_VECTOR(M,3,cv::Mat::ones(2, 3, CV_32F));

	//WARNING!!!
	//this initialization below will have 3 copies to each other,
	//which means,they share the same internal memory
	//std::vector<cv::Mat> M(3,cv::Mat::ones(2, 3, CV_32F));

	PRINTMAT_F(M[0], NPF);
	PRINTMAT_F(M[1], NPF);
	PRINTMAT_F(M[2], NPF);

	cv::RNG rng(clock());
	cv::Mat A=cv::Mat::zeros(2,3,CV_32FC3);
	cv::Vec3f &pixel = A.at<cv::Vec3f>(0, 0);
	std::cout << "\npixel[0,0] from A:\n"<<pixel[0] << "," << pixel[1] << "," << pixel[2] << std::endl;
	rng.fill(A, cv::RNG::UNIFORM, cv::Scalar(0,0,0), cv::Scalar(1,1,1));
	//rng.fill(A, cv::RNG::UNIFORM, cv::Scalar(0), cv::Scalar(1));

	std::cout << "\npixel[0,0] from A:\n" << pixel[0] << "," << pixel[1] << "," << pixel[2] << std::endl;
	cv::split(A, M);
	PRINTMAT_F(M[0], NPF);
	PRINTMAT_F(M[1], NPF);
	PRINTMAT_F(M[2], NPF);

	cv::Mat out;
	cv::merge(M, out);
	CV_MAT_INFO(out);

	int total = 6;
	std::for_each(std::begin(M), std::end(M), [&total](cv::Mat &m)
	{
		m = 255.f*m / total;
		std::partial_sum(m.begin<float>(), m.end<float>(), m.begin<float>());
	});
	PRINTMAT_F(M[0], NPF);
	PRINTMAT_F(M[1], NPF);
	PRINTMAT_F(M[2], NPF);
}

static void histogram_eq_withLUT(const cv::String &img_path)
{
	cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
	CV_Assert(!img.empty());

	int total = img.cols*img.rows;

	std::vector<cv::Mat> imgVec;
	CV_INIT_VECTOR(imgVec,img.channels(),cv::Mat::zeros(img.size(),img.depth()));
	cv::split(img, imgVec);

	std::vector<cv::Mat> bgr;
	CV_INIT_VECTOR(bgr, 3, cv::Mat::zeros(1, 256, CV_32F));

	for (int i=0;i<img.rows;++i)
	{

		for (int j = 0; j < img.cols; ++j)
		{
			cv::Vec3b &pixel = img.at<cv::Vec3b>(i, j);

			++bgr[0].at<float>(pixel[0]);
			++bgr[1].at<float>(pixel[1]);
			++bgr[2].at<float>(pixel[2]);
		}
	}

	std::vector<cv::Mat> histogram_eq;
	CV_INIT_VECTOR(histogram_eq, 3, cv::Mat::zeros(img.size(), CV_32F));
	std::vector<cv::Mat> histogram_eq_builtin;
	CV_INIT_VECTOR(histogram_eq_builtin, 3, cv::Mat::zeros(img.size(), CV_8U));

	for (size_t i=0;i<bgr.size();++i)
	{
		bgr[i] = 255.f*bgr[i] / total;
		std::partial_sum(bgr[i].begin<float>(), bgr[i].end<float>(), bgr[i].begin<float>());
		cv::LUT(imgVec[i],bgr[i],histogram_eq[i]);
		//compare with built-in eqHist
		cv::equalizeHist(imgVec[i], histogram_eq_builtin[i]);
	}

	cv::Mat dst,dst2;
	cv::merge(histogram_eq,dst);
	CV_MAT_INFO(dst);
	cv::Mat out;
	dst.convertTo(out, CV_8UC3);
	CV_MAT_INFO(out);
	cv::imshow("src", img);
	cv::imshow("histogram-eq", out);
	cv::merge(histogram_eq_builtin, dst2);
	cv::imshow("histogram-eq(built-in)", dst2);


	cv::waitKey();
}


int main_he_lut(_MAIN_ARGS)
{
	//ex();
	//return 0;
	char *name = (argc == 2) ? argv[1] : "lena";
	cv::String img_path;
	cv_helper::get_imgPathEx(name, img_path);
	CV_TRY_CATCH(histogram_eq_withLUT(img_path));
	return 0;
}