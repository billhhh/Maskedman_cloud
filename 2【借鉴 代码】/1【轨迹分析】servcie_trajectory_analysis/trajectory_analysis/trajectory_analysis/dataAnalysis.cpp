
/*
 * dataAnalysis.cpp
 *
 *  Created on: 2014-3-31
 *      Author: Cai
 */

#include "dataAnalysis.h"
using namespace std;
using namespace cv;

/*
#include <iostream>
using std::cout;
using std::endl;*/

//静态成员指针初始化
DataAnalysis* DataAnalysis::instance = 0;

//单例模式返回实例
DataAnalysis* DataAnalysis::GetInstance(){
	if( instance == 0)
		instance = new DataAnalysis();
	return instance;
}

//构造函数
DataAnalysis::DataAnalysis()
:imgWidth(0), imgHeight(0), fps(0), frameCount(0), totalPeople(0), hasPath(false){
	fileName.clear();
	vTrackInfo.clear();
	vPattern.clear();
	vSortCenter.clear();

	vAllPoint.clear();
	vFrameNum.clear();
	vTrackSegment.clear();
}

//析构函数
DataAnalysis::~DataAnalysis(){
	if( instance != 0)
		delete instance;
}

void DataAnalysis::initData(){
	vSortCenter.clear();
	vTrackSegment.clear();
	vCenter.clear();
}

//设置背景图片
bool DataAnalysis::setImage(const string& str){
	oriImg = imread(str.c_str());
	if( !oriImg.data)
		return false;

	//imshow("original", oriImg);
	return true;
}

//读取数据
bool DataAnalysis::readData(const string& str){
	vTrackInfo.clear();

	string buf;
	fstream coord;
	coord.open(str.c_str(), ios::in);
	if( !coord.is_open())
		return false;

	//记录数据读取步骤
	int step = 0;
	while( coord){
		getline(coord, buf);
		if(!buf.compare("[File]")){
			getline(coord, fileName);
			++step;
		}else if(!buf.compare("[Size]")){
			coord>>imgWidth;
			coord>>imgHeight;
			++step;
		}else if(!buf.compare("[Fps]")){
			coord>>fps;
			++step;
		}else if(!buf.compare("[FrameCount]")){
			coord>>frameCount;
			++step;
		}else if(!buf.compare("[Content]")){
			++step;
			break;
		}
	}
	if( step != 5)
		return false;

	char c;
	int frameNum;
	vector<COORDINFRAME> vCif;
	int contentLine = 0;
	while( !coord.eof()){
		coord>>frameNum;
		vCif.clear();
		COORDINFRAME coordTemp;
		do{
			coord>>coordTemp.x;
			coord>>coordTemp.y;
			coord>>coordTemp.w;
			coord>>coordTemp.h;
			coordTemp.frameNum = frameNum;
			vCif.push_back(coordTemp);
			coord.get(c);
			c = coord.peek();
		}while( c != '\n' && c != -1);
		vTrackInfo.push_back(vCif);
		++contentLine;
	}
	coord.close();
	if( 0 == contentLine)
		return false;
	return true;
}

//
void DataAnalysis::generatePtData(){
	ptData = Mat::zeros( imgHeight, imgWidth, CV_32SC1);
	ptData.setTo(0);
	for(int i = 0; i < vPattern.size(); ++i){
		int cols = vPattern[i][0];
		int rows = vPattern[i][1];
		ptData.at<int>(rows, cols)++;
	}

	//获得图像宽高
	int ImgCols = ptData.cols;
	int ImgRows = ptData.rows;
	//将图像分块，申请动态数组
	int blockLength = 10;
	int block_w = ImgCols/blockLength;
	int block_h = ImgRows/blockLength;
	int *blockData = new int[block_w*block_h];
	//初始化
	for( int i = 0; i < block_w*block_h; ++i)
		blockData[i] = 0;
	//将数据分块
	for( int j = 0; j < ImgRows - blockLength; j += blockLength)
		for( int i = 0; i < ImgCols - blockLength; i += blockLength)
			for( int n = 0; n < blockLength; ++n)
				for( int m = 0; m < blockLength; ++m)
					blockData[i/blockLength +j/blockLength*block_w] += ptData.at<int>(j+n, i+m);

	Mat ptDraw = Mat::zeros( oriImg.size(), CV_8UC3);
	ptDraw.setTo(255);
	for( int j = 0; j < block_h; ++j){
		for( int i = 0; i < block_w; ++i){
			if( blockData[i+j*block_w] > 0)
				circle(ptDraw, Point(i*blockLength+blockLength/2, j*blockLength+blockLength/2), blockLength/2, Scalar(0, 0, 0), blockLength/2, 8, 0);
		}
	}
	//imshow("ptDraw", ptDraw);

	//释放资源
	if( blockData != NULL){
		delete []blockData;
		blockData = NULL;
	}

}

