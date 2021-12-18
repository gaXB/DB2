/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：RunPID.c
* 功能描述：PID 算法。
* 1 带初始值。 PID 每一项的范围
* 2 适合温度控制的使用
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年5月20日
* 
*/

#include "RunPID.h"


BOOL Deal_PIDFun(PIDDATA *pPData, PIDPARAMETER *pParameter)
{
	int16 i16dTdt_I;
	int32 i32lData, i32DValue;
	if (TimeOutChkTenthSLong(&pPData->Timer01, pParameter->u16Period))
	{
		//处理下面pid逻辑
		//i16AM_FF0 由上层逻辑实现
		pParameter->pGetRunState();
		pPData->TenthSTimes++;

		if (pPData->bRunEn)
		{
			if (pPData->u8RunState == PIDRUN_STATE_CLOSE)
			{//第一次进入
				pPData->u8RunState = PIDRUN_STATE_INIT;
				pPData->TenthSTimes = 1;
				pPData->i16FilterValue = -32768; //不进行计算
				pPData->i32GetSum = 0;
			}
			else
			{
				if (pPData->TenthSTimes %10 == 0)
				{//每1s采集一次
					pPData->i32GetSum += pPData->i16Get;
					if (pPData->TenthSTimes >= 300)
					{ //30s
						pPData->TenthSTimes = 0;
						pPData->i16FilterValue_1 = pPData->i16FilterValue;
						pPData->i16FilterValue =(int16)(pPData->i32GetSum / 30);
						if (pPData->i16FilterValue_1 != -32768)
						{
							if (GAMath_abs(pPData->i16FilterValue_1 - pPData->i16FilterValue) > pPData->i16OutInitValue)
							{//脱离INIT
								pPData->u8RunState = PIDRUN_STATE_RUN;
							}
							else{}
						}
						else{}
					}
					else//时间未到
					{

					}
				}
				else//时间未到
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
		{//没开始则使用初始值
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
			//pPData->i16Set i16Get 由上层逻辑设置 
			pPData->i16Diff = pPData->i16Set - pPData->i16Get;
			
			//比例项
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
			//积分项
			i16dTdt_I = pPData->i16Diff;
			
			//I de zhi最小值和最大值判断
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
			
			//积分项
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

			//此判断放到最后
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
		{//PIDRUN_STATE_STOP 不变
			
		}
		
		pPData->i16Diff_1 = pPData->i16Diff;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
