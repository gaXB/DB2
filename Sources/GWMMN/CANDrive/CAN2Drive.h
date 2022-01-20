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


#ifndef CAN2DRIVE_H_
#define CAN2DRIVE_H_

#include "LIBDriveDefine.h"
#include "CANIDConfig.h"
#include "CanIf.h"

//#define   CAN_STANDARD_     1    //��׼֡Ϊ1
#define   _CAN2WAKE_FUN_     1      //CAN���ѹ����Ƿ���Ҫ
#define   CAN2UNWAKE_TIME    50     //*100ms   ����ʱ��CANû��������Ϊ��û��CAN �źſ�������

#define   O_CAN2EN   GPIOA_PTD1     //���û��CANWAKE ����������ΪGPIO_UNUSE,Ҳ���Ը���CANDrive_SleepIO
#define   CAN2WAKE_KBI_NUM       0    // kbi �ĺż�ͨ�������޴˹��ܣ������ⶨ��
#define   CAN2WAKE_KBI_CHANEL    27

/*************���²��ܸ���*************************************************/
//can ����ͽ����жϿ��ƺ궨��
#define EnableCan2ErrTxInterrupt()      NVIC_DisableIRQ(MSCAN_TX_IRQn)
#define DisableCan2ErrTxInterrupt()     NVIC_EnableIRQ(MSCAN_TX_IRQn)
#define DisableCan2RxInterrupt()			MCU_NOP//CAN_ReceiverFullIntDisable(MSCAN)
#define EnableCan2RxInterrupt()        MCU_NOP// CAN_ReceiverFullIntEn(MSCAN)

//can ��ع��ܺ���
void CAN2Driver_BusOffInit(void);
void CAN2Driver_ClearTransmitBuffer(void);
BOOL CAN2Drive_TxBufferEmptemp(void);
BOOL CAN2Drive_SendFram(uint32 id, uint8* u8SendData);
void CAN2Drive_Close(void);
void CAN2Drive_SleepIO(uint8 En);
void Init_Mscan2(void);

//wakeup ��ع��ܺ���
uint8 GetCan2WakeEn(void);
void Deal_CAN2Wake(void);
void CAN2Wake_Begin(uint8 bEN);
void CAN2Get_Fram(void);
void CAN2Drive_GotoAWake(void);
void CAN2Drive_GotoSleep(void);

#endif /* CAN2DRIVE_H_ */
