/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SignalFilter.h
* 功能描述：信号滤波稳定模块头文件， 需要定时器模块
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原模块的1.0移植
* 原作者：xb
* 完成日期：2019年5月15日
* 
* 版本：1.1
* 说明：增加SIG_MAX_NUM 由10改为15
* 作者：xb
* 完成日期：2020年1月6日
* 
* 版本：1.2
* 说明：
* 1 增加05的滤波用来温度回差功能
* 2 优化-数时的四舍五入
* 作者：xb
* 完成日期：2020年12月4日
*/

#ifndef  SIGNAL_FILTER_H_
#define  SIGNAL_FILTER_H_

#include "timerout.h"

#define SIG_MAX_NUM            15        //最大支持数

#define COLLECT_PRIDE_TIME     25        //采集周期， ms
#define COLLECT_TIMES           8        //采集此处    总共时间为 COLLECT_PRIDE_TIME × COLLECT_TIMES
#define COLLECT_CYCTIME     (COLLECT_PRIDE_TIME * COLLECT_TIMES)
#define COLLECT_WEIGHT         65       //本次的权重 %
/*********************不更改变量分界************************/


void SigFControl(void);
BOOL SigF_Init(uint8 u8MaxNum);
int16 SigF_GetResult(uint8 u8SigNum);
int16 SigF_GetResult05(uint8 u8SigNum);
#endif
