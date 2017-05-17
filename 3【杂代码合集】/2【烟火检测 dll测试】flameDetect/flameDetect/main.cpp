#include "AI_Flame_Detect.h"
#include "Controller.h"
#include "lib_flame_detect.h"
#include "iostream"
using namespace std;

void main(){
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
		return;*/
	savePath = "C:/Users/Administrator/Desktop/flameDetect_result/";
	
	std::cout<<"����(1) or Զ��(2):";
	std::cin>>type;

	AI_Flame_Detect afd;
	strcpy(afd.file, filePath.c_str());//�ļ�ȫ·��
	strcpy(afd.folder, savePath.c_str());//����·��
	afd.type = type; //���ͣ�����Զ��

	if(!flame_detect((void*)&afd, sizeof(afd)))
		printf("Flame detect failed. \n");
	string str = afd.eventInfo;
	std::cout<<str;
	system("pause");
}