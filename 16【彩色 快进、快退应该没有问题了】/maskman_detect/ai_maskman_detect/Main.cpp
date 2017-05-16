#include <iostream>
#include "Controller.h"

using namespace std;

int main()
{
	Controller ctrl;
	string path = "4.mp4";
	ctrl.startDetect(path);

	system("pause");
	return 0;
}