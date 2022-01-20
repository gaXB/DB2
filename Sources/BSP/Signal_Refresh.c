/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Signal_Refresh.c
* 功能描述：信号刷新模块，在此实现
* 1  温度更新。实现外温更新逻辑
* 2  阳光信号更新
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关；作为模板需要根据实际项目应用需求更改
* 具体设计可见参考文件
*/
/**************************更改记录******************************************************
×
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
* 
* 版本：1.1
* 说明：优化初始化结束的逻辑，
* 在没有车外温度传感器时，则2s后退出
* 作者：xb
* 完成日期：2020年7月6日
* 
* 版本：1.2
* 说明：1 优化初始化结束的逻辑，
* 在没有车外温度传感器时，则2s后退出,没实现没问题
* 2 非自动时，车内，车外，阳光，为默认值 去掉此功能
* 作者：xb
* 完成日期：2020年10月22日
* 
* 版本：1.3
* 说明：1 车内车外可用SigF_GetResult05来得到温度，以减少因温度回差导致的波动
* 作者：xb
* 完成日期：2020年12月19日
* 
* 版本：1.4
* 说明：1 此模板文件只实现上电外温稳定后才进入正常刷新模式，其他需要自己编程请看参考文件
* 作者：xb
* 完成日期：2021年8月20日
*/

#include "Signal_TempGet.h"
#include "APPDrive.h"
#include "Deal_DTC.h"

const uint16  TBL_PTC1_JAC1[] =
{
	991 , 964 , 922 , 861 , 776 ,
	675 , 566 , 454 , 354 , 270 ,
	203 , 152 , 113 , 85 ,  64 ,
	49 , 37 , 29 ,
};

const uint16  TBL_PRESSL_JAC1[] =
{
	965 , 922 , 861 , 778 ,
	680 , 568 , 457 , 356 , 273 ,
	207 , 155 , 117 , 88 ,  67 ,
	51, 39, 31, 23
};

const uint16  TBL_PTH[] =
{
	976 , 942 , 889 , 816 ,
	724 , 617 , 507 , 404 , 315 ,
	241 , 183 , 139 , 105 ,  80 ,
};

static ADTGETTYPE ADT_PTH_TEMP =
{
		TBL_PTH,
	   14,
	   -300,
	   100,
};


static ADTGETTYPE ADT_PressTemp =
{
		TBL_PTC1_JAC1,
	   17,
	   -400,
	   100,
};

static ADTGETTYPE ADT_PressLTemp =
{
		TBL_PRESSL_JAC1,
	   17,
	   -300,
	   100,
};
//刷新状态
static uint8 u8SigRefState;    //初始化时为1，正常滤波时0
static uint16 TimerInit;
//外温更新刷新逻辑
static TempType tAmbCal_Init, tAmbCal_Init_1;   //初始值，前一次值
static int16 i16TambSum;    //加权变量
static uint8 u8AddNum;
static TempType tWaterCal_Last;  //工作阶段的 水温，初始化时不更新

#if  NOINIT_TYPE ==  NOINIT_TYPE_KEA
__attribute__ ((section(".noinit"))) uint8 bInitTamb;   //第一次上电标记，之后0x5a，不能用MCU_IsBattRest
#elif NOINIT_TYPE == NOINIT_TYPE_RL78
#pragma section bss NOINIT
uint8 bInitTamb;   //第一次上电标记，之后0x5a，不能用MCU_IsBattRest
#pragma section 
#define __GOLBAL_VAR_
#endif

