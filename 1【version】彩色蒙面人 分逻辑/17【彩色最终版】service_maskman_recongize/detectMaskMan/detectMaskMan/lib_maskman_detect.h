/*
 * lib_maskman_detect.h
 *
 * funtion:These functions can be external invoked
 *
 *    Author: Bill Wang
 */

#ifndef _LIB_MASKMAN_DETECT_H_
#define _LIB_MASKMAN_DETECT_H_


#include "AI_Maskman_Detect.h"


extern "C" __declspec(dllexport) uint maskman_detect(void*, int);	//算法外部接口函数

extern "C"__declspec(dllexport) void sayHello();	//Hello测试

#endif
