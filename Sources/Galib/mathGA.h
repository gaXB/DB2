/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：mathGA.h
* 功能描述：数学模块的头文件。
*
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 *
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年4月25日
*
* 版本：1.1
* 说明：
* 1 优化求段函数
* GAMath_CalDataSegU8
* GAMath_CalDataSegI16
* 
* 2 增加求分段函数
*  MathG_GetSegLinei16
* 作者：xb
* 完成日期：2020年1月6日
*/
#ifndef MATH_GA_
#define MATH_GA_
#include "DataType.h"


int16 GAMath_LineFuni16(int16 x1, int16 x2, int16 y1, int16 y2, int16 x);
uint8 GAMath_CalDataSegU8(uint8 u8Cvalue, uint8* TBL_Data, uint8 u8MaxTBLNum, uint8 u8Inverted);
uint8 GAMath_CalDataSegI16(int16 i16Cvalue, int16* TBL_Data, uint8 u8MaxTBLNum, uint8 u8Inverted);
int16 GAMath_abs(int16 data);
uint8 GAMath_CalTBLu8Level(uint8 u8CurrentLevel, uint8 u8Value, uint8* TBL_Value, uint8 u8MaxLevel);
uint8 GAMath_CalTBLi16Level(uint8 u8CurrentLevel, int16 i16Value, int16* TBL_Value, uint8 u8MaxLevel);
int16 MathG_GetSegLinei16(int16 Cvalue,int16* DataX, int16 *Datay, uint8 Num);
#endif
