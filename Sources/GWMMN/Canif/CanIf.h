/************************�ļ�˵��********************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CANIf.h
* ����������CANIf ��ͷ�ļ���Ϊʵ�ִ˹�����Ҫ���������ļ�
 1 HardWareDrive.h 
 2 can common ͷ�ļ�
 3 CANIDConfig.h Ӧ��CAN ID �����ļ�
 4 InterLayCan�� NetLaye�� CanDrive�����������ļ�
*
*/
/**************************���ļ�¼********************************************
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�����ԭ���ߣ����޸��ߣ�����
* ������ڣ�2019��1��9��
* 
* * �汾��1.1
* ˵����CANIF_LEAVE_CRITICAL_SECTION
* CANIF_ENTER_CRITICAL_SECTION �����жϺ�������nvic
* ԭ���ߣ�����ԭ���ߣ����޸��ߣ�����
* ������ڣ�2019��9��22��
* 
* * * * * �汾��2.2
* ˵��������.c
* ԭ���ߣ�xb
* ������ڣ�2020��4��17��
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

//�ײ���Ҫ���õĺ���
void CanIf_ControllerWakeUp(void);
void CanIf_ControllerBusOff(void);
void CanIf_RxIndication(uint32 id, uint8* buff);
void CanIf_TXConfirmation(void);
//�ϲ�Ӧ�ÿɵ��õĺ���
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
