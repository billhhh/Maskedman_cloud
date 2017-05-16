#include "qttrajectoryanalysis.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include "serviceDll.h"

using namespace cv;
using namespace std;

QtTrajectoryAnalysis::QtTrajectoryAnalysis(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	if( !init()){
		QMessageBox::information(0, QString::fromLocal8Bit("提示"), 
			QString::fromLocal8Bit("服务初始化失败！"));
	}
}

QtTrajectoryAnalysis::~QtTrajectoryAnalysis()
{
	destroy();
}

bool QtTrajectoryAnalysis::loadData(){
	if( !serviceDll->getInstance()->loadDLL("..\\service_dll\\ai_detectMaskMan.dll")){
		printf("load Dll failed!\n");
		return false;
	}
	return true;
}

bool QtTrajectoryAnalysis::init(){
	//设置窗体标题栏隐藏并设置位于顶层
	this->setWindowFlags(Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/);
	//可获取鼠标跟踪效果
	this->setMouseTracking(true);
	mouse_press = false;

	isBatch = false;
	fileList.clear();
	connect(&waitingDlg, SIGNAL(procDone()), this, SLOT(batchDone()));
	connect(&waitingDlg, SIGNAL(procStop()), this, SLOT(closeWaitDlg()));

	ui.progress_bar->setRange(0,100); 

	m_extractThread = new extractThread(this);
	connect(m_extractThread, SIGNAL(threadEnd()), this, SLOT(getThreadEnd()));
	connect(m_extractThread, SIGNAL(threadStop()), this, SLOT(getThreadStop()));
	connect(m_extractThread, SIGNAL(threadFail()), this, SLOT(getThreadFail()));
	
	m_updateThread = new updateThread(this);
	connect(m_updateThread, SIGNAL(sendSliderVal(int)), this, SLOT(updateProcessBar(int)));
	connect(m_updateThread, SIGNAL(sendImg(cv::Mat*)), this, SLOT(updateImg(cv::Mat*)));
	connect(m_updateThread, SIGNAL(sendInfo(std::string)), this, SLOT(updateEventInfo(std::string)));

	isClicked = false;
	return true;
}

void QtTrajectoryAnalysis::destroy(){
	if( m_extractThread != NULL){
		m_extractThread->terminate();
		delete m_extractThread;
		m_extractThread = NULL;
	}
	if( m_updateThread != NULL){
		m_updateThread->terminate();
		delete m_updateThread;
		m_updateThread = NULL;
	}
}

bool QtTrajectoryAnalysis::loadImage(std::string path){
	videoName = path;

	VideoCapture videoCapture;
	if ( !videoCapture.open(videoName)){
		QMessageBox::information(0, QString::fromLocal8Bit("提示"), 
			QString::fromLocal8Bit("打开视频失败!"));
		return false;
	}

	cv::Mat image;
	videoCapture>>image;

	//输出到界面
	setImage(image);

	videoCapture.release();
	return true;
}

void QtTrajectoryAnalysis::setImage(cv::Mat& image){
	//输出到界面
	cv::Mat img;
	cv::resize( image, img, cv::Size(480, 360), 0, 0, cv::INTER_CUBIC);
	QImage qimage= mat_to_qimage_cpy(img, 0);
	QPixmap pixmap = QPixmap::fromImage(qimage);
	imgScene.clear();
	imgScene.addPixmap(pixmap);
	ui.imageView->setScene(&imgScene);
	//ui.label_video->setPixmap(pixmap);
}

void QtTrajectoryAnalysis::setFolder(QString path){
	ui.lEd_OutputFolder->setText(path);
}

