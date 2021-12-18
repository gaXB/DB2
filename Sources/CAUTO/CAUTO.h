/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：CAUTO.h
* 功能描述：AUTO 模块的头文件
* 
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2021年3月8日
* 
*/
#ifndef CAUTO_H_
#define CAUTO_H_
#include "APPDrive.h"


#define   _AFANVOLT_SAME_    1     //鼓风机电压和挡位是否和自动得一样， 一样则为1
#define   _CFAN_PROTECT_     1     //3：上汽的，1：基本版   0：不用
#define   _HFAN_PROTECT_     1     //1：基本版的   0：不用
#define   TADT_DIFF_LOHI      SCD_T(15)   //冷风保护用的温差系数，不用改
#define   CFAN_SEG4_PERVOLT   SCD_V(0.2)  //冷风保护时风量上升速度
#define   HMIX_VENTPID_EN      1   //1 带出风口温度 0 用查表得到不同TD对应的混合值
#define    AUTO_BASE_TSET      SCD_T(24)  //td值计算的基准值



typedef int16 TdValueType;
typedef int16 AutoFanVoltType;
typedef struct
{
	int16 TDResult;
	int16 i16TDRamb;
	int16 i16TDRsun;
	int16 i16TDTin;
	int16 i16TDTset;
}TDDATA;

typedef struct
{
	uint8 AC;
	uint8 NCF;
	uint8 MODE;
	AutoFanVoltType aFanVolt;   //电压×100
	int16 i16Hmix;       //混合千分比
	uint8  CFanState;
	uint8  HFanState;
}AUTORESULT;
 

typedef enum
{
	AUTOTD_TYPE_AC = 0,
	AUTOTD_TYPE_NCF,
	AUTOTD_TYPE_MODE,
	AUTOTD_TYPE_MIX,
	AUTOTD_TYPE_FAN,
}AUTOTYPE;



#define   _TDVALUE(data)   ((int16)((data)*10))


#if _AFANVOLT_SAME_
extern int16  TBL_FANVOLT[];
#define TBL_AFANVOLT  TBL_FANVOLT
#else
#endif



int16 AUTOTD_GetResult(AUTOTYPE u8TDType);
void AUTOTD_Init(void);
void AUTOTD_Control(void);
TDDATA AUTOTD_GetTD(void);
void  Cal_TD(void);
AUTORESULT AUTOTD_GetARes(void);
#endif /* CAUTO_S3_H_ */