//获得直观分析结果图
void DataAnalysis::getDrawImg(cv::Mat& img){
	img = drawImg.clone();
}

//提取适当的模式样本
void DataAnalysis::peekPattern(){
	//清除数据点
	vPattern.clear();
	vFrameNum.clear();

	int remainTime = fps/2;
	vector<double> pt;
	bool isRemain = false;
	for(int i = 0; i < vTrackInfo.size()- remainTime; ++i){////保留至少连续移动半秒的点
		vector<COORDINFRAME> &vCif = vTrackInfo[i];
		for( int m = 0; m < vCif.size(); ++m){
			int x = RECT_CENTERX(vCif[m]);
			int y = RECT_CENTERY(vCif[m]);
			for( int j = 1; j < remainTime; ++j){
				isRemain = false;
				vector<COORDINFRAME> &vRt = vTrackInfo[i+j];		
				for( int n = 0; n < vRt.size(); ++n){
					int x2 = RECT_CENTERX(vRt[n]);
					int y2 = RECT_CENTERY(vRt[n]);				
					Point vec = Point(x-x2, y-y2);
					if( vec.dot(vec) < 25){
						isRemain = true;
						break;
					}
				}
				if( !isRemain)
					break;
			}
			if( isRemain){
				pt.clear();
				pt.push_back(x);
				pt.push_back(y);
				vPattern.push_back(pt);
				vFrameNum.push_back(vCif[m].frameNum);
			}
		}
	}
}

//进行数据分析
void DataAnalysis::analysis(){

	initData();
	peekPattern();
	generatePtData();
	isodata->GetInstance()->setupPattern(vPattern);

	for( int i = 0; i < 7; ++i){
	vSortCenter.clear();

	isodata->GetInstance()->Process();
	//isodata->GetInstance()->removeExtra();
	
	isodata->GetInstance()->getSortCenter(vSortCenter);
	//drawCenter();
	//imshow("drawImg", drawImg);

	countRemainTime();
	countPeople();
	if( vSortCenter.size() <2)
		continue;

	selectCenterPre();
	}
	vSortCenter = vCenter;

	generateSegment();
	dividePoint();
	selectSegment();
	selectCenter();

	drawPeopleFlow();
	//drawSegment();
	if(! hasPath){
		//imshow("drawImg", drawImg);
	}else{
		string totalPath = filePath + "\\" + fileName + ".png";
		imwrite( totalPath, drawImg);
	}
	//imshow("drawImg", drawImg);
}

//绘出聚类中心
void DataAnalysis::drawCenter(){
	drawImg = Mat::zeros( imgHeight, imgWidth, CV_8UC3);
	drawImg.setTo(255);
	for( int i = 0; i < vSortCenter.size(); ++i){
		SortCenter &sc = vSortCenter[i];
		circle(drawImg, Point(sc.vDim[0], sc.vDim[1]), 30/*sc.rad*/, Scalar(0, 0, 0), 2, 8, 0);
	}
	addWeighted(oriImg, 0.8, drawImg, 0.2, 1, drawImg);
}

