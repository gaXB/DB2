/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：UDS_APP.c
* 功能描述：UDS诊断的22，2F服务控制系统应用的接口文件，此文件是将APPLay中需要更改项目提出来。请按照标准更改。
* 1  配置字
* 2  系统DID
* 3  2F控制
*    此文件是将APPLay中需要更改项目提出来。请按照标准更改。
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 

2019.1.17
ver0.1
1 初始版本
2 模板程序

2019.10.25
ver0.2
1 F110 DID长度和内容的宏定义模板

2021.8.23
ver0.3
1 优化uint8 DID_GetDidData(uint16 Num) 只用一个retrun
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
uint8 F110SaveNumber;  //记忆次数， 初始为0。 1：写第F111， ...15 F11F, 16：F112,.. 29 F11F.
                       //  15+ 14*n : f11f.   155 f11f , if >155  number = 16;    

uint8 TBL_F110ARRY[15][PID_F110_LENGTH]={F110_DEFAULT_DATA, F110_DEFAULT_DATA, F110_DEFAULT_DATA, F110_DEFAULT_DATA};

uint8 ProgramData[9];

/*******************************************************************************
 × 外部接口函数     
 * 只在此文件中调用
********************************************************************************/
extern void Save_F110(void); 
/*******************************************************************************
 * Function:        SetCANBUSConfig
 *
 * Description:    设置CANbus 的配置
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
 * Description: 设置bus config
 *
 * Returns:    0- 255 ,
 *
 * Notes:    需要记忆   
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
 * Description: 将现在温度转为以前的温度格式
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
 * Description: 设置当前的did 的  状态，按照CCTV_EE_xx规范
 *
 * Returns:    0- 255 ,
 *
 * Notes:    由uds 可调用此函数             
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
 * Description: 设置当前的did 的  状态，按照CCTV_EE_xx规范
 *
 * Returns:    0- 255 ,
 *
 * Notes:    由uds 可调用此函数             
 *                         
 *                          
*******************************************************************************/
uint8 DID_SetDidData(uint16 Num, uint8 state, uint8 ControlType)
{
	uint8 u8SetRes = 0; //返回值
	uint8 u8Ret = 0;  //是否直接返回
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
 * Description: 得到当前的systemcontrol 的 状态
 *
 * Returns:    当前systemcontrol，的状态，每一位按照协议定义
 *
 * Notes:    由uds 可调用此函数             
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
 * Description: 设置当前的systemcontrol 的 状态
 *
 * Returns:    none
 *
 * Notes:    由uds 可调用此函数             
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
* @ note ： did 被改之后通知的APP
*****************************************************************************/
void DID_UpdataList(void)
{
	Save_UDSDID();
}


//F110 逻辑
/*******************************************************************************
 * Function:       void Read_F110 (uint8 nId)
 *
 * Description:     记忆 退出时记忆
 *
 * Returns:         none
 *
 * Notes:           nId 当前记忆的个数			
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

从存储次数得到 存储的编号

*&************************************************************/
uint8 GetStorNumber_F110(uint8 F_Number)
{
   
   if (F_Number == 0)  F_Number = 1;
   else if (F_Number <= 15)
   {
   //没有到达一圈   
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

得到F110

*&************************************************************/
BOOL DID_ReadF110(uint8 *RDidData, uint8 F110Number)
{
   if (F110Number)
   {
      if (Read_F110(F110Number))
      {
         //有数据
      	MemCopy(RDidData, TBL_F110, PID_F110_LENGTH);
      	return TRUE;
      }
      else
      {  //此处没有返回信息，返回 超出范围 NRC
      	return FALSE;
      }
   }
   else
   {
   	F110Number = GetStorNumber_F110(F110SaveNumber);

      if (Read_F110(F110Number))
      {
         //有数据
      	MemCopy(RDidData, TBL_F110, PID_F110_LENGTH);
      	return TRUE;
      }
      else
      {  //此处没有返回信息，返回 超出范围 NRC
      	return FALSE;
      }
   }
}
/************************************************************

void DID_WriteF110(uint8* WDidData)

写 F110

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
* @ note ： 强制跳转到bootload 位置，保存关机前状态
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
* @ note ：UDS 需要进入bootoad模式
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
* @ note ：UDS 需要进入复位
*****************************************************************************/
void ApplUDS_Reset(void)
{
	(void)Save_ScurityTimes();  
	sSW_Reset();
}


