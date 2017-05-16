#ifndef _H_AI_TRAJECTORYANALYSIS_H_
#define _H_AI_TRAJECTORYANALYSIS_H_
#include "algorithmInterface.h"

class AI_TrajectoryAnalysis: public AlgorithmInterface{
public:
	AI_TrajectoryAnalysis();
	~AI_TrajectoryAnalysis();
	virtual void init();
	virtual void destroy();
	bool getImage(cv::Mat*&);
};

#endif