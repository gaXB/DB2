/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Encoder28.h
* ����������28������ ģ���ͷ�ļ���
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
* ������ڣ�2019��5��24��
* 
*/

#ifndef  TREAT_CODE28_H
#define  TREAT_CODE28_H

#include "ComdQueue.h"


#define  MAX_CODE28NUM   3


//������
#define  CODE28_ERROR_NONE        0x00
#define  CODE28_ERROR_HARDWARE    0x01
#define  CODE28_ERROR_KEYCODE     0x80





BOOL Init_Code28(uint8 num, uint8 *CodeKeyData);
void Treat_Read_Code28(void); 
#endif
