/*
 * LINEXV.h
 *
 *  Created on: 2021Äê12ÔÂ29ÈÕ
 *      Author: Administrator
 */

#ifndef MODULE_LINEXV_H_
#define MODULE_LINEXV_H_

#include "DataType.h"

typedef struct
{
	uint8 InitReq;
	uint8 bEnable;
	uint16 u16ComPostion;
	uint16 u16SetValue;
	uint16 u16RetPostion;
	uint8 InitRet;
	uint16 TimerRec;
	uint16 TimerControl;
	uint8 bLostErr;
	uint8 u8State;
}EXVDATA;

#define   EXV_NUM_EXV      0
#define   EXV_NUM_BEXV      1
#define   EXV_NUM_THREEWAY1     2
#define   EXV_NUM_THREEWAY2     3
#define   EXV_NUM_FOURWAY       4
#define   EXV_NUM_AGS_A         5
#define   EXV_NUM_AGS_B         6
#define   EXV_NUM_BEXV2         7
#define   EXVNUM                8


void EXVControl(void);
void EXVSetPostion(uint8 num, uint16 postion);
uint8 EXVGetLostErr(uint8 num);
#endif /* MODULE_LINEXV_H_ */
