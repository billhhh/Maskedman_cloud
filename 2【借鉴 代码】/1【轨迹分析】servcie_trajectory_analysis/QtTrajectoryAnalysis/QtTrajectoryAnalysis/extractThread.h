/*
 * extractThread.h
 *
 *  Created on: 2015-1-4
 *      Author: Cai
 */
#ifndef _H_EXTRACTTHREAD_CAI_
#define _H_EXTRACTTHREAD_CAI_
#include <Qt/qthread.h>
#include "serviceDll.h"
#include "AlgorithmInterface/algorithmInterface_Global.h"

class extractThread :public QThread{
	Q_OBJECT

public:
	void init(AI_TrajectoryAnalysis* pAlInterface);
	bool getStatus() const;
	void setStatus(bool status);

	int getPercent() const;
	bool getImage(cv::Mat*& );

	extractThread(QObject *parent);
	~extractThread();

signals:
	void sendSliderVal(int);
	void threadStop();
	void threadEnd(cv::Mat* );
	void threadFail();

protected:
	void run();

private:
	ServiceDLL* serviceDll;
	AI_TrajectoryAnalysis* pAI_ta;
};

#endif