#include "ai_trajectoryAnalysis.h"

AI_TrajectoryAnalysis::AI_TrajectoryAnalysis(){
	init();
};

AI_TrajectoryAnalysis::~AI_TrajectoryAnalysis(){
	destroy();
}

void AI_TrajectoryAnalysis::init(){
}

void AI_TrajectoryAnalysis::destroy(){
}

bool AI_TrajectoryAnalysis::getImage(cv::Mat*& pMat){
	if( pImg == NULL || pImg->empty())
		return false;

	lock();
	image = pImg->clone();
	unlock();
	pImg = &image;
	return true;
}