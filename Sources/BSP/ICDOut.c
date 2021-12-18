/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：ICDOut.c
* 功能描述： 通过硬线输出在 OutTask中实现
* 还需要实现
* 1 TrOut_5VEN 
* 2 OUT_AllClose/OUT_AllOpen
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
*  版本：1.1
* 说明：电机设置0xff为停机
* 原作者：xb
* 完成日期：2020年10月21日
*/
#include "ICDOut.h"




/*******************************************************************************
 * Function: void TrOut_5VEN(uint8 en)
 *
 * Description: 隔离电源控制
 *
 * Returns:     none
 *
 * Notes:    此函数不能删除,O_5VEN 为固定宏定义
 *
*******************************************************************************/
void TrOut_5VEN(uint8 en)
{
	if (en)
	{
		O_5VEN = GPIO_SET;
	}
	else
	{
		O_5VEN = GPIO_CLEAR;
	}
}


/*******************************************************************************
 * Function: void	Out_AllClose(void)
 *
 * Description: 所有输出关闭
 *
 * Returns:     none
 *
 * Notes:
 *
*******************************************************************************/
void	OUT_AllClose(void)
{
	//TODO: 编写所有输出关闭,风量功能保留
	OUTFAN_PWM = GPIO_CLEAR;
	O_FAN_RELAY = GPIO_CLEAR;
}

/*******************************************************************************
 * Function: void	Out_AllOpen(void)
 *
 * Description: 所有输出开启,自检调试使用
 *
 * Returns:     none
 *
 * Notes:    不关5ven
 *
*******************************************************************************/
void	OUT_AllOpen(void)
{
	//TODO: 编写所有输出开启,风量功能保留
	OUTFAN_PWM = PWMMODE_FAN/2;
	O_FAN_RELAY = GPIO_SET;
}


/*******************************************************************************
 * Function: void	OutTask(void)
 *
 * Description: 输出主任务
 *
 * Returns:     none
 *
 * Notes:
 *
*******************************************************************************/
void OutTask(void)
{
	//TODO: icd 输出在此实现,风量和电机控制保留，其他可见模板
	(void)Motor_SetPosition(MOTOR_ID_MODE, sOutData.MODEAD);//可直接设置，挡位0xff时为停止，注意电机模块为1.1以上
  
	(void)FanControl_SetVolt(sOutData.FANVolt); 
}

