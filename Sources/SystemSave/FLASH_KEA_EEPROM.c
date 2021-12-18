/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：FLASH_KEA_EEPROM.c
* 功能描述：KEA FLASH 模拟EEE 的功能实现。
* 为此模块能正常工作需要加入 需要加入底层flash 驱动
* 
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：只能用于KEAZ128和64
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立,由由原来0.06该过来
* 原作者：xb
* 完成日期：2019年9月5日
* 
*/

#include "FLASH_KEA_EEPROM.h"
#include "Galib_MCUComm.h"
#include "Memory.h"
#include "DealString.h"
typedef struct
{
	uint32 u32StartAddr;     			// 头地址
	uint16 u16Length;					/* 存储长度 */
	uint16 u16ActualLength;             // 实际存储长度
	uint16 u16PageRTimes;               // 一块地址可重复写的次数,初始化不需要定义此变量，程序自动计算
	uint16 u16Result; 
}FLASHSET, *pFLASHSET;

uint8 FlashTotalNum = 0;
static FLASHSET FlashSet[EEPROM_MAX_NUM];
#define MAX_FLASH_SIZE 4096

/*******************************************************************************
 * Function:        Flash_uint8_read， Flash_uint32_read
 *
 * Description：           读取数据
********************************************************************************/
static void FLASH_EraseSectorCheck(uint32 u32Addr)
{
	uint32 *pAddr = (uint32*)u32Addr;
	uint32 i;

	for (i = 0; i< EEPROM_PAGE_SIZE/4; i++)
	{
		if (*pAddr != 0xffffffffu) 
		{
		//	FLASH_EraseSector(u32Addr);
			return;
		}
		pAddr++;
	}
}

/*******************************************************************************
 * Function:        static uint8 KEA_EE_GetCheckSum(uin8 *src, uint16 u16Length)
 *
 * Description:     求校验码
 *
 * Returns:         校验码
 *
 * Notes:           none		
 *
********************************************************************************/
static uint8 KEA_EE_GetCheckSum(uint8 *src, uint16 u16Length)
{
	uint8 checksum = 0;
	uint16 u16Id;
	uint8 *pData = src;
	for (u16Id = 0; u16Id < u16Length; u16Id++)
	{
		checksum += *pData++;
	}
	
	checksum = ~checksum;
	checksum++;
	return checksum;
}

static uint32 KEA_EE_Get32CheckSum(uint8 *src, uint16 u16Length)
{
	uint32 checksum = 0;
	uint16 u16Id;
	uint8 *pData = src;
	for (u16Id = 0; u16Id < u16Length; u16Id++)
	{
		checksum += *pData++;
	}

	checksum = ~checksum;
	checksum++;
	return checksum;
}
/*******************************************************************************
 * Function:        uint8 KEA_EE_Init(uint8 id, uint32 u32StartAddr, uint16 Length)
 *
 * Description:     kea 模拟ee初始化，哪一块数据存储位置，并将下一块擦除。
 *
 * Returns:         如果没有找到存储数据则返回KEA_EE_ERROR_NODATA，将第一块写入数据FLAG_CURENT_PAGE
 *                  正确返回KEA_EE_ERROR_OK
 *                   参数 错误KEA_EE_ERROR_INIT， 
 * Notes:          u32StartAddr --- 头地址
 *                 Length -- 存储长度
********************************************************************************/
uint8 KEA_EE_Init(uint8 id, uint32 u32StartAddr, uint16 Length)
{
    return KEA_EE_ERROR_OK;	
}


