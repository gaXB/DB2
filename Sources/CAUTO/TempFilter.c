/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�TempFilter.c
* �����������¶��˲� ����HMIXPID ģ���У����ø���
* 
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��4��2��
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
 * Description:    PID ������¶��˲�����
 *
 * Returns:         0 ���� 0x55 ��ɣ� 1 δ���
 *
 * Notes:           �¶��ȶ����ټ���
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
			{//2�����ڿ��Ը���
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
