/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�TempFilter.h
* �����������¶��˲� ����HMIXPID ģ���У����ø���
* 
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��4��2��
* 
*/
#ifndef TEMPFILTER_H_
#define TEMPFILTER_H_


#include "DataType.h"
#define  TEMP_FILTER_TIMES    5
#define  TEMP_FILTER_NUM      2
#define  TEMP_FILTER_DIFFVALUE  20

extern void TempFilterCalBackValue(uint8 u8Num, int16 i16Value);  //�ص�����֪ͨӦ�ó������
uint8 TempFilterTask(int16* pTemp, uint8 num);
#endif /* TEMPFILTER_H_ */
