/*
 * isodata.cpp
 *
 *  Created on: 2014-3-29
 *      Author: Cai
 */

#include "isodata.h"
using std::vector;

/*
#include <iostream>
using std::cout;
using std::endl;*/

//静态成员指针初始化
Isodata* Isodata::instance = 0;

//单例模式返回实例
Isodata* Isodata::GetInstance(){
	if( instance == 0)
		instance = new Isodata();
	return instance;
}

//构造函数
Isodata::Isodata()
:dim(2), amount(0){
	expK = 4;
	initK = 4;
	thetaN = 60;
	thetaS = 1;
	thetaD = 65;
	maxPair = 1;
	maxIter = 4;

	avgDist = 0.0;
	nIter = 0;

	isAuto = true;

	vSort.clear();
	vPattern.clear();
	vCtrDist.clear();

	srand( (unsigned)time( NULL ));
}

//析构函数
Isodata::~Isodata(){
	if( instance != 0)
		delete instance;
}

/*
//配置二维模式样本
void Isodata::setupPattern(const std::vector<Point2f>& _pt){
	dim = 2;
	amount = _pt.size();
	for( int i = 0; i < amount; ++i){
		Pattern p;
		p.vDimx[0] = _pt[i].x;
		p.vDimx[1] = _pt[i].y;
		vPattern.push_back(p);
	}
}*/

//配置多维模式样本
void Isodata::setupPattern(const std::vector<std::vector<double>>& _pt){
	if( 0 == _pt.size())
		return;
	dim = _pt[0].size();
	amount = _pt.size();
	for( int i = 0; i < amount; ++i){
		Pattern p;
		for( int j = 0; j < dim; ++j)
			p.vDimx[j] = _pt[i][j];
		vPattern.push_back(p);
	}
}

void Isodata::initPara(){
	initK = 4;

	avgDist = 0.0;
	nIter = 0;

	vSort.clear();
	vCtrDist.clear();

}

//执行算法
int Isodata::Process(){
	initPara();

	int step = 1;
	bool isEnd = false;
	while( !isEnd){
		switch ( step){
			case 1:
				//step1:选定初始聚类中心
				if( isAuto)
					initCenter();
			case 2:
				//step2:依据最小距离原则将模式样本分给最近类
				clustering();
				reClustering();
			case 3:
				//step3:依据thetaN判断是否合并，若样本数过少，则取消该类中心
				combinInThetaN();
			case 4:
			case 5:
			case 6:
				//step4-6:计算分类后的参数：类中心、类内平均距离以及总体平均距离
				calPara();
			case 7:
				//step7: 入口选择
				if( nIter == maxIter){
					thetaD = 0.0;
					step = 11;
				}else if( initK < expK/2)
					step = 8;
				else if( 1 == nIter%2 || initK >= expK*2 )
					step = 11;
				else
					step = 8;
				break;
			case 8:
			case 9:
				//step8-9: 计算类内距离的标准差矢量、距离最大标准差矢量
				calSigma();
			case 10:
				//step10: 判断分裂条件
				if( split()){
					step = 2;
					++nIter;
					break;
				}
			case 11:
				//step11: 计算各类对中心间距离
				calCenterDist();
			case 12:
			case 13:
				//step12-13: 依据thetaD判断是否合并
				combinInThetaD();
			case 14:
				//step14: 判断结束条件
				if( nIter == maxIter)
					isEnd = true;
				else{
					step = 2;
					++nIter;
				}
				break;
			default:
				isEnd = true;
		}
	}

	return nIter;
}

//返回分类结果
void Isodata::getSortCenter(vector<SortCenter> & sortCtr){
	for( int i = 0; i < vSort.size(); ++i){
		SortCenter sc;
		sc.rad = vSort[i].avgDist + vSort[i].maxSigma;
		Pattern &p = vSort[i].center;
		for( int j = 0; j < vSort[i].dim; ++j)
			sc.vDim.push_back(p.vDimx[j]);
		sortCtr.push_back(sc);
	}
}

