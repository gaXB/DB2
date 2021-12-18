/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�DealString.c
* �����������ַ�������ģ�飬�������¹���
* 1  ���鸴��
* 2  ����Ƚ�
* 
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��5��15��
* 
* * �汾��1.1
* ˵��������GaMemSet
* ԭ���ߣ�xb
* ������ڣ�2019��12��15��
*/
#include "DealString.h"

/*******************************************************************************
 * Function:        void MemCopy(void* dest,const void* Src,uint16 u16Length)
 *
 * Description:     ����֮�䴫������
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
 * Description:     �ж�buf1��buf2�Ƿ����
 *
 * Returns:         1 ��� 0�����
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
 * Description:     �����ʼ��Ϊ u8SetData
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
