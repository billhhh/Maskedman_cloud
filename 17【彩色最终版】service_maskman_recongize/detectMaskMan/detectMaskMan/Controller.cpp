/*
 * Controller.cpp
 *
 * function: Control each part of the project and manage them
 *
 *  Created on: 2015-3-18
 *      Author: Bill Wang
 */

#include <Windows.h>
#include <fstream>
#include "Controller.h"
#include "StaticData.h"

using namespace std;
using namespace cv;

Controller* Controller::instance = NULL;


//constructor
Controller::Controller(){
}


//destructor
Controller::~Controller()
{
	if( capture.isOpened())
		capture.release();

	if( instance != NULL){
		delete instance;
		instance = NULL;
	}
}


//initialize the Controller class
//load the cascade classifier
bool Controller::init()
{
	//算法参数init
	this->isDetect = false;
	this->cascadeCnt = 0;
	this->fScale = 0.8;      //缩放倍数
	this->learningRate=0.5;    // 控制背景累积学习的速率
	this->area_threshold = 30;  //区域面积大小阈值
	this->nThreshold=30; //二值化阈值
	this->lastRectHeight=0; //存储上一帧待处理矩形高度

	StaticData staticdata;
	//从指定的文件目录中加载级联分类器
	//如果分类器没有正常加载，报错
	if( !cascade.load( staticdata.cascadePath ) )
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return false;
	}

	if( !nestedCascade.load( staticdata.nestedCascadePath ) )
	{
		cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
		return false;
	}

	if( !thirdCascade.load( staticdata.thirdCascadePath ) )
	{
		cerr << "WARNING: Could not load classifier cascade for thirdCascade objects" << endl;
		return false;
	}

	if( !forthCascade.load( staticdata.forthCascadePath ) )
	{
		cerr << "WARNING: Could not load classifier cascade for mouthCascade objects" << endl;
		return false;
	}

	return true;
}


void Controller::setParam(AI_Maskman_Detect *pAI_da){
	this->pAI_id = pAI_da;
	this->videoPath = pAI_da->file;
	this->savePath = pAI_da->folder;
	this->interval = pAI_da->interval;
	this->pAI_id->forwardInfo = ForwardResult::NORMAL; //默认是正常进行
}


Controller* Controller::getInstance(){
	if( instance == NULL){
		instance = new Controller();
	}
	return instance;
}

