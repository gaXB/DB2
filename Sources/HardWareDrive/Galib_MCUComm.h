/************************文件说明*****************************************************
* Copyright (c) 2021 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Galib_MCUComm.h
* 功能描述：1 用于.h为LIB生成使用，此文件作为声明。 
* 2 部分MCU 不同需要实现函数不一样也放在此处
* 编译器： codewarrior IDE 11.1
* MCU说明：此文件为LIB和MCU 层的接口文件应用，使用于KEA系列
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
* 
* 版本：2.0
* 说明：1 更改 NOP 为MCU_NOP
* 2 增加开关中断
* 3 读code flash
* 3 srand
* 作者：xb
* 完成日期：2021年8月23日
*/
#ifndef GALIB_MCUCOMM_H_
#define GALIB_MCUCOMM_H_

#include "DataType.h"


//#define   NOP       asm("NOP")        //以后删除
#define   MCU_NOP   asm("NOP")
#define  Clear_WatchDog()  GaLib_MCUClearWatchDog()


void MemAddrCopy(void* dest, uint32 Addr, uint16 u16Length);
uint8 Flash_uint8_read(uint32 Addr);
uint32 Flash_uint32_read(uint32 Addr);
void GaLib_MCUNOP(uint16 u16WaitTimes);
void GaLib_MCUClearWatchDog(void);
uint32 DisableInterrupt(void);
uint32 EnableInterrupt(void);
void RestoreInterrupt(uint32 intDis);
uint16 Ga_rand(void);
void Ga_srand(uint16 sTime);
#endif /* GALIB_MCUCOMM_H_ */
