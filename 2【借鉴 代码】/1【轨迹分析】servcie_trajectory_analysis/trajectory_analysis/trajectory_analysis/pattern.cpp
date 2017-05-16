/*
 * pattern.cpp
 *
 *  Created on: 2014-3-29
 *      Author: Cai
 */

#include "pattern.h"


int Pattern::dim = 2; 

double Pattern::Distance(const Pattern x1, const Pattern x2){
	double temp = 0.0;
	for( int i = 0; i < dim; ++i)
		temp += (x1.vDimx[i] - x2.vDimx[i])*(x1.vDimx[i] - x2.vDimx[i]);
	return sqrt(temp);
}

Pattern::Pattern()
:n(0), vDimx(0){
	dim = 2;
	for( int i = 0; i < dim; ++i)
		vDimx.push_back(0.0);
}

Pattern::Pattern(int i, int d, double x[])
:n(i), vDimx(0){
	dim = d;
	if( x != NULL){
		for( int i = 0; i < dim; ++i)
			vDimx.push_back(x[i]);
	}else{
		for( int i = 0; i < dim; ++i)
			vDimx.push_back(0.0);
	}
}

//重定义模式维度
void Pattern::reDefine(int d){
	dim = d;
	vDimx.clear();
	for( int i = 0; i < dim; ++i)
		vDimx.push_back(0.0);
}

Pattern & Pattern::operator =(const Pattern & p){
	if( this != &p){
		n = p.n;
		vDimx = p.vDimx;
	}
	return *this;
}

Pattern & Pattern::operator +=(const Pattern & p){
	n = 0;
	for( int i = 0; i < dim; ++i)
		vDimx[i] += p.vDimx[i];
	return *this;
}

Pattern & Pattern::operator /=(const int n){
	for( int i = 0; i < dim; ++i)
		vDimx[i] /= n;
	return *this;
}

Pattern operator +(const Pattern & p1, const Pattern & p2){
	Pattern p(0, p1.dim);
	for( int i = 0; i < p.dim; ++i)
		p.vDimx[i] = p1.vDimx[i] + p2.vDimx[i];
	return p;
}

Pattern operator *(const int & n, const Pattern & p1){
	Pattern p(0, p1.dim);
	for( int i = 0; i < p.dim; ++i)
		p.vDimx[i] = n * p1.vDimx[i];
	return p;
}

