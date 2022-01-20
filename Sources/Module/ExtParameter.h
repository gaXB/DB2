/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：ExParameter.h
* 功能描述：此文件提供标定参数变量的外部声明。只被MoudleDrive。c 调用
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月14日
* 
*/


#ifndef _EXT_PARAMETER_
#define _EXT_PARAMETER_
#include "DataType.h"
#include "CAUTO_Extern.h"
#include "CoolSpeed_PID.h"
#include "PTCPower_PID.h"
#include "HotSpeed_PID.h"
/**************************项目可配置参数****************************************/
//TODO： 在此放入需要标定的参数，除auto


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