//刷新状态
#define  SIGREF_STATE_INIT    0
#define  SIGREF_STATE_RUN     1
#define  SIGREF_STATE_STOP    2
//本模块静态函数声明
static void Signal_Refresh_EndInit(void);
static void Signal_Refresh_Normal(void);
/*******************************************************************************
 * Function:        Deal_Signal_Refresh
 *
 * Description:    温度刷新主控程序，放在lowerdrive-loop调用
 *
 * Returns:         none
 *
 * Notes:           此处为温度刷新，车内，外，阳光，在此刷新
 *                  部分逻辑需要根据实际逻辑进行修改
 *                 
********************************************************************************/
void Deal_Signal_Refresh(void)
{
	switch (u8SigRefState)
	{
		default:
		case SIGREF_STATE_INIT:
		  //TODO: 初始化阶段的处理
			//以下实现外温稳定后才进入初始化模式，如没有更改1s后进入正常模式
			if (TimeOutChkTenthSLong(&TimerInit, 1))
			{
				u8AddNum++;
#ifdef  ADID_TAMB
				//以下实现温度稳定策略每0.2s采集一次采集10次，上次的温度值和本次差0.5以内说明信号稳定进入正常工作模式
				i16TambSum += ADPhy_GetFact(ADID_TAMB);
				if (u8AddNum >= 10)
				{
					tAmbCal_Init = i16TambSum/10;
					if (GAMath_abs(tAmbCal_Init - tAmbCal_Init_1) < SCD_T(0.5))
					{//温度稳定后进入刷新模式
						u8SigRefState = SIGREF_STATE_RUN;
						Signal_Refresh_EndInit();
					}
					else
					{//重新计算2s
						u8AddNum = 0;
						i16TambSum = 0;
						tAmbCal_Init_1 = tAmbCal_Init;
					}
				}
				else{}
#else
//ADID_TAMB 则2s后退出初始化
				if (u8AddNum >= 20)
				{
					Signal_Refresh_EndInit();
					u8SigRefState = SIGREF_STATE_RUN;
				}
				else{}
#endif
			}
			break;
		case SIGREF_STATE_RUN:
			Signal_Refresh_Normal();
			break;
		case SIGREF_STATE_STOP:
			break;
	}

	(void)Deal_DTC();
}
/*******************************************************************************
 * Function:       static void Signal_Refresh_Normal(void)
 *
 * Description:    正常刷新流程
 *
 * Returns:         none
 *
 * Notes:          none  
 *                 
********************************************************************************/
static void Signal_Refresh_Normal(void)
{
	//TODO: 正常情况下的温度更新，请参考模板
	static  uint16 TimerReftin;
	static  uint16 TimerRefAdd, TimerRefDec;
	static  uint16 TimerRefAdd_TD, TimerRefDec_TD;
	int16	tAmbfact, tInfact; //外温实际值
	int16 tAmbfactdiff;      //tAmbfactdiff = 实际外温 - 显示外温
	//外温信号刷新
	//外温信号刷新
	if(DTCTFAILED(DTCNUM_TAMB))
	{
		sSenSorData.TambCal = _TAMB_DEFAULT_;         //传感器故障时默认为25℃
		sSenSorData.TambCal_TD = _TAMB_DEFAULT_;
	}
	else
	{
		tAmbfact = SigF_GetResult(SIG_NUM_TAMB);  //使用滤波后的外温进行刷新
//		tAmbfact = ADPhy_GetFact(ADID_TAMB);  //暂时使用采集值
		tAmbfactdiff = tAmbfact - sSenSorData.TambCal;
		if(1)
		{
			if(0 == sSenSorData.u8VehicleSpeed)
			{
				if(tAmbfact < sSenSorData.TambCal)
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,1))
					{
					   sSenSorData.TambCal -= 5;
					   if  (sSenSorData.TambCal <= tAmbfact)
					   {
					     sSenSorData.TambCal = tAmbfact;
					   }
					}
					(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
				}
				else
				{
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}

			}
			else if((0 < sSenSorData.u8VehicleSpeed) && (sSenSorData.u8VehicleSpeed <= 20))
			{
				if(tAmbfact > sSenSorData.TambCal)
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,90))
					{  //不能加2 只能加1
					   sSenSorData.TambCal += 5;
					   if  (sSenSorData.TambCal >= tAmbfact)
					   {
					     sSenSorData.TambCal = tAmbfact;
					   }
					 }
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}
				else if(sSenSorData.TambCal!=tAmbfact)
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,1))
					{
					   sSenSorData.TambCal -= 5;
					   if  (sSenSorData.TambCal <= tAmbfact)
					   {
					      sSenSorData.TambCal = tAmbfact;
					    }
					}
					(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
				}
				else
				{
					(void)TimeOutChkTenthSLong(&TimerRefAdd_TD, 0);
					(void)TimeOutChkTenthSLong(&TimerRefDec_TD, 0);
				}

			}
			else if((20 < sSenSorData.u8VehicleSpeed) && (sSenSorData.u8VehicleSpeed <= 60))
			{
				if(tAmbfact > sSenSorData.TambCal)
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,60))
					{  //不能加2 只能加1
						sSenSorData.TambCal += 5;
						if  (sSenSorData.TambCal >= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
				   }
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}
				else if(sSenSorData.TambCal!=tAmbfact)
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,1))
					{
						sSenSorData.TambCal -= 5;
						if  (sSenSorData.TambCal <= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
				}
				else
				{
					(void)TimeOutChkTenthSLong(&TimerRefAdd_TD, 0);
					(void)TimeOutChkTenthSLong(&TimerRefDec_TD, 0);
				}
			}
			else
			{
				if(tAmbfact > sSenSorData.TambCal)
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,30))
					{  //不能加2 只能加1
						sSenSorData.TambCal += 5;
						if(sSenSorData.TambCal >= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}
				else if(sSenSorData.TambCal!=tAmbfact)
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,1))
					{
						sSenSorData.TambCal -= 5;
						if(sSenSorData.TambCal <= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
				  (void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
			   }
			   else
			   {
					(void)TimeOutChkTenthSLong(&TimerRefAdd_TD, 0);
					(void)TimeOutChkTenthSLong(&TimerRefDec_TD, 0);
				}
			}

		}
		else
		{
			if((0 < sSenSorData.u8VehicleSpeed) && (sSenSorData.u8VehicleSpeed <= 50))
			{
				if(tAmbfactdiff > SCD_T(5))
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,90))
					{  //不能加2 只能加1
						sSenSorData.TambCal += 5;
						if(sSenSorData.TambCal >= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);

				}
				/*else if(tAmbfactdiff < SCD_T(-5))
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,50))
					{
						sSenSorData.TambCal -= 5;
						if(sSenSorData.TambCal <= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);

				}*/
				else
				{
					if(tAmbfact < sSenSorData.TambCal)
					{
						if(TimeOutChkTenthSLong(&TimerRefDec,90))
						{
							sSenSorData.TambCal -= 5;
							if(sSenSorData.TambCal <= tAmbfact)
							{
								sSenSorData.TambCal = tAmbfact;
							}
						}
						(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
					}

				}
			}
			else if((50 < sSenSorData.u8VehicleSpeed) && (sSenSorData.u8VehicleSpeed < 70))
			{
				if(tAmbfactdiff > SCD_T(3))
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,60))
					{  //不能加2 只能加1
						sSenSorData.TambCal += 5;
						/*if(sSenSorData.TambCal >= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}*/
					}
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}
				else
				{
					if(tAmbfact < sSenSorData.TambCal)
					{
						if(TimeOutChkTenthSLong(&TimerRefDec,60))
						{
							sSenSorData.TambCal -= 5;
							if(sSenSorData.TambCal <= tAmbfact)
							{
								sSenSorData.TambCal = tAmbfact;
							}
						}
						(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
					}
				}
			}
			else
			{
				if(tAmbfact > sSenSorData.TambCal)
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,30))
					{  //不能加2 只能加1
						sSenSorData.TambCal += 5;
					   if(sSenSorData.TambCal >= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}
				else if(sSenSorData.TambCal!=tAmbfact)
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,30))
					{
						sSenSorData.TambCal -= 5;
						if(sSenSorData.TambCal <= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
				}
				else
				{
					(void)TimeOutChkTenthSLong(&TimerRefAdd_TD, 0);
					(void)TimeOutChkTenthSLong(&TimerRefDec_TD, 0);
				}
			}
		}
	}
	if(sSenSorData.TambCal > SCD_T(500))
	{
		sSenSorData.TambCal =  SCD_T(500);
	}
	else{}
	sSenSorData.TambCal_TD = sSenSorData.TambCal;


