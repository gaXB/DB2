/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：RunPID_Increment.c
* 功能描述： 增量式PID 算法。不用更改
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年11月21日
* 
*/
#include "RunPID_Increment.h"


int Deal_PIDFun_INC(PIDDATA_INC *pPData, PIDPARAMETER_INC *pParameter)
{
	int32 i32lData;
	if (TimeOutChkTenthSLong(&pPData->Timer01, pParameter->u16Period))
	{
		//处理下面pid逻辑
		//i16AM_FF0 由上层逻辑实现
		pParameter->pGetRunState();
		//pPData->i16Set i16Get 由上层逻辑设置 
		pPData->i16Diff =pPData->i16Get - pPData->i16Set;
		
		//pid *10
		i32lData = (int32)pParameter->i16Kp * (pPData->i16Diff - pPData->i16Diff_1) +
				(int32)pParameter->i16Ki * pPData->i16Diff + 
				(int32)pParameter->i16Kd * (pPData->i16Diff - 2*pPData->i16Diff_1 + pPData->i16Diff_2);
		
		pPData->i16AM_Hfs = (int16)(i32lData/10);

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
		
		pPData->i16Diff_1 = pPData->i16Diff;
		return pPData->i16AM_Hfs;
	}
	else
	{
		return 0;
	}
}
