#ifndef _H_DETECT_H_
#define _H_DETECT_H_

#include <string>
#include <opencv2/opencv.hpp>

/*
extern "C"__declspec(dllexport) bool detectMaskMan(const std::string& file, const std::string& savePath, 
	float interval_s, std::string &eventInfo, cv::Mat &curFrame,
	int &percent, bool &isRunning);	*/

//算法外部接口函数
extern "C"__declspec(dllexport) bool detectMaskMan(const std::string& file, const std::string& savePath, 
	float interval_s, char *eventInfo, cv::Mat* &curFrame,
	int &percent, bool &isRunning);	

//测试的函数
extern "C"__declspec(dllexport) void sayHello();	
#endif
