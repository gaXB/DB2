/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：IIC_Drive_SoftWare.c
* 功能描述：IIC 软件模拟驱动，在整个系统设计时只能有1路IIC驱动
* 具有IIC的功能如下：
* 1 IIC 的开始，停止,ACK
* 2 IIC 数据发送和接收
* 3 属于协议层和底层MCU接口无关
* 
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原IIC模块的1.2移植
* 原作者：xb
* 完成日期：2019年6月2日
* 
* 
*/
#include"IIC_Drive_Software.h"


//mcu 底层接口函数 
extern void IICHW_SDAWrite(uint8 bSetState);
extern BOOL IICHW_SDARead(void);
extern void IICHW_SCLWrite(uint8 bSetState);
/*************************************************************************
 *Function:     IIC_Bit_Delay()
 *
 *Description:  8M总线时钟时，大概5us
 *
 *Return:       none
**************************************************************************/
static void IIC_Bit_Delay(void)
{  
	uint8 i;
	for (i=0; i < IIC_DELAY; i++)
	{
		MCU_NOP;
	}
}

/*************************************************************************
 *Function:     IIC_Start_Bit()
 *
 *Description:  I2C起始位发送
 *
 *Return:       no
 *************************************************************************/
void IIC_Start_Bit(void)
{
	IICHW_SDAWrite(1);
	IICHW_SCLWrite(1);
	IIC_Bit_Delay();
	
	IICHW_SDAWrite(0);
	IIC_Bit_Delay();
	IICHW_SCLWrite(0);
}

/*************************************************************************
 *Function:     IIC_Stop_Bit()
 *
 *Description:  I2C停止位发送
 *
 *Return:       no
 *************************************************************************/
void IIC_Stop_Bit(void)
{
	IICHW_SDAWrite(0);
	IICHW_SCLWrite(1);
	IIC_Bit_Delay();
	
	IICHW_SDAWrite(1);
	IIC_Bit_Delay();
	IICHW_SDAWrite(0);
}

/*************************************************************************
 *Function:     IIC_Send_Ack()
 *
 *Description:  I2C应答信号，data=1发送应答信号，data=0发送非应答信号
 *              
 *Return:       no
 *************************************************************************/
void IIC_Send_Ack(uint8 data) 
{
	if (data == 1) //发送响应信号
	{
		IICHW_SDAWrite(0);
	}
	else //发送不响应信号
	{
		IICHW_SDAWrite(1);
	}
	IIC_Bit_Delay();
	
	IICHW_SCLWrite(1);
	IIC_Bit_Delay();
	
	IICHW_SCLWrite(0);
	IIC_Bit_Delay();
}

/*************************************************************************
 *Function:     IIC_Send_Byte()
 *
 *Description:  I2C发送数据
 *
 *Return:       1发送成功，0发送失败
 *************************************************************************/
uint8 IIC_Send_Byte(uint8 data)
{
	uint8 i=0;
	 
   for (i=0; i < 8; i++) //打入数据
   {
		if ( (data<<i) & 0x80) //设置数据值
		{
			IICHW_SDAWrite(1);
		}
		else 
		{
			IICHW_SDAWrite(0);
		}
		//时钟高电平时将数据打入芯片
		IIC_Bit_Delay();
		IICHW_SCLWrite(1);
		IIC_Bit_Delay();
		IICHW_SCLWrite(0);
   }
   
   //打入结束，将时钟拉高
   IIC_Bit_Delay();
   IICHW_SDAWrite(1); //接收响应位
   IIC_Bit_Delay();
   IICHW_SCLWrite(1);
   IIC_Bit_Delay();
   
   if (IICHW_SDARead() == 0) 
   {
      IICHW_SCLWrite(0);
      IIC_Bit_Delay();
      return 1; 
   }
   else 
   {
      IICHW_SCLWrite(0);
      IIC_Bit_Delay();
      return  0;   
   }
}

/*************************************************************************
 *Function:     IIC_Receive_Byte()
 *
 *Description:  I2C接收数据
 *
 *Return:       返回接收的数据
 *************************************************************************/
uint8 IIC_Receive_Byte(void) 
{
	uint8 data=0,m=0;
  
	IICHW_SDAWrite(1);
  
	for (m = 0; m < 8; m++) //接收数据
	{
		IICHW_SCLWrite(0);
		IIC_Bit_Delay();
    
		IICHW_SCLWrite(1);
		IIC_Bit_Delay();
    
		data <<= 1;
		
		if (IICHW_SDARead() == 1) 
		{
			data |= 0b00000001;
		}
		else{}
  }
  
  //接收数据结束 将时钟拉低
  IICHW_SCLWrite(0);
  IIC_Bit_Delay();
  
  return (data); //返回数据
}
