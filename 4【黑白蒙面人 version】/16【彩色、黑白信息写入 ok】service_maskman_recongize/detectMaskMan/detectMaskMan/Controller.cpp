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
	this->isColor = false;
	this->cascadeCnt = 0;
	this->nestedCnt=0;
	this->fScale = 1.0;      //缩放倍数
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

	Mat firstFrame;
	if(!capture.read(firstFrame))
		return false;
	image_is_color(firstFrame,this->isColor);  //确定是否是彩色视频

	if(this->isColor == true)
	{
		detectInfo+="待检测视频是彩色视频！\n";
		strcpy(this->pAI_id->eventInfo , "待检测视频是彩色视频！\n");
	}
	else
	{
		detectInfo+="待检测视频是黑白视频！\n";
		strcpy(this->pAI_id->eventInfo , "待检测视频是黑白视频！\n");
	}

	while(1)
	{
// 		if (count == 500)
// 			this->pAI_id->forwardInfo = ForwardResult::FAST_FORWARD;
// 		waitKey( 1 );
// 		if( waitKey( 1 ) == 27 )
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
			if ( count>= frameCount-int(0.3*fps) )  //如果大于总帧数或者读不到文件
				break;
			capture.set(CV_CAP_PROP_POS_FRAMES,count);
			this->pAI_id->forwardInfo = ForwardResult::NORMAL;
			continue;
		}

		if(!capture.read(curFrame_ori))
			break;
		isDetect=false; //每次while，isDetect初始化

		//跳帧
// 		if(lastMaskFrame!=0 && count < lastMaskFrame + interval*fps){
// 			count++;
// 			if ( count>=frameCount )
// 				break;
// 			continue;
// 		}

		cout<<"\n\n*****************New Frame******************"<<endl;
		cout<<"count == "<<count<<endl;

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

		threshold(curFrame_diff, curFrame_diff_bin, nThreshold, 255, THRESH_BINARY);
		accumulateWeighted(curFrame_gray, firstGrayFrame,learningRate,curFrame_diff_bin);
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
			if(this->isColor == true)
				detectAndDrawColorful(sentToDetect,cascade, nestedCascade ,1.0);
			else
				detectAndDrawGray(sentToDetect,cascade, nestedCascade ,1.0);
			//imshow("frame", curFrame_resize);

			//保存蒙面人图片和信息
			if(this->isDetect==true){
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
		}//else !(c<=0 || d<=0)

		count++; //帧计数
		ratio = count/(double)capture.get(CAP_PROP_FRAME_COUNT);
		this->pAI_id->percent = ratio*100; //百分比
		pAI_id->lock(pAI_id->mutex);
		this->pAI_id->curFrame_out = curFrame_resize.clone();
		pAI_id->unlock(pAI_id->mutex);

		if( this->isDetect )//写入蒙面人信息
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
	{
		ratio=100;
		return true;
		//return false;
	}
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

//彩色蒙面人检测
void Controller::detectAndDrawColorful( Mat& img,
	CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
	double scale){
		Mat img_ori=img.clone();

		//注释掉下一行语句打开嵌套特征的检测
		//nestedCascade.~CascadeClassifier() ;

		int i = 0;
		double t = 0;
		vector<Rect> faces;
		const static Scalar colors[] =  { CV_RGB(0,0,255),
			CV_RGB(0,128,255),
			CV_RGB(0,255,255),
			CV_RGB(0,255,0),
			CV_RGB(255,128,0),
			CV_RGB(255,255,0),
			CV_RGB(255,0,0),
			CV_RGB(255,0,255)} ;//用不同的颜色表示不同的人脸

		Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );//将图片缩小，加快检测速度

		//	cvtColor( img, smallImg, CV_BGR2GRAY );
		cvtColor( img, gray, CV_BGR2GRAY );//因为用的是类haar特征，所以都是基于灰度图像的，这里要转换成灰度图像
		resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );//将尺寸缩小到1/scale,用线性插值
		equalizeHist( smallImg, smallImg );//直方图均衡

		t = (double)cvGetTickCount();//用来计算算法执行时间

		//检测人脸
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
			Size(50, 50) );

		t = (double)cvGetTickCount() - t;//相减为算法执行的时间
		printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );

		for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
		{
			Mat smallImgROI;
			vector<Rect> nestedObjects;
			Point center;
			Scalar color = colors[i%8];
			int radius;
			center.x = cvRound((r->x + r->width*0.5)*scale);//还原成原来的大小
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			//circle( img, center, radius, color, 3, 8, 0 );

			this->cascadeCnt++;

			if( nestedCascade.empty() )
				continue;

			smallImgROI = img_ori(*r);

			//和上面的函数功能一样
			nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
				1.1, 2, 0
				//|CV_HAAR_FIND_BIGGEST_OBJECT
				//|CV_HAAR_DO_ROUGH_SEARCH
				//|CV_HAAR_DO_CANNY_PRUNING
				|CV_HAAR_SCALE_IMAGE
				,
				Size(30, 30) );  
			for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
			{
				Mat supSmallImg;
				Mat ssimgResult;

				center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
				center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
				radius = cvRound((nr->width + nr->height)*0.25*scale);
				//circle( img, center, radius, color, 1, 8, 0 );

				this->nestedCnt++;
				supSmallImg = smallImgROI(*nr);

				//imshow("supSmallImg",supSmallImg);
				cvSkinOtsu(supSmallImg, ssimgResult);
				//imshow("ssimgResult",ssimgResult);

				//imshow("检测到正脸",img);
				//waitKey();

				if(findMaskMan(ssimgResult) == true)
				{
					isDetect=true;
					circle( img, center, radius, color, 3, 8, 0 );
// 					imshow("test",img);
// 					waitKey();
// 					destroyAllWindows();
				}
			}
		}

		cout<<"cnt == "<<this->cascadeCnt<<endl;
		cout<<"nestedCnt == "<<this->nestedCnt<<endl;
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

