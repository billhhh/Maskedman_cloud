/*
 * dataAnalysis.h
 *
 *  Created on: 2014-3-31
 *      Author: Cai
 */

#ifndef _H_DATAANALYSIS_CAI_
#define _H_DATAANALYSIS_CAI_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <vector>
#include <string>

#include "isodata.h"
#include "trackSegment.h"
#include "baseDefine.h"

#define RED Scalar(0, 0, 255)
#define YELLOW Scalar(0, 255, 255)
#define GREEN Scalar(0, 255, 0)
#define BLUE Scalar(255, 0, 0)

/*
 *  class name: DataAnalysis
 *  description: DataAnalysis类，该类可以对数据进行分析操作，
 *		以图片方式输出直观结果。
 */

class DataAnalysis{
public:
	static DataAnalysis* GetInstance();
	~DataAnalysis();

	bool setImage(const std::string& str);
	bool readData(const std::string& str);

	void getDrawImg(cv::Mat& img);
	void analysis();
	void setPath(const std::string& str);

private:
	static DataAnalysis* instance;
	Isodata* isodata;

	cv::Mat oriImg;
	cv::Mat drawImg;
	cv::Mat ptData;
	int imgWidth;
	int imgHeight;
	int fps;
	int frameCount;
	std::string fileName;
	std::string filePath;

	std::vector<std::vector<COORDINFRAME>> vTrackInfo;//记录所有帧轨迹
	std::vector<std::vector<double>> vPattern;
	std::vector<int> vFrameNum;
	std::vector<SortCenter> vSortCenter;
	std::vector<SortCenter> vCenter;

	std::vector<TrackSegment> vTrackSegment;
	std::vector<std::vector<double>> vAllPoint;
	int totalPeople;
	bool hasPath;

private:
	DataAnalysis();	
	void generatePtData();	
	void peekPattern();
	void drawCenter();

	void generateSegment();
	void dividePoint();
	void selectSegment();
	void drawSegment();
	void countPeople();
	void drawPeopleFlow();
	double rectIntersectAera(const COORDINFRAME& rect1, const COORDINFRAME& rect2);
	void countRemainTime();
	void initData();
	void selectCenterPre();
	void selectCenter();
};

#endif