/*
 * EACControl.c
 *
 *  Created on: 2021年12月10日
 *      Author: Administrator
 */


#include "APPDrive.h"
#include "KeyPadLogic.h"
#include "CoolSpeed_PID.h"
#include "PTCPower_PID.h"
#include "HotSpeed_PID.h"
#include "EACControl.h"




#define  BATCOOL_STEP_CLOSE     0
#define  BATCOOL_STEP_BEGIN     1
#define  BATCOOL_STEP_EWP2      2
#define  BATCOOL_STEP_EXV       3
#define  BATCOOL_STEP_COMP      4
#define  BATCOOL_STEP_EXVADJ      5


#define  EACSTEP_BEIGIN      0
#define  EACSTEP_EWP         1
#define  EACSTEP_SOV         2
#define  EACSTEP_COMP        3
#define  EACSTEP_SOVCLOSE    4
#define  EACSTEP_EWPCLOSE    5
static uint8 EACStepState;
static uint16 TimerEACStep;


uint32 EACErrCode;
uint32 EACStopReason;

static uint8 bTambCool;
static uint8 bTambDef;
static uint8 bTambHP;
static uint8 bTambWPTC;

EACCONTROL  EACControlState;
EACRUNSTATE sEACRunState;
int16 i16SignalCarMaxSpeed = 4000;//E-COM_Hcoolmax
int16 i16SignalBatMaxSpeed = 3000;
int16 i16BatCInitSpeed = 2300; //E-COM_Bcoolmax 电池初始转速
int16 i16BothCoolMaxSpeed = 7500; //E-COM_Hhot&E-COM_Bhotmax
int16 i16EXVCharge = 250;    //EXV_charge
int16 i16EXVMin = 150;
uint8 u8EXVAdjStep = 5;
uint8 u8EXVAdjTime = 10;
int16 i6CoolMinSpeed = 800;  //制冷最小转速
int16 i6HotMinSpeed = 800;  //制冷最小转速
int16 i16BEXVCool = 640;      /// BEXV_cool

TempType TBL_COOLTEVP[3] = {SCD_T(3), SCD_T(5), SCD_T(7)};
int16 TBL_COOLInitSpeed[3] = {4000, 3400, 2800};  //对应转速cool_H_1X
int16 TBL_CoolDefInitSpeed[4] = {800, 800, 800, 800};  //对应转速cool_H_1X
int16 TBL_CoolDefMix[4] = {400, 600, 700, 1000};

int16 TBL_HotDefInitSpeed[3] = {800, 800, 800};  //对应转速
int16 TBL_HotDefInitPTC[3] = {1500, 2000, 2500};    //PTCRH_5X- 7X
int16 TBL_HotDefMix[3] = {1000, 1000, 1000};
int16 TBL_HotDefTWater[3] = {SCD_T(35), SCD_T(40), SCD_T(45)};
int16 TBL_HPWaterTaget[3] = {SCD_T(45), SCD_T(50), SCD_T(55)};
MDModeType TBL_HotDefMode[3] = {MD_MODE_OSF, MD_MODE_OSFD, MD_MODE_OSD};
int16 Parameter_A = 170;
int16 Parameter_B = 150;

//热泵相关参数
int16 i16SignalCarMaxSpeed_HP = 4000;//E-COM_Hcoolmax
int16 TBL_HPInitSpeed[3] = {3000, 4000, 5000};
int16 i16BatHInitPSpeed = 2500;  //E-COM_Bhot
int16 i16BatHMaxSpeed = 3000; // E-COM_BHotmax

int16 TBL_BEXV_HOTX[2] = {2000, 8000};
int16 TBL_BEXV_HOTY[2] = {250, 310};
//WPTC相关参数
int16 TBLWPTC_TargetTemp[3] = {SCD_T(50), SCD_T(55),  SCD_T(65)};
int16 TBLWPTC_InitPower[3] = {3000, 4000,  5000};
int16 Parameter_C = 150;
int16 i16BatPTC_InitPower = 3000;
int16 i16PTCPowerMin = 200;
extern TempType   tACON_EVP ,tACOFF_EVP;

//状态值
static uint8 HighPreState;
static int16 i16HiP_LimtPress = 1000;
static int16 i16MinSpeed = 800;
int16 HPreePro_T1 = 10;
#define   HIGHPRESS_STATE_NULL    0
#define   HIGHPRESS_STATE_LIMT    1
#define   HIGHPRESS_STATE_OFF     2
#define   HIGHPRESS_STATE_UP      3
LevelModeType GetTSETLevelMode(uint8 level)
{
	LevelModeType u8LevelMode;
	if (level >= TSETTEM_MODE_COOL_1 && level <= TSETTEM_MODE_COOL_3)
	{
		u8LevelMode = LEVELMODE_COOL;
	}
	else if (level >= TSETTEM_MODE_COOLDEF_1 && level <= TSETTEM_MODE_COOLDEF_4)
	{
		u8LevelMode = LEVELMODE_COOLDEF;
	}
	else if (level >= TSETTEM_MODE_HOTDEF_1 && level <= TSETTEM_MODE_HOTDEF_3)
	{
		u8LevelMode = LEVELMODE_HOTDEF;
	}
	else if (level >= TSETTEM_MODE_HPMODE_1 && level <= TSETTEM_MODE_HPMODE_3)
	{
		u8LevelMode = LEVELMODE_HPMODE;
	}
	else if (level >= TSETTEM_MODE_WPTC_1 && level <= TSETTEM_MODE_WPTC_3)
	{
		u8LevelMode = LEVELMODE_WPTC;
	}
	else if (level == TSETTEM_MODE_DEBUG)
	{
		u8LevelMode = LEVELMODE_DEBUG;
	}
	else
	{
		u8LevelMode = LEVELMODE_DEFAULT;
	}

	return u8LevelMode;
}


//根据SYSTEMMODE得到comp mode
uint8 GetCompressMode(SYSTEMMODE mode)
{
	uint8 u8lMode;
	//	SYSTEM_MODE_CARCOOL = 3,
	//SYSTEM_MODE_BATTCOOL = 4,
	//SYSTEM_MODE_BOTHCOOL = 5,
	//SYSTEM_MODE_COOLDEF1 = 6,SYSTEM_MODE_HOTDEF2 = 7
	if (mode == SYSTEM_MODE_CARCOOL || mode == SYSTEM_MODE_BATTCOOL
			|| mode == SYSTEM_MODE_BOTHCOOL || mode == SYSTEM_MODE_COOLDEF1 || mode == SYSTEM_MODE_HOTDEF2)
	{
		u8lMode = 1;
	}//	SYSTEM_MODE_HPMODE = 8,
	//SYSTEM_MODE_BAT_HPMODE = 9,
	//SYSTEM_MODE_BAT_WPTC = 10,
	//SYSTEM_MODE_BAT_HOT = 11,
	//SYSTEM_MODE_WPTC = 12,
	//SYSTEM_MODE_BOTH_HPMODE = 13,
	//SYSTEM_MODE_BOTH_WPTC = 14,
	else	if (mode == SYSTEM_MODE_HPMODE || mode == SYSTEM_MODE_BAT_HPMODE || mode == SYSTEM_MODE_BOTH_HPMODE)

	{
		u8lMode = 2;
	}
	else
	{
		u8lMode = 0;
	}

	return u8lMode;
}
//模式切换时 调用
void DealModeSwitch(LevelModeType EnterMode)
{

	switch (SystemControl.LevelMode)
	{
		default:
		case LEVELMODE_DEFAULT:
			break;
		case LEVELMODE_COOL:
			//SystemControl.acMode
			SystemControl.mdMode = MD_MODE_OSF;
			SystemControl.ncfMode = NCF_MODE_CIRF;
			break;
		case LEVELMODE_COOLDEF:
			SystemControl.mdMode = MD_MODE_OSF;
			SystemControl.ncfMode = NCF_MODE_CIRF;
			break;
		case LEVELMODE_HOTDEF:
			SystemControl.mdMode = MD_MODE_OSFD;
			SystemControl.ncfMode = NCF_MODE_CIRF;
			break;
		case LEVELMODE_HPMODE:
			SystemControl.mdMode = MD_MODE_OSD;
			SystemControl.ncfMode = NCF_MODE_NEW;
			break;
		case LEVELMODE_WPTC:
			SystemControl.mdMode = MD_MODE_OSD;
			SystemControl.ncfMode = NCF_MODE_NEW;
			break;
	}
}

