/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�MIXVENT_PID.h
* ������������ϵ��PID
* ������¶�PID ����,���ø���
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��11��21��
* 
*/
#ifndef MIXVENT_PID_H_
#define MIXVENT_PID_H_
#include "RunPID_Increment.h"

extern PIDPARAMETER_INC sMIXVentPar;

int16 Deal_MIXVENTPID(void);
int16 HmixPID_Init(void);

#endif /* MIXVENT_PID_H_ */
