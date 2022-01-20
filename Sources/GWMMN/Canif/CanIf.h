/************************文件说明********************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：CANIf.h
* 功能描述：CANIf 的头文件，为实现此功能需要包含以下文件
 1 HardWareDrive.h 
 2 can common 头文件
 3 CANIDConfig.h 应用CAN ID 配置文件
 4 InterLayCan， NetLaye， CanDrive及网络管理的文件
*
*/
/**************************更改记录********************************************
* 版本：1.0
* 说明：初始版本建立
* 原作者：输入原作者（或修改者）名字
* 完成日期：2019年1月9日
* 
* * 版本：1.1
* 说明：CANIF_LEAVE_CRITICAL_SECTION
* CANIF_ENTER_CRITICAL_SECTION 开关中断函数利用nvic
* 原作者：输入原作者（或修改者）名字
* 完成日期：2019年9月22日
* 
* * * * * 版本：2.2
* 说明：跟随.c
* 原作者：xb
* 完成日期：2020年4月17日
*/


#ifndef CANIF_H_
#define CANIF_H_


#include "CanDrive.h"
#include "Can2Drive.h"
#include "ComStack_Types.h"

#define  CANIF_LEAVE_CRITICAL_SECTION()   EnableCanErrTxInterrupt() 
#define  CANIF_ENTER_CRITICAL_SECTION()   DisableCanErrTxInterrupt() 
#define  Can_IdType  uint32

typedef enum
{
	CANIF_SET_OFFLINE = 0x00,
	CANIF_SET_ONLINE = 0x07,
	CANIF_SET_RX_OFFLINE = 0x06,
	CANIF_SET_RX_ONLINE = 0x09,
	CANIF_SET_TX_OFFLINE = 0x01,
	CANIF_SET_TX_OFFLINE_ACTIVE = 0x04,
	CANIF_SET_TX_ONLINE = 0x0e
	
}CanIf_PduSetModeType ;


typedef enum
{
	CANIF_CS_UNINIT = 0,
	CANIF_CS_SLEEP,
	CANIF_CS_STARTED,
	CANIF_CS_STOPPED
}CanIf_ControllerModeType;


typedef struct
{
	uint8 CanRegBTR0;
	uint8 CanRegBTR1;
}Can_ControllerBaudrateConfigType;


#define	CANIF_RETURN_OK   0
#define	CANIF_RETURN_EOK  1
#define 	CANIF_RETURN_INVALID  0xff

//底层需要调用的函数
void CanIf_ControllerWakeUp(void);
void CanIf_ControllerBusOff(void);
void CanIf_RxIndication(uint32 id, uint8* buff);
void CanIf_TXConfirmation(void);
//上层应用可调用的函数
Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId, PduInfoType const * PduInfoPtr);
Std_ReturnType Can2If_Transmit(PduIdType CanTxPduId, PduInfoType const * PduInfoPtr);

void Can_InitController
(
	uint8 Controller,
	const Can_ControllerBaudrateConfigType* Config
);

void CanIf_SetPduMode
(
	uint8 ControllerId,
	CanIf_PduSetModeType PduModeRequest
);
void CanIf_SetControllerMode
(
	uint8 ControllerId,
	CanIf_ControllerModeType ControllerMode
);
void CanIf_ClearTransmitBuffer(uint8 ControllerId);

#endif /* CANIF_H_ */
