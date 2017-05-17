// DllTest.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include "AI_Flame_Detect.h"
//#include <qLibrary.h>
#include "iostream"
#include <Windows.h>
using namespace std;
void main(){
	/*typedef int (*Test)(void* ,int);//����ָ������
	HINSTANCE Hint = LoadLibrary(_T("flameDetect.dll"));//�������Ǹղ����ɵ�dll*/
	typedef uint (*FlameDetect)(void*, int);//����ָ������
	typedef void (*SayHello)();//����ָ������
	HINSTANCE Hint = LoadLibrary(_T("FlameDetect_dll.dll"));//�������Ǹղ����ɵ�dll
	FlameDetect flameRec = (FlameDetect)GetProcAddress(Hint,"flame_detect");//ȡ��dll������add����
	SayHello sayHello = (SayHello)GetProcAddress(Hint,"sayHello");
	sayHello();

	/*typedef uint (_cdecl * FlameDetect)( void*, int);
	typedef void (_cdecl * SayHello)();
	FlameDetect flameRec;
	SayHello sayHello;

	QLibrary qLib;
	qLib.setFileName(QString::fromStdString("flameDetect.dll"));
	if (!qLib.load()){
		printf("qLibrary ����ʧ�ܣ�\n");
		return false;
	}
	sayHello = (SayHello) qLib.resolve("sayHello");
	sayHello();
	flameRec = (FlameDetect) qLib.resolve("flame_detect");//�ӿں���
	if( flameRec == NULL){
		releaseDLL();
		return false;
	}*/
	
	string filePath;
	string savePath;
	int type;

	//filePath ="E:/�̻���/FireTestVideo/barbeqraw.avi";//OK �����޸���     
	//filePath ="E:/�̻���/FireTestVideo/fBackYardFire.avi";//OK            
	//filePath ="E:/�̻���/FireTestVideo/sWasteBasket.avi";//OK             

	//Զ��
	//filePath ="E:/�̻���/FireTestVideo/test1.avi"; 
	filePath ="E:/�̻���/FireTestVideo/test2.avi";                         	  
	//filePath ="E:/�̻���/FireTestVideo/forest5.avi";
	//filePath ="E:/�̻���/FireTestVideo/Pelco_Colakli.avi";                
	//filePath ="E:/�̻���/FireTestVideo/Smoke_Manavgat_Raw.avi";  
	/*if(argc==2)
		filePath = argv[1];
	else
		return;		*/
	
	savePath = "C:/Users/Administrator/Desktop/flameDetect_result/";
	
	std::cout<<"����(1) or Զ��(2):";
	std::cin>>type;

	
	AI_Flame_Detect afd;
	strcpy(afd.file, filePath.c_str());//�ļ�ȫ·��
	strcpy(afd.folder, savePath.c_str());//����·��
	afd.type = type; //���ͣ�����Զ��
	
	if(!flameRec((void*)&afd, sizeof(afd)))
		printf("Flame detect failed. \n");
	string str = afd.eventInfo;
	std::cout<<str;

	/*qLib.unload();
	maskManRec = NULL;*/

	system("pause");
}