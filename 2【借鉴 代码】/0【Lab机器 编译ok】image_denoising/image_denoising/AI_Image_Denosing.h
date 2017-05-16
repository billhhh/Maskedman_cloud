#ifndef _H_AI_IMAGE_DENOSING_H_
#define _H_AI_IMAGE_DENOSING_H_

#include "algorithmInterface.h"
class AI_Image_Denosing: public AlgorithmInterface{
public:
	int kel;
	cv::Mat imageSrc;
	cv::Mat* pImgSrc;
public:
	AI_Image_Denosing();
	~AI_Image_Denosing();
	virtual void init();
	virtual void destroy();
};

#endif