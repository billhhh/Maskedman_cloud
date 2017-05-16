#ifndef _H_ALGORITHMINTERFACE_H_
#define _H_ALGORITHMINTERFACE_H_
#include <string>
#include <opencv2/opencv.hpp>

enum AlgorithmResult{
	AR_DONE = 0,//算法成功执行完毕
	AR_BREAK,//算法因控制中断
	AR_INTERNAL,//算法因内部错误中断
	AR_EXTERNAL,//算法因外部错误中断
	AR_END//算法执行失败
};

#define MAXPATH 256
class AlgorithmInterface{
public:
	char file[MAXPATH];//保存文件的全路径。
	char folder[MAXPATH];//保存需要输出数据的文件夹路径。

	cv::Mat image;//图像矩阵。
	cv::Mat* pImg;//指向图像矩阵的指针。

	uint percent;//算法处理的进度，范围为（0-100）。
	bool isRun;//算法的外部控制变量，决定算法是否继续执行。

	int mutex;//信号量，防止同一数据的读写冲突。
public:
	AlgorithmInterface();
	~AlgorithmInterface();
	virtual void init() = 0;
	virtual void destroy() = 0;
	void lock();//对信号量加锁。s
	void unlock();//对信号量解锁。 
};

#endif