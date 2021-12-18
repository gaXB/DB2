/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�UDS_APP.h
* 1 ����PROGRAMDATA ������
* 2 F110 �ĳ��Ⱥ�����
* 3 DID �Ľṹ��PID_DATA ��DID �ܸ�����д��ĸ���
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 

2019.1.17
ver0.1
1 ��ʼ�汾
2 ģ�����

2019.10.25
ver0.2
1 F110 DID���Ⱥ����ݵĺ궨��ģ��

2021.8.23
ver0.3
1 ��PD_NUM_xx ���б�ע˵��
*/
#ifndef UDS_APP_H_
#define UDS_APP_H_

#define  PD_NUM_INTEGRITY        0   //������ 
#define  PD_NUM_DEPENDCE         1   //������
#define  PD_NUM_SESSIONMODE      2   //ģʽ�л�ʱ��ʹ��
#define  PD_NUM_PROCHECKH        3   //����У����
#define  PD_NUM_PROCHECKL        4
#define  PD_NUM_PROTIMESH        5   //ˢд����
#define  PD_NUM_PROTIMESL        6
#define  PD_NUM_EECHECK          7   //check sum 
#define  PD_NUM_EESTATE          8   //״̬����δ�õ�

//�����ı������ⲿ�������ɹ�����ģ��ʹ��
extern uint8 ProgramData[];
extern uint8 F110SaveNumber;
extern uint8 u8ConfigData[];
#define  PID_F110_LENGTH     16        //���ܳ���16
extern uint8 TBL_F110ARRY[15][PID_F110_LENGTH];
extern uint8 TBL_F110[];
#define F110_DEFAULT_DATA   {0x43, 0x12, 0x34, 0x56, 0x78, 0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x01, 0x38,0x80, 0xff, 0xff}


//did ���ݽṹ
typedef struct
{
   uint16 PID_ID;         //ID
   const uint8* pData;    //����
   uint8  PIDLength;      //����
}PID_DATA;

//DID �Ŷ���
#define MAX_PID_NUM   13
#define WRITE_PID_NUM  3

extern PID_DATA PID[];
#endif /* UDS_APP_H_ */
