#include "trajectory_analysis.h"
using std::string;
using namespace::cv;

trajectory_analysis* trajectory_analysis::instance = NULL;

trajectory_analysis::trajectory_analysis(){
	init();
}

trajectory_analysis::~trajectory_analysis(){
	destroy();
	printf("trajectory_analysis release.\n");
}

void trajectory_analysis::init(){
}

void trajectory_analysis::destroy(){
	if( instance != NULL){
		delete instance;
		instance = NULL;
	}
}

trajectory_analysis* trajectory_analysis::getInstance(){
	if( instance == NULL){
		instance = new trajectory_analysis();
	}
	return instance;
}

bool trajectory_analysis::execExtracting(AI_TrajectoryAnalysis* pAI_ta){
	m_videoTrack->GetInstance()->setOutputFolder(pAI_ta->folder);
	string str = pAI_ta->file;
	int ret = m_videoTrack->GetInstance()->openVideo( str);
	if( !ret){
		printf("Open video \" %s\" failed.", str.c_str());
		return false;
	}
	/////初始化参数
	m_videoTrack->GetInstance()->initPara();
	///开始文件记录
	if( !m_videoTrack->GetInstance()->startFileRecord(str)){
		printf("Open video record \" %s\" failed.", str.c_str());
		return false;
	}

	bool &running = pAI_ta->isRun;
	uint &percent = pAI_ta->percent;
	running = true;
	percent = 0;
	
	Mat* &pframe = pAI_ta->pImg;
	pframe = &pAI_ta->image;
	Mat img;
	while( running){
		percent = m_videoTrack->GetInstance()->getFrameNum();
		if( !m_videoTrack->GetInstance()->getEveryFrame(img))
			break;
		pAI_ta->lock();
		pAI_ta->image = img.clone();
		pAI_ta->unlock();
	}
	pAI_ta->lock();
	pframe->release();
	pAI_ta->unlock();

	m_videoTrack->GetInstance()->endFileRecord();

	clear();
	if( false == running)
		return false;
	return true;
}

bool trajectory_analysis::execAnalyzing(AI_TrajectoryAnalysis* pAI_da){
	m_dataAnalysis->GetInstance()->setPath(pAI_da->folder);

	string str = pAI_da->file;
	//读入数据文件
	int ret = m_dataAnalysis->GetInstance()->readData(str);
	if( !ret){
		printf("Open file \" %s\" failed.", str.c_str());
		return false;
	}

	//得到文件路径
	int pos = str.find_last_of('.');
	if( pos > 0)
		str = str.substr(0, pos);
	str += ".jpg";

	//设置图片并开始分析
	if( !m_dataAnalysis->GetInstance()->setImage(str))
		return false;
	m_dataAnalysis->GetInstance()->analysis();
	m_dataAnalysis->GetInstance()->getDrawImg(m_draw);
	pAI_da->lock();
	pAI_da->image = m_draw.clone();
	pAI_da->unlock();
	return true;
}


bool trajectory_analysis::execAnalyzing(string str, string path, Mat*& img){
	m_dataAnalysis->GetInstance()->setPath(path);

	//读入数据文件
	int ret = m_dataAnalysis->GetInstance()->readData(str);
	if( !ret){
		printf("Open file \" %s\" failed.", str.c_str());
		return false;
	}

	//得到文件路径
	int pos = str.find_last_of('.');
	if( pos > 0)
		str = str.substr(0, pos);
	str += ".jpg";

	//设置图片并开始分析
	if( !m_dataAnalysis->GetInstance()->setImage(str))
		return false;
	m_dataAnalysis->GetInstance()->analysis();
	m_dataAnalysis->GetInstance()->getDrawImg(m_draw);
	*img = m_draw.clone();
	return true;
}

std::string trajectory_analysis::getVideoTxtName() const{
	return m_videoTrack->GetInstance()->getFileName();
}

void trajectory_analysis::clear(){
	if( !m_image.empty()){
		m_image.release();
	}
	m_videoTrack->GetInstance()->clear();
}