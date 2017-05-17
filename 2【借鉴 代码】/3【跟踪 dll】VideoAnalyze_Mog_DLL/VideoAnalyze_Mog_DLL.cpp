// VideoAnalyze_Mog_DLL.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "VideoAnalyze_Mog_Dll.h"

#pragma comment( lib, "vfw32.lib" )
#pragma comment( lib, "comctl32.lib" ) 

cv::Scalar sampleColor[]={Scalar(0, 0, 0), Scalar(255, 0, 0), Scalar(0, 255, 0), Scalar(0, 0, 255), Scalar(255, 255, 0) };

VideoAnalyze_Mog* VideoAnalyze_Mog::instance = NULL;

VideoAnalyze_Mog* VideoAnalyze_Mog::getInstance()
{
	if (instance == NULL)
	{
		instance = new VideoAnalyze_Mog();
	}
	return instance;
}

vector<EventNode>* execAnalyze(std::string &videoName , int x_min , int y_min ,  std::string &savePath , Mat* & currentImage , int & progress ,
	int processInterval , bool &isReadFromFile , bool & isContinue , bool & isFinished)
{
	VideoAnalyze_Mog* ins = VideoAnalyze_Mog::getInstance();
	return ins->analyze(videoName , x_min , y_min , savePath , currentImage , progress , processInterval , isReadFromFile , isContinue , isFinished);
}

vector<EventNode>* VideoAnalyze_Mog::analyze(std::string &videoName ,  int x_min , int y_min , std::string &savePath , Mat* &currentImage ,  int & progress ,
	int processInterval , bool &isReadFromFile , bool & isContinue, bool & isFinished)
{
	vector<EventNode>* retList = new vector<EventNode>();

	cv::VideoCapture capture(videoName);
	if (!capture.isOpened())
		return NULL;

	cv::Size captureSize(capture.get(CV_CAP_PROP_FRAME_WIDTH ) , capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	int fps = capture.get(CV_CAP_PROP_FPS);
	int frameCount = capture.get(CV_CAP_PROP_FRAME_COUNT);

	Mat frame;
	int frameNum = 0;

	Mat img, fgmask, fgimg;
	BackgroundSubtractorMOG bg_model;

	while(frameNum<frameCount)
	{
		capture.read(frame);
		if (!frame.data || !isContinue)
		{
			progress = 100;
			currentImage = NULL;
			waitKey(100);//这里如果不暂停的话，会因为现实图片被释放而出现内存错误
			retList->clear();
			return retList;
		}

		if (frameNum % processInterval == 0)
		{
			bg_model(frame, fgmask, 0.005);
			refineSegments(retList , x_min , y_min , frame, fgmask, fgimg , frameNum , fps , frameCount);
			progress = (int)(100.0*(1+frameNum)/frameCount);
			currentImage = &frame;
			if (frameCount - frameNum <=3)  //frame即将要被释放，所以要先把currentImage置为空
			{
				currentImage = NULL;
			}
		}
		frameNum++;
	}

	//保存事件列表
	vector<std::string> videoName_split;
	std::string splitter = "/";
	splitString(videoName , splitter, videoName_split);
	if (videoName_split.size()==1)
	{
		videoName_split.clear();
		splitter = "\\";
		splitString(videoName , splitter, videoName_split);
	}
	if (videoName_split.size()>=1)
	{
		std::string saveName = videoName_split[videoName_split.size()-1];
		saveEventToFile(saveName , *retList , savePath , processInterval , fps);
	}else
	{
		printf("保存路径错误\n");
	}

	isContinue = false;
	progress = 0;
	isFinished = true;

	return retList;

}

void VideoAnalyze_Mog::refineSegments(vector<EventNode>* eventList ,  int x_min , int y_min , Mat img, Mat& mask, Mat& dst , int frameNum , int fps, int frameCount)
{
	int niters = 3;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	Mat temp;

	dilate(mask, temp, Mat(), Point(-1,-1), niters);//膨胀，3*3的element，迭代次数为niters
	erode(temp, temp, Mat(), Point(-1,-1), niters*2);//腐蚀
	dilate(temp, temp, Mat(), Point(-1,-1), niters);

	findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );//找轮廓

	dst = Mat::zeros(img.size(), CV_8UC3);

	if( contours.size() == 0 )
		return;

	int minArea = img.cols*0.05*img.rows*0.05;
	int maxArea = img.cols*img.rows*0.5;
	Scalar color( 0, 255, 0 );

	if (contours.size()>0)
	{
		for (int i = 0;i<contours.size();i++)
		{
			cv::Rect r = cv::boundingRect(contours[i]);
			double area = r.width*r.height;
			float x_ratio = x_min/100.0;
			float y_ratio = y_min/100.0;
			if (area <= maxArea && r.width>x_ratio*img.cols && r.height>y_ratio*img.rows
				&&r.x>15 &&r.y>15 && r.width<r.height*8 && r.height<r.width*8)
			{
				EventNode node;
				if ((*eventList).size() == 0&& (frameCount - frameNum)>2*fps)
				{
					node = insertEventNode(eventList, r, frameNum);
				}
				else
				{
					if ((frameCount - frameNum)>2*fps)
					{
						if(!searchEventList(eventList, r, node , frameNum))
							node = insertEventNode(eventList, r, frameNum);
					}	
				}
				cv::Scalar s = sampleColor[node.startFrame % 5];

				cv::rectangle(img , cv::Point(r.x, r.y), cv::Point(r.x + r.width, r.y + r.height), s, 1, CV_AA, 0);
			}
		}

	}

	int count = traverseEventList(eventList, frameNum, fps);
}

