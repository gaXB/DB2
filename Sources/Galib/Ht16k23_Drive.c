/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：mcp7708.c
* 功能描述：HT16K23 驱动模块，具有如下功能
* 1  液晶屏驱动，最多10个字节的LCD数据显示
* 2  实现20ms 更新一次数据，1s 初始化一次
* 3  按键读取功能
* 
* 本模块使用需要IIC模块
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原模块的1.0移植
* 原作者：xb
* 完成日期：2019年6月2日
* 
* 版本：1.1
* 说明：修正驱动size 不能更改的问题
* 原作者：xb
* 完成日期：2020年6月2日
*/
#include"Ht16k23_Drive.h"
#include"timerout.h"
#include"IIC_Drive_Software.h"


static uint8* u8HtData = PDATA_NULL;         //LCD 的指针
static uint8 u8HtSize = HT16K23_MAXLCDNUM;   //需要传入的数据长度


extern void Ht16k23APP_LCDSet(void);
 /*******************************************************************************
 * Function:   Ht16k23_Init()
 *
 * Description: 初始化16k23 ，可放在系统初始化，或者在主函数中每1s初始化一次
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
static void  Ht16k23_Init(void)
{
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(MODE_SEGMENGT); //设置模式，软件中断采集按键
	IIC_Stop_Bit();
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(NORMAL_DISPLAY_ON); //开液晶驱动
	IIC_Stop_Bit();
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(KEY_SCAN_PERIOD); //设置按键扫描周期
	IIC_Stop_Bit();
}
/*******************************************************************************
* Function:   Ht16k23_LCDDataSet(uint8* u8LCDData, uint8 u8Length)
*
* Description: 上电设置一次
*
* Returns:     设置正确 1, 错误 0
*
* Notes:       none
*		
*******************************************************************************/
BOOL Ht16k23_LCDDataSet(uint8* u8LCDData, uint8 u8Length)
{
	BOOL bHt16k23_InitOK;
	if (u8Length > HT16K23_MAXLCDNUM)
	{
		bHt16k23_InitOK = FALSE;
	}
	else
	{
		u8HtData = u8LCDData;
		u8HtSize = u8Length;
		bHt16k23_InitOK = TRUE;
	}
	Ht16k23_Init();
	
	return bHt16k23_InitOK;
}
 /*******************************************************************************
 * Function:   Ht16k23_StandbyMode()
 *
 * Description: 16k23休眠，点火关闭处理时调用
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
void  Ht16k23_StandbyMode(void)
{
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(STANDBY_DISPLAY_OFF); //关液晶驱动，休眠
	IIC_Stop_Bit();
}

 /*******************************************************************************
 * Function:  void  Ht16k23_Drvie(void)
 *
 * Description: 驱动16k23,将u8HtData数据传入16k23
 *
 * Returns:     none
 *
 * Notes:       none
 *
*******************************************************************************/
static void  Ht16k23_Drvie(void)
{
	uint8 i=0;
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(LCD_FIRST_ADDRESS);
	
	for (i = 0; i < u8HtSize; i++)
	{
		IIC_Send_Byte(u8HtData[i]);
	}
	IIC_Stop_Bit();
}

 /*******************************************************************************
 * Function:    Ht16k23_Read_Key()
 *
 * Description: 在Read_KeyBoard_Signal函数中调用
 *
 * Returns:     按键值
 *
 * Notes:       K19-K0数据分别存在key_data的19-0位
 *		
*******************************************************************************/
void  Ht16k23_Read_Key(uint32* pKeyData)
{
	static uint8	INT_SIGN,key_data[3];
	static uint16 timer_key16k23;
	 
	if (TimeOutChkMsLong(&timer_key16k23, 40)) //40ms读取一次
	{
		IIC_Start_Bit();
		IIC_Send_Byte(SLAVE_WRITE);
		IIC_Send_Byte(INT_REG_ADDRESS); 
		IIC_Stop_Bit();
		
		IIC_Start_Bit();
		IIC_Send_Byte(SLAVE_READ);
		INT_SIGN=IIC_Receive_Byte(); //读按键中断标记
		IIC_Send_Ack(ACK_NO);
		IIC_Stop_Bit();
			 
		if(INT_SIGN!=0)
		{
			INT_SIGN =0; //清按键中断标记  
			
			IIC_Start_Bit();
			IIC_Send_Byte(SLAVE_WRITE); //设置写模式
			IIC_Send_Byte(KEY_FRIST_ADDRESS); //写按键数据寄存器首地址0x20
			IIC_Stop_Bit();
			
			IIC_Start_Bit();
			IIC_Send_Byte(SLAVE_READ); //设置读模式
			key_data[0]=IIC_Receive_Byte(); //读地址为0x20的按键值8个
			IIC_Send_Ack(ACK_YES);
			key_data[1]=IIC_Receive_Byte(); //读地址为0x21的按键值8个
			IIC_Send_Ack(ACK_YES);
			key_data[2]=IIC_Receive_Byte(); //读地址为0x22的按键值4个
			IIC_Send_Ack(ACK_NO);
			IIC_Stop_Bit();
		}
		else 
		{//无按键响应时清按键数据
			key_data[0] = 0;
			key_data[1] = 0;
			key_data[2] = 0;
		}
	}
	 
	//将三组按键值插入数组key_data中，k0-k7放在key_data[0]，k8-k15放在key_data[1]，k16-k19放在key_data[2]
	*pKeyData = ((uint32)key_data[2] << 16) + ((uint32)key_data[1] << 8) + key_data[0];
}


 /*******************************************************************************
 * Function:  Ht16k23_Updata()
 *
 * Description: 更新16k23 ，放在主函数中更新液晶数据
 *
 * Returns:     none
 *
 * Notes:       实现20ms 更新一次数据，1s 初始化一次
 *		
*******************************************************************************/
void  Ht16k23_Updata(void)
{
	static uint16  timer_int16k23,timer_ref16k23;	
   
	if(TimeOutChkMsLong(&timer_int16k23,1000))//1s初始化一次
   {
		Ht16k23_Init(); //初始化Ht16k23
   }
   
   if (TimeOutChkMsLong(&timer_ref16k23, 20))
   {
   	Ht16k23APP_LCDSet();
   	Ht16k23_Drvie(); //LCD数据传输给ht16k23
   }  
   else{}
}

 /*******************************************************************************
 * Function:  Ht16k23_Clear()
 *
 * Description:  有需要时调用
 *
 * Returns:   none
 *
 * Notes:     清空16k23里面的数据,立即执行
 *
*******************************************************************************/
void  Ht16k23_Clear(void)
{
	uint8 i=0;
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(LCD_FIRST_ADDRESS);
	
	for (i = 0; i < u8HtSize; i++)
	{
		IIC_Send_Byte(0);
	}
	IIC_Stop_Bit();
}
