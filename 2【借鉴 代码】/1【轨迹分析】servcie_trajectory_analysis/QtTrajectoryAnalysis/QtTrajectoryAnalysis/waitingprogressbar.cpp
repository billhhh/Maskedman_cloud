
#include "waitingprogressbar.h"

WaitingProgressBar::WaitingProgressBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	fileCnt = 0;
	ui.progressBar->setRange(0, 100);
}

WaitingProgressBar::~WaitingProgressBar()
{

}

void WaitingProgressBar::setInfo(int i){
	fileCnt = i;
}

void WaitingProgressBar::on_cmdLinkBtn_clicked(){
	emit procDone();
}

void WaitingProgressBar::updateProgress(int i){
	ui.progressBar->setValue(i);
}

void WaitingProgressBar::updateLabel(int i){
	QString qstr = QString::number(i) + "/" + QString::number(fileCnt);
	ui.label_progress->setText(qstr);
}

void WaitingProgressBar::setBtnTip(QString qstr){
	ui.cmdLinkBtn->setText(qstr);
}

void WaitingProgressBar::setLabelFile(QString qstr){
	ui.label_file->setText(qstr);
}