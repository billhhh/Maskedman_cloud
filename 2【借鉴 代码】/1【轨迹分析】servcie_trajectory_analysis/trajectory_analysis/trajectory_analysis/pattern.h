/*
 * pattern.h
 *
 *  Created on: 2014-3-29
 *      Author: Cai
 */

#ifndef _H_PATTERN_CAI_
#define _H_PATTERN_CAI_
#include <cmath>
#include <vector>

/*
 *  class name: Pattern
 *  description:    模式结构
 *      类中重载定义了模式运算符，使ISODATA算法的书写更简洁。
 *      还定义了静态函数Distance()求模式间的距离。
 */

class Pattern{
public:
	int n;
	static int dim;
	std::vector<double> vDimx;

	Pattern();
	Pattern(int i, int d, double x[] = NULL);
	virtual ~Pattern(){};
	void reDefine(int d);

	static double Distance( const Pattern x1, const Pattern x2);

	Pattern & operator = (const Pattern &p);
	Pattern & operator += (const Pattern &p);
	Pattern & operator /= (const int n);
	friend Pattern operator + (const Pattern &p1, const Pattern &p2);
	friend Pattern operator * (const int &n, const Pattern &p);
};


#endif

