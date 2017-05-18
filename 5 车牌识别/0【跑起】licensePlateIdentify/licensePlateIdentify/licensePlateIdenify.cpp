#include "licensePlateIdenify.h"

using namespace cv;
using namespace easypr;
#define NULL 0

LicensePlateIdentify* LicensePlateIdentify::instance = NULL;

LicensePlateIdentify::LicensePlateIdentify(){
	init();
}

LicensePlateIdentify::~LicensePlateIdentify(){
	if( instance != NULL){
		delete instance;
		instance = NULL;
	}
}

LicensePlateIdentify* LicensePlateIdentify::getInstance(){
	if( instance == NULL){
		instance = new LicensePlateIdentify();
	}
	return instance;
}

void LicensePlateIdentify::init(){
	pr.LoadANN("..\\service_classifier\\ann_licensePlateChar.xml");
	pr.LoadSVM("..\\service_classifier\\svm_licensePlate.xml");

	pr.setLifemode(true);
	pr.setDebug(false);
}

bool LicensePlateIdentify::plateRecognize(AI_LicensePlate_Identify* pAI_pi){
	string file = pAI_pi->file;
	Mat image = imread(file.c_str());
	if( image.empty())
		return false;

	string str;
	vector<string> plateVec;
	int result = pr.plateRecognize(image, plateVec);
/*
	if (result == 0){
		int num = plateVec.size();
		for (int j = 0; j < num; j++){
			printf("License Plate: %s . \n", plateVec[j].c_str());
		}
	}*/
	if( result)
		return false;
	strcpy(pAI_pi->plateChar, str.c_str());
	//printf("License Plate: %s . \n", pAI_pi->plateChar);
	return true;
}

bool LicensePlateIdentify::plateIdentify(AI_LicensePlate_Identify* pAI_pi){
	uchar* data = pAI_pi->imageData.pImgData;
	if( NULL == data)
		return false;
	int rows = pAI_pi->imageData.imgRows;
	int cols = pAI_pi->imageData.imgCols;
	int channals = pAI_pi->imageData.imgChannels;
	int type;
	if( channals == 1)
		type = CV_8U;
	else
		type = CV_8UC3;
	Mat temp(rows, cols, type, data);
	Mat plate = temp.clone();
	string str;
	pr.setFileName(pAI_pi->file);
	int result = pr.plateIdentify(plate, str);
	if( result)
		return false;
	strcpy(pAI_pi->plateChar, str.c_str());
	//printf("License Plate: %s . \n", pAI_pi->plateChar);
	return true;
}

bool LicensePlateIdentify::plateIdentify_file(AI_LicensePlate_Identify* pAI_pi){
	Mat plate = imread(pAI_pi->file);
	if( plate.empty())
		return false;
	string str;
	int result = pr.plateIdentify(plate, str);
	if( result)
		return false;
	strcpy(pAI_pi->plateChar, str.c_str());
	//printf("License Plate: %s . \n", pAI_pi->plateChar);
	return true;
}