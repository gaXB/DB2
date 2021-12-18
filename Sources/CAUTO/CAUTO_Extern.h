/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CAUTO_Extern.h
* �������������ļ�Ϊ������㷨�Ķ�Ӧ�궨��CAUTO ���ⲿ����
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
#ifndef CAUTO_EXTERN_H_
#define CAUTO_EXTERN_H_
#include "CAUTO.h"
#include "MIXVENT_PID.h"
/**************************cauto �ⲿ��������****************************************/
extern  uint8  TDSunAddTimer, TDSunDecTimer;
//����
extern  int16 TBL_SEGTAMB[];
extern int16 TBL_OFFSETAMB[];

//ktset *10//Ktin *10
extern uint16  u16KTset ,u16KTin, u16KTamb, u16KSun;
//sun offset
extern  uint8 TBL_ADSUN[];
extern  int8 TBL_OFFSETSUN[];

//ac td
extern  int16 TBL_ACTAMB[];
//mode td
extern TdValueType TBL_MODETD[];
extern TdValueType TBL_NCFTD[];               
//�ķ��idֵ
extern  TdValueType TBL_FANTD[];
extern int16 TBL_AFANVOLT[], fVoltBLR_VSMAX;
//�·粹��                          
extern uint8   u8BV_FRESH, u8G_TD, u8T1Per;


//�����б궨�Ĳ���
extern TempType  tLOTamb_ProMode[];
extern TdValueType TBL_TEMPMOTOR_TD[];
extern int16 TBL_TEMPMOTOR_POSTION[];
//���ȷ籣���Ĳ��������б궨
#endif /* CAUTO_EXTERN_H_ */
