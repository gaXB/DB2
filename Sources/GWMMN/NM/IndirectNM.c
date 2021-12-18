/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：IndirectNM.c
* 功能描述：接间网络管理功能实现文件，不需要更改
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日
* 
* * 版本：2.0
* 说明：利用初始化来实现快慢恢复，注意调用时间需要小于3ms，老架构上面不能用此方式
* 原作者：xb
* 完成日期：2019年9月22日
* 
* 版本2.1
* 原作者：xb
* 完成日期：2019年12月16日
* 1 NmTxCfmMainFunc 发送中断，不改变u8BusOffState状态，只有快慢恢复后改变后改变
* 
*/

#include "IndirectNM.h"


#define   NMCHANELE    0

#define   BUSOFF_STATE_NORMAL   0
#define   BUSOFF_STATE_OFF      1

static uint8  u8BusOffState = BUSOFF_STATE_NORMAL;
static uint8  u8BusOffCoverTimes;
static uint16 u16TimerSendFF, u16TimerBusOffRecover;

static union
{
	uint8 Byte;
	struct
	{
		uint8 bSendFF      :1;
		uint8 bBusOffInd   :1;
		uint8 bTxCompInd  :1;
	}Bits;
}NMState;

#define bNMStateSendFF       NMState.Bits.bSendFF
#define bNMStateBusoffInd    NMState.Bits.bBusOffInd
#define bNMStateTxCompInd    NMState.Bits.bTxCompInd


//外部调用函数
extern void InterLay_SendNM(uint8 u8Inder);
/*******************************************************************************
 * Function:        void	IndirectNM_Init(void)
 *
 * Description:     初始化
 *
 * Returns:         none
 *
 * Notes:          应用层调用 
 *
********************************************************************************/
void	IndirectNM_Init(void)
{
	ApplNmCanStart(NMCHANELE);
	u8BusOffCoverTimes = 0;
	u8BusOffState = BUSOFF_STATE_NORMAL;
	u16TimerSendFF = NM_SENDFF_TIME / NM_MAINFUNCTION_PERIOD;
}
/*******************************************************************************
 * Function:        void	CanNm_BusOff(uint8 u8Chanle)
 *
 * Description:     busoff 
 *
 * Returns:         none
 *
 * Notes:           由canif 层调用
 *
********************************************************************************/
void CanNm_BusOff(uint8 u8Chanle)
{
	bNMStateBusoffInd = 1;
}
/*******************************************************************************
 * Function:        void CanNm_TxConfirmation(uint8 u8Chanle)
 *
 * Description:     初始化
 *
 * Returns:         none
 *
 * Notes:            由canif 层调用
 *
********************************************************************************/
void CanNm_TxConfirmation(uint8 u8Chanle)
{
	bNMStateTxCompInd = 1;
}
/*******************************************************************************
 * Function:        static void NmCanBusOffCheckFun(void)
 *
 * Description:     busoff 功能处理
 *
 * Returns:         none
 *
 * Notes:          
 *
********************************************************************************/
static void NmCanBusOffCheckFun(void)
{
	uint16 BusOffFlg;
	NmCanInterruptDisable();
   BusOffFlg = bNMStateBusoffInd;
   bNMStateBusoffInd = 0;
   NmCanInterruptRestore();
   
   if (BusOffFlg)
   {
  		u8BusOffState = BUSOFF_STATE_OFF;
  		if (u8BusOffCoverTimes < 255)
  		{
  			u8BusOffCoverTimes++;
  		}
  		else{}
  		ApplNmBusOff(NMCHANELE);
  		
  		if (u8BusOffCoverTimes < 6)
  		{
  			u16TimerBusOffRecover = BUSOFF_QUICKRECOVER_TIME / NM_MAINFUNCTION_PERIOD;
  		}
  		else
  		{
  			u16TimerBusOffRecover = BUSOFF_SLOWRECOVER_TIME / NM_MAINFUNCTION_PERIOD;
  		}
  		NMBusErrorCanInit(NMCHANELE);
   }
   else{}
}

static void NmTxCfmMainFunc(void)
{
   uint16 TxCfmFlg;
   NmCanInterruptDisable();
   TxCfmFlg = bNMStateTxCompInd;
   bNMStateTxCompInd = 0;
   NmCanInterruptRestore();
   
   if (TxCfmFlg)
   {
   	if (u8BusOffCoverTimes != 0)
   	{
   		u8BusOffCoverTimes = 0;
   		ApplNmBusOffRecoverySuccess(NMCHANELE);
   	}
   	else{}
   }
   else{}
}

static void NmRecoverFunc(void)
{
	if (u8BusOffState == BUSOFF_STATE_OFF)
	{
		if (0 != u16TimerBusOffRecover)
		{
			u16TimerBusOffRecover--;
		}
		else{}
		
		if (0 == u16TimerBusOffRecover)
		{
			InterLay_SendNM(CANNM_TXATTRID);
			ApplNmBusOffEnd(NMCHANELE);
			u8BusOffState = BUSOFF_STATE_NORMAL;
		}
		else{}
	}
	else{}
}
/*******************************************************************************
 * Function:        IndirectNMTask
 *
 * Description:     初始化
 *
 * Returns:         none
 *
 * Notes:           由应用层loop 调用
 *
********************************************************************************/
void IndirectNMTask(void)
{
	if (u16TimerSendFF != 0)
	{//初始化发送第一帧
		u16TimerSendFF--;
		if (0 == u16TimerSendFF)
		{
			InterLay_SendNM(CANNM_TXATTRID);
		}
	}
	else
	{
		NmRecoverFunc();
		NmCanBusOffCheckFun();
		NmTxCfmMainFunc();
		
	}
}
