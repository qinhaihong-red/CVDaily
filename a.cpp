#include<iostream>
#include"opencv2/opencv.hpp"
#include"opencv2/core/types_c.h"
#include"opencv2/highgui.hpp"

int main()
{
    
    cv::Mat img = cv::imread("/home/hh/images/jordan.jpg", cv::IMREAD_COLOR);
    cv::imshow("test",img);
    cv::waitKey();

    return 0;

}