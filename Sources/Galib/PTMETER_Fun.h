/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�PTMETER_Fun.h
* ������������λ������ͷ�ļ���
* ��ģ��������������Ҫ���� ��Ҫ��ʱ��ģ��
*
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 *
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��6��17��
*
*/ 
#ifndef PTMETER_FUN_H_
#define PTMETER_FUN_H_

#include "DataType.h"


#define  PTMETER_HYSTERESIS_PER   5       //�ز�������ٷֱ�
#define  MAX_PTMETER_NUM          3       //������

//��λ�����ö���
typedef struct
{
	int16   i16StartAD;     //��ʼ��ѹ
	int16   i16EndAD;       //������ѹ
	uint8   MaxLevel;       //���λֵ�����5����λ�ģ����ֵӦΪ4
}PTMETERCONFIG;



BOOL PTMETER_Init(uint8 u8Num, PTMETERCONFIG ConfigSet[]);
void PTMETER_Control(void);
uint8 PTMETER_GetLevel(uint8 u8PTId);
#endif /* PTMETER_FUN_H_ */
