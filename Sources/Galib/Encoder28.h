/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Encoder28.h
* 功能描述：28编码器 模块的头文件。
* 为此模块能正常工作需要加入 comdqueue 模块
* 
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年5月24日
* 
*/

#ifndef  TREAT_CODE28_H
#define  TREAT_CODE28_H

#include "ComdQueue.h"


#define  MAX_CODE28NUM   3


//故障码
#define  CODE28_ERROR_NONE        0x00
#define  CODE28_ERROR_HARDWARE    0x01
#define  CODE28_ERROR_KEYCODE     0x80





BOOL Init_Code28(uint8 num, uint8 *CodeKeyData);
void Treat_Read_Code28(void); 
#endif
