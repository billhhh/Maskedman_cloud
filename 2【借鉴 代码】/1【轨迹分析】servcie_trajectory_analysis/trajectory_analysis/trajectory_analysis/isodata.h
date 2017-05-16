/*
 * isodata.h
 *
 *  Created on: 2014-3-29
 *      Author: Cai
 */

#ifndef _H_ISODATA_CAI_
#define _H_ISODATA_CAI_

#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <algorithm>

#include "sort.h"

/*
 *  class name: Isodata
 *  description: Isodata�࣬����Ϊʵ��ISODATA�㷨����Ҫ��
 *		�㷨�����̰��ս̲��������Ĳ�����У���Ҫ����ִ�к���Ϊ��
 *      int ISODATA::Process();
 */

typedef struct{
	double dist;
	int i;
	int j;
}DistPair;

//��������
typedef struct{
	double rad;
	int remainTime;
	std::vector<double> vDim;
}SortCenter;

class Isodata{
private:
	int expK;		//Ԥ������K
	int initK;		//��ʼ�������ĸ���C
	int thetaN;		//ÿ����������������������N
	double thetaS;	//ͬһ��������������׼������ަ�s
	double thetaD;	//��ͬ���������ľ������ަ�c
	int maxPair;	//ÿ�ε��������Ժϲ��ľ������ĵ�������L
	int maxIter;    //����������I

	std::vector<Sort> vSort;
	std::vector<Pattern> vPattern;
	std::vector<std::vector<double>> vCtrDist;
	int dim;
	int amount;
	double avgDist;
	int nIter;

	static Isodata* instance;
	
	bool isAuto;
public:
	~Isodata();
	static Isodata* GetInstance();

	//void setupPattern(const std::vector<Point2f>& _pt);
	void setupPattern(const std::vector<std::vector<double>>& _pt);
	void getSortCenter(std::vector<SortCenter> & sortCtr);
	int Process();
	void removeExtra();
	int getK() const;

private:
	Isodata();
	void setupPara(int _expK, int _initK, int _thetaN, double _thetaS, double _thetaD, int _maxPair, int _maxIter);
	void setupCenter(const std::vector<Pattern> &vp);
	void initCenter();
	void clustering();
	void reClustering();
	void combinInThetaN();
	void calPara();
	void calSigma();
	bool split();
	void calCenterDist();
	bool combinInThetaD();
	void classify(const std::vector<Pattern> &vp);
	void initPara();

};


#endif