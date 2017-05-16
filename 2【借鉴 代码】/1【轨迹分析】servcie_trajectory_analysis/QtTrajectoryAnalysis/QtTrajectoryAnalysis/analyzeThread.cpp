
#include "analyzeThread.h"

#include<opencv2/opencv.hpp>
using namespace cv;

analyzeThread::analyzeThread(QObject *parent)
	:QThread(parent){
}

analyzeThread::~analyzeThread(){

};

void analyzeThread::run(){
	if( serviceDll->getInstance()->execServiceAnalyze(pAI_ta)){
		emit threadFail();
	}else{
		Mat* pImg;
		pAI_ta->getImage(pImg);
		emit threadEnd(pImg);
	}
}

void analyzeThread::init(AI_TrajectoryAnalysis* pda){
	pAI_ta = pda;
}
