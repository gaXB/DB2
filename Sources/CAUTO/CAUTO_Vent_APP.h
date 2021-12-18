/************************文件说明*****************************************************
* Copyright (c) 2021 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：CAUTO_Vent_APP.h
* 功能描述：本文件为出风口算法的对应标定的CAUTO 的参数变量的声明
* 
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：与MCU无关
* 注意此文件只能被CAUTO_Vent.c 包含
*/
/**************************更改记录******************************************************
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2021年3月8日
* 
*/

#include "CAUTO.h"


//parameter
//阳光渐变参数
uint8   TDSunAddTimer = 40, TDSunDecTimer = 40;		//  tdsun每0.1*data s 更新+-1

//外温分段 及对应的补偿值
#define  TD_TAMBSEG_MAXNUM   10
TempType TBL_SEGTAMB[TD_TAMBSEG_MAXNUM] = 
{
		SCD_T(-30),SCD_T(-20), SCD_T(-10), SCD_T(-5), SCD_T(0), 
		SCD_T(5), SCD_T(10), SCD_T(20), SCD_T(30),SCD_T(40),	
};

//Kamb
TdValueType TBL_OFFSETAMB[TD_TAMBSEG_MAXNUM]=
{
	_TDVALUE(-3), _TDVALUE(-2), _TDVALUE(-2), _TDVALUE(-1), _TDVALUE(0), 
	_TDVALUE(0), _TDVALUE(0), _TDVALUE(0), _TDVALUE(1), _TDVALUE(2), 
};

//ktset ，ktin， kamb ksun
uint16  u16KTset = 25 ,u16KTin = 13, u16KTamb = 6 ,u16KSun = 20;    //最终需要/10 

//查表根据sSenSorData.u8ADSunR 值到 sTDData.i16TDRsun， TD公式中还要 i16TDRsun*u16KSun
#define  TD_SUNSEG_MAXNUM   6
int8 TBL_OFFSETSUN[TD_SUNSEG_MAXNUM]=
{
	50, 40, 30, 15, 5, 0
};

uint8 TBL_ADSUN[TD_SUNSEG_MAXNUM] = 
{
	100, 140, 165, 180, 210, 230
};

//ac 外温开启条件
TempType TBL_ACTAMB[2]=
{
SCD_T(-1), SCD_T(1)
}  ;

//低温保护 对模式的补偿
//低温吹脚除霜 低温吹脚的温度值
TempType  tLOTamb_ProMode[2] ={SCD_T(-30), SCD_T(-27)};
//模式优先级  冷风保护-》热风保护-LOHI->低温保护-》自动控制

//模式TD控制
TdValueType TBL_MODETD[4]=
{
	_TDVALUE(-2), _TDVALUE(-1), _TDVALUE(2), _TDVALUE(3)	
};

//循环Td控制
TdValueType TBL_NCFTD[2]=
{
	_TDVALUE(5), _TDVALUE(9)	
} ;

//鼓风机td值
TdValueType TBL_FANTD[16]=
{
		_TDVALUE(-30), _TDVALUE(-25), _TDVALUE(-20), _TDVALUE(-12), _TDVALUE(-10), _TDVALUE(-8), _TDVALUE(-5), _TDVALUE(-2), 
		_TDVALUE(2), _TDVALUE(4), _TDVALUE(6), _TDVALUE(8), _TDVALUE(9), _TDVALUE(14), _TDVALUE(16), _TDVALUE(18), 
} ;


//TD对应混合 没有出封口温度传感器时使用
#define  TD_MIXSEG_MAXNUM   11  
TdValueType  TBL_TEMPMOTOR_TD[TD_MIXSEG_MAXNUM] =     //理论位置
{
		_TDVALUE(-30), _TDVALUE(-25), _TDVALUE(-15), _TDVALUE(-10), _TDVALUE(-5), _TDVALUE(0),
		_TDVALUE(5), _TDVALUE(10), _TDVALUE(15), _TDVALUE(20), _TDVALUE(25)
} ;

int16  TBL_TEMPMOTOR_POSTION[TD_MIXSEG_MAXNUM] = //实际位置
{
	1000, 900, 800, 700, 600, 500, 400, 300, 200, 100, 0	//
} ;//位置%

//鼓风机电压参数，自动风量和实际风量不一样时才有效
#if _AFANVOLT_SAME_
#else
AutoFanVoltType  TBL_AFANVOLT[9] = {0 , 390, 490, 590, 680, 790, 890, 1090, 1300};
#endif

//风量车速优化
uint8   u8BV_FRESH = 104;
AutoFanVoltType fVoltBLR_VSMAX = 200;


//出风口参数配比
uint8 u8T1Per = 50;   // %
uint8 u8G_TD = 17;   // /10 sSenSorData.Tddt = SCD_T(25) - sTDData.TDResult * u8G_TD /10;


//冷风保护参数，标准版本才有
TempType  Tamb_STHOT = SCD_T(20); 
TempType  Tadt_Lo1 = SCD_T(40), Tadt_Lo2 = SCD_T(25);
AutoFanVoltType fVoltBLR_1A = SCD_V(4);
AutoFanVoltType fVoltBLR_2A = SCD_V(5);
uint8 u8HOTSEQ_1 = 30;
//上汽的冷风保护参数
AutoFanVoltType TBL_CFAN[5] = {380, 640, 860, 1130, 1350};
//热风保护参数值
TdValueType TD_STHOT = _TDVALUE(-4);
TempType Tamb_STCOOL = SCD_T(30);
TdValueType TD_STCOOL = _TDVALUE(5); 
uint8 u8COOLSEQ_1 = 20, u8COOLSEQ_2 = 20;