//	if (bTambDelayFlg)
//	{
//		if (TimeOutChkSLong(&TimerTambDelay, 1800))
//		{
//			bTambDelayFlg = 0;
//		}
//		sSenSorData.TambCal = tAmbCal_Init;
//	}
//	else
//	{
//
//	}

	//sSenSorData.TinCal = SCD_T(25);
	//sSenSorData.TambCal = SCD_T(15);
   //车内信号刷新
	if(DTC[DTCNUM_TIN] & 0x01)
	{
		sSenSorData.TinCal = _TIN_DEFAULT_;//2014-2-16 12:42:07更改，车内传感器故障，面板默认为20℃
	}
	else
	{
		tInfact = SigF_GetResult(SIG_NUM_TIN);
//			tInfact = ADPhy_GetFact(ADID_TIN);  //暂时使用采集值
		if(TimeOutChkSLong(&TimerReftin, 2))
		{   //refresh tin 0.5 every10S  0.1 2S
			if(tInfact > sSenSorData.TinCal)
			{
				sSenSorData.TinCal++;
			}
			else  if(tInfact != sSenSorData.TinCal)
			{
				sSenSorData.TinCal--;
			}
			else{}
		}else{}
	}
   if (sSenSorData.TinCal >= SCD_T(75))
   {
   	sSenSorData.TinCal = SCD_T(75);
   }else{}

   sSenSorData.TinSimul = sSenSorData.TinCal;

   //蒸发温度
	if(DTC[DTCNUM_TEVP] & 0x01)
	{
		sSenSorData.TevpCal = _TEVP_DEFAULT_;
	}
	else
	{
		sSenSorData.TevpCal = SigF_GetResult(SIG_NUM_TEVP);
//		sSenSorData.TevpCal = ADPhy_GetFact(ADID_TEVP);  //暂时使用采集值
	}

	//吹脸信号刷新
