#ifndef WAITINGPROGRESSBAR_H
#define WAITINGPROGRESSBAR_H

#include <QWidget>
#include <QMouseEvent>
#include "ui_waitingprogressbar.h"

class WaitingProgressBar : public QWidget
{
	Q_OBJECT

public:
	WaitingProgressBar(QWidget *parent = 0);
	~WaitingProgressBar();
	void setInfo(int );
	void updateProgress(int i);
	void updateLabel(int i);
	void setBtnTip(QString);
	void setLabelFile(QString);
	void setProcStatus(bool);
	//void 
protected:
	void closeEvent(QCloseEvent *e);
	//鼠标按下事件
	void mousePressEvent(QMouseEvent *event);
	//鼠标释放事件
	void mouseReleaseEvent(QMouseEvent *event);
	//鼠标移动事件
	void mouseMoveEvent(QMouseEvent *event);

private:
	Ui::WaitingProgressBar ui;
	int fileCnt;
	bool procEnd;

	QPoint move_point; //移动的距离
	bool mouse_press; //鼠标按下

signals:
	void procStop();
	void procDone();

private slots:
	void on_cmdLinkBtn_clicked();
	void on_pBtn_exit_clicked();
	void on_pBtn_minimize_clicked();
};

#endif // WAITINGPROGRESSBAR_H