//生成轨迹线
void DataAnalysis::generateSegment(){
	//申请动态数组并初始化
	int n = vSortCenter.size();
	int *pline = new int[n*n];
	for( int i = 0; i < n; ++i){
		for( int j = 0; j < n; ++j)
			pline[i*n+j] = 0;
	}


	for( int i = 0; i < vSortCenter.size(); ++i){
		SortCenter &sc = vSortCenter[i];
		Point pt1 = Point(sc.vDim[0], sc.vDim[1]);

		double minDist = imgWidth*imgHeight;
		double minDist2nd = minDist;
		int minIndex = i;
		int minIndex2nd = minIndex;
		for( int j = 0; j < vSortCenter.size(); ++j){
			if( i == j)
				continue;

			SortCenter &sc2 = vSortCenter[j];
			Point pt2 = Point(sc2.vDim[0], sc2.vDim[1]);
			double dist = TrackSegment::Distance(pt1, pt2);
			if( dist < minDist){
				minDist2nd = minDist;
				minDist2nd = minIndex;
				minDist = dist;
				minIndex = j;
			}else if( dist < minDist2nd){
				minDist2nd = dist;
				minDist2nd = j;
			}
		}

		//求最近两点向量夹角
		Point vec1 = Point(vSortCenter[minIndex].vDim[0], vSortCenter[minIndex].vDim[1]) - pt1;
		Point vec2 = Point(vSortCenter[minIndex2nd].vDim[0], vSortCenter[minIndex2nd].vDim[1]) - pt1;
		double cosVal = vec1.ddot(vec2) / sqrtf(vec1.ddot(vec1) * vec2.ddot(vec2));

		int k = i;
		int l = minIndex;			
		if( k > l){
			k = l;
			l = i;
		}
		pline[k*n +l] = 1;

		if( cosVal < 0.0){
			if( i < minDist2nd){
				k = i;
				l = minDist2nd;
			}else{
				k = minDist2nd;
				l = i;
			}
			pline[k*n +l] = 1;
		}
		
		//vTrackSegment.push_back(TrackSegment(pt1, Point(vSortCenter[minIndex].vDim[0], vSortCenter[minIndex].vDim[1])));
	}

	for( int i = 0; i < n; ++i)
		for( int j = i; j < n; ++j){
			if( 1 == pline[i*n +j])
				vTrackSegment.push_back(TrackSegment(
				Point(vSortCenter[i].vDim[0], vSortCenter[i].vDim[1]), 
				Point(vSortCenter[j].vDim[0], vSortCenter[j].vDim[1])));
		}


	//归还申请空间
	if( pline != NULL)
		delete []pline;
}

//划分点集到轨迹线
void DataAnalysis::dividePoint(){
	//找到最近连线
	for( int i = 0; i < vAllPoint.size() ; ++i){
		Point pt = Point(vAllPoint[i][0], vAllPoint[i][1]);

		double minDist = vTrackSegment[0].pointSegment(pt);
		int indexMin = 0;
		for( int j = 1; j < vTrackSegment.size(); ++j){
			double dist = vTrackSegment[j].pointSegment(pt);
			if( dist < minDist){
				minDist = dist;
				indexMin = j;
			}
		}

		vTrackSegment[indexMin].addPoint(pt);
	}
}

//筛选合适轨迹线
void DataAnalysis::selectSegment(){
	int n = vTrackSegment.size();
	int total = 0;
	double *cnt = new double[n];
	for( int j = 0; j < vTrackSegment.size(); ++j){
		cnt[j] = vTrackSegment[j].getCount();
		total += cnt[j];
	}

	vector<TrackSegment> vts;
	for( int i = 0; i < n; ++i){
		cnt[i] /= total;
		if( cnt[i] > 0.35)
			vTrackSegment[i].setLevel(RANK_RED);
		else if( cnt[i] > 0.20)
			vTrackSegment[i].setLevel(RANK_GREEN);
		else if( cnt[i] > 0.05)
			vTrackSegment[i].setLevel(RANK_BLUE);
		else
			continue;

		vts.push_back(vTrackSegment[i]);
	}
	vTrackSegment = vts;

	if( cnt != NULL)
		delete []cnt;
}

//按重要性画出轨迹线
void DataAnalysis::drawSegment(){
	for( int i = 0; i < vTrackSegment.size(); ++i){
		TrackSegment &ts = vTrackSegment[i];

		Scalar color = Scalar(255, 255, 255);
		switch( ts.getLevel()){
			case RANK_NOCOLOR:
				break;
				//continue;
			case RANK_BLUE:
				color = BLUE;
				break;
			case RANK_GREEN:
				color = GREEN;
				break;
			case RANK_YELLOW:
				color = YELLOW;
				break;
			case RANK_RED:
				color = RED;
				break;
			default:
				continue;
		}
		line(drawImg, ts.getPoint1(), ts.getPoint2(), color, 2, 8);
	}
	//addWeighted(oriImg, 0.7, drawImg, 0.3, 1, drawImg);
}

