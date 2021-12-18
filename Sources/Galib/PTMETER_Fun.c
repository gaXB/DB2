/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：PTMETER_Fun.c
* 功能描述：电位器处理模块，通过当前电位器的AD得到电位器的电压
* 
*
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 *
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年6月17日
*
*/ 
#include "PTMETER_Fun.h"
#include "MathGa.h"
#include "timerout.h"

static PTMETERCONFIG PTConfig[MAX_PTMETER_NUM];   //配置变量

uint8 u8PTLevel[MAX_PTMETER_NUM];  //计算的电位器党档位值
uint8 u8PTTotalNum; //最大个数


//外部函数调用
extern uint16 PTMETERHW_Read(uint8 num);
/*******************************************************************************
 * Function:    BOOL PTMETER_Init(uint8 u8Num, PTMETERCONFIG ConfigSet[])
 *
 * Description:  电位器初始化
 *
 * Returns:     是否成功
 *
 * Notes:      
*******************************************************************************/
BOOL PTMETER_Init(uint8 u8Num, PTMETERCONFIG ConfigSet[])
{
	uint8 i;
	BOOL ptInitRes;

	if (u8Num <= MAX_PTMETER_NUM)
	{
		ptInitRes = TRUE;
		
		for (i = 0; i< u8Num; i++)
		{
			PTConfig[i] = ConfigSet[i];
		}
		u8PTTotalNum = u8Num;
	}
	else
	{
		ptInitRes = FALSE;
	}
	
	return ptInitRes;
}
/*******************************************************************************
 * Function:    void PTMETER_Control(void)
 *
 * Description:  主控函数
 *
 * Returns:     none 
 *
 * Notes:      loop 调用
*******************************************************************************/
void PTMETER_Control(void)
{
	static uint16 TimerColt;
	uint8 u8Id;
	uint16 u16AD;
	int16 i16lData;
	uint8 u8Level;
	int16 i16HystPer;
	PTMETERCONFIG* pConfig;
	pConfig = PTConfig;
	
	if (TimeOutChkMsLong(&TimerColt, 10))
	{
		for (u8Id = 0; u8Id < u8PTTotalNum; u8Id++)
		{
			u16AD = PTMETERHW_Read(u8Id);
			
			i16lData = GAMath_LineFuni16(pConfig->i16StartAD, pConfig->i16EndAD, 0, pConfig->MaxLevel*100, u16AD);
			if (i16lData <= 0)
			{
				u8Level = 0;
			}
			else
			{
				u8Level = (uint8)(i16lData/100); 
				i16lData = i16lData%100;
				if (pConfig->MaxLevel >= 19)
				{
					i16HystPer = 0;
				}
				else
				{
					i16HystPer = PTMETER_HYSTERESIS_PER;
				}
				if (i16lData > (50+i16HystPer))
				{
					u8Level++;
				}
				else if (i16lData < (50-i16HystPer))
				{
					
				}
				else
				{
					if (u8PTLevel[u8Id] > u8Level)
					{//当前档位大于
						u8Level++;
					}
					else
					{
					}
				}
				
				if (u8Level >= pConfig->MaxLevel)
				{
					u8Level = pConfig->MaxLevel;
				}
				else{}
			}//calc level
			u8PTLevel[u8Id] = u8Level;
			pConfig++;
		}//for
	}//time
	else
	{
		
	}
}

/*******************************************************************************
 * Function:      uint8 PTMETER_GetLevel(uint8 u8PTId)
 *
 * Description:   得到当前的id电位器的档位值
 *
 * Returns:     none 
 *
 * Notes:       应用调用
*******************************************************************************/
uint8 PTMETER_GetLevel(uint8 u8PTId)
{
	return u8PTLevel[u8PTId];
}
