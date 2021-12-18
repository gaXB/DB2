/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SystemType.h
* ������������Ϊϵͳʹ�õ����Ͷ��������
 *  1 �¶ȣ�TD,������type
 *  2 ϵͳ����ֵ
 *  3 ����������
 *  4 ������������
 *  5 ��ʾ�������
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�.
* ���ļ���Ϊϵͳ���ݽṹ�Ķ��壬�밴��ϵͳӦ��Ҫ��������
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
* 
* �汾��1.1
* ˵���������������ǿ�ƹ���
* ���ߣ�xb
* ������ڣ�2020��5��30��
* 
* �汾��1.2
* ˵����1 TdValueType ���ͷ���autoģ��
* 2 ������FanModeType ��Ϊuint8 ����
* ���ߣ�xb
* ������ڣ�2021��8��23��
*/

#ifndef SYSTEMTYPE_H_
#define SYSTEMTYPE_H_

#include "DataType.h"

typedef int16  TempType;                //�¶�����
typedef int16  VoltType;                //��ѹ����

//���Ͷ���
typedef enum       	//ģʽ��� ����ģʽ��������
{
  MD_MODE_OSF=0,
  MD_MODE_OSFD ,
  MD_MODE_OSD,
  MD_MODE_OSDT,
  MD_MODE_OST,
  MD_MODE_MAXOST,
}MDModeType;


//���� ����ģʽ��������

#define  FAN_MODE_OFF  0
#define  FAN_MODE_1L   1
#define  FAN_MODE_2L   2
#define  FAN_MODE_3L   3
#define  FAN_MODE_4L   4
#define  FAN_MODE_5L   5
#define  FAN_MODE_6L   6
#define  FAN_MODE_7L   7
#define  FAN_MODE_8L   8

typedef enum {									//������ſ���ģʽ��������
	NCF_MODE_NEW=0,
//	NCF_MODE_MID,
	NCF_MODE_CIRF,
}NcfModeType;


typedef enum {									//ac����ģʽ��������
	AC_MODE_OFF=0,
	AC_MODE_ON,
}ACModeType;

//ACMAX
typedef enum{          
	AC_MAX_MODE_OFF=0,
	AC_MAX_MODE_ON,
}ACMaxType;  

typedef enum {									//RDEF����ģʽ��������
	RDEF_MODE_OFF=0,
	RDEF_MODE_ON,
}RdefModeType;

typedef enum {									//XSTSET����ģʽ��������
   MIX_MODE_OFF=0,
	MIX_MODE_AUTO,
	MIX_MODE_INVALID,
}MixModeType;


typedef enum {									//XSTSET����ģʽ��������
   OFF_STATE_ON =0,
	OFF_STATE_OFF = 1,
}OFFStateType;


 typedef enum {									//��
   SOV_OFF=0,
   SOV_ON,
}SOVType;


// auto ״̬����
typedef struct
{
   unsigned char AC       :1;
   unsigned char MODE     :1;
   unsigned char NCF      :1;
   unsigned char FAN      :1;
} AUTOBIT;                 //�������Ʒ�ʽ�Ƿ�Ϊ auto 1: ��  0�� ��

//פ��ʱ��AUTO ״̬����
typedef struct
{
   unsigned char Park_NCF      :1;
   unsigned char Park_FAN      :1;
} Park_AUTOBIT;                 //�������Ʒ�ʽ�Ƿ�Ϊ auto 1: ��  0�� ��

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
}TYPE_PHACMODE;  //0��δ���� ��1��A/C Cooling��2��Battery Cooling3��Both Cooling 4��HPAC heating 5��PTC heating 6��Dehumidification 7�� De-Icing�������ȴѭ������������ģʽ1����8����ʪ����������ģʽ2��9�����仯��������ģʽ3��

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

 //ϵͳ��Ҫ�������ͽṹ�嶨��
typedef struct
{
	TempType      tset;                // �����¶�
	ACModeType    acMode;              // ac���Ʒ�ʽ״̬ 
	NcfModeType   ncfMode;             // ������Ʒ�ʽ 
	uint8   fanMode;             // ��������
	MDModeType    mdMode;              // ģʽ���
	OFFStateType  OffState;            // ϵͳ�Ƿ�Ϊoff ״̬ 1��Ϊoff  0�� ��off
	AUTOBIT       AutoBits;            // �Զ����Ʊ����ṹ
	RdefModeType  RdefMode;            // ���˪���� 
   uint8         MaxACMode;           // MAXac״̬          :1��MAX��       0����MAX��
   uint8         MaxDefMode;          // MAXǰ��˪״̬    :1��MAX��       0����MAX��
   uint8         ParkAC;              // פ��ָʾ��
   uint8         u8TestLevel;
   uint8         ptcMode;
   uint8         WaterWay1;          //1-2 2-3 ��50%    1-2 �� 0%  1-3 ��100%
   uint8         WaterWay2;
   uint8         FourWay;            // 0��1-2��3-4   1��1-4�� 2-3
   SYSTEMMODE    systemMode;
   TESTMMODE     TestMode;            //ʵ������level
   LevelModeType     LevelMode;             //level ��Ӧ��ģʽ
   ThemalModeType    u8CompressMode;      //0 ��   1 ����  2 ����
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
	TempType TambCal;        //������ʾ������
	TempType TinCal;
	TempType TevpCal;
	TempType TwaterCal;    //ˮ��Ӧ���������ֽ�	
	uint8 u8ADSunR;
	uint8 u8VehicleSpeed;
	TempType TVentF;
	TempType TVentD;
	TempType Tadt;           //������¶Ȳο�ֵ
	TempType Tddt;           //Ŀ�������¶�
	TempType TambCal_TD;     //����td���������
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
//�����������
typedef struct
{
	BOOL  ACOut;            //ѹ��������    0�� ������  1 : ��
	VoltType FANVolt;          //���� �����ѹ
	uint8 NCFAD;           //��Ϊ������ʱ 1����ѭ�� 0 ��ѭ��
	uint8 MODEAD;
	uint8 MIXAD;
	BOOL  RHeatOut;
	uint8 u8RCheat;
	uint8 u8LCheat;
}OUTDATA;

//�����������
typedef struct
{
	uint16  ThreeWay1_Postion;            //LIN
	uint16  ThreeWay2_Postion;            //LIN
	uint16   FourWay_Postion;             //LIN 1/0  0��1-2��3-4   1��1-4�� 2-3
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
	uint8  IncarFAN;      //0 �أ� 1 ���٣� 2 ����
	uint8  APTC;
	//uint8  bACStart;
	BOOL ThemalSysRequest;
	BOOL  HeatConfirmation;
	BOOL  CoolConfirmation;
}EVOUTDATA;

//��ʾ��������
typedef struct
{
	uint8 LEDMode;          //0: OFF    1: ��ʾ     2�� �䰵ģʽ
	uint8 LCDMode;          //0: OFF    1: ��ʾ
	uint8 LCDDiagMode;      //lCD �Լ�
	int16 i16Temp;          //�¶�����ʾ��-400 ~800 Ϊֱ����ʾ�� 10000 ��HI
	BOOL AC;
	MDModeType VentMode;         //����ģʽ,ͬsystemcontrol.mdMdoe
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
