/*
 * sort.h
 *
 *  Created on: 2014-3-29
 *      Author: Cai
 */

#ifndef _H_SORT_CAI_
#define _H_SORT_CAI_
#include "pattern.h"

/*
 *  class name: Sort
 *  description:    分类，该类实现计算某一聚类的特征及合并类的
 *		方法，对分类进行一定封装。
 */

class Sort{
public:
	int dim;						//数据维度	
	int amount;					    //类中模式数目	
	Pattern center;					//类中心
	std::vector<Pattern> vPattern;  //模式容器
	std::vector<double> vSigma;		//x分量与y分量的标准差
	std::vector<double> vDist;      //模式到类心距离
	double avgDist;					//模式到类心的平均距离
	int maxIndex;					//最大标准差索引
	double maxSigma;				//最大标准差

public:
	Sort();
	Sort(int d);
	~Sort();

	int insert(const Pattern &p); //将p分为该类
	Pattern calCenter();		  //计算类中心
	double calAvgDist();		  //计算模式到类心的平均距离
	int calSigma();				  //计算类内距离的标准差矢量
	void removeExtra();            //剔除该类边缘点
	//bool combin(Sort &k);		  //当两类均未合并过时，合并
};

#endif