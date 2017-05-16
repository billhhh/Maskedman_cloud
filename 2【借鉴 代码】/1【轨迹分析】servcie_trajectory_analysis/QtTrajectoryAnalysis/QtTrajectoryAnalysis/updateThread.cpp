
#include "updateThread.h"
using namespace cv;

updateThread::updateThread(QObject *parent)
	:QThread(parent){
	pExtractThread = NULL;
	percent = 0;
	isBatch = false;
}

updateThread::~updateThread(){

};

void updateThread::run(){
	while( !pExtractThread->isFinished()){
		int p = pExtractThread->getPercent();
		if( p <= 0 || p > 100)
			continue;
		if( percent != p){
			percent = p;
			sendSliderVal(percent);
		}			
		if( !isBatch){//////����������������
			if( pExtractThread->getImage(image))
				sendFrameImg(image);
		}
		msleep(50);
	}
	int p = pExtractThread->getPercent();
	if( p == 0 || p == 100){
		percent = p;
		sendSliderVal(percent);
	}
}

void updateThread::init(extractThread* pAT){
	pExtractThread = pAT;
}

void updateThread::getThreadEnd(){

}

void updateThread::setIsBatch(bool val){
	isBatch = val;
}