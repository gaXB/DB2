/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�DealString.h
* ����������string ģ���ͷ�ļ���
* 
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��5��15��
*
* �汾��1.1
* ˵��������GaMemSet
* ԭ���ߣ�xb
* ������ڣ�2019��12��15��
*/

#ifndef DEALSTRING_H_
#define DEALSTRING_H_

#include"DataType.h"

//�ⲿ�ɵ��ú���
void MemCopy(void* dest,const void* Src,uint16 u16Length);
BOOL MemCompareEqual(uint8* buf1,uint8* buf2,uint8 Length);
void GaMemSet(void* S,uint8 u8SetData,uint16 u16Length);
#endif /* DEALSTRING_H_ */
