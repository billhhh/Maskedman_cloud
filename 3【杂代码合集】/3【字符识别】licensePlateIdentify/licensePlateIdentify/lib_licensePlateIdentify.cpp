
#include "lib_licensePlateIdentify.h"
#include "licensePlateIdenify.h"
using namespace cv;
using namespace easypr;

uint licensePlate_identify(void* pVoid, int size){
	AI_LicensePlate_Identify* pAI_lpi = (AI_LicensePlate_Identify*)pVoid;
	LicensePlateIdentify* pLicensePlateIdentify;
	if( !pLicensePlateIdentify->getInstance()->plateIdentify(pAI_lpi))
		return AlgorithmResult::AR_END;
	return AlgorithmResult::AR_DONE;
}

uint licensePlate_identify_file(void* pVoid, int size){
	AI_LicensePlate_Identify* pAI_lpi = (AI_LicensePlate_Identify*)pVoid;
	LicensePlateIdentify* pLicensePlateIdentify;
	if( !pLicensePlateIdentify->getInstance()->plateIdentify_file(pAI_lpi))
		return AlgorithmResult::AR_END;
	return AlgorithmResult::AR_DONE;
}

uint licensePlate_recognize(void* pVoid, int size){
	AI_LicensePlate_Identify* pAI_lpi = (AI_LicensePlate_Identify*)pVoid;
	LicensePlateIdentify* pLicensePlateIdentify;
	if( !pLicensePlateIdentify->getInstance()->plateRecognize(pAI_lpi))
		return AlgorithmResult::AR_END;
	return AlgorithmResult::AR_DONE;
}
