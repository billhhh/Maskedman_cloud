#ifndef _TESTDLL_H_
#define _TESTDLL_H_

#include <opencv2/opencv.hpp>
#include <qlibrary.h>
#include "AlgorithmInterface/AI_Maskman_Detect.h"

typedef uint (_cdecl * MaskManDetect)(void *, int length);
typedef void (_cdecl * SayHello)();

class TestDll{

public:
	QLibrary qLib;
	MaskManDetect detection;
	SayHello sayHello;
	void testHello();

};

#endif