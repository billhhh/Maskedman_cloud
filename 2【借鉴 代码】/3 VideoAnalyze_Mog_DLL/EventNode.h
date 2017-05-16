#ifndef EVENTNODE_H
#define EVENTNODE_H

#include <opencv.hpp>
#include <highgui/highgui.hpp>
using namespace cv;

struct EventElement
{
	Rect rect;
	int frameNum;
};

class EventNode
{
public:
	EventNode(void){}
	~EventNode(void){}
public:
	bool mark;
	int startFrame;
	int endFrame;
	Rect eventRect;
	vector<EventElement> trackList;
};

#endif