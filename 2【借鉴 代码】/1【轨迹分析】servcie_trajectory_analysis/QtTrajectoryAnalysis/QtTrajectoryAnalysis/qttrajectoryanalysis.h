#ifndef QTTRAJECTORYANALYSIS_H
#define QTTRAJECTORYANALYSIS_H

#include <QtGui/QDialog>
#include <QtGui/QMainWindow>
#include "ui_qttrajectoryanalysis.h"
#include <string>
#include <opencv2/opencv.hpp>
#include "extractThread.h"
#include "updateThread.h"
#include "analyzeThread.h"
#include "waitingprogressbar.h"
#include <QMetaType>
#include "AlgorithmInterface/algorithmInterface_Global.h"

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
	extractThread *m_extractThread;
	updateThread *m_updateThread;
	analyzeThread *m_analyzeThread;
	std::string outputFolder; //输出文件夹
	std::string videoName;//文件夹名字
	bool isClicked;
	QIcon iconRun;
	QIcon iconStop;
	QGraphicsScene imgScene;
	QGraphicsScene dstScene;

	///批量处理
	WaitingProgressBar waitingDlg;
	QStringList fileList;
	QString folder;
	bool isBatch;
	int batchCnt;

	AI_TrajectoryAnalysis AI_ta;
	AI_TrajectoryAnalysis AI_da;

private:
	bool init();
	void destroy();
	void setImage(cv::Mat&, QGraphicsScene*, QGraphicsView*);
	void setRun();
	void setStop();
	void batchFiles();
	void continueBatch();
	std::string getFileName(const QString&, QString, QString&);
	QImage const copy_mat_to_qimage(cv::Mat const &mat, QImage::Format format);
	QImage const mat_to_qimage_cpy(cv::Mat &mat, int qimage_format);

private slots:
	void on_pBtn_Run_clicked();
	void on_pBtn_OutputFolder_clicked();
	void on_pBtn_Check_clicked();
	void on_pBtn_Analysis_clicked();
	void on_pBtn_quit_clicked();
	void updateProcessBar(int);

	void getThreadEnd(cv::Mat*);
	void getThreadStop();
	void getThreadFail();
	void getFrameImg(cv::Mat*);

	void AnalyzeThreadEnd(cv::Mat*);
	void AnalyzeThreadFail();

	void closeWaitDlg();
	void batchDone();
};
Q_DECLARE_METATYPE(cv::Mat);

#endif // QTTRAJECTORYANALYSIS_H
