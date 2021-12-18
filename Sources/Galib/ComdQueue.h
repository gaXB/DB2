/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：ComdQueue.h
* 功能描述：队列 模块
* 
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原队列模块的1.0移植
* 原作者：xb
* 完成日期：2019年4月29日
* 
* 
*/
#ifndef    COMD_QUEUE_H__
#define    COMD_QUEUE_H__
#include "DataType.h"



#define  MAX_COMD_NUM     10       //队列的最大长度
/***************不可改分界*****************************/
#define  COMD_NONE         0     // 0 命令码被作为无效命令使用，其他命令不能定义命令



BOOL  InsertComd(uint8 Comd);
void  InitComdQueue(void);
uint8  ReadComd(void);
#endif 
