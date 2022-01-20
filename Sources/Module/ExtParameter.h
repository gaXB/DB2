/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ExParameter.h
* �������������ļ��ṩ�궨�����������ⲿ������ֻ��MoudleDrive��c ����
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��14��
* 
*/


#ifndef _EXT_PARAMETER_
#define _EXT_PARAMETER_
#include "DataType.h"
#include "CAUTO_Extern.h"
#include "CoolSpeed_PID.h"
#include "PTCPower_PID.h"
#include "HotSpeed_PID.h"
/**************************��Ŀ�����ò���****************************************/
//TODO�� �ڴ˷�����Ҫ�궨�Ĳ�������auto


//extern int
extern int16 TBL_COOLInitSpeed[3];
extern int16 i16SignalCarMaxSpeed;
extern int16 i16EXVCharge, i16EXVDrive, i16EXVMin;
extern uint8 u8EXVAdjStep, u8EXVAdjTime;
extern uint16 Adj3wayTime;
extern uint8 u8Bat3WayAdjStep;

extern int16 i16BatCInitSpeed;
extern int16 i16BothCoolMaxSpeed;
extern TempType   tACON_EVP ,tACOFF_EVP;

//humidef
extern int16 TBL_CoolDefInitSpeed[4], TBL_HotDefInitSpeed[3], TBL_HotDefInitPTC[3];
extern int16 TBL_CoolDefMix[4], TBL_HotDefMix[3];

//hp
extern int16 TBL_HPInitSpeed[3], i163Way2Min;
//wptc
extern int16 i16BatPTC_InitPower, i16BatPTC_MaxPower, TBLWPTC_InitPower[3];
//paremeter
extern int16 Parameter_A, Parameter_B, Parameter_C, Parameter_D;
extern int16 i16BEXVCool,  TBL_BEXV_HOTX[2], TBL_BEXV_HOTY[2];

//
extern int16 i16SignalCarMaxSpeed_HP, i16BatHInitPSpeed, i16BatHMaxSpeed, i16BothMaxSpeed_HP;
extern int16 HPreePro_T1;

//hvac
extern uint8  u8MIXMOTOR_HI_AD, u8MIXMOTOR_LO_AD;
extern uint8  TBL_MODE_OSET[5];
extern uint8 TBL_NCFAD[2];
extern int16   TBL_FANVOLT[];
#endif