EventNode VideoAnalyze_Mog::insertEventNode(vector<EventNode> *eventList, Rect r, int frameNum)
{
	EventNode insert;
	insert.startFrame = frameNum;
	insert.endFrame = -1;
	insert.eventRect = r;
	insert.mark = true;

	//事件跟踪的表头
	EventElement element;
	element.rect = r;
	element.frameNum = frameNum;
	insert.trackList.push_back(element);
	eventList->push_back(insert);
	return insert;
}

bool VideoAnalyze_Mog::searchEventList(vector<EventNode> *eventList, Rect r2, EventNode&node , int frameNum)
{
	for (int i = 0; i < eventList->size(); i++)
	{
		if (eventList->at(i).endFrame != -1)
			continue;
		Rect r1 = eventList->at(i).eventRect;
		if (isTheSameRect(r1, r2))
		{
			eventList->at(i).mark = true;
			eventList->at(i).eventRect = r2;

			EventElement element;
			element.rect = r2;
			element.frameNum = frameNum;
			//在跟踪列表插入最后一帧
			eventList->at(i).trackList.push_back(element);
			node = eventList->at(i);

			return true;
		}
	}
	return false;
}

int VideoAnalyze_Mog::traverseEventList(vector<EventNode> *eventList, int endFrame, int fps)
{
	int count = 0;
	if (eventList->size() == 0)
		return count;
	for(vector<EventNode>::iterator iter=eventList->begin(); iter!=eventList->end(); )
	{

		if ((*iter).mark == false)
		{
			count++;
			if ((*iter).endFrame == -1)
			{
				(*iter).endFrame = endFrame;
				if( ((*iter).endFrame - (*iter).startFrame < fps*2) || ((*iter).startFrame < 17))  //2秒存在就记为事件,另外这里要开始帧大于17是因为如果小于17的话opencv无法定位，所以过滤掉
				{
					iter = eventList->erase(iter);
					count--;
					continue;
				}
			}
		}else
		{
			(*iter).mark = false;
		}
		iter++;
	}
	return count;
}

bool VideoAnalyze_Mog::isTheSameRect(Rect r1, Rect r2)
{
	if((r1.x+r1.width)<r2.x || r1.x>(r2.x+r2.width) || (r1.y+r1.height)<r2.y || r1.y>(r2.y+r2.height))
	{
		return false;
	}
	else if(r1.width/(r2.width+0.0)<0.5||r2.width/(r1.width+0.0)<0.5 || r1.height/(r2.height+0.0)<0.5||r2.height/(r1.height+0.0)<0.5)
	{
		return false;
	}else
	{
		int xLength[] = {r1.x, r1.x+r1.width, r2.x, r2.x+r2.width};
		int yHeight[] = {r1.y, r1.y+r1.height, r2.y, r2.y+r2.height};
		double mianji = findLength(xLength) * findLength(yHeight);
		double mianji1 = r1.width*r1.height;
		double mianji2 = r2.width*r2.height;
		if((mianji/mianji1 + mianji/mianji2)/2 > 0.65 && (mianji/mianji1 + mianji/mianji2)/2 <0.95 )
		{
			return true;
		}
	}
}

int VideoAnalyze_Mog::findLength(int a[])
{
	int minIndex = 0, maxIndex = 0;
	for (int i = 0; i < 4; i++)
	{
		if (a[i] < a[minIndex])
			minIndex = i;
		if (a[i] > a[maxIndex])
			maxIndex = i;
	}
	int length[2];
	int index = 0;
	for(int i = 0; i < 4; i++)
	{
		if(i != minIndex && i != maxIndex)
		{
			length[index] = a[i];
			index++;
		}
	}
	int result = length[0] - length[1];
	if (result > 0)
		return result;
	else
		return -result;
}

