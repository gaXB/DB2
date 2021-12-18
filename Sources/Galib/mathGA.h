/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�mathGA.h
* ������������ѧģ���ͷ�ļ���
*
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 *
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��4��25��
*
* �汾��1.1
* ˵����
* 1 �Ż���κ���
* GAMath_CalDataSegU8
* GAMath_CalDataSegI16
* 
* 2 ������ֶκ���
*  MathG_GetSegLinei16
* ���ߣ�xb
* ������ڣ�2020��1��6��
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