///TestModeControl  FUNCTIONO
static uint8 bHotPTCTamb;
void TestModeControl(void)
{
	static LevelModeType  LastLevel;
	static uint16 TimerLevelSwitch;
	LevelModeType curLevel;
	if (SystemControl.OffState == OFF_STATE_OFF)
	{
		curLevel = LEVELMODE_DEFAULT;
	}
	else
	{
		curLevel = GetTSETLevelMode(SystemControl.u8TestLevel);
	}

	if (sSenSorData.TambCal >= SCD_T(-7))
	{
		bHotPTCTamb = 0;
	}

	if (sSenSorData.TambCal <= SCD_T(-12))
	{
		bHotPTCTamb = 1;
	}
	//档位切换时 需要延时1min
	if (LastLevel != curLevel)
	{
		if (SystemControl.LevelMode != LEVELMODE_DEFAULT)
		{//不是在默认模式，则需要重新定时,且运行在0档模式
			(void)TimeOutChkTenthSLong(&TimerLevelSwitch, 0);
			SystemControl.LevelMode = LEVELMODE_DEFAULT;
			SystemControl.TestMode = TSETTEM_MODE_DEFAULT;
		}
		else
		{//在默认模式
			if (TimeOutChkTenthSLong(&TimerLevelSwitch, 600))
			{
				SystemControl.LevelMode = curLevel;
				LastLevel = curLevel;
				SystemControl.TestMode = SystemControl.u8TestLevel;
				//第一次进入对应模式
				DealModeSwitch(SystemControl.LevelMode);
			}
			else
			{
				//
			}
		}
	}
	else
	{//一样直接切换
		SystemControl.LevelMode = curLevel;
		SystemControl.TestMode = SystemControl.u8TestLevel;
	}

	if (SystemControl.OffState == OFF_STATE_OFF)
	{
		SystemControl.acMode = AC_MODE_OFF;
		SystemControl.ptcMode = 0;
		SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
	}
	else
	{
		switch (SystemControl.TestMode)
		{
			default:
			case TSETTEM_MODE_DEFAULT:
			case TSETEM_MODE_JIAZHU:
				EACControlState.ThreeWay1_Postion = 50;
				EACControlState.ThreeWay2_Postion = 50;
				EACControlState.FourWay_Postion = 0;
				EACControlState.AGS_Postion = 0;
				EACControlState.BEXV_Postion = 640;
				EACControlState.EXV_Postion = 250;
				EACControlState.SOV1 = 0;
				EACControlState.SOV2 = 0;
				EACControlState.EWPT1 = 0;
				EACControlState.EWPT2 = 0;
				EACControlState.EWPT3 = 0;
				EACControlState.EACStart = 0;
				EACControlState.PTCStart = 0;
				EACControlState.IncarFAN = 0;
				SystemControl.systemMode = SYSTEM_MODE_JIAZHU;
				SystemControl.thBathMode = THEMAL_MODE_NULL;
				SystemControl.thCarMode = THEMAL_MODE_NULL;
				break;
			case TSETTEM_MODE_WATERWAY_DEBUG:
				EACControlState.ThreeWay1_Postion = 50;
				EACControlState.ThreeWay2_Postion = 50;
				EACControlState.FourWay_Postion = 0;
				EACControlState.AGS_Postion = 0;
				EACControlState.BEXV_Postion = 640;
				EACControlState.EXV_Postion = 250;
				EACControlState.SOV1 = 0;
				EACControlState.SOV2 = 0;
				EACControlState.EWPT1 = 1;
				EACControlState.EWPT2 = 1;
				EACControlState.EWPT3 = 1;
				EACControlState.EACStart = 0;
				EACControlState.PTCStart = 0;
				EACControlState.IncarFAN = 0;
				SystemControl.systemMode = SYSTEM_MODE_JIAZHU;
				SystemControl.thBathMode = THEMAL_MODE_NULL;
				SystemControl.thCarMode = THEMAL_MODE_NULL;
				break;
			case TSETTEM_MODE_COOL_1:
			case TSETTEM_MODE_COOL_2:
			case TSETTEM_MODE_COOL_3:
				if (sVehicleControl.bBattHotReq)
				{//有制热，直接到制热
					if (bHotPTCTamb)
					{
						SystemControl.systemMode = SYSTEM_MODE_BAT_WPTC;
					}
					else
					{
						SystemControl.systemMode = SYSTEM_MODE_BAT_HPMODE;
					}
				}
				else if (SystemControl.acMode == AC_MODE_ON && sVehicleControl.bBattCoolReq)
				{
					SystemControl.systemMode = SYSTEM_MODE_BOTHCOOL;
				}
				else if (SystemControl.acMode == AC_MODE_ON)
				{
					SystemControl.systemMode = SYSTEM_MODE_CARCOOL;
				}
				else if (sVehicleControl.bBattCoolReq)
				{
					SystemControl.systemMode = SYSTEM_MODE_BATTCOOL;
				}
				else
				{
					SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
				}
				EACControlState.ThreeWay1_Postion = 100;
				EACControlState.ThreeWay2_Postion = 0;
				EACControlState.FourWay_Postion = 0;
				EACControlState.AGS_Postion = 0;
				EACControlState.BEXV_Postion = 640;
				EACControlState.EXV_Postion = 250;
				EACControlState.SOV1 = 0;
				EACControlState.SOV2 = 0;
				EACControlState.EWPT1 = 1;
				EACControlState.EWPT2 = 1;
				EACControlState.EWPT3 = 1;
				EACControlState.EACStart = 1;
				EACControlState.PTCStart = 0;
				EACControlState.IncarFAN = 0;
				break;
			case TSETTEM_MODE_COOLDEF_1:
			case TSETTEM_MODE_COOLDEF_2:
			case TSETTEM_MODE_COOLDEF_3:
			case TSETTEM_MODE_COOLDEF_4:
				if (sVehicleControl.bBattHotReq)
				{
					if (bHotPTCTamb)
					{
						SystemControl.systemMode = SYSTEM_MODE_BAT_WPTC;
					}
					else
					{
						SystemControl.systemMode = SYSTEM_MODE_BAT_HPMODE;
					}
				}
				else	if (sVehicleControl.bBattCoolReq)
				{
					SystemControl.systemMode = SYSTEM_MODE_BATTCOOL;
				}
				else if (SystemControl.acMode == AC_MODE_ON)
				{
					SystemControl.systemMode = SYSTEM_MODE_COOLDEF1;
				}
				else
				{
					SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
				}
				EACControlState.ThreeWay1_Postion = 100;
				EACControlState.ThreeWay2_Postion = 0;
				EACControlState.FourWay_Postion = 0;
				EACControlState.AGS_Postion = 0;
				EACControlState.BEXV_Postion = 640;
				EACControlState.EXV_Postion = 250;
				EACControlState.SOV1 = 0;
				EACControlState.SOV2 = 0;
				EACControlState.EWPT1 = 1;
				EACControlState.EWPT2 = 1;
				EACControlState.EWPT3 = 1;
				EACControlState.EACStart = 1;
				EACControlState.PTCStart = 0;
				EACControlState.IncarFAN = 0;
				break;
			case TSETTEM_MODE_HOTDEF_1:
			case TSETTEM_MODE_HOTDEF_2:
			case TSETTEM_MODE_HOTDEF_3:
				if (sVehicleControl.bBattHotReq)
				{
					if (bHotPTCTamb)
					{
						SystemControl.systemMode = SYSTEM_MODE_BAT_WPTC;
					}
					else
					{
						SystemControl.systemMode = SYSTEM_MODE_BAT_HPMODE;
					}
				}
				else	if (sVehicleControl.bBattCoolReq)
				{
					SystemControl.systemMode = SYSTEM_MODE_BATTCOOL;
				}
				else if (SystemControl.acMode == AC_MODE_ON || SystemControl.ptcMode == 1)
				{

					//else
					//{
						SystemControl.systemMode = SYSTEM_MODE_HOTDEF2;
					//}
				}
				else
				{
					SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
				}
				break;
			case TSETTEM_MODE_HPMODE_1:
			case TSETTEM_MODE_HPMODE_2:
			case TSETTEM_MODE_HPMODE_3:
				if (sVehicleControl.bBattCoolReq)
				{
					SystemControl.systemMode = SYSTEM_MODE_BATTCOOL;
				}
				else if (SystemControl.acMode == AC_MODE_ON && sVehicleControl.bBattHotReq)
				{
					SystemControl.systemMode = SYSTEM_MODE_BOTH_HPMODE;
				}
				else if (SystemControl.acMode == AC_MODE_ON)
				{
					SystemControl.systemMode = SYSTEM_MODE_HPMODE;
				}
				else if (sVehicleControl.bBattHotReq)
				{
					SystemControl.systemMode = SYSTEM_MODE_BAT_HPMODE;
				}
				else
				{
					SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
				}
				break;
			case TSETTEM_MODE_WPTC_1:
			case TSETTEM_MODE_WPTC_2:
			case TSETTEM_MODE_WPTC_3:
				if (sVehicleControl.bBattCoolReq)
				{
					SystemControl.systemMode = SYSTEM_MODE_BATTCOOL;
				}
				else if (SystemControl.ptcMode == 1 && sVehicleControl.bBattHotReq)
				{
					SystemControl.systemMode = SYSTEM_MODE_BOTH_WPTC;
				}
				else if (SystemControl.ptcMode == 1)
				{
					SystemControl.systemMode = SYSTEM_MODE_WPTC;
				}
				else if (sVehicleControl.bBattHotReq)
				{
					SystemControl.systemMode = SYSTEM_MODE_BAT_WPTC;
				}
				else
				{
					SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
				}
				break;
			case TSETTEM_MODE_DEBUG:
				break;
		}
	}

	//外温进入条件设置
	if (sSenSorData.TambCal >= SCD_T(15))
	{
		bTambCool = 0;
	}
	else
	{
		if (sEVOutData.EACSpeed > 0)
		{
		}
		else
		{
			bTambCool = 1;
		}
	}

	//def
	if (sSenSorData.TambCal <= SCD_T(20) && sSenSorData.TambCal >= SCD_T(0))
	{
		bTambDef = 0;
	}
	else
	{
		bTambDef = 1;
	}

	//def
	if (sSenSorData.TambCal <= SCD_T(15) && sSenSorData.TambCal >= SCD_T(-7))
	{
		bTambHP = 0;
	}
	else
	{
		bTambHP = 1;
	}


	//def
	if (sSenSorData.TambCal <= SCD_T(15) )
	{
		bTambWPTC = 0;
	}
	else
	{
		bTambWPTC = 1;
	}


	switch(SystemControl.systemMode)
	{
		default:
			break;
		case SYSTEM_MODE_CARCOOL:
		case SYSTEM_MODE_BATTCOOL:
		case SYSTEM_MODE_BOTHCOOL:
			if (bTambCool)
			{
				SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
			}
			else{}
			break;
		case SYSTEM_MODE_COOLDEF1:
		case SYSTEM_MODE_HOTDEF2:
			if (bTambDef)
			{
				SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
			}
			else{}
			break;
		case SYSTEM_MODE_HPMODE:
		case SYSTEM_MODE_BAT_HPMODE:
		case SYSTEM_MODE_BOTH_HPMODE:
			if (bTambHP)
			{
				SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
			}
			else{}
			break;
		case SYSTEM_MODE_WPTC:
		case SYSTEM_MODE_BOTH_WPTC:
		case SYSTEM_MODE_BAT_WPTC:
			if (bTambWPTC)
			{
				SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
			}
			else{}
			break;
	}


	static uint8 lastCompMode = 0;
	SystemControl.u8CompressMode = GetCompressMode(SystemControl.systemMode);
	if ( (lastCompMode == 1 && SystemControl.u8CompressMode == 2) ||
        (lastCompMode == 2 && SystemControl.u8CompressMode == 1)
	)
	{//当压缩机模式切换时 需要等待 ，此处逻辑暂时。。。。
	//	SystemControl.systemMode = SYSTEM_MODE_DEFAULT;
		SystemControl.u8CompressMode = 0;
	}
	else
	{

	}
	lastCompMode = SystemControl.u8CompressMode;
}

