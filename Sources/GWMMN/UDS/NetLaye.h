/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�NetLaye.h
* ����������������ͷ�ļ���ʵ�ִ�ģ����Ҫ���� CANIF��
* ���ֺ궨����Ҫ����
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��

 * 
* �汾��0.5
* ˵��������.c�ļ��ð汾
* ���ߣ�xb
* ������ڣ�2019��10��25��
*/

#ifndef NET_LAY_H__
#define NET_LAY_H__
#include "CanIf.h"


//��ʱ����
#define     TIMER_N_AR     70   //����㷢�͵�ʱ��
#define     TIMER_N_AS     70
#define     TIMER_N_BS     150 // ���ͷŽ���������֡��ʱ��
#define     TIMER_N_CR     150 // ���ܷŽ�������֡��ʱ��

//��������
#define  NETLAY_BS      8     //fc  continue times
#define  NETLAY_STmin   20    // continue fram  send time must small than it
#define  NETLAY_N_WFTMAX      0  // The FC Receive times must small than it

//����
#define  NETLAY_MAXLENGTH    255   //������������ܽ��ܵ���󳤶�,����������أ�֧��dtc12 12*4+3=51
#define  MAX_NPDU_BUFFER     3   //����㴦�������Ļ���
// result define
#define  N_Success      0
#define  N_Timeout_A    1
#define  N_Timeout_Bs   2
#define  N_Timeout_Cr   3
#define  N_Wrong_SN     4
#define  N_Invalid_FS   5   //�Ƿ���������֡
#define  N_Buffer_Overflow 6  
#define  N_UNEXP_PDU       7   //���յ���Ԥ�ڵ�PDU�����շ�Ӧ���Ըñ��ģ�
#define  N_WFT_OVRN        8   //�ȴ�������֡������
#define  N_ERROR           9

//result of ChangeParameter
#define  N_Success                  0
#define  N_Undefined_Parameter      1
#define  N_Wrong_Value              2
#define  N_Busy                     3



#define   AID_PHYSIC     ID_DIAG_PHYSIC_IF
#define   AID_FUNIC      ID_DIAG_FUNCT_IF
#define  CANDIAG_IFID                         1       //can if ���ID

 
/*******************************************************************************
 *���ݽṹ����
 *
********************************************************************************/
#define  N_AI     uint8
#define  N_DATA   uint8

//netlay fram pci struct
typedef  struct
{
   unsigned    Length_SN  :4;
   unsigned    nPciType   :4;   //    
}N_PCI;

//netlay protect data unit struct
typedef struct
{
   N_AI  nId;
   N_PCI Pci;
   N_DATA   Data[7];
}N_PDU;

//���ͽ������ݵ� ���ݽṹ
typedef  struct
{
   N_AI     nId;             //����ʱΪ Ŀ��ID,����ʱΪ�����ߵ�ID
   uint8    Data[NETLAY_MAXLENGTH];
   uint16   Length;
   uint8    N_Result;
}N_USDATA_IND;


/// n_usdata_con to app  struct
typedef  struct
{
   N_AI     nId;
   uint8    N_Result;
}N_USDATA_CON;


//����1֡�����ݽṹ
typedef struct
{
   uint8    S_State;   //����״̬
   uint16   S_Length;  //����
   uint8    S_mId;     //Ŀ��ID
   uint8    S_SN;     //SN 
   uint8    S_BS;       //BS 
   uint16    S_STmin;    //stmin 
   uint16   S_TimerSend;    
} N_SendData;


//����1֡�����ݽṹ
typedef struct
{
   uint8    R_State;   //����״̬
   uint16   R_Length;  //����
   uint8    R_mId;     //���ܵ�ID
   uint8    R_SN;     //SN 
   uint8    R_BS;       //BS 
   uint16    R_STmin;    //stmin   
} N_RecData;
/**********************************************************************
�������� 
************************************************************************/

//�ⲿ���ú���
void Send_78NRC(uint8 SID);
//��ģ��ʵ�֣��ṩ�ⲿ���ú���
uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target);
void  Deal_UDSNetLay(void);
#endif
