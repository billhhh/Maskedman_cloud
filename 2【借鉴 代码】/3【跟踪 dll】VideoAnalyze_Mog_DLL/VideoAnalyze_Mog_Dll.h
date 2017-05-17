#ifdef  DLL_API
#else
#define DLL_API _declspec(dllimport)
#endif

#include "EventNode.h"
#include <time.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

extern "C"  __declspec(dllexport) vector<EventNode>* execAnalyze(std::string &videoName , int x_min , int y_min , std::string &savePath , Mat* &currentImage , int & progress ,
	int processInterval , bool &isReadFromFile , bool & isContinue , bool & isFinished);

extern "C"  __declspec(dllexport) bool writeEventVideo(vector<EventNode>&eventList , std::string &originVideoPath , std::string &savePath ,int &progress, int interval ,bool & isContinue);

class VideoAnalyze_Mog
{
public:
	static VideoAnalyze_Mog* instance;

	static VideoAnalyze_Mog* getInstance();

	vector<EventNode>* analyze(std::string &videoName ,  int x_min , int y_min , std::string &savePath , Mat* &currentImage , int & progress ,
		int processInterval , bool &isReadFromFile , bool & isContinue , bool & isFinished);   //������Ƶ���õ��¼����Ϻ�Ũ����Ƶ

	void refineSegments(vector<EventNode>* eventList ,  int x_min , int y_min , Mat img, Mat& mask, Mat& dst , int frameNum, int fps , int frameCount); //����Ҫ���õĺ���

	int findLength(int a[]);   //�ж��Ƿ�ͬһ������ĸ�������

	bool isTheSameRect(Rect r1, Rect r2);  //�ж�2�������Ƿ�Ϊͬһ����

	EventNode insertEventNode(vector<EventNode> *eventList, Rect r, int frameNum);

	bool searchEventList(vector<EventNode> *eventList, Rect r2, EventNode&node, int frameNum); //��������,�����û����ͬ�¼�

	int traverseEventList(vector<EventNode> *eventList, int endFrame, int fps);  //�����������²�ɾ�����

	char* getTimeFromFrameNum(int frameNum, int fps);

	void saveEventToFile(std::string &videoName , vector<EventNode> eventList , std::string &savePath , int interval , int fps);     //���¼��������ļ�

	void splitString(std::string& s, std::string& delim,std::vector< std::string >& ret);
};