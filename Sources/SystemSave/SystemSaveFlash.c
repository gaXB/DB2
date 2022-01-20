/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SystemSaveMoudle.c
* 功能描述：记忆存储模块和存到flash的相关记忆进行封装, 此模块不封装但不能更改
* 
* 1  本模块具有实现系统的存储接口功能，具体flah的存储实现在FLASH_KEA_EEPROM中实现
* 定义flash 存储的分区，及初始化。 跟MCU 相关。
* 2  掉电记忆， 系统状态值和传感器值的存储，由主控函数 MemoryControl来实现，周期记忆一
* 3  progarm data 记忆  独立放在一个区域
* 4  UDS 记忆
* 1) dtc数据 独立放在一块区域，用于IGN 低触发
* 2） uds did 数据，安全访问次数，配置字放入一个区域
* 3） F110 独立放一个区域
* 4）标定参数 独立放在一个区域
* 
* 编译器： codewarrior IDE 11.1
* MCU说明： 为上层应用和MCU数据存储之间的接口。
* 只适用于KEAZ128不支持KEAZ64
* 此模块不封装但不能更改
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年9月5日
* 
* 版本：2.0
* 说明：读取config bug更新，1。0 直接更新
*       2 增加DID 大小不能超过200，否则死循环
*        3  增加security parameter
* 作者：xb
* 完成日期：2019年12月30日
* 
* * 版本：3.0
* 说明：1 load_allcycle bug
* 作者：xb
* 完成日期：2020年1月14日
*
** * 版本：3.1
* 说明：1 增加 MCUdata的存储
* 作者：xb
* 完成日期：2020年5月13日
* 
* 版本：4.0
* 说明：1 自动除霜变为吹面的功能取消
* 作者：xb
* 完成日期：2020年12月9日
*/

#include "APPDrive.h"
#include "SystemSaveFlash.h"
#include "FLASH_KEA_EEPROM.h"
#include "Memory.h"
static SYSTEMCONTROL SysConData;


//外部使用的函数
extern void UDS_SetSecuriTimes(uint8 u8Times);
extern uint8 UDS_GetSecuriTimes(void);
extern BOOL  SetParameterData(uint8* rtx);

/**************************************************************
 * 定义没一块的ID，和存储长度0
 * 
**************************************************************/

//2片存储方案编号定义
#define   FLASH_ID_IGN           3       //点火存储
#define   FLASH_ID_DTC           4       //其他,dTC,
#define   FLASH_ID_MCUDATA       5       //mcudata

#define   FLASH_ID_PROGRAMDATA   0
#define   FLASH_ID_F110          1
#define   FLASH_ID_UDS           2
#define   FLASH_ID_PARAMETER     6
#define   FLASH_ID_PARAMETER_SEC     7
#define   FLASH_ID_MAXNUM        8
// 2片存储方案，长度定义
#define  SAVE_SIZE     sizeof(SYSTEMCONTROL)
#define  FLASH_EE_LENGTH0  SAVE_SIZE
#define  FLASH_EE_LENGTH1  (DTC_MAXNUM * sizeof(DTCSNAPDATA) + DTC_MAXNUM)
#define  FLASH_EE_LENGTH2  0x1c
//1片存储方案，长度定义
#define  DID_SAVE_MAXLENGTH            200      //使用的最大的长度
#define  UDS_EE_LENGTH                 (DID_SAVE_MAXLENGTH + 4)    //UDS
#define  FLASH_EE_LENGTH_F110          241
#define  FLASH_EE_LENGTH_PROGRAM       9
#define  FLASH_EE_LENGTH_PARAMETER     CAILBRATION_PARAMETER_NUM


//整体地址定义
#define  FLASH_EE_ADDR0             0x1F000u
#define  FLASH_EE_ADDR1             0x1F400u  
#define  FLASH_EE_ADDR2             0x1E000u  
//以下为一片
#define  FLASH_EE_ADDR_PROGRAMDATA  0x1F800u
#define  FLASH_EE_ADDR_F110         0x1FC00u
#define  FLASH_EE_ADDR_UDS          0x1FA00u
#define  FLASH_EE_ADDR_PARAMETER    0x1FE00u  
#define  FLASH_EE_ADDR_PARAMETER_SEC    0x1EE00u  


