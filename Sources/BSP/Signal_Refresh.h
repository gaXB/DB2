/************************文件说明*****************************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Signal_Refresh.h
* 功能描述：信号刷新模块头文件
*
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关；作为模板需要根据实际项目应用需求更改
*/
/**************************更改记录******************************************************
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月19日
* 
* 版本：1.1
* 说明：跟随.c
* 作者：xb
* 完成日期：2020年7月6日
* 
* 版本：1.2
* 说明: 将SIG_NUM_XXX 放在此处
* 作者：xb
* 完成日期：2020年10月22日
*/
#ifndef SIGNAL_REFRESH_H_
#define SIGNAL_REFRESH_H_
#include "APPDrive.h"


//TODO: 对SIGFLITER 的编号定义
#define  SIG_NUM_TIN       0
#define  SIG_NUM_TAMB      1 
#define  SIG_NUM_TEVP      2
#define  SIG_NUM_VENTF     3
#define  SIG_NUM_VENTD     4
#define  SIG_NUM_PT1TEMP   5
#define  SIG_NUM_PT2TEMP   6
#define  SIG_NUM_PT3TEMP   7
#define  SIG_NUM_PT1PRESS  8
#define  SIG_NUM_PT2PRESS  9
#define  SIG_NUM_PT3PRESS  10
#define  SIG_NUM_PTCTEMP   11
#define  SIG_NUM_RHEAT_FB  12
#define  SIG_NUM_SUN       13
#define   SIG_MAXNUM       14 //设置最大个数，应用程序初始化时使用


void Deal_Signal_Refresh(void);
void SigRefresh_IgnON(void);
void SigRefresh_IgnOFF(void);
void SignalCAN_RXComplete(uint32 u32Id);
void SignalCAN_RecTimeOut(uint32 u32Id);

#endif /* SIGNAL_REFRESH_H_ */
