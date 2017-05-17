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
	int videoType;//����1��Զ��2
	AI_Flame_Detect* pAI_id;
	string videoPath;
	string savePath;

	//����
	static const int frequency=5;//����Ƶ��
	static const int secondnum=3;//��ȡ����
	static const int imagenum=15;//����֡��
	static const int diffnum=14;//���֡��	
	static const int peakvalue1=10;//��ֵ
	static const int peakvalue2=5;
	static const int maxpeakvalue1=20;//����ֵ ���������ֵ������Ϊ�������
	static const int maxpeakvaule2=20;
	static const int blockw=8; //����
	static const int blockh=8; //��߶�
	const float smokeThresholdRatio;
	static const int maxNumber=80;//���������
	float highParam[3];//��ͨ�˲�ϵ��
	float lowParam[3];//��ͨ�˲�ϵ��

	//Զ��
	cv::Ptr<cv::BackgroundSubtractorMOG2> bg;
	Mat firstFrame;
	Mat background;

public:
	Controller();
	~Controller(void);	

	void setParam(AI_Flame_Detect *pAI_da);
	bool startDetect();
	void drawFlame(Mat &frame);

	/** ���� **/
	void flameDetect_close();
	void twoStageFilter(float xn[diffnum],float dn[diffnum],float en[diffnum]);//[diffnum]
	bool isSmokePoint(float dn[diffnum],float en[diffnum]);//[diffnum]
	void getFilterImage(Mat &filter, Mat diff[diffnum],int mark[maxNumber][maxNumber]);//Mat diff[diffnum],int mark[maxNumber][maxNumber]
	bool ifFlame_close(Mat &frame,int mark[maxNumber][maxNumber],int blockwNum,int blockhNum);//int mark[maxNumber][maxNumber]

	/** Զ�� **/
	void flameDetect_far();
	void grad_fore();
	bool keepSomeMaxCon(Mat &src, int n);
	void frameFore(Mat frame,Mat &fore);
	bool buildBackground();
};

#endif