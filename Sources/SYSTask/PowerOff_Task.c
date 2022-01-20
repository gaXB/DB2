/************************文件说明*****************************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：PowerOff_Task.c
* 功能描述：电源OFF时的系统任务处理
* 提供模板 ，按实际应用编写
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月19日
* 
* 版本：1.1
* 说明：枚举类型赋值优化 
* 作者：xb
* 完成日期：2021年8月23日
*/

#include "PowerMange.h"
#include "APPDrive.h"



static uint16 TimerPowerDelay;
static uint8 bDelayEn;    //bDelayEn 代表应用已经任务结束，可以休眠


/*******************************************************************************
 * Function:        void PowerOffTask(void)
 *
 * Description:     电源故障时的工作任务
 *
 * Returns:         none
 *
 * Notes:           主循环调用
 *
********************************************************************************/
void PowerOffTask(void)
{
	//点火电压故障
	//TODO: 在此编写电源OFF时候的处理
	Clear_LCD();
	sDisplayData.AC = 0;
	sDisplayData.AUTO = 0;
	sDisplayData.CIRF = 0;
	sDisplayData.FANLevel = 0;
	sDisplayData.LCDDiagMode = 0;
	sDisplayData.LCDMode = 0;
	sDisplayData.LCheat = 0;
	sDisplayData.LEDMode = 0;
	sDisplayData.MAXAC = 0;
	sDisplayData.MODE_MAXDEF = 0;
	sDisplayData.NEW = 0;
	sDisplayData.RHEAT = 0;
	sDisplayData.RCheat = 0;
	sDisplayData.VentMode = MD_MODE_OSF;
	sDisplayData.i16Temp = 0;
	
	sOutData.ACOut = 0;
	sOutData.FANVolt = 0;
	//sOutData.MIXAD
	//sOutData.MODEAD
	//sOutData.NCFAD
	sOutData.RHeatOut = 0;
	sOutData.u8LCheat = 0;
	sOutData.u8RCheat = 0;

	sEVOutData.AGS_Postion = 0;
	sEVOutData.SOV1 = 0;
	sEVOutData.SOV2 = 0;
	sEVOutData.EWPT1 = 0;
	sEVOutData.EWPT2 = 0;
	sEVOutData.EWPT3 = 0;
	sEVOutData.EACSpeed = 0;
	sEVOutData.PTCPower = 0;
	sEVOutData.IncarFAN_L = 0;
	sEVOutData.IncarFAN_H = 0;
	sEVOutData.ThemalSysRequest = 0;
	sEVOutData.HeatConfirmation = 0;
	sEVOutData.CoolConfirmation = 0;
	sEVOutData.TMU_Realay = 0;
	//(void)NcfMotor_SetPosition(0,NCF_MODE_CIRF);
	//NcfMotor_Control();

   //	LinControl();
	
	if (TimeOutChkMsLong(&TimerPowerDelay, 4000))
	{
		bDelayEn = 1;
	}
	if ( bDelayEn)//NcfMotor_GetState(0) == NCFMOTOR_STATE_HALT &&
	{	
		PowerMN_AppSleepEn(1);
	}
	else
	{}  //继续运行
}

/*******************************************************************************
 * Function:        void PowerOffTask_Enter(void)
 *
 * Description:     进入poweroff 模式
 *
 * Returns:         none
 *
 * Notes:           进入时由pm_app外部调用
 *
********************************************************************************/
void PowerOffTask_Enter(void)
{
	(void)TimeOutChkMsLong(&TimerPowerDelay, 0);
	bDelayEn = 0;
	PowerMN_AppSleepEn(0);
}
