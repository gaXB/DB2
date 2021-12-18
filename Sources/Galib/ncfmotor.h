/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ncfmotor.h
* �������������ģ���ͷ�ļ���
* Ϊ��ģ��������������Ҫ���� ��Ҫ��ʱ��ģ��
*
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 *
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��5��20��
*
* �汾��1.1
* ˵�������Ĭ��Ϊ���������趨Ϊ255ʱΪ������
* ���ߣ�xb
* ������ڣ�2020��9��29��
*/ 
#ifndef NCFMOTOR_H__
#define NCFMOTOR_H__

#include "timerout.h"

#define MAX_NCFMOTOR_NUM   3 //���֧�ֵ������


//���״̬
#define NCFMOTOR_STATE_HALT   	0x00       //ֹͣ
#define NCFMOTOR_STATE_RESET  	0x01       //��ʼ��
#define NCFMOTOR_STATE_RUNBYSET  0x02    	  //�����趨ֵ����

//���������� ����
typedef enum
{
	NCFMOTOR_POSITION_BACKWARD = 0,    //��תλ��
	NCFMOTOR_POSITION_FORWARD,         //��תλ��
	NCFMOTOR_POSITION_INVALID,         //�����趨
	NCFMOTOR_POSITION_STOP = 255,
}NcfMotorSetType;


BOOL NcfMotor_Init(uint16 outtime, uint8 nmotor);
void NcfMotor_Reset(void);
void NcfMotor_Control(void);
void NcfMotor_SetPosition(uint8 motorId, NcfMotorSetType mPostion);
void NcfMotor_ChangePosition(uint8 motorId);
NcfMotorSetType NcfMotor_GetPosition(uint8 motorId);
#endif

