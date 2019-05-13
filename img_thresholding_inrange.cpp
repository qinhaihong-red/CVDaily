#include "cv_helper.h"

static void videocapture_ex()
{
	cv::VideoCapture cap(0);
	cv::Mat input;
	cap >> input;
	cv::imshow("capture", input);
	cv::waitKey();
}


static int max_val = 255;
static int max_val_H = 360 / 2;
static const char *wnd_cap_name = "Video Capture";
static const char *wnd_object_detection = "Object Detection";
static int low_H = 0; static int high_H = max_val_H;
static int low_S = 0; static int high_S = max_val;
static int low_V = 0; static int high_V = max_val;

static void low_H_callback(int, void *)
{
	low_H = cv::min(high_H-1,low_H);
	cv::setTrackbarPos("LowH", wnd_object_detection, low_H);
}

static void high_H_callback(int, void *)
{
	high_H = cv::max(high_H,low_H+1);
	cv::setTrackbarPos("HighH", wnd_object_detection, high_H);
}

static void low_S_callback(int, void *)
{
	low_S = cv::min(high_S-1,low_S);
	cv::setTrackbarPos("LowS", wnd_object_detection, low_S);
}

static void high_S_callback(int, void *)
{
	high_S = cv::max(high_S, low_S + 1);
	cv::setTrackbarPos("HighS", wnd_object_detection, high_S);
}

static void low_V_callback(int, void *)
{
	low_V = cv::min(high_V-1,low_V);
	cv::setTrackbarPos("LowV", wnd_object_detection, low_V);
}

static void high_V_callback(int, void *)
{
	high_V = cv::max(high_V,low_V+1);
	cv::setTrackbarPos("HighV", wnd_object_detection, high_V);
}

static void thresholding_inrange()
{
	cv::namedWindow(wnd_cap_name, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(wnd_object_detection, cv::WINDOW_AUTOSIZE);

	cv::VideoCapture vc(0);

	cv::createTrackbar("LowH", wnd_object_detection, &low_H, max_val_H, low_H_callback);
	cv::createTrackbar("HighH", wnd_object_detection, &high_H, max_val, high_H_callback);
	cv::createTrackbar("LowS", wnd_object_detection, &low_S, max_val, low_S_callback);
	cv::createTrackbar("HighS", wnd_object_detection, &high_S, max_val, high_S_callback);
	cv::createTrackbar("LowV", wnd_object_detection, &low_V, max_val, low_V_callback);
	cv::createTrackbar("HighV", wnd_object_detection, &high_V, max_val, high_V_callback);

	cv::Mat frame, frameHSV, frame_threshold;
	while (true)
	{
		vc >> frame;
		CV_Assert(!frame.empty());
		cv::cvtColor(frame, frameHSV, cv::COLOR_BGR2HSV);
		cv::inRange(frameHSV, cv::Scalar(low_H, low_S, low_V),cv::Scalar(high_H,high_S,high_V),frame_threshold);
		cv::imshow(wnd_cap_name, frame);
		cv::imshow(wnd_object_detection, frame_threshold);

		char key = (char)cv::waitKey(30);
		if (key=='q'||key==27)break;

	}
}

int main_thresholding_inrange()
{
	CV_TRY_CATCH(thresholding_inrange());
	return 0;
}