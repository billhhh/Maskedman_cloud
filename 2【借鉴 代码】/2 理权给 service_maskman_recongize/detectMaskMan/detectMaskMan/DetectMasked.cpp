#include "DetectMasked.h"
#include <fstream> 
using namespace std;

DetectMasked* DetectMasked::instance = NULL;

//构造函数
DetectMasked::DetectMasked(void){
}

//析构函数
DetectMasked::~DetectMasked(void){
	if( capture.isOpened()){
		capture.release();
	}

	if( instance != NULL){
		delete instance;
		instance = NULL;
	}
}

//获得一个DetectMasked实例
DetectMasked* DetectMasked::getInstance(){
	if( instance == NULL){
		instance = new DetectMasked();
	}
	return instance;
}

bool DetectMasked::init(){
	this->cascadeName = "./data/service_classifier/cascades.xml";//人头检测训练数据
	this->nestedCascadeName = "./data/service_classifier/haarcascades/haarcascade_frontalface_alt2.xml"; //正脸分类器
	this->lastRectHeight=0; //一般都是矩形的高度变化，存储上一个矩形高度
	this->isDetect=false;
	this->fScale = 0.8;      //缩放倍数
	this->learningRate=0.5;    // 控制背景累积学习的速率
	this->area_threshold = 30;  //区域面积大小阈值
	this->nThreshold=30; //二值化阈值
	this->lastMaskFrame = 0;

	//从指定的文件目录中加载级联分类器
	if( !this->cascade.load( this->cascadeName ) ){
		//找到当前路径
		//imwrite("zhaolujing.jpg",0);
		printf("ERROR: Could not load classifier cascade");
		return false;
	}

	//如果分类器没有正常加载，报错
	if( !this->nestedCascade.load( this->nestedCascadeName ) ){
		printf("WARNING: Could not load classifier cascade for nested objects");
		return false;
	}
	return true;
}

void DetectMasked::setParam(string file, string savePath, float interval_s){
	strFile = file;
	strPath =savePath;
	interval = interval_s;
}

/*
void DetectMasked::setParam(const char* file, const char* path, const float &interval_s){
	strFile = file;
	strPath =path;
	interval = interval_s;
}*/

