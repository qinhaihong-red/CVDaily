#include "cv_helper.h"

//dft lena then idft to recover it
void func_idft1()
{
	cv::String img_path;
	CV_Assert(cv_helper::get_imgPathEx("lena", img_path));
	cv::Mat m_real = cv::imread(img_path, cv::IMREAD_GRAYSCALE);//must be grayscale...
	CV_Assert(!m_real.empty());
	
	cv::Mat padded;
	int m = cv::getOptimalDFTSize(m_real.rows);
	int n = cv::getOptimalDFTSize(m_real.cols);

	cv::copyMakeBorder(m_real, padded, 0, m - m_real.cols, 0, n - m_real.rows, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	
	
	cv::Mat m_img = cv::Mat::zeros(padded.size(), CV_32FC1);
	
	cv::Mat complex;
	cv::Mat _m[] = {cv::Mat_<float>(padded),m_img};
	cv::merge(_m, 2, complex);
	cv::dft(complex,complex);

	cv::Mat inverse_complex;
	cv::idft(complex, inverse_complex,  cv::DFT_REAL_OUTPUT);
	cv::normalize(inverse_complex, inverse_complex, 0, 1, cv::NORM_MINMAX);
	
	cv::Mat inverse_complex_uchar;
	inverse_complex.convertTo(inverse_complex_uchar, CV_8UC1,255);

	cv::imshow("src", m_real);
	cv::imshow("complex", inverse_complex_uchar);
	cv::waitKey();

}

//dft the image,then show the magnitude of image
void func_dft(cv::Mat i)
{
	std::cerr.rdbuf(std::cout.rdbuf());

	cv::Mat img = i.clone();
	cv::Mat padded;
	int m = cv::getOptimalDFTSize(img.rows);
	int n = cv::getOptimalDFTSize(img.cols);

	cv::copyMakeBorder(img, padded, 0, m - img.rows, 0, n - img.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::Mat _m(padded.size(), CV_32FC1);
	padded.convertTo(_m, CV_32FC1);
	cv::Mat planes[] = { cv::Mat_<float>(padded),cv::Mat::zeros(padded.size(),CV_32FC1) };
	cv::Mat complex;
	cv::merge(planes, 2, complex);

	std::cout << std::boolalpha << (complex.type() == CV_32FC1) << std::endl;
	std::cout << complex.type() << std::endl;
	cv::dft(complex, complex);



	cv::split(complex, planes);
	cv::magnitude(planes[0], planes[1], planes[0]);
	cv::Mat mag = planes[0];

	cv::Mat mag_without_log = mag.clone();
	double _min = 0, _max = 0;
	cv::minMaxLoc(mag_without_log, &_min, &_max);
	CV_Assert(_max > 0/* && _max > 255.f*/);
	mag_without_log = 255.f*mag_without_log / _max;

	mag += cv::Scalar::all(1);
	log(mag, mag);
	cv::Mat mag_clone = mag.clone();

	auto func_quadrants_switch = [](cv::Mat &mag)
	{
		mag = cv::Mat(mag, cv::Rect(0, 0, mag.cols&(-2), mag.rows&(-2)));
		int cx = mag.cols / 2;
		int cy = mag.rows / 2;

		cv::Mat q0(mag, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
		cv::Mat q1(mag, cv::Rect(cx, 0, cx, cy));  // Top-Right
		cv::Mat q2(mag, cv::Rect(0, cy, cx, cy));  // Bottom-Left
		cv::Mat q3(mag, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
		cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
		q0.copyTo(tmp);
		q3.copyTo(q0);
		tmp.copyTo(q3);
		q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
		q2.copyTo(q1);
		tmp.copyTo(q2);
	};

	func_quadrants_switch(mag);
	func_quadrants_switch(mag_without_log);


	cv::normalize(mag, mag, 0, 1, cv::NORM_MINMAX);
	cv::normalize(mag_clone, mag_clone, 0, 1, cv::NORM_MINMAX);

	//std::ofstream of("without_log.txt");
	//std::cerr.rdbuf(of.rdbuf());
	//PRINTMAT_F(mag_without_log,NPF);
	cv::Mat mag_without_log_gray(mag_without_log.size(), CV_8UC1);
	mag_without_log.convertTo(mag_without_log_gray, CV_8UC1);
	cv::normalize(mag_without_log_gray, mag_without_log_gray, 0, 255, cv::NORM_MINMAX);

	cv::Mat mag_gray(mag.size(), CV_8UC1);
	mag.convertTo(mag_gray, CV_8UC1, 255);

	cv::Mat mag_clone_gray(mag_clone.size(), CV_8UC1);
	mag_clone.convertTo(mag_clone_gray, CV_8UC1, 255);

	cv::imwrite("dft_outxxx.png", mag_gray);

	cv::imshow("src", img);
	cv::imshow("shifted without log", mag_without_log_gray);
	cv::imshow("no-shifted with log", mag_clone_gray);
	cv::imshow("shifted with log", mag_gray);

}

//create a simple fourier image,then idft it to the intensity image
void func_idft2()
{

	auto func_show_corner_value = [](cv::Mat &m)
	{
		float *pf = m.ptr<float>(0);
		float tl = *pf;
		float tr = *(pf + m.cols - 1);
		pf = m.ptr<float>(m.rows-1);
		float bl = *pf;
		float br = *(pf + m.cols - 1);

		printf("rows:%d, cols:%d, corner-vals:[%f,%f,%f,%f]\n",m.rows,m.cols,tl,tr,bl,br);
	};

	cv::Mat m_real = cv::Mat::zeros(800, 800, CV_32FC1);
	std::cout << "before padded:\n";
	func_show_corner_value(m_real);

	cv::Mat padded;
	int m = cv::getOptimalDFTSize(m_real.rows);
	int n = cv::getOptimalDFTSize(m_real.cols);

	cv::copyMakeBorder(m_real, padded, 0, m - m_real.cols, 0, n - m_real.rows, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	std::cout << "after padded:\n";
	func_show_corner_value(padded);

	cv::Mat m_img = cv::Mat::zeros(padded.size(), CV_32FC1);

	float *pf = padded.ptr<float>(0);
	//dc-value,no side-effect without setting it
	//*pf = 1000000;
	
	//x-axis frequency
	*(pf + 50) = 255;

	//y-axis frequency
	//pf= padded.ptr<float>(50);
	//*pf = 255;

	//diagnal x-y requency
	//*(pf + 50) = 255;

	cv::Mat complex;
	cv::Mat _m[] = { cv::Mat_<float>(padded),m_img };
	cv::merge(_m, 2, complex);
	//cv::dft(complex, complex);

	cv::Mat inverse_complex;
	cv::idft(complex, inverse_complex, cv::DFT_REAL_OUTPUT);
	cv::normalize(inverse_complex, inverse_complex, 0, 1, cv::NORM_MINMAX);

	cv::Mat inverse_complex_uchar;
	inverse_complex.convertTo(inverse_complex_uchar, CV_8UC1, 255);

	func_dft(inverse_complex_uchar);
	cv::imshow("complex", inverse_complex_uchar);
	cv::waitKey();
}

int main_idft()
{
	//CV_TRY_CATCH(func_idft());
	//custom-def 
	CV_TRY_CATCH(func_idft2());

	return 0;
}