/**************************�ļ�˵��*****************************************************
* Copyright (c) 2021,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�EACControl.h
* ����������EACControl��Ͷ�ļ�
* �������� codewarrior IDE 11.1
* MCU˵���� ��MCU�޹أ��������ø���
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.5
* ˵��������.c
* ԭ���ߣ�xb
* ������ڣ�2021��12��17��
*
*/
#ifndef EACCONTROL_H_
#define EACCONTROL_H_
#include "APPDrive.h"

typedef struct
{
	uint16  ThreeWay1_Postion;            //LIN  1~2 ��0%�� 1-3��100%    1~2	1~3 50%
	uint16  ThreeWay2_Postion;            //LIN
	uint16   FourWay_Postion;             // LIN 1/0  0��1-2��3-4   1��1-4�� 2-3
	uint16  BEXV_Postion;
	uint16  EXV_Postion;
	uint16  AGS_Postion;
	uint8 SOV1;
	uint8 SOV2;
	uint8 EWPT1;
	uint8 EWPT2;
	uint8 EWPT3;
	uint8  IncarFAN;      //0 �أ� 1 ���٣� 2 ����
	//���
	uint16 MixPer;
	uint16 ModePer;
	uint16 NcfPer;
	ThemalModeType bCompMode;      // 0 �ر�  1 ���� 2 ����
	uint8  EACStart;      //
	uint8  EACStartReq;      //
	uint16 PTCStart;      //
	uint16 i16MaxSpeed;
	int16 i16MinSpeed;
}EACCONTROL;

typedef struct
{
	uint8 BatCoolStep;
	int16 BatSpeedLimt;
	int16 TotalPower;
}EACRUNSTATE;

#define EAC_ECODE_NULL  0
#define EAC_ECODE_SOV   0x0001


#define EAC_REASON_NULL  0
#define EAC_REASON_HIGHPRESS   0x0001
#define EAC_REASON_STEP        0x0002

#endif /* EACCONTROL_H_ */