////点击输出文件夹
void QtTrajectoryAnalysis::on_pBtn_OutputFolder_clicked(){
	QString dir = QFileDialog::getExistingDirectory(
		this,
		QString::fromLocal8Bit("选择图片输出目录"),
		QDir::homePath (),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if( dir.size() > 0){
		this->outputFolder = dir.toStdString();
		ui.lEd_OutputFolder->setText(dir);
	}
}
///开始运行
void QtTrajectoryAnalysis::on_pBtn_Run_clicked(){
	if( isBatch){
		batchFiles();
		return;
	}
	if( isClicked){
		//m_extractThread->setStatus(false);
		AI_md.forwardInfo = ForwardResult::PAUSE;
		setRun();
		return;
	}else{
		AI_md.forwardInfo = ForwardResult::NORMAL;
		setStop();
		if( m_extractThread->isRunning())
			return;
	}

	outputFolder = ui.lEd_OutputFolder->text().toStdString();
	if( outputFolder.length() == 0){
		QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请确保输出路径正确"));
		return;
	}
	//loadImage(videoName); !!!!!!!!!!!!!!!!!!!!!!

	if( !m_extractThread->isRunning()){
		strcpy(AI_md.file, videoName.c_str());
		strcpy(AI_md.folder, outputFolder.c_str());
		m_extractThread->init(&AI_md);
		m_extractThread->start();
		m_updateThread->init(m_extractThread);
		m_updateThread->setIsBatch(isBatch);
		m_updateThread->start();

		setStop();
	}
}

void QtTrajectoryAnalysis::updateProcessBar(int value){
	if( isBatch){
		waitingDlg.updateProgress(value);
	}else
		ui.progress_bar->setValue(value);
}

void QtTrajectoryAnalysis::updateImg(cv::Mat* img){
	AI_md.lock(AI_md.mutex);
	setImage(*img);
	AI_md.unlock(AI_md.mutex);
}

void QtTrajectoryAnalysis::updateEventInfo(std::string str){
	QString qstr = QString::fromLocal8Bit("文件名：") + AI_md.file +
		QString::fromLocal8Bit(" 检测开始。\n")+
		QString::fromStdString(str);
	ui.textBrowser->setText(qstr);
}

void QtTrajectoryAnalysis::getThreadEnd(){
	if( isBatch){
		QString qstr = QString::fromLocal8Bit("文件名：") + AI_md.file +
			QString::fromLocal8Bit(" 检测结束.");
		ui.textBrowser->append(qstr);
		detectLog += "\n" +qstr;
		if( batchCnt != fileList.count() ){
			continueBatch();
		}else
			batchDone();
	}else{
		setRun();
		QString qstr = QString::fromLocal8Bit("文件名：") + AI_md.file +
			QString::fromLocal8Bit(" 检测结束.");
		ui.textBrowser->append(qstr);
		QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("检测已完成!"));
	}
}

void QtTrajectoryAnalysis::getThreadStop(){
	setRun();
	QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("检测已取消"));
}

void QtTrajectoryAnalysis::getThreadFail(){
	setRun();
	QString qstr = QString::fromLocal8Bit("文件名：") + AI_md.file +
		QString::fromLocal8Bit(" 检测出错中止.");
	ui.textBrowser->append(qstr);
	
	if( isBatch){
		detectLog += "\n" +qstr;
		if( batchCnt != fileList.count() ){
			continueBatch();
		}else
			batchDone();
	}
	//QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("检测失败!"));
}

QImage const QtTrajectoryAnalysis::copy_mat_to_qimage(cv::Mat const &mat, QImage::Format format){
	QImage image(mat.cols, mat.rows, format);
	for (int i = 0; i != mat.rows; ++i){
		memcpy(image.scanLine(i), mat.ptr(i), image.bytesPerLine() );
	}
	return image;
}  
QImage const QtTrajectoryAnalysis::mat_to_qimage_cpy(cv::Mat &mat, int qimage_format){
	if(mat.type() == CV_8UC3){        
		cv::cvtColor(mat, mat, COLOR_BGR2RGB);
		return copy_mat_to_qimage(mat, QImage::Format_RGB888);
	}
	if(mat.type() == CV_8U){
		return copy_mat_to_qimage(mat, QImage::Format_Indexed8);
	}
	if(mat.type() == CV_8UC4){        
		if(qimage_format == -1){
			return copy_mat_to_qimage(mat, QImage::Format_ARGB32);
		}else{
			return copy_mat_to_qimage(mat, QImage::Format(qimage_format));
		}
	}
	return QImage();
}

void QtTrajectoryAnalysis::setRun(){
	ui.pBtn_Run->setStyleSheet(QString::fromUtf8("QToolButton{border-image: url(:/QtTrajectoryAnalysis/Resources/play.png);}\n"
		"QToolButton:hover{border-image: url(:/QtTrajectoryAnalysis/Resources/play-hover.png);}\n"
		"QToolButton:pressed{border-image: url(:/QtTrajectoryAnalysis/Resources/play-pressed.png);}"));
	isClicked = false;
}

