#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdint.h>


uint16_t scale(
	uint16_t in,
	uint16_t min,
	uint16_t max,
	uint16_t min_prime,
	uint16_t max_prime);


void inRangeHSVPercent(cv::Mat* img, cv::Scalar s1, cv::Scalar s2);

#endif
