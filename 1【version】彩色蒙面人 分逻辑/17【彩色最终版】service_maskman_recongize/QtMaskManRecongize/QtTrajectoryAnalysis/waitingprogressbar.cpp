
#include "waitingprogressbar.h"

WaitingProgressBar::WaitingProgressBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//设置窗体标题栏隐藏并设置位于顶层
	this->setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	//可获取鼠标跟踪效果
	this->setMouseTracking(true);
	mouse_press = false;

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
	on_pBtn_exit_clicked();
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

void WaitingProgressBar::closeEvent(QCloseEvent *e){
	on_pBtn_exit_clicked();
}

void WaitingProgressBar::mousePressEvent(QMouseEvent *event){  
	if(event->button() == Qt::LeftButton){
		mouse_press = true;
		//鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
		move_point = event->pos();;
	}
}
void WaitingProgressBar::mouseMoveEvent(QMouseEvent *event){
	//若鼠标左键被按下
	if(mouse_press){
		//鼠标相对于屏幕的位置
		QPoint move_pos = event->globalPos();

		//移动主窗体位置
		this->move(move_pos - move_point);
	}
}

void WaitingProgressBar::mouseReleaseEvent(QMouseEvent *event){  
	//设置鼠标为未被按下
	mouse_press = false;
}

void WaitingProgressBar::on_pBtn_minimize_clicked(){
	this->showMinimized();
}

void WaitingProgressBar::on_pBtn_exit_clicked(){
	if( !procEnd)
		emit procStop();
	updateProgress(0);
	this->close();
}

void WaitingProgressBar::setProcStatus(bool val){
	procEnd = val;
}