uint8 KEA_EE_AutoInit(uint8 num, uint16* tbl_Length)
{
	uint8 i;
	FLASHSET* pFlashSet = FlashSet;
	uint16 Addr = 0;
    for (i = 0; i < num; i++)
    {
    	pFlashSet->u16Length = tbl_Length[i];
    	pFlashSet->u32StartAddr = Addr;
    	if (pFlashSet->u16Length & 0x3)
    	{//不是 4的倍数
    		pFlashSet->u16ActualLength = pFlashSet->u16Length & 0xFFFC;
    		pFlashSet->u16ActualLength = pFlashSet->u16ActualLength + 4;
    	}
    	else
    	{
    		pFlashSet->u16ActualLength = pFlashSet->u16Length;
    	}

    	pFlashSet->u16ActualLength += 4;
    	Addr += pFlashSet->u16ActualLength;
    	if (Addr >= MAX_FLASH_SIZE)
    	{
    		return KEA_EE_ERROR_INIT;
    	}else{}
    	pFlashSet++;
    }
    FlashTotalNum = num;
	return KEA_EE_ERROR_OK;
}
/*******************************************************************************
 * Function:        uint8 KEA_EE_WriteRecord(uint8 id, uint8 *src)
 *
 * Description:     kea 模拟ee写数据
 *
 * Returns:         KEA_EE_ERROR_OK 写入成功。 KEA_EE_ERROR_WRITE 写入失败
 *
 * Notes:           none		
 *
********************************************************************************/
uint8 KEA_EE_WriteRecord(uint8 id, uint8 *src)
{
	status_t ret;
	FLASHSET* pFlashSet;
	uint32 u32CheckSum;
	if (id < FlashTotalNum)
	{
		pFlashSet = &FlashSet[id];
		ret = DATAFLASH_Write(pFlashSet->u32StartAddr, src, pFlashSet->u16Length);

		u32CheckSum = KEA_EE_Get32CheckSum(src, pFlashSet->u16Length);
		ret = DATAFLASH_Write(pFlashSet->u32StartAddr + pFlashSet->u16ActualLength -4 , (uint8*)&u32CheckSum, 4);
	}
	else
	{
		return KEA_EE_ERROR_WRITE;
	}

	if (ret == STATUS_SUCCESS)
	{
		return KEA_EE_ERROR_OK;
	}
	else
	{
		return KEA_EE_ERROR_WRITE;
	}
}

/*******************************************************************************
 * Function:       uint8 KEA_EE_ReadRecord(uint8 id, uint8 *dest)
 *
 * Description:     kea 读数据 ，如果没数据返回 KEA_EE_ERROR_OK
 *
 * Returns:         如果没数据返回 KEA_EE_ERROR_NODATA, 正常KEA_EE_ERROR_OK
 *
 * Notes:           none		
 *
********************************************************************************/
uint8 KEA_EE_ReadRecord(uint8 id, uint8 *dest)
{
	status_t ret;
	FLASHSET* pFlashSet;
	uint32 u32CheckSum;
	uint8* pAddr;
	uint8 j;
	if (id < FlashTotalNum)
	{
		pFlashSet = &FlashSet[id];
		pAddr = (uint8*)(pFlashSet->u32StartAddr + DATAFLASH_BASE_ADDR);

		//u32CheckSum = KEA_EE_Get32CheckSum(pAddr, pFlashSet->u16Length);
		u32CheckSum = *((uint32*)(pFlashSet->u32StartAddr + pFlashSet->u16ActualLength -4 + DATAFLASH_BASE_ADDR));
		if (u32CheckSum == KEA_EE_Get32CheckSum(pAddr, pFlashSet->u16Length))
		{
			ret = STATUS_SUCCESS;
			pAddr = (uint8*)(pFlashSet->u32StartAddr + DATAFLASH_BASE_ADDR);
			for (j = 0; j < pFlashSet->u16Length; j++)
			{
				*dest++ = *pAddr++;
			}
		}
		else
		{
			ret = STATUS_ERROR;
		}
	}
	else
	{
		return KEA_EE_ERROR_CHECKSUM;
	}

	if (ret == STATUS_SUCCESS)
	{
		return KEA_EE_ERROR_OK;
	}
	else
	{
		return KEA_EE_ERROR_CHECKSUM;
	}
}

/*******************************************************************************
 * Function:       uint16 KEA_EE_GetErrCode(uint8 id)
 *
 * Description:     kea 返回此块操作结果
 *
 * Returns:         返回写入的操作结果
 *
 * Notes:           none		
 *
********************************************************************************/
uint16 KEA_EE_GetErrCode(uint8 id)
{
	if (id >= EEPROM_MAX_NUM) return  KEA_EE_ERROR_NODATA; 

	return  FlashSet[id].u16Result;
}

//以下为bootload中使用
/*******************************************************************************
 * Function:       void EraseFlash(uint8* Addr)
 *
 * Description:    檫除地址,地址为uint8类型
 *
 * Returns:         none
 *
 * Notes:           檫除地址，只在bootload 使用。因为地址为uint8* 如果不是4的倍数
 *                  功能就不会实现，但不会报错		
 *
********************************************************************************/
void EraseFlash(uint8* Addr)
{

}

//=============================================================================
//此函数为bootload 调用需要确认2个问题
// 1 check the flash is blank
// 2 此快地址是否能被烧入， 在kea中 需要确认，eeAddr 的地址是能被4整除的
//=============================================================================
uint8 Check_FlashBlank(uint32 eeAddr, int16 Size)
{
	uint32* p32Addr;
	uint8* pu8Data;
	while((uint32)eeAddr & 0x03)
	{
		pu8Data = (uint8*)eeAddr;
		if (*pu8Data != 0xff)
		{
			return FALSE;
		}
		else
		{
			eeAddr++;
		}
	}
	
	p32Addr = (uint32*)eeAddr;
	
	if ( (uint32)p32Addr & 0x03) return FALSE;
	
	while (Size > 0)
	{    
		if (*p32Addr != 0xffffffff)
		{
			return FALSE;
		}
		
		Size-= 4;
		p32Addr++;
	}
	
	return TRUE;
}

