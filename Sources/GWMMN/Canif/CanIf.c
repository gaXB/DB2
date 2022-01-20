/************************�ļ�˵��*****************************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CabIf.c
* ����������canif�� , CAN Ӳ��������Ӧ��֮�����ӿ�
* 1  can���ݷ��� �� 
* 2  CAN���� 
* 3  BUSOFF �Ľӿڣ�
* 4  ʵ��ACK ���߼�CanIf_TXConfirmation
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
* * �汾��1.1
* ˵����֧����չ֡�����
* ԭ���ߣ�xb
* ������ڣ�2019��7��20��
* 
* * * �汾��2.0
* ˵�������innm 2.0����
* ԭ���ߣ�xb
* ������ڣ�2019��7��20��
*
* * * �汾��2.1
* ˵����֧��autosar
* ԭ���ߣ�xb
* ������ڣ�2019��12��24��
* 
* * * * �汾��2.2
* ˵������ӦOSEK ������������
* ԭ���ߣ�xb
* ������ڣ�2020��4��17��
*/

#include "CANIf.h"
#if _NEED_OSKNM_
	#include "OsekNm_Cbk.h"
#endif
#if _INDIRECT_NM_
	#include "IndirectNM.h"
#endif

#if  _NEED_AUTOSARNM_
	#include "CanNm.h"
	#include "IndirectNMAutosar.h"
	#include "PMAssist_AutoSar.h"
#endif
//��������
static uint8 u8AckLogic;
//static uint16 IfBusTimes, IfBusEndTimes;    //������
volatile CanIf_PduSetModeType pduSetMode;   //������

//Ϊ���ϲ�Ӧ���ṩ�ӿڵ��ⲿ������������
//CANINTERLAY
extern void InCAN_BusOff(void);
extern void InCAN_TxConfirmation(void);
extern void InCAN_L_Data_Indication(uint32 Identifier, uint8 DLC, uint8 *Data);
extern void InCAN_ClearTXBuffer(void);
//�����
extern void  NetLay_ReciveDiag(uint8 nId,uint8* L_data);
/*******************************************************************************
 * Function:        Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId, PduInfoType const * PduInfoPtr)
 *
 * Description:     can if ����һ��pdu ��ȥ
 *
 * Returns:         none
 *
 * Notes:          �ϲ�Ӧ�õ���
 *
********************************************************************************/
Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId, PduInfoType const * PduInfoPtr)
{
	uint32 u32ID;
	if (CanTxPduId >= MAX_SEND_NUM)
	{
		return CANIF_RETURN_INVALID;
	}
	u32ID = TBL_SENDID[CanTxPduId];
	
//	if (pduSetMode == CANIF_SET_RX_OFFLINE)
//	{��ʱȡ���˹���
		
//	}
//	else
	{
#if (_NEED_OSKNM_ || _NEED_AUTOSARNM_)
		if (u32ID == CANID_NETMANGE)
		{
			u8AckLogic = 1;
		}
#endif
		if (CANDrive_SendFram(u32ID, PduInfoPtr->SduDataPtr))
		{
			return CANIF_RETURN_OK;
		}
		else
		{
			return CANIF_RETURN_EOK;
		}
	}
	return CANIF_RETURN_OK;
}

Std_ReturnType Can2If_Transmit(PduIdType CanTxPduId, PduInfoType const * PduInfoPtr)
{
	uint32 u32ID;
	if (CanTxPduId >= MAX_SEND2_NUM)
	{
		return CANIF_RETURN_INVALID;
	}
	u32ID = TBL_SEND2ID[CanTxPduId];

//	if (pduSetMode == CANIF_SET_RX_OFFLINE)
//	{��ʱȡ���˹���

//	}
//	else
	{
#if (_NEED_OSKNM_ || _NEED_AUTOSARNM_)
		if (u32ID == CANID_NETMANGE)
		{
			u8AckLogic = 1;
		}
#endif
		if (CAN2Drive_SendFram(u32ID, PduInfoPtr->SduDataPtr))
		{
			return CANIF_RETURN_OK;
		}
		else
		{
			return CANIF_RETURN_EOK;
		}
	}
	return CANIF_RETURN_OK;
}
/*******************************************************************************
 * Function:        void CanIf_SetPduMode(uint8 ControllerId, CanIf_PduSetModeType PduModeRequest)
 *
 * Description:     ����canif ���ģʽ
 *
 * Returns:         none
 * 
 * Notes:   ��ʵ�ʹ���          
 *
********************************************************************************/
void CanIf_SetPduMode
(
	uint8 ControllerId,
	CanIf_PduSetModeType PduModeRequest
)
{
	pduSetMode = PduModeRequest;
	//can �ر�����Ӧ��
}
/*******************************************************************************
 * Function:        void CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode)
 *
 * Description:     ����canif ���ģʽ
 *
 * Returns:         none
 * 
 * Notes:   ��ʵ�ʹ���     reserve     
 *
********************************************************************************/
void CanIf_SetControllerMode
(
	uint8 ControllerId,
	CanIf_ControllerModeType ControllerMode
)
{
	//reserve
}
/*******************************************************************************
 * Function:      void Can_InitController(uint8 Controller, const Can_ControllerBaudrateConfigType* Config)
 *
 * Description:     ���ó�ʼ��CAN
 *
 * Returns:         none
 * 
 * Notes:   ��ʵ�ʹ���     reserve     
 *
********************************************************************************/
void Can_InitController
(
	uint8 Controller,
	const Can_ControllerBaudrateConfigType* Config
)
{
	u8AckLogic = 0;
	//�˳�����Ҫʵ��CAN ��ʼ�����رշ��͵�CAN �����ô˷������ر�CAN �����޷�ʵ�֡�
	//��Ȼʹ��busoff ʱ ��ջ���ķ�ʽʵ�֣��˴����ӵķ�ʽ
	CANDriver_BusOffInit();
}
/*******************************************************************************
 * Function:      void CanIf_RxIndication(uint32 id, uint8* buff)
 *
 * Description:     can ��������һ֡���ģ�ifָʾ��������
 *
 * Returns:         none
 * 
 * Notes:       can ���������
 *
********************************************************************************/
void CanIf_RxIndication(uint32 id, uint8* buff)
{
	
#if (_NEED_OSKNM_ || _NEED_AUTOSARNM_)
	if (ID_R_NM_BIEGIN <= id && id <= ID_R_NM_END)
	{
		PduInfoType pduData;
		pduData.SduDataPtr = buff;
		pduData.SduLength = 8;
#if _NEED_AUTOSARNM_		
		CanNm_RxIndication(0, (SAICPduInfo_t*)&pduData);
		NMAPP_RxIndication();
#else
		CanNm_RxIndication(0, id, &pduData);
#endif
	}
	else
#endif
	if (id == ID_DIAG_FUNCT)
	{
		NetLay_ReciveDiag(ID_DIAG_FUNCT_IF, buff);    
	}
	else if (id == ID_DIAG_PHYSIC)
	{
		NetLay_ReciveDiag(ID_DIAG_PHYSIC_IF, buff);    
	}
	else
	{
		InCAN_L_Data_Indication(id, 8, buff);
	}

}


