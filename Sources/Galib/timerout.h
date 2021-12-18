 /************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：timerout.h
* 功能描述： 定时器模块的头文件。
* 
* 
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：与MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年4月25日
* 
* 版本：1.1
* 说明：增加uint16 TimeOut_GetMsData(void)
* 作者：xb
* 完成日期：2020年1月6日
*/
#ifndef TIMER_OUT_H_
#define TIMER_OUT_H_
#include "DataType.h"

//可更改参数
#define  TIMEOUT_CPU_8BIT      0      //8 位 MCU 则为1， 则需要增加暂停，恢复中断函数，16位为0不需要
#define  LOOP_MEASURE_TIME   3000     //测试LOOP的周期

/***************不可改分界*****************************/
//主控函数调用
void MsCounterUpadat(void);
void TenthSCounterUpadat(void);
void SCounterUpadat(void);
//应用层可调用函数
BOOL TimeOutChkMsLong(uint16 *timer, uint16 timeOutVal);
BOOL TimeOutChkTenthSLong(uint16 *timer, uint16 timeOutVal);
BOOL TimeOutChkSLong(uint16 *timer, uint16 timeOutVal);
BOOL TimeOutChkTMsLong(uint16 *timer, uint16 timeOutVal);
uint32 TimeOut_TestLoop(void);
uint16 TimeOut_GetMaxLoop(void);
uint16 TimeOut_GetMsData(void);
#endif

