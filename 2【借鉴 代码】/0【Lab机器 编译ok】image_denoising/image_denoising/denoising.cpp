#include"denoising.h"
#include "AI_Image_Denosing.h"
using namespace cv;

uint image_denoising(void* pVoid, int size){
	AI_Image_Denosing* pAI_id = (AI_Image_Denosing*)pVoid;
	Mat &src = pAI_id->imageSrc;
	Mat &dst = pAI_id->image;
	int k = pAI_id->kel;

	while (k % 2 == 0 || k == 1 || k == 0){
		k++;
	}
	Mat img ;
	medianBlur(src, img, k);
	dst = img.clone();
	return AlgorithmResult::AR_DONE;
}
