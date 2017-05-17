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
	this->pAI_id->forwardInfo = ForwardResult::NORMAL; //Ĭ������������
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
		cout<<"��������"<<endl;
		return false;
	}
	return true;
}

//�߿��ɫ
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

/*************** ������Ƶ��� ***********************/
void Controller::flameDetect_close(){
	int fps=video.get(cv::CAP_PROP_FPS);
	int width=video.get(cv::CAP_PROP_FRAME_WIDTH);
	int height=video.get(cv::CAP_PROP_FRAME_HEIGHT);
	int blockwNum=width/blockw;
	int blockhNum=height/blockh;
	int interval=fps/frequency;//�������
	long ncount=0;//֡����
	double frameCount = video.get(cv::CAP_PROP_FRAME_COUNT); //����֡��

	Mat frame;//��ǰ֡
	Mat gray;//��ǰ֡�Ҷ�ͼ��
	Mat p[imagenum];//����ͼ�����15
	Mat diff[diffnum];//���ͼ�����14
	Mat filter;//�˲�ͼ��
	int mark[maxNumber][maxNumber];//����� �������ڿ����������� 80����Ƶ֡��С<80*8?
	//����ͼ������ʼ��
	for (int i=0;i<imagenum;i++)
		p[i] = Mat::zeros(height,width,CV_32FC1);		
	//���ͼ������ʼ��
	for (int i=0;i<diffnum;i++)
		diff[i] = Mat::zeros(height,width,CV_32FC1);			
	//�Ҷ�ͼ���ʼ��
	gray = Mat(height,width,CV_8UC1);
	//�˲�ͼ���ʼ��
	filter = Mat(height,width,CV_8UC1);

	string detectInfo = "";
	int count = 1; //֡����
	int h,m,s;//ʱ����
	double ratio;
	strcpy(this->pAI_id->eventInfo,  "");
	this->pAI_id->curFrame_out.setTo(0);	
	string filename;//���log��Ϣ�ļ���
	string videoName;
	int seq = 1;
	int startIndex = videoPath.find_last_of('/');  //�Ҳ�������-1
	int startIndex2 = videoPath.find_last_of('\\');
	startIndex = startIndex>startIndex2?startIndex:startIndex2;
	int endIndex = videoPath.find_last_of('.');
	videoName = videoPath.substr(startIndex+1,endIndex-startIndex-1);  //ֻ����Ƶ������
	string textName = savePath+"\\"+videoName+"_"+"events_log.txt";

	//��ѭ��
	while (1){
		if (ncount>1000000000)
			ncount=0;
		ncount++;

		if (!video.read(frame))//��ȡ��ǰ֡
			break;

		if( !this->pAI_id->isRun){//�˳���
			break;
		}
		if( this->pAI_id->forwardInfo == ForwardResult::PAUSE){//��ͣ
			Sleep(100);
			continue;
		}

		cvtColor(frame,gray,cv::COLOR_RGB2GRAY);//����ǰ֡תΪ�Ҷ�ͼ�񱣴���gray��		
		//������ʼ��
		for (int i=0;i<blockhNum;i++)
			for (int j=0;j<blockwNum;j++)
				mark[i][j]=0;

		if (ncount%interval==1)//1��ȡ5��
		{
			//���±���ͼ��
			for (int i=1;i<imagenum;i++)
				p[i-1] = p[i].clone();
			gray.convertTo(p[imagenum-1],CV_32F);

			//���²��ͼ��
			for (int i=1;i<imagenum;i++)
				absdiff(p[i],p[i-1],diff[i-1]);

			if(ncount>fps*imagenum/frequency)//��3s֮�ᣬ����P[i]�����
				getFilterImage(filter,diff,mark);//filter�㣬mark[80][80]�㣬diff[14]����15���Ĳ�ֵ
				
			//imshow("FilterImage",filter);
		}
		if(ifFlame_close(frame,mark,blockwNum,blockhNum)){ //��⵽�̻�
			//�ļ���+ _ + h + _ + m + _ + s + _ +���.jpg
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
			imwrite(savePath+filename,frame); //�����̻�ͼƬ

			//ʱ�� h:m:s ��⵽�̻�
			temp.str("");
			detectInfo += "����Ƶʱ�䣺";
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
			detectInfo += temp.str()+ "  ��⵽�̻�!\n";
			seq++;
			strcpy(this->pAI_id->eventInfo , detectInfo.c_str());//д���̻���Ϣ
		}
		ncount++; //֡����
		ratio = ncount/frameCount;
		this->pAI_id->percent = ratio*100; //�ٷֱ�
		pAI_id->lock(pAI_id->mutex);
		this->pAI_id->curFrame_out = frame.clone();
		pAI_id->unlock(pAI_id->mutex);		
	}
	//��eventд��text
	ofstream out(textName);
	if(out.is_open())
		out<<detectInfo;
	out.close();

	if( video.isOpened())
		video.release();
	//this->pAI_id->curFrame_out.setTo(0);
}

