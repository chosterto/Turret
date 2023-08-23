#include <iostream>
#include "camera.h"


uint16_t scale(uint16_t in, uint16_t min, uint16_t max, uint16_t min_prime, uint16_t max_prime)
{
	if (in <= min)
		return min_prime;
	if (in >= max)
		return max_prime;

	return (uint16_t)round((max_prime - min_prime)*(in - min)/(max - min) + min_prime);
}


void inRangeHSVPercent(cv::Mat* img, cv::Scalar s1, cv::Scalar s2)
{
	s1[0] = scale(s1[0], 0, 360, 0, 179);
	s1[1] = scale(s1[1], 0, 100, 0, 255);
	s1[2] = scale(s1[2], 0, 100, 0, 255);

	s2[0] = scale(s2[0], 0, 360, 0, 179);
	s2[1] = scale(s2[1], 0, 100, 0, 255);
	s2[2] = scale(s2[2], 0, 100, 0, 255);

	cv::inRange(*img, s1, s2, *img);
}


