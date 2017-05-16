/*
 * lib_trajectory_analysis.h
 *
 *  Created on: 2014-12-28
 *      Author: Cai
 */

#ifndef LIB_TRAJECTORY_ANALYSIS_H
#define LIB_TRAJECTORY_ANALYSIS_H

#include <opencv2/opencv.hpp>
#include "AlgorithmInterface/algorithmInterface_Global.h"

extern "C" __declspec(dllexport) uint analyzingData(void*, int);
extern "C" __declspec(dllexport) uint autoAnalyzingData(void*, int);

extern "C" __declspec(dllexport) void sayHello();

#endif
