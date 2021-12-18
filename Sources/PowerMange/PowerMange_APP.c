/************************文件说明*****************************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：PowerMange—_App.c
* 功能描述：电源管理回调函数接口
* 提供接口函数，具体内容按应用要求编写
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
* 
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月19日
* 
* * 版本：1.1
* 说明：增加 ApplPM_SleepEnter 和 ApplPM_SleepReady 回调函数
* 作者：xb
* 完成日期：2019年3月30日
* 
*  版本：1.2
*  作者：xb
* 完成日期：2019年10月21日
* 
* 版本：1.3
* 说明： 跟随pm 1.3版本
*  作者：xb
* 完成日期：2019年10月21日
*
* 版本：1.4
* 说明： 跟随pm 1.4版本
*  作者：xb
* 完成日期：2019年12月24日
* 
* * * 版本：1.5
* 说明：跟随.c文件
* * 作者：xb
* 完成日期：2020年3月7日
* 
* * * * 版本：1.5.1
* 说明：1 增加 MCUPMLogic 模块的驱动
* 2 在2575电路时，IGN ON 时才能开启5ven
* 作者：xb
* 完成日期：2020年5月13日
* 
* 版本：1.6
* 说明：
* 1) 配合1.6 的pm
* 作者：xb
* 完成日期：2020年5月30日
*/
#include "DataType.h"
#include "APPDrive.h"
#include "PowerMange.h"

/*******************************************************************************
需要外部调用函数声明
********************************************************************************/
extern void PowerOffTask_Enter(void);
extern void PowerStandByTask_Enter(void);
extern void SigRefresh_IgnOFF(void);
extern void SigRefresh_IgnON(void);
extern void System_GOTOInitTask(void);
//extern void Lin_Init(void);

/*******************************************************************************
 * Function:       void ApplPM_IgnOFF(void)
 *
 * Description:    电源管理从POWER_STATE_WORK->POWER_STATE_OFF时调用一次
 *
 * Returns:         none
 *
 * Notes:          回调函数下同； 编写掉电后需要把执行的内容
 *
********************************************************************************/
void ApplPM_IgnOFF(void)
{
	PowerOffTask_Enter();       //进入offtask的通知task
	SigRefresh_IgnOFF();       //通知信号刷新IgnOFF了
}
/*******************************************************************************
 * Function:      void ApplPM_IgnON(void)
 *
 * Description:    电源管理从POWER_STATE_INIT/POWER_STATE_OFF/ POWER_STATE_STANDBY
 *                                                  ->POWER_STATE_ON 时调用一次
 *
 * Returns:         none
 *
 * Notes:        电源从OFF到ON 时的处理内容
 *
********************************************************************************/
void ApplPM_IgnON(void)
{
#if (_PROJECT_LDO_MODE == LDO_MODE_2575)
	TrOut_5VEN(1);                    //开启2575
#endif
	System_GOTOInitTask();            //通知系统状态进入初始化模式
	Motor_Reset();                    //电机初始化
	SigRefresh_IgnON();               //通知信号刷新模块
	MCUPM_IgnOn();                    //MCU PM 模块固定用法
#if _PM_AUTOSARNM_
	CanNm_NetworkRequest(0);
#endif
}


/*******************************************************************************
 * Function:      void ApplPM_SleepReady(void)
 *
 * Description:     进入STANDBY_WAIT 状态，准备睡眠的通知APP
 *
 * Returns:         none
 *
 * Notes:       此时CANEN已经关掉，会等待1s后进入sleep模式，这1s可以处理一些需要延时才能完成的任务
 *              比如： 发送1个LIN睡眠信号。
 *
********************************************************************************/
void ApplPM_SleepReady(void)
{

}

/*******************************************************************************
 * Function:      void ApplPM_SleepEnter(void)
 *
 * Description:     进入SLEEP 前通知APP
 *
 * Returns:         none
 *
 * Notes:       需要关闭所有输出来降低功耗
 *
********************************************************************************/
void ApplPM_SleepEnter(void)
{
	Mcp_Close();
	MCUPM_Over();
	Init_GPIO_0(); 
}
/*******************************************************************************
 * Function:      void ApplPM_WakeUp(void)
 *
 * Description:    电源管理休眠到唤醒时调用一次
 *
 * Returns:         none
 *
 * Notes:        CAN和点火唤醒都会调用 如果直接复位将不会调用
 *
********************************************************************************/
void ApplPM_WakeUp(void)
{
	Init_GPIO_1(); 
	//Lin_Init();
	
	System_GOTOInitTask();
}
/*******************************************************************************
 * Function:      void ApplPM_WakeUp(void)
 *
 * Description:    电源管理由于CAN唤醒时调用一次
 *
 * Returns:         none
 *
 * Notes:        如果直接复位将不会调用
 *
********************************************************************************/
void ApplPM_CANWakeUp(void)
{

}


void ApplPM_StateChange(const PMStateType pmPreviousState, const PMStateType pmCurrentState)
{
	
	switch (pmCurrentState)
	{//切换后的状态
		default:
		case	POWER_STATE_INIT:
			break;
		case POWER_STATE_WORK:
			ApplPM_IgnON();
			break;
		case POWER_STATE_OFF:
			if (pmPreviousState != POWER_STATE_STANDBY)
			{
				ApplPM_IgnOFF();
			}
			else{}
			break;
		case POWER_STATE_SLEEP:
			ApplPM_SleepEnter();
			break;
		case POWER_STATE_STANDBY:
			PowerStandByTask_Enter();
			break;
		case POWER_STATE_STANDBY_WAIT:
			ApplPM_SleepReady();
			break;	
	}
	
	if (pmPreviousState == POWER_STATE_SLEEP && pmCurrentState == POWER_STATE_INIT)
	{
		ApplPM_WakeUp();
	}
	
}
