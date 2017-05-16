#include "detect.h"
#include "DetectMasked.h"
using namespace std;
using namespace cv;

//算法外部接口函数
bool detectMaskMan(const string& file, const string& savePath, float interval_s, 
	char* eventInfo, Mat* &curFrame,
	int &percent, bool &isRunning){
		string name = file;
		string path = savePath;
		DetectMasked* detect;

		if( !detect->getInstance()->init())//未初始化成功
			return false;
		detect->getInstance()->setParam(file, path, interval_s);//已保证每次只能得到一个instance
		return detect->getInstance()->detectMaskedMan(eventInfo, curFrame, percent, isRunning);	
}

void sayHello(){
	printf("sayHello!\n");
}