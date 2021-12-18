/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SystemType.h
* 功能描述：认为系统使用的类型定义包括：
 *  1 温度，TD,风量的type
 *  2 系统控制值
 *  3 传感器数据
 *  4 其他控制类型
 *  5 显示输出类型
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关.
* 此文件会为系统数据结构的定义，请按照系统应用要求进行设计
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
* 
* 版本：1.1
* 说明：增加输入输出强制功能
* 作者：xb
* 完成日期：2020年5月30日
* 
* 版本：1.2
* 说明：1 TdValueType 类型放在auto模块
* 2 风量由FanModeType 改为uint8 类型
* 作者：xb
* 完成日期：2021年8月23日
*/

#ifndef SYSTEMTYPE_H_
#define SYSTEMTYPE_H_

#include "DataType.h"

typedef int16  TempType;                //温度类型
typedef int16  VoltType;                //电压类型

//类型定义
typedef enum       	//模式电机 控制模式常数定义
{
  MD_MODE_OSF=0,
  MD_MODE_OSFD ,
  MD_MODE_OSD,
  MD_MODE_OSDT,
  MD_MODE_OST,
  MD_MODE_MAXOST,
}MDModeType;


//风量 控制模式常数定义

#define  FAN_MODE_OFF  0
#define  FAN_MODE_1L   1
#define  FAN_MODE_2L   2
#define  FAN_MODE_3L   3
#define  FAN_MODE_4L   4
#define  FAN_MODE_5L   5
#define  FAN_MODE_6L   6
#define  FAN_MODE_7L   7
#define  FAN_MODE_8L   8

typedef enum {									//内外风门控制模式常数定义
	NCF_MODE_NEW=0,
//	NCF_MODE_MID,
	NCF_MODE_CIRF,
}NcfModeType;


typedef enum {									//ac控制模式常数定义
	AC_MODE_OFF=0,
	AC_MODE_ON,
}ACModeType;

//ACMAX
typedef enum{          
	AC_MAX_MODE_OFF=0,
	AC_MAX_MODE_ON,
}ACMaxType;  

typedef enum {									//RDEF控制模式常数定义
	RDEF_MODE_OFF=0,
	RDEF_MODE_ON,
}RdefModeType;

typedef enum {									//XSTSET控制模式常数定义
   MIX_MODE_OFF=0,
	MIX_MODE_AUTO,
	MIX_MODE_INVALID,
}MixModeType;


typedef enum {									//XSTSET控制模式常数定义
   OFF_STATE_ON =0,
	OFF_STATE_OFF = 1,
}OFFStateType;


 typedef enum {									//阀
   SOV_OFF=0,
   SOV_ON,
}SOVType;


// auto 状态变量
typedef struct
{
   unsigned char AC       :1;
   unsigned char MODE     :1;
   unsigned char NCF      :1;
   unsigned char FAN      :1;
} AUTOBIT;                 //各个控制方式是否为 auto 1: 是  0： 不

//驻车时，AUTO 状态变量
typedef struct
{
   unsigned char Park_NCF      :1;
   unsigned char Park_FAN      :1;
} Park_AUTOBIT;                 //各个控制方式是否为 auto 1: 是  0： 不

typedef union
{
	struct
	{
		unsigned char   MP5   :1;
		unsigned char   TBOX  :1;
		unsigned char   Press :1;
		unsigned char   B5    :5;
	}Bits;
	uint8 Byte;
}HVACCONFIG;

typedef enum
{
	PHACMODE_OFF = 0,
	PHACMODE_ACCOOLING = 1,
	PHACMODE_BATTCOOLING = 2,
	PHACMODE_BOTHCOOLING = 3,
	PHACMODE_HPAC = 4,
	PHACMODE_PTC = 5,
	PHACMODE_DEHUMID = 6,
	PHACMODE_DEICE1 = 7,
	PHACMODE_DEICE2 = 8,
	PHACMODE_DEICE3 = 9,
}TYPE_PHACMODE;  //0：未工作 ；1：A/C Cooling，2：Battery Cooling3：Both Cooling 4：HPAC heating 5：PTC heating 6：Dehumidification 7： De-Icing（电池冷却循环化冰（化冰模式1））8：除湿化冰（化冰模式2）9：制冷化冰（化冰模式3）

