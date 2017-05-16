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
	//�㷨����init
	this->isDetect = false;
	this->cascadeCnt = 0;
	this->fScale = 0.8;      //���ű���
	this->learningRate=0.5;    // ���Ʊ����ۻ�ѧϰ������
	this->area_threshold = 30;  //���������С��ֵ
	this->nThreshold=30; //��ֵ����ֵ
	this->lastRectHeight=0; //�洢��һ֡��������θ߶�

	StaticData staticdata;
	//��ָ�����ļ�Ŀ¼�м��ؼ���������
	//���������û���������أ�����
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
	this->pAI_id->forwardInfo = ForwardResult::NORMAL; //Ĭ������������
}


Controller* Controller::getInstance(){
	if( instance == NULL){
		instance = new Controller();
	}
	return instance;
}

bool Controller::startDetect()
{
	//����Ƶ����
	capture.open(this->videoPath);
	if (!capture.isOpened())
		return false;
	if (!firstGrayFrame.empty())  //�������
		firstGrayFrame.release();

	string detectInfo = "";
	double fps = capture.get(CAP_PROP_FPS);
	double frameCount = capture.get(CAP_PROP_FRAME_COUNT); //����֡��
	int count = 1; //֡����
	int h,m,s;//ʱ����
	double ratio;
	strcpy(this->pAI_id->eventInfo,  "");
	this->pAI_id->curFrame_out.setTo(0);

	//���log��Ϣ�ļ���
	string filename;
	string videoName;
	int seq = 1;
	int startIndex = videoPath.find_last_of('/');  //�Ҳ�������-1
	int startIndex2 = videoPath.find_last_of('\\');
	startIndex = startIndex>startIndex2?startIndex:startIndex2;

	int endIndex = videoPath.find_last_of('.');
	videoName = videoPath.substr(startIndex+1,endIndex-startIndex-1);  //ֻ����Ƶ������
	string textName = savePath+"\\"+videoName+"_"+"events_log.txt";

	while(capture.read(curFrame_ori))
	{
// 		if (count == 500)
// 			this->pAI_id->forwardInfo = ForwardResult::FAST_FORWARD;
// 		cvWaitKey( 1 );
// 		if( cvWaitKey( 1 ) == 27 )
// 			waitKey();
		if( !this->pAI_id->isRun){//�˳���
			break;
		}

		if( this->pAI_id->forwardInfo == ForwardResult::PAUSE){//��ͣ
			Sleep(100);
			continue;
		}else if(this->pAI_id->forwardInfo == ForwardResult::BACKWARD){//����
			if ( count-(int)fps >0 ){
				count-=(int)fps;
			}else{
				count=0;
			}
			capture.set(CV_CAP_PROP_POS_FRAMES,count);
			this->pAI_id->forwardInfo = ForwardResult::NORMAL;
			continue;

		}else if(this->pAI_id->forwardInfo == ForwardResult::FAST_FORWARD){//���
			count+=(int)fps;
			if ( count>=frameCount )  //���������֡�����߶������ļ�
				break;
			capture.set(CV_CAP_PROP_POS_FRAMES,count);
			this->pAI_id->forwardInfo = ForwardResult::NORMAL;
			continue;
		}


		isDetect=false; //whileʱ��isDetect�Ƿ��⵽������

		//��֡
// 		if(lastMaskFrame!=0 && count < lastMaskFrame + interval*fps){
// 			count++;
// 			if ( count>=frameCount )
// 				break;
// 			continue;
// 		}

		cout<<"\n\n*****************another new frame******************"<<endl;

		//��ԭͼ������
		resize(curFrame_ori,curFrame_resize,Size(curFrame_ori.cols * fScale,curFrame_ori.rows * fScale),0,0,CV_INTER_LINEAR);
		//תΪ��ͨ���Ҷ�ͼ
		cvtColor(curFrame_resize, curFrame_gray, CV_BGR2GRAY);

		if(firstGrayFrame.empty())
		{
			//��¼��һ֡
			curFrame_gray.convertTo(firstGrayFrame,CV_32F);
		}
		firstGrayFrame.convertTo(firstGrayFrame_8U, CV_8U);

		//���
		absdiff(firstGrayFrame_8U, curFrame_gray, curFrame_diff);

		//��ֵ��
		threshold(curFrame_diff, curFrame_diff_bin, nThreshold, 255, THRESH_BINARY);

		//��̬����
		accumulateWeighted(curFrame_gray, firstGrayFrame,learningRate,curFrame_diff_bin);

		//��ֵ�˲����˴�СΪ5
		medianBlur(curFrame_diff_bin, curFrame_diff_bin,5);

		//��ʼ���߽缫ֵ������
		Point A,C;
		A.x=0x7fffffff; A.y=0x7fffffff;
		C.x=0; C.y=0;
		//�������
		vector<vector<Point>> _contours;//������������
		vector<Vec4i>hierarchy;
		cv::findContours( curFrame_diff_bin, _contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		for(size_t i = 0; i< _contours.size(); ++i){
			//������������
			//�����������
			double contArea =  fabs(contourArea(_contours[i]));
			//ȥ����С���������
			if( contArea < area_threshold)
				continue;
			//�����Ӿ���
			Rect r0 = boundingRect(Mat(_contours[i]));
			//�ҳ���߽�ĵ�
			findBoundary(r0,A,C);
			//ʵʱ����ÿ������
			//rectangle(img_diff_bi,r0,cvScalar(255,255,255),1,8,0);
		}
		biggerRect(A,C,curFrame_resize);
		int a=A.x,b=A.y,c=C.x-A.x,d=C.y-A.y;

		if (c<=0 || d<=0){
			//���ο򲻴��ڣ���ⲻ��
		}else{
			if(d<lastRectHeight*0.7) //�Ŵ�����ο����
				hugeRect(A,C,curFrame_resize);

			//�õ�Ҫʶ��ľ��ο�
			Rect r=Rect(a,b,c,d);
			//��ȡcurFrame_rz�е�r���ο��С
			Mat sentToDetect=curFrame_resize(r);

			//��ȥ�������
			//detectAndDraw(sentToDetect,cascade, nestedCascade , thirdCascade ,forthCascade ,1.0);
			detectAndDrawColorful(sentToDetect,cascade, nestedCascade ,1.0);
			//imshow("frame", curFrame_resize);

			//����������ͼƬ
			if(isDetect==true){
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

				imwrite(savePath+filename,sentToDetect); //����������ͼƬ
				//ʱ�� h:m:s ����������
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
				detectInfo += temp.str()+ "  ��⵽������!\n";
				seq++;
				lastMaskFrame = count;
			}

			lastRectHeight = d;
		}//else
		count++; //֡����
		ratio = count/(double)capture.get(CAP_PROP_FRAME_COUNT);
		this->pAI_id->percent = ratio*100; //�ٷֱ�
		pAI_id->lock(pAI_id->mutex);
		this->pAI_id->curFrame_out = curFrame_resize.clone();
		pAI_id->unlock(pAI_id->mutex);

		if( isDetect )//д����������Ϣ
			strcpy(this->pAI_id->eventInfo , detectInfo.c_str());
	}//while
	//��eventд��text
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


//��һ֡�����о��εı߽��ĵ�
void Controller::findBoundary(Rect r,Point &a,Point &c){
	//a�Ǿ���ԭ��
	//c��ԭ��Խ��ߵ�
	//r.x����a.x�����
	if (r.x < a.x)
		a.x = r.x;
	//r.y ���� a.y ����
	if(r.y < a.y)
		a.y = r.y;
	//r.x+r.width ����c.x���ұ�
	if ((r.x+r.width) > c.x)
		c.x = r.x+r.width;

	//(r.y+r.height) ���� c.y ����
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


//��ɫ������������Ⲣ����
void Controller::detectAndDrawColorful( Mat& img,
	CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
	double scale){
		//cout<<"����detectAndDraw"<<endl;

		Mat img_ori=img.clone();

		//ע�͵���һ������Ƕ�������ļ��
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
			Scalar(255,0,255)} ;//�ò�ͬ����ɫ��ʾ��ͬ������

		Mat curFrame_rz_gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );//��ͼƬ��С���ӿ����ٶ�

		cvtColor( img, curFrame_rz_gray, COLOR_BGR2GRAY );//��Ϊ�õ�����haar���������Զ��ǻ��ڻҶ�ͼ��ģ�����Ҫת���ɻҶ�ͼ��
		resize( curFrame_rz_gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );//���ߴ���С��1/scale,�����Բ�ֵ
		equalizeHist( smallImg, smallImg );//ֱ��ͼ����

		//t = (double)cvGetTickCount();//���������㷨ִ��ʱ��
		//��⵽��ͷ����
		cascade.detectMultiScale( smallImg, faces,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			|//CV_HAAR_SCALE_IMAGE
			2
			,
			Size(30, 30) );

		//t = (double)cvGetTickCount() - t;//���Ϊ�㷨ִ�е�ʱ��
		//printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
		for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ ){
			//cout<<"��⵽��ͷ"<<endl;
			Mat smallImgROI;
			vector<Rect> nestedObjects;
			Point center;
			Scalar color = colors[i%8];
			int radius;
			center.x = cvRound((r->x + r->width*0.5)*scale);//��ԭ��ԭ���Ĵ�С
			center.y = cvRound((r->y + r->height*0.5)*scale);
			radius = cvRound((r->width + r->height)*0.25*scale);
			//Mat show = img.clone();
			//circle( show, center, radius, color, 3, 8, 0 );
			//imshow("first�����",show);	
			//cnt++;
			//�����ͣ���ÿ������ͼ�ϻ���Ƕ������
			if( nestedCascade.empty() )
				continue;
			smallImgROI = img_ori(*r);

			//������ĺ�������һ��
			nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
				1.1, 2, 0
				//|CV_HAAR_FIND_BIGGEST_OBJECT
				//|CV_HAAR_DO_ROUGH_SEARCH
				//|CV_HAAR_DO_CANNY_PRUNING
				|//CV_HAAR_SCALE_IMAGE
				2
				,
				Size(30, 30) );

			//��⵽��������
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
	//cout<<"����findMaskMan"<<endl;

	double scale=0.3;

	int width = img.cols;
	int height = img.rows;

	//�����ο�λ���°벿��
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


//�ڰ�������������Ⲣ����
void Controller::detectAndDraw( Mat& img,
	CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
	CascadeClassifier& thirdCascade,CascadeClassifier& forthCascade ,
	double scale)
{
	Mat img_ori=img.clone();

	//ע�͵���һ�����򿪵�2�������ļ��
	//nestedCascade.~CascadeClassifier() ;


	int i = 0;
	double t = 0;
	vector<Rect> faces;

	Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );//���ż���

	cvtColor( img, gray, CV_BGR2GRAY );//��haar������תΪ�Ҷ�ͼ��
	resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );//��С��1/scale,�����Բ�ֵ
	equalizeHist( smallImg, smallImg );//ֱ��ͼ����

	t = (double)cvGetTickCount();//���������㷨ִ��ʱ��


	//detectMultiScale������smallImg��ʾ����Ҫ��������ͼ��ΪsmallImg��faces��ʾ��⵽������Ŀ�����У�1.1��ʾ
	//ÿ��ͼ��ߴ��С�ı���Ϊ1.1��2��ʾÿһ��Ŀ������Ҫ����⵽3�β��������Ŀ��(��Ϊ��Χ�����غͲ�ͬ�Ĵ��ڴ�
	//С�����Լ�⵽����),CV_HAAR_SCALE_IMAGE��ʾ�������ŷ���������⣬��������ͼ��Size(30, 30)ΪĿ���
	//��С���ߴ�
	cascade.detectMultiScale( smallImg, faces,
		1.1, 2, 0
		//|CV_HAAR_FIND_BIGGEST_OBJECT
		//|CV_HAAR_DO_ROUGH_SEARCH
		|CV_HAAR_SCALE_IMAGE
		,
		Size(30, 30) );

	t = (double)cvGetTickCount() - t;//���Ϊ�㷨ִ�е�ʱ��
	printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );

	//��ͷ����ɫ��
	for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
	{
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		int radius;
		center.x = cvRound((r->x + r->width*0.5)*scale);//��ԭ��ԭ���Ĵ�С
		center.y = cvRound((r->y + r->height*0.5)*scale);
		radius = cvRound((r->width + r->height)*0.25*scale);
		circle( img, center, radius, CV_RGB(255,0,0), 1, 8, 0 );

		imshow("head",img);
		//waitKey();

		cascadeCnt++;

		if( nestedCascade.empty() )//û����nestedCascade
			continue;

		smallImgROI = img_ori(*r); //r:��ͷ����뾶
		Mat smallImgROI_clone = smallImgROI.clone();
		Mat smallImgROI_clone2 = smallImgROI.clone();

		//������ĺ�������һ��
		nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CV_HAAR_FIND_BIGGEST_OBJECT
			//|CV_HAAR_DO_ROUGH_SEARCH
			//|CV_HAAR_DO_CANNY_PRUNING
			|CV_HAAR_SCALE_IMAGE
			,
			Size(30, 30) );

		//����������ɫ��ʶ
		for( vector<Rect>::const_iterator nr = nestedObjects.begin(); nr != nestedObjects.end(); nr++ )
		{
			isDetect=true;

			//Mat supSmallImg;
			vector<Rect> thirdNestedObjects;
			vector<Rect> mouthNestedObjects;

			center.x = cvRound((r->x + nr->x + nr->width*0.5)*scale);
			center.y = cvRound((r->y + nr->y + nr->height*0.5)*scale);
			radius = cvRound((nr->width + nr->height)*0.25*scale);
			circle( img, center, radius, CV_RGB(0,0,255), 1, 8, 0 );//�����Ҳ���������Ͷ�Ӧ������ͼ����һ����

			imshow("face",img);
			//waitKey();

			nestedCnt++;

			//supSmallImg = smallImgROI(*nr);

			//�����б�
			thirdCascade.detectMultiScale( smallImgROI_clone, thirdNestedObjects,
				1.1, 2, 0
				|CV_HAAR_SCALE_IMAGE
				,
				Size(30, 30) );

			//���ӡ���ɫ��
			if (thirdNestedObjects.size()>0)
			{
				isDetect=false;

				center.x = cvRound((r->x + thirdNestedObjects.begin()->x + thirdNestedObjects.begin()->width*0.5)*scale);
				center.y = cvRound((r->y + thirdNestedObjects.begin()->y + thirdNestedObjects.begin()->height*0.5)*scale);
				radius = cvRound((thirdNestedObjects.begin()->width + thirdNestedObjects.begin()->height)*0.25*scale);
				circle( img, center, radius, CV_RGB(0,255,0), 1, 8, 0 );//�����Ҳ���������Ͷ�Ӧ������ͼ����һ����

				imshow("nose",img);

				cout<<"����size��"<<thirdNestedObjects.size()<<endl;
				waitKey();
			}

			//��͡�������ɫ��
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
				circle( img, center, radius, CV_RGB(128,128,0), 1, 8, 0 );//�����Ҳ���������Ͷ�Ӧ������ͼ����һ����

				imshow("mouth",img);

				cout<<"���size��"<<mouthNestedObjects.size()<<endl;
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
			//��⵽������
			imshow("smallImgROI",smallImgROI);
			imwrite(path,smallImgROI);
			//cout<<path<<endl;
			//waitKey();
		}
	}//��ͷfor

}

