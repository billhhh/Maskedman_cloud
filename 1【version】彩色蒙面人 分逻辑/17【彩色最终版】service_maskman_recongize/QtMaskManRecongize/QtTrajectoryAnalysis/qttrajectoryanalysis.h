#ifndef QTTRAJECTORYANALYSIS_H
#define QTTRAJECTORYANALYSIS_H

#include <QtGui/QDialog>
#include <QtGui/QMainWindow>
#include "ui_qttrajectoryanalysis.h"
#include <string>
#include <opencv2/opencv.hpp>
#include "updateThread.h"
#include "waitingprogressbar.h"
#include "AlgorithmInterface/AI_Maskman_Detect.h"
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
	bool loadData();

private:
	Ui::QtTrajectoryAnalysisClass ui;

	ServiceDLL* serviceDll;
	extractThread *m_extractThread;
	updateThread *m_updateThread;
	std::string outputFolder; //输出文件夹
	std::string videoName;//文件夹名字
	bool isClicked;
	QGraphicsScene imgScene;

	///批量处理
	WaitingProgressBar waitingDlg;
	QStringList fileList;
	QString folder;
	bool isBatch;
	int batchCnt;

	AI_Maskman_Detect AI_md;

	QPoint move_point; //移动的距离
	bool mouse_press; //鼠标按下
private:
	bool init();
	void destroy();
	void setImage(cv::Mat&);
	void setRun();
	void setStop();
	void batchFiles();
	void continueBatch();
	std::string getFileName(const QString&, QString, QString&);
	QString detectLog;

	QImage const copy_mat_to_qimage(cv::Mat const &mat, QImage::Format format);
	QImage const mat_to_qimage_cpy(cv::Mat &mat, int qimage_format);

protected:
	//鼠标按下事件
	void mousePressEvent(QMouseEvent *event);
	//鼠标释放事件
	void mouseReleaseEvent(QMouseEvent *event);
	//鼠标移动事件
	void mouseMoveEvent(QMouseEvent *event);

private slots:
	void on_pBtn_Run_clicked();
	void on_pBtn_forward_clicked();
	void on_pBtn_backward_clicked();
	void on_pBtn_OutputFolder_clicked();
	void on_pBtn_exit_clicked();
	void on_pBtn_minimize_clicked();

	void updateProcessBar(int);

	void getThreadEnd();
	void getThreadStop();
	void getThreadFail();

	void updateImg(cv::Mat*);
	void updateEventInfo(std::string);

	void closeWaitDlg();
	void batchDone();
};
Q_DECLARE_METATYPE(cv::Mat);
Q_DECLARE_METATYPE(std::string);
#endif // QTTRAJECTORYANALYSIS_H
