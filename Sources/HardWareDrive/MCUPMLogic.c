/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�MCUPMLogic.c
* ����������MCU �����߼���
* 1  �ɼ���MCU ����������λ��������𱣳�ʱ��λ�������㣬
* 2  �ж�MCU �ϴ��µ�ʱ�Ƿ�����Ҫ��
* 3  �ж�MCU �Ƿ�ȫ����
* 4  ��¼��λ�Ĵ�����ֵ
* �������� codewarrior IDE 11.1
* MCU˵������MCU�޹�
* ����˵��: ���ܸ���
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��5��13��
* 
* �汾��1.1
* ˵����֧��renesas
* ԭ���ߣ�xb
* ������ڣ�2021��8��21��
*/

#include "DataType.h"
#include "MCUPMLogic.h"


#if  NOINIT_TYPE ==  NOINIT_TYPE_KEA
__attribute__ ((section(".mcudata"))) uint32 u32MCUData[7];
__attribute__ ((section(".mcudata_rtimes"))) uint32 u32PowerOnRestTimes;
__attribute__ ((section(".noinit"))) uint32  u32RestCheckSum;  //�����ж��Ƿ���ȫ����
#elif NOINIT_TYPE == NOINIT_TYPE_RL78
#pragma section bss MCUDATA
uint32 u32MCUData[7];

#pragma section bss MCUDATA_RTIMES
uint32 u32PowerOnRestTimes;

#pragma section bss NOINIT
uint32  u32RestCheckSum;  //�����ж��Ƿ���ȫ����

#pragma section 
#define __GOLBAL_VAR_
#endif


uint32 u32lResetData;
BOOL bMCULastOffOK;

//�ⲿ��������
extern void Save_MCUDATA(void);
extern BOOL Load_MCUDATA(void);

#define   MCUDATA_FLG_INIT       0x0
#define   MCUDATA_FLG_IGNWAKE    0x01
#define   MCUDATA_FLG_SAVEOK     0x02
/*******************************************************************************
* Function:      void MCU_SetReset(uint32 u32ResonRegister)
*
* Description:   ���ø�λ�Ĵ��� u32ResonRegister
*                 
* return :       none
* Note  :        �ϵ����һ��

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
* Description:   ����ɹر�ON ʱ����һ��
*                 
* return :        none
* Note  :         �ϵ����һ��

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
* Description:    ����ʱ����һ��
*                 
* return :        none
* Note  :         �ȴ�MCU ��λ

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
* Description:   �ϵ� EE ��ʼ�����ʱ�� ��ȡ����ֵ
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
	{//�ϴ��µ�ʱ������ȷ
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
	//Ϊ�˱����ϵ���д洢���˴������м���
}


/*******************************************************************************
 * Function:    BOOL MCUPM_IsLPowerRest(void)
 *
 * Description: �ж�MCU �Ƿ�͵�ѹ ��λ
 *
 * Returns:     1�� �ǵ͵�ѹ���µĸ�λ  0�� ����
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
 * Description: ��Դ����ʱ��Reset����
 *
 * Returns:     ��Դ����ʱ��Reset����
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
 * Description: �ж��ϴζϵ�ʱ�����Ƿ���ȷ
 *
 * Returns:     1�� ��ȷ   0�� ����
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
 * Description: ��IgnCount �� u32timesʱ ��0��������TRUE
 *
 * Returns:     1�� ����u32times 0��δ��
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
