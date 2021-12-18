/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Ht16k23_Drive.h
* 功能描述：ht16k23 模块的头文件。
* 为此模块能正常工作需要加入 IIC 模块
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
* 版本：1.1
* 说明：跟随.c
* 原作者：xb
* 完成日期：2020年6月2日
*/

#ifndef  Ht16k23_Drive_
#define  Ht16k23_Drive_
#include "DataType.h"


#define   HT16K23_MAXLCDNUM      10       //seg0-19
/************************************************************************
 * ht16k23指令集
 *
*************************************************************************/
#define   SLAVE_WRITE   0xe6//写从机
#define   SLAVE_READ    0xe7//读从机
#define   NORMAL_DISPLAY_ON    0x83//正常工作，开显示驱动
#define   STANDBY_DISPLAY_OFF  0x80//待机，关显示驱动
#define   MODE_SEGMENGT        0xa0//软件中断采集按键
#define   LCD_FIRST_ADDRESS    0x00//LCD寄存器首地址
#define   INT_REG_ADDRESS      0x30//按键中断寄存器地址
#define   KEY_FRIST_ADDRESS    0x20//按键数据寄存器首地址
#define   KEY_SCAN_PERIOD      0xfc//设置按键扫描周期
              
//函数类型说明        
BOOL Ht16k23_LCDDataSet(uint8* u8LCDData, uint8 u8Length);                
void  Ht16k23_Updata(void);
void  Ht16k23_Read_Key(uint32* pKeyData);
void  Ht16k23_Clear(void);

#endif
