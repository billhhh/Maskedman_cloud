
#include "extractThread.h"

#include<opencv2/opencv.hpp>
using namespace cv;

extractThread::extractThread(QObject *parent)
	:QThread(parent){
		setStatus(false);
		//dataInfo = NULL;
		//dataInfo = new char[16384];
		//dataInfo  = {0};
}

extractThread::~extractThread(){
/*
	if(dataInfo != NULL){
		delete []dataInfo;
		dataInfo = NULL;
	}*/
};

void extractThread::run(){
	//execServiceExtract(char* eventInfo, cv::Mat*& img, int& percent, bool& running)
	//maskManRec(m_file, m_path, 0.5, eventInfo, img, percent, running)
	strcpy(dataInfo, "");
	percent = 0;
	pImg = NULL;
	isRunning = true;
	if( !serviceDll->getInstance()->execServiceExtract(dataInfo, pImg, percent, isRunning)){
		emit threadFail();
		percent = 0;
	}else if( isRunning){
		emit threadEnd();
		percent = 100;
	}else{
		emit threadStop();
		percent = 0;
	}
	isRunning = false;
}

//Ïß³Ìinit
void extractThread::init(std::string file, std::string path){
	setStatus(false);
	videoPath = file;
	serviceDll->getInstance()->setPara(file, path);
}

bool extractThread::getStatus() const{
	return isRunning;
}

void extractThread::setStatus(bool status){
	isRunning = status;
}

int extractThread::getPercent() const{
	return percent;
}

bool extractThread::getImg(cv::Mat& _img){
	if( NULL == pImg)
		return false;
	img = pImg->clone();
	_img = img;
	return true;
}

std::string extractThread::getEventInfo(){
	eventInfo = dataInfo;
	return eventInfo;
}