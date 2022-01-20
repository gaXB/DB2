/************************文件说明*****************************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：CabIf.c
* 功能描述：canif层 , CAN 硬件驱动与应用之间抽象接口
* 1  can数据发送 ， 
* 2  CAN接收 
* 3  BUSOFF 的接口，
* 4  实现ACK 的逻辑CanIf_TXConfirmation
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
* * 版本：1.1
* 说明：支持扩展帧的诊断
* 原作者：xb
* 完成日期：2019年7月20日
* 
* * * 版本：2.0
* 说明：配合innm 2.0以上
* 原作者：xb
* 完成日期：2019年7月20日
*
* * * 版本：2.1
* 说明：支持autosar
* 原作者：xb
* 完成日期：2019年12月24日
* 
* * * * 版本：2.2
* 说明：适应OSEK 而不会编译出错
* 原作者：xb
* 完成日期：2020年4月17日
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
//变量定义
static uint8 u8AckLogic;
//static uint16 IfBusTimes, IfBusEndTimes;    //调试用
volatile CanIf_PduSetModeType pduSetMode;   //调试用

//为了上层应用提供接口的外部函数调用声明
//CANINTERLAY
extern void InCAN_BusOff(void);
extern void InCAN_TxConfirmation(void);
extern void InCAN_L_Data_Indication(uint32 Identifier, uint8 DLC, uint8 *Data);
extern void InCAN_ClearTXBuffer(void);
//网络层
extern void  NetLay_ReciveDiag(uint8 nId,uint8* L_data);
/*******************************************************************************
 * Function:        Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId, PduInfoType const * PduInfoPtr)
 *
 * Description:     can if 发送一个pdu 出去
 *
 * Returns:         none
 *
 * Notes:          上层应用调用
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
//	{暂时取消此功能
		
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
//	{暂时取消此功能

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
 * Description:     设置canif 层的模式
 *
 * Returns:         none
 * 
 * Notes:   无实际功能          
 *
********************************************************************************/
void CanIf_SetPduMode
(
	uint8 ControllerId,
	CanIf_PduSetModeType PduModeRequest
)
{
	pduSetMode = PduModeRequest;
	//can 关闭所有应用
}
/*******************************************************************************
 * Function:        void CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode)
 *
 * Description:     设置canif 层的模式
 *
 * Returns:         none
 * 
 * Notes:   无实际功能     reserve     
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
 * Description:     设置初始化CAN
 *
 * Returns:         none
 * 
 * Notes:   无实际功能     reserve     
 *
********************************************************************************/
void Can_InitController
(
	uint8 Controller,
	const Can_ControllerBaudrateConfigType* Config
)
{
	u8AckLogic = 0;
	//此程序主要实现CAN 初始化来关闭发送的CAN ，但用此方法来关闭CAN 发送无法实现。
	//仍然使用busoff 时 清空缓冲的方式实现，此处不加的方式
	CANDriver_BusOffInit();
}
/*******************************************************************************
 * Function:      void CanIf_RxIndication(uint32 id, uint8* buff)
 *
 * Description:     can 驱动层收一帧报文，if指示给其他层
 *
 * Returns:         none
 * 
 * Notes:       can 驱动层调用
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
 * Description:     can 驱动层收收到唤醒信号
 *
 * Returns:         none
 * 
 * Notes:       can 驱动层调用； 功能预留
 *
********************************************************************************/
void CanIf_ControllerWakeUp(void)
{
//	CanNm_WakeUp(0);
}
/*******************************************************************************
 * Function:     void CanIf_ControllerBusOff(void)
 *
 * Description:     进入busoff
 *
 * Returns:         none
 * 
 * Notes:       can 驱动层busoff时调用
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
 * Description:     can数据发送完成
 *
 * Returns:         none
 * 
 * Notes:       can 驱动层调用，
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
 * Description:     清空缓存
 *
 * Returns:         none
 * 
 * Notes:       can 驱动层调用，
 *
********************************************************************************/
void CanIf_ClearTransmitBuffer(uint8 ControllerId)
{
	//CANDriver_ClearTransmitBuffer();
	InCAN_ClearTXBuffer();
}