//�����˲���
//�����Сdiffnum
void Controller::twoStageFilter(float xn[diffnum],float dn[diffnum],float en[diffnum])
{
	//�����ź�Ϊxn ���Ϊһ�׵ʹ����ź�en��һ�׸ߴ����ź�dn
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
		xn[i]=en[i];//��һ�׵ʹ����źŸ��Ƹ�xn
	}
	//�����ź�xnΪһ�׵ʹ����ź� ���Ϊ���׸ߴ����ź�en
	en[0]=xn[0]*highParam[1]+xn[1]*highParam[2];
	for (int i=1;i<diffnum-1;i++)
	{
		en[i]=xn[i-1]*highParam[0]+xn[i]*highParam[1]+xn[i+1]*highParam[2];
	}
	en[diffnum-1]=xn[diffnum-2]*highParam[0]+xn[diffnum-1]*highParam[1];
}

//�ж�һ�����Ƿ��������
//�����Сdiffnum
bool Controller::isSmokePoint(float dn[diffnum],float en[diffnum])
{
	int dnCount=0;
	int enCount=0;
	for (int i=0;i<diffnum;i++)
	{
		if(dn[i]>peakvalue1&&dn[i]<maxpeakvalue1)//10~20 һ�׸ߴ����ź�dn
			dnCount++;
		if(en[i]>peakvalue2&&en[i]<maxpeakvalue1)//5~20 ���׸ߴ����ź�en
			enCount++;
	}
	//en����dn������ֵ���� ���ж�Ϊ�����
	if(dnCount>2||enCount>2)
		return true;

	return false;
}

//����˲���ͼ��
/*
* filterֻ��ʾ�̻��ֵͼ
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
				xn[i] = diff[i].at<float>(row,col);//��diff[14]�������ֵȡ����xn[i]
			twoStageFilter(xn,dn,en);
			if(isSmokePoint(dn,en)){
				filter.at<uchar>(row,col) = 255;				
				mark[col/blockh][row/blockw]+=1;//����������ڿ����ͳ��
			}
			else
				filter.at<uchar>(row,col) = 0;
		}
	}
}

/*
* ����⵽����(�����>6)�Ŀ黭��ɫ����
* int mark[maxNumber][maxNumber]
*/
bool Controller::ifFlame_close(Mat &frame,int mark[maxNumber][maxNumber],int blockwNum,int blockhNum){
	for (int row=0;row<blockwNum;row++){
		for(int col=0;col<blockhNum;col++){
			if (mark[row][col]>=smokeThresholdRatio*blockh*blockw){ // һ�����ڵ������>6			
				return true;
			}
		}
	}
	return false;
}


/*************** Զ����Ƶ��� ***********************/
void Controller::flameDetect_far(){
	if(this->buildBackground())
		this->grad_fore();
}

//���ݶȽ���ǰ����ȡ
void Controller::grad_fore(){
	Mat frame;
	Mat /*firstFrame,*/firstFrame_hsv,firstFrame_hsv_fore;
	Mat fore[5];
	Mat fiveForeAnd,fiveForeAnd_src,fiveForeAnd_src_hsv;
	const int skip = 5;//��֡
	int count=0;
	long ncount=0;//֡����
	double frameCount = video.get(cv::CAP_PROP_FRAME_COUNT); //����֡��
	int fps=video.get(cv::CAP_PROP_FPS);

// 	if(!video.read(firstFrame))
// 		return;
	cvtColor(firstFrame,firstFrame_hsv,cv::COLOR_BGR2HSV);
	for(int i=0;i<5;i++){
		fore[i] = Mat(firstFrame.size(),CV_8UC1).clone();
		fore[i].setTo(0);
	}

	string detectInfo = "";
	//int count = 1; //֡����
	int h,m,s;//ʱ����
	double ratio;
	strcpy(this->pAI_id->eventInfo,  "");
	this->pAI_id->curFrame_out.setTo(0);	
	string filename;//���log��Ϣ�ļ���
	string videoName;
	int seq = 1;
	int startIndex = videoPath.find_last_of('/');  //�Ҳ�������-1
	int startIndex2 = videoPath.find_last_of('\\');
	startIndex = startIndex>startIndex2?startIndex:startIndex2;
	int endIndex = videoPath.find_last_of('.');
	videoName = videoPath.substr(startIndex+1,endIndex-startIndex-1);  //ֻ����Ƶ������
	string textName = savePath+"\\"+videoName+"_"+"events_log.txt";

	while(1)
	{	
		ncount++;
		if(!video.read(frame)){//����height=0
			video.release();
			break;
		}
		if(++count <skip)//����֡ȡһ��
			continue;

		count = 0;
		for(int i=0;i<4;i++)
			fore[i] = fore[i+1].clone();		
		frameFore(frame,fore[4]); //��ȡǰ��
		fiveForeAnd = fore[0].clone(); //5֡����
		for(int i=1;i<5;i++)
			//fiveForeAnd.copyTo(fiveForeAnd, fore[i]);
			fiveForeAnd = fiveForeAnd & fore[i];

		frame.copyTo(fiveForeAnd_src,fiveForeAnd);
		cvtColor(fiveForeAnd_src,fiveForeAnd_src_hsv,cv::COLOR_BGR2HSV);
		firstFrame_hsv.copyTo(firstFrame_hsv_fore,fiveForeAnd);//��һ֡��Ӧǰ�����ֵ�hsv
		fiveForeAnd_src_hsv -= firstFrame_hsv_fore; //��ȥ��һ֡
		Mat hsv_split[3];
		split(fiveForeAnd_src_hsv,hsv_split);
		cv::Scalar avg_scalar = mean(hsv_split[1],fiveForeAnd);
		//cout<<avg_scalar[0]<<endl; //<20
		//fileout<<avg_scalar[0]<<endl;

		//��⵽�̻�
		if(avg_scalar[0]>0 && avg_scalar[0]<18){ //0<���Ͷ�<18��������������Ե��ɫ
			//�ļ���+ _ + h + _ + m + _ + s + _ +���.jpg
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
			imwrite(savePath+filename,frame); //�����̻�ͼƬ

			//ʱ�� h:m:s ��⵽�̻�
			temp.str("");
			detectInfo += "����Ƶʱ�䣺";
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
			detectInfo += temp.str()+ "  ��⵽�̻�!\n";
			seq++;
			strcpy(this->pAI_id->eventInfo , detectInfo.c_str());//д���̻���Ϣ
		}
		ncount++; //֡����
		ratio = ncount/frameCount;
		this->pAI_id->percent = ratio*100; //�ٷֱ�
		pAI_id->lock(pAI_id->mutex);
		this->pAI_id->curFrame_out = frame.clone();
		pAI_id->unlock(pAI_id->mutex);		
	}
	//��eventд��text
	ofstream out(textName);
	if(out.is_open())
		out<<detectInfo;
	out.close();

	if( video.isOpened())
		video.release();
}

