/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�EncoderESE24.h
* ����������ESE24������ ģ���ͷ�ļ���
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
#ifndef  TREAT_CODEESE24_H
#define  TREAT_CODEESE24_H

#include "ComdQueue.h"


#define  MAX_CODEESE24NUM   3


//������
#define  CODEESE24_ERROR_NONE        0x00
#define  CODEESE24_ERROR_HARDWARE    0x01
#define  CODEESE24_ERROR_KEYCODE     0x80





BOOL Init_CodeESE24(uint8 num, uint8 *CodeKeyData);
void Treat_Read_CodeESE24(void); 
#endif
