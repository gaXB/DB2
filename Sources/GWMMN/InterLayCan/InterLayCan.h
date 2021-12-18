/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�InterLayCAN.h
* ����������InterLay���ͷ�ļ���ʵ�ִ�ģ����Ҫ���� CANIF��
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��

* �汾��2.0
* ˵������LIB �ĵ�ѹ��Χ
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��

* �汾��2.3
* ˵�������� .c
* ԭ���ߣ�xb
* ������ڣ�2019��12��24��
* 
* �汾��2.4
* ˵�������� .c
* ԭ���ߣ�zhupan
* ������ڣ�2021��08��31��
* 
*/

#ifndef INTERLAYCAN_H_
#define INTERLAYCAN_H_
#include "InterLayMatrix.h"

/******************************************************************************
���͹���Ĺ�������λ
******************************************************************************/		
#define  _TX_USENM_   1    //�Ƿ�ʹ���������ֱ�ӽӼ䶼��
#define  _TX_USEUDS_  1    //UDS ��Ϲ���
#define  _TX_USEVOLT_ 1    //��Դ����can ���������OSK ������������һ��ҪΪ1


/******************************************************************************
 //CAN ���ͣ���busoff�������
******************************************************************************/
#define   BUSOFF_CLEAR_TIME     5000         //busoff ���϶���ʱ������0��* ms
#define   CANTX_VOLT_HIGHERR    ACD_V(16.1)        //can���͵�ѹ��Χ
#define   CANTX_VOLT_HIGHOK     ACD_V(15.9)
#define   CANTX_VOLT_LOWERR     ACD_V(8.9)
#define   CANTX_VOLT_LOWOK      ACD_V(9.1)

#define  BUSOFF_DTC_TIMES   3            //busoff ���ٴκ����BUSOFF״̬

/******************************************************************************
CANͨ���շ�ʹ��״̬
******************************************************************************/
#define   InCAN_RX_DISABLE                     0      
#define   InCAN_RX_ENABLE                      1
#define   InCAN_PSEUDO_DISABLE                 2   //��״̬ʱ�����в��Խ��ճ�ʱ

/******************************************************************************
CAN���ķ�������״̬
******************************************************************************/
#define   InCAN_TX_DISABLE                  0
#define   InCAN_TX_ENABLE                   1

/******************************************************************************
CAN���ĳ�ʼ����Ԥ��
******************************************************************************/
#define   InCAN_MODE_NORMAL                 0
#define   InCAN_MODE_INIT                   1

#define   CAN_INITMODE_TIME              100    

/***  ����״̬Ԥ����  ***/
/*  ====================================================================================
            Bit 7       |    Bit 6 ~ Bit 2   |        Bit 1       |        Bit 0       
    -----------------------------------------------------------------------------------
          CAN_TX_EN     |        Rsvd        |     CAN_UPDATE     |    CAN_MSG_LOST
                        |                    |                    |      ���Ķ�ʧ
          ����ʹ��      |        ����        |    ���ĸ��±�־    |   (���Ľ��ճ�ʱ)
    ===================================================================================*/
#define   CAN_TX_EN                         0x80
#define   CAN_UPDATE                        0x02
#define   CAN_MSG_LOST                      0x01


//Ӧ�ò㹦�ܺ���
void InterLayTask(void);
void InterLayInit(void);
uint8 InterLay_GetRxMode(void);
uint8 InterLay_GetTxMode(void);
uint8 InCAN_GetBusOffDTC(void);
//��ҪCANIF ����õĺ���
void InCAN_BusOff(void);
void InCAN_TxConfirmation(void);
void InCAN_L_Data_Indication(uint32 Identifier, uint8 DLC, uint8 *Data);
void InCAN_ClearTXBuffer(void);
void InCAN_TXImmediate(uint8 u8IfId);
/*Basic INTER CallBack function for the Application*/
/*indicate nm normal*/
extern void APPlIL_RXComplete(uint32 u32Id);
extern void APPlIL_TXComplete(uint8 u8IfId);
extern void APPlIL_RecTimeOut(uint32 u32Id);
extern BOOL APPlIL_TXStart(uint8 u8IfId);
#endif /* INTERLAYCAN_H_ */
