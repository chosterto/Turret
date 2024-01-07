#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/aruco.hpp>
#include <stdint.h>
#include <vector>

#define CENTER cv::Point(320, 240)
#define CAMERA_PERIOD_SEC 0.03333

using namespace std;


typedef struct Aruco {

	uint8_t id;
	uint8_t size;
	double dim;
	double searchRegionPercent;
	double maxBlack;
	double minWhite;

	cv::Point2f corners[4];

} Aruco;


uint16_t scale(
	uint16_t in,
	uint16_t min,
	uint16_t max,
	uint16_t min_prime,
	uint16_t max_prime);


void inRangeHSVPercent(cv::Mat* img, cv::Scalar s1, cv::Scalar s2, cv::Mat* img2);


int32_t findLargestContour(const cv::Mat& img, vector< vector<cv::Point> >* contours);


void distanceFromCenter(cv::Point* distance, const vector<cv::Point>& points);


void findArUco(const cv::Mat& img, Aruco* aruco);


void filterPoly(const vector< vector<cv::Point> >& points, vector< vector<cv::Point> >& candidates);


void orderCorners(const vector<cv::Point>& p, cv::Point* tl, cv::Point* tr, cv::Point* bl, cv::Point* br);


bool verifyMarker(const cv::Mat& img, Aruco* aruco);

#endif
