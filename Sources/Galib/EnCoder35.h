/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Encoder35.h
* ����������35������ ģ���ͷ�ļ���
* Ϊ��ģ��������������Ҫ���� comdqueue ģ��
* 
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��7��3��
* 
*/

#ifndef  TREAT_CODE35_H
#define  TREAT_CODE35_H


#include "ComdQueue.h"

#define  MAX_CODE35NUM   3


//������
#define  CODE35_ERROR_NONE        0x00
#define  CODE35_ERROR_HARDWARE    0x01
#define  CODE35_ERROR_KEYCODE     0x80







BOOL Init_Code35(uint8 num ,uint8 *CodeKeyData);
void Treat_Read_Code35(void); 
#endif
