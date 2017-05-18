#ifndef _H_AI_LICENSEPLATEIDENTIFY_H_
#define _H_AI_LICENSEPLATEIDENTIFY_H_

#include "algorithmInterface.h"

class AI_LicensePlate_Identify: public AlgorithmInterface{
public:
	struct ImageData{
		uchar *pImgData;
		int imgRows;
		int imgCols;
		int imgChannels;
	};
	ImageData imageData;
	char plateChar[MAXPATH];

public:
	AI_LicensePlate_Identify();
	~AI_LicensePlate_Identify();
	virtual void init();
	virtual void destroy();
	void createImageData(ImageData*, int, int, int, uchar*);
	void releaseImageData(ImageData*);
};

#endif