bool DetectMasked::detectMaskedMan(char* eventInfo, Mat* &dstImg,    //dstImg 即 curFrame
	int &percent, bool &isRunning ){
		capture.open(strFile);
		if(!capture.isOpened())
			return false;

		double fps = capture.get(CAP_PROP_FPS);
		double frameCount = capture.get(CAP_PROP_FRAME_COUNT);
		int count = 1; //帧计数
		int h,m,s;//时分秒		
		double ratio;		
		strcpy(eventInfo,  "");
		dstImg = NULL;
		/*if(!capture.read(firstFrame)) //读取第一帧
			return false;
		resize(firstFrame,firstFrame,Size(firstFrame.cols * fScale,firstFrame.rows * fScale),0,0,CV_INTER_LINEAR);
		cvtColor(firstFrame, firstFrame, CV_BGR2GRAY);
		firstFrame.convertTo(firstFrame, CV_32F);*/

		string filename;
		string videoName;
		int seq = 1;
		int startIndex = strFile.find_last_of('/');
		int endIndex = strFile.find_last_of('.');
		videoName = strFile.substr(startIndex+1,endIndex-startIndex-1);
		string textName = strPath+"/"+videoName+"_"+"events_log.txt";
		
		Mat frameVideo;
		Mat curFrame;
		string detectInfo = "";
		bool isChanged = false;
		if( !firstFrame.empty())
			firstFrame.release();

		while(capture.read(frameVideo)){
			////////退出
			if( !isRunning){
				break;
			}
			isChanged = false;

			curFrame = frameVideo.clone();
			//dstImg = frameVideo.clone();
			if(lastMaskFrame!=0 && count < lastMaskFrame + interval*fps){
				count++;
				continue;
			}

			resize(curFrame, curFrame_rz,Size(curFrame.cols * fScale,curFrame.rows * fScale),0,0,INTER_LINEAR);
			cvtColor(curFrame_rz, curFrame_rz_gray, COLOR_BGR2GRAY);

			if(firstFrame.empty())
				curFrame_rz_gray.convertTo(firstFrame,CV_32F);
			firstFrame.convertTo(firstFrame_8U, CV_8U);

			absdiff(firstFrame_8U, curFrame_rz_gray, img_diff);
			threshold(img_diff, img_diff_bi, nThreshold, 255, THRESH_BINARY);

			accumulateWeighted(curFrame_rz_gray, firstFrame,learningRate,img_diff_bi);
			cv::medianBlur(img_diff_bi, img_diff_bi,5);

			//初始化边界极值点坐标
			Point A,C;
			A.x=0x7fffffff; A.y=0x7fffffff;
			C.x=0; C.y=0;
			//轮廓检测
			vector<vector<Point>> _contours;//储存所有轮廓
			vector<Vec4i>hierarchy;
			cv::findContours( img_diff_bi, _contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
			for(size_t i = 0; i< _contours.size(); ++i){
				//遍历所有轮廓
				//计算轮廓面积
				double contArea =  fabs(contourArea(_contours[i]));
				//去除较小面积的轮廓
				if( contArea < area_threshold)
					continue;
				//获得外接矩形
				Rect r0 = boundingRect(Mat(_contours[i]));
				//找出最边界的点
				findBoundary(r0,A,C);
				//实时画出每个矩形
				//rectangle(img_diff_bi,r0,cvScalar(255,255,255),1,8,0);
			}
			biggerRect(A,C,curFrame_rz);
			int a=A.x,b=A.y,c=C.x-A.x,d=C.y-A.y;
			//矩形框不存在，检测不了
			if (c<=0 || d<=0){
			}else{
				if(d<lastRectHeight*0.7)
					hugeRect(A,C,curFrame_rz);
				//得到要识别的矩形框
				Rect r=Rect(a,b,c,d);
				//截取curFrame_rz中的r矩形框大小
				Mat src=curFrame_rz(r);
				//imshow("处理前",src);
				//送去人脸检测

				detectAndDraw(src,cascade, nestedCascade,1.0);
				if(isDetect==true){
					isChanged = true;
					//文件名+ _ + h + _ + m + _ + s + _ +编号.jpg
					stringstream temp;
					filename = "/" + videoName + "_";
					s = count / fps +1;
					h = s / 3600;
					m = (s % 3600)/60;
					s = s % 60;
					temp.str("");
					temp<<h;
					if(h/10 == 0)
						filename += "0";
					filename += temp.str() + "_";
					temp.str("");
					temp<<m;
					if(m/10 == 0)
						filename += "0" ;
					filename += temp.str()+ "_";
					temp.str("");
					temp<<s;
					if(s/10 == 0)
						filename += "0";
					filename += temp.str()+ "_";
					temp.str("");
					temp<<seq;
					filename += temp.str()+ ".jpg";

					imwrite(strPath+filename,src); //保存蒙面人图片
					//时间 h:m:s 发现蒙面人
					temp.str("");
					detectInfo += "在视频时间：";
					temp<<h;
					if(h/10 == 0)
						detectInfo += "0" ;
					detectInfo += temp.str() + ":";
					temp.str("");
					temp<<m;
					if(m/10 == 0)
						detectInfo += "0";
					detectInfo += temp.str()+ ":";
					temp.str("");
					temp<<s;
					if(s/10 == 0)
						detectInfo += "0";
					detectInfo += temp.str()+ "  检测到蒙面人!\n";
					isDetect=false;
					seq++;
					lastMaskFrame = count;
				}

				lastRectHeight = d;
			}//else	
			count++; //帧计数
			ratio = count/(double)capture.get(CAP_PROP_FRAME_COUNT);
			percent = ratio*100; //百分比
			//dstImg = curFrame.clone();
			dstImg = &curFrame_rz;

			if( isChanged)
				strcpy(eventInfo, detectInfo.c_str());
		}//while		
		//将event写进text
		ofstream out(textName);
		if(out.is_open())
			out<<detectInfo;
		out.close();

		if( capture.isOpened())
			capture.release();
		dstImg = NULL;

		if(count < frameCount)
			return false;
		return true;			
}

//找一帧中所有矩形的边界四点
void DetectMasked::findBoundary(Rect r,Point &a,Point &c){
	//a是矩形原点
	//c是原点对角线点
	//r.x还在a.x的左边
	if (r.x < a.x)
		a.x = r.x;
	//r.y 还在 a.y 上面
	if(r.y < a.y)
		a.y = r.y;
	//r.x+r.width 还在c.x的右边
	if ((r.x+r.width) > c.x)
		c.x = r.x+r.width;

	//(r.y+r.height) 还在 c.y 下面
	if((r.y+r.height) > c.y)
		c.y = r.y+r.height;
}


void DetectMasked::biggerRect(Point &a,Point &c,Mat img){
	if (c.x-a.x<0 || c.y-a.y<0)
		return;

	a.x -= 30;
	a.y -= 100;
	c.x += 30;
	c.y += 100;

	if(a.x<0)
		a.x=0;

	if(a.y<0)
		a.y=0;

	if(c.x>img.cols)
		c.x=img.cols;

	if(c.y>img.rows)
		c.y=img.rows;
}

void DetectMasked::hugeRect(Point &a,Point &c,Mat img){
	a.y = 0;
	c.y = img.rows;
}

//人脸检测并画出
void DetectMasked::detectAndDraw( Mat& img,
	CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
	double scale){
	//cout<<"进入detectAndDraw"<<endl;

	Mat img_ori=img.clone();
	//嘴巴检测手动开关
	//注释掉下一行语句打开嵌套特征的检测
	//nestedCascade.~CascadeClassifier() ;

	int i = 0;
	double t = 0;
	vector<Rect> faces;
	const static Scalar colors[] =  { Scalar(0,0,255),
		Scalar(0,128,255),
		Scalar(0,255,255),
		Scalar(0,255,0),
		Scalar(255,128,0),
		Scalar(255,255,0),
		Scalar(255,0,0),
		Scalar(255,0,255)} ;//用不同的颜色表示不同的人脸

	Mat curFrame_rz_gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );//将图片缩小，加快检测速度

	cvtColor( img, curFrame_rz_gray, COLOR_BGR2GRAY );//因为用的是类haar特征，所以都是基于灰度图像的，这里要转换成灰度图像
	resize( curFrame_rz_gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );//将尺寸缩小到1/scale,用线性插值
	equalizeHist( smallImg, smallImg );//直方图均衡

	//t = (double)cvGetTickCount();//用来计算算法执行时间
	//检测到人头进入
	cascade.detectMultiScale( smallImg, faces,
		1.1, 2, 0
		//|CV_HAAR_FIND_BIGGEST_OBJECT
		//|CV_HAAR_DO_ROUGH_SEARCH
		|//CV_HAAR_SCALE_IMAGE
		2
		,
		Size(30, 30) );

	//t = (double)cvGetTickCount() - t;//相减为算法执行的时间
	//printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
	for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ ){
		//cout<<"检测到人头"<<endl;
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i%8];
		int radius;
		center.x = cvRound((r->x + r->width*0.5)*scale);//还原成原来的大小
		center.y = cvRound((r->y + r->height*0.5)*scale);
		radius = cvRound((r->width + r->height)*0.25*scale);
		//Mat show = img.clone();
		//circle( show, center, radius, color, 3, 8, 0 );
		//imshow("first脸检测",show);	
		//cnt++;
		//检测嘴巴，在每幅人脸图上画出嵌套特征
		if( nestedCascade.empty() )
			continue;
		smallImgROI = img_ori(*r);

		//和上面的函数功能一样
		nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			//|CV_HAAR_DO_CANNY_PRUNING
			|//CV_HAAR_SCALE_IMAGE
			2
			,
			Size(30, 30) );

		//检测到正脸进入
		for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ ){
			Mat supSmallImg;
			Mat ssimgResult;

			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
			radius = cvRound((nr->width + nr->height)*0.25*scale);
			supSmallImg = smallImgROI(*nr);

			cvSkinOtsu(supSmallImg, ssimgResult);
			//findMaskMan(ssimgResult);
			if(findMaskMan(ssimgResult) == true){
				isDetect=true;
				circle( img, center, radius, color, 3, 8, 0 );
			}
		}
	}
}