//	if(DTC[DTCNUM_FVENT_BACKERR] & 0x01)
//	{
//		sSenSorData.TVentF = _VENT_DEFAULT_;
//	}
//	else
//	{
//		sSenSorData.TVentF = SigF_GetResult(SIG_NUM_VENTF);
////		sSenSorData.TVentF = ADPhy_GetFact(ADID_VENTF);  //暂时使用采集值
//	}

	//吹脚信号刷新
//	if(DTC[DTCNUM_DVENT_BACKERR] & 0x01)
//	{
//		sSenSorData.TVentD = _FOOT_DEFAULT_;
//	}
//	else
//	{
//		sSenSorData.TVentD = SigF_GetResult(SIG_NUM_VENTD);
////		sSenSorData.TVentD = ADPhy_GetFact(ADID_VENTD);  //暂时使用采集值
//	}
   //阳光信号刷新        0-125 60s
	//uint8 adsun = Get_8ad(ADID_SUN);
	sSenSorData.u8ADSunR = ADValue_Get(ADID_SUN, ADVALUE_GETTYPE_8AD);
	//sSenSorData.TwaterCal = _TWATER_DEFAULT_;


	if(DTC[DTCNUM_PT1TEMP] & 0x01)
	{
		sEVSensorData.TPL1_Temp = _TPRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPL1_Temp = SigF_GetResult(SIG_NUM_PT1TEMP);
	}

	if(DTC[DTCNUM_PT2TEMP] & 0x01)
	{
		sEVSensorData.TPL2_Temp = _TPRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPL2_Temp = SigF_GetResult(SIG_NUM_PT2TEMP);
	}

	if(DTC[DTCNUM_PTHTEMP] & 0x01)
	{
		sEVSensorData.TPH_Temp = _TPRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPH_Temp = SigF_GetResult(SIG_NUM_PT3TEMP);
	}

//	if(DTC[DTCNUM_PTCTEMP] & 0x01)
//	{
//		sHPSenSorData.tPTCTemp = _TPTC_DEFAULT_;
//	}
//	else
//	{
//		sHPSenSorData.tPTCTemp = SigF_GetResult(SIG_NUM_PTCTEMP) + 800;
//	}

	if(DTC[DTCNUM_PT1PRESS] & 0x01)
	{
		sEVSensorData.TPL1_Press = _PRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPL1_Press = SigF_GetResult(SIG_NUM_PT1PRESS);
	}

	if(DTC[DTCNUM_PT2PRESS] & 0x01)
	{
		sEVSensorData.TPL2_Press = _PRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPL2_Press = SigF_GetResult(SIG_NUM_PT2PRESS);
	}

	if(DTC[DTCNUM_PTHPRESS] & 0x01)
	{
		sEVSensorData.TPH_Press = _PRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPH_Press = SigF_GetResult(SIG_NUM_PT3PRESS);
	}

	//if(DTC[DTCNUM_SUN_BACKERR] & 0x01)
	//{
	//	sSenSorData.u8ADSunR = 0xff;
	//}
	//else
	//{
		sSenSorData.u8ADSunR = SigF_GetResult(SIG_NUM_SUN);
	//}
//信号设置

	sVehicleControl.i16BackEACSpeed = (sCANMsg490.Bits.COMPSPD_1 <<8) + sCANMsg490.Bits.COMPSPD_2;
	sVehicleControl.i16PTCUsePower = sCANMsg432.Bits.AIRPTC_W_HVHTR_POWER *50;

}
/*******************************************************************************
 * Function:        void Signal_Refresh_EndInit(void)
 *
 * Description:     结束初始化流程
 *
 * Returns:         none
 *
 * Notes:           初始化流程结束时调用
 *                 
********************************************************************************/
static void Signal_Refresh_EndInit(void)
{
	//TODO: 编写初始化结束后的 控制流程，模板实现外温初始化逻辑，如不需要可删除
	if (bInitTamb == 0x5a)
	{

	}
	else
	{//是第一次上电的标记
		bInitTamb = 0x5a;
	}
}

