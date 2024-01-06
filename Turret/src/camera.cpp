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


void inRangeHSVPercent(cv::Mat* img, cv::Scalar s1, cv::Scalar s2, cv::Mat* img2)
{
	s1[0] = scale(s1[0], 0, 360, 0, 179);
	s1[1] = scale(s1[1], 0, 100, 0, 255);
	s1[2] = scale(s1[2], 0, 100, 0, 255);

	s2[0] = scale(s2[0], 0, 360, 0, 179);
	s2[1] = scale(s2[1], 0, 100, 0, 255);
	s2[2] = scale(s2[2], 0, 100, 0, 255);

	cv::inRange(*img, s1, s2, *img2);
}


int32_t findLargestContour(const cv::Mat& img, vector< vector<cv::Point> >* contours)
{
	cv::findContours(img, *contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	int32_t largest_idx = -1;

	double area, largest_contour = 30.0;

	for (int32_t i = 0; i < contours->size(); i++)
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


void distanceFromCenter(cv::Point* distance, const vector<cv::Point>& points)
{
	cv::Point centroid(0.0, 0.0);
	uint16_t n = points.size();

	for (uint16_t i = 0; i < n; i++)
		centroid += points[i];

	centroid /= n;

	*distance = (CENTER - centroid);
}


void findArUco(const cv::Mat& img, Aruco* aruco)
{
	cv::Mat thres, gs;
	cv::cvtColor(img, gs, cv::COLOR_BGR2GRAY);
	cv::adaptiveThreshold(
		gs, thres, 40, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 13, 5
	);

	vector< vector<cv::Point> > contours, potential_markers;

	cv::findContours(thres, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	filterShapes(contours, potential_markers);

	for (int i = 0; i < potential_markers.size(); i++) {
		for (int j = 0; j < 4; j++)
			cv::circle(img, potential_markers[i][j], 4, cv::Scalar(255, 0, 0), 2);
	}
	cv::imshow("img", img);
	cv::waitKey(0);
}


void filterShapes(const vector< vector<cv::Point> >& points, vector< vector<cv::Point> >& candidates)
{
	vector<cv::Point> curve_f;
	double ep = 1.5;
	for (uint16_t i = 0; i < points.size(); i++)
	{
		if (cv::contourArea(points[i]) < 30) continue;

		cv::approxPolyDP(points[i], curve_f, ep, true);
		if (curve_f.size() == 4)
			candidates.push_back(curve_f);
	}
}
