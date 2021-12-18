/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：RunPID.h
* 功能描述：PID 算法头文件，需要底层驱动文件
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年5月20日
* 
*/
#ifndef RUN_PID_H_
#define RUN_PID_H_

#include "LibDriveDefine.h"


typedef void (*PIDGetInit)(void);
typedef struct
{
	uint16 u16Period;   //0.1s
	int16 i16Kp;        // 1
	int16 i16Ki;        // /10
	int16 i16Kd;        // *1
	int16 i16PMax;      //*1
	int16 i16IMax;      //*0.1
	int16 i16DMax;      //*0.1
	int16 i16DI_Min;   //d i 的min  /10
	int16 i16DI_Max;   //d i 的max  /10	
	int16 i16ResMin;   //结果最小值
	int16 i16ResMax;   //结果最大值
	PIDGetInit pGetRunState;
}PIDPARAMETER;

typedef struct
{
	int16 i16Set;        //设置值  由上层逻辑实现
	int16 i16Get;        //实际值 由上层逻辑实现
	int16 i16Diff;       //差值
	int16 i16Diff_1;     //差值-1
	int16 i16Diff_2;     //差值-2
	int16 i16AM_FF0;     // 初始值 由上层逻辑实现
	int16 int16AM_P;     // AM_P 
	int16 int16AM_I;     // AM_I
	int16 int16AM_D;     // AM_I
	int16 i16AM_Hfs;     // AM_Hfs
	uint16 Timer01;
	uint16 TenthSTimes;
   int32 i32GetSum;
   int16 i16FilterValue;
   int16 i16FilterValue_1;
   int16 i16OutInitValue;
	uint8 u8RunState;       //0 关闭， 1使用初始值， 其他 运行pid,由上层逻辑实现
	uint8 bInit;        //内部控制变量，应用层不需要注意
	uint8 bRunEn;

}PIDDATA;


#define    PIDRUN_STATE_CLOSE       3
#define    PIDRUN_STATE_INIT         1
#define    PIDRUN_STATE_RUN         2
#define    PIDRUN_STATE_STOP        0

BOOL Deal_PIDFun(PIDDATA *pPData, PIDPARAMETER *pParameter);
#endif /* RUN_PID_EAC_H_*/