void VideoAnalyze_Mog::saveEventToFile(std::string &videoName , vector<EventNode> eventList , std::string &savePath , int interval , int fps)
{
	std::string newFilePath;
	if (newFilePath.find_last_of("\\") != strlen(newFilePath.c_str())-1)
	{
		newFilePath = savePath +"\\" + videoName + ".txt";
	}
	else
	{
		newFilePath = savePath + videoName + ".txt";
	}


	FILE*fs = fopen(newFilePath.c_str(), "w+");
	fprintf(fs, "%d %d %d\n", eventList.size(), interval, fps);

	for (int i = 0; i < eventList.size(); i++)
	{
		EventNode node = eventList[i];
		int startFrame = node.startFrame;
		int endFrame = node.endFrame;
		fprintf(fs, "%d %d %d ", startFrame, endFrame, node.trackList.size());
		for (int j = 0; j < node.trackList.size(); j++)
		{
			int x = node.trackList[j].rect.x;
			int y = node.trackList[j].rect.y;
			int width = node.trackList[j].rect.width;
			int height = node.trackList[j].rect.height;

			int frameNum = node.trackList[j].frameNum;
			fprintf(fs, "%d %d %d %d %d ", x, y, width, height , frameNum);
		}
		fprintf(fs, "\n");
	}

	fclose(fs);
}

void VideoAnalyze_Mog::splitString(std::string& s, std::string& delim,std::vector< std::string >& ret)  
{  
	size_t last = 0;  
	size_t index=s.find_first_of(delim,last);  
	while (index!=std::string::npos)  
	{  
		ret.push_back(s.substr(last,index-last));  
		last=index+1;  
		index=s.find_first_of(delim,last);  
	}  
	if (index-last>0)  
	{  
		ret.push_back(s.substr(last,index-last));  
	}  
}  

bool writeEventVideo(vector<EventNode>&eventList , std::string &originVideoPath , std::string &savePath , int &progress,int interval , bool & isContinue)
{
	//printf("%s\n" , savePath.c_str());
	cv::VideoCapture capture = VideoCapture(originVideoPath);
	int fps = capture.get(CV_CAP_PROP_FPS);
	cv::Size captupeSize = Size(capture.get(CV_CAP_PROP_FRAME_WIDTH) , capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	cv::VideoWriter videoWriter = VideoWriter(savePath , CV_FOURCC('X', 'V', 'I', 'D') , fps , captupeSize);

	int LIMIT = 100;
	int part = eventList.size() / LIMIT;
	if(eventList.size()%LIMIT!=0)
		part++;
	double alpha_value = 0.7;
	char eventNumber[128];
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0.5, 1, CV_AA);

	Mat allEventImage;
	Mat allEventImage_roi;
	capture.set(CV_CAP_PROP_POS_FRAMES , 18);
	Mat baseFrame;
	capture.read(baseFrame);

	Mat frame;
	Mat frame_roi;

	int totalWork = 0;
	totalWork = (part-1)*LIMIT+eventList.size();

	//将视频分成part端，分别对每段进行合成保存
	for(int i = 0; i < part; i++)
	{
		//计算每一段的下标
		int l_index, r_index;
		if(i != part-1)
		{
			l_index = i*LIMIT;
			r_index = (i+1)*LIMIT;
		}else
		{
			l_index = i*LIMIT;
			r_index = eventList.size();
		}

		int frameCount = 0;   //记录帧偏移量
		int endCount = 0;     //记录有几个节点已经完成

		while(endCount < r_index-l_index)
		{
			if (!isContinue)   //暂停的时候，返回
			{
				videoWriter.release();
				FILE* file = fopen(savePath.c_str(),"w");
				fclose(file);
				if( remove(savePath.c_str()) == 0 )
					printf("Removed %s.", savePath.c_str());
				else
					perror("remove");
				return false;
			}

			progress = 100.0*(endCount/(totalWork+0.0));  //计算进度
			allEventImage = baseFrame.clone();

			for(int j = l_index; j < r_index; j++)
			{
				int frameNum = eventList[j].startFrame+frameCount*interval;
				if(frameCount == eventList[j].trackList.size())  //注意这里一定是“==”，如果是“>=”的话会使endCount重复多加几次，损失很多帧
				{
					endCount++;
					continue;
				}else if(frameCount > eventList[j].trackList.size())
				{
					continue;
				}
				cv::Rect rect = eventList[j].trackList[frameCount].rect;

				capture.set(CV_CAP_PROP_POS_FRAMES, frameNum);
				capture.read(frame);
				//截取矩形图像合成
				if (!frame.empty())
				{
					frame_roi = frame(rect);
					allEventImage_roi = allEventImage(rect);
					sprintf(eventNumber, "%d", j+1);
					cv::putText(frame_roi, eventNumber, cv::Point(10, 15), CV_FONT_HERSHEY_SIMPLEX, 0.5, sampleColor[1]);
					cv::addWeighted(frame_roi , alpha_value , allEventImage_roi , 1.0-alpha_value , 0 , allEventImage_roi);
					Mat temp(allEventImage , rect);
					allEventImage_roi.copyTo(temp);
				}
			}
			for(int j = 0; j < interval; j++)  //重复写入jiange帧
			{
				videoWriter.write(allEventImage);
			}

			frameCount++;
		} // end while
	} // end for(int i = 0; i < part; i++)
	progress = 100;
	return true;
}