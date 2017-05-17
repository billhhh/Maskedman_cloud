#include "Controller.h"
#include<iostream>
#include<vector>
#include <fstream>
#include <Windows.h>
using std::cout;
using std::ofstream;
using std::stringstream;
using std::vector;
using std::endl;

Controller::
	Controller():firstFrame(),background(),smokeThresholdRatio(0.1f)
{
	bg = cv::createBackgroundSubtractorMOG2();
	bg->setHistory(1);
	bg->setNMixtures(3);
	bg->setBackgroundRatio(0.00005);
	bg->setDetectShadows(false);

	highParam[0] = -0.25;
	highParam[1] = 0.5;
	highParam[2] = -0.25;
	lowParam[0] = 0.25;
	lowParam[1] = 0.5;
	lowParam[2] = 0.25;
}

Controller::~Controller(){
	this->video.release();
}

void Controller::setParam(AI_Flame_Detect *pAI_da){
	this->pAI_id = pAI_da;
	this->videoPath = pAI_da->file;
	this->savePath = pAI_da->folder;
	this->videoType = pAI_da->type;
	this->pAI_id->forwardInfo = ForwardResult::NORMAL; //默认是正常进行
}

bool Controller::startDetect(){
	video.open(this->videoPath);
	if (!video.isOpened())
		return false;

	if(videoType ==1)
		flameDetect_close();
	else if(videoType ==2)
		flameDetect_far();
	else{
		cout<<"输入有误！"<<endl;
		return false;
	}
	return true;
}

//边框红色
void Controller::drawFlame(Mat &frame){
	for(int r=0;r<frame.rows;r++){
		frame.at<cv::Vec3b>(r,0)[0] = 0;
		frame.at<cv::Vec3b>(r,0)[1] = 0;
		frame.at<cv::Vec3b>(r,0)[2] = 255;
		frame.at<cv::Vec3b>(r,1)[0] = 0;
		frame.at<cv::Vec3b>(r,1)[1] = 0;
		frame.at<cv::Vec3b>(r,1)[2] = 255;
		frame.at<cv::Vec3b>(r,frame.cols-2)[0] = 0;
		frame.at<cv::Vec3b>(r,frame.cols-2)[1] = 0;
		frame.at<cv::Vec3b>(r,frame.cols-2)[2] = 255;
		frame.at<cv::Vec3b>(r,frame.cols-1)[0] = 0;
		frame.at<cv::Vec3b>(r,frame.cols-1)[1] = 0;
		frame.at<cv::Vec3b>(r,frame.cols-1)[2] = 255;
	}
	for(int c=0;c<frame.cols;c++){
		frame.at<cv::Vec3b>(0,c)[0] = 0;
		frame.at<cv::Vec3b>(0,c)[1] = 0;
		frame.at<cv::Vec3b>(0,c)[2] = 255;
		frame.at<cv::Vec3b>(1,c)[0] = 0;
		frame.at<cv::Vec3b>(1,c)[1] = 0;
		frame.at<cv::Vec3b>(1,c)[2] = 255;
		frame.at<cv::Vec3b>(frame.rows-2,c)[0] = 0;
		frame.at<cv::Vec3b>(frame.rows-2,c)[1] = 0;
		frame.at<cv::Vec3b>(frame.rows-2,c)[2] = 255;
		frame.at<cv::Vec3b>(frame.rows-1,c)[0] = 0;
		frame.at<cv::Vec3b>(frame.rows-1,c)[1] = 0;
		frame.at<cv::Vec3b>(frame.rows-1,c)[2] = 255;
	}
}

