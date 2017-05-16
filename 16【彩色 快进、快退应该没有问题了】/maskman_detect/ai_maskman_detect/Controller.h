/*
 * Controller.h
 *
 *  Created on: 2015-3-18
 *      Author: Bill Wang
 */

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "cv.h"
#include "highgui.h"
#include "AlgorithmInterface/AI_Maskman_Detect.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>

#include <iostream>
#include <string>

class Controller
{
public:
	AI_Maskman_Detect* pAI_id;
	static Controller* instance;
	cv::CascadeClassifier cascade;
	cv::CascadeClassifier nestedCascade;
	cv::CascadeClassifier thirdCascade;
	cv::CascadeClassifier forthCascade;

	bool init();
	Controller();  //constructor
	~Controller(void); //destructor
	static Controller* getInstance();
	bool startDetect();
	void setParam(AI_Maskman_Detect *pAI_da);

private:
	double fScale;      //缩放倍数
	double learningRate;    // 控制背景累积学习的速率
	double area_threshold;  //区域面积大小阈值
	int nThreshold; //二值化阈值

	cv::Mat curFrame_ori;		//每一帧原图像，绝不处理
	cv::Mat curFrame_resize;
	cv::Mat curFrame_gray;
	cv::Mat firstGrayFrame;
	cv::Mat firstGrayFrame_8U;
	cv::Mat curFrame_diff;		//差分后图像
	cv::Mat curFrame_diff_bin;  //差分并二值化后
	int lastRectHeight; //存储上一帧待处理矩形高度
	double lastMaskFrame; //上一个蒙面人出现的帧数，用于跳帧
	bool isDetect;   //是否检测到蒙面人

private:
	int cascadeCnt;
	int nestedCnt;
	cv::VideoCapture capture;
	std::string videoPath;
	std::string savePath;
	float interval; //帧间隔，用于跳帧，加快处理速度

	void findBoundary(cv::Rect r,cv::Point &a,cv::Point &c);
	void biggerRect(cv::Point &a,cv::Point &c,cv::Mat img);
	void hugeRect(cv::Point &a,cv::Point &c,cv::Mat img);
	//黑白蒙面人检测
	void detectAndDraw( cv::Mat& src,
		cv::CascadeClassifier& cascade, cv::CascadeClassifier& nestedCascade,
		cv::CascadeClassifier& thirdCascade,cv::CascadeClassifier& forthCascade,
		double scale);

	//彩色蒙面人检测
	void detectAndDrawColorful( cv::Mat& src,
		cv::CascadeClassifier& cascade, cv::CascadeClassifier& nestedCascade,
		double scale);
	void whThresholdOtsu(cv::Mat &src, cv::Mat &dst);
	void cvSkinOtsu(cv::Mat &src, cv::Mat &dst);
	bool findMaskMan(cv::Mat img);

};



#endif
