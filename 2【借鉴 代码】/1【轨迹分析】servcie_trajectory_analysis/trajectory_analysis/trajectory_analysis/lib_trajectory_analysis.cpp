
#include "lib_trajectory_analysis.h"
#include "trajectory_analysis.h"

using namespace std;
using namespace cv;

uint analyzingData(void* pVoid, int length){
	AI_TrajectoryAnalysis* pAI_ta = (AI_TrajectoryAnalysis*)pVoid;
	trajectory_analysis* pTrajectoryAnalysis;
	if( !(pTrajectoryAnalysis->getInstance()->execAnalyzing(pAI_ta)))
		return AlgorithmResult::AR_END;
	return AlgorithmResult::AR_DONE;
}

uint autoAnalyzingData(void* pVoid, int length){
	AI_TrajectoryAnalysis* pAI_ta = (AI_TrajectoryAnalysis*)pVoid;
	trajectory_analysis* pTrajectoryAnalysis;
	if( !(pTrajectoryAnalysis->getInstance()->execExtracting(pAI_ta)))
		return AlgorithmResult::AR_END;
	std::string fileTxt = pTrajectoryAnalysis->getInstance()->getVideoTxtName();
	if( !(pTrajectoryAnalysis->getInstance()->execAnalyzing(fileTxt, pAI_ta->folder, pAI_ta->pImg)))
		return AlgorithmResult::AR_END;
	return AlgorithmResult::AR_DONE;
}

void sayHello(){
	printf("say Hello!\n");
}