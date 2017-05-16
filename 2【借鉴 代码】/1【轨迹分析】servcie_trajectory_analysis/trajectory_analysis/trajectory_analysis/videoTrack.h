#ifndef _H_FOREGROUNDTRACK_CAI
#define _H_FOREGROUNDTRACK_CAI

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/photo/photo.hpp"
#include "opencv2/video.hpp"
#include <fstream>
#include <vector> 
#include <string>

#include "baseDefine.h"
using cv::BackgroundSubtractor;
using cv::Ptr;

const double AREA_THRESHOLD = 0.005;

//轨迹区域
typedef struct RectFrame{
	cv::Rect rect;
	int frameNum;//帧数
}RECTFRAME;


class videoTrack{
public:
	static videoTrack* GetInstance();
	void fixBg(cv::Mat &img);
	void inPaint(cv::Mat &img);
	bool openVideo(const std::string& strVideo);
	void getFrame(cv::Mat& img);

	bool getEveryFrame(cv::Mat& img);
	void setBg(cv::Mat& img);
	void initPara();
	bool startFileRecord(std::string& fileName);
	void endFileRecord();
	bool isRuningVideo() const;
	int getFrameNum() const;
	void setOutputFolder(std::string str);
	std::string getFileName() const;
	void clear();
private:
	static videoTrack* instance;
	cv::VideoCapture capture;
	cv::Mat capFrame;
	
	Ptr<BackgroundSubtractor> bg_model;
	cv::Mat fgMask;
	cv::Mat bgImg;
	cv::Mat drawImg;

	std::vector<COORDINFRAME> vTrackInfo;//记录所有帧轨迹
	int frame_num;
	int total_frameNum;
	int peopleNum;
	int fps;
	

	std::ofstream coord_txt;
	std::string fileName;
	std::string fileTxt;
	std::string outputFolder;

	int imgH;
	int imgV;
	double ratio;

	videoTrack();
	virtual ~videoTrack();

	void copyToROI(cv::Mat &img, cv::Rect &rectTo, cv::Rect &rectFrom);
	void processing(cv::Mat &frame, cv::Mat &bg, cv::Mat &output);

};

#endif