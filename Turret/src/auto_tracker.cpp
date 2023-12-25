#include <iostream>
#include <signal.h>
#include <chrono>
#include "servo.h"
#include "camera.h"


ServoCR servo_base, servo_top;
PID pid1 = {.K_p=0.001, .K_i=0.0005, .K_d=0.0};
PID pid2 = {.K_p=0.0, .K_i=0.0, .K_d=0.0};

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

	typedef std::chrono::high_resolution_clock clock;
	typedef std::chrono::duration<float> d_secs;
	auto t0 = clock::now();
	auto t1 = clock::now();
	d_secs deltaTime;


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

		t1 = clock::now();

		deltaTime = t1 - t0;
		distanceFromCenter(&dist, contours[target]);
		computePID(dist.x, &x_speed, deltaTime.count(), &pid1);
		computePID(dist.y, &y_speed, deltaTime.count(), &pid2);

		setPower(&servo_base, x_speed);
		setPower(&servo_top, y_speed);

		t0 = t1;
		delay(1);
	}
	return 0;
}