//黑白蒙面人检测
void Controller::detectAndDrawGray( Mat& img,
	CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
	double scale)
{
	bool flag=false;
	Mat faceImg;
	Mat img_ori=img.clone();

	vector<Rect> faces;

	Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );//将图片缩小，加快检测速度

	cvtColor( img, gray, CV_BGR2GRAY );//因为用的是类haar特征，所以都是基于灰度图像的，这里要转换成灰度图像
	resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );//将尺寸缩小到1/scale,用线性插值
	equalizeHist( smallImg, smallImg );//直方图均衡


	//detectMultiScale函数中smallImg表示的是要检测的输入图像为smallImg，faces表示检测到的人脸目标序列，1.1表示
	//每次图像尺寸减小的比例为1.1，2表示每一个目标至少要被检测到3次才算是真的目标(因为周围的像素和不同的窗口大
	//小都可以检测到人脸),CV_HAAR_SCALE_IMAGE表示不是缩放分类器来检测，而是缩放图像，Size(30, 30)为目标的
	//最小最大尺寸
	cascade.detectMultiScale( smallImg, faces,
		1.1, 2, 0
		//|CV_HAAR_FIND_BIGGEST_OBJECT	//只检测最大的物体
		//|CV_HAAR_DO_ROUGH_SEARCH	//只做初略检测
		|CV_HAAR_SCALE_IMAGE	//按比例正常检测
		,
		Size(50, 50) );

	//人头【红色】
	for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++)
	{
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		int radius;
		center.x = cvRound((r->x + r->width*0.5)*scale);//还原成原来的大小
		center.y = cvRound((r->y + r->height*0.5)*scale);
		radius = cvRound((r->width + r->height)*0.25*scale);
		//circle( img, center, radius, CV_RGB(255,0,0), 1, 8, 0 );

		//imshow("head",img);
		this->cascadeCnt++;
		//waitKey(1);

		if( nestedCascade.empty() )//第2层分类器没有的情况
			continue;

		smallImgROI = img_ori(*r);
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
			this->isDetect=false;

			//Mat supSmallImg;
			vector<Rect> thirdNestedObjects;
			vector<Rect> mouthNestedObjects;

			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
			radius = cvRound((nr->width + nr->height)*0.25*scale);
			//circle( img, center, radius, CV_RGB(0,0,255), 1, 8, 0 );

			faceImg = smallImgROI(*nr);
			//计算嘴巴区域
			Rect re=Rect(faceImg.cols*0.25,faceImg.rows*0.65,faceImg.cols*0.5,faceImg.rows*0.35-1);
			Mat face2MouthRoi=faceImg(re);
			//imshow("face",faceImg);
			//imshow("mouth_area",face2MouthRoi);

			//嘴巴区域送检判定蒙面人
			con_num_detect(face2MouthRoi,this->isDetect);

			this->nestedCnt++;
			//waitKey(1);

			if (this->isDetect == true)
			{
				flag=true;
				circle( img, center, radius, CV_RGB(0,0,255), 3, 8, 0 );
// 				imshow("蒙面人出现",faceImg);
// 				waitKey();
			}
		}
	}//人头

	if(flag == true)
		this->isDetect=true;
}

