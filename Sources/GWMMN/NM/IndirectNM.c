/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�IndirectNM.c
* �����������Ӽ����������ʵ���ļ�������Ҫ����
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��
* 
* * �汾��2.0
* ˵�������ó�ʼ����ʵ�ֿ����ָ���ע�����ʱ����ҪС��3ms���ϼܹ����治���ô˷�ʽ
* ԭ���ߣ�xb
* ������ڣ�2019��9��22��
* 
* �汾2.1
* ԭ���ߣ�xb
* ������ڣ�2019��12��16��
* 1 NmTxCfmMainFunc �����жϣ����ı�u8BusOffState״̬��ֻ�п����ָ���ı��ı�
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


//�ⲿ���ú���
extern void InterLay_SendNM(uint8 u8Inder);
/*******************************************************************************
 * Function:        void	IndirectNM_Init(void)
 *
 * Description:     ��ʼ��
 *
 * Returns:         none
 *
 * Notes:          Ӧ�ò���� 
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
 * Notes:           ��canif �����
 *
********************************************************************************/
void CanNm_BusOff(uint8 u8Chanle)
{
	bNMStateBusoffInd = 1;
}
/*******************************************************************************
 * Function:        void CanNm_TxConfirmation(uint8 u8Chanle)
 *
 * Description:     ��ʼ��
 *
 * Returns:         none
 *
 * Notes:            ��canif �����
 *
********************************************************************************/
void CanNm_TxConfirmation(uint8 u8Chanle)
{
	bNMStateTxCompInd = 1;
}
/*******************************************************************************
 * Function:        static void NmCanBusOffCheckFun(void)
 *
 * Description:     busoff ���ܴ���
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
 * Description:     ��ʼ��
 *
 * Returns:         none
 *
 * Notes:           ��Ӧ�ò�loop ����
 *
********************************************************************************/
void IndirectNMTask(void)
{
	if (u16TimerSendFF != 0)
	{//��ʼ�����͵�һ֡
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
