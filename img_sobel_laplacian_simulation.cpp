#include "cv_helper.h"


static void sobel_simulation()
{
	cv::Mat m = (cv::Mat_<uchar>(3, 3) << \
		1, 2, 5,
		4, 5, 11,
		7, 8, 9);

	cv::Mat dst;
	PRINTMAT_F(m, NPF);

	//1. first order derivatives in x direction:
	//1.1 using built-in Sobel function
	cv::Sobel(m, dst, CV_16S, 1, 0);
	PRINTMAT_F(dst, NPF);
	
	//1.2 using filter2d with explicit sobel_x kernel. have same result as 1.1
	cv::Mat sobel_x = (cv::Mat_<schar>(3,3)<< \
		-1,0,1,
		-2,0,2,
		-1,0,1);
	cv::Mat x_out;
	cv::filter2D(m, x_out, CV_16S, sobel_x);
	PRINTMAT_F(x_out,NPF);
	
	//2.first order derivatives in y direction
	//2.1 using built-in Sobel function
	cv::Sobel(m, dst, CV_16S, 0, 1);
	PRINTMAT_F(dst, NPF);
	//2.2 using filter2d with explicit sobel_y kernel. have same result as 2.1
	cv::Mat sobel_y = (cv::Mat_<schar>(3, 3) << \
	   -1, -2, -1,
		0,   0,  0,
		1,   2,  1);
	cv::Mat y_out;
	cv::filter2D(m, y_out, CV_16S, sobel_y);
	PRINTMAT_F(y_out, NPF);


	//3. laplacian operator
	//3.1 built-in Laplacian function has a default kernel with sz=1 ,
	//while it uses the typical 3x3 kernel in 3.2.
	//a little confused.
	cv::Laplacian(m, dst, CV_16S);
	PRINTMAT_F(dst, NPF);

	//3.2 explicit kernel as below has a kernel with sz=3,results same effect as 3.1
	cv::Mat lap_kernel = (cv::Mat_<schar>(3, 3) << \
		0, 1, 0,
		1, -4, 1,
		0, 1, 0);
	cv::Mat lap_out;
	cv::filter2D(m, lap_out, CV_16S, lap_kernel);
	PRINTMAT_F(lap_out,NPF);

	//3.3 while explicitly using sz=3 in Laplacian, 
	//will lead a different result comparing with 3.1 and 3.2
	cv::Laplacian(m, dst, CV_16S,3);
	PRINTMAT_F(dst, NPF);

	//4 second-order derivatives 
	//4.1.1 in x direction: using Sobel
	cv::Mat grad2_x;
	cv::Sobel(m, grad2_x, CV_16S, 2, 0);
	PRINTMAT_F(grad2_x,NPF);
	//4.1.2 in x direction:using explicit kernel, result same as 4.1.1
	cv::Mat sobel_x2 = (cv::Mat_<schar>(3, 3) << \
		1, -2, 1,
		2, -4, 2,
		1, -2, 1);
	cv::Mat x2_out;
	cv::filter2D(m, x2_out, CV_16S, sobel_x2);
	PRINTMAT_F(x2_out, NPF);

	//4.2.1 in y direction:using Sobel
	cv::Mat grad2_y;
	cv::Sobel(m,grad2_y,CV_16S,0,2);
	PRINTMAT_F(grad2_y, NPF);

	//4.2.2 in y direction:using explicit kernel, result same as 4.2.1
	cv::Mat sobel_y2 = (cv::Mat_<schar>(3, 3) << \
		1, 2, 1,
		-2, -4, -2,
		1, 2, 1);
	cv::Mat y2_out;
	cv::filter2D(m, y2_out, CV_16S, sobel_y2);
	PRINTMAT_F(y2_out, NPF);


	//4.3 the result of grad2_xy(sum of x2+y2) eq2 Laplacian with sz=3 in 3.3
	cv::Mat grad2_xy = grad2_x + grad2_y;
	PRINTMAT_F(grad2_xy, NPF);

}

static void laplacian_operator_comparison()
{
	//compare effects of different laplacian operator
	cv::String img_path;
	cv_helper::get_imgPathEx("lena", img_path);
	cv::Mat img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	CV_Assert(!img.empty());
	cv::imshow("src", img);
	cv::GaussianBlur(img, img, cv::Size(3, 3), 0);
	cv::imshow("after gauss-blur",img);
	//default kernel sz=1, but it uses the classical 3x3 kernel
	cv::Mat k1out,k1out_abs;
	cv::Laplacian(img, k1out, CV_16S);
	cv::convertScaleAbs(k1out, k1out_abs);
	cv::imshow("Laplacian with kernel=1", k1out_abs);
	//specify kernel sz=3 
	cv::Mat k3out, k3out_abs;
	cv::Laplacian(img, k3out, CV_16S,3);
	cv::convertScaleAbs(k3out, k3out_abs);
	cv::imshow("Laplacian with kernel=3", k3out_abs);

	cv::Mat grad2_x, grad2_y;
	cv::Sobel(img, grad2_x, CV_16S, 2, 0);
	cv::Sobel(img, grad2_y, CV_16S, 0, 2);
	cv::Mat grad2_xy = grad2_x + grad2_y;
	//compare grad2_xy with k3out
	cv::Mat com = grad2_xy != k3out;
	bool eq = cv::countNonZero(com)==0;
	//should be true
	std::cout << std::boolalpha << eq << std::endl;
	
	cv::Mat grad2_out;
	cv::convertScaleAbs(grad2_xy, grad2_out);
	cv::imshow("Laplacian with Sobel", grad2_out);

	cv::waitKey();
}


int main_sobel_laplacian_ex()
{

	CV_TRY_CATCH(sobel_simulation());
	CV_TRY_CATCH(laplacian_operator_comparison());
	return 0;
}