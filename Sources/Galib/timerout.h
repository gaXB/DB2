 /************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�timerout.h
* ���������� ��ʱ��ģ���ͷ�ļ���
* 
* 
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��4��25��
* 
* �汾��1.1
* ˵��������uint16 TimeOut_GetMsData(void)
* ���ߣ�xb
* ������ڣ�2020��1��6��
*/
#ifndef TIMER_OUT_H_
#define TIMER_OUT_H_
#include "DataType.h"

//�ɸ��Ĳ���
#define  TIMEOUT_CPU_8BIT      0      //8 λ MCU ��Ϊ1�� ����Ҫ������ͣ���ָ��жϺ�����16λΪ0����Ҫ
#define  LOOP_MEASURE_TIME   3000     //����LOOP������

/***************���ɸķֽ�*****************************/
//���غ�������
void MsCounterUpadat(void);
void TenthSCounterUpadat(void);
void SCounterUpadat(void);
//Ӧ�ò�ɵ��ú���
BOOL TimeOutChkMsLong(uint16 *timer, uint16 timeOutVal);
BOOL TimeOutChkTenthSLong(uint16 *timer, uint16 timeOutVal);
BOOL TimeOutChkSLong(uint16 *timer, uint16 timeOutVal);
BOOL TimeOutChkTMsLong(uint16 *timer, uint16 timeOutVal);
uint32 TimeOut_TestLoop(void);
uint16 TimeOut_GetMaxLoop(void);
uint16 TimeOut_GetMsData(void);
#endif

