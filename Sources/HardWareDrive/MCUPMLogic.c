/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：MCUPMLogic.c
* 功能描述：MCU 掉电逻辑。
* 1  可计算MCU 点火次数，复位次数，点火保持时复位次数计算，
* 2  判断MCU 上次下电时是否满足要求
* 3  判断MCU 是否全掉电
* 4  记录复位寄存器的值
* 编译器： codewarrior IDE 11.1
* MCU说明：跟MCU无关
* 更改说明: 不能更改
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年5月13日
* 
* 版本：1.1
* 说明：支持renesas
* 原作者：xb
* 完成日期：2021年8月21日
*/

#include "DataType.h"
#include "MCUPMLogic.h"


#if  NOINIT_TYPE ==  NOINIT_TYPE_KEA
__attribute__ ((section(".mcudata"))) uint32 u32MCUData[7];
__attribute__ ((section(".mcudata_rtimes"))) uint32 u32PowerOnRestTimes;
__attribute__ ((section(".noinit"))) uint32  u32RestCheckSum;  //用来判断是否是全掉电
#elif NOINIT_TYPE == NOINIT_TYPE_RL78
#pragma section bss MCUDATA
uint32 u32MCUData[7];

#pragma section bss MCUDATA_RTIMES
uint32 u32PowerOnRestTimes;

#pragma section bss NOINIT
uint32  u32RestCheckSum;  //用来判断是否是全掉电

#pragma section 
#define __GOLBAL_VAR_
#endif


uint32 u32lResetData;
BOOL bMCULastOffOK;

//外部函数声明
extern void Save_MCUDATA(void);
extern BOOL Load_MCUDATA(void);

#define   MCUDATA_FLG_INIT       0x0
#define   MCUDATA_FLG_IGNWAKE    0x01
#define   MCUDATA_FLG_SAVEOK     0x02
/*******************************************************************************
* Function:      void MCU_SetReset(uint32 u32ResonRegister)
*
* Description:   设置复位寄存器 u32ResonRegister
*                 
* return :       none
* Note  :        上电调用一次

************************************************************************************/
void MCUPM_SetReset(uint32 u32ResonRegister)
{
	u32lResetData = u32ResonRegister;

	if (u32RestCheckSum == 0x55aa55aa)
	{
		u32PowerOnRestTimes++; 
	}
	else
	{
		u32RestCheckSum = 0x55aa55aa;
		u32PowerOnRestTimes = 0;
	}
}
/*******************************************************************************
* Function:      void MCU_IgnOn(void)
*
* Description:   点火由关变ON 时调用一次
*                 
* return :        none
* Note  :         上电调用一次

************************************************************************************/
void MCUPM_IgnOn(void)
{
	u32MCUData[MCUDATA_NUM_IGNCOUNT]++;
	
	if ( (u32MCUData[MCUDATA_NUM_FLG] & 0xff) != MCUDATA_FLG_IGNWAKE)
	{
		u32MCUData[MCUDATA_NUM_IGNWAKE]++;
	}
	else
	{
		
	}
	u32MCUData[MCUDATA_NUM_FLG] &= 0xFFFFFF00;
	u32MCUData[MCUDATA_NUM_FLG] |= MCUDATA_FLG_IGNWAKE;  
	
	//printf("bMCULastOffOK = %d, u32PowerOnRestTimes = %d, ResetTimes = %d \n" ,bMCULastOffOK, u32PowerOnRestTimes, u32MCUData[MCUDATA_NUM_RESETCOUNT]);
	//printf("IGNTimes = %d \n", u32MCUData[MCUDATA_NUM_IGNCOUNT]);
	Save_MCUDATA();
}
/*******************************************************************************
* Function:      void MCUPM_Over(void)
*
* Description:    休眠时调用一次
*                 
* return :        none
* Note  :         等待MCU 复位

************************************************************************************/
void MCUPM_Over(void)
{
	if ( (u32MCUData[MCUDATA_NUM_FLG] & 0xff) != MCUDATA_FLG_SAVEOK)
	{
		u32MCUData[MCUDATA_NUM_FLG] &= 0xFFFFFF00;
		u32MCUData[MCUDATA_NUM_FLG] |= MCUDATA_FLG_SAVEOK;  
		Save_MCUDATA();
	}
	else
	{
		
	}
}