void BatCoolStep_Fun(void)
{
	static uint16 TimerBatCool;
	if (SystemControl.systemMode == SYSTEM_MODE_BATTCOOL &&
			SystemControl.systemMode == SYSTEM_MODE_BOTHCOOL)
	{
		switch (sEACRunState.BatCoolStep)
		{
			default:
			case BATCOOL_STEP_CLOSE:
				sEACRunState.BatCoolStep = BATCOOL_STEP_BEGIN;
				(void)TimeOutChkTenthSLong(&TimerBatCool, 0);
			case BATCOOL_STEP_BEGIN:
				sEACRunState.BatCoolStep = BATCOOL_STEP_EWP2;
				(void)TimeOutChkTenthSLong(&TimerBatCool, 0);
				break;
			case BATCOOL_STEP_EWP2:
				if (sActorData.EWP2Error == 0)
				{
					if (TimeOutChkTenthSLong(&TimerBatCool, 10))
					{
						sEACRunState.BatCoolStep = BATCOOL_STEP_EXV;
					}else{}
				}
				break;
			case BATCOOL_STEP_EXV:
				if (sActorData.EXVError == 0)
				{
					sEACRunState.BatCoolStep = BATCOOL_STEP_COMP;
					(void)TimeOutChkTenthSLong(&TimerBatCool, 0);
					sEACRunState.BatSpeedLimt = 800;
				}
				else{}
				break;
			case BATCOOL_STEP_COMP:
				if (TimeOutChkTenthSLong(&TimerBatCool, 10))
				{
					sEACRunState.BatSpeedLimt += (i16BatCInitSpeed - 800) /5;
					if (sEACRunState.BatSpeedLimt >= i16BatCInitSpeed)
					{
						sEACRunState.BatSpeedLimt = i16BatCInitSpeed;
						sEACRunState.BatCoolStep = BATCOOL_STEP_EXVADJ;
					}
					else{}
				}
				break;
			case BATCOOL_STEP_EXVADJ:
				if (SystemControl.systemMode == SYSTEM_MODE_BATTCOOL)
				{
					EACControlState.EXV_Postion = i16EXVCharge;
				}
				else
				{//both cooling
					if (TimeOutChkTenthSLong(&TimerBatCool, 300))
					{
						if (sEVSensorData.BattInWater <= SCD_T(20))
						{
							EACControlState.EXV_Postion -= 20;
							if (EACControlState.EXV_Postion < i16EXVMin)
							{
								EACControlState.EXV_Postion = i16EXVMin;
							}else{}
						}
					}
				}
				break;
		}
	}
	else
	{
		sEACRunState.BatCoolStep = BATCOOL_STEP_CLOSE;
	}
}

