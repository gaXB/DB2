/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Ht16k23_Drive.h
* ����������ht16k23 ģ���ͷ�ļ���
* Ϊ��ģ��������������Ҫ���� IIC ģ��
* 
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��6��2��
* 
* �汾��1.1
* ˵��������.c
* ԭ���ߣ�xb
* ������ڣ�2020��6��2��
*/

#ifndef  Ht16k23_Drive_
#define  Ht16k23_Drive_
#include "DataType.h"


#define   HT16K23_MAXLCDNUM      10       //seg0-19
/************************************************************************
 * ht16k23ָ�
 *
*************************************************************************/
#define   SLAVE_WRITE   0xe6//д�ӻ�
#define   SLAVE_READ    0xe7//���ӻ�
#define   NORMAL_DISPLAY_ON    0x83//��������������ʾ����
#define   STANDBY_DISPLAY_OFF  0x80//����������ʾ����
#define   MODE_SEGMENGT        0xa0//����жϲɼ�����
#define   LCD_FIRST_ADDRESS    0x00//LCD�Ĵ����׵�ַ
#define   INT_REG_ADDRESS      0x30//�����жϼĴ�����ַ
#define   KEY_FRIST_ADDRESS    0x20//�������ݼĴ����׵�ַ
#define   KEY_SCAN_PERIOD      0xfc//���ð���ɨ������
              
//��������˵��        
BOOL Ht16k23_LCDDataSet(uint8* u8LCDData, uint8 u8Length);                
void  Ht16k23_Updata(void);
void  Ht16k23_Read_Key(uint32* pKeyData);
void  Ht16k23_Clear(void);

#endif
