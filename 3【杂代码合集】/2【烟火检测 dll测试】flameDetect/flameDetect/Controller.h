#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "AI_Flame_Detect.h"
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>
#include <string>
//using namespace std;
using std::string;
//using namespace cv;
using cv::VideoCapture;
using cv::Mat;


class Controller
{
private:
	VideoCapture video;
	int videoType;//近景1，远景2
	AI_Flame_Detect* pAI_id;
	string videoPath;
	string savePath;

	//近景
	static const int frequency=5;//采样频率
	static const int secondnum=3;//读取秒数
	static const int imagenum=15;//保存帧数
	static const int diffnum=14;//差分帧数	
	static const int peakvalue1=10;//峰值
	static const int peakvalue2=5;
	static const int maxpeakvalue1=20;//最大峰值 超过这个峰值将不认为是烟雾点
	static const int maxpeakvaule2=20;
	static const int blockw=8; //块宽度
	static const int blockh=8; //块高度
	const float smokeThresholdRatio;
	static const int maxNumber=80;//数组最大数
	float highParam[3];//高通滤波系数
	float lowParam[3];//低通滤波系数

	//远景
	cv::Ptr<cv::BackgroundSubtractorMOG2> bg;
	Mat firstFrame;
	Mat background;

public:
	Controller();
	~Controller(void);	

	void setParam(AI_Flame_Detect *pAI_da);
	bool startDetect();
	void drawFlame(Mat &frame);

	/** 近景 **/
	void flameDetect_close();
	void twoStageFilter(float xn[diffnum],float dn[diffnum],float en[diffnum]);//[diffnum]
	bool isSmokePoint(float dn[diffnum],float en[diffnum]);//[diffnum]
	void getFilterImage(Mat &filter, Mat diff[diffnum],int mark[maxNumber][maxNumber]);//Mat diff[diffnum],int mark[maxNumber][maxNumber]
	bool ifFlame_close(Mat &frame,int mark[maxNumber][maxNumber],int blockwNum,int blockhNum);//int mark[maxNumber][maxNumber]

	/** 远景 **/
	void flameDetect_far();
	void grad_fore();
	bool keepSomeMaxCon(Mat &src, int n);
	void frameFore(Mat frame,Mat &fore);
	bool buildBackground();
};

#endif