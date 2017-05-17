#include "AI_Flame_Detect.h"
#include "Controller.h"
#include "lib_flame_detect.h"
#include "iostream"
using namespace std;

void main(){
	string filePath;
	string savePath;
	int type;

	//filePath ="E:/ÑÌ»ð¼ì²â/FireTestVideo/barbeqraw.avi";//OK ½ü¾°ÎÞ¸ÉÈÅ     
	//filePath ="E:/ÑÌ»ð¼ì²â/FireTestVideo/fBackYardFire.avi";//OK            
	//filePath ="E:/ÑÌ»ð¼ì²â/FireTestVideo/sWasteBasket.avi";//OK             

	//Ô¶¾°
	//filePath ="E:/ÑÌ»ð¼ì²â/FireTestVideo/test1.avi"; 
	filePath ="E:/ÑÌ»ð¼ì²â/FireTestVideo/test2.avi";                         	  
	//filePath ="E:/ÑÌ»ð¼ì²â/FireTestVideo/forest5.avi";
	//filePath ="E:/ÑÌ»ð¼ì²â/FireTestVideo/Pelco_Colakli.avi";                
	//filePath ="E:/ÑÌ»ð¼ì²â/FireTestVideo/Smoke_Manavgat_Raw.avi";  

	/*if(argc==2)
		filePath = argv[1];
	else
		return;*/
	savePath = "C:/Users/Administrator/Desktop/flameDetect_result/";
	
	std::cout<<"½ü¾°(1) or Ô¶¾°(2):";
	std::cin>>type;

	AI_Flame_Detect afd;
	strcpy(afd.file, filePath.c_str());//ÎÄ¼þÈ«Â·¾¶
	strcpy(afd.folder, savePath.c_str());//±£´æÂ·¾¶
	afd.type = type; //ÀàÐÍ£º½ü¾°Ô¶¾°

	if(!flame_detect((void*)&afd, sizeof(afd)))
		printf("Flame detect failed. \n");
	string str = afd.eventInfo;
	std::cout<<str;
	system("pause");
}