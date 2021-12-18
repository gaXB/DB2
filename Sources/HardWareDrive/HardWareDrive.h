/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：HardWareDrive.h
* 功能描述：HardWareDrive 的头文件。需要定义：
* 1 MCU 型号
* 2 Clear_WatchDog
* 3 NOP
* 编译器： codewarrior IDE 10.7
* MCU说明：为接口模板，接口函数不变，内容和MCU相关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年3月14日
* 
* 版本：1.1
* 说明：MCU头文件为 "MCUDevice.h" 
* 作者：xb
* 完成日期：2021年8月23日
*/
#ifndef __HARDWARE_DRIVE__
#define __HARDWARE_DRIVE__

#include "Galib_MCUComm.h"
#include "MCUDevice.h" 
	

//AD模块
void sSW_Reset(void);



#endif