//系统模式设置
void SystemModeControl(void)
{
	int16 lData1;
	(void)Deal_CoolSpeedPID();
	(void)Deal_PTCPowerPID();


	if (SystemControl.systemMode == SYSTEM_MODE_CARCOOL)
	{
		SystemControl.thBathMode = THEMAL_MODE_NULL;
		SystemControl.thCarMode = THEMAL_MODE_COOL;
		lData1 = SystemControl.TestMode - TSETTEM_MODE_COOL_1;

		sCoolSpeedPidData.i16Set = TBL_COOLTEVP[lData1];
		sCoolSpeedPidData.i16Get = sSenSorData.TevpCal;
		sCoolSpeedPidData.i16AM_FF0 = TBL_COOLInitSpeed[lData1];
		if (sSenSorData.TambCal <= SCD_T(25))
		{
			lData1 = SCD_T(25);
		}
		else if (sSenSorData.TambCal >= SCD_T(43))
		{
			lData1 = SCD_T(43);
		}
		else
		{
			lData1 = sSenSorData.TambCal;
		}
		sCoolSpeedPidData.i16AM_FF0 +=  (lData1 - 430) * Parameter_A/ 10 ;

		if (SystemControl.fanMode > 6)
		{
			lData1 = 6;
		}
		else
		{
			lData1 = SystemControl.fanMode;
		}
		sCoolSpeedPidData.i16AM_FF0 += (lData1 -4) * Parameter_B;

		EACControlState.MixPer = 0;
		EACControlState.EACStart = 1;
		EACControlState.i16MaxSpeed = i16SignalCarMaxSpeed;
		EACControlState.i16MinSpeed = i6CoolMinSpeed;
		EACControlState.bCompMode = THEMAL_MODE_COOL;
	//	EACControlState.ModePer = 0;
	//	EACControlState.NcfPer = 0;
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_BOTHCOOL)
	{
		SystemControl.thBathMode = THEMAL_MODE_COOL;
		SystemControl.thCarMode = THEMAL_MODE_COOL;
		EACControlState.bCompMode = THEMAL_MODE_COOL;

		lData1 = SystemControl.TestMode - TSETTEM_MODE_COOL_1;

		sCoolSpeedPidData.i16Set = TBL_COOLTEVP[lData1];
		sCoolSpeedPidData.i16Get = sSenSorData.TevpCal;
		sCoolSpeedPidData.i16AM_FF0 = TBL_COOLInitSpeed[lData1];
		if (sSenSorData.TambCal <= SCD_T(25))
		{
			lData1 = SCD_T(25);
		}
		else if (sSenSorData.TambCal >= SCD_T(43))
		{
			lData1 = SCD_T(43);
		}
		else
		{
			lData1 = sSenSorData.TambCal;
		}
		sCoolSpeedPidData.i16AM_FF0 +=  (lData1 - 430) * Parameter_A/ 10 ;

		if (SystemControl.fanMode > 6)
		{
			lData1 = 6;
		}
		else
		{
			lData1 = SystemControl.fanMode;
		}
		sCoolSpeedPidData.i16AM_FF0 += (lData1 -4) * Parameter_B;

		EACControlState.MixPer = 0;
		EACControlState.i16MaxSpeed = i16BothCoolMaxSpeed;
		EACControlState.i16MinSpeed = i6CoolMinSpeed;

		EACControlState.EWPT2 = 1;
//		if (sEACRunState.BatCoolStep >= BATCOOL_STEP_EXV)
//		{
//			EACControlState.EXV_Postion = i16EXVCharge;
//		}
//		else
//		{
//
//		}

		if (sEACRunState.BatCoolStep >= BATCOOL_STEP_COMP)
		{
			EACControlState.IncarFAN = 1;
			EACControlState.bCompMode = 1;
			EACControlState.EACStart = 1;
		}
		else
		{

		}
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_BATTCOOL)
	{//电池制冷

		SystemControl.thBathMode = THEMAL_MODE_COOL;
		SystemControl.thCarMode = THEMAL_MODE_NULL;
		EACControlState.bCompMode = THEMAL_MODE_COOL;

		sCoolSpeedPidData.i16Set = sEVSensorData.BattTargetWater;  //以电池水温为目标
		sCoolSpeedPidData.i16Get = sEVSensorData.BattInWater;
		sCoolSpeedPidData.i16AM_FF0 = i16BatCInitSpeed;  //

		EACControlState.MixPer = 0;
		EACControlState.i16MaxSpeed = i16SignalBatMaxSpeed;
		EACControlState.i16MinSpeed = i6CoolMinSpeed;

		EACControlState.EWPT2 = 1;
//		if (sEACRunState.BatCoolStep >= BATCOOL_STEP_EXV)
//		{
//			EACControlState.EXV_Postion = i16EXVCharge;
//		}
//		else
//		{
//
//		}
		if (sEACRunState.BatCoolStep >= BATCOOL_STEP_COMP)
		{
			EACControlState.IncarFAN = 1;
			EACControlState.bCompMode = 1;
			EACControlState.EACStart = 1;
		}
		else
		{
			EACControlState.EACStart = 0;
			EACControlState.bCompMode = 0;
			EACControlState.IncarFAN = 0;
		}
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_COOLDEF1)
	{
		SystemControl.thBathMode = THEMAL_MODE_NULL;
		SystemControl.thCarMode = THEMAL_MODE_COOL;
		EACControlState.bCompMode = THEMAL_MODE_COOL;

		sCoolSpeedPidData.i16Set = SCD_T(3);
		sCoolSpeedPidData.i16Get = sSenSorData.TevpCal;
		lData1 = SystemControl.TestMode - TSETTEM_MODE_COOLDEF_1;
		sCoolSpeedPidData.i16AM_FF0 = TBL_CoolDefInitSpeed[lData1];
		EACControlState.MixPer = TBL_CoolDefMix[lData1];

		EACControlState.EACStart = 1;
		EACControlState.i16MaxSpeed = i16SignalCarMaxSpeed;
		EACControlState.i16MinSpeed = i6CoolMinSpeed;
		EACControlState.bCompMode = 1;
	//	EACControlState.ModePer = 0;
	//	EACControlState.NcfPer = 0;
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_HOTDEF2)
	{//此处需要判定是否开AC
		SystemControl.thBathMode = THEMAL_MODE_NULL;
		SystemControl.thCarMode = THEMAL_MODE_NULL;
		//EACControlState.bCompMode = THEMAL_MODE_COOL;

		EACControlState.i16MaxSpeed = i16SignalBatMaxSpeed;
		EACControlState.i16MinSpeed = i6CoolMinSpeed;
		if (SystemControl.acMode)
		{
			SystemControl.thCarMode = THEMAL_MODE_COOL;
		}else{}

		if (SystemControl.ptcMode)
		{
			SystemControl.thCarMode += THEMAL_MODE_HOT;
		}else{}

		sCoolSpeedPidData.i16Set = SCD_T(3);
		sCoolSpeedPidData.i16Get = sSenSorData.TevpCal;
		lData1 = SystemControl.TestMode - TSETTEM_MODE_COOLDEF_1;
		sCoolSpeedPidData.i16AM_FF0 = TBL_HotDefInitSpeed[lData1];
		EACControlState.MixPer = TBL_HotDefMix[lData1];

		if (SystemControl.acMode == AC_MODE_ON)
		{
			EACControlState.EACStart = 1;
			EACControlState.bCompMode = THEMAL_MODE_COOL;
		}
		else
		{
			EACControlState.EACStart = 0;
			EACControlState.bCompMode = THEMAL_MODE_NULL;
		}

		sPTCPowerPidData.i16Set = TBL_HotDefTWater[lData1];
		sPTCPowerPidData.i16Get = sSenSorData.TwaterCal;
		sPTCPowerPidData.i16AM_FF0 = TBL_HotDefInitPTC[lData1];
		if (SystemControl.ptcMode)
		{
			EACControlState.PTCStart = 1;
		}
		else
		{
			EACControlState.PTCStart = 0;
		}
	//	EACControlState.ModePer = 0;
	//	EACControlState.NcfPer = 0;
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_HPMODE)
	{
		SystemControl.thBathMode = THEMAL_MODE_NULL;
		SystemControl.thCarMode = THEMAL_MODE_HOT;
		EACControlState.bCompMode = THEMAL_MODE_HOT;

		int16 lTBL_TempX[2] = {SCD_T(0), SCD_T(5)};
		int16 lTBL_SpeedY[2] = {800, 1500};
		EACControlState.i16MaxSpeed = i16SignalCarMaxSpeed_HP;
		EACControlState.i16MinSpeed = MathG_GetSegLinei16(sSenSorData.TambCal, lTBL_TempX, lTBL_SpeedY, 2);

		lData1 = SystemControl.TestMode - TSETTEM_MODE_HOTDEF_1;
		sHotSpeedPidData.i16Set = TBL_HPWaterTaget[lData1];
		sHotSpeedPidData.i16Get = sSenSorData.TwaterCal;
		sHotSpeedPidData.i16AM_FF0 = TBL_HPInitSpeed[lData1];

		sHotSpeedPidData.i16AM_FF0 -= (sSenSorData.TambCal + SCD_T(7)) * Parameter_A/ 10;
		if (sHotSpeedPidData.i16AM_FF0 < 800)
		{
			sHotSpeedPidData.i16AM_FF0 = 800;
		}else{}

		if (SystemControl.fanMode >= 4)
		{
			lData1 = 4;
		}
		else
		{
			lData1 = SystemControl.fanMode;
		}
		sHotSpeedPidData.i16AM_FF0 += (SystemControl.fanMode - 2) * Parameter_B;

		EACControlState.MixPer = 1000;

		if (SystemControl.acMode == AC_MODE_ON)
		{
			EACControlState.EACStart = 1;
			EACControlState.bCompMode = 2;
		}
		else
		{
			EACControlState.EACStart = 0;
			EACControlState.bCompMode = 0;
		}
		EACControlState.EWPT2 = 1;
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_BOTH_HPMODE)
	{
		SystemControl.thBathMode = THEMAL_MODE_HOT;
		SystemControl.thCarMode = THEMAL_MODE_HOT;
		EACControlState.bCompMode = THEMAL_MODE_HOT;

		EACControlState.EACStart = 1;
		EACControlState.bCompMode = 2;

		lData1 = SystemControl.TestMode - TSETTEM_MODE_HOTDEF_1;
		sHotSpeedPidData.i16Set = TBL_HPWaterTaget[lData1];
		sHotSpeedPidData.i16Get = sSenSorData.TwaterCal;
		sHotSpeedPidData.i16AM_FF0 = TBL_HPInitSpeed[lData1];

		sHotSpeedPidData.i16AM_FF0 -= (sSenSorData.TambCal + SCD_T(7)) * Parameter_A/ 10;
		if (sHotSpeedPidData.i16AM_FF0 < 800)
		{
			sHotSpeedPidData.i16AM_FF0 = 800;
		}else{}

		if (SystemControl.fanMode >= 4)
		{
			lData1 = 4;
		}
		else
		{
			lData1 = SystemControl.fanMode;
		}
		sHotSpeedPidData.i16AM_FF0 += (SystemControl.fanMode - 2) * Parameter_B;

		EACControlState.MixPer = 1000;

		if (SystemControl.acMode == AC_MODE_ON)
		{
			EACControlState.EACStart = 1;
			EACControlState.bCompMode = 2;
		}
		else
		{
			EACControlState.EACStart = 0;
			EACControlState.bCompMode = 0;
		}

		int16 lTBL_TempX[2] = {SCD_T(0), SCD_T(5)};
		int16 lTBL_SpeedY[2] = {800, 1500};
		EACControlState.i16MinSpeed = MathG_GetSegLinei16(sSenSorData.TambCal, lTBL_TempX, lTBL_SpeedY, 2);
		EACControlState.i16MaxSpeed = i16SignalCarMaxSpeed_HP;
		EACControlState.EWPT2 = 1;
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_BAT_HPMODE)
	{
		SystemControl.thBathMode = THEMAL_MODE_HOT;
		SystemControl.thCarMode = THEMAL_MODE_NULL;
		EACControlState.bCompMode = THEMAL_MODE_HOT;

		lData1 = SystemControl.TestMode - TSETTEM_MODE_HOTDEF_1;
		sHotSpeedPidData.i16Set = sEVSensorData.BattTargetWater;
		sHotSpeedPidData.i16Get = sEVSensorData.BattInWater;
		sHotSpeedPidData.i16AM_FF0 = i16BatHInitPSpeed;

		EACControlState.MixPer = 1000;

		if (SystemControl.acMode == AC_MODE_ON)
		{
			EACControlState.EACStart = 1;
			EACControlState.bCompMode = 2;
		}
		else
		{
			EACControlState.EACStart = 0;
			EACControlState.bCompMode = 0;
		}

		int16 lTBL_TempX[2] = {SCD_T(0), SCD_T(5)};
		int16 lTBL_SpeedY[2] = {800, 1500};
		EACControlState.i16MinSpeed = MathG_GetSegLinei16(sSenSorData.TambCal, lTBL_TempX, lTBL_SpeedY, 2);
		EACControlState.i16MaxSpeed = i16SignalCarMaxSpeed_HP;

		EACControlState.EWPT2 = 1;
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_WPTC)
	{
		SystemControl.thBathMode = THEMAL_MODE_NULL;
		SystemControl.thCarMode = THEMAL_MODE_HOT;
		EACControlState.bCompMode = THEMAL_MODE_NULL;

		EACControlState.EACStart = 0;
		EACControlState.bCompMode = 0;
		EACControlState.PTCStart = 1;

		lData1 = SystemControl.TestMode - TSETTEM_MODE_WPTC_1;

		sPTCPowerPidData.i16Set = TBLWPTC_TargetTemp[lData1];
		sPTCPowerPidData.i16Get = sSenSorData.TwaterCal;
		sPTCPowerPidData.i16AM_FF0 = TBLWPTC_InitPower[lData1];;
		sPTCPowerPidData.i16AM_FF0 -= (sSenSorData.TambCal + SCD_T(20)) * Parameter_C;

		if (SystemControl.fanMode >= 6)
		{
			SystemControl.fanMode = 6;
		}else{}
		sPTCPowerPidData.i16AM_FF0 += (SystemControl.fanMode -3) * Parameter_B;

		if (sPTCPowerPidData.i16AM_FF0 <= 300)
		{
			sPTCPowerPidData.i16AM_FF0 = 300;
		}else{}
		EACControlState.MixPer = 1000;


//		EACControlState.EWPT2 = 1;
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_BOTH_WPTC)
	{
		SystemControl.thBathMode = THEMAL_MODE_HOT;
		SystemControl.thCarMode = THEMAL_MODE_HOT;
		EACControlState.bCompMode = THEMAL_MODE_NULL;

		EACControlState.EACStart = 0;
		EACControlState.bCompMode = 0;
		EACControlState.PTCStart = 1;

		lData1 = SystemControl.TestMode - TSETTEM_MODE_WPTC_1;

		sPTCPowerPidData.i16Set = TBLWPTC_TargetTemp[lData1];
		sPTCPowerPidData.i16Get = sSenSorData.TwaterCal;
		sPTCPowerPidData.i16AM_FF0 = TBLWPTC_InitPower[lData1];;
		sPTCPowerPidData.i16AM_FF0 -= (sSenSorData.TambCal + SCD_T(20)) * Parameter_C;

		if (SystemControl.fanMode >= 6)
		{
			SystemControl.fanMode = 6;
		}else{}
		sPTCPowerPidData.i16AM_FF0 += (SystemControl.fanMode -3) * Parameter_B;

		if (sPTCPowerPidData.i16AM_FF0 <= 300)
		{
			sPTCPowerPidData.i16AM_FF0 = 300;
		}else{}
		EACControlState.MixPer = 1000;
//		EACControlState.EWPT2 = 1;
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_BAT_WPTC)
	{
		SystemControl.thBathMode = THEMAL_MODE_HOT;
		SystemControl.thCarMode = THEMAL_MODE_NULL;
		EACControlState.bCompMode = THEMAL_MODE_NULL;

		EACControlState.EACStart = 0;
		EACControlState.bCompMode = 0;
		EACControlState.PTCStart = 1;

		sPTCPowerPidData.i16Set =  sEVSensorData.BattTargetWater;
		sPTCPowerPidData.i16Get =  sEVSensorData.BattInWater;
		sPTCPowerPidData.i16AM_FF0 = i16BatPTC_InitPower;

		if (SystemControl.fanMode >= 6)
		{
			SystemControl.fanMode = 6;
		}else{}
		sPTCPowerPidData.i16AM_FF0 += (SystemControl.fanMode -3) * Parameter_B;

		if (sPTCPowerPidData.i16AM_FF0 <= 300)
		{
			sPTCPowerPidData.i16AM_FF0 = 300;
		}else{}
		EACControlState.MixPer = 1000;
//		EACControlState.EWPT2 = 1;
	}
	else
	{	//其他模式
		SystemControl.thBathMode = THEMAL_MODE_NULL;
		SystemControl.thCarMode = THEMAL_MODE_NULL;
		EACControlState.bCompMode = THEMAL_MODE_NULL;
	}
}


