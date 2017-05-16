#include "testDll.h"
#include <iostream>

using namespace std;

void TestDll::testHello()
{
	qLib.setFileName("..\\..\\Release\\ai_maskman_detect.dll");
	this->detection = (MaskManDetect) qLib.resolve("maskman_detect");
	this->sayHello = (SayHello)  qLib.resolve( "sayHello");

	//合成传递过去的ai对象
	AI_Maskman_Detect ai;
	ai.forwardInfo = ForwardResult::NORMAL;
	ai.isRun = true;
	strcpy(ai.file,"E:\\Project\\faceDetect\\faceDetect\\4.mp4");
	strcpy(ai.folder,"F:\\mmr_img");

	sayHello();
	cout<<"Error Num = "<<detection(&ai,sizeof(ai))<<"\n";

}