//统计总人数
void DataAnalysis::countPeople(){
	//清除数据点
	vAllPoint.clear();
	totalPeople = 0;

	vector<COORDINFRAME> vPossiblePt;
	vector<COORDINFRAME> vPossiblePt2;

	vector<double> pt;
	bool isRemain = false;

	vector<COORDINFRAME> vRect = vTrackInfo[0];
	for(int i = 1; i < vTrackInfo.size(); ++i){
		vector<COORDINFRAME> &vCif = vTrackInfo[i];
		for( int m = 0; m < vCif.size(); ++m){
			int x = RECT_CENTERX(vCif[m]);
			int y = RECT_CENTERY(vCif[m]);

			isRemain = false;
			
			for( int j = 0; j < vPossiblePt.size(); ++j){
				for( int n = 0; n < vRect.size(); ++n){
					if( rectIntersectAera(vRect[n], vPossiblePt[j]) > 0){
						vPossiblePt[j].frameNum++;
						if( vPossiblePt[j].frameNum > fps*2.5)
							totalPeople++;
						else
							vPossiblePt2.push_back(vPossiblePt[j]);
						break;
					}
				}
			}
			vPossiblePt = vPossiblePt2;
			vPossiblePt2.clear();

			for( int n = 0; n < vRect.size(); ++n){
				//double rect_threshold = MIN( RECT_AREA(vCif[m]), RECT_AREA( vRect[n]))/2;
				if( rectIntersectAera(vRect[n], vCif[m]) > 0){
					isRemain = true;
					break;
				}
			}
			if( !isRemain){
				//totalPeople++;
				COORDINFRAME ppt = vCif[m];
				ppt.frameNum = 0;
				vPossiblePt.push_back(ppt);
			}
			vRect = vTrackInfo[i];

			//保存所有点
			pt.clear();
			pt.push_back(x);
			pt.push_back(y);
			vAllPoint.push_back(pt);
		}
	}
}

//判断是否相交，返回相交面积
double DataAnalysis::rectIntersectAera(const COORDINFRAME& rect1, const COORDINFRAME& rect2){
	double minx = MAX(rect1.x, rect2.x);
	double miny = MAX(rect1.y, rect2.y);
	double maxx = MIN((rect1.x+rect1.w), (rect2.x +rect2.w));
	double maxy = MIN((rect1.y+rect1.h), (rect2.y +rect2.h));

	if( minx>maxx || miny>maxy)
		return 0.0;

	return (maxx-minx)*(maxy-miny); 
}

void DataAnalysis::countRemainTime(){
	vector<vector<int>> vSortTimeSet;
	for( int i = 0; i < vSortCenter.size(); ++i){
		vector<int> timeFrame;
		vSortTimeSet.push_back(timeFrame);
	}

	for(int j = 0; j < vPattern.size(); ++j){
		Point pt = Point(vPattern[j][0], vPattern[j][1]);
		double minDist = 50;
		for( int i = 0; i < vSortCenter.size(); ++i){
			SortCenter &sc = vSortCenter[i];
			Point ctrPt = Point(sc.vDim[0], sc.vDim[1]);
			
			double dist = TrackSegment::Distance(pt, ctrPt);
			if( dist < minDist){
				(vSortTimeSet[i]).push_back(vFrameNum[j]);
				break;
			}
		}
	}

	for( int i = 0; i < vSortTimeSet.size(); ++i){
		vector<int> &timeFrame = vSortTimeSet[i];
		vector<int> perTime;
		double waitFrame = 0;
		for( int j = 1; j < timeFrame.size(); ++j){
			if( timeFrame[j] - timeFrame[j-1] > 6){
				if( waitFrame > 10){
					perTime.push_back(waitFrame);
					waitFrame = 0;
				}
			}
			waitFrame++;
		}

		double sumWaitTime = 0.0;
		for( int j = 0; j < perTime.size(); ++j){
			sumWaitTime += perTime[j];
		}
		double avgTime = perTime.size() > 0? sumWaitTime/ perTime.size() :0;
		vSortCenter[i].remainTime = avgTime;
	}

}

