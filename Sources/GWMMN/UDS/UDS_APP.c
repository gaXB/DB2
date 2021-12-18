/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�UDS_APP.c
* ����������UDS��ϵ�22��2F�������ϵͳӦ�õĽӿ��ļ������ļ��ǽ�APPLay����Ҫ������Ŀ��������밴�ձ�׼���ġ�
* 1  ������
* 2  ϵͳDID
* 3  2F����
*    ���ļ��ǽ�APPLay����Ҫ������Ŀ��������밴�ձ�׼���ġ�
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 

2019.1.17
ver0.1
1 ��ʼ�汾
2 ģ�����

2019.10.25
ver0.2
1 F110 DID���Ⱥ����ݵĺ궨��ģ��

2021.8.23
ver0.3
1 �Ż�uint8 DID_GetDidData(uint16 Num) ֻ��һ��retrun
*/
#include "APPDrive.h"
#include "UDS_APP.h"




typedef struct
{
	uint8 System; 
	uint8 fan;
	uint8 mode;
	uint8 test;
}IOCONTROLPARAMETER;
IOCONTROLPARAMETER sIOControl;



uint8 TBL_F110[PID_F110_LENGTH] = F110_DEFAULT_DATA;
uint8 F110SaveNumber;  //��������� ��ʼΪ0�� 1��д��F111�� ...15 F11F, 16��F112,.. 29 F11F.
                       //  15+ 14*n : f11f.   155 f11f , if >155  number = 16;    

uint8 TBL_F110ARRY[15][PID_F110_LENGTH]={F110_DEFAULT_DATA, F110_DEFAULT_DATA, F110_DEFAULT_DATA, F110_DEFAULT_DATA};

uint8 ProgramData[9];

/*******************************************************************************
 �� �ⲿ�ӿں���     
 * ֻ�ڴ��ļ��е���
********************************************************************************/
extern void Save_F110(void); 
/*******************************************************************************
 * Function:        SetCANBUSConfig
 *
 * Description:    ����CANbus ������
 *
 * Returns:         none
 *
 * Notes:          
 *
********************************************************************************/
uint8 DID_GetCANBUSConfig(uint8 u8Num)
{
	if (u8Num >= 1 && u8Num <= 3)
	{
		return u8ConfigData[u8Num - 1];
	}
	else
	{
		return 0;
	}
}
/*******************************************************************************
 * Function:  SetCANBUSConfig
 * 
 * Description: ����bus config
 *
 * Returns:    0- 255 ,
 *
 * Notes:    ��Ҫ����   
 *                         
 *                          
*******************************************************************************/
void DID_SetCANBUSConfig(uint8 u8Num, uint8 state)
{
	if (u8Num >= 1 && u8Num <= 3)
	{
		u8ConfigData[u8Num - 1] = state;
	}
	Save_UDSConfig();  
}
/*******************************************************************************
 * Function:  uint8 GetTempType(int16 i16Temp)
 * 
 * Description: �������¶�תΪ��ǰ���¶ȸ�ʽ
 *
 * Returns:    0- 255 ,
 *
 * Notes:       
 *                         
 *                          
*******************************************************************************/
static uint8 GetTempType(int16 i16Temp)
{
	if (i16Temp <= -400)
	{
		return 0;
	}
	else if (i16Temp >= 800)
	{
		return 240;
	}
	else
	{
		return  (uint8)(i16Temp/5 + 80);
	}
}
/*******************************************************************************
 * Function:  uint8 GetDidData(uint8 Num)
 * 
 * Description: ���õ�ǰ��did ��  ״̬������CCTV_EE_xx�淶
 *
 * Returns:    0- 255 ,
 *
 * Notes:    ��uds �ɵ��ô˺���             
 *                         
 *                          
*******************************************************************************/
uint8 DID_GetDidData(uint16 Num)
{
	uint8 u8RetData;
	switch(Num)
	{
		default:
			u8RetData = 255;
			break;
		case 0:
			u8RetData = GetTempType(sSenSorData.TinCal);
			break;
		case	1:
			u8RetData = GetTempType(sSenSorData.TambCal);
			break;
		case 2:
			u8RetData = GetTempType(sSenSorData.TevpCal);
			break;
		case 3:
			u8RetData = GetTempType(sSenSorData.TwaterCal);
			break;
		case 4:
			if (SystemControl.OffState)
			{
				u8RetData = 0;
			}
			else
			{
				u8RetData = (uint8)SystemControl.fanMode ;
			}
			break;
		case 5:
			u8RetData = (uint8)SystemControl.mdMode;
			break;
		case 6:
			u8RetData = (uint8)DCS_T(SystemControl.tset);
			break;
		case 7:
			u8RetData = ADValue_Get(ADID_FAN, ADVALUE_GETTYPE_8AD);
			break;
		case 8:
			u8RetData = 0;
			break;
		case 9:
			u8RetData = Motor_GetPosition(MOTOR_ID_MIX);
			break;
		case 0x0a:
			u8RetData = Motor_GetPosition(MOTOR_ID_MODE);
			break;
		case 0x0b:
			u8RetData = sSenSorData.u8VehicleSpeed;
			break;
		case 0x0c:
			u8RetData = 0;
			break;
		case 0x0d:
			u8RetData = 1;
			break;
		case 0x0E:
			u8RetData = 2;
			break;
	}
	return u8RetData;
}

