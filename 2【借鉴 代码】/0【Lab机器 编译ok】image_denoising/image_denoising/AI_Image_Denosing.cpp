#include "AI_Image_Denosing.h"

AI_Image_Denosing::AI_Image_Denosing(){
	init();
};

AI_Image_Denosing::~AI_Image_Denosing(){
	destroy();
}

void AI_Image_Denosing::init(){
	kel = 0;
	pImgSrc = &imageSrc;
}

void AI_Image_Denosing::destroy(){

}