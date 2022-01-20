/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�BU9796_Drive.h
* ����������BU9796 ģ���ͷ�ļ���
* Ϊ��ģ��������������Ҫ���� IIC ģ��
* 
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xwl
* ������ڣ�2021��10��21��
*/

#ifndef  BU9796_DRIVE_
#define  BU9796_DRIVE_
#include "DataType.h"


#define   BU9796_MAXLCDNUM      10       //seg0-19
/************************************************************************
 * ht16k23ָ�
 *
*************************************************************************/
#define   SLAVE_ADDRESS    0x7c//�ӻ���ַ
#define   ICSET_RESET       0xea//Execute Software Reset
#define   ICSET     				0xe8//Execute internal OSC mode
#define   BLINK_CONTROL      0xf0//Set BLKCTL
#define   DISPLAY_CONTROL    0xa7//Set Display Control
#define   AP_CONTROL    0xfc//Set APCTL
#define   LCD_FIRST_ADDRESS    0x00//LCD�Ĵ����׵�ַ

#define   MODE_SET_ON    0xc8//��������������ʾ����
#define   MODE_SET_OFF    0xc0//����ʾ����

              
//��������˵��        
BOOL BU9796_LCDDataSet(uint8* u8LCDData, uint8 u8Length);                
void  BU9796_Updata(void);
void  BU9796_StandbyMode(void);
void  BU9796_Clear(void);
#endif
