/**************************文件说明*****************************************************
* Copyright (c) 2021 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：BU9796_Drive.c
* 功能描述：BU9796 驱动模块，具有如下功能
* 1  液晶屏驱动，最多10个字节的LCD数据显示
* 2  实现20ms 更新一次数据
* 
* 本模块使用需要IIC模块
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xwl
* 完成日期：2021年10月21日
* 
*/
#include"BU9796_Drive.h"
#include"timerout.h"
#include"IIC_Drive_Software.h"


static uint8* u8HtData = PDATA_NULL;         //LCD 的指针
static uint8 u8HtSize = BU9796_MAXLCDNUM;   //需要传入的数据长度


extern void BU9796APP_LCDSet(void);
 /*******************************************************************************
 * Function:   BU9796_Init()
 *
 * Description: 初始化BU9796 ，可放在系统初始化，或者在主函数中每1s初始化一次
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
static void  BU9796_Init(void)
{
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(ICSET_RESET); 
	IIC_Send_Byte(MODE_SET_OFF); 	
	IIC_Stop_Bit();
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(ICSET); 
	IIC_Send_Byte(DISPLAY_CONTROL);  //开液晶驱动
	IIC_Send_Byte(BLINK_CONTROL);
	IIC_Send_Byte(AP_CONTROL);
	IIC_Send_Byte(MODE_SET_ON);
	IIC_Stop_Bit();

}
/*******************************************************************************
* Function:   BU9796_LCDDataSet(uint8* u8LCDData, uint8 u8Length)
*
* Description: 上电设置一次
*
* Returns:     设置正确 1, 错误 0
*
* Notes:       none
*		
*******************************************************************************/
BOOL BU9796_LCDDataSet(uint8* u8LCDData, uint8 u8Length)
{
	uint8 i=0;
	BOOL BU9796_InitOK;
	if (u8Length > BU9796_MAXLCDNUM)
	{
		BU9796_InitOK = FALSE;
	}
	else
	{
		u8HtData = u8LCDData;
		u8HtSize = u8Length;
		BU9796_InitOK = TRUE;
	}
	BU9796_Init();
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(LCD_FIRST_ADDRESS);
	
	for (i = 0; i < 20; i++)
	{
		IIC_Send_Byte(0);
	}
	IIC_Stop_Bit();
	
	return BU9796_InitOK;
}
 /*******************************************************************************
 * Function:   BU9796_StandbyMode()
 *
 * Description: BU9796休眠，点火关闭处理时调用
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
void  BU9796_StandbyMode(void)
{
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(ICSET);
	IIC_Send_Byte(MODE_SET_OFF); //关液晶驱动，休眠
	IIC_Stop_Bit();
}

 /*******************************************************************************
 * Function:  void  BU9796_Drvie(void)
 *
 * Description: 驱动BU9796,将u8HtData数据传入9796
 *
 * Returns:     none
 *
 * Notes:       none
 *
*******************************************************************************/
static void  BU9796_Drvie(void)
{
	uint8 i=0;
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(ICSET);
	IIC_Send_Byte(DISPLAY_CONTROL);
	IIC_Send_Byte(BLINK_CONTROL);
	IIC_Send_Byte(AP_CONTROL);
	IIC_Send_Byte(MODE_SET_ON);
	IIC_Send_Byte(LCD_FIRST_ADDRESS);
	
	for (i = 0; i < u8HtSize; i++)
	{
		IIC_Send_Byte(u8HtData[i]);
	}
	IIC_Stop_Bit();
	
}

 /*******************************************************************************
 * Function:  BU9796_Updata()
 *
 * Description: 更新9796 ，放在主函数中更新液晶数据
 *
 * Returns:     none
 *
 * Notes:       实现20ms 更新一次数据，1s 初始化一次
 *		
*******************************************************************************/
void  BU9796_Updata(void)
{
	static uint16  timer_int9796,timer_ref9796;	
   
//	if(TimeOutChkMsLong(&timer_int9796,1000))//1s初始化一次
//   {
//		BU9796_Init(); //初始化BU9796
//   }
   
   if (TimeOutChkMsLong(&timer_ref9796, 20))
   {
   	BU9796APP_LCDSet();
   	BU9796_Drvie(); //LCD数据传输给BU9796
   }  
   else{}
}

 /*******************************************************************************
 * Function:  BU9796_Clear()
 *
 * Description:  有需要时调用
 *
 * Returns:   none
 *
 * Notes:     清空BU9796里面的数据,立即执行
 *
*******************************************************************************/
void  BU9796_Clear(void)
{
	uint8 i=0;
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(ICSET); 
	IIC_Send_Byte(MODE_SET_OFF); 	
	IIC_Send_Byte(LCD_FIRST_ADDRESS);
	
	for (i = 0; i < 20; i++)
	{
		IIC_Send_Byte(0);
	}
	IIC_Stop_Bit();
}
