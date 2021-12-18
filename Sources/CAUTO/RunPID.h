/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�RunPID.h
* ����������PID �㷨ͷ�ļ�����Ҫ�ײ������ļ�
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��5��20��
* 
*/
#ifndef RUN_PID_H_
#define RUN_PID_H_

#include "LibDriveDefine.h"


typedef void (*PIDGetInit)(void);
typedef struct
{
	uint16 u16Period;   //0.1s
	int16 i16Kp;        // 1
	int16 i16Ki;        // /10
	int16 i16Kd;        // *1
	int16 i16PMax;      //*1
	int16 i16IMax;      //*0.1
	int16 i16DMax;      //*0.1
	int16 i16DI_Min;   //d i ��min  /10
	int16 i16DI_Max;   //d i ��max  /10	
	int16 i16ResMin;   //�����Сֵ
	int16 i16ResMax;   //������ֵ
	PIDGetInit pGetRunState;
}PIDPARAMETER;

typedef struct
{
	int16 i16Set;        //����ֵ  ���ϲ��߼�ʵ��
	int16 i16Get;        //ʵ��ֵ ���ϲ��߼�ʵ��
	int16 i16Diff;       //��ֵ
	int16 i16Diff_1;     //��ֵ-1
	int16 i16Diff_2;     //��ֵ-2
	int16 i16AM_FF0;     // ��ʼֵ ���ϲ��߼�ʵ��
	int16 int16AM_P;     // AM_P 
	int16 int16AM_I;     // AM_I
	int16 int16AM_D;     // AM_I
	int16 i16AM_Hfs;     // AM_Hfs
	uint16 Timer01;
	uint16 TenthSTimes;
   int32 i32GetSum;
   int16 i16FilterValue;
   int16 i16FilterValue_1;
   int16 i16OutInitValue;
	uint8 u8RunState;       //0 �رգ� 1ʹ�ó�ʼֵ�� ���� ����pid,���ϲ��߼�ʵ��
	uint8 bInit;        //�ڲ����Ʊ�����Ӧ�ò㲻��Ҫע��
	uint8 bRunEn;

}PIDDATA;


#define    PIDRUN_STATE_CLOSE       3
#define    PIDRUN_STATE_INIT         1
#define    PIDRUN_STATE_RUN         2
#define    PIDRUN_STATE_STOP        0

BOOL Deal_PIDFun(PIDDATA *pPData, PIDPARAMETER *pParameter);
#endif /* RUN_PID_EAC_H_*/
