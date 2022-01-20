/************************文件说明************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司   All rights reserved.
*
* 文件名称：CANDriveWake.c
* 功能描述：实现CAN 休眠，唤醒功能
* 编译器： codewarrior IDE 10.7
* MCU说明：TJA1040 底层驱动, KEAZ128 kbi唤醒
*/
/**************************更改记录******************************************************
 *  此文件没特殊情况不能被更改
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日
* 
* 版本：1.1
* 说明：KBI 端口使用宏定义在CANDrive.h 中实现。在使用KEAZ128和TJ1040 系列 时此文件不用更改
* 原作者：xb
* 完成日期：2019年10月22日
*
* 版本：1.2
* 说明：  CANDrive_SleepIO(0);	
	      Init_Mscan();  方向反一下
* 原作者：xb
* 完成日期：2019年12月24日

*/
#include "CAN2Drive.h"


//CAN 中断唤醒 底层驱动变量定义
static	uint8	bCANWake;            //当前can是否存在工作状态， 1: 为正在工作，或者被唤醒， 0：can发送已经停止可以休眠 
static  	uint8	bCANWakeBegin;       //是否可以使能唤醒 1：可以， 0：不可以
static  	uint8	bCANKBIInd;          //中断标记
#if _CANWAKE_FUN_
static uint16 TimerCANWake;     //canwke定时器
static uint8  u8CANWakeTimes;      //计时100ms进入一次
#endif
/*******************************************************************************
 * Function:     void CANDrive_SleepIO(uint8 En)
 *  *
 * Description:    CAN 睡眠口状态
 *
 * Returns:         none
 *
 * Notes:           En: 1 为休眠    0:为工作
 *
********************************************************************************/
void CAN2Drive_SleepIO(uint8 En)
{
	if (En)
	{
		O_CAN2EN = GPIO_SET;
	}
	else
	{
		O_CAN2EN = GPIO_CLEAR;
	}
}
/*******************************************************************************
* Function:     void Init_KBI_1()
*
* Description:    高低电平中断配置
* 
* return :       none
* 
* Note  :        
************************************************************************************/ 
void Init_KBI2_1(void)
{

	//KBI_SetCallback(KBI0, &KBI0_Task);
	//KBI_SetCallback(KBI1, &KBI1_Task);
}

/*******************************************************************************
* Function:     void Init_KBI_0()
*
* Description:    高低电平中断配置
* 
* return :       none
* 
* Note  :        
************************************************************************************/ 
void Init_KBI2_0(void)
{

}

/*******************************************************************************
 * Function:        uint8 GetCanWakeEn(void)
 *
 * Description:     返回当前CAN 是否在工作1 :为被唤醒。 0： 没有被唤醒
 *                  //如果不带can 唤醒功能，直接返回0即可
 * Returns:         none
 *
 * Notes:           电源管理模块调用
 *
********************************************************************************/
uint8 GetCan2WakeEn(void)
{
#if _CANWAKE_FUN_
	return bCANWake;
#else
	return 0;
#endif
}

/*******************************************************************************
 * Function:      void KBI1_IRQHandler(void)
 *  *
 * Description:     KBI 中断
 *
 * Returns:         none
 *
 * Notes:           none
 *
********************************************************************************/
void KBI21_IRQHandler(void)
{

}

void KBI20_IRQHandler(void)
{

}
/*******************************************************************************
 * Function:        void Deal_CANWake(void)
 *  *
 * Description:     判断当前的wakeen 状态
 *
 * Returns:         none
 *
 * Notes:           电源管理模块调用，注意在sleep循环时，也需要调用
 *                实现:
 *                一 工作模式时，
 *                1 通过ind信号判断当前是否有中断
 *                2 在ind信号没有的时候持续2s则认为，无CAN
 *                二 休眠模式时
 *                1 通过ind信号判断当前是否有中断
 *
********************************************************************************/
void Deal_CAN2Wake(void)
{
#if _CANWAKE_FUN_
	if (bCANKBIInd && bCANWakeBegin)
	{
		u8CANWakeTimes = 0;
		bCANWake = 1;
		(void)TimeOutChkMsLong(&TimerCANWake, 0);
	}
	else
	{
		if (TimeOutChkMsLong(&TimerCANWake, 100))
		{
			if (u8CANWakeTimes >= CANUNWAKE_TIME)
			{
				bCANWake = 0;
				bCANWakeBegin = 1;
			}
			else
			{
				u8CANWakeTimes++;
			}
		}
	}
	bCANKBIInd = 0;
#endif
}
/*******************************************************************************
 * Function:       void CANWake_Begin(uint8 bEN)
 *  *
 * Description:    是否使能canwake，1：使能 0：禁止
 *
 * Returns:         none
 *
 * Notes:           电源管理模块调用
 *
********************************************************************************/
void CAN2Wake_Begin(uint8 bEN)
{
	bCANWakeBegin = bEN;
	bCANWake = 0;
	bCANKBIInd = 0;
}

/*******************************************************************************
 * Function:       void CANDrive_GotoSleep(void)
 *  *
 * Description:    can 驱动进入sleep 模式
 *
 * Returns:         none
 *
 * Notes:           电源管理模块调用
 *
********************************************************************************/
void CAN2Drive_GotoSleep(void)
{
	CANDrive_SleepIO(1);
	CANDrive_Close();
#if _CANWAKE_FUN_
	Init_KBI2_1();
#endif
}
/*******************************************************************************
 * Function:      void CANDrive_GotoAWake(void)
 *  *
 * Description:    can 驱动准备进入正常工作 模式
 *
 * Returns:         none
 *
 * Notes:           电源管理模块调用
 *
********************************************************************************/
void CAN2Drive_GotoAWake(void)
{
	CAN2Drive_SleepIO(0);	
	Init_Mscan2();  
#if _CAN2WAKE_FUN_
	Init_KBI2_0();
#endif
}
/*******************************************************************************
 * Function:     void CANGet_Fram(void)
 *  *
 * Description:    接收到can帧
 *
 * Returns:         none
 *
 * Notes:           底层candrive调用
 *
********************************************************************************/
void CAN2Get_Fram(void)
{
	bCANKBIInd = 1;
}