void DataAnalysis::drawPeopleFlow(){
	drawImg = oriImg.clone();

	char cNum[10];
	char flow[30] = "Total of people flow: ";
	itoa( totalPeople, cNum, 10);
	strcat(flow, cNum);

	Mat ptDraw = Mat::zeros( oriImg.size(), CV_8UC3);
	ptDraw.setTo(0);
	putText( ptDraw, flow, Point(20, 60), 4, 0.4, Scalar(255, 255, 255), 1, 8);

	for( int i = 0; i < vTrackSegment.size(); ++i){
		TrackSegment &ts = vTrackSegment[i];

		Scalar color = Scalar(255, 255, 255);
		switch( ts.getLevel()){
			case RANK_NOCOLOR:
				continue;
			case RANK_BLUE:
				color = BLUE;
				break;
			case RANK_GREEN:
				color = GREEN;
				break;
			case RANK_RED:
				color = RED;
				break;
			default:
				continue;
		}
		line(ptDraw, ts.getPoint1(), ts.getPoint2(), color, 2, 8);
	}	
	for( int i = 0; i < vSortCenter.size(); ++i){
		SortCenter &sc = vSortCenter[i];
		if( sc.remainTime > 0){
			circle(ptDraw, Point(sc.vDim[0], sc.vDim[1]), 30/*sc.rad*/, YELLOW, 2, 8, 0);
			itoa( sc.remainTime, cNum, 10);
			putText( ptDraw, cNum, Point(sc.vDim[0], sc.vDim[1]), 4, 0.4, Scalar(255, 255, 255), 1, 8);
		}
	}

	addWeighted(ptDraw, 0.6, drawImg, 0.4, 1, drawImg);
/*
	drawImg = Mat::zeros( imgHeight, imgWidth, CV_8UC3);
	drawImg.setTo(255);

	addWeighted(oriImg, 0.8, drawImg, 0.2, 1, drawImg);

	itoa( totalPeople, cNum, 10);
	putText( drawImg, cNum, Point(0.25*imgWidth - 1, 0.25*imgHeight -1), 1, 3, Scalar(0, 255, 0), 3, 8);*/
}

void DataAnalysis::selectCenterPre(){
	vector<SortCenter> &_vsc = vSortCenter;
	//cout<<"vsc"<<vSortCenter.size()<<endl;
	//cout<<"vc"<<vCenter.size()<<endl;

	vector<SortCenter> vsc;
	for( int i = 0; i < _vsc.size(); ++i){
		SortCenter &sc = _vsc[i];
		if( sc.remainTime > 0){
			vsc.push_back(sc);
		}
	}
	if( vsc.empty())
		return;

	if( vCenter.empty()){
		vCenter.push_back(vsc[0]);
	}
	for( int i = 0; i < vsc.size();  ++i){
		SortCenter &sc = vsc[i];
		Point center1(sc.vDim[0], sc.vDim[1]);
		bool isMerge = false;
		for( int j = 0; j < vCenter.size(); ++j){
			SortCenter &sc2 = vCenter[j];
			Point center2(sc2.vDim[0], sc2.vDim[1]);
			if( TrackSegment::Distance(center1, center2) < 70){
				double ratio = sc.remainTime / (double)(sc.remainTime +sc2.remainTime);
				sc2.vDim[0] = (int)(center1.x*ratio + center2.x*(1-ratio));
				sc2.vDim[1] = (int)(center1.y*ratio + center2.y*(1-ratio));
				sc2.remainTime = (int)((sc.remainTime + sc2.remainTime)/2);
				isMerge = true;
				break;
			}
		}
		if( !isMerge)
			vCenter.push_back(sc);
	}
}

void DataAnalysis::selectCenter(){
	vector<SortCenter> vsc;
	for( int i = 0; i < vSortCenter.size(); ++i){
		SortCenter &sc = vSortCenter[i];
		Point center(sc.vDim[0],  sc.vDim[1]);
		for( int j = 0; j < vTrackSegment.size(); ++j){
			if( vTrackSegment[j].getPoint1() == center || vTrackSegment[j].getPoint2() == center){
				vsc.push_back(sc);
				break;
			} 
		}
	}
	vSortCenter = vsc;
}

void DataAnalysis::setPath(const std::string& str){
	if( str.empty()){
		hasPath = false;
		filePath.clear();
	}else{
		hasPath = true;
		filePath = str;
	}
}