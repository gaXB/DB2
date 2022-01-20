/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：HotSpeed_PID.c
* 功能描述：混合电机PID
* 出风口温度PID 控制,不用更改
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年11月21日
* 
*/

#include "TempFilter.h"
#include "HotSpeed_PID.h"
#include "APPDrive.h"



void HotSpeedPID_GetState(void);
PIDDATA   sHotSpeedPidData;
PIDPARAMETER  sHotSpeedPar = {50, 50, 50, 0, 500, 10000, 0,5, 300, 800, 10000, HotSpeedPID_GetState};
PIDPARAMETER  sBatHotSpeedPar = {50, 50, 50, 0, 500, 10000, 0,5, 300, 800, 10000, HotSpeedPID_GetState};

static BOOL bBathEn;

/*******************************************************************************
 * Function:      int16 Deal_HotSpeedPID(void)
 *
 * Description:    PID 控制
 *
 * Returns:         none
 *
 * Notes:           
 *
********************************************************************************/
int16 Deal_HotSpeedPID(void)
{
	if (sHotSpeedPidData.u8RunState == PIDRUN_STATE_INIT && sHotSpeedPidData.i16Get > sHotSpeedPidData.i16Set)
	{//运行时 温度已经到达目标值可直接进入运行
		sHotSpeedPidData.u8RunState = PIDRUN_STATE_RUN;
	}
	else{}

	if (bBathEn)
	{
		return Deal_PIDFun(&sHotSpeedPidData, &sBatHotSpeedPar);
	}
	else
	{
		return Deal_PIDFun(&sHotSpeedPidData, &sHotSpeedPar);
	}
}

void HotSpeedPID_GetState(void)
{
	//sHotSpeedPidData.i16Get = sSenSorData.Tadt;
	//sHotSpeedPidData.i16Set = sSenSorData.Tddt;
}

/*******************************************************************************
 * Function:        void HmixPID_Init(void)
 *
 * Description:    混合PID iniit
 *
 * Returns:         none
 *
 * Notes:           得到 
 *
********************************************************************************/	
int16 HotSpeedPID_Init(void)
{
	//混合控制 初始化
	(void)TimeOutChkTenthSLong(&sHotSpeedPidData.Timer01, 0);
	return 1;//GAMath_LineFuni16(TSET_LO, TSET_HI, 0, 1000, SystemControl.tset);
}


void HotSpeedPIDMode(uint8 bBathMode)
{
	bBathEn = bBathMode;
}
