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
#ifndef COOLSPEED_PID_H_
#define COOLSPEED_PID_H_
#include "RunPID.h"

extern PIDPARAMETER sCoolSpeedPar;
extern PIDDATA   sCoolSpeedPidData;
int16 Deal_CoolSpeedPID(void);
int16 CoolSpeed_Init(void);

#endif /* MIXVENT_PID_H_ */
