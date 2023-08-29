#include <iostream>
#include "servo.h"
#include "camera.h"

int main()
{
	cv::Mat frame, raw_frame;
	cv::VideoCapture cap;
	vector< vector<cv::Point> > contours;
	cv::Point dist;

	cap.open(0, cv::CAP_ANY);
	if (!cap.isOpened())
	{
		cout << "Unable to open camera\n";
		return -1;
	}

	cap.read(raw_frame);
	cv::cvtColor(raw_frame, frame, cv::COLOR_BGR2HSV);
	//cv::inRange(frame, cv::Scalar(45, 100, 100), cv::Scalar(85, 255, 255), frame);
	inRangeHSVPercent(&frame, cv::Scalar(90, 30, 30), cv::Scalar(150, 100, 100));
	//cv::findContours(frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	uint16_t target = findLargestContour(frame, &contours);
	distanceFromCenter(&dist, contours[target]);
	cout << dist.x << " " << dist.y << endl;
	cv::drawContours(raw_frame, contours, target, cv::Scalar(255, 0, 0), 5);
	cv::imshow("Testing",raw_frame);
	cv::waitKey(0);

	return 0;
}
