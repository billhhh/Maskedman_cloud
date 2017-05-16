#include "qttrajectoryanalysis.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QProcess>
#include "serviceDll.h"

using namespace cv;
using namespace std;

//构造函数
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

bool QtTrajectoryAnalysis::init(){
	isBatch = false;//默认isBatch是false
	ui.pBtn_Stop->setEnabled(false);//开始stop按钮置空
	fileList.clear();
	connect(&waitingDlg, SIGNAL(procDone()), this, SLOT(closeWaitDlg()));

	ui.progress_bar->setRange(0,100); 

	m_extractThread = new extractThread(this);
	connect(m_extractThread, SIGNAL(threadEnd()), this, SLOT(getThreadEnd()));
	connect(m_extractThread, SIGNAL(threadStop()), this, SLOT(getThreadStop()));
	connect(m_extractThread, SIGNAL(threadFail()), this, SLOT(getThreadFail()));
	
	m_updateThread = new updateThread(this);
	connect(m_updateThread, SIGNAL(sendSliderVal(int)), this, SLOT(updateProcessBar(int)));
	connect(m_updateThread, SIGNAL(sendImg(cv::Mat&)), this, SLOT(updateImg(cv::Mat&)));
	connect(m_updateThread, SIGNAL(sendInfo(std::string)), this, SLOT(updateEventInfo(std::string)));

	if( !serviceDll->getInstance()->loadDLL("..\\service_dll\\detectMaskMan.dll")){
		printf("load Dll failed!\n");
		return false;
	}
	return true;
}

void QtTrajectoryAnalysis::destroy(){
	m_extractThread->setStatus(false);
	m_extractThread->wait();
	m_updateThread->wait();

	if( m_extractThread != NULL){
		//m_extractThread->terminate();
		delete m_extractThread;
		m_extractThread = NULL;
	}
	if( m_updateThread != NULL){
		//m_updateThread->terminate();
		delete m_updateThread;
		m_updateThread = NULL;
	}
}

//只load第一帧图像
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
	cv::resize( image, img, cv::Size(355, 265), 0, 0, cv::INTER_CUBIC);
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

//按下开始按钮
///开始运行
void QtTrajectoryAnalysis::on_pBtn_Run_clicked(){
	if( isBatch){
		batchFiles();
		return;
	}

	outputFolder = ui.lEd_OutputFolder->text().toStdString();
	if( outputFolder.length() == 0){
		QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请确保输出路径正确"));
		return;
	}
	loadImage(videoName);

	//如果没有处于运行状态
	if( !m_extractThread->getStatus()){
		//string videoPath = outputFolder + '\\' + videoName;
		m_extractThread->init(videoName, outputFolder);
		m_extractThread->start();
		m_updateThread->init(m_extractThread);
		m_updateThread->setIsBatch(isBatch);
		m_updateThread->start();

		ui.pBtn_Stop->setEnabled(true);  //可用
		ui.pBtn_Run->setEnabled(false); //不可用
	}
}

void QtTrajectoryAnalysis::on_pBtn_Stop_clicked(){
	m_extractThread->setStatus(false);
}

void QtTrajectoryAnalysis::on_pBtn_Check_clicked(){
	//打开上面获取的目录
	QProcess::startDetached("explorer "+ QString::fromStdString(outputFolder));
}

void QtTrajectoryAnalysis::updateProcessBar(int value){
	if( isBatch){
		waitingDlg.updateProgress(value);
	}else
		ui.progress_bar->setValue(value);
}

void QtTrajectoryAnalysis::updateImg(cv::Mat& img){
	setImage(img);
}

void QtTrajectoryAnalysis::updateEventInfo(std::string str){
	QString qstr = QString::fromStdString(str);
	ui.textBrowser->setText(qstr);
}

void QtTrajectoryAnalysis::getThreadEnd(){
	if( isBatch){
		if( batchCnt != fileList.count() )
			continueBatch();
		else
			batchDone();
	}else{
		ui.pBtn_Stop->setEnabled(false);
		ui.pBtn_Run->setEnabled(true);
		QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("分析已完成"));
	}
}

void QtTrajectoryAnalysis::getThreadStop(){
	ui.pBtn_Stop->setEnabled(false);
	ui.pBtn_Run->setEnabled(true);
	QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("分析已取消"));
}

void QtTrajectoryAnalysis::getThreadFail(){
	ui.pBtn_Stop->setEnabled(false);
	ui.pBtn_Run->setEnabled(true);
	QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("分析已取消"));
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

void QtTrajectoryAnalysis::closeWaitDlg(){
	m_extractThread->setStatus(false);
/*
	if( !m_extractThread->isFinished()){
		m_updateThread->setStatus(false);
		m_extractThread->terminate();
	}
	if( !m_updateThread->isFinished()){
		m_updateThread->terminate();
	}*/
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
	if( !m_extractThread->getStatus()){
		m_extractThread->init(file, path);
		m_extractThread->start();
		m_updateThread->init(m_extractThread);
		m_updateThread->setIsBatch(isBatch);
		m_updateThread->start();
		waitingDlg.setLabelFile(name);
		waitingDlg.updateLabel(batchCnt+1);
		batchCnt++;
	}
}

void QtTrajectoryAnalysis::batchDone(){
	waitingDlg.setBtnTip(QString::fromLocal8Bit("文件处理完毕！"));
}

void QtTrajectoryAnalysis::on_pBtn_quit_clicked(){
	close();
}