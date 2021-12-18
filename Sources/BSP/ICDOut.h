/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：ICDOut.h
* 功能描述： 此文件不用更改
*
* 编译器： codewarrior IDE 11.1
* MCU说明：和MCU 无关
*  
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年4月2日
* 
* 
*/
#ifndef OUT_H_
#define OUT_H_

#include "APPDrive.h"

void TrOut_5VEN(uint8 en);
void	OUT_AllClose(void);
void	OUT_AllOpen(void);
void OutTask(void);
#endif /* OUT_H_ */
