/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：RunPID_Increment.h
* 功能描述： 增量式PID 算法。不用更改
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.1
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年11月21日
* 
*/
#ifndef RUN_PID_EAC_H_
#define RUN_PID_EAC_H_

#include "LibDriveDefine.h"


typedef void (*PIDGetInit)(void);

typedef struct
{
	uint16 u16Period;   //0.1s
	int16 i16Kp;        // 1
	int16 i16Ki;        // 1
	int16 i16Kd;        // *1
	int16 i16ResMin;   //结果最小值
	int16 i16ResMax;   //结果最大值
	PIDGetInit pGetRunState;
}PIDPARAMETER_INC;

typedef struct
{
	int16 i16Set;        //设置值  由上层逻辑实现
	int16 i16Get;        //实际值 由上层逻辑实现
	int16 i16Diff;       //差值
	int16 i16Diff_1;     //差值-1
	int16 i16Diff_2;     //差值-2
	int16 i16AM_Hfs;     // AM_Hfs
	uint16 Timer01;
}PIDDATA_INC;



int Deal_PIDFun_INC(PIDDATA_INC *pPData, PIDPARAMETER_INC *pParameter);
#endif /* RUN_PID_EAC_H_*/
