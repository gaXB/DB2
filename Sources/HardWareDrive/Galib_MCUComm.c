/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Galib_MCUComm.c
* 功能描述：1 用于.h为LIB生成使用，此文件作为实现。 
* 2 部分MCU 不同需要实现函数不一样也放在此处
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，适用于KEA系列
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
* 
* 版本：2.0
* 说明：1 更改 NOP 为MCU_NOP
* 2 增加开关中断
* 3 读code flash
* 4 srand 函数
* 作者：xb
* 完成日期：2021年8月23日
*/
#include "MCUDevice.h" 
#include "Galib_MCUComm.h"



/*******************************************************************************
 * Function:     void GaLib_MCUNOP(uint16 u16WaitTimes)
 *
 * Description:  NOP waite； u16WaitTimes 等待次数
 *
 * Returns:    none
 *
 * Notes:   			
 *
*******************************************************************************/
void GaLib_MCUNOP(uint16 u16WaitTimes)
{
	while(u16WaitTimes--)
	{
		MCU_NOP;
	};
}
/*******************************************************************************
 * Function:    void GaLib_MCUClearWatchDog(void)
 *
 * Description: 请看门狗
 *
 * Returns:    none
 *
 * Notes:   			
 *
*******************************************************************************/
extern void WDOG_Feed(void);
void GaLib_MCUClearWatchDog(void)
{
	WDOG_Feed();
}

/*******************************************************************************
 * Function:     uint32 EnableInterrupt(void)
 *               uint32 DisableInterrupt(void)
 *               void RestoreInterrupt(uint32 intDis)
 * Description:  MCU 的总中断的开关，恢复
 *
 * Returns:    none
 *
 * Notes:   			
 *
*******************************************************************************/
//=================================================================================================
// Enable all interrupt & return original interrupt enable/disable setting
// Return: !0, if PRIMASK=1
//          0, if PRIMASK=0
//=================================================================================================
uint32 EnableInterrupt(void)
{
	register int ret_val __asm__("r0");
	__asm("MRS R0, PRIMASK");
	__asm("CPSIE i");
	
	return(ret_val);
}
//=================================================================================================
// Disable all interrupt & return original interrupt enable/disable setting
// Return: !0, if PRIMASK=1
//          0, if PRIMASK=0
//=================================================================================================
uint32 DisableInterrupt(void)
{
	register int ret_val __asm__("r0");
	__asm("MRS R0, PRIMASK");
	__asm("CPSID i");
	
	return(ret_val);
}

//=================================================================================================
// Restore interrupt
// In:   intDis!=0, disable interrupt;  intDis==0, enable interrupt
// Return:
//       No
//=================================================================================================
void RestoreInterrupt(uint32 intDis)
{
	if (intDis)
		__asm("CPSID i");	//disable interrupt
	else
		__asm("CPSIE i");	//enable interrupt
}


/*******************************************************************************
 * Function:    inline uint32 Flash_uint32_read(uint32 Addr)
 *              inline uint8 Flash_uint8_read(uint32 Addr)
 * 
 * Description:  读取codeflash 固定地址的值
 *
 * Returns:    none
 *
 * Notes:   			
 *
*******************************************************************************/
inline uint32 Flash_uint32_read(uint32 Addr)
{
	return *((uint32*)Addr);
}

inline uint8 Flash_uint8_read(uint32 Addr)
{
	return *((uint8*)Addr);
}

/*******************************************************************************
 * Function:    void MemAddrCopy(void* dest, uint32 Addr, uint16 u16Length)
 *              
 * 
 * Description:  将ADDR 地址内数据复制到dest 数组中
 *
 * Returns:    none
 *
 * Notes:   			
 *
*******************************************************************************/
void MemAddrCopy(void* dest, uint32 Addr, uint16 u16Length)
{
	uint16 i;
	uint8* pdest = dest;
   uint8* pSrc = (uint8*)Addr; 
   for(i=0; i<u16Length; i++)
   {
   	*(uint8*)pdest++ = *(uint8*)pSrc++;
   }
}

/*******************************************************************************
 * Function:   void Ga_Srand(void)
 *             void Ga_srand(uint16)
 * 
 * Description:  将ADDR 地址内数据复制到dest 数组中
 *
 * Returns:    none
 *
 * Notes:   			
 *
*******************************************************************************/
int rand(void);
void 	srand(uint16 sTime); 
uint16 Ga_rand(void)
{
	return rand();
}


void Ga_srand(uint16 sTime)
{
	srand(sTime);
}
