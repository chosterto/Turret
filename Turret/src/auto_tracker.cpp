#include <iostream>
#include <signal.h>
#include "servo.h"
#include "camera.h"


ServoCR servo_base, servo_top;

void terminate(int s)
{
	cout << "\nSTOPPING MOTORS" << endl;
	setPower(&servo_base, 0.0);
	setPower(&servo_top, 0.0);
	exit(1);
}

int main()
{
	cv::Mat frame, raw_frame;
	cv::VideoCapture cap;
	vector< vector<cv::Point> > contours;
	cv::Point dist;
	double x_speed, y_speed;


	signal(SIGINT, terminate);

	if (wiringPiSetup() == -1)
		exit(1);

	servoInitialize(&servo_base, &servo_top, 150, 153);

	cap.open(0, cv::CAP_ANY);
	if (!cap.isOpened())
	{
		cout << "Unable to open camera\n";
		return -1;
	}

	while (1)
	{
		cap.read(raw_frame);
		cv::cvtColor(raw_frame, frame, cv::COLOR_BGR2HSV);
		inRangeHSVPercent(&frame, cv::Scalar(90, 30, 30), cv::Scalar(150, 100, 100));
		//cv::findContours(frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		int32_t target = findLargestContour(frame, &contours);
		if (target < 0)
		{
			setPower(&servo_base, 0.0);
			setPower(&servo_top, 0.0);
			continue;
		}

		distanceFromCenter(&dist, contours[target]);
		x_speed = dist.x * 0.001;
		y_speed = dist.y * -0.001;

		setPower(&servo_base, x_speed);
		setPower(&servo_top, y_speed);

		delay(1);
	}
	return 0;
}
