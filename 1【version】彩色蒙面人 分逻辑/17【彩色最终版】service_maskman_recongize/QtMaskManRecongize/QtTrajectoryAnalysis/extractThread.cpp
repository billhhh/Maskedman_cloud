
#include "extractThread.h"

#include<opencv2/opencv.hpp>
using namespace cv;

extractThread::extractThread(QObject *parent)
	:QThread(parent){
}

extractThread::~extractThread(){
	pAI_md = NULL;
};

void extractThread::run(){
	pAI_md->forwardInfo = ForwardResult::NORMAL;
	pAI_md->isRun = true;
	if( !serviceDll->getInstance()->execServiceExtract(pAI_md)){
		if( !pAI_md->isRun)
			emit threadStop();
		else
			emit threadFail();
		pAI_md->percent = 0;
	}else if( pAI_md->isRun){
		emit threadEnd();
		pAI_md->percent = 100;
	}
	pAI_md->isRun = false;
}

void extractThread::init(AI_Maskman_Detect* al){
	pAI_md = al;
}

bool extractThread::getStatus() const{
	return pAI_md->isRun;
}

void extractThread::setStatus(bool status){
	pAI_md->isRun = status;
	pAI_md->mutex = 0;
}

int extractThread::getPercent() const{
	return pAI_md->percent;
}

bool extractThread::getImg(cv::Mat*& _img){
	if( pAI_md->curFrame_out.empty())
		return false;

	_img = &pAI_md->curFrame_out;
	return true;
}

std::string extractThread::getEventInfo(){
	return pAI_md->eventInfo;
}