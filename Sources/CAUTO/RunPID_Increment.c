/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�RunPID_Increment.c
* ���������� ����ʽPID �㷨�����ø���
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��11��21��
* 
*/
#include "RunPID_Increment.h"


int Deal_PIDFun_INC(PIDDATA_INC *pPData, PIDPARAMETER_INC *pParameter)
{
	int32 i32lData;
	if (TimeOutChkTenthSLong(&pPData->Timer01, pParameter->u16Period))
	{
		//��������pid�߼�
		//i16AM_FF0 ���ϲ��߼�ʵ��
		pParameter->pGetRunState();
		//pPData->i16Set i16Get ���ϲ��߼����� 
		pPData->i16Diff =pPData->i16Get - pPData->i16Set;
		
		//pid *10
		i32lData = (int32)pParameter->i16Kp * (pPData->i16Diff - pPData->i16Diff_1) +
				(int32)pParameter->i16Ki * pPData->i16Diff + 
				(int32)pParameter->i16Kd * (pPData->i16Diff - 2*pPData->i16Diff_1 + pPData->i16Diff_2);
		
		pPData->i16AM_Hfs = (int16)(i32lData/10);

		//���жϷŵ����
		if (pPData->i16AM_Hfs >= pParameter->i16ResMax)
		{
			pPData->i16AM_Hfs = pParameter->i16ResMax;
		}
		else if (pPData->i16AM_Hfs <= pParameter->i16ResMin)
		{
			pPData->i16AM_Hfs = pParameter->i16ResMin;
		}
		else{}
		
		pPData->i16Diff_1 = pPData->i16Diff;
		return pPData->i16AM_Hfs;
	}
	else
	{
		return 0;
	}
}
