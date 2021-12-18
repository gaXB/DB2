/************************文件说明*****************************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Signal_TempGet.h
* 功能描述：温度信号数据得到的辅助功能
*
* 编译器： codewarrior IDE 11.1
* MCU说明： 无
*/
/**************************更改记录******************************************************
×
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
* 
* * * 版本：2.0
* 说明：跟随.c
* 作者：xb
* 完成日期：2020年7月6日
* 
*/
#ifndef SIGNAL_TempGet_H_
#define SIGNAL_TempGet_H_
#include "ADTreat.h"

typedef struct
{
	const uint16 *tbl_tad;       //表
	uint8 u8TBLLength;           //表长
	ADTempType atOffsetTemp;     //最低温度
	ADTempType atAccurate;       //精度
}ADTGETTYPE;


ADTempType Signal_GetAdT (uint16 adcal, ADTGETTYPE *pADTemp);
uint16 Signal_GetAd_Press(uint8 hrol,uint16 adcal);
#endif /* SIGNAL_REFRESH_H_ */
