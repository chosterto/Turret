#include <iostream>
#include "servo.h"
#include "camera.h"

int main()
{
	cv::Mat frame, raw_frame;
	cv::VideoCapture cap;
	vector< vector<cv::Point> > contours;

	cap.open(0, cv::CAP_ANY);
	if (!cap.isOpened())
	{
		cout << "Unable to open camera\n";
		return -1;
	}

	cap.read(raw_frame);
	cv::cvtColor(raw_frame, frame, cv::COLOR_BGR2HSV);
	//inRange(frame, cv::Scalar(45, 100, 100), cv::Scalar(85, 255, 255), frame);
	inRangeHSVPercent(&frame, cv::Scalar(90, 30, 30), cv::Scalar(150, 100, 100));
	//findContours(frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	uint16_t i = findLargestContour(frame, &contours);
	for (int j = 0; j < contours.size(); j++)
		cout << j << " " <<  contours[j].size() << endl;
	cout << i << endl;
	cv::drawContours(raw_frame, contours, i, cv::Scalar(255, 0, 0), 5);
	cv::imshow("Testing",raw_frame);
	cv::waitKey(0);

	return 0;
}
