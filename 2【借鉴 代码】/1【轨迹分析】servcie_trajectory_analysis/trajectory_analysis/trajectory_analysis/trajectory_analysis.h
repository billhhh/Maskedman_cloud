#ifndef _H_TRAJECTORY_ANALYSIS_H
#define _H_TRAJECTORY_ANALYSIS_H


/*
 * trajectory_analysis.h
 *
 *  Created on: 2014-12-28
 *      Author: Cai
 */

#include "videoTrack.h"
#include "dataAnalysis.h"
#include "AlgorithmInterface/algorithmInterface_Global.h"

class trajectory_analysis{
private:
	static trajectory_analysis* instance;
	trajectory_analysis();

	videoTrack* m_videoTrack;
	DataAnalysis* m_dataAnalysis;
	cv::Mat m_image;
	cv::Mat m_draw;
	//std::string
public:
	static trajectory_analysis* getInstance();
	~trajectory_analysis();
	void init();
	void destroy();
	void clear();

	bool execExtracting(AI_TrajectoryAnalysis*);
	bool execAnalyzing(AI_TrajectoryAnalysis*);
	bool execAnalyzing(std::string str, std::string path, cv::Mat*& img);
	std::string getVideoTxtName() const;
};

#endif