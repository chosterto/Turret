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

	inRange(*img, s1, s2, *img);
}


uint16_t findLargestContour(const cv::Mat& img, vector< vector<cv::Point> >* contours)
{
	cv::findContours(img, *contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	uint16_t largest_idx;

	double area, largest_contour;

	for (uint16_t i = 0; i < contours->size(); i++)
	{
		area = cv::contourArea(contours->at(i));
		if (largest_contour < area)
		{
			largest_contour = area;
			largest_idx = i;
		}
	}

	return largest_idx;
}


