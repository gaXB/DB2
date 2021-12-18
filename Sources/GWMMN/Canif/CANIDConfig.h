/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CANIDConfig.h
* ����������CANID ����
* 1  ���ID,����ID ,����ID 
* 2  ����IF id ����
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
* ����˵���� �ļ����չ淶����
* ���ļ���PC �Զ����ɣ�PC �汾 V1.0, DBC �ļ�����  CVTC_SV51-02&B1_CMX_20200409_BD_V4.81 - HCM.dbc
* �������ڣ�  01/28/21 09:23:21
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��7��22��

* �汾��1.1
* ˵��������autosar
* ԭ���ߣ�xb
* ������ڣ�2019��10��23��
* 
* �汾��1.2
* ˵���� CAN_STANDARD_ �ĺ궨���ڴ˴����ã� CANDrive.H �ļ�������궨������ 
* ���ߣ�XB
* ������ڣ�2020��7��7��

* �汾��1.2
* ˵�������ļ���CAN �˲����룬���鹤��ʦ���¶��� 
* ���ߣ�
* ������ڣ�
*/

#ifndef CANIDCONFIG_H_
#define CANIDCONFIG_H_



#define   CAN_STANDARD_    1   //��׼֡

 //CAN ���յ��źŶ���     
#define     R_ID_1F2     0x1F2
#define     R_ID_1F5     0x1F5
#define     ID_DIAG_FUNCT   0x7DF
#define     ID_DIAG_PHYSIC   0x721

#if   (CAN_STANDARD_ ==1)
#define  ID_DIAG_FUNCT_IF           ((uint8)(ID_DIAG_FUNCT&0xff))
#define  ID_DIAG_PHYSIC_IF           ((uint8)(ID_DIAG_PHYSIC&0xff))
#else
#define  ID_DIAG_FUNCT_IF            2
#define  ID_DIAG_PHYSIC_IF           1
#endif

//CAN �������붨�壬 ��ʼ�����������,��չֻ֡��2����׼֡��4��
//д��ͨ��1 ������ֵ
#define   RECEIVE_ID1_1        0x1F2
#define   RECEIVE_ID1_2        0x1F2
#define   RECEIVE_ID1_3        0x1F2
#define   RECEIVE_ID1_4        0x1F2


#define  RECEIVE_ID1     (RECEIVE_ID1_1 & RECEIVE_ID1_2 & RECEIVE_ID1_3 & RECEIVE_ID1_4) 
#define  RECEIVE_ID1_OR   (RECEIVE_ID1_1 | RECEIVE_ID1_2 | RECEIVE_ID1_3 | RECEIVE_ID1_4) 
#define  RECEIVE_ID1_MASK   (RECEIVE_ID1 ^ RECEIVE_ID1_OR)

//д��ͨ��2 ������ֵ
#define   RECEIVE_ID2_1        0x1F5
#define   RECEIVE_ID2_2        0x1F5
#define   RECEIVE_ID2_3        0x1F5
#define   RECEIVE_ID2_4        0x1F5


#define  RECEIVE_ID2     (RECEIVE_ID2_1 & RECEIVE_ID2_2 & RECEIVE_ID2_3 & RECEIVE_ID2_4) 
#define  RECEIVE_ID2_OR   (RECEIVE_ID2_1 | RECEIVE_ID2_2 | RECEIVE_ID2_3 | RECEIVE_ID2_4) 
#define  RECEIVE_ID2_MASK   (RECEIVE_ID2 ^ RECEIVE_ID2_OR)

//д��ͨ��3 ������ֵ
#define   RECEIVE_ID3_1        0x721
#define   RECEIVE_ID3_2        0x721
#define   RECEIVE_ID3_3        0x721
#define   RECEIVE_ID3_4        0x721


#define  RECEIVE_ID3     (RECEIVE_ID3_1 & RECEIVE_ID3_2 & RECEIVE_ID3_3 & RECEIVE_ID3_4) 
#define  RECEIVE_ID3_OR   (RECEIVE_ID3_1 | RECEIVE_ID3_2 | RECEIVE_ID3_3 | RECEIVE_ID3_4) 
#define  RECEIVE_ID3_MASK   (RECEIVE_ID3 ^ RECEIVE_ID3_OR)

//д��ͨ��4 ������ֵ
#define   RECEIVE_ID4_1        0x7DF
#define   RECEIVE_ID4_2        0x7DF
#define   RECEIVE_ID4_3        0x7DF
#define   RECEIVE_ID4_4        0x7DF


#define  RECEIVE_ID4     (RECEIVE_ID4_1 & RECEIVE_ID4_2 & RECEIVE_ID4_3 & RECEIVE_ID4_4) 
#define  RECEIVE_ID4_OR   (RECEIVE_ID4_1 | RECEIVE_ID4_2 | RECEIVE_ID4_3 | RECEIVE_ID4_4) 
#define  RECEIVE_ID4_MASK   (RECEIVE_ID4 ^ RECEIVE_ID4_OR)

//CAN ���͵��źŶ���
#define   ID_DIAG_CLM    0x7A1       //���ID
#define   ID_TX1    	   0x0C9
#define   ID_TX2    	   0x1F1

//CAN ���͵��ź� CANIF ���߼��Ŷ���
#define   IDIF_TX1      2
#define   IDIF_TX2      3

//CAN �������ʹ��˵��  
#define  _NEED_OSKNM_    0
#define  _NEED_AUTOSARNM_   0
#define  _INDIRECT_NM_      1
#if      _NEED_OSKNM_ || _NEED_AUTOSARNM_
#define   CANID_NETMANGE   0x403
#define   ID_R_NM_BIEGIN   0x400
#define  ID_R_NM_END  0x4FF
#else
#define  CANID_NETMANGE  0x403
#endif

// CAN IF ����߼�������������
#define MAX_SEND_NUM  4
static const uint32 TBL_SENDID[MAX_SEND_NUM] ={CANID_NETMANGE, ID_DIAG_CLM, ID_TX1, ID_TX2};

#endif /* CANIDCONFIG_H_ */
