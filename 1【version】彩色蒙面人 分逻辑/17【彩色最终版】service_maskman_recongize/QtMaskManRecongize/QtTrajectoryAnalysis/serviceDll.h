#ifndef _H_SERVICEDLL_CAI_H_
#define _H_SERVICEDLL_CAI_H_

#include <opencv2/opencv.hpp>
#include <string>
#include <qLibrary.h>
#include <QMainWindow>
#include "AlgorithmInterface/AI_Maskman_Detect.h"

typedef uint (_cdecl * DetectMaskMan)( void*, int);
typedef void (_cdecl * SayHello)();

class ServiceDLL{
protected:
	static ServiceDLL* instance;
	ServiceDLL();

	QLibrary qLib;
	DetectMaskMan maskManRec;
	SayHello sayHello;

public:
	~ServiceDLL();
	static ServiceDLL* getInstance();

	bool execServiceExtract(AI_Maskman_Detect*&);
	void releaseDLL();
	bool loadDLL(std::string path);

};

#endif