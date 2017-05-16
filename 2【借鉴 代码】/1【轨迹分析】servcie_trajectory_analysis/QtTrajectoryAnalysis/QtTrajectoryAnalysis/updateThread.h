/*
 * updatethread.h
 *
 *  Created on: 2015-1-5
 *      Author: Cai
 */
#ifndef _H_UPDATETHREAD_CAI_
#define _H_UPDATETHREAD_CAI_
#include <Qt/qthread.h>
#include "extractThread.h"

class updateThread :public QThread{
	Q_OBJECT

public:
	void init(extractThread*);
	void setStatus(bool status);
	void setIsBatch(bool isBatch);
	updateThread(QObject *parent);
	~updateThread();

signals:
	void sendSliderVal(int);
	void sendFrameImg(cv::Mat*);

protected:
	void run();

private:
	extractThread* pExtractThread;
	int percent;
	cv::Mat* image;
	bool isBatch;

public slots:
	void getThreadEnd();
};

#endif