/*
 * sort.cpp
 *
 *  Created on: 2014-3-29
 *      Author: Cai
 */

#include "sort.h"

//构造函数
Sort::Sort()
:dim(2), amount(0), avgDist(0), maxIndex(0), maxSigma(0){
	vPattern.clear();
	vSigma.clear();
	vDist.clear();
}

Sort::Sort(int d)
:dim(d), amount(0), avgDist(0), maxIndex(0), maxSigma(0){
	vPattern.clear();
	vSigma.clear();
	vDist.clear();
}

Sort::~Sort(){}

//将p分为该类
int Sort::insert(const Pattern &p){
	vPattern.push_back(p);
	vSigma.push_back(0.0);
	amount = vPattern.size();

	return amount;
}

//计算类中心
Pattern Sort::calCenter(){
	center = Pattern(0, dim);
	for( int i = 0; i < amount; ++i)
		center += vPattern[i];
	center /= amount;

	return center;
}

//计算模式到类心的平均距离
double Sort::calAvgDist(){
	vDist.clear();
	avgDist = 0.0;
	for( int i = 0; i < amount; ++i){
		double dist = Pattern::Distance(vPattern[i], center);
		vDist.push_back(dist);
		avgDist += dist;
	}
	avgDist /= amount;

	return avgDist;
}

//计算类内距离的标准差矢量
int Sort::calSigma(){
	maxIndex = 0;
	for( int j = 0; j < dim; ++j){
		double t = 0.0;
		for( int i = 0; i < amount; ++i)
			t += ( vPattern[i].vDimx[j] - center.vDimx[j])* (vPattern[i].vDimx[j] - center.vDimx[j]);
		vSigma[j] = sqrt(t/amount);

		if( maxSigma < vSigma[j]){
			maxSigma = vSigma[j];
			maxIndex = j;
		}
	}
	return maxIndex;
}

/*
//当两类均未合并过时，合并
bool Sort::combin(Sort &k){
	if( (center.n < 0) || (k.center.n < 0))
		return false;

	center = ( 1/ (amount + k.amount))* (amount* center + k.amount* k.center);
	center.n = -1;		//作为新的聚类中心
	k.center.n = -2;	//应取消的聚类中心

	return true;
}*/

void Sort::removeExtra(){
	for(int i = 0; i < vDist.size(); ++i){
		if( vDist[i] > avgDist){
			vDist.erase(vDist.begin()+i);
			vPattern.erase(vPattern.begin()+i);
		}
	}

	amount = vPattern.size();
	calCenter();
	calAvgDist();
	calSigma();
}