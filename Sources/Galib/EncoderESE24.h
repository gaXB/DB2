/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：EncoderESE24.h
* 功能描述：ESE24编码器 模块的头文件。
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
* 完成日期：2019年7月3日
* 
*/
#ifndef  TREAT_CODEESE24_H
#define  TREAT_CODEESE24_H

#include "ComdQueue.h"


#define  MAX_CODEESE24NUM   3


//故障码
#define  CODEESE24_ERROR_NONE        0x00
#define  CODEESE24_ERROR_HARDWARE    0x01
#define  CODEESE24_ERROR_KEYCODE     0x80





BOOL Init_CodeESE24(uint8 num, uint8 *CodeKeyData);
void Treat_Read_CodeESE24(void); 
#endif