void QtTrajectoryAnalysis::setStop(){
	ui.pBtn_Run->setStyleSheet(QString::fromUtf8("QToolButton{border-image: url(:/QtTrajectoryAnalysis/Resources/pause.png);}\n"
		"QToolButton:hover{border-image: url(:/QtTrajectoryAnalysis/Resources/pause-hover.png);}\n"
		"QToolButton:pressed{border-image: url(:/QtTrajectoryAnalysis/Resources/pause-pressed.png);}"));
	isClicked = true;
}

void QtTrajectoryAnalysis::closeWaitDlg(){
	if( m_extractThread->getStatus()){
		AI_md.isRun = false;
	}
	waitingDlg.close();
}

void QtTrajectoryAnalysis::setBatchInfo(QStringList& qstrl){
	fileList = qstrl;
	isBatch = true;
}

void QtTrajectoryAnalysis::batchFiles(){
	folder = ui.lEd_OutputFolder->text();
	if( folder == ""){
		////打开文件夹
		QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
		QString directory = QFileDialog::getExistingDirectory(this,
			tr("选择文件保存路径"),
			".",
			options);
		if (directory.isEmpty())
			return;
		folder = directory;
	}
	int cnt = fileList.count();
	waitingDlg.setWindowTitle(QString::fromLocal8Bit("当前文件处理进度"));
	waitingDlg.setBtnTip(QString::fromLocal8Bit("放弃继续处理文件！"));
	waitingDlg.setInfo(cnt);
	waitingDlg.setProcStatus(false);
	waitingDlg.show();
	batchCnt = 0;
	continueBatch();
}

string QtTrajectoryAnalysis::getFileName(const QString& file, QString path, QString& name){
	QString fileName;
	QStringList qstrl = file.split("\\");
	if( 0 == qstrl.count()){
		qstrl = file.split("/");
		name = qstrl.at(qstrl.count()-1);
		path.replace("\\", "/");
		fileName = path + "/" + name;
	}else{
		name = qstrl.at(qstrl.count()-1);
		path.replace("/", "\\");
		fileName = path + "\\" + name;
	}
	return fileName.toStdString();
}

void QtTrajectoryAnalysis::continueBatch(){
	string file = fileList.at(batchCnt).toStdString();
	QString name;
	string path = getFileName(fileList.at(batchCnt), folder, name);
	path = folder.toStdString();
	if( !m_extractThread->isRunning()){
		strcpy(AI_md.file, file.c_str());
		strcpy(AI_md.folder, path.c_str());
		m_extractThread->init(&AI_md);
		m_extractThread->start();
		m_updateThread->init(m_extractThread);
		m_updateThread->setIsBatch(isBatch);
		m_updateThread->start();
		waitingDlg.setLabelFile(name);
		waitingDlg.updateLabel(batchCnt+1);
		batchCnt++;

		QString qstr = QString::fromLocal8Bit("文件名：") + AI_md.file +
			QString::fromLocal8Bit(" 检测开始。\n");
		ui.textBrowser->setText(qstr);
	}
}

void QtTrajectoryAnalysis::batchDone(){
	waitingDlg.setBtnTip(QString::fromLocal8Bit("文件处理完毕！"));
	waitingDlg.setProcStatus(true);
	ui.textBrowser->setText(detectLog);
}

void QtTrajectoryAnalysis::on_pBtn_forward_clicked(){
	AI_md.forwardInfo = ForwardResult::FAST_FORWARD;
	if( m_extractThread->isRunning())
		setStop();
}

void QtTrajectoryAnalysis::on_pBtn_backward_clicked(){
	AI_md.forwardInfo = ForwardResult::BACKWARD;
	if( m_extractThread->isRunning())
		setStop();
}


void QtTrajectoryAnalysis::mousePressEvent(QMouseEvent *event){  
	if(event->button() == Qt::LeftButton){
		mouse_press = true;
		//鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
		move_point = event->pos();;
	}
}
void QtTrajectoryAnalysis::mouseMoveEvent(QMouseEvent *event){
	//若鼠标左键被按下
	if(mouse_press){
		//鼠标相对于屏幕的位置
		QPoint move_pos = event->globalPos();

		//移动主窗体位置
		this->move(move_pos - move_point);
	}
}

void QtTrajectoryAnalysis::mouseReleaseEvent(QMouseEvent *event){  
	//设置鼠标为未被按下
	mouse_press = false;
}

void QtTrajectoryAnalysis::on_pBtn_minimize_clicked(){
	this->showMinimized();
}

void QtTrajectoryAnalysis::on_pBtn_exit_clicked(){
	this->close();
}
