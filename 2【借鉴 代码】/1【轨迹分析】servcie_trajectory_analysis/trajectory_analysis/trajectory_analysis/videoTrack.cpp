#include "videoTrack.h"
using namespace std;
using namespace cv;

//鼠标框选参数
cv::Rect select;
bool select_flag = false;
cv::Point origin;
cv::Mat frame;

//鼠标响应函数
void onMouse(int event,int x,int y,int,void*){
	if( select_flag){
		select.x = MIN( origin.x, x);//实时计算框选框
		select.y = MIN( origin.y, y);
		select.width = abs( x - origin.x);//计算矩形宽度和高度
		select.height = abs( y - origin.y);
		select &= Rect( 0, 0,frame.cols, frame.rows);//保证所选矩形框在显示区域之内
	}if(event==EVENT_LBUTTONDOWN){
		select_flag = true;//鼠标按下
		origin = Point(x, y);//保存起止点
		select = Rect(x, y, 0, 0);//初始化
	}else if(event== EVENT_LBUTTONUP){
		select_flag=false;
	}
}

videoTrack* videoTrack::instance = 0;

videoTrack::videoTrack(){
	//建立窗口
	//namedWindow("FixBackground", 1);//显示视频原图像的窗口
	//捕捉鼠标
	//setMouseCallback("FixBackground", onMouse, 0);
	bg_model = createBackgroundSubtractorMOG2();
}

videoTrack::~videoTrack(){
	clear();
	if( instance != 0){
		delete instance;
		instance = NULL;
	}
}

//单例模式返回实例
videoTrack* videoTrack::GetInstance(){
	if( instance == 0)
		instance = new videoTrack();
	return instance;
}

//修补指定区域
void videoTrack::copyToROI(Mat &img, Rect &rectTo, Rect &rectFrom){
	Mat imgFrom = img(rectFrom);
	Mat imgTo;
	resize( imgFrom, imgTo, rectTo.size());
	imgTo.copyTo(img(rectTo));
}

