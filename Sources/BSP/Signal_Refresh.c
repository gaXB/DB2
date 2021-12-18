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
void SignalCAN_RecTimeOut(uint32 u32Id)
{
	//TODO:当信号超时的处理
	switch(u32Id)
	{
		default:
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
	int16 i16SigData = 0;
	switch(u8Num)
	{
		default:
			i16SigData = 0xffff;
			break;
		case SIG_NUM_TIN:
			i16SigData = ADPhy_GetFact(ADID_TIN);
			break;
	}
	
	return i16SigData;
}

