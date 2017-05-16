#include "serviceDll.h"

#define FUNC_EXTRACT "extractingData"
#define FUNC_ANALYZE "analyzingData"
#define FUNC_AUTO "autoAnalyzingData"

ServiceDLL* ServiceDLL::instance = NULL;

ServiceDLL* ServiceDLL::getInstance(){
	if( instance == NULL)
		instance = new ServiceDLL();
	return instance;
}

ServiceDLL::ServiceDLL(){
}

ServiceDLL::~ServiceDLL(){
	releaseDLL();
}

bool ServiceDLL::execServiceAnalyze(AI_TrajectoryAnalysis*& pda){
/*
		if( !m_ta->getInstance()->execAnalyzing(m_file, m_path, img))
			return false;*/
	if ( !analyzingData((void*)pda, sizeof(pda))){
		printf("Analyze Fail.\n");
		return false;
	}else{
		printf("Analyze End.\n");
	}
	return true;
}

bool ServiceDLL::execServiceAuto(AI_TrajectoryAnalysis*& pta){
	if (!autoAnalyzingData((void*)pta, sizeof(pta)))
		return false;
	return true;
}

///// Õ∑≈HDLL
void ServiceDLL::releaseDLL(){
	qLib.unload();
	analyzingData = NULL;
	autoAnalyzingData = NULL;
}

////º”‘ÿHDLL
bool ServiceDLL::loadDLL(std::string path){
	releaseDLL();
	qLib.setFileName(QString::fromStdString(path));
	if (!qLib.load()){
		printf("qLibrary ‘ÿ»Î ß∞‹£°\n");
		return false;
	}

	analyzingData = (AnalyzingData)  qLib.resolve( FUNC_ANALYZE);
	autoAnalyzingData = (AutoAnalyzingData)  qLib.resolve( FUNC_AUTO);
	sayHello = (SayHello)  qLib.resolve( "sayHello");

	if( analyzingData == NULL || 
		autoAnalyzingData == NULL || sayHello == NULL){
		releaseDLL();
		return false;
	}
	sayHello();
	return true;
}