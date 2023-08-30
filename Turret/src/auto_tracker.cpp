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

	while (1) {
		cap.read(raw_frame);
		cv::cvtColor(raw_frame, frame, cv::COLOR_BGR2HSV);
		inRangeHSVPercent(&frame, cv::Scalar(90, 30, 30), cv::Scalar(150, 100, 100));
		cv::findContours(frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		int32_t target = findLargestContour(frame, &contours);
		if (target >= 0)
		{
			distanceFromCenter(&dist, contours[target]);
			cout << dist.x << " " << dist.y << endl;
		}
		//cv::drawContours(raw_frame, contours, target, cv::Scalar(255, 0, 0), 5);
		//cv::imshow("Testing",raw_frame);
		//cv::waitKey(0);
	}
	return 0;
}