typedef enum
{
	TSETTEM_MODE_DEFAULT = 0,
	TSETEM_MODE_JIAZHU = 1,
	TSETTEM_MODE_WATERWAY_DEBUG = 2,
	TSETTEM_MODE_NULL_3 = 3,
	TSETTEM_MODE_COOL_1 = 4,
	TSETTEM_MODE_COOL_2 = 5,
	TSETTEM_MODE_COOL_3 = 6,
	TSETTEM_MODE_NULL_7 = 7,
	TSETTEM_MODE_COOLDEF_1 = 8,
	TSETTEM_MODE_COOLDEF_2 = 9,
	TSETTEM_MODE_COOLDEF_3 = 10,
	TSETTEM_MODE_COOLDEF_4 = 11,
	TSETTEM_MODE_HOTDEF_1 = 12,
	TSETTEM_MODE_HOTDEF_2 = 13,
	TSETTEM_MODE_HOTDEF_3 = 14,
	TSETTEM_MODE_NULL_15 = 15,
	TSETTEM_MODE_HPMODE_1 = 16,
	TSETTEM_MODE_HPMODE_2 = 17,
	TSETTEM_MODE_HPMODE_3 = 18,
	TSETTEM_MODE_NULL_19= 19,
	TSETTEM_MODE_WPTC_1 = 20,
	TSETTEM_MODE_WPTC_2 = 21,
	TSETTEM_MODE_WPTC_3 = 22,
	TSETTEM_MODE_DEBUG = 23,
}TESTMMODE;


typedef enum
{
	SYSTEM_MODE_DEFAULT = 0,
	SYSTEM_MODE_JIAZHU = 1,
	SYSTEM_MODE_WATERWAY_DEBUG =2,
	SYSTEM_MODE_CARCOOL = 3,
	SYSTEM_MODE_BATTCOOL = 4,
	SYSTEM_MODE_BOTHCOOL = 5,
	SYSTEM_MODE_COOLDEF1 = 6,
	SYSTEM_MODE_HOTDEF2 = 7,
	SYSTEM_MODE_HPMODE = 8,
	SYSTEM_MODE_BAT_HPMODE = 9,
	SYSTEM_MODE_BAT_WPTC = 10,
	SYSTEM_MODE_BAT_HOT = 11,
	SYSTEM_MODE_WPTC = 12,
	SYSTEM_MODE_BOTH_HPMODE = 13,
	SYSTEM_MODE_BOTH_WPTC = 14,
	SYSTEM_MODE_DEBUG = 15,
	SYSTEM_MODE_HOTDEF2_BATHOT = 16,
}SYSTEMMODE;

typedef enum
{
	LEVELMODE_DEFAULT = 0,
	LEVELMODE_COOL = 1,
	LEVELMODE_COOLDEF =2,
	LEVELMODE_HOTDEF = 3,
	LEVELMODE_HPMODE = 4,
	LEVELMODE_WPTC = 5,
	LEVELMODE_DEBUG = 6,
}LevelModeType;

typedef enum
{
	THEMAL_MODE_NULL = 0,
	THEMAL_MODE_COOL = 0x1,
	THEMAL_MODE_HOT = 0x2,
	THEMAL_MODE_BOTH = 3,
}ThemalModeType;

 //系统主要控制类型结构体定义
typedef struct
{
	TempType      tset;                // 设置温度
	ACModeType    acMode;              // ac控制方式状态 
	NcfModeType   ncfMode;             // 内外控制方式 
	uint8   fanMode;             // 风量控制
	MDModeType    mdMode;              // 模式电机
	OFFStateType  OffState;            // 系统是否为off 状态 1：为off  0： 非off
	AUTOBIT       AutoBits;            // 自动控制变量结构
	RdefModeType  RdefMode;            // 后除霜控制 
   uint8         MaxACMode;           // MAXac状态          :1：MAX；       0：非MAX；
   uint8         MaxDefMode;          // MAX前除霜状态    :1：MAX；       0：非MAX；
   uint8         ParkAC;              // 驻车指示灯
   uint8         u8TestLevel;
   uint8         ptcMode;
   uint8         WaterWay1;          //1-2 2-3 是50%    1-2 是 0%  1-3 是100%
   uint8         WaterWay2;
   uint8         FourWay;            // 0：1-2，3-4   1：1-4， 2-3
   SYSTEMMODE    systemMode;
   TESTMMODE     TestMode;            //实际运行level
   LevelModeType     LevelMode;             //level 对应的模式
   ThemalModeType    u8CompressMode;      //0 关   1 制冷  2 制热
   ThemalModeType    thBathMode;
   ThemalModeType    thCarMode;
}SYSTEMCONTROL ;

typedef struct
{
	BOOL bBattCoolReq;
	BOOL bBattHotReq;
	int16 i16PowerLimte;
	int16 i16BackEACSpeed;
	int16 i16SpeedLimte;
	int16 i16PTCPowerLimte;
	int16 i16PTCPower;
	int16 i16COMPPower;
}VEHICLECONTROL;

