/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：MIXVENT_PID.h
* 功能描述：混合电机PID
* 出风口温度PID 控制,不用更改
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年11月21日
* 
*/
#ifndef PTCPOWER_PID_H_
#define PTCPOWER_PID_H_
#include "RunPID.h"

extern PIDPARAMETER sPTCPowerPar;
extern PIDPARAMETER sBatPTCPowerPar;
extern PIDDATA   sPTCPowerPidData;
int16 Deal_PTCPowerPID(void);
int16 PTCPower_Init(void);
void  PTCPowerPIDMode(uint8 bBathMode);
#endif /* MIXVENT_PID_H_ */
