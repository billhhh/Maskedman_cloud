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
 *  description: Isodata类，该类为实现ISODATA算法的主要类
 *		算法的流程按照教材所给出的步骤进行，主要流程执行函数为：
 *      int ISODATA::Process();
 */

typedef struct{
	double dist;
	int i;
	int j;
}DistPair;

//聚类中心
typedef struct{
	double rad;
	int remainTime;
	std::vector<double> vDim;
}SortCenter;

class Isodata{
private:
	int expK;		//预期类数K
	int initK;		//初始聚类中心个数C
	int thetaN;		//每个聚类域中最少样本数θN
	double thetaS;	//同一聚类域中样本标准差的上限θs
	double thetaD;	//不同聚类域中心距离下限θc
	int maxPair;	//每次迭代最多可以合并的聚类中心的最多对数L
	int maxIter;    //最大迭代次数I

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