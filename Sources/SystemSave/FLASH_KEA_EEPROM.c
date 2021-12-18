/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�FLASH_KEA_EEPROM.c
* ����������KEA FLASH ģ��EEE �Ĺ���ʵ�֡�
* Ϊ��ģ��������������Ҫ���� ��Ҫ����ײ�flash ����
* 
* �������� codewarrior IDE 10.7-KEA
* MCU˵����ֻ������KEAZ128��64
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����,����ԭ��0.06�ù���
* ԭ���ߣ�xb
* ������ڣ�2019��9��5��
* 
*/

#include "FLASH_KEA_EEPROM.h"
#include "Galib_MCUComm.h"
#include "Memory.h"
#include "DealString.h"
typedef struct
{
	uint32 u32StartAddr;     			// ͷ��ַ
	uint16 u16Length;					/* �洢���� */
	uint16 u16ActualLength;             // ʵ�ʴ洢����
	uint16 u16PageRTimes;               // һ���ַ���ظ�д�Ĵ���,��ʼ������Ҫ����˱����������Զ�����
	uint16 u16Result; 
}FLASHSET, *pFLASHSET;

uint8 FlashTotalNum = 0;
static FLASHSET FlashSet[EEPROM_MAX_NUM];
#define MAX_FLASH_SIZE 4096

/*******************************************************************************
 * Function:        Flash_uint8_read�� Flash_uint32_read
 *
 * Description��           ��ȡ����
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
 * Description:     ��У����
 *
 * Returns:         У����
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
 * Description:     kea ģ��ee��ʼ������һ�����ݴ洢λ�ã�������һ�������
 *
 * Returns:         ���û���ҵ��洢�����򷵻�KEA_EE_ERROR_NODATA������һ��д������FLAG_CURENT_PAGE
 *                  ��ȷ����KEA_EE_ERROR_OK
 *                   ���� ����KEA_EE_ERROR_INIT�� 
 * Notes:          u32StartAddr --- ͷ��ַ
 *                 Length -- �洢����
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
    	{//���� 4�ı���
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
 * Description:     kea ģ��eeд����
 *
 * Returns:         KEA_EE_ERROR_OK д��ɹ��� KEA_EE_ERROR_WRITE д��ʧ��
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
 * Description:     kea ������ �����û���ݷ��� KEA_EE_ERROR_OK
 *
 * Returns:         ���û���ݷ��� KEA_EE_ERROR_NODATA, ����KEA_EE_ERROR_OK
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
 * Description:     kea ���ش˿�������
 *
 * Returns:         ����д��Ĳ������
 *
 * Notes:           none		
 *
********************************************************************************/
uint16 KEA_EE_GetErrCode(uint8 id)
{
	if (id >= EEPROM_MAX_NUM) return  KEA_EE_ERROR_NODATA; 

	return  FlashSet[id].u16Result;
}

//����Ϊbootload��ʹ��
/*******************************************************************************
 * Function:       void EraseFlash(uint8* Addr)
 *
 * Description:    �߳���ַ,��ַΪuint8����
 *
 * Returns:         none
 *
 * Notes:           �߳���ַ��ֻ��bootload ʹ�á���Ϊ��ַΪuint8* �������4�ı���
 *                  ���ܾͲ���ʵ�֣������ᱨ��		
 *
********************************************************************************/
void EraseFlash(uint8* Addr)
{

}

//=============================================================================
//�˺���Ϊbootload ������Ҫȷ��2������
// 1 check the flash is blank
// 2 �˿��ַ�Ƿ��ܱ����룬 ��kea�� ��Ҫȷ�ϣ�eeAddr �ĵ�ַ���ܱ�4������
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
 * Notes:           	eeAddr ��4�ֽڶ���
 *
********************************************************************************/
//��SIZE�ܴ�ʱʹ�ô˺���,
#define  DEAL_BLANK_SIZE       0X800   //��Ҫ��֤
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
 * Description:    BOOT LOAD д���ݣ�
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
	{//��ַ����
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
			{//˵���㷨���쳣�����������쳣
				return FALSE;
			}
		}
		else//��8�ֽڵ�ַ���ù�
		{}
	}
	else
	{//��ַ����������0,������û��д���ֵ�÷���
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
	{//��8�ֽڳ���
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



