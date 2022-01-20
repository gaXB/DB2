/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SystemSaveMoudle.c
* ��������������洢ģ��ʹ浽flash����ؼ�����з�װ, ��ģ�鲻��װ�����ܸ���
* 
* 1  ��ģ�����ʵ��ϵͳ�Ĵ洢�ӿڹ��ܣ�����flah�Ĵ洢ʵ����FLASH_KEA_EEPROM��ʵ��
* ����flash �洢�ķ���������ʼ���� ��MCU ��ء�
* 2  ������䣬 ϵͳ״ֵ̬�ʹ�����ֵ�Ĵ洢�������غ��� MemoryControl��ʵ�֣����ڼ���һ
* 3  progarm data ����  ��������һ������
* 4  UDS ����
* 1) dtc���� ��������һ����������IGN �ʹ���
* 2�� uds did ���ݣ���ȫ���ʴ����������ַ���һ������
* 3�� F110 ������һ������
* 4���궨���� ��������һ������
* 
* �������� codewarrior IDE 11.1
* MCU˵���� Ϊ�ϲ�Ӧ�ú�MCU���ݴ洢֮��Ľӿڡ�
* ֻ������KEAZ128��֧��KEAZ64
* ��ģ�鲻��װ�����ܸ���
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��9��5��
* 
* �汾��2.0
* ˵������ȡconfig bug���£�1��0 ֱ�Ӹ���
*       2 ����DID ��С���ܳ���200��������ѭ��
*        3  ����security parameter
* ���ߣ�xb
* ������ڣ�2019��12��30��
* 
* * �汾��3.0
* ˵����1 load_allcycle bug
* ���ߣ�xb
* ������ڣ�2020��1��14��
*
** * �汾��3.1
* ˵����1 ���� MCUdata�Ĵ洢
* ���ߣ�xb
* ������ڣ�2020��5��13��
* 
* �汾��4.0
* ˵����1 �Զ���˪��Ϊ����Ĺ���ȡ��
* ���ߣ�xb
* ������ڣ�2020��12��9��
*/

#include "APPDrive.h"
#include "SystemSaveFlash.h"
#include "FLASH_KEA_EEPROM.h"
#include "Memory.h"
static SYSTEMCONTROL SysConData;


//�ⲿʹ�õĺ���
extern void UDS_SetSecuriTimes(uint8 u8Times);
extern uint8 UDS_GetSecuriTimes(void);
extern BOOL  SetParameterData(uint8* rtx);

/**************************************************************
 * ����ûһ���ID���ʹ洢����0
 * 
**************************************************************/

//2Ƭ�洢������Ŷ���
#define   FLASH_ID_IGN           3       //���洢
#define   FLASH_ID_DTC           4       //����,dTC,
#define   FLASH_ID_MCUDATA       5       //mcudata

#define   FLASH_ID_PROGRAMDATA   0
#define   FLASH_ID_F110          1
#define   FLASH_ID_UDS           2
#define   FLASH_ID_PARAMETER     6
#define   FLASH_ID_PARAMETER_SEC     7
#define   FLASH_ID_MAXNUM        8
// 2Ƭ�洢���������ȶ���
#define  SAVE_SIZE     sizeof(SYSTEMCONTROL)
#define  FLASH_EE_LENGTH0  SAVE_SIZE
#define  FLASH_EE_LENGTH1  (DTC_MAXNUM * sizeof(DTCSNAPDATA) + DTC_MAXNUM)
#define  FLASH_EE_LENGTH2  0x1c
//1Ƭ�洢���������ȶ���
#define  DID_SAVE_MAXLENGTH            200      //ʹ�õ����ĳ���
#define  UDS_EE_LENGTH                 (DID_SAVE_MAXLENGTH + 4)    //UDS
#define  FLASH_EE_LENGTH_F110          241
#define  FLASH_EE_LENGTH_PROGRAM       9
#define  FLASH_EE_LENGTH_PARAMETER     CAILBRATION_PARAMETER_NUM


//�����ַ����
#define  FLASH_EE_ADDR0             0x1F000u
#define  FLASH_EE_ADDR1             0x1F400u  
#define  FLASH_EE_ADDR2             0x1E000u  
//����ΪһƬ
#define  FLASH_EE_ADDR_PROGRAMDATA  0x1F800u
#define  FLASH_EE_ADDR_F110         0x1FC00u
#define  FLASH_EE_ADDR_UDS          0x1FA00u
#define  FLASH_EE_ADDR_PARAMETER    0x1FE00u  
#define  FLASH_EE_ADDR_PARAMETER_SEC    0x1EE00u  


