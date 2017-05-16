
#include "extractThread.h"

#include<opencv2/opencv.hpp>
using namespace cv;

extractThread::extractThread(QObject *parent)
	:QThread(parent){
}

extractThread::~extractThread(){
	pAI_ta = NULL;
};

void extractThread::run(){
	pAI_ta->percent = 0;
	pAI_ta->mutex = 0;
	pAI_ta->pImg = NULL;
	pAI_ta->isRun = true;
	if( serviceDll->getInstance()->execServiceAuto(pAI_ta)){
		emit threadFail();
		pAI_ta->percent = 0;
	}else if( pAI_ta->isRun){
		Mat* pImg;
		pAI_ta->getImage(pImg);
		emit threadEnd(pImg);
		pAI_ta->percent = 100;
	}else{
		emit threadStop();
		pAI_ta->percent = 0;
	}
	pAI_ta->isRun = false;
}

void extractThread::init(AI_TrajectoryAnalysis* al){
	pAI_ta = al;
}

bool extractThread::getStatus() const{
	return pAI_ta->isRun;
}

void extractThread::setStatus(bool status){
	pAI_ta->isRun = status;
	pAI_ta->mutex = 0;
}

int extractThread::getPercent() const{
	return pAI_ta->percent;
}

bool extractThread::getImage(cv::Mat*& img){
	if( !pAI_ta->getImage(img))
		return false;
	return true;
}