bool Controller::startDetect()
{
	//从视频读入
	capture.open(this->videoPath);
	if (!capture.isOpened())
		return false;
	if (!firstGrayFrame.empty())  //首先清除
		firstGrayFrame.release();

	string detectInfo = "";
	double fps = capture.get(CAP_PROP_FPS);
	double frameCount = capture.get(CAP_PROP_FRAME_COUNT); //共有帧数
	int count = 1; //帧计数
	int h,m,s;//时分秒
	double ratio;
	strcpy(this->pAI_id->eventInfo,  "");
	this->pAI_id->curFrame_out.setTo(0);

	//输出log信息文件名
	string filename;
	string videoName;
	int seq = 1;
	int startIndex = videoPath.find_last_of('/');  //找不到返回-1
	int startIndex2 = videoPath.find_last_of('\\');
	startIndex = startIndex>startIndex2?startIndex:startIndex2;

	int endIndex = videoPath.find_last_of('.');
	videoName = videoPath.substr(startIndex+1,endIndex-startIndex-1);  //只是视频的名字
	string textName = savePath+"\\"+videoName+"_"+"events_log.txt";

	while(capture.read(curFrame_ori))
	{
// 		if (count == 500)
// 			this->pAI_id->forwardInfo = ForwardResult::FAST_FORWARD;
// 		cvWaitKey( 1 );
// 		if( cvWaitKey( 1 ) == 27 )
// 			waitKey();
		if( !this->pAI_id->isRun){//退出的
			break;
		}

		if( this->pAI_id->forwardInfo == ForwardResult::PAUSE){//暂停
			Sleep(100);
			continue;
		}else if(this->pAI_id->forwardInfo == ForwardResult::BACKWARD){//快退
			if ( count-(int)fps >0 ){
				count-=(int)fps;
			}else{
				count=0;
			}
			capture.set(CV_CAP_PROP_POS_FRAMES,count);
			this->pAI_id->forwardInfo = ForwardResult::NORMAL;
			continue;

		}else if(this->pAI_id->forwardInfo == ForwardResult::FAST_FORWARD){//快进
			count+=(int)fps;
			if ( count>=frameCount )  //如果大于总帧数或者读不到文件
				break;
			capture.set(CV_CAP_PROP_POS_FRAMES,count);
			this->pAI_id->forwardInfo = ForwardResult::NORMAL;
			continue;
		}


		isDetect=false; //while时，isDetect是否检测到蒙面人

		//跳帧
// 		if(lastMaskFrame!=0 && count < lastMaskFrame + interval*fps){
// 			count++;
// 			if ( count>=frameCount )
// 				break;
// 			continue;
// 		}

		cout<<"\n\n*****************another new frame******************"<<endl;

		//将原图像缩放
		resize(curFrame_ori,curFrame_resize,Size(curFrame_ori.cols * fScale,curFrame_ori.rows * fScale),0,0,CV_INTER_LINEAR);
		//转为单通道灰度图
		cvtColor(curFrame_resize, curFrame_gray, CV_BGR2GRAY);

		if(firstGrayFrame.empty())
		{
			//记录第一帧
			curFrame_gray.convertTo(firstGrayFrame,CV_32F);
		}
		firstGrayFrame.convertTo(firstGrayFrame_8U, CV_8U);

		//差分
		absdiff(firstGrayFrame_8U, curFrame_gray, curFrame_diff);

		//二值化
		threshold(curFrame_diff, curFrame_diff_bin, nThreshold, 255, THRESH_BINARY);

		//动态更新
		accumulateWeighted(curFrame_gray, firstGrayFrame,learningRate,curFrame_diff_bin);

		//中值滤波，核大小为5
		medianBlur(curFrame_diff_bin, curFrame_diff_bin,5);

		//初始化边界极值点坐标
		Point A,C;
		A.x=0x7fffffff; A.y=0x7fffffff;
		C.x=0; C.y=0;
		//轮廓检测
		vector<vector<Point>> _contours;//储存所有轮廓
		vector<Vec4i>hierarchy;
		cv::findContours( curFrame_diff_bin, _contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
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
		biggerRect(A,C,curFrame_resize);
		int a=A.x,b=A.y,c=C.x-A.x,d=C.y-A.y;

		if (c<=0 || d<=0){
			//矩形框不存在，检测不了
		}else{
			if(d<lastRectHeight*0.7) //放大检测矩形框面积
				hugeRect(A,C,curFrame_resize);

			//得到要识别的矩形框
			Rect r=Rect(a,b,c,d);
			//截取curFrame_rz中的r矩形框大小
			Mat sentToDetect=curFrame_resize(r);

			//送去人脸检测
			//detectAndDraw(sentToDetect,cascade, nestedCascade , thirdCascade ,forthCascade ,1.0);
			detectAndDrawColorful(sentToDetect,cascade, nestedCascade ,1.0);
			//imshow("frame", curFrame_resize);

			//保存蒙面人图片
			if(isDetect==true){
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

				imwrite(savePath+filename,sentToDetect); //保存蒙面人图片
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
				seq++;
				lastMaskFrame = count;
			}

			lastRectHeight = d;
		}//else
		count++; //帧计数
		ratio = count/(double)capture.get(CAP_PROP_FRAME_COUNT);
		this->pAI_id->percent = ratio*100; //百分比
		pAI_id->lock(pAI_id->mutex);
		this->pAI_id->curFrame_out = curFrame_resize.clone();
		pAI_id->unlock(pAI_id->mutex);

		if( isDetect )//写入蒙面人信息
			strcpy(this->pAI_id->eventInfo , detectInfo.c_str());
	}//while
	//将event写进text
	ofstream out(textName);
	if(out.is_open())
		out<<detectInfo;
	out.close();

	if( capture.isOpened())
		capture.release();
	this->pAI_id->curFrame_out.setTo(0);

	if(count < frameCount-int(0.3*fps))
		return false;
	return true;
}


//找一帧中所有矩形的边界四点
void Controller::findBoundary(Rect r,Point &a,Point &c){
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

void Controller::biggerRect(Point &a,Point &c,Mat img){
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

void Controller::hugeRect(Point &a,Point &c,Mat img){
	a.y = 0;
	c.y = img.rows;
}


//彩色蒙面人人脸检测并画出
void Controller::detectAndDrawColorful( Mat& img,
	CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
	double scale){
		//cout<<"进入detectAndDraw"<<endl;

		Mat img_ori=img.clone();

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


void Controller::whThresholdOtsu(Mat &src, Mat &dst){
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

void Controller::cvSkinOtsu(Mat &src, Mat &dst){
	Mat ycrcb;
	Mat cr;
	cvtColor(src,ycrcb,COLOR_BGR2YCrCb);

	vector<Mat> mv;
	split(ycrcb,mv);
	whThresholdOtsu(mv[1],cr);

	dst = cr.clone();
}

bool Controller::findMaskMan(Mat img){
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


//黑白蒙面人人脸检测并画出
void Controller::detectAndDraw( Mat& img,
	CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
	CascadeClassifier& thirdCascade,CascadeClassifier& forthCascade ,
	double scale)
{
	Mat img_ori=img.clone();

	//注释掉下一行语句打开第2层特征的检测
	//nestedCascade.~CascadeClassifier() ;


	int i = 0;
	double t = 0;
	vector<Rect> faces;

	Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );//缩放加速

	cvtColor( img, gray, CV_BGR2GRAY );//类haar特征，转为灰度图像
	resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );//缩小到1/scale,用线性插值
	equalizeHist( smallImg, smallImg );//直方图均衡

	t = (double)cvGetTickCount();//用来计算算法执行时间


	//detectMultiScale函数中smallImg表示的是要检测的输入图像为smallImg，faces表示检测到的人脸目标序列，1.1表示
	//每次图像尺寸减小的比例为1.1，2表示每一个目标至少要被检测到3次才算是真的目标(因为周围的像素和不同的窗口大
	//小都可以检测到人脸),CV_HAAR_SCALE_IMAGE表示不是缩放分类器来检测，而是缩放图像，Size(30, 30)为目标的
	//最小最大尺寸
	cascade.detectMultiScale( smallImg, faces,
		1.1, 2, 0
		//|CV_HAAR_FIND_BIGGEST_OBJECT
		//|CV_HAAR_DO_ROUGH_SEARCH
		|CV_HAAR_SCALE_IMAGE
		,
		Size(30, 30) );

	t = (double)cvGetTickCount() - t;//相减为算法执行的时间
	printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );

	//人头【红色】
	for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
	{
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		int radius;
		center.x = cvRound((r->x + r->width*0.5)*scale);//还原成原来的大小
		center.y = cvRound((r->y + r->height*0.5)*scale);
		radius = cvRound((r->width + r->height)*0.25*scale);
		circle( img, center, radius, CV_RGB(255,0,0), 1, 8, 0 );

		imshow("head",img);
		//waitKey();

		cascadeCnt++;

		if( nestedCascade.empty() )//没传入nestedCascade
			continue;

		smallImgROI = img_ori(*r); //r:人头区域半径
		Mat smallImgROI_clone = smallImgROI.clone();
		Mat smallImgROI_clone2 = smallImgROI.clone();

		//和上面的函数功能一样
		nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			//|CV_HAAR_DO_CANNY_PRUNING
			|CV_HAAR_SCALE_IMAGE
			,
			Size(30, 30) );

		//人脸，用蓝色标识
		for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
		{
			isDetect=true;

			//Mat supSmallImg;
			vector<Rect> thirdNestedObjects;
			vector<Rect> mouthNestedObjects;

			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
			radius = cvRound((nr->width + nr->height)*0.25*scale);
			circle( img, center, radius, CV_RGB(0,0,255), 1, 8, 0 );//将嘴巴也画出来，和对应人脸的图形是一样的

			imshow("face",img);
			//waitKey();

			nestedCnt++;

			//supSmallImg = smallImgROI(*nr);

			//鼻子判别
			thirdCascade.detectMultiScale( smallImgROI_clone, thirdNestedObjects,
				1.1, 2, 0
				|CV_HAAR_SCALE_IMAGE
				,
				Size(30, 30) );

			//鼻子【绿色】
			if (thirdNestedObjects.size()>0)
			{
				isDetect=false;

				center.x = cvRound((r->x + thirdNestedObjects.begin()->x + thirdNestedObjects.begin()->width*0.5)*scale);
				center.y = cvRound((r->y + thirdNestedObjects.begin()->y + thirdNestedObjects.begin()->height*0.5)*scale);
				radius = cvRound((thirdNestedObjects.begin()->width + thirdNestedObjects.begin()->height)*0.25*scale);
				circle( img, center, radius, CV_RGB(0,255,0), 1, 8, 0 );//将嘴巴也画出来，和对应人脸的图形是一样的

				imshow("nose",img);

				cout<<"鼻子size："<<thirdNestedObjects.size()<<endl;
				waitKey();
			}

			//嘴巴【另外颜色】
			thirdCascade.detectMultiScale( smallImgROI_clone2, mouthNestedObjects,
				1.1, 2, 0
				|CV_HAAR_SCALE_IMAGE
				,
				Size(30, 30) );

			if (mouthNestedObjects.size()>0)
			{
				isDetect=false;

				center.x = cvRound((r->x + mouthNestedObjects.begin()->x + mouthNestedObjects.begin()->width*0.5)*scale);
				center.y = cvRound((r->y + mouthNestedObjects.begin()->y + mouthNestedObjects.begin()->height*0.5)*scale);
				radius = cvRound((mouthNestedObjects.begin()->width + mouthNestedObjects.begin()->height)*0.25*scale);
				circle( img, center, radius, CV_RGB(128,128,0), 1, 8, 0 );//将嘴巴也画出来，和对应人脸的图形是一样的

				imshow("mouth",img);

				cout<<"嘴巴size："<<mouthNestedObjects.size()<<endl;
				waitKey();
			}
		}
		std::stringstream ss;
		std::string str;
		ss<<cascadeCnt;
		ss>>str;
		string path = "f:\\mmr\\";
		path.append(str);
		path.append(".jpg");

		if (isDetect == true)
		{
			//检测到蒙面人
			imshow("smallImgROI",smallImgROI);
			imwrite(path,smallImgROI);
			//cout<<path<<endl;
			//waitKey();
		}
	}//人头for

}

