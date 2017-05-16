#ifndef _H_DETECTMASKED_H_
#define  _H_DETECTMASKED_H_

#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
using namespace std;
using namespace cv;

class DetectMasked{
private:
	static DetectMasked* instance;//test
	VideoCapture capture;
	string strFile;
	string strPath;
	float interval;

	//全局变量
	string cascadeName;//人头检测训练数据
	string nestedCascadeName; //正脸分类器
	CascadeClassifier cascade, nestedCascade;//创建级联分类器对象

	Mat firstFrame;	//第一帧 
	Mat firstFrame_8U;	//第一帧 的8U图像
	Mat curFrame_rz;		//当前帧图像(缩小)处理
	Mat curFrame_rz_gray;		//curFrame_rz转成的灰度图
	Mat img_diff;		//差分输出
	Mat img_diff_bi; //二值图输出

	int lastRectHeight; //一般都是矩形的高度变化，存储上一个矩形高度
	bool isDetect;
	double fScale;      //缩放倍数
	double learningRate;    // 控制背景累积学习的速率
	double area_threshold;  //区域面积大小阈值
	int nThreshold; //二值化阈值
	double lastMaskFrame; //上一个蒙面人出现的帧数，用于跳帧

private:
	DetectMasked(void);

	void findBoundary(Rect r,Point &a,Point &c);
	void biggerRect(Point &a,Point &c,Mat img);
	void hugeRect(Point &a,Point &c,Mat img);
	void detectAndDraw( Mat& img,
		CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
		double scale);
	void whThresholdOtsu(Mat &src, Mat &dst);
	void cvSkinOtsu(Mat &src, Mat &dst);
	bool findMaskMan(Mat img);

public:
	~DetectMasked(void);
	static DetectMasked* getInstance();
	bool init();
	void setParam(string file, string savePath, float interval_s);
	//void setParam(const char* file, const char* path, const float& interval_s);
	bool detectMaskedMan(char* eventInfo, Mat* &curFrame, 
		int &percent, bool &isRunning);	
};

#endif