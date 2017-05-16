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
 *  description:    ģʽ�ṹ
 *      �������ض�����ģʽ�������ʹISODATA�㷨����д����ࡣ
 *      �������˾�̬����Distance()��ģʽ��ľ��롣
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

