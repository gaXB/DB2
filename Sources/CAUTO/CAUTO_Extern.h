/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：CAUTO_Extern.h
* 功能描述：本文件为出风口算法的对应标定的CAUTO 的外部变量
* 
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年4月2日
* 
*/
#ifndef CAUTO_EXTERN_H_
#define CAUTO_EXTERN_H_
#include "CAUTO.h"
#include "MIXVENT_PID.h"
/**************************cauto 外部变量声明****************************************/
extern  uint8  TDSunAddTimer, TDSunDecTimer;
//外温
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
//鼓风机id值
extern  TdValueType TBL_FANTD[];
extern int16 TBL_AFANVOLT[], fVoltBLR_VSMAX;
//新风补偿                          
extern uint8   u8BV_FRESH, u8G_TD, u8T1Per;


//不进行标定的参数
extern TempType  tLOTamb_ProMode[];
extern TdValueType TBL_TEMPMOTOR_TD[];
extern int16 TBL_TEMPMOTOR_POSTION[];
//冷热风保护的参数不进行标定
#endif /* CAUTO_EXTERN_H_ */
