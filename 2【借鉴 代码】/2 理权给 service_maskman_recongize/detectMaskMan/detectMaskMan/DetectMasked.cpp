#include "DetectMasked.h"
#include <fstream> 
using namespace std;

DetectMasked* DetectMasked::instance = NULL;

//���캯��
DetectMasked::DetectMasked(void){
}

//��������
DetectMasked::~DetectMasked(void){
	if( capture.isOpened()){
		capture.release();
	}

	if( instance != NULL){
		delete instance;
		instance = NULL;
	}
}

//���һ��DetectMaskedʵ��
DetectMasked* DetectMasked::getInstance(){
	if( instance == NULL){
		instance = new DetectMasked();
	}
	return instance;
}

bool DetectMasked::init(){
	this->cascadeName = "./data/service_classifier/cascades.xml";//��ͷ���ѵ������
	this->nestedCascadeName = "./data/service_classifier/haarcascades/haarcascade_frontalface_alt2.xml"; //����������
	this->lastRectHeight=0; //һ�㶼�Ǿ��εĸ߶ȱ仯���洢��һ�����θ߶�
	this->isDetect=false;
	this->fScale = 0.8;      //���ű���
	this->learningRate=0.5;    // ���Ʊ����ۻ�ѧϰ������
	this->area_threshold = 30;  //���������С��ֵ
	this->nThreshold=30; //��ֵ����ֵ
	this->lastMaskFrame = 0;

	//��ָ�����ļ�Ŀ¼�м��ؼ���������
	if( !this->cascade.load( this->cascadeName ) ){
		//�ҵ���ǰ·��
		//imwrite("zhaolujing.jpg",0);
		printf("ERROR: Could not load classifier cascade");
		return false;
	}

	//���������û���������أ�����
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

bool DetectMasked::detectMaskedMan(char* eventInfo, Mat* &dstImg,    //dstImg �� curFrame
	int &percent, bool &isRunning ){
		capture.open(strFile);
		if(!capture.isOpened())
			return false;

		double fps = capture.get(CAP_PROP_FPS);
		double frameCount = capture.get(CAP_PROP_FRAME_COUNT);
		int count = 1; //֡����
		int h,m,s;//ʱ����		
		double ratio;		
		strcpy(eventInfo,  "");
		dstImg = NULL;
		/*if(!capture.read(firstFrame)) //��ȡ��һ֡
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
			////////�˳�
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

			//��ʼ���߽缫ֵ������
			Point A,C;
			A.x=0x7fffffff; A.y=0x7fffffff;
			C.x=0; C.y=0;
			//�������
			vector<vector<Point>> _contours;//������������
			vector<Vec4i>hierarchy;
			cv::findContours( img_diff_bi, _contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
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
			biggerRect(A,C,curFrame_rz);
			int a=A.x,b=A.y,c=C.x-A.x,d=C.y-A.y;
			//���ο򲻴��ڣ���ⲻ��
			if (c<=0 || d<=0){
			}else{
				if(d<lastRectHeight*0.7)
					hugeRect(A,C,curFrame_rz);
				//�õ�Ҫʶ��ľ��ο�
				Rect r=Rect(a,b,c,d);
				//��ȡcurFrame_rz�е�r���ο��С
				Mat src=curFrame_rz(r);
				//imshow("����ǰ",src);
				//��ȥ�������

				detectAndDraw(src,cascade, nestedCascade,1.0);
				if(isDetect==true){
					isChanged = true;
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

					imwrite(strPath+filename,src); //����������ͼƬ
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
					isDetect=false;
					seq++;
					lastMaskFrame = count;
				}

				lastRectHeight = d;
			}//else	
			count++; //֡����
			ratio = count/(double)capture.get(CAP_PROP_FRAME_COUNT);
			percent = ratio*100; //�ٷֱ�
			//dstImg = curFrame.clone();
			dstImg = &curFrame_rz;

			if( isChanged)
				strcpy(eventInfo, detectInfo.c_str());
		}//while		
		//��eventд��text
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

//��һ֡�����о��εı߽��ĵ�
void DetectMasked::findBoundary(Rect r,Point &a,Point &c){
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

//������Ⲣ����
void DetectMasked::detectAndDraw( Mat& img,
	CascadeClassifier& cascade, CascadeClassifier& nestedCascade,
	double scale){
	//cout<<"����detectAndDraw"<<endl;

	Mat img_ori=img.clone();
	//��ͼ���ֶ�����
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