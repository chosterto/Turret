#include <iostream>
#include <signal.h>
#include <chrono>
#include "servo.h"
#include "camera.h"


ServoCR servo_base, servo_top;
PID pid1 = {.K_p=0.001, .K_i=0.0005, .K_d=0.00001};
PID pid2 = {.K_p=-0.001, .K_i=-0.0001, .K_d=0.0};

Aruco marker;

void terminate(int s)
{
	cout << "\nSTOPPING MOTORS" << endl;
	setPower(&servo_base, 0.0);
	setPower(&servo_top, 0.0);
	exit(1);
}


int main(int argc, char** argv)
{
	cv::Mat frame, raw_frame;
	cv::VideoCapture cap;
	cv::Point dist;
	double x_speed, y_speed;

	typedef chrono::high_resolution_clock clock;
	typedef chrono::duration<float> fsecs;
	auto t0 = clock::now();
	auto t1 = clock::now();
	fsecs deltaTime;
	fsecs camera_fps(CAMERA_PERIOD_SEC);


	signal(SIGINT, terminate);

	if (wiringPiSetup() == -1)
		exit(1);

	servoInitialize(&servo_base, &servo_top, 150, 153);


	cap.open(0, cv::CAP_ANY);
	if (!cap.isOpened())
	{
		cout << "Unable to open camera\n";
		exit(1);
	}

	delay(1000);

	cap.read(raw_frame);
	findArUco(raw_frame, &marker);
	/**
	while (1)
	{
		cap.read(raw_frame);

		t1 = clock::now();

		deltaTime = t1 - t0;
		//distanceFromCenter(&dist, contours[target]);
		computePID(dist.x, &x_speed, (deltaTime + camera_fps).count(), &pid1, 1.0);
		computePID(dist.y, &y_speed, (deltaTime + camera_fps).count(), &pid2, 0.75);

		setPower(&servo_base, x_speed);
		setPower(&servo_top, y_speed);

		t0 = t1;
	}
	**/
	return 0;
}
