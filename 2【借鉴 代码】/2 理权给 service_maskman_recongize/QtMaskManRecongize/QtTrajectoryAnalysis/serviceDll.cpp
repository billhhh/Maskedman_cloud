#include "serviceDll.h"

#define FUNC_EXTRACT "detectMaskMan"
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

bool ServiceDLL::execServiceExtract(char* eventInfo, cv::Mat*& img, 
	int& percent, bool& running){
	if (!maskManRec(m_file, m_path, 0.5, eventInfo, img, percent, running))
		return false;
	running = true;
/*
	if( !detectMaskMan(m_file, m_path, 0.5, eventInfo, img, percent, running))
		return false;*/
	return true;
}

/////�ͷ�DLL
void ServiceDLL::releaseDLL(){
	qLib.unload();
	maskManRec = NULL;
}

////����DLL
bool ServiceDLL::loadDLL(std::string path){
	////�ͷ�dll
	releaseDLL();

	qLib.setFileName(QString::fromStdString(path));
	if (!qLib.load()){
		printf("qLibrary ����ʧ�ܣ�\n");
		return false;
	}

	sayHello = (SayHello) qLib.resolve("sayHello");
	sayHello();
	//maskManRec = (DetectMaskMan) qLib.resolve(FUNC_EXTRACT);
	maskManRec = (DetectMaskMan) qLib.resolve("detectMaskMan");
	if( maskManRec == NULL){
		releaseDLL();

		return false;
	}

	return true;
}

void ServiceDLL::setPara(std::string file, std::string path){
	m_file = file;
	m_path = path;
}