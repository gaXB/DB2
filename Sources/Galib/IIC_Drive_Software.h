/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�IIC_Drive_Software.h
* ��������: 	iic ����ģ���ͷ�ļ�����Ҫ���� Galib_MCUComm.h �ļ�
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
*/

#ifndef   IIC_DRIVE_SOFTWARE_
#define   IIC_DRIVE_SOFTWARE_

#include "Galib_MCUComm.h"
#include "DataType.h"





#define   IIC_DELAY  2  //��Ƭ������ʱ�Ӵ���8Mʱ�����������ֵ

#define   ACK_YES  1  //������Ӧ�ź�
#define   ACK_NO   0  //���Ͳ���Ӧ�ź�



//�ⲿ�ɵ��ú���
void IIC_Start_Bit(void);
void IIC_Stop_Bit(void);
void IIC_Send_Ack(uint8 data);
uint8 IIC_Send_Byte(uint8 data);
uint8 IIC_Receive_Byte(void);
#endif