/*******************************************************************************
 * Function:       void FLASH_EE_Init(void)
 *
 * Description:       flash初始化，
 *
 * Returns:         none
 *
 * Notes:     
 *
********************************************************************************/
void FLASH_EE_Init(void)
{
	uint16 TBL_Length[FLASH_ID_MAXNUM];
	(void)DisableInterrupt();
	Memory_Init();

	TBL_Length[FLASH_ID_IGN] = FLASH_EE_LENGTH0;
	TBL_Length[FLASH_ID_DTC] = FLASH_EE_LENGTH1;
	TBL_Length[FLASH_ID_MCUDATA] = FLASH_EE_LENGTH2;
	TBL_Length[FLASH_ID_PROGRAMDATA] = FLASH_EE_LENGTH_PROGRAM;
	TBL_Length[FLASH_ID_F110] = FLASH_EE_LENGTH_F110;
	TBL_Length[FLASH_ID_UDS] = UDS_EE_LENGTH;
	TBL_Length[FLASH_ID_PARAMETER] = FLASH_EE_LENGTH_PARAMETER;
	TBL_Length[FLASH_ID_PARAMETER_SEC] = FLASH_EE_LENGTH_PARAMETER;

	(void)KEA_EE_AutoInit(FLASH_ID_MAXNUM, TBL_Length);
}

/*******************************************************************************
 * Function:       void Save_Ign (void)    void Load_Ign (void)
 *
 * Description:     点火的相关记忆
 *
 * Returns:         none
 *
 * Notes:          掉点SystemControl内容记忆，放在第一块flah
 *                 当读取错误时默认25，全AUTO. 即刷完程序后的第一次上电
 *                 void	MemoryControl(void) 主控函数，loop调用
********************************************************************************/
static void Save_Ign(void) 
{
	uint8 u8Data[SAVE_SIZE+2];
   
	MemCopy(&u8Data[0], (uint8*)(&SystemControl), sizeof(SYSTEMCONTROL) );
	
   (void)KEA_EE_WriteRecord(FLASH_ID_IGN, u8Data);
}

BOOL Load_Ign(void) 
{
	uint8 u8Data[SAVE_SIZE+2];
	uint8 bLoadRes;
	if (KEA_EE_ReadRecord(FLASH_ID_IGN, u8Data) == KEA_EE_ERROR_OK)
	{//读取正常
		MemCopy((uint8*)(&SystemControl), &u8Data[0], sizeof(SYSTEMCONTROL) );
		bLoadRes = TRUE;
	}
	else
	{ //读取错误
		bLoadRes = FALSE;
	}
	SysConData = SystemControl;

	return bLoadRes;
}


void	MemoryControl(void)
{
	static uint16 TimerSaveSys;
	static uint16 u16Times;

	if (TimeOutChkTenthSLong(&TimerSaveSys, SAVE_IGNDEALY))
	{
		u16Times++;
		if (MemCompareEqual((uint8*)&SysConData, (uint8*)&SystemControl, sizeof(SYSTEMCONTROL)) == FALSE)
		{
			Save_Ign();
			SysConData = SystemControl;
			u16Times = 0;
		}else{}
	}
}
/*******************************************************************************
 * Function:       void Save_DTC (void)    void Load_DTC (void)
 *
 * Description:     DTC 相关记忆
 *
 * Returns:         none
 *
 * Notes:         
********************************************************************************/
void Save_DTC(void)
{
	uint8 u8Data[FLASH_EE_LENGTH1];
   
	MemCopy(&u8Data[0], DTC, DTC_MAXNUM);
	MemCopy(&u8Data[DTC_MAXNUM], &DTCSnap[0], DTC_MAXNUM * sizeof(DTCSNAPDATA) );
	
	(void)KEA_EE_WriteRecord(FLASH_ID_DTC, u8Data);
}

uint8 Load_DTC(void)
{
   uint8 u8Data[FLASH_EE_LENGTH1];
   uint8 bLoadErr;
   if ((KEA_EE_ReadRecord(FLASH_ID_DTC, u8Data) == KEA_EE_ERROR_OK))
   {
		MemCopy(DTC, &u8Data[0], DTC_MAXNUM);
		MemCopy(&DTCSnap[0], &u8Data[DTC_MAXNUM], DTC_MAXNUM * sizeof(DTCSNAPDATA) );
		bLoadErr = TRUE;
   }
   else
   {
	   MemCompareEqual(DTC, 0, DTC_MAXNUM);
	   MemCompareEqual((uint8*)DTCSnap, 0, DTC_MAXNUM * sizeof(DTCSNAPDATA));
	   bLoadErr = FALSE;
   }
   return bLoadErr;
}

/*******************************************************************************
 * Function:        BOOL Load_Parameter(void)
 *
 * Description:     载入参数
 *
 * Returns:         none
 *
 * Notes:         	
 *
********************************************************************************/
BOOL Load_Parameter(void)
{
	uint8 rtx[243];
	uint8 bLoadErr;

	if ((KEA_EE_ReadRecord(FLASH_ID_PARAMETER, rtx) == KEA_EE_ERROR_OK))
	{
		(void)SetParameterData(rtx);
		bLoadErr = TRUE;
	}
	else
	{
		bLoadErr = FALSE;
	}

	if ((KEA_EE_ReadRecord(FLASH_ID_PARAMETER_SEC, rtx) == KEA_EE_ERROR_OK))
	{
		(void)SetParameterData_Sec(rtx);
		bLoadErr = TRUE;
	}
	else
	{
		bLoadErr = FALSE;
	}

	return bLoadErr;
}

