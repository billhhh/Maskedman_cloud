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
		this->close();
	}
}

QtTrajectoryAnalysis::~QtTrajectoryAnalysis()
{
	destroy();
}

bool QtTrajectoryAnalysis::init(){
	isBatch = false;
	fileList.clear();
	connect(&waitingDlg, SIGNAL(procDone()), this, SLOT(closeWaitDlg()));

	ui.progress_bar->setRange(0,100); 

	m_extractThread = new extractThread(this);
	connect(m_extractThread, SIGNAL(threadEnd(cv::Mat*)), this, SLOT(getThreadEnd(cv::Mat*)));
	connect(m_extractThread, SIGNAL(threadStop()), this, SLOT(getThreadStop()));
	connect(m_extractThread, SIGNAL(threadFail()), this, SLOT(getThreadFail()));
	
	m_updateThread = new updateThread(this);
	connect(m_updateThread, SIGNAL(sendSliderVal(int)), this, SLOT(updateProcessBar(int)));
	connect(m_updateThread, SIGNAL(sendFrameImg(cv::Mat*)), this, SLOT(getFrameImg(cv::Mat*)));

	m_analyzeThread = new analyzeThread(this);
	connect(m_analyzeThread, SIGNAL(threadEnd(cv::Mat*)), this, SLOT(AnalyzeThreadEnd(cv::Mat*)));
	connect(m_analyzeThread, SIGNAL(threadFail()), this, SLOT(AnalyzeThreadFail()));

	isClicked = false;     
	iconRun.addFile(QString::fromUtf8(":/QtTrajectoryAnalysis/Resources/startanalyze.png"), QSize(), QIcon::Normal, QIcon::Off);
	iconStop.addFile(QString::fromUtf8(":/QtTrajectoryAnalysis/Resources/stopanalyze.png"), QSize(), QIcon::Normal, QIcon::Off);
	if( !serviceDll->getInstance()->loadDLL("..\\..\\Release\\trajectory_analysis.dll")){
		return false;
	}
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
	if( m_analyzeThread != NULL){
		m_analyzeThread->terminate();
		delete m_analyzeThread;
		m_analyzeThread = NULL;
	}
}

bool QtTrajectoryAnalysis::loadImage(std::string path){
	videoName = path;

	VideoCapture videoCapture;
	if ( !videoCapture.open(videoName)){
		printf("failed to load \"%s\".\n", videoName.c_str());
		QMessageBox::information(0, QString::fromLocal8Bit("提示"), 
			QString::fromLocal8Bit("打开视频失败!"));
		return false;
	}

	cv::Mat image;
	videoCapture>>image;

	//输出到界面
	setImage(image, &imgScene, ui.srcImgView);

	videoCapture.release();
	return true;
}

void QtTrajectoryAnalysis::setImage(cv::Mat& image, 
	QGraphicsScene* qScene, QGraphicsView* qView){
	//输出到界面
	QImage qimage= mat_to_qimage_cpy(image, 0);
	qimage = qimage.scaled(QSize(355, 265));
	QPixmap pixmap = QPixmap::fromImage(qimage);
	qScene->clear();
	qScene->addPixmap(pixmap);
	qView->setScene(qScene);
	qView->show();
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
		m_extractThread->setStatus(false);
		return;
	}

	outputFolder = ui.lEd_OutputFolder->text().toStdString();
	if( outputFolder.length() == 0){
		QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请确保输出路径正确"));
		return;
	}
	//loadImage(videoName);

	if( !m_extractThread->isRunning()){
		strcpy(AI_ta.file, videoName.c_str());
		strcpy(AI_ta.folder, outputFolder.c_str());
		m_extractThread->init(&AI_ta);
		m_extractThread->start();
		m_updateThread->init(m_extractThread);
		m_updateThread->start();
		
		setStop();
	}
}