//typedef
//{
//
//}OutType;
EVOUTDATA sEVOutState;
void EACSpeedControl(void)
{
	//int16 lData1;
	static BOOL bACTevp;
	static BOOL bACPressCool, bACPressHot;
	static BOOL bHPTamb;
	(void)Deal_CoolSpeedPID();
	(void)Deal_HotSpeedPID();



//蒸发器控制点
	if (sSenSorData.TevpCal >= tACON_EVP)
	{
		bACTevp = 0;
	}
	if (sSenSorData.TevpCal <= tACOFF_EVP)
	{
		bACTevp = 1;
	}
//压力保护
	if ( (sEVSensorData.TPL1_Press <= 300 || (sEVSensorData.TPH_Press - sEVSensorData.TPL1_Press) > 500)
			&& sEVOutData.EACSpeed == 0)
	{
		bACPressCool = 1;
	}

	if ( (sEVSensorData.TPL1_Press <= 200 || (sEVSensorData.TPH_Press - sEVSensorData.TPL1_Press) > 500)
			&& sEVOutData.EACSpeed == 0)
	{
		bACPressHot = 1;
	}
//热泵进入的条件
	if (sSenSorData.TambCal >= SCD_T(-7) && sSenSorData.TambCal <= SCD_T(15))
	{
		bHPTamb = 0;
	}

	if (sSenSorData.TambCal >= SCD_T(20) || sSenSorData.TambCal <= SCD_T(-12))
	{
		bHPTamb = 1;
	}
	if ( (SystemControl.thCarMode & THEMAL_MODE_COOL) >0 ||
			(SystemControl.thBathMode & THEMAL_MODE_COOL) >0)
	{//制冷
		//SystemControl.acMode == AC_MODE_ON && 不需要判断
		EACControlState.EACStartReq = 1;
		if ( bACTevp == 0 && bACPressCool == 0 && bTambCool == 0)
		{
			sCoolSpeedPidData.bRunEn = 1;
			sEVOutState.EACSpeed = sCoolSpeedPidData.i16AM_Hfs;
		}
		else
		{
			sCoolSpeedPidData.bRunEn = 0;
			sEVOutState.EACSpeed = 0;
		}
	}
	else if ((SystemControl.thCarMode & THEMAL_MODE_HOT) >0 ||
			(SystemControl.thBathMode & THEMAL_MODE_HOT) >0 )
	{//制热
		EACControlState.EACStartReq = 1;
		if ( bACPressHot == 0 && bHPTamb == 0)
		{
			sHotSpeedPidData.bRunEn = 1;
			sEVOutState.EACSpeed = sHotSpeedPidData.i16AM_Hfs;
		}
		else
		{
			sHotSpeedPidData.bRunEn = 0;
			sEVOutState.EACSpeed = 0;
		}
	}
	else
	{
		EACControlState.EACStartReq = 0;
		sEVOutState.EACSpeed = 0;
	}

	static int16 i16SumPress;
	static uint8 u8SumTimes;
	static uint16 TimerHiPress;
	if (TimeOutChkTenthSLong(&TimerHiPress, 10))
	{
		u8SumTimes++;
		i16SumPress += sEVSensorData.TPH_Press;
		if (u8SumTimes >= HPreePro_T1)
		{
			u8SumTimes = 0;
			sEVSensorData.TPH_Press = i16SumPress /10;
			i16SumPress = 0;

			switch (HighPreState)
			{
				default:
				case HIGHPRESS_STATE_NULL:
					if (sEVSensorData.TPH_Press > 2100)
					{
						if (sEVOutData.EACSpeed > (i16MinSpeed +200))
						{
							i16HiP_LimtPress = sEVOutData.EACSpeed -200;
						}
						else
						{
							i16HiP_LimtPress = i16MinSpeed;
						}
						HighPreState = HIGHPRESS_STATE_LIMT;
					}
					else
					{
						i16HiP_LimtPress = 10000;
					}
					break;
				case HIGHPRESS_STATE_LIMT:
					if (sEVSensorData.TPH_Press > 2100)
					{
						if (sEVOutData.EACSpeed <= i16MinSpeed + 50)
						{
							HighPreState = HIGHPRESS_STATE_OFF;
						}
						else if (sEVOutData.EACSpeed > (i16MinSpeed +200))
						{
							i16HiP_LimtPress = sEVOutData.EACSpeed -200;
						}
						else
						{
							i16HiP_LimtPress = i16MinSpeed;
						}
					}
					else if (sEVSensorData.TPH_Press >= 1600)
					{
						//i16HiP_LimtPress keep
					}
					else
					{
						i16HiP_LimtPress += 100;
						if (i16HiP_LimtPress > 10000)
						{
							HighPreState = HIGHPRESS_STATE_NULL;
						}else{}
					}
					break;
				case HIGHPRESS_STATE_OFF:
					i16HiP_LimtPress = 0;
					break;
			}//end switch
		}
		else{}//时间未到
	}else{}//时间未到

	if (HighPreState == HIGHPRESS_STATE_OFF)
	{//高压故障
		sEVOutState.EACSpeed = 0;
		EACStopReason |= EAC_REASON_HIGHPRESS;
	}
	else
	{
		EACStopReason &= ~EAC_REASON_HIGHPRESS;
	}

	if (EACControlState.EACStartReq > 0)
	{
		sEVOutState.ThemalSysRequest = 1;
	}else{}


	switch (EACStepState)
	{
		default:
		case EACSTEP_BEIGIN:
			//关闭状态
			if (EACControlState.EACStartReq > 0)
			{
				EACStepState = EACSTEP_EWP;
			}
			(void)TimeOutChkTenthSLong(&TimerEACStep, 0);
			break;
		case EACSTEP_EWP:
			//等水泵
			if (TimeOutChkTenthSLong(&TimerEACStep, 10))
			{
				EACStepState = EACSTEP_SOV;
			}
			break;
		case EACSTEP_SOV:
			//等待阀,
			if (TimeOutChkTenthSLong(&TimerEACStep, 10))
			{
				EACStepState = EACSTEP_COMP;
			}
			break;
		case EACSTEP_COMP:
			//根据实际输出
			if (EACControlState.EACStartReq == 0)
			{
				if (sVehicleControl.i16BackEACSpeed == 0)
				{
					if (TimeOutChkTenthSLong(&TimerEACStep, 50))
					{
						EACStepState = EACSTEP_SOVCLOSE;
					}else{}
				}
				else
				{
					(void)TimeOutChkTenthSLong(&TimerEACStep, 0);
				}
			}
			else
			{}
			break;
		case EACSTEP_SOVCLOSE:
			//等待阀关
			if (TimeOutChkTenthSLong(&TimerEACStep, 10))
			{
				EACStepState = EACSTEP_EWPCLOSE;
			}
			break;
		case EACSTEP_EWPCLOSE:
			//等待阀关
			if (TimeOutChkTenthSLong(&TimerEACStep, 10))
			{
				EACStepState = EACSTEP_BEIGIN;
			}
			break;
	}


	int16 lACSpeed;
	if (EACStepState == EACSTEP_COMP)
	{
	//功率限制
		lACSpeed = sEVOutState.EACSpeed;
		if (lACSpeed > sVehicleControl.i16SpeedLimte)
		{
			lACSpeed = sVehicleControl.i16SpeedLimte;
		}
		else{}

		if (lACSpeed > EACControlState.i16MaxSpeed)
		{
			lACSpeed = EACControlState.i16MaxSpeed;
		}
		else{}

		if (lACSpeed < EACControlState.i16MinSpeed)
		{
			lACSpeed = EACControlState.i16MinSpeed;
		}
		else{}

		if (lACSpeed < EACControlState.i16MinSpeed)
		{
			lACSpeed = EACControlState.i16MinSpeed;
		}
		else{}

		if (lACSpeed > i16HiP_LimtPress)
		{
			lACSpeed = i16HiP_LimtPress;
		}else{}

		EACStopReason &= ~EAC_REASON_STEP;
	}
	else
	{
		lACSpeed = 0;
		EACStopReason |= EAC_REASON_STEP;
	}

	static uint16 TimerSpeedAdj;
	if (TimeOutChkMsLong(&TimerSpeedAdj, 50))
	{//0.1 S调整一次
		if (lACSpeed == 0)
		{
			sEVOutData.EACSpeed = 0;
		}
		else if ( GAMath_abs(sEVOutData.EACSpeed - lACSpeed) >=30)
		{//30 以上进行调整
			if (sEVOutData.EACSpeed > lACSpeed)
			{
				sEVOutData.EACSpeed -= 30;
			}
			else if (sEVOutData.EACSpeed < lACSpeed)
			{
				sEVOutData.EACSpeed += 30;
			}
			else{}

			if (sEVOutData.EACSpeed < i16MinSpeed)
			{
				sEVOutData.EACSpeed = i16MinSpeed;
			}else{}
		}
		else{}
	}
}

