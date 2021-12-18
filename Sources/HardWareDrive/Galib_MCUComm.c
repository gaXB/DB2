/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Galib_MCUComm.c
* ����������1 ����.hΪLIB����ʹ�ã����ļ���Ϊʵ�֡� 
* 2 ����MCU ��ͬ��Ҫʵ�ֺ�����һ��Ҳ���ڴ˴�
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã�������KEAϵ��
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
* 
* �汾��2.0
* ˵����1 ���� NOP ΪMCU_NOP
* 2 ���ӿ����ж�
* 3 ��code flash
* 4 srand ����
* ���ߣ�xb
* ������ڣ�2021��8��23��
*/
#include "MCUDevice.h" 
#include "Galib_MCUComm.h"



/*******************************************************************************
 * Function:     void GaLib_MCUNOP(uint16 u16WaitTimes)
 *
 * Description:  NOP waite�� u16WaitTimes �ȴ�����
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
 * Description: �뿴�Ź�
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
 * Description:  MCU �����жϵĿ��أ��ָ�
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
 * Description:  ��ȡcodeflash �̶���ַ��ֵ
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
 * Description:  ��ADDR ��ַ�����ݸ��Ƶ�dest ������
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
 * Description:  ��ADDR ��ַ�����ݸ��Ƶ�dest ������
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