/*******************************************************************************
 * Function:        void  Save_Parameter(uint8 *rtx)
 *
 * Description:     记忆参数
 *
 * Returns:         none
 *
 * Notes:         	
 *
********************************************************************************/
void  Save_Parameter(uint8 *rtx)
{
	(void)KEA_EE_WriteRecord(FLASH_ID_PARAMETER, rtx);
}

void  Save_Parameter_Sec(uint8 *rtx)
{
	(void)KEA_EE_WriteRecord(FLASH_ID_PARAMETER_SEC, rtx);
}

#if	_PROJECT_CAN_
/*******************************************************************************
 * Function:       void Save_F110 (uint8 nId)
 *
 * Description:     记忆 退出时记忆
 *
 * Returns:         none
 *
 * Notes:           nId 第一个信息 F111-F11F 对应 0-14		
 *
********************************************************************************/
void Save_F110(void)
{
	uint8 rtx[243];
	
	MemCopy(rtx, &TBL_F110ARRY[0][0], sizeof(TBL_F110ARRY));
	rtx[240] = F110SaveNumber;
	(void)KEA_EE_WriteRecord(FLASH_ID_F110, rtx);
}

/*******************************************************************************
 * Function:      void Save_ProgramData(void)
 *
 * Description:     存入flash中
 *
 * Returns:         none
 *
 * Notes:          none 
 *
********************************************************************************/
void Save_ProgramData(void)
{
   uint8 i;

   ProgramData[7] = 0;
   for (i = 0; i < 7; i++)
   {
      ProgramData[7] += ProgramData[i]; 
   }  
   ProgramData[7] = ~ProgramData[7];  //此处flash 做了校验和，因flash模块中就做了错误处理，结果读取时不校验此项。
   (void)KEA_EE_WriteRecord(FLASH_ID_PROGRAMDATA, ProgramData);
}
/*******************************************************************************
 * Function:        void Load_ProgramData(void)
 *
 * Description:     载入自检数据集
 *
 * Returns:         返回 programdata   
 *
 * Notes:         none		
 *
********************************************************************************/
void Load_ProgramData(void)
{
	if (KEA_EE_ReadRecord(FLASH_ID_PROGRAMDATA, ProgramData) == KEA_EE_ERROR_OK)
	{
      ProgramData[8]= 1;
	}
   else
   {//故障判断并没有堆programdata【7】做校验和
      ProgramData[0] = 0xff;
      ProgramData[1] = 0xff;
      ProgramData[2] = 0;
      ProgramData[3] = 0;
      ProgramData[4] = 0;
      ProgramData[5] = 0;
      ProgramData[6] = 0;
            
      ProgramData[8] = 0;
   }
}
#endif

/*******************************************************************************
 * Function:        BOOL Save_CycEE(void)
 *
 * Description:     掉电时调用
 *
 * Returns:         存储错误  FALSE
 *
 * Notes:           none		
 *
********************************************************************************/
static BOOL Save_ALLCycEE(void)
{  
	uint8 SData[UDS_EE_LENGTH];
	uint8 bSaveOK;
#if	_PROJECT_CAN_
	uint8 u8DOffset =0, i;
	PID_DATA *pPIDTbl = PID;
	
	for (i = 0; i < WRITE_PID_NUM; i++)
	{
		MemCopy(&SData[u8DOffset], pPIDTbl->pData, pPIDTbl->PIDLength);
		u8DOffset += pPIDTbl->PIDLength;
		pPIDTbl++;
	} 
  
	if (u8DOffset >= DID_SAVE_MAXLENGTH)
	{
		while (1)
		{
			MCU_NOP;
		}
	}
	SData[DID_SAVE_MAXLENGTH] = UDS_GetSecuriTimes();
#endif

	MemCopy(&SData[DID_SAVE_MAXLENGTH+1], u8ConfigData, 3);
	bSaveOK = KEA_EE_WriteRecord(FLASH_ID_UDS, SData);
	return bSaveOK;
}