/*******************************************************************************
 * Function:  uint8 GetDidData(uint8 Num)
 * 
 * Description: ���õ�ǰ��did ��  ״̬������CCTV_EE_xx�淶
 *
 * Returns:    0- 255 ,
 *
 * Notes:    ��uds �ɵ��ô˺���             
 *                         
 *                          
*******************************************************************************/
uint8 DID_SetDidData(uint16 Num, uint8 state, uint8 ControlType)
{
	uint8 u8SetRes = 0; //����ֵ
	uint8 u8Ret = 0;  //�Ƿ�ֱ�ӷ���
	switch(Num)
	{
		default:
			u8SetRes = 255;
			u8Ret = 1;
			break;
		case 4:
			sIOControl.fan = state;
			break;
		case 5:
			sIOControl.mode = state;
			break;
		case 6:
			sIOControl.test = state;
			break;
	}
	if (ControlType == 0 || ControlType == 2)
	{
		u8Ret = 0;
	}
	else
	{
		u8Ret = 1;
		switch(Num)
		{
			default:
				u8SetRes = 0;
				break;
			case 4:
				if (state <= MAX_FELVL)
				{
					SystemControl.fanMode = state;
					SystemControl.AutoBits.FAN = 0;
					u8SetRes = state;
				}
				else
				{
					u8SetRes = 0;
				}
				break;
			case 5:
				if (state <= MD_MODE_OST)
				{
					SystemControl.mdMode = (MDModeType)state;
					SystemControl.AutoBits.MODE = 0;
					u8SetRes = state;
				}
				else
				{
					u8SetRes = 0;
				}
			case 6:
				if (state <= 32 && state >= 18)
				{
					SystemControl.tset = SCD_T(state);
					u8SetRes = state;
					u8Ret = 1;
				}
				else
				{
					u8SetRes = 0;
				}
			case 0x0d:
				if (state <= 2)
				{
					OtherControl.CHeatMode_L = state;
					u8SetRes = state;
				}
				else
				{
					u8SetRes = state;
				}
			case 0x0e:
				if (state <= 2)
				{
					OtherControl.CHeatMode_R = state;
					u8SetRes = state;
				}
				else
				{
					u8SetRes = 0;
				}
		}
	}
	
	if (u8Ret == 0)
	{
		u8SetRes = DID_GetDidData(Num);  
	}
	else
	{
		
	}
	return u8SetRes;
}



/*******************************************************************************
 * Function:  uint8 GetIODid_SystemControl(void)
 * 
 * Description: �õ���ǰ��systemcontrol �� ״̬
 *
 * Returns:    ��ǰsystemcontrol����״̬��ÿһλ����Э�鶨��
 *
 * Notes:    ��uds �ɵ��ô˺���             
 *                         
 *                          
*******************************************************************************/
uint8 DID_IOGetSystemControl(void)
{
	uint8 u8StateData = 0;
	if (SystemControl.acMode)
	{
		u8StateData |= 0x01;
	}else{}
	
	if (SystemControl.ncfMode)
	{
		u8StateData |= 0x02;
	}else{}
	

	if (OtherControl.CHeatMode_L)
	{
		u8StateData |= 0x04;
	}else{}
	
	if (OtherControl.CHeatMode_R)
	{
		u8StateData |= 0x08;
	}else{}
	
	if (SystemControl.RdefMode == RDEF_MODE_ON)
	{
		u8StateData |= 0x10;
	}else{}
	
	
	return u8StateData;
}
/*******************************************************************************
 * Function:  void SetIODid_SystemControl(uint8 u8IOData)
 * 
 * Description: ���õ�ǰ��systemcontrol �� ״̬
 *
 * Returns:    none
 *
 * Notes:    ��uds �ɵ��ô˺���             
 *                         
 *                          
*******************************************************************************/
uint8 DID_IOSetSystemControl(uint8* u8IOData, uint8 ControlType)
{
	//uint8 u8State = 0;
	uint8 u8ControlData = u8IOData[0];
	uint8 u8MaskData = u8IOData[2];
	sIOControl.System = ControlType;
	if (ControlType == 0 || ControlType == 2)
	{
		
	}
	else if(ControlType == 3)
	{
		if (u8MaskData & 0x01)
		{
			if (u8ControlData & 0x01)
			{
				SystemControl.acMode = AC_MODE_ON;
			}
			else
			{
				SystemControl.acMode = AC_MODE_OFF;
			}
			SystemControl.AutoBits.AC = 0;
		}
		
		if (u8MaskData & 0x02)
		{
			if (u8ControlData & 0x02)
			{
				SystemControl.ncfMode = NCF_MODE_CIRF;
			}
			else
			{
				SystemControl.ncfMode = NCF_MODE_NEW;
			}
			SystemControl.AutoBits.NCF = 0;
		}
		
		if (u8MaskData & 0x04)
		{
			if (u8ControlData & 0x04)
			{
				OtherControl.CHeatMode_L = 1;
			}
			else
			{
				OtherControl.CHeatMode_L = 0;
			}
		}
		

		if (u8MaskData & 0x08)
		{
			if (u8ControlData & 0x08)
			{
				OtherControl.CHeatMode_R = 1;
			}
			else
			{
				OtherControl.CHeatMode_R = 0;
			}
		}
		

		if (u8MaskData & 0x10)
		{
			if (u8ControlData & 0x10)
			{
				SystemControl.RdefMode = RDEF_MODE_ON;
			}
			else
			{
				SystemControl.RdefMode = RDEF_MODE_OFF;
			}
		}
	}
	
	return DID_IOGetSystemControl();
}

