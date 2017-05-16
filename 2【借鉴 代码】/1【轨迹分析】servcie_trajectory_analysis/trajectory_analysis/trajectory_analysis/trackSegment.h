/*
 * trackSegment.h
 *
 *  Created on: 2014-3-29
 *      Author: Cai
 */

#ifndef _H_TRACKSEGMENT_CAI_
#define _H_TRACKSEGMENT_CAI_

#include "opencv2/core/core.hpp"
#include <cmath>
#include <vector>
#define PI 180

//轨迹人流比例等级
enum SegmentRank{
	RANK_NOCOLOR = 0,
	RANK_BLUE,
	RANK_GREEN,
	RANK_YELLOW,
	RANK_RED
};

//人流轨迹
class TrackSegment{
private:
	cv::Point pt1;
	cv::Point pt2;
	std::vector<cv::Point> vPoint;

	SegmentRank level;
	int cnt;
public:
	TrackSegment();
	TrackSegment(double x1, double y1, double x2, double y2);
	TrackSegment(const cv::Point& _pt1, const cv::Point& _pt2);
	~TrackSegment();

	static bool EQ(double n1, double n2);

	static double Distance(double x1, double y1, double x2, double y2);
	static double Distance(const cv::Point& _pt1, const cv::Point& _pt2);

	static double pointSegment(const double x1, const double y1, const double x2,
		const double y2, const double x, const double y);

	double pointSegment(const cv::Point& _pt);

	void addPoint(const cv::Point& _pt);
	void clearPoint();
	int getCount() const;

	void setLevel(SegmentRank _level);
	SegmentRank getLevel() const;

	cv::Point getPoint1() const;
	cv::Point getPoint2() const;
};

#endif