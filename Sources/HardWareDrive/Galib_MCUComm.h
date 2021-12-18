/************************�ļ�˵��*****************************************************
* Copyright (c) 2021 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Galib_MCUComm.h
* ����������1 ����.hΪLIB����ʹ�ã����ļ���Ϊ������ 
* 2 ����MCU ��ͬ��Ҫʵ�ֺ�����һ��Ҳ���ڴ˴�
* �������� codewarrior IDE 11.1
* MCU˵�������ļ�ΪLIB��MCU ��Ľӿ��ļ�Ӧ�ã�ʹ����KEAϵ��
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
* 3 srand
* ���ߣ�xb
* ������ڣ�2021��8��23��
*/
#ifndef GALIB_MCUCOMM_H_
#define GALIB_MCUCOMM_H_

#include "DataType.h"


//#define   NOP       asm("NOP")        //�Ժ�ɾ��
#define   MCU_NOP   asm("NOP")
#define  Clear_WatchDog()  GaLib_MCUClearWatchDog()


void MemAddrCopy(void* dest, uint32 Addr, uint16 u16Length);
uint8 Flash_uint8_read(uint32 Addr);
uint32 Flash_uint32_read(uint32 Addr);
void GaLib_MCUNOP(uint16 u16WaitTimes);
void GaLib_MCUClearWatchDog(void);
uint32 DisableInterrupt(void);
uint32 EnableInterrupt(void);
void RestoreInterrupt(uint32 intDis);
uint16 Ga_rand(void);
void Ga_srand(uint16 sTime);
#endif /* GALIB_MCUCOMM_H_ */
