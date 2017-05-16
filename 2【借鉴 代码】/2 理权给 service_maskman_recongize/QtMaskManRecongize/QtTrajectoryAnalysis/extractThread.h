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

class extractThread :public QThread{
	Q_OBJECT

public:
	void init(std::string, std::string);
	bool getStatus() const;
	void setStatus(bool status);

	int getPercent() const;
	bool getImg(cv::Mat&);
	std::string getEventInfo();
	extractThread(QObject *parent);
	~extractThread();

signals:
	void sendSliderVal(int);
	void threadStop();
	void threadEnd();
	void threadFail();

protected:
	void run();

private:
	ServiceDLL* serviceDll;
	std::string videoPath;
	std::string eventInfo;
	char dataInfo[16384];
	cv::Mat img;
	cv::Mat *pImg;
	int percent;
	bool isRunning;
};

#endif