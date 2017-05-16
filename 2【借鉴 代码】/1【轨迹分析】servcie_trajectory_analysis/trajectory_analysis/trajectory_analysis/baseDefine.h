/*
 * baseDefine.h
 *
 *  Created on: 2014-5-17
 *      Author: Cai
 */

#ifndef _H_BASEDEFINE_CAI_
#define _H_BASEDEFINE_CAI_

#define RECT_CENTERX(r) r.x+0.5*r.w
#define RECT_CENTERY(r) r.y+0.75*r.h

#define RECT_CENTERX_2(r) r.x+0.5*r.width
#define RECT_CENTERY_2(r) r.y+0.75*r.height

#define RECT_AREA(r) r.w * r.h

/*
#define MAX(x, y) x > y? x: y
#define MIN(x, y) x < y? x: y*/

//ÿ֡�켣
typedef struct coordInFrame{
	int x;//x����
	int y;//y����
	int w;
	int h;
	int frameNum;//�ڼ�֡
}COORDINFRAME;

#endif