#ifndef WAITINGPROGRESSBAR_H
#define WAITINGPROGRESSBAR_H

#include <QWidget>
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

private:
	Ui::WaitingProgressBar ui;
	int fileCnt;

signals:
	void procDone();

private slots:
	void on_cmdLinkBtn_clicked();

};

#endif // WAITINGPROGRESSBAR_H