/*******************************************************************************
 * Function:       void FLASH_EE_Init(void)
 *
 * Description:       flash��ʼ����
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
 * Description:     ������ؼ���
 *
 * Returns:         none
 *
 * Notes:          ����SystemControl���ݼ��䣬���ڵ�һ��flah
 *                 ����ȡ����ʱĬ��25��ȫAUTO. ��ˢ������ĵ�һ���ϵ�
 *                 void	MemoryControl(void) ���غ�����loop����
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
	{//��ȡ����
		MemCopy((uint8*)(&SystemControl), &u8Data[0], sizeof(SYSTEMCONTROL) );
		bLoadRes = TRUE;
	}
	else
	{ //��ȡ����
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
 * Description:     DTC ��ؼ���
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
 * Description:     �������
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
 * Description:     �������
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
 * Description:     ���� �˳�ʱ����
 *
 * Returns:         none
 *
 * Notes:           nId ��һ����Ϣ F111-F11F ��Ӧ 0-14		
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
 * Description:     ����flash��
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
   ProgramData[7] = ~ProgramData[7];  //�˴�flash ����У��ͣ���flashģ���о����˴����������ȡʱ��У����
   (void)KEA_EE_WriteRecord(FLASH_ID_PROGRAMDATA, ProgramData);
}
/*******************************************************************************
 * Function:        void Load_ProgramData(void)
 *
 * Description:     �����Լ����ݼ�
 *
 * Returns:         ���� programdata   
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
   {//�����жϲ�û�ж�programdata��7����У���
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
 * Description:     ����ʱ����
 *
 * Returns:         �洢����  FALSE
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
 * Description:     ��ȡ����
 *
 * Returns:         �Ƿ�洢��ȷ
 *
 * Notes:           none		
 *
********************************************************************************/
BOOL Load_ALLCycEE(void) 
{	
	uint8 SData[256];   //256 ����FLASH_EE_LENGTH_F110 ��UDS_EE_LENGTH
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
	   (void)Save_UDSDID();   //��Ĭ��ֵд����boot�����ﲻ�ܼӴ˾�
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
	   (void)Save_UDSDID();   //��Ĭ��ֵд����boot�����ﲻ�ܼӴ˾�
	   bLoadErr = FALSE;
	}
#endif
	return bLoadErr;
}

/*******************************************************************************
 * Function:       void Save_xx(void)
 *
 * Description:     ��ϼ���ӿں���
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
 * Description:     ����MCUDATA
 *
 * Returns:         none
 *
 * Notes:          ֻ��MCUPM ģ�����	
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
 * Description:     ��ȡMCUDATA
 *
 * Returns:         none
 *
 * Notes:          ֻ��MCUPM ģ�����	
 *
********************************************************************************/
BOOL Load_MCUDATA(void)
{
	uint8 u8Data[FLASH_EE_LENGTH2+2];
	uint8 bLoadRes;
	if (KEA_EE_ReadRecord(FLASH_ID_MCUDATA, u8Data) == KEA_EE_ERROR_OK)
	{//��ȡ����
		MemCopy((uint8*)(u32MCUData), &u8Data[0], FLASH_EE_LENGTH2);
		bLoadRes = TRUE;
	}
	else
	{ //��ȡ����
		bLoadRes = FALSE;
	}

	return bLoadRes;
}


/*******************************************************************************
 * Function:       void clear_save(void)
 *
 * Description:     BTˢдʱ��������ֵ(noint��EEPROM)
 *
 * Returns:
 *
 * Notes:           none
 *
********************************************************************************/
void Clear_save(void)
{
	//��������ʱ���в���EEPROM
//	uint32 *pData = (uint32*)(RAM_NOINIT_ADDR);
//	uint8 i;
//	for (i = 0; i < 64; i++)
//	{
//		*pData++ = 0;
//	}//��noint��
//
//	Clear_WatchDog();
//	EraseFlash(FLASH_EE_ADDR_PARAMETER_SEC);//��궨����
//	EraseFlash(FLASH_EE_ADDR_PARAMETER);
//	Clear_WatchDog();
	//��DTC ������ҪFDL �����Դ˹����޷�ʵ��
	//��UDS ����Ҫ

}