/*************** 近景视频检测 ***********************/
void Controller::flameDetect_close(){
	int fps=video.get(cv::CAP_PROP_FPS);
	int width=video.get(cv::CAP_PROP_FRAME_WIDTH);
	int height=video.get(cv::CAP_PROP_FRAME_HEIGHT);
	int blockwNum=width/blockw;
	int blockhNum=height/blockh;
	int interval=fps/frequency;//采样间隔
	long ncount=0;//帧计数
	double frameCount = video.get(cv::CAP_PROP_FRAME_COUNT); //共有帧数

	Mat frame;//当前帧
	Mat gray;//当前帧灰度图像
	Mat p[imagenum];//保存图像矩阵15
	Mat diff[diffnum];//差分图像矩阵14
	Mat filter;//滤波图像
	int mark[maxNumber][maxNumber];//块矩阵 保存所在块烟雾点的数量 80，视频帧大小<80*8?
	//保存图像矩阵初始化
	for (int i=0;i<imagenum;i++)
		p[i] = Mat::zeros(height,width,CV_32FC1);		
	//差分图像矩阵初始化
	for (int i=0;i<diffnum;i++)
		diff[i] = Mat::zeros(height,width,CV_32FC1);			
	//灰度图像初始化
	gray = Mat(height,width,CV_8UC1);
	//滤波图像初始化
	filter = Mat(height,width,CV_8UC1);

	string detectInfo = "";
	int count = 1; //帧计数
	int h,m,s;//时分秒
	double ratio;
	strcpy(this->pAI_id->eventInfo,  "");
	this->pAI_id->curFrame_out.setTo(0);	
	string filename;//输出log信息文件名
	string videoName;
	int seq = 1;
	int startIndex = videoPath.find_last_of('/');  //找不到返回-1
	int startIndex2 = videoPath.find_last_of('\\');
	startIndex = startIndex>startIndex2?startIndex:startIndex2;
	int endIndex = videoPath.find_last_of('.');
	videoName = videoPath.substr(startIndex+1,endIndex-startIndex-1);  //只是视频的名字
	string textName = savePath+"\\"+videoName+"_"+"events_log.txt";

	//主循环
	while (1){
		if (ncount>1000000000)
			ncount=0;
		ncount++;

		if (!video.read(frame))//读取当前帧
			break;

		if( !this->pAI_id->isRun){//退出的
			break;
		}
		if( this->pAI_id->forwardInfo == ForwardResult::PAUSE){//暂停
			Sleep(100);
			continue;
		}

		cvtColor(frame,gray,cv::COLOR_RGB2GRAY);//将当前帧转为灰度图像保存在gray中		
		//块矩阵初始化
		for (int i=0;i<blockhNum;i++)
			for (int j=0;j<blockwNum;j++)
				mark[i][j]=0;

		if (ncount%interval==1)//1秒取5幀
		{
			//更新保存图像
			for (int i=1;i<imagenum;i++)
				p[i-1] = p[i].clone();
			gray.convertTo(p[imagenum-1],CV_32F);

			//更新差分图像
			for (int i=1;i<imagenum;i++)
				absdiff(p[i],p[i-1],diff[i-1]);

			if(ncount>fps*imagenum/frequency)//第3s之後，所有P[i]不為空
				getFilterImage(filter,diff,mark);//filter零，mark[80][80]零，diff[14]相鄰15幀的差值
				
			//imshow("FilterImage",filter);
		}
		if(ifFlame_close(frame,mark,blockwNum,blockhNum)){ //检测到烟火
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

			drawFlame(frame);
			imwrite(savePath+filename,frame); //保存烟火图片

			//时间 h:m:s 检测到烟火
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
			detectInfo += temp.str()+ "  检测到烟火!\n";
			seq++;
			strcpy(this->pAI_id->eventInfo , detectInfo.c_str());//写入烟火信息
		}
		ncount++; //帧计数
		ratio = ncount/frameCount;
		this->pAI_id->percent = ratio*100; //百分比
		pAI_id->lock(pAI_id->mutex);
		this->pAI_id->curFrame_out = frame.clone();
		pAI_id->unlock(pAI_id->mutex);		
	}
	//将event写进text
	ofstream out(textName);
	if(out.is_open())
		out<<detectInfo;
	out.close();

	if( video.isOpened())
		video.release();
	//this->pAI_id->curFrame_out.setTo(0);
}

//二阶滤波器
//数组大小diffnum
void Controller::twoStageFilter(float xn[diffnum],float dn[diffnum],float en[diffnum])
{
	//输入信号为xn 输出为一阶低带子信号en和一阶高带子信号dn
	dn[0]=xn[0]*highParam[1]+xn[1]*highParam[2];
	en[0]=xn[0]*lowParam[1]+xn[1]*lowParam[2];
	for (int i=1;i<diffnum-1;i++)
	{
		dn[i]=xn[i-1]*highParam[0]+xn[i]*highParam[1]+xn[i+1]*highParam[2];
		en[i]=xn[i-1]*lowParam[0]+xn[i]*lowParam[1]+xn[i+1]*lowParam[2];
	}
	dn[diffnum-1]=xn[diffnum-2]*highParam[0]+xn[diffnum-1]*highParam[1];
	en[diffnum-1]=xn[diffnum-2]*lowParam[0]+xn[diffnum-1]*lowParam[1];

	for(int i=0;i<diffnum;i++)
	{
		xn[i]=en[i];//将一阶低带子信号复制给xn
	}
	//输入信号xn为一阶低带子信号 输出为二阶高带子信号en
	en[0]=xn[0]*highParam[1]+xn[1]*highParam[2];
	for (int i=1;i<diffnum-1;i++)
	{
		en[i]=xn[i-1]*highParam[0]+xn[i]*highParam[1]+xn[i+1]*highParam[2];
	}
	en[diffnum-1]=xn[diffnum-2]*highParam[0]+xn[diffnum-1]*highParam[1];
}