/*******************************************************************************
 * Function:       uint8 Check_FlashBlank(uint32 eeAddr, int16 Size)
 *
 * Description:     check the flash is blank
 *
 * Returns:         none
 *
 * Notes:           	eeAddr ，4字节对齐
 *
********************************************************************************/
//当SIZE很大时使用此函数,
#define  DEAL_BLANK_SIZE       0X800   //需要验证
uint8 Check_FlashBlank_Long(uint32 eeAddr, int16 Size)
{
	//__far uint8* pfAddr = (__far uint8*)eeAddr;
	while (Size > DEAL_BLANK_SIZE)
	{
		if (Check_FlashBlank(eeAddr, DEAL_BLANK_SIZE) == FALSE)
		{
			return FALSE;
		}
		else
		{
			Clear_WatchDog();
			Size -= DEAL_BLANK_SIZE;
			eeAddr += DEAL_BLANK_SIZE;
		}
	}

	if (Size)
	{
		if (Check_FlashBlank(eeAddr, Size) == FALSE)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		return TRUE;
	}
}
/*******************************************************************************
 * Function:      BOOL WriteFlash(uint32 u32Addr, uint8* datBuff, uint8 byteCount)
 *
 * Description:    BOOT LOAD 写数据，
 *
 * Returns:         none
 *
 * Notes:           none		
 *
********************************************************************************/
BOOL WriteFlash(uint32 u32Addr, uint8* datBuff, uint8 byteCount)
{
	static uint8 u8LastData[8];
	static uint8 u8LastNum;
	static uint32 u32LastAddr;
	static uint8 u8LastCount;
	uint8 u8BeginId = 0;
	uint8 i, j;

	if (byteCount == 0)
	{
		for (i = u8LastNum, j= 0; i <8; i++,j++)
		{
			u8LastData[i] = 0xff;
		}
		Download_Data_to_Flash((u32Addr - u8LastNum), u8LastData, 8);
		return;
	}
	else if ((u32LastAddr + u8LastCount) == u32Addr)
	{//地址连续
		u32LastAddr = u32Addr;
		u8LastCount = byteCount;
		if (u32Addr & 0x07)
		{
			if ( ((u32Addr - u8LastNum)&0x07) == 0)
			{
				for (i = u8LastNum, j= 0; i <8; i++,j++)
				{
					u8LastData[i] = datBuff[j];
				}
				Download_Data_to_Flash((u32Addr - u8LastNum), u8LastData, 8);
				byteCount -= j;
				u32Addr -= u8LastNum;
				u32Addr += 8;
				u8BeginId = j;
			}
			else
			{//说明算法有异常，或者数据异常
				return FALSE;
			}
		}
		else//是8字节地址不用管
		{}
	}
	else
	{//地址不连续则清0,可能有没有写入得值得风险
		u8LastNum = 0;
		u32LastAddr = u32Addr;
		u8LastCount = byteCount;
	}

	if ((byteCount &0x07) !=0)
	{
		u8LastNum = byteCount &0x07;
		byteCount -= u8LastNum;
		for (i = 0; i < u8LastNum; i++)
		{
			u8LastData[i] = datBuff[u8BeginId+byteCount+i];
		}
	}
	else
	{//是8字节长度
		u8LastNum = 0;
	}

	Download_Data_to_Flash(u32Addr, &datBuff[u8BeginId], byteCount);
	return 1;
}

uint16 FLASH_Program1LongWord(uint32 u32NVMTargetAddress, uint32 u32DwData)
{
	uint8 u8SaveData[8];
	uint32 u32WAddr;
	if ((u32NVMTargetAddress & 0x07) == 0)
	{
		MemCopy(u8SaveData, (uint8*)&u32DwData, 4);
		u8SaveData[4] = 0xff;
		u8SaveData[5] = 0xff;
		u8SaveData[6] = 0xff;
		u8SaveData[7] = 0xff;
		u32WAddr = u32NVMTargetAddress;
	}
	else
	{
		u32WAddr = u32NVMTargetAddress - 4;
		u8SaveData[0] = 0xff;
		u8SaveData[1] = 0xff;
		u8SaveData[2] = 0xff;
		u8SaveData[3] = 0xff;
		MemCopy(&u8SaveData[4], (uint8*)&u32DwData, 4);
	}
	(void)Download_Data_to_Flash(u32WAddr, u8SaveData, 8);
	return 1;
}



