/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�AppLayInterface.c
* ����������UDS��ϵ�Ӧ�ò�������Ľӿ��ļ������ļ��ǽ�APPLay����Ҫ������Ŀ��������밴�ձ�׼���ġ�
* 1  ��ȫ���ʲ���
* 2  DTC list
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 

2019.10.25
ver0.1
1 ��ʼ�汾
2 ģ�����
*/
#include "DataType.h"
#include "AppLayInterface.h"


uint32  SECURITY_MASK_LEVEL1  = 0x51c5635c;
uint32  SECURITY_MASK_LEVEL2  = 0x51f5635f;
/*******************************************************************************
 * Function:     static uint32 seedToKey (uint32 nSeed, uint8 SecurityLevel)
 *
 * Description:   ͨ��SEED �� ��ȫ�ȼ��õ���ǰ��key
 *
 * Returns:         key
 *
 * Notes:         nSeed ���� 
 *               SecurityLevel 1 ���ȼ�  �����ȼ�2
********************************************************************************/
static uint32 seedToKey (uint32 nSeed, uint8 SecurityLevel)
{
	uint32 key = 0;
	uint32  MASK;  
	uint8 i; 


	if (SecurityLevel == 1)
	{
	   MASK =   SECURITY_MASK_LEVEL1;
	}
	else      MASK =   SECURITY_MASK_LEVEL2;
	if (nSeed !=0 )
	{
		for ( i=0; i<35; i++ )
		{
			if (nSeed & 0x80000000)
			{
				nSeed = nSeed <<1;
				nSeed = nSeed^MASK;
			}
			else
			{
				nSeed = nSeed << 1;
			}
		}
		key = nSeed;
	}
	return key;
}

/*******************************************************************************
 * Function:   BOOL CheckKeyBySeed(uint32 nKey, uint32 nSeed, uint8 u8Level)
 *
 * Description:   �ж���ǰ�ȼ������Ӻ�key �ǲ��ǿ��Զ�Ӧ
 *
 * Returns:         ��ȷ ��TRUE  ���� FALSE
 *
 * Notes:         nSeed ���� nKey:��Կ��  u8Level�ȼ�
 *               ��APPLAYLAY.c ����
********************************************************************************/
BOOL CheckKeyBySeed(uint32 nKey, uint32 nSeed, uint8 u8Level)
{
	if (seedToKey(nSeed, u8Level) == nKey)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}




