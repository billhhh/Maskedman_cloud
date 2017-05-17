#include "lib_flame_detect.h"
#include "Controller.h"

using namespace std;

//算法外部接口函数
unsigned int flame_detect(void* pVoid, int size){
	AI_Flame_Detect* pAI_id = (AI_Flame_Detect*)pVoid;
	Controller flameDetect;
	flameDetect.setParam(pAI_id);
	if(!flameDetect.startDetect())
		return AlgorithmResult::AR_INTERNAL;

	return AlgorithmResult::AR_DONE;
}

//Hello测试
void sayHello(){
	printf("say Hello!\n");
}
