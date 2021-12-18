/************************�ļ�˵��********************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CANDrive.h
* ����������CANDrive�� CANDriveWake ��ͷ�ļ���Ϊʵ�ִ˴˹�����Ҫ�����ļ�
* 1 GALIB ���ļ���derivative.h mcu�ײ������ļ���
* 2 CanIf.h  ��common ͷ�ļ�
* 3 CANIDConfig.h  Ӧ��CAN ID �����ļ�
* MCU˵����keaz128 ���ļ��и�MCU ��ء�
* ����˵���� ���ֺ궨����Ҫ���չ淶Ҫ�����
*/
/**************************���ļ�¼********************************************
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��
* 
* * �汾��2.0
* ˵��������CAN_STANDARD_ �궨��
* ԭ���ߣ�xb
* ������ڣ�2019��7��22��
* 
* * * �汾��4.0
* ˵��������EnableCanErrTxInterrupt 
* DisableCanErrTxInterrupt ����
* 2 ����4.0 .c �ļ�
* ԭ���ߣ�xb 
* ������ڣ�2019��9��22��
*
* * * �汾��4.1
* 1 ����4.1 .c �ļ�
* ԭ���ߣ�xb 
* ������ڣ�2019��12��24��


* * * �汾��4.2
* 1 CAN_STANDARD_      �˺궨����CONFIG����
* ԭ���ߣ�xb 
* ������ڣ�2020��7��8��
*/


#ifndef CANDRIVE_H_
#define CANDRIVE_H_

#include "LIBDriveDefine.h"
#include "CANIDConfig.h"
#include "CanIf.h"

//#define   CAN_STANDARD_     1    //��׼֡Ϊ1
#define   _CANWAKE_FUN_     1      //CAN���ѹ����Ƿ���Ҫ
#define   CANUNWAKE_TIME    50     //*100ms   ����ʱ��CANû��������Ϊ��û��CAN �źſ�������

#define   O_CANEN   GPIOA_PTD1     //���û��CANWAKE ����������ΪGPIO_UNUSE,Ҳ���Ը���CANDrive_SleepIO
#define   CANWAKE_KBI_NUM       0    // kbi �ĺż�ͨ�������޴˹��ܣ������ⶨ��
#define   CANWAKE_KBI_CHANEL    27

/*************���²��ܸ���*************************************************/
//can ����ͽ����жϿ��ƺ궨��
#define EnableCanErrTxInterrupt()      NVIC_DisableIRQ(MSCAN_TX_IRQn)
#define DisableCanErrTxInterrupt()     NVIC_EnableIRQ(MSCAN_TX_IRQn)
#define DisableCanRxInterrupt()			CAN_ReceiverFullIntDisable(MSCAN)
#define EnableCanRxInterrupt()         CAN_ReceiverFullIntEn(MSCAN)

//can ��ع��ܺ���
void CANDriver_BusOffInit(void);
void CANDriver_ClearTransmitBuffer(void);
BOOL CANDrive_TxBufferEmptemp(void);
BOOL CANDrive_SendFram(uint32 id, uint8* u8SendData);
void CANDrive_Close(void);
void CANDrive_SleepIO(uint8 En);
void Init_Mscan(void);

//wakeup ��ع��ܺ���
uint8 GetCanWakeEn(void);
void Deal_CANWake(void);
void CANWake_Begin(uint8 bEN);
void CANGet_Fram(void);
void CANDrive_GotoAWake(void);
void CANDrive_GotoSleep(void);

#endif /* CANDRIVE_H_ */
