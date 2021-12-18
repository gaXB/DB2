/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：TempFilter.h
* 功能描述：温度滤波 用在HMIXPID 模块中，不用更改
* 
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年4月2日
* 
*/
#ifndef TEMPFILTER_H_
#define TEMPFILTER_H_


#include "DataType.h"
#define  TEMP_FILTER_TIMES    5
#define  TEMP_FILTER_NUM      2
#define  TEMP_FILTER_DIFFVALUE  20

extern void TempFilterCalBackValue(uint8 u8Num, int16 i16Value);  //回掉函数通知应用程序更新
uint8 TempFilterTask(int16* pTemp, uint8 num);
#endif /* TEMPFILTER_H_ */
