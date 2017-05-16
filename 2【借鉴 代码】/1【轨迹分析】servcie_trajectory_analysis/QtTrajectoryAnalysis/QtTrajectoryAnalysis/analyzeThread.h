/*
 * analyzethread.h
 *
 *  Created on: 2015-1-4
 *      Author: Cai
 */
#ifndef _H_ANALYZETHREAD_CAI_
#define _H_ANALYZETHREAD_CAI_
#include <Qt/qthread.h>
#include "serviceDll.h"
#include "AlgorithmInterface/ai_trajectoryAnalysis.h"

class analyzeThread :public QThread{
	Q_OBJECT

public:
	void init(AI_TrajectoryAnalysis * );

	analyzeThread(QObject *parent);
	~analyzeThread();

signals:
	void threadEnd(cv::Mat*);
	void threadFail();

protected:
	void run();

private:
	ServiceDLL* serviceDll;
	AI_TrajectoryAnalysis* pAI_ta;
};

#endif