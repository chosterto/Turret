#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <stdint.h>
#include <vector>

using namespace std;

uint16_t scale(
	uint16_t in,
	uint16_t min,
	uint16_t max,
	uint16_t min_prime,
	uint16_t max_prime);


void inRangeHSVPercent(cv::Mat* img, cv::Scalar s1, cv::Scalar s2);

uint16_t findLargestContour(const cv::Mat& img, vector< vector<cv::Point> >* contours);

#endif
