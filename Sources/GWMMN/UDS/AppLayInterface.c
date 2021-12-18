/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：AppLayInterface.c
* 功能描述：UDS诊断的应用层服务程序的接口文件，此文件是将APPLay中需要更改项目提出来。请按照标准更改。
* 1  安全访问策略
* 2  DTC list
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 

2019.10.25
ver0.1
1 初始版本
2 模板程序
*/
#include "DataType.h"
#include "AppLayInterface.h"


uint32  SECURITY_MASK_LEVEL1  = 0x51c5635c;
uint32  SECURITY_MASK_LEVEL2  = 0x51f5635f;
/*******************************************************************************
 * Function:     static uint32 seedToKey (uint32 nSeed, uint8 SecurityLevel)
 *
 * Description:   通过SEED 和 安全等级得到当前的key
 *
 * Returns:         key
 *
 * Notes:         nSeed 种子 
 *               SecurityLevel 1 ：等级  其他等级2
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
 * Description:   判定当前等级，种子和key 是不是可以对应
 *
 * Returns:         正确 ：TRUE  错误： FALSE
 *
 * Notes:         nSeed 种子 nKey:密钥，  u8Level等级
 *               被APPLAYLAY.c 调用
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




