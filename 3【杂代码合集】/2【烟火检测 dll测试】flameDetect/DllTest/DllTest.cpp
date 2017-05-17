// DllTest.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "AI_Flame_Detect.h"
//#include <qLibrary.h>
#include "iostream"
#include <Windows.h>
using namespace std;
void main(){
	/*typedef int (*Test)(void* ,int);//函数指针类型
	HINSTANCE Hint = LoadLibrary(_T("flameDetect.dll"));//加载我们刚才生成的dll*/
	typedef uint (*FlameDetect)(void*, int);//函数指针类型
	typedef void (*SayHello)();//函数指针类型
	HINSTANCE Hint = LoadLibrary(_T("FlameDetect_dll.dll"));//加载我们刚才生成的dll
	FlameDetect flameRec = (FlameDetect)GetProcAddress(Hint,"flame_detect");//取得dll导出的add方法
	SayHello sayHello = (SayHello)GetProcAddress(Hint,"sayHello");
	sayHello();

	/*typedef uint (_cdecl * FlameDetect)( void*, int);
	typedef void (_cdecl * SayHello)();
	FlameDetect flameRec;
	SayHello sayHello;

	QLibrary qLib;
	qLib.setFileName(QString::fromStdString("flameDetect.dll"));
	if (!qLib.load()){
		printf("qLibrary 载入失败！\n");
		return false;
	}
	sayHello = (SayHello) qLib.resolve("sayHello");
	sayHello();
	flameRec = (FlameDetect) qLib.resolve("flame_detect");//接口函数
	if( flameRec == NULL){
		releaseDLL();
		return false;
	}*/
	
	string filePath;
	string savePath;
	int type;

	//filePath ="E:/烟火检测/FireTestVideo/barbeqraw.avi";//OK 近景无干扰     
	//filePath ="E:/烟火检测/FireTestVideo/fBackYardFire.avi";//OK            
	//filePath ="E:/烟火检测/FireTestVideo/sWasteBasket.avi";//OK             

	//远景
	//filePath ="E:/烟火检测/FireTestVideo/test1.avi"; 
	filePath ="E:/烟火检测/FireTestVideo/test2.avi";                         	  
	//filePath ="E:/烟火检测/FireTestVideo/forest5.avi";
	//filePath ="E:/烟火检测/FireTestVideo/Pelco_Colakli.avi";                
	//filePath ="E:/烟火检测/FireTestVideo/Smoke_Manavgat_Raw.avi";  
	/*if(argc==2)
		filePath = argv[1];
	else
		return;		*/
	
	savePath = "C:/Users/Administrator/Desktop/flameDetect_result/";
	
	std::cout<<"近景(1) or 远景(2):";
	std::cin>>type;

	
	AI_Flame_Detect afd;
	strcpy(afd.file, filePath.c_str());//文件全路径
	strcpy(afd.folder, savePath.c_str());//保存路径
	afd.type = type; //类型：近景远景
	
	if(!flameRec((void*)&afd, sizeof(afd)))
		printf("Flame detect failed. \n");
	string str = afd.eventInfo;
	std::cout<<str;

	/*qLib.unload();
	maskManRec = NULL;*/

	system("pause");
}