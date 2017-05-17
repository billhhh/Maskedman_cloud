
#include <opencv2/opencv.hpp>
#include "include/plate_recognize.h"
#include "lib_licensePlateIdentify.h"
#include "AI_LicensePlate_Identify.h"
#include <iostream>  
#include <fstream>
#include <string>
#include <vector>

using namespace cv;
using namespace easypr;

int main(int argc, char * argv[]){
	if( argc < 2)
		return 1;

	string filetxt = argv[1];
	string folder = argv[2];
	string buf;
	//¶ÁÈë¼ì²âÑù±¾  
	vector<string> img_tst_path;  
	ifstream img_tst( filetxt );  
	while( img_tst ){  
		if( getline( img_tst, buf ) ){  
			img_tst_path.push_back( buf );  
		}  
	}  
	img_tst.close(); 

	for( int i = 0; i < img_tst_path.size(); ++i){
		string file = img_tst_path[i];

		int pos = file.find_last_of("\\") +1;
		string strfile = file.substr( pos, file.length() - pos);
		pos = strfile.find_last_of(".jpg.plate");
		strfile = strfile.substr(0, pos-13);
		string output = folder + '\\' + strfile;

		AI_LicensePlate_Identify ai_lpi;
		strcpy(ai_lpi.file, output.c_str());
	/*
		if(!licensePlate_recognize((void*)&ai_lpi, sizeof(ai_lpi)))
			printf("License Plate Recognize Failed. \n");

		string str = ai_lpi.plateChar;
		printf("License Plate: %s . \n", str.c_str());
	*/


		Mat image = imread(file);
		ai_lpi.imageData.imgCols = image.cols;
		ai_lpi.imageData.imgRows = image.rows;
		ai_lpi.imageData.imgChannels = image.channels();
		ai_lpi.imageData.pImgData = image.data;
		if(!licensePlate_identify((void*)&ai_lpi, sizeof(ai_lpi)))
			printf("License Plate Recognize Failed. \n");
		string str = ai_lpi.plateChar;
		printf("License Plate: %s . \n", str.c_str());

	/*
		if(!licensePlate_identify_file((void*)&ai_lpi, sizeof(ai_lpi)))
			printf("License Plate Recognize Failed. \n");
		string str = ai_lpi.plateChar;
		printf("License Plate: %s . \n", str.c_str());*/
	}
	return 0;
}