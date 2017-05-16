#include "detect.h"
#include "DetectMasked.h"
using namespace std;
using namespace cv;

//�㷨�ⲿ�ӿں���
bool detectMaskMan(const string& file, const string& savePath, float interval_s, 
	char* eventInfo, Mat* &curFrame,
	int &percent, bool &isRunning){
		string name = file;
		string path = savePath;
		DetectMasked* detect;

		if( !detect->getInstance()->init())//δ��ʼ���ɹ�
			return false;
		detect->getInstance()->setParam(file, path, interval_s);//�ѱ�֤ÿ��ֻ�ܵõ�һ��instance
		return detect->getInstance()->detectMaskedMan(eventInfo, curFrame, percent, isRunning);	
}

void sayHello(){
	printf("sayHello!\n");
}