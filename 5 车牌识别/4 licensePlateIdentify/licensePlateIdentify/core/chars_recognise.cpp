#include "../include/chars_recognise.h"

/*! \namespace easypr
    Namespace where all the C++ EasyPR functionality resides
*/
namespace easypr{

CCharsRecognise::CCharsRecognise()
{
	//cout << "CCharsRecognise" << endl;
	m_charsSegment = new CCharsSegment();
	m_charsIdentify = new CCharsIdentify();
}

void CCharsRecognise::LoadANN(string s)
{
	m_charsIdentify->LoadModel(s.c_str());
}

//识别车牌字体，传回引用 plateLicense
int CCharsRecognise::charsRecognise(Mat plate, string& plateLicense)
{
	//车牌字符方块集合
	vector<Mat> matVec;

	string plateIdentify = "";

	int result = m_charsSegment->charsSegment(plate, matVec); //字符分割
	if (result == 0)
	{
		int num = matVec.size();
		for (int j = 0; j < num; j++)
		{
			Mat charMat = matVec[j];
			bool isChinses = false;

			//默认首个字符块是中文字符
			if (j == 0)
				isChinses = true;

			string charcater = m_charsIdentify->charsIdentify(charMat, isChinses);
			plateIdentify = plateIdentify + charcater;

			if( !isChinses){
				char buf[10];
				itoa(j, buf, 10);
				string str = file + "_" + buf +".jpg";
				imwrite(str, charMat);  //唯一输出口
			}
		}
	}

	plateLicense = plateIdentify;

	return 0;
}

void CCharsRecognise::setFileName(string s){
	file = s;
}


}	/*! \namespace easypr*/

