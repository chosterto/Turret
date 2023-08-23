#include <iostream>
#include "servo.h"
#include "camera.h"

int main()
{
	cv::Mat frame;
	cv::VideoCapture cap;

	cap.open(0, cv::CAP_ANY);
	if (!cap.isOpened())
	{
		std::cout << "Unable to open camera\n";
		return -1;
	}

	cap.read(frame);
	cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV);
	//cv::inRange(frame, cv::Scalar(45, 100, 100), cv::Scalar(85, 255, 255), frame);
	inRangeHSVPercent(&frame, cv::Scalar(100, 30, 30), cv::Scalar(150, 100, 100));
	cv::imshow("Testing", frame);
	cv::waitKey(0);

	return 0;
}