/*******************************************************************************
 * Function:    SigRefresh_IgnON /SigRefresh_IgnOFF
 *
 * Description: 信号刷新通知函数，电源由错误/正常跳变时调用一次
 *
 * Returns:     none
 *
 * Notes:       按照应用要求编写
 *
*******************************************************************************/
void SigRefresh_IgnON(void)
{
	//TODO: 第一次上电的操作
	tWaterCal_Last = sSenSorData.TwaterCal;
	u8SigRefState = SIGREF_STATE_INIT;
	(void)TimeOutChkTenthSLong(&TimerInit, 0);
	i16TambSum = 0;
	tAmbCal_Init_1 = ADPhy_GetFact(ADID_TAMB);
	u8AddNum = 0;
}


void SigRefresh_IgnOFF(void)
{
	//u8SigRefState = SIGREF_STATE_STOP; //如果IGN掉电后，不需要采集刷新温度
}

/*******************************************************************************
 * Function:  void SignalCAN_RXComplete(uint32 u32Id)
 *
 * Description:  can rxcomplete
 *
 * Returns:      none
 *
 * Notes:     u32Id： 接收到的CAN ID
 *        
*******************************************************************************/
void SignalCAN_RXComplete(uint32 u32Id)
{
	//TODO: Add code to read Frame singal here
	switch(u32Id)
	{		
		default: 
			break;
		case R_ID_12F85050:
			sSenSorData.u8VehicleSpeed = sCANMsg12F85050.Bits.C_VSP;
			break;
	}
}

void SignalCAN2_RXComplete(uint32 u32Id)
{
	//TODO: Add code to read Frame singal here
	switch(u32Id)
	{
		default:
			break;
		case R_ID_22F:
			sVehicleControl.i16PowerLimte = (sCANMsg22F.Bits.VCU_TO_TMU_TMS_POWER_LIMIT_1 << 8) + sCANMsg22F.Bits.VCU_TO_TMU_TMS_POWER_LIMIT_2;
			break;
		case R_ID_424:
			sVehicleControl.bPump = sCANMsg424.Bits.PUMPEN;
			if (sCANMsg424.Bits.BMSHEATREQUEST >= 2)
			{
				sVehicleControl.bBattHotReq = 1;
			}
			else
			{
				sVehicleControl.bBattHotReq = 0;
			}

			sEVSensorData.BattInWater = sCANMsg424.Bits.INLETCURRENTTEMP*5 -400;
			sEVSensorData.BattOutWater = sCANMsg424.Bits.OUTLETCURRENTTEMP*5 -400;
			sEVSensorData.BattTargetWater = sCANMsg424.Bits.INLETTARGETTEMP*5 -400;
			sEVSensorData.BattTemp = sCANMsg424.Bits.AVERAGEBATTEMP*5 -400;
			sVehicleControl.bBattCoolReq = sCANMsg424.Bits.BMSCOOLREQUEST;
			break;
		case R_ID_26D:
			sSenSorData.TwaterCal = sCANMsg26D.Bits.HVHTR_OUTLET_TEMP *10 -400;
			break;
		case R_ID_131:
			sEVSensorData.CurrentMotorTemp = sCANMsg131.Bits.CURRENTMOTORTEMP*10 -400;
			sEVSensorData.CurrentIGBTemp = sCANMsg131.Bits.CURRENTIGBTTEMP *10 -400;
			break;
		case R_ID_460:
			sEVSensorData.OBCtemp = sCANMsg460.Bits.OBCTEMP*10 -550;
			break;
		case R_ID_470:
			sEVSensorData.DCDCbodyTemp = sCANMsg470.Bits.DCDCBODYTEMP*10 -550;
			break;
	}
}
/*******************************************************************************
 * Function:  void SignalCAN_RecTimeOut(uint32 u32Id)
 *
 * Description:  can rx timeout
 *
 * Returns:      none
 *
 * Notes:     u32Id： 接收到的CAN ID
 *        
*******************************************************************************/
#define   BATTWATER_DEFAULT_TEMP   SCD_T(25)
#define   SENSOR_DEFAULT_TEMP   SCD_T(25)
void SignalCAN_RecTimeOut(uint32 u32Id)
{
	//TODO:当信号超时的处理
	switch(u32Id)
	{
		default:
			break;
		case R_ID_12F85050:
			sSenSorData.u8VehicleSpeed = 0;
			break;
	}
}

