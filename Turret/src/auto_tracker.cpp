#include <iostream>
#include "servo.h"
#include "camera.h"

int main()
{
	cv::Mat image1 = cv::imread("../../media/test2.jpg");
	cv::Mat image2;
	cv::resize(image1, image2, cv::Size(image1.cols * 0.25, image1.rows * 0.25), 0, 0, CV_INTER_LINEAR) ;

	cv::imshow("Testing", image2);
	cv::waitKey(0);
	return 0;
}
