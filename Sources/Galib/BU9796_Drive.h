/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：BU9796_Drive.h
* 功能描述：BU9796 模块的头文件。
* 为此模块能正常工作需要加入 IIC 模块
* 
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xwl
* 完成日期：2021年10月21日
*/

#ifndef  BU9796_DRIVE_
#define  BU9796_DRIVE_
#include "DataType.h"


#define   BU9796_MAXLCDNUM      10       //seg0-19
/************************************************************************
 * ht16k23指令集
 *
*************************************************************************/
#define   SLAVE_ADDRESS    0x7c//从机地址
#define   ICSET_RESET       0xea//Execute Software Reset
#define   ICSET     				0xe8//Execute internal OSC mode
#define   BLINK_CONTROL      0xf0//Set BLKCTL
#define   DISPLAY_CONTROL    0xa7//Set Display Control
#define   AP_CONTROL    0xfc//Set APCTL
#define   LCD_FIRST_ADDRESS    0x00//LCD寄存器首地址

#define   MODE_SET_ON    0xc8//正常工作，开显示驱动
#define   MODE_SET_OFF    0xc0//关显示驱动

              
//函数类型说明        
BOOL BU9796_LCDDataSet(uint8* u8LCDData, uint8 u8Length);                
void  BU9796_Updata(void);
void  BU9796_StandbyMode(void);
void  BU9796_Clear(void);
#endif
