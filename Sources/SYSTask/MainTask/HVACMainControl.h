/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：HVACMainControl.h
* 功能描述：HVACMainControl的投文件
* 编译器： codewarrior IDE 11.1
* MCU说明： 与MCU无关，基本不用更改
* */
/**************************更改记录******************************************************
 * 
* 版本：1.5
* 说明：跟随.c
* 原作者：xb
* 完成日期：2021年4月6日
*
*/
#ifndef HVACMAINCONTROL_H_
#define HVACMAINCONTROL_H_
#include "APPDrive.h"


#define  _APPCONFIG_FANDISPLAY_      1      //风量显示跟随输出 为1， 显示跟随设置为0



void Run_Park_Task(void);
void Run_Remote_Task(void);
void Run_Normal_Task(void);
void HVACControl_Init(void);	
#endif /* HVACMAINCONTROL_H_ */