//PTC
void PTCPowerControl(void)
{
	(void)Deal_PTCPowerPID();

	if (SystemControl.ptcMode &&
			((SystemControl.thBathMode & THEMAL_MODE_HOT) > 0 || (SystemControl.thCarMode & THEMAL_MODE_HOT) > 0)
		)
	{
		sPTCPowerPidData.bRunEn = 1;
		sEVOutState.PTCPower = sPTCPowerPidData.i16AM_Hfs;

		if (sEVOutState.PTCPower < i16PTCPowerMin)
		{
			sEVOutState.PTCPower = i16PTCPowerMin;
		}else{}
	}
	else
	{
		sPTCPowerPidData.bRunEn = 1;
		sEVOutState.PTCPower = 0;
	}

	if (sEVOutState.PTCPower > sVehicleControl.i16PTCPowerLimte)
	{
		sEVOutState.PTCPower = sVehicleControl.i16PTCPowerLimte;
	}
	else{}


}
//SOVCONTORL
void SOVControl(void)
{
	if (SystemControl.systemMode == SYSTEM_MODE_BATTCOOL)
	{
		sEVOutState.SOV1 = 0;
	   sEVOutState.SOV2 = 1;
	}
	else if (EACControlState.bCompMode == THEMAL_MODE_HOT)
	{
		sEVOutState.SOV1 = 1;
		sEVOutState.SOV2 = 1;
	}
	else
	{
		sEVOutState.SOV1 = 0;
		sEVOutState.SOV2 = 0;
	}

	//SOV1 SOV2 是跟压缩机的工作模式有关，所以可以用EACStepState
	if (EACStepState == EACSTEP_SOV || EACStepState == EACSTEP_SOVCLOSE)
	{
		sEVOutData.SOV1 = sEVOutState.SOV1;
		sEVOutData.SOV2 = sEVOutState.SOV2;
	}
	else if (EACStepState == EACSTEP_COMP)
	{
		if ( ((sEVOutData.SOV1 != sEVOutState.SOV1) || (sEVOutData.SOV2 != sEVOutState.SOV2))
			&& sEVOutState.EACSpeed >0)
		{//压缩机是开的但不一样，说明有问题
			EACErrCode |= EAC_ECODE_SOV;
		}
		else{}
	}
	else
	{//可以关
		if (sEVOutState.SOV1 == 0)
		{
			sEVOutData.SOV1 = 0;
		}else{}

		if (sEVOutState.SOV2 == 0)
		{
			sEVOutData.SOV2 = 0;
		}else{}
	}

	//FBEXV 控制
	if (SystemControl.systemMode == SYSTEM_MODE_DEFAULT)
	{
		//默认模式保持
	}
	else if (EACControlState.bCompMode == THEMAL_MODE_HOT)
	{
		sEVOutState.BEXV_Postion = MathG_GetSegLinei16(sEVOutData.EACSpeed, TBL_BEXV_HOTX, TBL_BEXV_HOTY, 2);
	}
	else
	{
		sEVOutState.BEXV_Postion = i16BEXVCool;
	}
	sEVOutData.BEXV_Postion = sEVOutState.BEXV_Postion;

	//EXV 调节
	static uint16 TimerEXVAdj;
	if (SystemControl.systemMode == SYSTEM_MODE_DEFAULT)
	{
		//默认模式保持
	}
	else if (SystemControl.TestMode == TSETEM_MODE_JIAZHU ||
			SystemControl.TestMode == TSETTEM_MODE_WATERWAY_DEBUG)
	{
		sEVOutState.EXV_Postion = 250;
	}
	else if (SystemControl.thBathMode == THEMAL_MODE_COOL)
	{//制冷时需要开启EXV
		if (EACStepState == EACSTEP_SOV || EACStepState == EACSTEP_SOVCLOSE)
		{
			sEVOutState.EXV_Postion = i16EXVCharge;
			(void)TimeOutChkSLong(&TimerEXVAdj, 0);
		}
		else if (EACStepState == EACSTEP_COMP)
		{
			if (TimeOutChkSLong(&TimerEXVAdj, u8EXVAdjTime))
			{
				if (sEVSensorData.BattInWater <= (sEVSensorData.BattTargetWater - SCD_T(2)))
				{
					sEVOutState.EXV_Postion -= u8EXVAdjStep;
					if (sEVOutState.EXV_Postion < i16EXVMin)
					{
						sEVOutState.EXV_Postion = i16EXVMin;
					}else{}
				}
				else{}  //不变
			}else{}
		}
		else
		{
			//sEVOutState.EXV_Postion 不变
		}
	}
	else
	{//其他模式不改变

	}
	sEVOutData.EXV_Postion = sEVOutState.EXV_Postion;

	//3way1
	switch(SystemControl.systemMode)
	{
		default:
		case 	SYSTEM_MODE_DEFAULT:
			//keep in default
			break;
		case  SYSTEM_MODE_JIAZHU:
		case  SYSTEM_MODE_WATERWAY_DEBUG:
		case  SYSTEM_MODE_BOTHCOOL:
		case	SYSTEM_MODE_BOTH_HPMODE:
		case	SYSTEM_MODE_BOTH_WPTC:
			sEVOutState.ThreeWay1_Postion = 50;
			break;
		case  SYSTEM_MODE_BAT_HPMODE:
		case  SYSTEM_MODE_BAT_WPTC:
			sEVOutState.ThreeWay1_Postion = 100;
			break;
		case  SYSTEM_MODE_CARCOOL:
		case  SYSTEM_MODE_BATTCOOL:
		case  SYSTEM_MODE_HOTDEF2:
		case  SYSTEM_MODE_COOLDEF1:
		case  SYSTEM_MODE_HPMODE:
		case  SYSTEM_MODE_WPTC:
			sEVOutState.ThreeWay1_Postion = 0;
			break;
	}
	sEVOutData.ThreeWay1_Postion = sEVOutState.ThreeWay1_Postion;

	static uint8 bLastEWPT2;
	static uint16 TimerEWP2;
	if (SystemControl.systemMode == SYSTEM_MODE_DEFAULT)
	{
		//默认模式保持
	}
	else if (SystemControl.systemMode == SYSTEM_MODE_JIAZHU || SystemControl.systemMode == SYSTEM_MODE_JIAZHU
			||	SystemControl.systemMode == SYSTEM_MODE_WATERWAY_DEBUG)
	{
		sEVOutState.ThreeWay2_Postion = 50;
	}
	else
	{
		if (sEVOutState.EWPT2 == 1 )
		{
			if (bLastEWPT2 == 0)
			{
				sEVOutState.ThreeWay2_Postion = 50;
				(void)TimeOutChkSLong(&TimerEWP2, 0);
			}
			else
			{
				if (TimeOutChkSLong(&TimerEWP2, 60))
				{
					sEVOutState.ThreeWay2_Postion = 0;
				}else{}
			}
		}
		else
		{
			sEVOutState.ThreeWay2_Postion = 0;
		}
	}
	bLastEWPT2 = sEVOutState.EWPT2;

	//4way
	switch(SystemControl.systemMode)
	{
		default:
		case 	SYSTEM_MODE_DEFAULT:
			//keep in default
			break;
		case  SYSTEM_MODE_JIAZHU:
		case  SYSTEM_MODE_WATERWAY_DEBUG:
			sEVOutState.FourWay_Postion = 1;
			break;
		case  SYSTEM_MODE_BOTHCOOL:
		case	SYSTEM_MODE_BOTH_HPMODE:
		case	SYSTEM_MODE_BOTH_WPTC:
		case  SYSTEM_MODE_BAT_HPMODE:
		case  SYSTEM_MODE_BAT_WPTC:
		case  SYSTEM_MODE_CARCOOL:
		case  SYSTEM_MODE_BATTCOOL:
		case  SYSTEM_MODE_HOTDEF2:
		case  SYSTEM_MODE_COOLDEF1:
		case  SYSTEM_MODE_HPMODE:
		case  SYSTEM_MODE_WPTC:
			sEVOutState.FourWay_Postion = 0;
			break;
	}
	sEVOutData.FourWay_Postion = sEVOutState.FourWay_Postion;

	//EWP1 工作
}