extern void InCAN2_L_Data_Indication(uint32 Identifier, uint8 DLC, uint8 *Data);
void Can2If_RxIndication(uint32 id, uint8* buff)
{

#if (_NEED_OSKNM_ || _NEED_AUTOSARNM_)
	if (ID_R_NM_BIEGIN <= id && id <= ID_R_NM_END)
	{
		PduInfoType pduData;
		pduData.SduDataPtr = buff;
		pduData.SduLength = 8;
#if _NEED_AUTOSARNM_
		CanNm_RxIndication(0, (SAICPduInfo_t*)&pduData);
		NMAPP_RxIndication();
#else
		CanNm_RxIndication(0, id, &pduData);
#endif
	}
	else
#endif
	if (id == ID_DIAG_FUNCT)
	{
		NetLay_ReciveDiag(ID_DIAG_FUNCT_IF, buff);
	}
	else if (id == ID_DIAG_PHYSIC)
	{
		NetLay_ReciveDiag(ID_DIAG_PHYSIC_IF, buff);
	}
	else
	{
		InCAN2_L_Data_Indication(id, 8, buff);
	}

}
/*******************************************************************************
 * Function:      void CanIf_ControllerWakeUp(void)
 *
 * Description:     can ���������յ������ź�
 *
 * Returns:         none
 * 
 * Notes:       can ��������ã� ����Ԥ��
 *
********************************************************************************/
void CanIf_ControllerWakeUp(void)
{
//	CanNm_WakeUp(0);
}
/*******************************************************************************
 * Function:     void CanIf_ControllerBusOff(void)
 *
 * Description:     ����busoff
 *
 * Returns:         none
 * 
 * Notes:       can ������busoffʱ����
 *
********************************************************************************/
void CanIf_ControllerBusOff(void)
{
	CanNm_BusOff(0);	
	//IfBusTimes++;
	InCAN_BusOff();
}
/*******************************************************************************
 * Function:     void CanIf_TXConfirmation(void)
 *
 * Description:     can���ݷ������
 *
 * Returns:         none
 * 
 * Notes:       can ��������ã�
 *
********************************************************************************/
void CanIf_TXConfirmation(void)
{
#if	(_NEED_OSKNM_ || _NEED_AUTOSARNM_)
	if (u8AckLogic)
	{
		u8AckLogic = 0;
		CanNm_TxConfirmation(0);
#if (_NEED_AUTOSARNM_)
		NMAutoA_TxConfirmation(0);
#endif
		InCAN_TxConfirmation();
//		if (IfBusTimes)
//		{
//			IfBusEndTimes = IfBusTimes;
//			IfBusTimes = 0;
//		}
	}
#elif _INDIRECT_NM_
	CanNm_TxConfirmation(0);
	InCAN_TxConfirmation();
#else//autosar
	CanNm_TxConfirmation(0);
	InCAN_TxConfirmation();
#endif
}
/*******************************************************************************
 * Function:     void CanIf_ClearTransmitBuffer(uint8 ControllerId)
 *
 * Description:     ��ջ���
 *
 * Returns:         none
 * 
 * Notes:       can ��������ã�
 *
********************************************************************************/
void CanIf_ClearTransmitBuffer(uint8 ControllerId)
{
	//CANDriver_ClearTransmitBuffer();
	InCAN_ClearTXBuffer();
}