//actor struct
typedef struct
{
	uint8 EWP2Error;
	uint8 EXVError;
}ACTORDATA;
//sensor struct
typedef struct
{
	TempType TambCal;        //用于显示的外温
	TempType TinCal;
	TempType TevpCal;
	TempType TwaterCal;    //水温应采用两个字节	
	uint8 u8ADSunR;
	uint8 u8VehicleSpeed;
	TempType TVentF;
	TempType TVentD;
	TempType Tadt;           //出风口温度参考值
	TempType Tddt;           //目标出风口温度
	TempType TambCal_TD;     //用于td计算的外温
	TempType TinSimul;
}SENSORDATA;


typedef struct
{
	TempType RHTemp;
	int16 RHHumidity;
	TempType TPH_Temp;
	int16 TPH_Press;
	TempType TPL1_Temp;
	int16 TPL1_Press;    //pa
	TempType TPL2_Temp;
	int16 TPL2_Press;
	TempType BattInWater;
	TempType BattOutWater;
	TempType BattTargetWater;
	TempType DCDCbodyTemp;
	TempType CurrentMotorTemp;
	TempType CurrentIGBTemp;
	TempType OBCtemp;
}EVSENSORDATA;
//输出数据类型
typedef struct
{
	BOOL  ACOut;            //压缩机请求    0： 无请求  1 : 有
	VoltType FANVolt;          //风量 输出电压
	uint8 NCFAD;           //作为内外电机时 1：内循环 0 外循环
	uint8 MODEAD;
	uint8 MIXAD;
	BOOL  RHeatOut;
	uint8 u8RCheat;
	uint8 u8LCheat;
}OUTDATA;

//输出数据类型
typedef struct
{
	uint16  ThreeWay1_Postion;            //LIN
	uint16  ThreeWay2_Postion;            //LIN
	uint16   FourWay_Postion;             //LIN 1/0  0：1-2，3-4   1：1-4， 2-3
	uint16  BEXV_Postion;
	uint16  EXV_Postion;
	uint16  AGS_Postion;
	uint8 SOV1;
	uint8 SOV2;
	uint8 EWPT1;
	uint8 EWPT2;
	uint8 EWPT3;
	uint16 EACSpeed;
	uint16 PTCPower;
	uint8  IncarFAN;      //0 关， 1 低速， 2 高速
	uint8  APTC;
	//uint8  bACStart;
	BOOL ThemalSysRequest;
	BOOL  HeatConfirmation;
	BOOL  CoolConfirmation;
}EVOUTDATA;

//显示数据类型
typedef struct
{
	uint8 LEDMode;          //0: OFF    1: 显示     2： 变暗模式
	uint8 LCDMode;          //0: OFF    1: 显示
	uint8 LCDDiagMode;      //lCD 自检
	int16 i16Temp;          //温度区显示，-400 ~800 为直接显示， 10000 ：HI
	BOOL AC;
	MDModeType VentMode;         //吹风模式,同systemcontrol.mdMdoe
	uint8 CIRF;
	uint8 NEW;
	uint8 FANLevel;
	BOOL AUTO;
	BOOL MODE_MAXDEF;
	BOOL MAXAC;
	BOOL RHEAT;
	uint8 RCheat;
	uint8 LCheat;
}DISPLAYDATA; 

#if _PROJECT_CAN_
typedef union
{
	struct
	{
		unsigned b0 :1;
		unsigned b1 :1;
		unsigned b2 :1;
		unsigned b3 :1;
		unsigned b4 :1;
		unsigned b5 :1;
		unsigned b6 :1;
		unsigned b7 :1;
		unsigned b8 :1;
		unsigned b9 :1;
		unsigned b10 :1;
		unsigned b11 :1;
		unsigned b12 :1;
		unsigned b13 :1;
		unsigned b14 :1;
		unsigned b15 :1;
		unsigned b16 :1;
		unsigned b17 :1;
		unsigned b18 :1;
		unsigned b19 :1;
		unsigned b20 :1;
		unsigned b21 :1;
		unsigned b22 :1;
		unsigned b23 :1;
		unsigned b24 :1;
		unsigned b25 :1;
		unsigned b26 :1;
		unsigned b27 :1;
		unsigned b28 :1;
		unsigned b29 :1;
		unsigned b30 :1;
		unsigned b31 :1;
		unsigned b32 :1;
		unsigned b33 :1;
		unsigned b34 :1;
		unsigned b35 :1;
		unsigned b36 :1;
		unsigned b37 :1;
		unsigned b38 :1;
		unsigned b39 :1;
		unsigned b40 :1;
	}Bits;	
	uint8 Data[10];
}PCCONTROLEN_TYPE;
#endif

extern LevelModeType GetTSETLevelMode(uint8 level);
#endif /* SYSTEMTYPE_H_ */