void PowerUseCal(void)
{
	if (SystemControl.systemMode == SYSTEM_MODE_HOTDEF2)
	{
		if (sVehicleControl.i16PowerLimte > 500)
		{
			sVehicleControl.i16SpeedLimte = sVehicleControl.i16PowerLimte - 500 *80/43;
		}
		else
		{

		}
	}
	else
	{
		sVehicleControl.i16SpeedLimte = sVehicleControl.i16PowerLimte *80/43;
	}


	sVehicleControl.i16COMPPower = sEVOutState.EACSpeed *43 /80;
	sVehicleControl.i16PTCPower = sEVOutData.PTCPower;

	if (sVehicleControl.i16PowerLimte > sVehicleControl.i16COMPPower)
	{
		sVehicleControl.i16PTCPowerLimte = sVehicleControl.i16PowerLimte > sVehicleControl.i16COMPPower;
	}
	else
	{
		sVehicleControl.i16PTCPowerLimte = 0;
	}

	if (sVehicleControl.i16PTCPowerLimte < 500)
	{
		sVehicleControl.i16PTCPowerLimte = 0;
	}else{}
}
//EACCONTROL
void EVACControl(void)
{
	TestModeControl();
	SystemModeControl();
	EACSpeedControl();
	PTCPowerControl();
	SOVControl();
	PowerUseCal();
}
