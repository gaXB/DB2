/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：IIC_Drive_Software.h
* 功能描述: 	iic 驱动模块的头文件。需要加入 Galib_MCUComm.h 文件
* 
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年6月2日
* 
*/

#ifndef   IIC_DRIVE_SOFTWARE_
#define   IIC_DRIVE_SOFTWARE_

#include "Galib_MCUComm.h"
#include "DataType.h"





#define   IIC_DELAY  2  //单片机总线时钟大于8M时，需增大该数值

#define   ACK_YES  1  //发送响应信号
#define   ACK_NO   0  //发送不响应信号



//外部可调用函数
void IIC_Start_Bit(void);
void IIC_Stop_Bit(void);
void IIC_Send_Ack(uint8 data);
uint8 IIC_Send_Byte(uint8 data);
uint8 IIC_Receive_Byte(void);
#endif
