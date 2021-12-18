/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：DealString.c
* 功能描述：字符串处理模块，具有以下功能
* 1  数组复制
* 2  数组比较
* 
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年5月15日
* 
* * 版本：1.1
* 说明：增加GaMemSet
* 原作者：xb
* 完成日期：2019年12月15日
*/
#include "DealString.h"

/*******************************************************************************
 * Function:        void MemCopy(void* dest,const void* Src,uint16 u16Length)
 *
 * Description:     数组之间传递数据
 *
 * Returns:         none
 *
 * Notes:           none		
 *
********************************************************************************/
void MemCopy(void* dest,const void* Src,uint16 u16Length)
{
   uint16 i;
   for(i=0; i<u16Length; i++)
   {
   	*(uint8*)dest++ = *(uint8*)Src++;
   }
}
/*******************************************************************************
 * Function:         BOOL MemCompare(uint8* buf1,uint8* buf2,uint8 Length)
 *
 * Description:     判断buf1，buf2是否相等
 *
 * Returns:         1 相等 0不相等
 *
 * Notes:           none		
 *
********************************************************************************/
BOOL MemCompareEqual(uint8* buf1,uint8* buf2,uint8 Length)
{
   uint8 i;
   uint8 bEqual = TRUE;
   for(i=0; i < Length; i++)
   {
   	if(*buf1++ != *buf2++)
   	{
   		bEqual = FALSE;
   		break;
   	}
   	else{}
   }
   
   return bEqual;
}

/*******************************************************************************
 * Function:        void GaMemSet(void* S,uint8 u8SetData,uint16 u16Length)
 *
 * Description:     数组初始化为 u8SetData
 *
 * Returns:         none
 *
 * Notes:           none		
 *
********************************************************************************/
void GaMemSet(void* S,uint8 u8SetData,uint16 u16Length)
{
   uint16 i;
   uint8 *pData = (uint8*)S;

   for(i = 0; i < u16Length; i++)
   {
	   *pData++ = u8SetData;
   }
}