//����ǰn����ͨ���������Ϊ0
//����ǰҪȷ����ͨ���������n
bool Controller::keepSomeMaxCon(Mat &src, int n){
	///////// !!!findcontours���ܲ���λ��ͼ���Ե�ĵ㣬���±�Ե�ĵ�û�����
	for(int i=0;i<src.cols;i++)
		src.at<uchar>(0,i) = src.at<uchar>(src.rows -1,i) = 0;
	for(int j=0;j<src.rows;j++)
		src.at<uchar>(j,0) = src.at<uchar>(j,src.cols -1) = 0;

	//��ͨ��ȥ�룬ֻʣ��n��
	Mat temp = src.clone();
	vector<vector<cv::Point> > contours;
	findContours(temp,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE); //����,������ı�temp

	//ԭͼ��ͨ������������Ҫ��������ͨ������
	if(contours.size() < n)	
		return false;

	Mat srt(cv::Size(contours.size(),1),CV_32SC1);
	for(int c=0;c<(int)contours.size();c++){
		srt.at<int>(0,c) = (int)cv::contourArea(contours[c]);
	}
	cv::sortIdx(srt,srt,cv::SORT_EVERY_ROW + cv::SORT_DESCENDING);
	for(int i=n; i<(int)contours.size(); i++){ //ֻ����ǰn����ͨ��
		int idx = srt.at<int>(i);
		vector<cv::Point> tmp = contours[idx];
		const cv::Point* elementPoints[1] = { &tmp[0] };
		int numberOfPoints = (int)contours[idx].size();	
		fillPoly(src,elementPoints, &numberOfPoints ,1,cv::Scalar(0,0,0));//��䵥����ͨ��		
	}

	return true;
}

//��ȡһ֡��ǰ��
//�õ���fore�Ƕ�ֵͼ
void Controller::frameFore(Mat frame,Mat &fore){
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	//Scharr���ݶ�
	Mat src_gray,grad_x,grad_y,abs_grad_x,abs_grad_y,grad;
	GaussianBlur( frame, frame, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
	cvtColor(frame,src_gray,cv::COLOR_BGR2GRAY);		
	Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );
	Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, cv::BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

	//ǰ����ȡ
	fore.setTo(0);//fore���㣬fore�ǵ�ͨ������3ͨ����
	Mat grad_bgr;
	cvtColor(grad,grad_bgr,cv::COLOR_GRAY2BGR);
	bg->apply(grad_bgr,fore); //�õ���fore�Ƕ�ֵͼ
	medianBlur(fore, fore, 3);
	medianBlur(fore, fore, 3);
	Mat element(5, 5, CV_8UC1);
	element.setTo(1);
	dilate(fore, fore, element); //����
	keepSomeMaxCon(fore,1);//���������ͨ��
	//imshow("frame",frame);
	/*imshow("fore",fore);	
 	waitKey();
 	destroyAllWindows();*/
}

//��������,ȡǰ1������֡��Ȩ����
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