/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�RunPID_Increment.h
* ���������� ����ʽPID �㷨�����ø���
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.1
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��11��21��
* 
*/
#ifndef RUN_PID_EAC_H_
#define RUN_PID_EAC_H_

#include "LibDriveDefine.h"


typedef void (*PIDGetInit)(void);

typedef struct
{
	uint16 u16Period;   //0.1s
	int16 i16Kp;        // 1
	int16 i16Ki;        // 1
	int16 i16Kd;        // *1
	int16 i16ResMin;   //�����Сֵ
	int16 i16ResMax;   //������ֵ
	PIDGetInit pGetRunState;
}PIDPARAMETER_INC;

typedef struct
{
	int16 i16Set;        //����ֵ  ���ϲ��߼�ʵ��
	int16 i16Get;        //ʵ��ֵ ���ϲ��߼�ʵ��
	int16 i16Diff;       //��ֵ
	int16 i16Diff_1;     //��ֵ-1
	int16 i16Diff_2;     //��ֵ-2
	int16 i16AM_Hfs;     // AM_Hfs
	uint16 Timer01;
}PIDDATA_INC;



int Deal_PIDFun_INC(PIDDATA_INC *pPData, PIDPARAMETER_INC *pParameter);
#endif /* RUN_PID_EAC_H_*/
