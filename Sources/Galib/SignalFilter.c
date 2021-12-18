/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SignalFilter.c
* 功能描述：信号滤波稳定模块，用于 ad，温度等信号不稳定的平滑处理
* 处理时间为 ：  COLLECT_PRIDE_TIME × COLLECT_TIMES （ms）
* 处理方式：   本次值 =  本次平均 × COLLECT_WEIGHT% +  上一次* (1-COLLECT_WEIGHT)
* 注意 ：初始化时第一次滤波需要500ms左右才能采集完成 
* 
* 本模块使用需要定时器模块
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原模块的1.0移植
* 原作者：xb
* 完成日期：2019年5月14日
* 
* 版本：1.1
* 说明：增加SIG_MAX_NUM 由10改为15
* 作者：xb
* 完成日期：2020年1月6日
* 
* 版本：1.2
* 说明：
* 1 增加05的滤波用来温度回差功能
* 2 优化-数时的四舍五入
* 作者：xb
* 完成日期：2020年12月4日
*/
#include "SignalFilter.h"



//信号滤波结构体定义
typedef struct
{
	int16  i16SigData[COLLECT_TIMES];      //缓存器
	int16  i16SigResult;                   //计算结果
	int16  i16Result05;
}SIGNALDATA;


static SIGNALDATA sSigData[SIG_MAX_NUM];
static uint8 u8TotalNum = SIG_MAX_NUM;

//底层驱动接口函数
extern int16 SigF_GetCurrentState(uint8 u8Num);
/*******************************************************************************
* Function:      BOOL SigF_Init(uint8 u8MaxNum)
*
* Description:   初始化，定义当前的总数
                 
* return :       TRUE : 成功， false 失败
* Note  :        初始化处理个数，且把结果i16SigResult =0

************************************************************************************/
BOOL SigF_Init(uint8 u8MaxNum)
{
	uint8 i;
	BOOL bInitOK;
	SIGNALDATA *pSigData;
	
	if (u8MaxNum <= SIG_MAX_NUM)
	{
		u8TotalNum = u8MaxNum;
		bInitOK = TRUE;
		
		pSigData = sSigData;
		for(i = 0; i < u8TotalNum; i++)
		{
			pSigData->i16SigResult = 0;
			pSigData++;
		}
	}	
	else
	{
		bInitOK = FALSE;
	}
			
	return bInitOK;
}

/*******************************************************************************
* Function:     int16 SigF_GetResult(uint8 u8SigNum)
*
* Description:   u8SigNum  信号通道
                 
* return :       得到当前信号值，已经滤波
* Note  :        

************************************************************************************/
int16 SigF_GetResult(uint8 u8SigNum)
{
	int16 i16Result;
	if (u8SigNum < SIG_MAX_NUM)
	{
		i16Result = sSigData[u8SigNum].i16SigResult;
	}
	else
	{
		i16Result = 0;
	}
	
	return i16Result;
}

/*******************************************************************************
* Function:     int16 SigF_GetResult(uint8 u8SigNum)
*
* Description:   u8SigNum  信号通道
                 
* return :       得到当前信号值，已经滤波
* Note  :        

************************************************************************************/
int16 SigF_GetResult05(uint8 u8SigNum)
{
	int16 i16Result;
	if (u8SigNum < SIG_MAX_NUM)
	{
		i16Result = sSigData[u8SigNum].i16Result05;
	}
	else
	{
		i16Result = 0;
	}
	
	return i16Result;
}

/*******************************************************************************
* Function:      static void SigF_CalcResult(uint8 u8SigNum)
*
* Description:  对第u8SigNum 信号进行滤波算法
                 i16SigResult = AVG(i16SigData) COLLECT_WEIGHT% +  i16SigResult* (1-COLLECT_WEIGHT)
* return :       none
* Note  :        

************************************************************************************/
static void SigF_CalcResult(uint8 u8SigNum)
{
	uint8 i;
	int32 i32AvgSig = 0;
	SIGNALDATA *pSigData = &sSigData[u8SigNum];
	
	for (i = 0; i < COLLECT_TIMES; i++)
	{
		i32AvgSig += pSigData->i16SigData[i];
	}
	
	i32AvgSig /= COLLECT_TIMES;
	
	i32AvgSig = i32AvgSig * COLLECT_WEIGHT + pSigData->i16SigResult * (100-COLLECT_WEIGHT);
	
	if (i32AvgSig % 100 >= 50)
	{//4 舍 5入
		i32AvgSig += 100;
	}
	else if (i32AvgSig % 100 <= -50)
	{
		i32AvgSig -= 100;
	}
	else{}

	pSigData->i16SigResult = (int16)(i32AvgSig / 100);
	if ( (pSigData->i16SigResult - pSigData->i16Result05) >= 4)
	{
		pSigData->i16Result05 = (pSigData->i16SigResult+1)/5;
		pSigData->i16Result05 = pSigData->i16Result05 *5;
	}
	else if ((pSigData->i16SigResult - pSigData->i16Result05) <= -4)
	{
		pSigData->i16Result05 = (pSigData->i16SigResult-1)/5;
		pSigData->i16Result05 = pSigData->i16Result05 *5;
	}
	else
	{//两者之间不变
		
	}
}
/*******************************************************************************
* Function:      void SigFControl(void)
*
* Description:    主控函数，放在loop中调用
                 
* return :       NONE
* Note  :        

************************************************************************************/
void SigFControl(void)
{
	static uint16 TimerCollect;
	static uint8  u8CollectTimes;
	SIGNALDATA *pSigData;
	uint8 i;
	
	if (TimeOutChkMsLong(&TimerCollect, COLLECT_PRIDE_TIME))
	{
		pSigData = sSigData;
		for (i = 0; i < u8TotalNum; i++)
		{
			pSigData->i16SigData[u8CollectTimes] = SigF_GetCurrentState(i);
			pSigData++;
		}
		
		u8CollectTimes++;
		if (u8CollectTimes >= COLLECT_TIMES)
		{
			u8CollectTimes = 0;
			for (i = 0; i < u8TotalNum; i++)
			{
				SigF_CalcResult(i);
			}	
		}
		else{}
	}
}
