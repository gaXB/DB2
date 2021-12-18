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
                      
/**************************项目可配置参数****************************************/
//TODO： 在此放入需要标定的参数，除auto
extern uint8  u8MIXMOTOR_HI_AD, u8MIXMOTOR_LO_AD;   

extern uint8  TBL_MODE_OSET[5]; 
extern uint8 TBL_NCFAD[2];

//蒸发传感器参数
extern TempType   tACON_EVP ,tACOFF_EVP;
extern int16   TBL_FANVOLT[];
#endif
