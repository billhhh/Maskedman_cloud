#ifndef _AI_FLAME_DETECT_H_
#define _AI_FLAME_DETECT_H_

#include "algorithmInterface.h"

enum ForwardResult{
	BACKWARD = 0,  //快退
	NORMAL,   //正常
	FAST_FORWARD,  //快进
	PAUSE,  //暂停
};

#define MAXSIZE 4096
class AI_Flame_Detect: public AlgorithmInterface{
public:
	AI_Flame_Detect();
	~AI_Flame_Detect();
	virtual void init();
	virtual void destroy();

	float interval;  //帧间隔，用于跳帧
	//注：file （要打开的视频文件）
	//	  folder （保存需要输出数据的文件夹路径）  已在父类algorithmInterface.h中定义
	char eventInfo[MAXSIZE];  //蒙面人信息
	cv::Mat curFrame_out; //往外传的现有帧
	unsigned int forwardInfo;  //是否快进、暂停
	int type; // 近景1，远景2
};

#endif