/*
 * trackSegment.cpp
 *
 *  Created on: 2014-3-29
 *      Author: Cai
 */

#include "trackSegment.h"

using namespace cv;


TrackSegment::TrackSegment()
:pt1(Point(0,0)), pt2(Point(0,0)), vPoint(0), cnt(0), level(RANK_NOCOLOR){
}

TrackSegment::TrackSegment(double x1, double y1, double x2, double y2)
:pt1(Point(x1,y1)), pt2(Point(x2,y2)), vPoint(0), cnt(0), level(RANK_NOCOLOR){
}

TrackSegment::TrackSegment(const cv::Point& _pt1, const cv::Point& _pt2)
:pt1(_pt1), pt2(_pt2), vPoint(0), cnt(0), level(RANK_NOCOLOR){

}

TrackSegment::~TrackSegment(){
}

bool TrackSegment::EQ(double n1, double n2){
	if( abs( n1 - n2) < 0.001)
		return true;
	else
		return false;
}

double TrackSegment::Distance(double x1, double y1, double x2, double y2){
	return sqrtf((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

double TrackSegment::Distance(const cv::Point& _pt1, const cv::Point& _pt2){
	Point pt = _pt2 - _pt1;
	return sqrtf( pt.ddot(pt));
}

double TrackSegment::pointSegment(const double x1, const double y1, const double x2,
					const double y2, const double x, const double y){
						double dbLen1, dbLen2, dbLen3;
						double dbAng, dbAng1, dbAng2;

						dbLen1 = Distance(x, y, x1, y1);
						if( EQ(0.0, dbLen1))
							return 0.0;
						dbLen2 = Distance(x, y, x2, y2);
						if( EQ(0.0, dbLen2))
							return 0.0;
						dbLen3 = Distance(x1, y1, x2, y2);
						if( EQ(0.0, dbLen3))
							return dbLen1;

						if( dbLen1 < dbLen2){
							if( EQ(y1, y2)){
								if( x1 < x2)
									dbAng1 = 0.0;
								else
									dbAng1 = PI;
							}else{
								dbAng1 = acos(( x2 - x1) / dbLen3);
								if( y1 > y2)
									dbAng1 = 2 * PI - dbAng1;
							}
							dbAng2 = acos( (x - x1) / dbLen1);
							if( y1 > y)
								dbAng2 = 2 * PI - dbAng2;
							dbAng = dbAng2 - dbAng1;
							if( dbAng < 0.0)
								dbAng =- dbAng;
							if( dbAng > PI/2)
								return dbLen1;
							else
								return (dbLen1 * sin(dbAng));
						}else{
							if( EQ( y1, y2)){
								if( x1 < x2)
									dbAng1 = PI;
								else
									dbAng1 = 0.0;
							}else{
								dbAng1 = acos(( x1 - x2) / dbLen3);
								if( y2 > y1)
									dbAng2 = 2 * PI - dbAng1;
							}
							dbAng2 = acos(( x - x2) / dbLen2);
							if( y2 > y)
								dbAng2 = 2 * PI - dbAng2;
							dbAng2 = dbAng2 - dbAng1;
							if( dbAng < 0.0)
								dbAng = - dbAng;
							if( dbAng > PI)
								dbAng = 2 * PI - dbAng;
							if( dbAng > PI/2)
								return dbLen2;
							else
								return (dbLen2 * sin(dbAng));
						}
}

double TrackSegment::pointSegment(const cv::Point& _pt){
	double a, b, c;

	a = Distance(_pt, pt1);
	if( EQ(0.0, a))
		return 0.0;
	b= Distance(_pt, pt2);
	if( EQ(0.0, b))
		return 0.0;
	c = Distance( pt1, pt2);
	if( EQ(0.0, c))
		return a;

	if( a*a >= b*b + c*c)
		return b;

	if( b*b >= a*a + c*c)
		return a;

	double l = (a+b+c)/2.0;
	//海伦公式求面积
	double s = sqrtf(l * (l - a)*(l - b)*(l - c));
	return 2*s/c;
}

void TrackSegment::addPoint(const cv::Point& _pt){
	vPoint.push_back(_pt);
	cnt++;
}

void TrackSegment::clearPoint(){
	vPoint.clear();
	cnt = 0;
}

int TrackSegment::getCount() const{
	return cnt;
}

void TrackSegment::setLevel(SegmentRank _level){
	level = _level;
}

SegmentRank TrackSegment::getLevel() const{
	return level;
}

cv::Point TrackSegment::getPoint1() const{
	return pt1;
}

cv::Point TrackSegment::getPoint2() const{
	return pt2;
}