/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�HVACMainControl.h
* ����������HVACMainControl��Ͷ�ļ�
* �������� codewarrior IDE 11.1
* MCU˵���� ��MCU�޹أ��������ø���
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.5
* ˵��������.c
* ԭ���ߣ�xb
* ������ڣ�2021��4��6��
*
*/
#ifndef HVACMAINCONTROL_H_
#define HVACMAINCONTROL_H_
#include "APPDrive.h"


#define  _APPCONFIG_FANDISPLAY_      1      //������ʾ������� Ϊ1�� ��ʾ��������Ϊ0



void Run_Park_Task(void);
void Run_Remote_Task(void);
void Run_Normal_Task(void);
void HVACControl_Init(void);	
#endif /* HVACMAINCONTROL_H_ */
