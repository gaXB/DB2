/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：MIXVENT_PID.c
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
#include "MIXVENT_PID.h"
#include "APPDrive.h"



void MIXVENTPID_GetState(void);
PIDDATA_INC sMIXVentPidData;
PIDPARAMETER_INC sMIXVentPar = {50, 45, 20, 0, -100, 100, MIXVENTPID_GetState};



/*******************************************************************************
 * Function:      uint8 Deal_MIXVENTPID(void)
 *
 * Description:    PID 控制
 *
 * Returns:         none
 *
 * Notes:           
 *
********************************************************************************/
int16 Deal_MIXVENTPID(void)
{
	return Deal_PIDFun_INC(&sMIXVentPidData, &sMIXVentPar);
}

void MIXVENTPID_GetState(void)
{
	sMIXVentPidData.i16Get = sSenSorData.Tadt;
	sMIXVentPidData.i16Set = sSenSorData.Tddt;
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
int16 HmixPID_Init(void)
{
	//混合控制 初始化
	(void)TimeOutChkTenthSLong(&sMIXVentPidData.Timer01, 0);
	return GAMath_LineFuni16(TSET_LO, TSET_HI, 0, 1000, SystemControl.tset);
}
