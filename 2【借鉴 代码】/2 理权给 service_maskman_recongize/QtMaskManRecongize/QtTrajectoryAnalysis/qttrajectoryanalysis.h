#ifndef QTTRAJECTORYANALYSIS_H
#define QTTRAJECTORYANALYSIS_H

#include <QtGui/QDialog>
#include <QtGui/QMainWindow>
#include "ui_qttrajectoryanalysis.h"
#include <string>
#include <opencv2/opencv.hpp>
#include "updateThread.h"
#include "waitingprogressbar.h"
#include <QMetaType>

class QtTrajectoryAnalysis : public QDialog
{
	Q_OBJECT

public:
	QtTrajectoryAnalysis(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QtTrajectoryAnalysis();

	bool loadImage(std::string);
	void setFolder(QString);
	void setBatchInfo(QStringList&);

private:
	Ui::QtTrajectoryAnalysisClass ui;

	ServiceDLL* serviceDll;
	extractThread *m_extractThread;	//处理线程
	updateThread *m_updateThread;//负责把处理情况反馈
	std::string outputFolder; //输出文件夹
	std::string videoName;//文件夹名字
	QGraphicsScene imgScene;

	///批量处理
	WaitingProgressBar waitingDlg;
	QStringList fileList;
	QString folder;
	bool isBatch;
	int batchCnt;

private:
	bool init();
	void destroy();
	void setImage(cv::Mat&);
	void batchFiles();
	void continueBatch();
	std::string getFileName(const QString&, QString, QString&);

	QImage const copy_mat_to_qimage(cv::Mat const &mat, QImage::Format format);
	QImage const mat_to_qimage_cpy(cv::Mat &mat, int qimage_format);

private slots:
	void on_pBtn_Run_clicked();
	void on_pBtn_Stop_clicked();
	void on_pBtn_OutputFolder_clicked();
	void on_pBtn_Check_clicked();
	void on_pBtn_quit_clicked();
	void updateProcessBar(int);

	void getThreadEnd();
	void getThreadStop();
	void getThreadFail();

	void updateImg(cv::Mat&);
	void updateEventInfo(std::string);

	void closeWaitDlg();
	void batchDone();
};
Q_DECLARE_METATYPE(cv::Mat);
Q_DECLARE_METATYPE(std::string);
#endif // QTTRAJECTORYANALYSIS_H