/*****************************************************************************//*!
*
* @brief   void DID_SaveList(void)
*        
* @param  none
*
* @return none
*
* @ note �� did ����֮��֪ͨ��APP
*****************************************************************************/
void DID_UpdataList(void)
{
	Save_UDSDID();
}


//F110 �߼�
/*******************************************************************************
 * Function:       void Read_F110 (uint8 nId)
 *
 * Description:     ���� �˳�ʱ����
 *
 * Returns:         none
 *
 * Notes:           nId ��ǰ����ĸ���			
 *
********************************************************************************/
BOOL Read_F110(uint8 nId)
{
   nId--;
   if (nId >= 0x0f)  return FALSE;

   MemCopy(TBL_F110, &TBL_F110ARRY[nId][0], PID_F110_LENGTH);
   return TRUE;
}
/************************************************************

uint8 GetStorNumber_F110(uint8 F_Number)

�Ӵ洢�����õ� �洢�ı��

*&************************************************************/
uint8 GetStorNumber_F110(uint8 F_Number)
{
   
   if (F_Number == 0)  F_Number = 1;
   else if (F_Number <= 15)
   {
   //û�е���һȦ   
   }
   else
   {
      do
      {    //cal the number
         F_Number -= 14;    
      }while (F_Number > 15);
   }
   return F_Number;
}
/************************************************************

BOOL DID_ReadF110(uint8 *RDidData, uint8 num)

�õ�F110

*&************************************************************/
BOOL DID_ReadF110(uint8 *RDidData, uint8 F110Number)
{
   if (F110Number)
   {
      if (Read_F110(F110Number))
      {
         //������
      	MemCopy(RDidData, TBL_F110, PID_F110_LENGTH);
      	return TRUE;
      }
      else
      {  //�˴�û�з�����Ϣ������ ������Χ NRC
      	return FALSE;
      }
   }
   else
   {
   	F110Number = GetStorNumber_F110(F110SaveNumber);

      if (Read_F110(F110Number))
      {
         //������
      	MemCopy(RDidData, TBL_F110, PID_F110_LENGTH);
      	return TRUE;
      }
      else
      {  //�˴�û�з�����Ϣ������ ������Χ NRC
      	return FALSE;
      }
   }
}
/************************************************************

void DID_WriteF110(uint8* WDidData)

д F110

*&************************************************************/
void DID_WriteF110(uint8* WDidData)
{
	uint8 nId;	
	MemCopy(TBL_F110, WDidData, PID_F110_LENGTH);
	
	F110SaveNumber++ ;
	if (F110SaveNumber >= 155)  F110SaveNumber = 16;	
 
	nId = GetStorNumber_F110(F110SaveNumber);
	nId--;
	if (nId >= 0x0f)  return;
    MemCopy(&TBL_F110ARRY[nId][0], TBL_F110, PID_F110_LENGTH);
   
   Save_F110();
}
/*****************************************************************************//*!
*
* @brief    void  Run_BootLoader(void)
*        
* @param  none
*
* @return none
*
* @ note �� ǿ����ת��bootload λ�ã�����ػ�ǰ״̬
*****************************************************************************/
void  Run_BootLoader(void)
{
	sSW_Reset();
}
/*****************************************************************************//*!
*
* @brief    void ApplUDS_GotoProgramMode(void)
*        
* @param  none
*
* @return none
*
* @ note ��UDS ��Ҫ����bootoadģʽ
*****************************************************************************/
void ApplUDS_GotoProgramMode(void)
{
   ProgramData[PD_NUM_SESSIONMODE] = 2;
   Save_ProgramData();
   Save_ScurityTimes(); 
   Run_BootLoader();
}

/*****************************************************************************//*!
*
* @brief    void ApplUDS_Reset(void)
*        
* @param  none
*
* @return none
*
* @ note ��UDS ��Ҫ���븴λ
*****************************************************************************/
void ApplUDS_Reset(void)
{
	(void)Save_ScurityTimes();  
	sSW_Reset();
}


