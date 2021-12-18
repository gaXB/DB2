/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：NM_APP.c
* 功能描述：CAN 通讯是否使能功能模块，为应用层，虽为模板，但不用更改
* 可用在接间和osek网络管理，都使用此文件
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日

* 版本：1.1 
* 说明：支持autosar
* 原作者：xb
* 完成日期：2019年12月24日
*/


#include "NM_APP.h"
#include "CanDrive.h"

static uint8 bCANCOMRecEn, bCANCOMSendEn;  //应用保文接受使能
static uint8 bCANCOMDelay;
static uint16 TimertApp;

/*******************************************************************************
 * Function:       void Com_RxStart(void)  void Com_RxStop(void)
 *                 void Com_TxStart(void)   void Com_TxStop(void)
 * Description:     com 发送，接收 使能关闭
 *
 * Returns:         none
 *
 * Notes:          应用层调用 
 *
********************************************************************************/
void Com_RxStart(void)
{
	bCANCOMRecEn = 1;
}

void Com_RxStop(void)
{
	bCANCOMRecEn = 0;
}

void Com_TxStart(void)
{
	bCANCOMSendEn = 1;
}

void Com_TxStop(void)
{
	bCANCOMSendEn = 0;
}
/*******************************************************************************
 * Function:       void Com_TxEnDealy(void)
 *                
 * Description:     com tx 发送延时
 *
 * Returns:         none
 *
 * Notes:          应用层调用 
 *
********************************************************************************/
void Com_TxEnDealy(void)
{
	bCANCOMDelay = 1;
	(void)TimeOutChkMsLong(&TimertApp, 0);
}

/*******************************************************************************
 * Function:      uint8 ComGetTxEn(void)
 *                
 * Description:   com 发送使能值 ，返回
 *
 * Returns:         none
 *
 * Notes:          交互层调用 
 *
********************************************************************************/
uint8 AutoSarNM_TxEn(void);
uint8 ComGetTxEn(void)
{
	if (bCANCOMDelay)
	{
		if (TimeOutChkMsLong(&TimertApp, APPSEND_DELAY_TIME))
		{
			bCANCOMDelay = 0;
		}
		return 0;
	}
#if _NEED_AUTOSARNM_ == 0
	return bCANCOMSendEn;
#else
	return (AutoSarNM_TxEn() && bCANCOMSendEn);
#endif
	
}

