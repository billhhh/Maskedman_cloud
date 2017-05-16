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
	void init(extractThread* );
	void setStatus(bool status);
	void setIsBatch(bool isBatch);

	updateThread(QObject *parent);
	~updateThread();

signals:
	void sendSliderVal(int);
	void sendImg(cv::Mat&);
	void sendInfo(std::string);

protected:
	void run();

private:
	extractThread* pExtractThread;
	cv::Mat img;
 	int percent;
	std::string eventInfo;
	bool isBatch;

public slots:
	void getThreadEnd();
};

#endif