//判断一个点是否是烟雾点
//数组大小diffnum
bool Controller::isSmokePoint(float dn[diffnum],float en[diffnum])
{
	int dnCount=0;
	int enCount=0;
	for (int i=0;i<diffnum;i++)
	{
		if(dn[i]>peakvalue1&&dn[i]<maxpeakvalue1)//10~20 一阶高带子信号dn
			dnCount++;
		if(en[i]>peakvalue2&&en[i]<maxpeakvalue1)//5~20 二阶高带子信号en
			enCount++;
	}
	//en或者dn超过峰值两次 则判定为烟雾点
	if(dnCount>2||enCount>2)
		return true;

	return false;
}

//获得滤波器图像
/*
* filter只显示烟火二值图
* diff CV_32FC1  diff[diffnum]
*int mark[maxNumber][maxNumber]
*/
void Controller::getFilterImage(Mat &filter, Mat diff[diffnum],int mark[maxNumber][maxNumber])
{
	float xn[diffnum]= {0};//14
	float dn[diffnum]={0};
	float en[diffnum]={0};
	double temp;
	for (int row=0;row<filter.rows;row++){		
		for (int col=0;col<filter.cols;col++){
			for (int i=0;i<diffnum;i++)
				xn[i] = diff[i].at<float>(row,col);//将diff[14]逐个像素值取出到xn[i]
			twoStageFilter(xn,dn,en);
			if(isSmokePoint(dn,en)){
				filter.at<uchar>(row,col) = 255;				
				mark[col/blockh][row/blockw]+=1;//对烟雾点所在块进行统计
			}
			else
				filter.at<uchar>(row,col) = 0;
		}
	}
}

/*
* 将检测到烟雾(烟雾点>6)的块画红色方框
* int mark[maxNumber][maxNumber]
*/
bool Controller::ifFlame_close(Mat &frame,int mark[maxNumber][maxNumber],int blockwNum,int blockhNum){
	for (int row=0;row<blockwNum;row++){
		for(int col=0;col<blockhNum;col++){
			if (mark[row][col]>=smokeThresholdRatio*blockh*blockw){ // 一个块内的烟雾点>6			
				return true;
			}
		}
	}
	return false;
}


/*************** 远景视频检测 ***********************/
void Controller::flameDetect_far(){
	if(this->buildBackground())
		this->grad_fore();
}

//用梯度进行前景提取
void Controller::grad_fore(){
	Mat frame;
	Mat /*firstFrame,*/firstFrame_hsv,firstFrame_hsv_fore;
	Mat fore[5];
	Mat fiveForeAnd,fiveForeAnd_src,fiveForeAnd_src_hsv;
	const int skip = 5;//跳帧
	int count=0;
	long ncount=0;//帧计数
	double frameCount = video.get(cv::CAP_PROP_FRAME_COUNT); //共有帧数
	int fps=video.get(cv::CAP_PROP_FPS);

// 	if(!video.read(firstFrame))
// 		return;
	cvtColor(firstFrame,firstFrame_hsv,cv::COLOR_BGR2HSV);
	for(int i=0;i<5;i++){
		fore[i] = Mat(firstFrame.size(),CV_8UC1).clone();
		fore[i].setTo(0);
	}

	string detectInfo = "";
	//int count = 1; //帧计数
	int h,m,s;//时分秒
	double ratio;
	strcpy(this->pAI_id->eventInfo,  "");
	this->pAI_id->curFrame_out.setTo(0);	
	string filename;//输出log信息文件名
	string videoName;
	int seq = 1;
	int startIndex = videoPath.find_last_of('/');  //找不到返回-1
	int startIndex2 = videoPath.find_last_of('\\');
	startIndex = startIndex>startIndex2?startIndex:startIndex2;
	int endIndex = videoPath.find_last_of('.');
	videoName = videoPath.substr(startIndex+1,endIndex-startIndex-1);  //只是视频的名字
	string textName = savePath+"\\"+videoName+"_"+"events_log.txt";

	while(1)
	{	
		ncount++;
		if(!video.read(frame)){//出现height=0
			video.release();
			break;
		}
		if(++count <skip)//隔几帧取一次
			continue;

		count = 0;
		for(int i=0;i<4;i++)
			fore[i] = fore[i+1].clone();		
		frameFore(frame,fore[4]); //提取前景
		fiveForeAnd = fore[0].clone(); //5帧相与
		for(int i=1;i<5;i++)
			//fiveForeAnd.copyTo(fiveForeAnd, fore[i]);
			fiveForeAnd = fiveForeAnd & fore[i];

		frame.copyTo(fiveForeAnd_src,fiveForeAnd);
		cvtColor(fiveForeAnd_src,fiveForeAnd_src_hsv,cv::COLOR_BGR2HSV);
		firstFrame_hsv.copyTo(firstFrame_hsv_fore,fiveForeAnd);//第一帧对应前景部分的hsv
		fiveForeAnd_src_hsv -= firstFrame_hsv_fore; //减去第一帧
		Mat hsv_split[3];
		split(fiveForeAnd_src_hsv,hsv_split);
		cv::Scalar avg_scalar = mean(hsv_split[1],fiveForeAnd);
		//cout<<avg_scalar[0]<<endl; //<20
		//fileout<<avg_scalar[0]<<endl;

		//检测到烟火
		if(avg_scalar[0]>0 && avg_scalar[0]<18){ //0<饱和度<18，烟雾，报警，边缘红色
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

			drawFlame(frame);
			imwrite(savePath+filename,frame); //保存烟火图片

			//时间 h:m:s 检测到烟火
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
			detectInfo += temp.str()+ "  检测到烟火!\n";
			seq++;
			strcpy(this->pAI_id->eventInfo , detectInfo.c_str());//写入烟火信息
		}
		ncount++; //帧计数
		ratio = ncount/frameCount;
		this->pAI_id->percent = ratio*100; //百分比
		pAI_id->lock(pAI_id->mutex);
		this->pAI_id->curFrame_out = frame.clone();
		pAI_id->unlock(pAI_id->mutex);		
	}
	//将event写进text
	ofstream out(textName);
	if(out.is_open())
		out<<detectInfo;
	out.close();

	if( video.isOpened())
		video.release();
}

