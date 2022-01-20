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
#ifndef PTCPOWER_PID_H_
#define PTCPOWER_PID_H_
#include "RunPID.h"

extern PIDPARAMETER sPTCPowerPar;
extern PIDPARAMETER sBatPTCPowerPar;
extern PIDDATA   sPTCPowerPidData;
int16 Deal_PTCPowerPID(void);
int16 PTCPower_Init(void);
void  PTCPowerPIDMode(uint8 bBathMode);
#endif /* MIXVENT_PID_H_ */
