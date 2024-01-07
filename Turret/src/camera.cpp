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
	uint8_t marker_size;
	switch(aruco->size)
	{
		case 4:
			marker_size = cv::aruco::DICT_4X4_250; break;
		case 5:
			marker_size = cv::aruco::DICT_5X5_250; break;
		case 6:
			marker_size = cv::aruco::DICT_6X6_250; break;
		case 7:
			marker_size = cv::aruco::DICT_7X7_250; break;
		default:
			marker_size = cv::aruco::DICT_4X4_250; break;
	}

	cv::Mat thres, gs;
	cv::cvtColor(img, gs, cv::COLOR_BGR2GRAY);
	cv::adaptiveThreshold(
		gs, thres, 40, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 13, 5
	);

	vector< vector<cv::Point> > contours, potential_markers;
	cv::findContours(thres, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	filterPoly(contours, potential_markers);

	uint16_t i;
	cv::Point TL, TR, BL, BR;
	cv::Mat p_mat, marker_img;
	for (i = 0; i < potential_markers.size(); i++)
	{
		orderCorners(potential_markers[i], &TL, &TR, &BL, &BR);

		cv::Point2f corners[4] = {TL, TR, BL, BR};
		cv::Point2f target[4] = {
			cv::Point(0, 0),
			cv::Point(aruco->dim, 0),
			cv::Point(0, aruco->dim),
			cv::Point(aruco->dim, aruco->dim)
		};

		p_mat = cv::getPerspectiveTransform(corners, target);

		cv::warpPerspective(gs, marker_img, p_mat, cv::Size(aruco->dim, aruco->dim));
		//cv::circle(img, TL, 2, cv::Scalar(255, 0, 0), 2);
		//cv::circle(img, TR, 2, cv::Scalar(0, 255, 0), 2);
		//cv::circle(img, BL, 2, cv::Scalar(0, 0, 255), 2);
		//cv::circle(img, BR, 2, cv::Scalar(255, 0, 255), 2);
		//cv::imshow("img", marker_img);
		//cv::waitKey(0);
		verifyMarker(marker_img, aruco);
	}
}


void filterPoly(const vector< vector<cv::Point> >& points, vector< vector<cv::Point> >& candidates)
{
	vector<cv::Point> curve_f;
	double ep = 1.5;
	for (uint16_t i = 0; i < points.size(); i++)
	{
		if (cv::contourArea(points[i]) < 50) continue;

		cv::approxPolyDP(points[i], curve_f, ep, true);
		if (curve_f.size() == 4)
			candidates.push_back(curve_f);
	}
}


void orderCorners(const vector<cv::Point>& p,
		  cv::Point* tl, cv::Point* tr, cv::Point* bl, cv::Point* br)
{
	cv::Point dP0 = p[1] + p[2] + p[3] - 3*p[0];
	cv::Point dP1 = p[0] + p[2] + p[3] - 3*p[1];
	cv::Point dP2 = p[1] + p[0] + p[3] - 3*p[2];
	cv::Point dP3 = p[1] + p[2] + p[0] - 3*p[3];
	vector<cv::Point> dP{dP0, dP1, dP2, dP3};

	for (uint8_t i = 0; i < 4; i++)
	{
		if      (dP[i].x >= 0 && dP[i].y > 0) *tl = p[i];
		else if (dP[i].x > 0 && dP[i].y <= 0) *bl = p[i];
		else if (dP[i].x <= 0 && dP[i].y > 0) *tr = p[i];
		else if (dP[i].x < 0 && dP[i].y <= 0) *br = p[i];
	}
}


bool verifyMarker(const cv::Mat& img, Aruco* aruco)
{
	cv::Mat bits(aruco->size, aruco->size, CV_8U), region;

	uint16_t bitSize = (aruco->dim)/(aruco->size + 2);
	double   avg;

	for (uint8_t i = 0; i < (aruco->size)*(aruco->size); i++)
	{
		cv::Point p1(bitSize*(i%aruco->size+1) + bitSize/2 * (1-aruco->searchRegionPercent),
			     bitSize*(i/aruco->size+1) + bitSize/2 * (1-aruco->searchRegionPercent));
		cv::Point p2(bitSize*(i%aruco->size+1) + bitSize/2 * (1+aruco->searchRegionPercent),
			     bitSize*(i/aruco->size+1) + bitSize/2 * (1+aruco->searchRegionPercent));
		cv::rectangle(img, p1,p2, cv::Scalar(255,0,0), 2);

		region = img(cv::Rect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y));
		avg = cv::mean(region)[0];

		if (avg < aruco->maxBlack)
			bits.at<uint8_t>(i/aruco->size, i%aruco->size) = 0;
		else if (avg > aruco->minWhite)
			bits.at<uint8_t>(i/aruco->size, i%aruco->size) = 1;
		else
			return false;
	}
	cout << bits << endl;
	cv::imshow("img", img);
	cv::waitKey(0);
	return true;
}