void SignalCAN2_RecTimeOut(uint32 u32Id)
{
	//TODO:当信号超时的处理
	switch(u32Id)
	{
		default:
			break;
		case R_ID_22F:
#if PCDEBUG_CONFIG > 0
			sVehicleControl.i16PowerLimte = 5000;
#else
			sVehicleControl.i16PowerLimte = 1000;
#endif

			break;
		case R_ID_26D:
			sSenSorData.TwaterCal = _TWATER_DEFAULT_;
			break;
		case R_ID_424:
			sVehicleControl.bPump = 0;
			sVehicleControl.bBattHotReq = 0;
			sEVSensorData.BattInWater = SCD_T(25);
			sEVSensorData.BattOutWater = SCD_T(25);
			sEVSensorData.BattTargetWater = SCD_T(25);
			sEVSensorData.BattTemp = SCD_T(25);
#if PCDEBUG_CONFIG > 0
			sVehicleControl.bBattCoolReq = 0;
			sVehicleControl.bBattHotReq = 0;
#endif
			break;
		case R_ID_131:
			sEVSensorData.CurrentMotorTemp = SENSOR_DEFAULT_TEMP;
			sEVSensorData.CurrentIGBTemp = SENSOR_DEFAULT_TEMP;
			break;
		case R_ID_460:
			sEVSensorData.OBCtemp = SENSOR_DEFAULT_TEMP;
			break;
		case R_ID_470:
			sEVSensorData.DCDCbodyTemp = SENSOR_DEFAULT_TEMP;
			break;
	}
}
/*******************************************************************************
* Function:      int16 SigF_GetCurrentState(uint8 u8Num)
*
* Description:   sigfilter 模块驱动函数 
                 
* return :       返回当前状态
* Note  :        请按模块说明文件编写

************************************************************************************/
int16 SigF_GetCurrentState(uint8 u8Num)
{
	//TODO:sigfilter 模块驱动函数
	int16 i16SigData;
	switch(u8Num)
	{
		default:
			i16SigData = 0xffff;
			break;
		case SIG_NUM_TIN:
			i16SigData = SCD_T(25);//ADPhy_GetFact(ADID_TIN);
			break;
		case SIG_NUM_TAMB:
			i16SigData =  ADPhy_GetFact(ADID_TAMB);
			break;
		case SIG_NUM_TEVP:
			i16SigData = ADPhy_GetFact(ADID_TEVP);
			break;
//		case SIG_NUM_VENTF:
//			i16SigData =  ADPhy_GetFact(ADID_VENTF);
//			break;
//		case SIG_NUM_VENTD:
//			i16SigData =  ADPhy_GetFact(ADID_VENTD);
//			break;
		case SIG_NUM_PT1TEMP:
			i16SigData =  Signal_GetAdT (ADValue_Get(ADID_TPTL1, ADVALUE_GETTYPE_10AD), &ADT_PTH_TEMP);
			break;
		case SIG_NUM_PT2TEMP:
			i16SigData =  Signal_GetAdT (ADValue_Get(ADID_TPTL2, ADVALUE_GETTYPE_10AD), &ADT_PTH_TEMP);
			break;
		case SIG_NUM_PT3TEMP:
			i16SigData =  Signal_GetAdT (ADValue_Get(ADID_TPTH, ADVALUE_GETTYPE_10AD), &ADT_PTH_TEMP);
			break;
//		case SIG_NUM_PTCTEMP:
//			i16SigData =  ADPhy_GetFact(ADID_PTC);
//			break;

		case SIG_NUM_PT1PRESS:
			i16SigData = Signal_GetAd_Press(1,ADValue_Get(ADID_PPTL1, ADVALUE_GETTYPE_10AD));
			break;
		case SIG_NUM_PT2PRESS:
			i16SigData = Signal_GetAd_Press(1,ADValue_Get(ADID_PPTL2, ADVALUE_GETTYPE_10AD));
			break;
		case SIG_NUM_PT3PRESS:
			i16SigData = Signal_GetAd_Press(0,ADValue_Get(ADID_PPTH, ADVALUE_GETTYPE_10AD));
			break;
		case SIG_NUM_SUN:
			i16SigData = ADValue_Get(ADID_SUN, ADVALUE_GETTYPE_8AD);
			break;
	}
	return i16SigData;
}

