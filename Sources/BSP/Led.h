/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Led.h
* 
* ���������� LED.h�ļ�������LED�ߵͰ汾�ĵ�
*  
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��13��
* 
* * * �汾��2.0
* ˵����LED ��ʾ�汾�Ź�������
* ԭ���ߣ�lyc
* ������ڣ�2020��5��29��
*/
#ifndef LED_H_
#define LED_H_

#include "APPDrive.h"
#include "SystemType.h"


//�汾�ߵ�λ����
#define  SWVERH_LED   sDisplayData.CIRF  
#define  SWVERL_LED   sDisplayData.AC    


//�ⲿ�ɵ��ú���
void	LED_AllOpen(void);
void	LED_AllClose(void);
void  LedDisVerEn(uint16 u16VerData);
void LEDSetTask(void);
BOOL  LED_GetDisVer(void);
#endif /* LED_H_ */
