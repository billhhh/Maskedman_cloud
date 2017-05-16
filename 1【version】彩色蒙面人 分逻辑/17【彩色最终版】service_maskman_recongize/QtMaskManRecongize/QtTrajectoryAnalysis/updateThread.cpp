
#include "updateThread.h"

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
		if( !isBatch){//////不是批量处理的情况
			if( pExtractThread->getImg(img))
				sendImg(img);
		}
		std::string str = pExtractThread->getEventInfo();
		if( str != eventInfo){
			eventInfo = str;
			sendInfo(eventInfo);
		}		
		msleep(10);
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