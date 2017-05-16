#include <QtGui/QApplication>
#include "qtmaskmandetect.h"
#include "testDll.h"

void testHello()
{
	TestDll testdll;
	testdll.testHello();
}

int main(int argc, char *argv[])
{
	testHello();
// 	QApplication a(argc, argv);
// 	QtMaskManDetect w;
// 	w.show();
// 	return a.exec();

	system("pause");
	return 0;
}
