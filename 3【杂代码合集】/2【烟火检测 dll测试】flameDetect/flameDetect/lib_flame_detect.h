/*
 * lib_Flame_detect.h
 *
 * funtion:These functions can be external invoked
 *
 *    Author: Bill Wang
 */

#ifndef _LIB_FLAME_DETECT_H_
#define _LIB_FLAME_DETECT_H_


#include "AI_Flame_Detect.h"


extern "C" __declspec(dllexport) unsigned int flame_detect(void*, int);	//算法外部接口函数

extern "C"__declspec(dllexport) void sayHello();	//Hello测试

#endif