void Controller::con_num_detect(Mat src,bool &isDet)
{
	//连通域个数来判断
	Mat src_ori = src.clone();
	//切割图片
	cut_image_percent(src,0.2,0,0.8,1);

	Mat src_gray;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	int con_size_threshold=4;
	double nor_max_2_con_threshold = 0.13;

	if( !src.data ){
		return ;
	}

	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

	//转成灰度图
	cvtColor( src, src_gray, CV_RGB2GRAY );

	Mat grad_y;
	Mat abs_grad_y;

	Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );

	abs_grad_y *=1.5;
	threshold(abs_grad_y,abs_grad_y,30,255,THRESH_OTSU | THRESH_BINARY);
	//threshold(abs_grad_y,abs_grad_y,30,255,THRESH_BINARY);

	Mat before_contours = abs_grad_y.clone();
	Mat before_contours2 = abs_grad_y.clone();

	//轮廓检测
	vector<vector<Point>> _contours;//储存所有轮廓
	vector<Vec4i>hierarchy;
	cv::findContours( abs_grad_y, _contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	double nonZeroRate = get_max_2_con_rate(before_contours);
	if (_contours.size() <= con_size_threshold)
	{
		//初步判断为正常嘴
		if(nonZeroRate < nor_max_2_con_threshold) //针对深色纯色口罩，发现蒙面人
			isDet = true;
	}

	else{
		//已初步判定为蒙面人
		if(nonZeroRate > nor_max_2_con_threshold); //发现是正常人误判
		else
			isDet=true;
	}
}

//按百分比截取图片
void Controller::cut_image_percent(Mat &img,double left,double upper,double right,double bottom)
{
	Point a,c;
	a.x = img.cols * left;
	a.y = img.rows * upper;
	c.x = img.cols * right;
	c.y = img.rows * bottom;
	Rect r = Rect(a,c);
	img = img(r);
}

double Controller::get_max_2_con_rate(Mat &img)
{
	keepSomeMaxCon(img,2);
	int count=countNonZero(img);
	double rate = (double)count / (img.rows*img.cols);
	return rate;
}

//src是传入二值图，n是保留前N大连通域
bool Controller::keepSomeMaxCon(Mat &src, int n){
	///////// !!!findcontours不能查找位于图像边缘的点，导致边缘的点没被填充
	for(int i=0;i<src.cols;i++)
		src.at<uchar>(0,i) = src.at<uchar>(src.rows -1,i) = 0;
	for(int j=0;j<src.rows;j++)
		src.at<uchar>(j,0) = src.at<uchar>(j,src.cols -1) = 0;

	//连通域去噪，只剩下n个
	Mat temp = src.clone();
	vector<vector<Point> > contours;
	findContours(temp,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE); //轮廓,函数会改变temp

	//原图连通域数量少于所要保留的连通域数量
	if(contours.size() < n)	
		return false;

	Mat srt(Size(contours.size(),1),CV_32SC1);
	for(int c=0;c<(int)contours.size();c++)
		srt.at<int>(0,c) = (int)contourArea(contours[c]);

	cv::sortIdx(srt,srt,CV_SORT_EVERY_ROW + CV_SORT_DESCENDING);
	for(int i=n; i<(int)contours.size(); i++){ //只保留前n大连通域
		int idx = srt.at<int>(i);
		vector<Point> tmp = contours[idx];
		const Point* elementPoints[1] = { &tmp[0] };
		int numberOfPoints = (int)contours[idx].size();	
		fillPoly(src,elementPoints, &numberOfPoints ,1,Scalar(0,0,0));//填充单个连通域		
	}

	return true;
}

//判断一幅图像是否是彩色图像
bool Controller::image_is_color(Mat image,bool& is_color)
{
	if(!image.data)
		return false;

	int row = image.rows;
	int col = image.cols;
	isColor = false;
	for(int i = 0; i < row;i++)
		for(int j = 0; j< col;j++)
			if (  ((int)(image.at<Vec3b>(i,j)[0]) != (int)(image.at<Vec3b>(i,j)[1]))
				||((int)(image.at<Vec3b>(i,j)[1]) != (int)(image.at<Vec3b>(i,j)[2]))
				||((int)(image.at<Vec3b>(i,j)[2]) != (int)(image.at<Vec3b>(i,j)[0]))
			   )
			{
				//是彩色视频
				is_color=true;
				return true;
			}

	return true;
}