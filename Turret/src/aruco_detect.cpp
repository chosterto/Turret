#include <iostream>
#include <signal.h>
#include <string>
#include <wiringPi.h>
#include "camera.h"


using namespace std;


int main(int argc, char** argv)
{
	cv::Mat frame;
	cv::VideoCapture cap;
	Aruco markers;

	arucoInitialize(&markers, 4, 360.0, 0.3, 60, 110, 0.1);

	cap.open(0, cv::CAP_ANY);
	if (!cap.isOpened())
	{
		cout << "Unable to open camera\n";
		exit(1);
	}

	delay(2000);

	cap.read(frame);

	findArUco(frame, &markers);

	if (!markers.isDetected)
	{
		cout << "Cannot find Aruco markers" << endl;
	}

	cv::String id_name;
	for (int i = 0; i < markers.ids.size(); i++)
	{
		cv::line(
			frame,
			markers.corners[i][0],
			markers.corners[i][1],
			cv::Scalar(255, 0, 0), 1
		);
		cv::line(
			frame,
			markers.corners[i][1],
			markers.corners[i][3],
			cv::Scalar(255, 0, 0), 1
		);
		cv::line(
			frame,
			markers.corners[i][2],
			markers.corners[i][3],
			cv::Scalar(255, 0, 0), 1
		);
		cv::line(
			frame,
			markers.corners[i][2],
			markers.corners[i][0],
			cv::Scalar(255, 0, 0), 1
		);

		id_name = to_string(markers.ids[i]);
		cv::putText(
			frame,
			"ID:" + id_name,
			markers.corners[i][1],
			cv::FONT_HERSHEY_PLAIN, 1.0,
			cv::Scalar(255, 0, 0), 2
		);
	}

	cv::imshow("img", frame);
	cv::waitKey(0);
	return 0;
}
