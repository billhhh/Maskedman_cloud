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
 *  description:    ���࣬����ʵ�ּ���ĳһ������������ϲ����
 *		�������Է������һ����װ��
 */

class Sort{
public:
	int dim;						//����ά��	
	int amount;					    //����ģʽ��Ŀ	
	Pattern center;					//������
	std::vector<Pattern> vPattern;  //ģʽ����
	std::vector<double> vSigma;		//x������y�����ı�׼��
	std::vector<double> vDist;      //ģʽ�����ľ���
	double avgDist;					//ģʽ�����ĵ�ƽ������
	int maxIndex;					//����׼������
	double maxSigma;				//����׼��

public:
	Sort();
	Sort(int d);
	~Sort();

	int insert(const Pattern &p); //��p��Ϊ����
	Pattern calCenter();		  //����������
	double calAvgDist();		  //����ģʽ�����ĵ�ƽ������
	int calSigma();				  //�������ھ���ı�׼��ʸ��
	void removeExtra();            //�޳������Ե��
	//bool combin(Sort &k);		  //�������δ�ϲ���ʱ���ϲ�
};

#endif