/*******************************************************************************
 * Function:        BOOL Load_ALLCycEE(void) 
 *
 * Description:     读取数据
 *
 * Returns:         是否存储正确
 *
 * Notes:           none		
 *
********************************************************************************/
BOOL Load_ALLCycEE(void) 
{	
	uint8 SData[256];   //256 大于FLASH_EE_LENGTH_F110 ，UDS_EE_LENGTH
	uint8 bLoadErr;
#if	_PROJECT_CAN_	
	uint8 u8DOffset =0, i;
	PID_DATA *pPIDTbl = PID;

	if (KEA_EE_ReadRecord(FLASH_ID_F110, SData) == KEA_EE_ERROR_OK )
	{
	   MemCopy(&TBL_F110ARRY[0][0], SData, sizeof(TBL_F110ARRY));
	   F110SaveNumber = SData[240];
	}
	else
	{
		for (u8DOffset = 0; u8DOffset< 15; u8DOffset++)
		{
		   MemCopy(&TBL_F110ARRY[u8DOffset][0], TBL_F110, 16);
		}
		F110SaveNumber = 0;
	}
	
	
	if (KEA_EE_ReadRecord(FLASH_ID_UDS, SData)== KEA_EE_ERROR_OK )
	{
		u8DOffset = 0;
		for (i = 0; i < WRITE_PID_NUM; i++)
		{
			MemCopy((uint8*)pPIDTbl->pData, &SData[u8DOffset], pPIDTbl->PIDLength);
			u8DOffset += pPIDTbl->PIDLength;
			pPIDTbl++;
		}

		UDS_SetSecuriTimes(SData[DID_SAVE_MAXLENGTH]);	  
		MemCopy(u8ConfigData, &SData[DID_SAVE_MAXLENGTH+1], 3);
		bLoadErr = TRUE;
	}	
	else
	{
	   u8ConfigData[0] = 0;
	   (void)Save_UDSDID();   //将默认值写入在boot程序里不能加此句
	   bLoadErr = FALSE;
	}
#else
	if (KEA_FLASH_IMLoad(FLASH_EE_ADDR_UDS, SData, UDS_EE_LENGTH))
	{
		MemCopy(u8ConfigData, &SData[DID_SAVE_MAXLENGTH+1], 3);
		bLoadErr = TRUE;
	}
	else
	{
	   u8ConfigData[0] = 0;
	   (void)Save_UDSDID();   //将默认值写入在boot程序里不能加此句
	   bLoadErr = FALSE;
	}
#endif
	return bLoadErr;
}

/*******************************************************************************
 * Function:       void Save_xx(void)
 *
 * Description:     诊断记忆接口函数
 *
 * Returns:         
 *
 * Notes:           none		
 *
********************************************************************************/
void Save_UDSDID(void)
{
	(void)Save_ALLCycEE();
}

void Save_UDSConfig(void)
{
	(void)Save_ALLCycEE();
}

void Save_ScurityTimes(void)
{
	(void)Save_ALLCycEE();
}

/*******************************************************************************
 * Function:       void Save_MCUDATA(void)
 *
 * Description:     记忆MCUDATA
 *
 * Returns:         none
 *
 * Notes:          只被MCUPM 模块调用	
 *
********************************************************************************/
void Save_MCUDATA(void)
{
   uint8 u8Data[FLASH_EE_LENGTH2+2];
   
	MemCopy(&u8Data[0], (uint8*)(u32MCUData), FLASH_EE_LENGTH2);
	
   (void)KEA_EE_WriteRecord(FLASH_ID_MCUDATA, u8Data);
}

/*******************************************************************************
 * Function:       BOOL Load_MCUDATA(void)
 *
 * Description:     读取MCUDATA
 *
 * Returns:         none
 *
 * Notes:          只被MCUPM 模块调用	
 *
********************************************************************************/
BOOL Load_MCUDATA(void)
{
	uint8 u8Data[FLASH_EE_LENGTH2+2];
	uint8 bLoadRes;
	if (KEA_EE_ReadRecord(FLASH_ID_MCUDATA, u8Data) == KEA_EE_ERROR_OK)
	{//读取正常
		MemCopy((uint8*)(u32MCUData), &u8Data[0], FLASH_EE_LENGTH2);
		bLoadRes = TRUE;
	}
	else
	{ //读取错误
		bLoadRes = FALSE;
	}

	return bLoadRes;
}


/*******************************************************************************
 * Function:       void clear_save(void)
 *
 * Description:     BT刷写时擦除记忆值(noint及EEPROM)
 *
 * Returns:
 *
 * Notes:           none
 *
********************************************************************************/
void Clear_save(void)
{
	//擦除命令时进行擦除EEPROM
//	uint32 *pData = (uint32*)(RAM_NOINIT_ADDR);
//	uint8 i;
//	for (i = 0; i < 64; i++)
//	{
//		*pData++ = 0;
//	}//清noint区
//
//	Clear_WatchDog();
//	EraseFlash(FLASH_EE_ADDR_PARAMETER_SEC);//清标定参数
//	EraseFlash(FLASH_EE_ADDR_PARAMETER);
//	Clear_WatchDog();
	//清DTC 功能需要FDL ，所以此功能无法实现
	//清UDS 不需要

}
