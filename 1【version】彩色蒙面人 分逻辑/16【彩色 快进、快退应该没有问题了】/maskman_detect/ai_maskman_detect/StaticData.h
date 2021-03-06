/*
 * StaticData.h
 *
 * funtion: record some const data that never change ,
 *          in this project, they are cascades' names
 *
 *    Author: Bill Wang
 */

#ifndef _STATICDATA_H_
#define _STATICDATA_H_

#include <iostream>
#include <string>

using namespace std;

//need get the instance first and then access the static const class parameters
class StaticData
{
public:
	const static string cascadePath;
	const static string nestedCascadePath;
	const static string thirdCascadePath;
	const static string forthCascadePath;

};

//�������������
const string StaticData::cascadePath = "./haarcascades/cascades.xml";  //��һ�㣺��ͷ���
const string StaticData::nestedCascadePath = "./haarcascades/haarcascade_frontalface_alt2.xml"; //�������
const string StaticData::thirdCascadePath = "./haarcascades/haarcascade_mcs_nose.xml";  //���Ӽ��
const string StaticData::forthCascadePath = "./haarcascades/haarcascade_mcs_mouth.xml";  //��ͼ��


#endif