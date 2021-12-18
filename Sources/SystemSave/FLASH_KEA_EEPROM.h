/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�FLASH_KEA_EEPROM.h
* ����������FLASH ģ��keaz ģ���ͷ�ļ���
* Ϊ��ģ��������������Ҫ���� ��Ҫ��ʱ��ģ�鼰��ѧ��������ģ��
* 
* �������� codewarrior IDE 11.1-KEA
* MCU˵����ֻ������KEAZ ϵ��
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��9��5��
* 
*/
#ifndef FLASH_KEA_EEPROM_H_
#define FLASH_KEA_EEPROM_H_

#include "DataType.h"



#define  EEPROM_MAX_NUM     10


#define KEA_FLASH_PAGE_SIZE 0x200

/**
** FLASH block protection. Default is off.
** It is stongly to turn it on to protect the code space of CPU.
**/
#define KEA_FLASH_PROTECTION 0xFF

/**
** Possible return values of operation result.
**/
#define KEA_EE_ERROR_OK 0x00
#define KEA_EE_ERROR_UNKOWN 0xFF
#define KEA_EE_ERROR_OUT_OF_RANGE 0x01
#define KEA_EE_ERROR_NODATA   0x02
#define KEA_EE_ERROR_INIT     0x03
#define KEA_EE_ERROR_WRITE    0x04   //д��ʧ��
#define KEA_EE_ERROR_UNBLANK   0x05
#define KEA_EE_ERROR_CHECKSUM  0x06
/**
** Below macros are defined for API use only.
** Do NOT modify it.
**/
#define FLAG_CURENT_PAGE 0xAA55AA55u
#define FLAG_RECORD_USED 0x55AA5500u    //���һλΪУ����

#define EEPROM_PAGE_SIZE KEA_FLASH_PAGE_SIZE


/**
** Functions declarations of API.
**/
uint8 KEA_EE_Init(uint8 id, uint32 u32StartAddr, uint16 Length);
uint8 KEA_EE_WriteRecord(uint8 id, uint8 *src);
uint8 KEA_EE_ReadRecord(uint8 id, uint8 *dest);
uint8 Check_FlashBlank(uint32 eeAddr, int16 Size);
BOOL KEA_FLASH_IMLoad(uint32 eeAddr, uint8* datBuff, uint16 u16Length);
BOOL KEA_FLASH_IMSave(uint32 eeAddr, uint8* datBuff, uint16 u16Length);
uint8 KEA_EE_AutoInit(uint8 num, uint16* tbl_Length);
uint16 FLASH_Program1LongWord(uint32 u32NVMTargetAddress, uint32 u32DwData);
#endif /* FLASH_KEA_EEPROM_H_ */
