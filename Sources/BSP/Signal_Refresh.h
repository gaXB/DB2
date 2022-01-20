/************************�ļ�˵��*****************************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Signal_Refresh.h
* �����������ź�ˢ��ģ��ͷ�ļ�
*
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹أ���Ϊģ����Ҫ����ʵ����ĿӦ���������
*/
/**************************���ļ�¼******************************************************
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��19��
* 
* �汾��1.1
* ˵��������.c
* ���ߣ�xb
* ������ڣ�2020��7��6��
* 
* �汾��1.2
* ˵��: ��SIG_NUM_XXX ���ڴ˴�
* ���ߣ�xb
* ������ڣ�2020��10��22��
*/
#ifndef SIGNAL_REFRESH_H_
#define SIGNAL_REFRESH_H_
#include "APPDrive.h"


//TODO: ��SIGFLITER �ı�Ŷ���
#define  SIG_NUM_TIN       0
#define  SIG_NUM_TAMB      1 
#define  SIG_NUM_TEVP      2
#define  SIG_NUM_VENTF     3
#define  SIG_NUM_VENTD     4
#define  SIG_NUM_PT1TEMP   5
#define  SIG_NUM_PT2TEMP   6
#define  SIG_NUM_PT3TEMP   7
#define  SIG_NUM_PT1PRESS  8
#define  SIG_NUM_PT2PRESS  9
#define  SIG_NUM_PT3PRESS  10
#define  SIG_NUM_PTCTEMP   11
#define  SIG_NUM_RHEAT_FB  12
#define  SIG_NUM_SUN       13
#define   SIG_MAXNUM       14 //������������Ӧ�ó����ʼ��ʱʹ��


void Deal_Signal_Refresh(void);
void SigRefresh_IgnON(void);
void SigRefresh_IgnOFF(void);
void SignalCAN_RXComplete(uint32 u32Id);
void SignalCAN_RecTimeOut(uint32 u32Id);

#endif /* SIGNAL_REFRESH_H_ */
