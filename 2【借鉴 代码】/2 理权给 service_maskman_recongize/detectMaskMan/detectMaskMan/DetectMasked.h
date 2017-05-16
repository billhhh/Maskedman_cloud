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

	//ȫ�ֱ���
	string cascadeName;//��ͷ���ѵ������
	string nestedCascadeName; //����������
	CascadeClassifier cascade, nestedCascade;//������������������

	Mat firstFrame;	//��һ֡ 
	Mat firstFrame_8U;	//��һ֡ ��8Uͼ��
	Mat curFrame_rz;		//��ǰ֡ͼ��(��С)����
	Mat curFrame_rz_gray;		//curFrame_rzת�ɵĻҶ�ͼ
	Mat img_diff;		//������
	Mat img_diff_bi; //��ֵͼ���

	int lastRectHeight; //һ�㶼�Ǿ��εĸ߶ȱ仯���洢��һ�����θ߶�
	bool isDetect;
	double fScale;      //���ű���
	double learningRate;    // ���Ʊ����ۻ�ѧϰ������
	double area_threshold;  //���������С��ֵ
	int nThreshold; //��ֵ����ֵ
	double lastMaskFrame; //��һ�������˳��ֵ�֡����������֡

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