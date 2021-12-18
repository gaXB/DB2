/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：PTMETER_Fun.h
* 功能描述：电位器处理头文件。
* 此模块能正常工作需要加入 需要定时器模块
*
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 *
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年6月17日
*
*/ 
#ifndef PTMETER_FUN_H_
#define PTMETER_FUN_H_

#include "DataType.h"


#define  PTMETER_HYSTERESIS_PER   5       //回差区间±百分比
#define  MAX_PTMETER_NUM          3       //最大个数

//电位器配置定义
typedef struct
{
	int16   i16StartAD;     //初始电压
	int16   i16EndAD;       //结束电压
	uint8   MaxLevel;       //最大档位值，如果5个挡位的，这个值应为4
}PTMETERCONFIG;



BOOL PTMETER_Init(uint8 u8Num, PTMETERCONFIG ConfigSet[]);
void PTMETER_Control(void);
uint8 PTMETER_GetLevel(uint8 u8PTId);
#endif /* PTMETER_FUN_H_ */
