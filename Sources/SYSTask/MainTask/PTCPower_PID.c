/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：PTCPower_PID.c
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
#include "PTCPower_PID.h"
#include "APPDrive.h"



void PTCPowerPID_GetState(void);
PIDDATA   sPTCPowerPidData;
PIDPARAMETER  sPTCPowerPar = {50, 50, 50, 0, 500, 10000, 0,5, 300, 200, 5000, PTCPowerPID_GetState};
PIDPARAMETER  sBatPTCPowerPar = {50, 50, 50, 0, 500, 10000, 0,5, 300, 200, 5000, PTCPowerPID_GetState};
static BOOL bBathEn;


/*******************************************************************************
 * Function:      int16 Deal_PTCPowerPID(void)
 *
 * Description:    PID 控制
 *
 * Returns:         none
 *
 * Notes:           
 *
********************************************************************************/
int16 Deal_PTCPowerPID(void)
{
	if (sPTCPowerPidData.u8RunState == PIDRUN_STATE_INIT && sPTCPowerPidData.i16Get > sPTCPowerPidData.i16Set)
	{//运行时 温度已经到达目标值可直接进入运行
		sPTCPowerPidData.u8RunState = PIDRUN_STATE_RUN;
	}
	else{}

	if (bBathEn)
	{
		return Deal_PIDFun(&sPTCPowerPidData, &sPTCPowerPar);
	}
	else
	{
		return Deal_PIDFun(&sPTCPowerPidData, &sBatPTCPowerPar);
	}
}

void PTCPowerPID_GetState(void)
{
	//sPTCPowerPidData.i16Get = sSenSorData.Tadt;
	//sPTCPowerPidData.i16Set = sSenSorData.Tddt;
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
int16 PTCPowerPID_Init(void)
{
	//混合控制 初始化
	(void)TimeOutChkTenthSLong(&sPTCPowerPidData.Timer01, 0);
	return 1;//GAMath_LineFuni16(TSET_LO, TSET_HI, 0, 1000, SystemControl.tset);
}


void  PTCPowerPIDMode(uint8 bBathMode)
{
	bBathEn = bBathMode;
}
