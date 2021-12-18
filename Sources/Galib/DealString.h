/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：DealString.h
* 功能描述：string 模块的头文件。
* 
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年5月15日
*
* 版本：1.1
* 说明：增加GaMemSet
* 原作者：xb
* 完成日期：2019年12月15日
*/

#ifndef DEALSTRING_H_
#define DEALSTRING_H_

#include"DataType.h"

//外部可调用函数
void MemCopy(void* dest,const void* Src,uint16 u16Length);
BOOL MemCompareEqual(uint8* buf1,uint8* buf2,uint8 Length);
void GaMemSet(void* S,uint8 u8SetData,uint16 u16Length);
#endif /* DEALSTRING_H_ */