//保留前n大连通域，其他填充为0
//调用前要确保连通域个数大于n
bool Controller::keepSomeMaxCon(Mat &src, int n){
	///////// !!!findcontours不能查找位于图像边缘的点，导致边缘的点没被填充
	for(int i=0;i<src.cols;i++)
		src.at<uchar>(0,i) = src.at<uchar>(src.rows -1,i) = 0;
	for(int j=0;j<src.rows;j++)
		src.at<uchar>(j,0) = src.at<uchar>(j,src.cols -1) = 0;

	//连通域去噪，只剩下n个
	Mat temp = src.clone();
	vector<vector<cv::Point> > contours;
	findContours(temp,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE); //轮廓,函数会改变temp

	//原图连通域数量少于所要保留的连通域数量
	if(contours.size() < n)	
		return false;

	Mat srt(cv::Size(contours.size(),1),CV_32SC1);
	for(int c=0;c<(int)contours.size();c++){
		srt.at<int>(0,c) = (int)cv::contourArea(contours[c]);
	}
	cv::sortIdx(srt,srt,cv::SORT_EVERY_ROW + cv::SORT_DESCENDING);
	for(int i=n; i<(int)contours.size(); i++){ //只保留前n大连通域
		int idx = srt.at<int>(i);
		vector<cv::Point> tmp = contours[idx];
		const cv::Point* elementPoints[1] = { &tmp[0] };
		int numberOfPoints = (int)contours[idx].size();	
		fillPoly(src,elementPoints, &numberOfPoints ,1,cv::Scalar(0,0,0));//填充单个连通域		
	}

	return true;
}

//提取一帧的前景
//得到的fore是二值图
void Controller::frameFore(Mat frame,Mat &fore){
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	//Scharr求梯度
	Mat src_gray,grad_x,grad_y,abs_grad_x,abs_grad_y,grad;
	GaussianBlur( frame, frame, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
	cvtColor(frame,src_gray,cv::COLOR_BGR2GRAY);		
	Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );
	Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, cv::BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

	//前景提取
	fore.setTo(0);//fore置零，fore是单通道还是3通道？
	Mat grad_bgr;
	cvtColor(grad,grad_bgr,cv::COLOR_GRAY2BGR);
	bg->apply(grad_bgr,fore); //得到的fore是二值图
	medianBlur(fore, fore, 3);
	medianBlur(fore, fore, 3);
	Mat element(5, 5, CV_8UC1);
	element.setTo(1);
	dilate(fore, fore, element); //膨胀
	keepSomeMaxCon(fore,1);//保留最大连通域
	//imshow("frame",frame);
	/*imshow("fore",fore);	
 	waitKey();
 	destroyAllWindows();*/
}

//建立背景,取前1秒所有帧加权构成
bool Controller::buildBackground(){
	Mat frame;
	int fps = this->video.get(cv::CAP_PROP_FPS)*0.5;
	double ratio = 1.0/fps;
	int count = 0;
	while(this->video.read(frame)){
		if(count == 0){
			this->background = Mat::zeros(frame.size(),frame.type());
			this->firstFrame = frame.clone();
		}
		addWeighted(frame,ratio,background,1.0,0,background);
		count++;
		if(count==fps)
			break;
	}
	if(count==fps){
		Mat fore(background.size(),CV_8UC1);
		this->frameFore(background,fore);
	}
	/*imshow("firstFrame",firstFrame);
	imshow("background",background);
	waitKey();
	destroyAllWindows();*/
	if(count<fps)
		return false;
	return true;
}