//step1: 
//初始化种子
void Isodata::initCenter(){
	vSort.clear();
	int n = amount;
	int* no = new int[n];
	for( int i = 0; i < n; ++i)
		no[i] = i;
	for( int j = 0; j < initK; ++j, --n){
		Sort _s;
		int k = rand()%n;

		//cout<<no[k]<<endl;

		_s.center = vPattern[no[k]];
		vSort.push_back(_s);

		for(int l = k; l < n; ++l)
			no[l] = no[l + 1];
	}
	if( no != NULL)
		delete []no;

	//cout<<endl;
/*
	vSort.clear();
	Sort _s;
	_s.center = vPattern[2273];
	vSort.push_back(_s);
	_s.center = vPattern[2792];
	vSort.push_back(_s);
	_s.center = vPattern[649];
	vSort.push_back(_s);
	_s.center = vPattern[3098];
	vSort.push_back(_s);
*/


/*

	int n = amount;
	int avg = amount/(initK+1);
	for( int j = 0; j < initK; ++j){
		Sort _s;
		_s.center = vPattern[avg*(j+1)];
		vSort.push_back(_s);
	}*/
}

//配置初始中心点
void Isodata::setupCenter(const std::vector<Pattern> &vp){
	vSort.clear();
	for( int i = 0; i < vp.size(); ++i){
		Sort _s;
		_s.center = vp[i];
		vSort.push_back(_s);
	}
	isAuto = false;
}

//设置控制参数
void Isodata::setupPara(int _expK, int _initK, int _thetaN, double _thetaS, double _thetaD, int _maxPair, int _maxIter){
	expK = _expK;
	initK = _initK;
	thetaN = _thetaN;
	thetaS = _thetaS;
	thetaD = _thetaD;
	maxPair = _maxPair;
	maxIter = _maxIter;
}

//step2: 以最小距离将模式集的每个模式分类
void Isodata::clustering(){
	for(int j = 0;j < initK; ++j){
		vSort[j].amount = 0;
		vSort[j].center.n = 0;
		vSort[j].vPattern.clear();
	}

	classify(vPattern);
}

void Isodata::reClustering(){
	for(int i = 0; i < vSort.size(); ++i){
		vSort[i].calCenter();
	}
	clustering();
}

void Isodata::classify(const std::vector<Pattern> &vp){
	int index, n = vp.size();
	double dist, minDist;
	for(int i = 0; i < n; ++i){
		index = 0;
		minDist = Pattern::Distance(vp[i], vSort[0].center);
		for(int j = 1; j < initK; ++j){
			dist = Pattern::Distance(vp[i], vSort[j].center);
			if( dist < minDist){
				minDist = dist;
				index = j;
			}
		}
		vSort[index].insert(vp[i]);
	}
}


//step3: 依据thetaN判断是否合并，若样本数过少，则取消该类中心
void Isodata::combinInThetaN(){
	vector<Pattern> vp;
	bool isAll = true;
	while( isAll){
		isAll = false;
		vp.clear();
		for( vector<Sort>::iterator iter = vSort.begin(); iter != vSort.end(); ++iter){
			if( (*iter).amount < thetaN){
				vp = (*iter).vPattern;
				iter = vSort.erase(iter);
				initK = vSort.size();
				isAll = true;
				classify(vp);
				break;
			}
		}
	}
}

//step4-6: 计算分类后的参数：类中心、类内平均距离以及总体平均距离
void Isodata::calPara(){
	avgDist = 0.0;
	for( int j = 0; j < initK; ++j){
		//计算中心
		vSort[j].calCenter();
		//类内平均距离
		avgDist += vSort[j].amount *vSort[j].calAvgDist();
	}
	//计算总体平均距离
	avgDist /= amount;
}

