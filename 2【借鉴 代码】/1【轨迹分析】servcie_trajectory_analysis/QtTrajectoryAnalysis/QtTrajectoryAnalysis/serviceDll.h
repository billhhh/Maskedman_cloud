#ifndef _H_SERVICEDLL_CAI_H_
#define _H_SERVICEDLL_CAI_H_

#include <opencv2/opencv.hpp>
#include <string>
#include <qlibrary.h>
#include "AlgorithmInterface/algorithmInterface_Global.h"

typedef bool (_cdecl * AnalyzingData)(void *, int length);
typedef bool (_cdecl * AutoAnalyzingData)(void *, int length);
typedef void (_cdecl * SayHello)();

class ServiceDLL{
protected:
	static ServiceDLL* instance;
	ServiceDLL();

	QLibrary qLib;
	AnalyzingData analyzingData;
	AutoAnalyzingData autoAnalyzingData;
	SayHello sayHello;
public:
	~ServiceDLL();
	static ServiceDLL* getInstance();

	bool execServiceAnalyze(AI_TrajectoryAnalysis*&);
	bool execServiceAuto(AI_TrajectoryAnalysis*&);
	void releaseDLL();
	bool loadDLL(std::string path);
};

#endif