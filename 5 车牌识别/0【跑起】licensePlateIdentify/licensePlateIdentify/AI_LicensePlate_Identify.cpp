#include "AI_LicensePlate_Identify.h"

AI_LicensePlate_Identify::AI_LicensePlate_Identify(){
	init();
};

AI_LicensePlate_Identify::~AI_LicensePlate_Identify(){
	destroy();
}

void AI_LicensePlate_Identify::init(){
	imageData.pImgData = NULL;
	imageData.pImgData = NULL;
}

void AI_LicensePlate_Identify::destroy(){

}

void AI_LicensePlate_Identify::createImageData(ImageData* imgData, int rows, int cols, int channels, uchar* pData){
	releaseImageData(imgData);

	imgData->imgCols = cols;
	imgData->imgRows = rows;
	imgData->imgChannels = channels;
	int n = cols*rows*channels;
	imgData->pImgData = new uchar[n];
	for( int i = 0; i < n; ++i){
		imgData->pImgData[i] = *(pData+i);
	}
}

void AI_LicensePlate_Identify::releaseImageData(ImageData* imgData){
	if( imgData->pImgData != NULL){
		delete []imgData->pImgData;
		imgData->pImgData = NULL;
	}
}