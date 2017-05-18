#ifndef _H_LICENSEPLATEIDENTIFY_H_
#define _H_LICENSEPLATEIDENTIFY_H_

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "include/plate_recognize.h"
#include "AI_LicensePlate_Identify.h"

class LicensePlateIdentify{
public:
	static LicensePlateIdentify* getInstance();
	~LicensePlateIdentify();
	bool plateRecognize(AI_LicensePlate_Identify* );
	bool plateIdentify(AI_LicensePlate_Identify* );
	bool plateIdentify_file(AI_LicensePlate_Identify* );

private:
	LicensePlateIdentify();
	void init();

private:
	static LicensePlateIdentify* instance;
	easypr::CPlateRecognize pr;
};

#endif