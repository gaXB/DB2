/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�RunPID.c
* ����������PID �㷨��
* 1 ����ʼֵ�� PID ÿһ��ķ�Χ
* 2 �ʺ��¶ȿ��Ƶ�ʹ��
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��5��20��
* 
*/

#include "RunPID.h"


BOOL Deal_PIDFun(PIDDATA *pPData, PIDPARAMETER *pParameter)
{
	int16 i16dTdt_I;
	int32 i32lData, i32DValue;
	if (TimeOutChkTenthSLong(&pPData->Timer01, pParameter->u16Period))
	{
		//��������pid�߼�
		//i16AM_FF0 ���ϲ��߼�ʵ��
		pParameter->pGetRunState();
		pPData->TenthSTimes++;

		if (pPData->bRunEn)
		{
			if (pPData->u8RunState == PIDRUN_STATE_CLOSE)
			{//��һ�ν���
				pPData->u8RunState = PIDRUN_STATE_INIT;
				pPData->TenthSTimes = 1;
				pPData->i16FilterValue = -32768; //�����м���
				pPData->i32GetSum = 0;
			}
			else
			{
				if (pPData->TenthSTimes %10 == 0)
				{//ÿ1s�ɼ�һ��
					pPData->i32GetSum += pPData->i16Get;
					if (pPData->TenthSTimes >= 300)
					{ //30s
						pPData->TenthSTimes = 0;
						pPData->i16FilterValue_1 = pPData->i16FilterValue;
						pPData->i16FilterValue =(int16)(pPData->i32GetSum / 30);
						if (pPData->i16FilterValue_1 != -32768)
						{
							if (GAMath_abs(pPData->i16FilterValue_1 - pPData->i16FilterValue) > pPData->i16OutInitValue)
							{//����INIT
								pPData->u8RunState = PIDRUN_STATE_RUN;
							}
							else{}
						}
						else{}
					}
					else//ʱ��δ��
					{

					}
				}
				else//ʱ��δ��
				{

				}
			}//else end
		}
		else
		{
			pPData->u8RunState = PIDRUN_STATE_CLOSE;
		}
		if (pPData->u8RunState == PIDRUN_STATE_CLOSE)
		{
			pPData->i16AM_Hfs = 0;
			pPData->int16AM_I = 0;
			pPData->bInit = 0;
		}
		else if (pPData->u8RunState == PIDRUN_STATE_INIT)
		{//û��ʼ��ʹ�ó�ʼֵ
			pPData->i16AM_Hfs = pPData->i16AM_FF0;
			pPData->int16AM_I = 0;
			pPData->bInit = 0;
		}
		else if (pPData->u8RunState == PIDRUN_STATE_RUN)
		{
			if (pPData->bInit == 0)
			{
				pPData->bInit = 1;
				pPData->i16AM_Hfs = pPData->i16AM_FF0;
			}
			else{}
			//pPData->i16Set i16Get ���ϲ��߼����� 
			pPData->i16Diff = pPData->i16Set - pPData->i16Get;
			
			//������
			i32lData = (int32)pParameter->i16Kp * pPData->i16Diff/10;
			
			if (i32lData > pParameter->i16PMax) 
			{
				pPData->int16AM_P = pParameter->i16PMax;
			}
			else if (i32lData < -pParameter->i16PMax)
			{
				pPData->int16AM_P = -(pParameter->i16PMax);
			}
			else
			{
				pPData->int16AM_P = (int16)i32lData;
			}
			//������
			i16dTdt_I = pPData->i16Diff;
			
			//I de zhi��Сֵ�����ֵ�ж�
			if (i16dTdt_I >= -(pParameter->i16DI_Min) && i16dTdt_I <= pParameter->i16DI_Min)
			{
				i16dTdt_I = 0;
			}
			else if (i16dTdt_I >= pParameter->i16DI_Max)
			{
				i16dTdt_I = pParameter->i16DI_Max;
			}
			else if (i16dTdt_I <= -(pParameter->i16DI_Max))
			{
				i16dTdt_I = -(pParameter->i16DI_Max);
			}
			else{}
			
			if (pPData->i16AM_Hfs >= pParameter->i16ResMax) 
			{
				if (i16dTdt_I > 0)
					i16dTdt_I = 0;
			}
			else if (pPData->i16AM_Hfs <= pParameter->i16ResMin) 
			{
				if (i16dTdt_I < 0)
					i16dTdt_I = 0;
			}else{} 
			
			//AM_I=AM_I + T3* dTdt_I;
			pPData->int16AM_I = pPData->int16AM_I + pParameter->i16Ki * i16dTdt_I/10;
			
			if (pPData->int16AM_I >= pParameter->i16IMax)
			{
				pPData->int16AM_I = pParameter->i16IMax;
			}
			else if (pPData->int16AM_I <= -(pParameter->i16IMax))
			{
				pPData->int16AM_I = -(pParameter->i16IMax);
			}
			else{}
			
			//������
			i32DValue = pParameter->i16Kd * (pPData->i16Diff - pPData->i16Diff_1);
			if (i32DValue >= pParameter->i16DMax)
			{
				i32DValue = pParameter->i16DMax;
			}
			else if (i32DValue <= -(pParameter->i16DMax))
			{
				i32DValue = -(pParameter->i16DMax);
			}
			else{}
			pPData->int16AM_D = (int16)i32DValue;	
					
			//pid 
			pPData->i16AM_Hfs = pPData->i16AM_FF0 + pPData->int16AM_P + pPData->int16AM_I/10 + pPData->int16AM_D/10;

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
		}
		else
		{//PIDRUN_STATE_STOP ����
			
		}
		
		pPData->i16Diff_1 = pPData->i16Diff;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
