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

};

//各层分类器数据
// const string StaticData::cascadePath = "./data/service_classifier/haarcascades/cascades.xml";  //第一层：人头检测
// const string StaticData::nestedCascadePath = "./data/service_classifier/haarcascades/haarcascade_frontalface_alt2.xml"; //正脸检测

const string StaticData::cascadePath = "E:/Project/service_maskman_recongize/QtMaskManRecongize/Win32/service_classifier/haarcascades/cascades.xml";  //第一层：人头检测
const string StaticData::nestedCascadePath = "E:/Project/service_maskman_recongize/QtMaskManRecongize/Win32/service_classifier/haarcascades/haarcascade_frontalface_alt2.xml"; //正脸检测

#endif