/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：TempFilter.c
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
#include "TempFilter.h"


typedef struct
{
	int16 i16Data[TEMP_FILTER_TIMES];
	int16 i16Value;
	int16 i16LastValue;
}TEMPFILTERDATA;
static uint8 u8CollectTimes = 0;

TEMPFILTERDATA  sTempFilterData[TEMP_FILTER_NUM];

extern int16 GAMath_abs(int16 data);

/*******************************************************************************
 * Function:        uint8 TempFilterTask(int16* pTemp, uint8 num)
 *
 * Description:    PID 计算的温度滤波函数
 *
 * Returns:         0 错误， 0x55 完成， 1 未完成
 *
 * Notes:           温度稳定后再计算
 *
********************************************************************************/
uint8 TempFilterTask(int16* pTemp, uint8 num)
{
	uint8 u8Id, u8nNum;
	TEMPFILTERDATA* pTempFilter = sTempFilterData;
	int16 i16Sum;
	if (num > TEMP_FILTER_NUM)
	{
		return 0;
	}
	
	for (u8Id = 0; u8Id < num; u8Id++)
	{
		pTempFilter->i16Data[u8CollectTimes] = pTemp[u8Id];
		pTempFilter++;
	}
	
	u8CollectTimes++;
	if (u8CollectTimes >= TEMP_FILTER_TIMES)
	{
		u8CollectTimes = 0;
		pTempFilter = sTempFilterData;
		for (u8Id = 0; u8Id < num; u8Id++)
		{
			i16Sum = 0;
			for (u8nNum = 0; u8nNum < TEMP_FILTER_TIMES; u8nNum++)
			{
				i16Sum += pTempFilter->i16Data[u8nNum];
			}
			i16Sum /= TEMP_FILTER_TIMES;
			if ( GAMath_abs(i16Sum - pTempFilter->i16LastValue) <= TEMP_FILTER_DIFFVALUE)
			{//2度以内可以更新
				pTempFilter->i16Value = i16Sum;
				pTempFilter->i16LastValue = i16Sum;
				TempFilterCalBackValue(u8Id, i16Sum);
			}
			else
			{
				pTempFilter->i16LastValue = i16Sum;
			}
			pTempFilter++;
		}
		return 0x55;
	}
	return 1;
}