void DetectMasked::whThresholdOtsu(Mat &src, Mat &dst){
	int height=src.rows;
	int width=src.cols;
	//histogram
	float histogram[256]={0};
	for(int i=0;i<height;i++) {
		unsigned char* p=(unsigned char*)src.data+src.step*i;
		for(int j=0;j<width;j++) {
			histogram[*p++]++;
		}
	}
	//normalize histogram
	int size=height*width;
	for(int i=0;i<256;i++) {
		histogram[i]=histogram[i]/size;
	}

	//average pixel value
	float avgValue=0;
	for(int i=0;i<256;i++) {
		avgValue+=i*histogram[i];
	}

	int threshold;	
	float maxVariance=0;
	float w=0,u=0;
	for(int i=0;i<256;i++) {
		w+=histogram[i];
		u+=i*histogram[i];

		float t=avgValue*w-u;
		float variance=t*t/(w*(1-w));
		if(variance>maxVariance) {
			maxVariance=variance;
			threshold=i;
		}
	}

	cv::threshold( src, dst, threshold, 255, THRESH_BINARY);
}

void DetectMasked::cvSkinOtsu(Mat &src, Mat &dst){
	Mat ycrcb;
	Mat cr;
	cvtColor(src,ycrcb,COLOR_BGR2YCrCb);

	vector<Mat> mv;
	split(ycrcb,mv);
	whThresholdOtsu(mv[1],cr);

	dst = cr.clone();
}

bool DetectMasked::findMaskMan(Mat img){
	//cout<<"进入findMaskMan"<<endl;

	double scale=0.3;

	int width = img.cols;
	int height = img.rows;

	//将矩形框定位在下半部分
	Rect r=Rect(0,height*(1-scale),width,height*scale);
	img=img(r);

	int count=countNonZero(img);
	int area=width*height*scale;
	double rate=(double)count/area;

	if(rate<0.3){
		//rectangle(img,r0,cvScalar(255,255,255),1,8,0);
		//cvWaitKey(0);
		return true;
	}
	return false;
}