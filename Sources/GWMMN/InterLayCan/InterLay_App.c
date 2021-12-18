/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：InterLay_App.c
* 功能描述：InterLay层 的回调函数实现，具体内容需要根据实际项目更改
* 1  InterLay层 的回调函数
* 2  InterlAY 层的数据设置， 
* 3   超时数据设置
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日
* 
* 版本：1.2
* 说明：适用于1.2的模板
* 原作者：xb
* 完成日期：2019年10月25日

* 版本：2.3
* 说明：适用于InterLaycan 2.3的模板
* 原作者：xb
* 完成日期：2020年1月2日
* 
* 版本：2.4
* 说明：
* 1 部分id 不是常用ID 进行删除，
* 2 语音功能调用删除
* 3 循环发送DTC 功能删除
* 用法见参考文件
* 原作者：xb
* 完成日期：2020年7月14日
* 
* * 版本：2.5
* 说明：
* 1 timeout关联  SignalCAN_RecTimeOut(u32Id);
* 
* 原作者：xb
* 完成日期：2020年7月14日
* 
*/

#include "InterLayMatrix.h"
#include "APPDrive.h"
#include "AppLayInterface.h"


volatile uint8 u8APPTest;
volatile uint8 u8Test1;

/*******************************************************************************
*   Call back functions for INTERLAY Module
*******************************************************************************/
void MP5Logic_RecTask(void);
void MP5Logic_TimeOutTask(void);

 /*******************************************************************************
 * Function:  void APPlIL_RXComplete(uint8 u8RT, uint32 u32Id)
 *
 * Description:  app 数据发送完成通知app
 *
 * Returns:      none
 *
 * Notes:     u32Id： 接收到的CAN ID
 *        
*******************************************************************************/
void SignalCAN_RXComplete(uint32 u32Id);
void SignalCAN_RecTimeOut(uint32 u32Id);
void APPlIL_RXComplete(uint32 u32Id)
{
	//TODO: Add code to read Frame singal here
	SignalCAN_RXComplete(u32Id);
	switch(u32Id)
	{
		default: 
			break;

	}
}
/*******************************************************************************
* Function:  void APPlIL_TXComplete(uint8 u8IfId)
*
* Description:  app 数据发送完成通知app
*
* Returns:      none
*
* Notes:     u8IfId：发送的CAN IFID
*        
*******************************************************************************/
void APPlIL_TXComplete(uint8 u8IfId)
{
	
}
/*******************************************************************************
* Function:  BOOL APPlIL_TXStart(uint8 u8IfId)
*
* Description:  app 数据开始发送的回调函数
*
* Returns:      TRUE 可以发送， FALSE 不可以
*
* Notes:     NONE
*        
*******************************************************************************/
BOOL APPlIL_TXStart(uint8 u8IfId)
{
	//TODO: 信号发送时设置，返回是否发送，可见参考文件
	BOOL bCANSend = TRUE; //默认可开启
	
	
	return bCANSend;
}
/*******************************************************************************
* Function:  void APPlIL_RecTimeOut(uint32 u32Id)
*
* Description:  数据接收超时通知app
*
* Returns:      none
*
* Notes:       u32Id ： id
*           
*******************************************************************************/
void APPlIL_RecTimeOut(uint32 u32Id)
{
	SignalCAN_RecTimeOut(u32Id);
	switch(u32Id)
	{
		default:
			break;
	}
}

