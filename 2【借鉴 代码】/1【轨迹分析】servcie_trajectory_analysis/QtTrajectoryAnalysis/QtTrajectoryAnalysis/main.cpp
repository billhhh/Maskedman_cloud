#include "qttrajectoryanalysis.h"
#include <QtGui/QApplication>
#include <QTextCodec>
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

	qRegisterMetaType<cv::Mat>("Mat");
	qRegisterMetaType<cv::Mat>("cv::Mat&");
	qRegisterMetaType<cv::Mat>("cv::Mat*");

	QString qpath(argv[1]);
	std::string file = std::string((const char *)qpath.toLocal8Bit());

	QtTrajectoryAnalysis w;
	//////批量处理
	if( argc > 3){
		QStringList files;
		for( int i = 1; i < argc -1; ++i){
			files << QString::fromLocal8Bit(argv[i]);
		}
		w.setBatchInfo(files);
	}
	QString folder = QString::fromLocal8Bit(argv[argc-1]);
	w.setFolder(folder);

	if (!w.loadImage(file)){
		QMessageBox::information(0, QString::fromLocal8Bit("提示"), 
			QString::fromLocal8Bit("请正确打开程序！"));
		a.quit();
		return -1;
	}

	w.show();
	return a.exec();
}