//step7: 入口选择

//step8-9: 计算类内距离的标准差矢量、距离最大标准差矢量
void Isodata::calSigma(){
	for( int j = 0; j < initK; ++j)
		vSort[j].calSigma();
}

//step10: 判断分裂条件
bool Isodata::split(){
	double k = 0.5;
	for( int j = 0; j < initK; ++j){
		if((( vSort[j].maxSigma > thetaS) && 
			((vSort[j].amount > 2*(thetaN+1))|| (initK <= expK/2)))){
			int n = vSort[j].maxIndex;
			double sigma = vSort[j].maxSigma;

			Sort _s = vSort[j];
			vSort[j].center.vDimx[n] += k*sigma;
			_s.center.vDimx[n] -= k*sigma;
			vSort.push_back(_s);
			initK = vSort.size();
			return true;
		}
	}
	return false;
}

//step11: 计算各类对中心间距离
void Isodata::calCenterDist(){
	vCtrDist.clear();
	for( int i = 0; i < initK; ++i){
		vector<double> vDist;
		for( int j = 0; j < initK; ++j){
			double dist = Pattern::Distance(vSort[i].center, vSort[j].center);
			vDist.push_back(dist);
		}
		vCtrDist.push_back(vDist);
	}
}

//step12-13: 依据thetaD判断是否合并
bool Isodata::combinInThetaD(){
	bool isCombin = false;

	//获得小于距离阈值的类对
	vector<DistPair> vDistPair;
	for( int i = 0; i < initK -1; ++i)
		for( int j = i +1; j < initK; ++j)
			if( vCtrDist[i][j] < thetaD){
				DistPair dp;
				dp.dist = vCtrDist[i][j];
				dp.i = i;
				dp.j = j;
				vDistPair.push_back(dp);
			}

	//初始化索引值
	int n;
	n = vDistPair.size();
	if( 0 == n)
		return false;

	int *index = new int[n];
	for( int i = 0; i < n; ++i)
		index[i] = i;
	//排序
	for( int i = 1; i < n; ++i){
		double dist = vDistPair[i].dist;
		int m = index[i];
		int j = 0;
		while( j < i && vDistPair[j].dist <= dist)
			j++;

		if( j < i){
			int k = i;
			while( k > j){
				vDistPair[k].dist = vDistPair[k-1].dist;
				index[k] = index[k-1];
				k--;
			}				
			vDistPair[j].dist = n;
			index[j] = m;
		}
	}

	//从最小距离开始，将相应两类合并
	for( int i = 0; i < maxPair; ++i){
		Sort & sort_m = vSort[vDistPair[index[i]].i];
		Sort & sort_n = vSort[vDistPair[index[i]].j];

		if( (sort_m.center.n < 0) || (sort_n.center.n < 0))
			continue;

		sort_m.center = (1.0/ (sort_m.amount + sort_n.amount))*(sort_m.amount* sort_m.center + sort_n.amount* sort_n.center);
		for( int j = 0; j < sort_n.vPattern.size(); ++j)
			sort_m.vPattern.push_back(sort_n.vPattern[j]);

		sort_m.center.n = -1;	//作为新的聚类中心
		sort_n.center.n = -2;	//应取消的聚类中心

		isCombin = true;
	}


	//去掉已撤销的类心
	if( isCombin){
		for( vector<Sort>::iterator iter = vSort.begin(); iter != vSort.end(); ){
			if( (*iter).center.n == -2){
				iter = vSort.erase(iter);
				continue;
			}
			(*iter).center.n = 0;
			++iter;
		}
	}
	initK = vSort.size();

	if( index != NULL)
		delete []index;

	return isCombin;
}

void Isodata::removeExtra(){
	for(int i = 0; i < vSort.size(); ++i){
		vSort[i].removeExtra();
	}
}

int Isodata::getK() const{
	return vSort.size();
}

//step14: 判断结束条件