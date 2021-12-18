/************************�ļ�˵��*****************************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Signal_TempGet.h
* �����������¶��ź����ݵõ��ĸ�������
*
* �������� codewarrior IDE 11.1
* MCU˵���� ��
*/
/**************************���ļ�¼******************************************************
��
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
* 
* * * �汾��2.0
* ˵��������.c
* ���ߣ�xb
* ������ڣ�2020��7��6��
* 
*/
#ifndef SIGNAL_TempGet_H_
#define SIGNAL_TempGet_H_
#include "ADTreat.h"

typedef struct
{
	const uint16 *tbl_tad;       //��
	uint8 u8TBLLength;           //��
	ADTempType atOffsetTemp;     //����¶�
	ADTempType atAccurate;       //����
}ADTGETTYPE;


ADTempType Signal_GetAdT (uint16 adcal, ADTGETTYPE *pADTemp);
uint16 Signal_GetAd_Press(uint8 hrol,uint16 adcal);
#endif /* SIGNAL_REFRESH_H_ */
