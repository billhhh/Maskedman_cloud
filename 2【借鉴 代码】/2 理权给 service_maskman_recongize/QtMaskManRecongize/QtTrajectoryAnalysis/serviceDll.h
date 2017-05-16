#ifndef _H_SERVICEDLL_CAI_H_
#define _H_SERVICEDLL_CAI_H_

#include <opencv2/opencv.hpp>
#include <string>
#include <qLibrary.h>
#include <QMainWindow>

typedef bool (_cdecl * DetectMaskMan)(const std::string& ,const  std::string&, 
	float, char* , cv::Mat*&, 
	int&, bool&);
typedef void (_cdecl * SayHello)();

class ServiceDLL{
protected:
	static ServiceDLL* instance;
	ServiceDLL();

	std::string m_file;
	std::string m_path;

	QLibrary qLib;
	DetectMaskMan maskManRec;
	SayHello sayHello;

public:
	~ServiceDLL();
	static ServiceDLL* getInstance();

	void setPara(std::string, std::string);
	bool execServiceExtract(char*, cv::Mat*&, int&, bool&);
	void releaseDLL();
	bool loadDLL(std::string path);

/*std::wstring s2ws(const std::string& s){
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r.c_str();
	}*/
};

#endif