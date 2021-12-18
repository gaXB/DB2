/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�IndirectNM.h
* �����������Ӽ����������ͷ�ļ������ֺ궨���밴˵�����ġ� �����Ҫʵ�ִ�ģ�飬��Ҫ��������CANͨѶ�ļ�
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
* �汾��2.0
* ˵��������2.0��.c
* ���ߣ�xb
* ������ڣ�2019��9��22��
* 
* * �汾��2.1
* ˵��������2.1��.c
* ���ߣ�xb
* ������ڣ�2019��12��16��
*/

#ifndef INDIRECTNM_H_
#define INDIRECTNM_H_
#include "canIf.h"

#define  NM_MAINFUNCTION_PERIOD                 (2)     //IndirectNMTask ��loop���õ�����ms
#define  CANNM_TXATTRID        0        //������������ʹ�õ�ID����Ӧ�÷����б��е���ţ�һ��ѡ��Ӧ�ñ����е�һ�����ģ�

#define   NM_SENDFF_TIME            10    //���͵�һ֡��ʱ��

#define   BUSOFF_QUICKRECOVER_TIME    50   //��ָ���ʱ��
#define   BUSOFF_SLOWRECOVER_TIME    200   //���ָ���ʱ��
 
typedef uint16 nmIndexType;
typedef uint8 nmNetIdType;

//��ģ��ʵ�ֵĹ���
void  IndirectNMTask(void);
void	IndirectNM_Init(void);
//�ײ���������CANIF �������º���
void CanNm_BusOff(uint8 u8Chanle);
void CanNm_TxConfirmation(uint8 u8Chanle);
/*Basic NM CallBack function for the Application*/
/*indicate nm normal*/
extern void ApplNmCanStart(const nmIndexType nmChannelHandle);
/*This callback sets the communication hardware (CAN controller,bus transceiver)
* to a state from which they can be woken up by the CAN bus.*/
extern void ApplNmCanSleep(const nmIndexType nmChannelHandle);
/*indicates bus off error occured*/
extern void ApplNmBusOff(const nmIndexType nmChannelHandle);
/*indicates can bus wake up*/
extern void ApplNmBusOffEnd(const nmIndexType nmChannelHandle);
extern void ApplNmBusOffRecoverySuccess(const nmIndexType nmChannelHandle);

#define NmCanInterruptRestore()              \
   CANIF_LEAVE_CRITICAL_SECTION()
#define NmCanInterruptDisable()              \
    CANIF_ENTER_CRITICAL_SECTION()
#define NMBusErrorCanInit(NmChanneld)        \
    Can_InitController(NmChanneld, (const Can_ControllerBaudrateConfigType*)(0))

#endif /* INDIRECTNM_H_ */