//前景提取
void videoTrack::processing(Mat &frame, Mat &bg, Mat &output){
	Mat gray;
	Mat foreground;
	Mat background;
	int nThreshold = 30;// 前景提取的阈值

	cvtColor(frame, gray, COLOR_BGR2GRAY);
	bg.convertTo(background, CV_8U);
	// 计算当前图片和背景的差别
	absdiff(background, gray, foreground);
	// 对得到的前景进行阈值选取，去掉伪前景
	threshold(foreground, output, nThreshold, 255, THRESH_BINARY_INV);

	//形态学膨胀
	Mat imgInv;
	threshold(foreground, imgInv, nThreshold, 255, THRESH_BINARY);

	cv::Mat element3_20(3, 20, CV_8U, cv::Scalar(1));
	cv::Mat element20_5(20, 5, CV_8U, cv::Scalar(1));
	cv::Mat element10_3(10, 3, CV_8U, cv::Scalar(1));

	cv::medianBlur(imgInv, imgInv, 5);

	cv::Mat dilated;
	cv::dilate( imgInv, dilated, element20_5);	

	cv::Mat eroded;
	cv::erode( dilated, eroded, element3_20);
	cv::Mat opened;
	cv::morphologyEx( eroded, opened, cv::MORPH_OPEN, element10_3);

	//output = opened.clone();

	//轮廓检测
	vector<vector<Point>> _contours;
	vector<Vec4i>hierarchy;

	Mat imageROI;
	cv::findContours( opened, _contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	//计算轮廓面积阈值
	double area_threshold = frame.cols * frame.rows * AREA_THRESHOLD;
	//imshow("output", opened);

	int isMark = 0;
	//vector<Rect> vRect(0);
	for(size_t i = 0; i< _contours.size(); ++i){
		//获得外接矩形
		Rect r0 = boundingRect(Mat(_contours[i]));
		//计算轮廓面积
		double contArea =  fabs(contourArea(_contours[i]));
		//去除较小面积的轮廓
		if( contArea < area_threshold)
			continue;

		if( isMark == 0){
			coord_txt<<"\n"<<frame_num<<" ";
			isMark = 1;
		}
		rectangle(frame, r0, Scalar(255, 0, 0) );
		coord_txt/*<<"[x, y]"*/<<(int)(r0.x*ratio)<<" "<<(int)(r0.y*ratio)<<" ";
		coord_txt/*<<"[w, h]"*/<<(int)(r0.width*ratio)<<" "<<(int)(r0.height*ratio)<<" ";
	}
	_contours.clear();
	hierarchy.clear();

	output = frame;
	//imshow("output", output);
}

void videoTrack::fixBg(cv::Mat &img){
	//建立窗口
	namedWindow("FixBackground", 1);//显示视频原图像的窗口
	//捕捉鼠标
	setMouseCallback("FixBackground", onMouse, 0);

	int select_state = 0;
	frame = img.clone();
	Mat bgImg = img.clone();
	Mat selectImg = bgImg.clone();
	Mat drawImg = bgImg.clone();
	Rect rectTo;
	Rect rectFrom;

	Scalar color = Scalar( 0, 0, 255);
	char c;
	while( true){
		if( !select_state){
			drawImg = bgImg.clone();
			rectangle( drawImg, select, color, 1, 8, 0);//能够实时显示在画矩形窗口时的痕迹
			//显示视频图片到窗口
			imshow("FixBackground", drawImg);

			c = ( char)waitKey( 33);
			if( 27 == c)//ESC键
				return;
			else if( 32 == c){
				if ( select.area() < 0.1)
					continue;
				select_state = 1;
				selectImg = drawImg.clone();
				rectTo = select;
				select = Rect( 0, 0, 0, 0);
				color = Scalar( 255, 0, 0);
			}else if( 8 ==c){
				select_state = 1;
				select = Rect( 0, 0, 0, 0);
				color = Scalar( 0, 0, 255);
			}else if( 13 == c){
				break;
			}
		}else{
			drawImg = selectImg.clone();
			rectangle( drawImg, select, color, 1, 8, 0);//能够实时显示在画矩形窗口时的痕迹
			//显示视频图片到窗口
			imshow("FixBackground", drawImg);

			c = ( char)waitKey( 33);
			if( 27 == c)//ESC键
				return;
			else if( 32 == c ){
				if ( select.area() < 0.1)
					continue;
				select_state = 0;
				rectFrom = select;
				select = Rect( 0, 0, 0, 0);
				color = Scalar( 0, 0, 255);
				copyToROI(bgImg, rectTo, rectFrom);
			}else if( 8 ==c){
				select_state = 0;
				select = Rect( 0, 0, 0, 0);
				color = Scalar( 0, 0, 255);
			}else if( 13 == c){;
				break;
			}
		}
	}

	img = bgImg;
	destroyWindow("FixBackground");
}

void videoTrack::inPaint(cv::Mat &img){
	//建立窗口
	namedWindow("FixBackground", 1);//显示视频原图像的窗口
	//捕捉鼠标
	setMouseCallback("FixBackground", onMouse, 0);

	frame = img.clone();
	vector<Rect> vRect(0);
	Mat bgImg = img.clone();
	Mat drawImg = bgImg.clone();
	Mat backupImg = bgImg.clone();
	Rect rectTo;

	Scalar color = Scalar( 0, 0, 255);
	char c;
	while( true){
		drawImg = bgImg.clone();
		rectangle( drawImg, select, color, 1, 8, 0);//能够实时显示在画矩形窗口时的痕迹

		//显示视频图片到窗口
		imshow("FixBackground", drawImg);
		c = ( char)waitKey( 33);
		if( 27 == c)//ESC键
			return;
		else if( 32 == c){
			if ( select.area() < 0.1)
				continue;
			backupImg = bgImg.clone();
			rectTo = select;
			select = Rect( 0, 0, 0, 0);
			color = Scalar( 255, 0, 0);
			vRect.push_back(rectTo);
			bgImg = drawImg;
		}else if( 8 == c){
			select = Rect( 0, 0, 0, 0);
			color = Scalar( 0, 0, 255);
			bgImg = backupImg.clone();
			vRect.pop_back();
		}else if( 13 == c)
			break;
	}

	//修补框选部分
	Mat bgTemp;
	for( vector<Rect>::iterator iter = vRect.begin(); iter != vRect.end(); ++iter ){
		Mat inpaintMask = Mat::zeros(bgImg.size(), CV_8U);
		Mat rectMask = Mat::ones((*iter).size(), CV_8U);
		rectMask.copyTo(inpaintMask(*iter));
		inpaint( img, inpaintMask, bgTemp, 3, INPAINT_TELEA);
		bgTemp(*iter).copyTo(bgImg(*iter));
	}

	img = bgImg;
	destroyWindow("FixBackground");
}

bool videoTrack::openVideo(const string& strVideo){
	if( capture.isOpened())
		capture.release();

	capture.open(strVideo.c_str());
	Mat img;
	//视频打开失败
	if( !capture.isOpened() || !capture.read(img))
		return false;

	fps = capture.get(CAP_PROP_FPS);
	total_frameNum = capture.get(CAP_PROP_FRAME_COUNT);
	bgImg = img.clone();
	return true;
}

void videoTrack::getFrame(cv::Mat& img){
	capture.read(img);
}

bool videoTrack::getEveryFrame(cv::Mat& img){
	++frame_num;

	if( !capture.read(capFrame)){
		capture.release();
		return false;
	}
	frame = capFrame.clone();
	cv::resize( frame, frame, cv::Size(imgH, imgV), 0, 0, INTER_CUBIC);

	Mat source = frame.clone();
	bg_model->apply(source, fgMask, 0.001);

	Mat _bgimg;
	bg_model->getBackgroundImage(_bgimg);
	cvtColor(_bgimg, bgImg, COLOR_RGB2GRAY);
	processing(source, bgImg, img);
	return true;
}

void videoTrack::setBg(cv::Mat& img){
	bgImg = img.clone();
	drawImg = img.clone();
	bg_model->apply( bgImg, fgMask, -1);
}

void videoTrack::initPara(){
	frame_num = 0;
	peopleNum = 0;
	vTrackInfo.clear();
	fileName.clear();

	imgV = bgImg.rows;
	imgH = bgImg.cols;
	double ratioV = bgImg.rows / 240.0;
	double ratioH = bgImg.cols / 320.0;
	if( ratioH > ratioV){
		ratio = ratioH;
	}else{
		ratio = ratioV;
	}
	imgV /= ratio;
	imgH /= ratio;
}

void videoTrack::setOutputFolder(std::string str){
	outputFolder = str;
}

bool videoTrack::startFileRecord(std::string& _fileName){
	int pos = _fileName.find_last_of('.');
	if( pos > 0)
		fileName = _fileName.substr(0, pos);

	string videoName;
	pos = fileName .find_last_of('/');
	if( pos > 0)
		videoName = fileName .substr(pos + 1, fileName .length() - pos + 1);
	else{
		pos = fileName .find_last_of('\\');
		if( pos > 0)
			videoName = fileName.substr(pos + 1, fileName .length() - pos + 1);
		else
			videoName = fileName;
	}

	fileTxt = outputFolder+ "\\" + videoName + ".txt";

	coord_txt.open(fileTxt.c_str(), ios::in|ios::out|ios::trunc/*|ios::_Noreplace*/);
	if( !coord_txt.is_open()){
		return false;
	}

	coord_txt<<"[File]\n"<<videoName<<"\n";
	coord_txt<<"[Size]\n"<<capture.get(CAP_PROP_FRAME_WIDTH)<<' '
		<<capture.get(CAP_PROP_FRAME_HEIGHT)<<"\n";
	coord_txt<<"[Fps]\n"<<capture.get(CAP_PROP_FPS)<<"\n";
	coord_txt<<"[FrameCount]\n"<<capture.get(CAP_PROP_FRAME_COUNT)<<"\n";
	coord_txt<<"[Content]";

	fileName = outputFolder+ "\\" + videoName + ".jpg";
	imwrite(fileName, bgImg);
	return true;
}

void videoTrack::endFileRecord(){
	coord_txt.close();
	fileName.clear();
}

bool videoTrack::isRuningVideo() const{
	if( capture.isOpened())
		return true;
	else
		return false;
}

int videoTrack::getFrameNum() const{
	return frame_num*100/total_frameNum;
}

string videoTrack::getFileName() const{
	return fileTxt;
}

void videoTrack::clear(){
}