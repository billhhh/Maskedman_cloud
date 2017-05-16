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
	if( image.empty())
		return false;

	pMat = pImg;
	return true;
}