/*******************************************************************************
* Function:      void MCUPM_ReadEE(void)
*
* Description:   上电 EE 初始化完成时， 读取参数值
*                 
* return :        none
* Note  :         

************************************************************************************/
void MCUPM_ReadEE(void)
{
	if (Load_MCUDATA())
	{
		u32MCUData[MCUDATA_NUM_REST_1] = u32MCUData[MCUDATA_NUM_REST];
	   u32MCUData[MCUDATA_NUM_REST] = u32lResetData;
		u32MCUData[MCUDATA_NUM_RESETCOUNT]++;
	}
	else
	{
		u32MCUData[MCUDATA_NUM_RESETCOUNT] = 0;
		u32MCUData[MCUDATA_NUM_IGNCOUNT] = 0;
		u32MCUData[MCUDATA_NUM_IGNWAKE] = 0;
		u32MCUData[MCUDATA_NUM_FLG] = 0;
		u32MCUData[MCUDATA_NUM_REST_1] = u32lResetData;
	   u32MCUData[MCUDATA_NUM_REST] = u32lResetData;
	}		
	
	u32MCUData[MCUDATA_NUM_FLG] &= 0xFFFF00FF;
	if ((u32MCUData[MCUDATA_NUM_FLG] & 0xff) == MCUDATA_FLG_SAVEOK)
	{//上次下电时记忆正确
		u32MCUData[MCUDATA_NUM_FLG] |= 0x0100;
		bMCULastOffOK = TRUE;
	}
	else
	{
		//u32MCUData[MCUDATA_NUM_FLG] |= 0x0000;
		u32MCUData[MCUDATA_NUM_IGNCOUNT] = 0;
		u32MCUData[MCUDATA_NUM_IGNWAKE] = 0;
		bMCULastOffOK = FALSE;
	}	
	u32MCUData[MCUDATA_NUM_FLG] &= 0xFFFFFF00;
	//为了避免上电进行存储，此处不进行记忆
}


/*******************************************************************************
 * Function:    BOOL MCUPM_IsLPowerRest(void)
 *
 * Description: 判断MCU 是否低电压 复位
 *
 * Returns:     1： 是低电压导致的复位  0： 不是
 *
 * Notes:     	
 *
*******************************************************************************/
BOOL MCUPM_IsLPowerRest(void)
{
	return (u32PowerOnRestTimes == 0? TRUE:FALSE);
}

/*******************************************************************************
 * Function:    uint32 MCUPM_PowerOnResetTimes(void)
 *
 * Description: 电源正常时的Reset次数
 *
 * Returns:     电源正常时的Reset次数
 *
 * Notes:     none	
 *
*******************************************************************************/
uint32 MCUPM_PowerOnResetTimes(void)
{
	return u32PowerOnRestTimes;
}
/*******************************************************************************
 * Function:    BOOL MCUPM_IsIgnONOK(void)
 *
 * Description: 判断上次断电时记忆是否正确
 *
 * Returns:     1： 正确   0： 不是
 *
 * Notes:     	
 *
*******************************************************************************/
BOOL MCUPM_IsLastIgnOFFNormal(void)
{
	return bMCULastOffOK; 
}


/*******************************************************************************
 * Function:    BOOL MCUPM_IgnCountReset(uint32 u32times)
 *
 * Description: 当IgnCount 》 u32times时 清0，并返回TRUE
 *
 * Returns:     1： 到达u32times 0：未到
 *
 * Notes:     	
 *
*******************************************************************************/
BOOL MCUPM_IgnCountReset(uint32 u32times)
{
	BOOL IsReset;
	if (u32MCUData[MCUDATA_NUM_IGNCOUNT] >= u32times)
	{
		u32MCUData[MCUDATA_NUM_IGNCOUNT] = 0;
		IsReset = TRUE;
	}
	else
	{
		IsReset = FALSE;
	}
	
	return IsReset;
}