void QtTrajectoryAnalysis::on_pBtn_Check_clicked(){
	//打开上面获取的目录
	QProcess::startDetached("explorer "+ ui.lEd_OutputFolder->text());
}
/*
void QtTrajectoryAnalysis::on_pBtn_FixBg_clicked(){
	if( !videoOpen ){
		QMessageBox::information(0, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先打开视频文件！"));
		return;
	}

	//修复背景并设置为背景模型
	m_videoTrack->GetInstance()->inPaint(m_image);
	m_videoTrack->GetInstance()->setBg(m_image);

	Mat img;
	cv::resize( m_image, img, cv::Size(241, 181), 0, 0, INTER_CUBIC);
	QImage image= mat_to_qimage_cpy(img, 0);
	QPixmap pixmap = QPixmap::fromImage(image);
	ui.label_video->setPixmap(pixmap);
}*/

void QtTrajectoryAnalysis::on_pBtn_Analysis_clicked(){
	outputFolder = ui.lEd_OutputFolder->text().toStdString();
	QString fileName = QFileDialog::getOpenFileName(
		this, QString::fromLocal8Bit("选择打开的数据文件"), ".",
		tr("Files (*.txt)"));
	if( fileName.size() == 0)
		return;

	//读入数据文件
	if( !m_analyzeThread->isRunning()){
		strcpy(AI_da.file, fileName.toStdString().c_str());
		strcpy(AI_da.folder, outputFolder.c_str());
		m_analyzeThread->init(&AI_da);
		m_analyzeThread->start();
	}

	//setImage(image);
}

void QtTrajectoryAnalysis::updateProcessBar(int value){
	if( isBatch){
		waitingDlg.updateProgress(value);
	}else
		ui.progress_bar->setValue(value);
}

void QtTrajectoryAnalysis::getThreadEnd(cv::Mat* img){
	if( isBatch){
		if( batchCnt != fileList.count() )
			continueBatch();
		else
			batchDone();
	}else{
		setRun();
		setImage(*img, &dstScene, ui.dstImgView);
		QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("分析已完成"));
	}
}

void QtTrajectoryAnalysis::getThreadStop(){
	setRun();
	QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("分析已取消"));
}

void QtTrajectoryAnalysis::getThreadFail(){
	setRun();
	QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("分析失败"));
}
/*
void QtTrajectoryAnalysis::getThreadFileEnd(){
	QMessageBox::information(0, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("分析文件已存在"));
}*/

void QtTrajectoryAnalysis::AnalyzeThreadEnd(cv::Mat* img){
	setImage(*img, &dstScene, ui.dstImgView);
	QMessageBox::information(0, QString::fromLocal8Bit("提示"), 
		QString::fromLocal8Bit("数据分析完毕！"));
}

void QtTrajectoryAnalysis::AnalyzeThreadFail(){
	QMessageBox::information(0, QString::fromLocal8Bit("提示"), 
		QString::fromLocal8Bit("数据分析失败！"));
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

void QtTrajectoryAnalysis::setFolder(QString qstr){
	ui.lEd_OutputFolder->setText(qstr);
}

void QtTrajectoryAnalysis::getFrameImg(cv::Mat* img){
	AI_ta.lock();
	setImage(*img, &imgScene, ui.srcImgView);
	AI_ta.unlock();
}

void QtTrajectoryAnalysis::setRun(){
	ui.pBtn_Run->setIcon(iconRun);
	ui.pBtn_Run->setIconSize(QSize(60, 60));
	ui.pBtn_Run->setText(QString::fromLocal8Bit("智能分析"));
	isClicked = false;
}

void QtTrajectoryAnalysis::setStop(){
	ui.pBtn_Run->setIcon(iconStop);
	ui.pBtn_Run->setIconSize(QSize(60, 60));
	ui.pBtn_Run->setText(QString::fromLocal8Bit("分析中止"));
	isClicked = true;
}

void QtTrajectoryAnalysis::closeWaitDlg(){
	if( !m_extractThread->isFinished())
		m_extractThread->terminate();
	if( !m_updateThread->isFinished())
		m_updateThread->terminate();
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
	if( !m_extractThread->isRunning()){
		strcpy(AI_ta.file, file.c_str());
		m_extractThread->init(&AI_ta);
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