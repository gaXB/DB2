/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：GALib.h
* 功能描述：全体库的头文件。
* 
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年7月25日
*
* 版本：1.3
* 具体更改见说明文件
* 作者：xb
* 完成日期：2020年1月6日
* 
* 版本：1.4
* AD 模块升级到1.1 
* ADInit 原型改变
* 作者：xb
* 完成日期：2020年3月28日
* 
* 版本：1.5
* keypad 1.3
* ht16kk23 1.1
* 作者：xb
* 完成日期：2020年6月17日
* 
* * 版本：1.6
* 1 加入电位器模块
* 
* 作者：xb
* 完成日期：2020年7月17日
* 
* * * 版本：1.7
* 1 内外电机1.1
* 2 电机1.1
* 设置0xff为停机
* 作者：xb
* 完成日期：2020年10月21日
* 
* 版本：1.8
* 1 电机1.2
* 2 鼓风机 1.4
* 电机抖动BUG
* 作者：xb
* 完成日期：2020年12月2日
* 
* 版本：1.9
* 1 sigfilter v1.2
* 2 motor  v1.3
* 3 fancontrol  v1.5
* 作者：xb
* 完成日期：2020年12月17日
*/


#ifndef GALIB_H_
#define GALIB_H_

#include "DataType.h"
#include "Encoder28.h"
#include	"FanControl.h"
#include	"Ht16k23_Drive.h"
#include	"IIC_Drive_Software.h"
#include "mcp7708.h"
#include	"SPI_Drive.h"
#include "Encoder35.h"
#include "EncoderESE24.h"
#include "ComdQueue.h"
#include "ADTreat.h"
#include	"keypad.h"
#include	"mathGA.h"
#include "motor.h" 
#include "ncfmotor.h"
#include "SignalFilter.h"
#include "timerout.h"
#include "DealString.h"
#include "FanControl.h"
#include "PTMETER_Fun.h"

#endif /* GALIB_H_ */
