/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ICDOut.h
* ���������� ���ļ����ø���
*
* �������� codewarrior IDE 11.1
* MCU˵������MCU �޹�
*  
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��4��2��
* 
* 
*/
#ifndef OUT_H_
#define OUT_H_

#include "APPDrive.h"

void TrOut_5VEN(uint8 en);
void	OUT_AllClose(void);
void	OUT_AllOpen(void);
void OutTask(void);
#endif /* OUT_H_ */
