/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：CommSet.c
* 功能描述： CAN LIN 信号设置，在 CommSetTask()中实现
* 1 100ms设置一次，其他特殊功能需要在InterLay_App.c中实现
* 编译器： codewarrior IDE 11.1
* MCU说明：和MCU 无关
*  
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月13日
* 
*/
#include "CommSet.h"
#include "APPDrive.h"
#include "OutANDDisplay.h"
#if _PROJECT_CAN_
#include "InterLayMatrix.h"
#endif



/*******************************************************************************
 * Function:        void CommSetTask(void)
 *
 * Description:     信号设置，底层驱动loop调用 
 *
 * Returns:        none
 *
 * Notes:           		
 *
********************************************************************************/
void CommSetTask(void)
{
	static uint16 TimerSetMessage;
	
	if (TimeOutChkMsLong(&TimerSetMessage, 10))
	{//TODO: 信号值发送在此处编写

	}
	else{}
	
	
	//实现回话模式的切换立即发送见模板
}
