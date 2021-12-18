/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：MoudleDrive.c
* 功能描述：模块驱动接口
* 1  SignalFilter模块接口改到信号刷新模块
* 2  DTC 
* 3  CAN UDS
* 编译器： codewarrior IDE 11.1
* MCU说明：此文件跟MCU 相关，适用于KEAZ128
* 需要底层和应用状态
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
* 
*  版本：1.1
* 说明：增加输入输出强制功能
* 作者：xb
* 完成日期：2020年5月30日

*  版本：1.2
* 说明：优化不同宏定义时的编译问题
* 作者：xb
* 完成日期：2020年9月27日
* 
*  版本：1.3
* 说明：参数更改函数的写法更改，便于更改
* 作者：xb
* 完成日期：2021年3月12日
* 
* 版本：1.4
* 说明：枚举类型赋值优化
* 作者：xb
* 完成日期：2021年8月23日
*/
#include "MoudleDrive.h"
#include "APPDrive.h"
#include "ExtParameter.h"



/*******************************************************************************
 * Function:  uint8 GetDTCState(uint8 DtcNum)
 *
 * Description: 返回此dtc 是否故障
 *
 * Returns:     故障为1 
 *
 * Notes:     供Deal_dtc 使用
 *
*******************************************************************************/
eDTCRETSTATE GetDTCState(uint8 DtcNum)
{
	//TODO: DTC模块驱动编写，可参考模板
	eDTCRETSTATE u8DTCResult;
	switch(DtcNum)
	{
		default:
			//其余都认为故障信号
			u8DTCResult = DTC_RETSTATE_OK;
			break;
#if _PROJECT_CAN_
		case  DTCNUM_BUSOFF:   
			if (InCAN_GetBusOffDTC())
			{
				u8DTCResult = DTC_RETSTATE_COMFIRM_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_IMMEDI_OK;
			}
			break;
#endif
	}

	return u8DTCResult;
}


void ReCord_DTCSnap(uint8 num)
{
	//TODO: DTC模块驱动编写
	DTCSnap[num].u8Volt =(uint8) (ADPhy_GetFact(ADID_VOLT) / 10); 
	DTCSnap[num].u32ODO = 0x0100;//Get_Odometer();
	DTCSnap[num].u8WaterTemp = 140;//(uint8)sSenSorData.TwaterCal;
	DTCSnap[num].IgnState = 0xff;
}


/*******************************************************************************
 * Function:  void GetSystemData(uint8* pSystemData)
 * 
 * Description: 读取系统状态， 22服务的 FDFF （DID 读取）
 *
 * Returns:    none
 *
 * Notes:    由uds 可调用此函数     ，在上位机第一个界面中完成        
 *                         
 *                          
*******************************************************************************/
#if _PROJECT_CAN_
void GetSystemData(uint8* pSystemData)
{
	//TODO: 标定界面，按ramdata.csv 编写， 可参考模板
#if _PROJECT_AUTO_  
	TDDATA lTDData;
	AUTORESULT lAutoResult;
	uint8 u8RxNum;
	
	lTDData = AUTOTD_GetTD();
	lAutoResult = AUTOTD_GetARes(); 

	pSystemData[0] = (uint8)SystemControl.tset;
	pSystemData[1] = (uint8)(SystemControl.tset >> 8);
	
	
	if (SystemControl.AutoBits.MODE)
		pSystemData[2] = 0x55;
	else   
		pSystemData[2] = SystemControl.mdMode;

	if (SystemControl.AutoBits.NCF)
		pSystemData[3] = 0x55;
	else 
		pSystemData[3] = SystemControl.ncfMode;
	
	if (SystemControl.AutoBits.FAN)
		pSystemData[4] = 0x55;
	else  
		pSystemData[4] = SystemControl.fanMode;		
	
	if (SystemControl.AutoBits.AC)
		pSystemData[5] = 0x55;
	else  
		pSystemData[5] = SystemControl.acMode;	
	
	u8RxNum = 6;
	pSystemData[u8RxNum++]=	(uint8)sSenSorData.TinCal;	
	pSystemData[u8RxNum++]=	(uint8)(sSenSorData.TinCal >> 8);

   pSystemData[u8RxNum++] = (uint8)ADPhy_GetFact(ADID_TIN);
   pSystemData[u8RxNum++] = (uint8)(ADPhy_GetFact(ADID_TIN) >> 8);
	
	pSystemData[u8RxNum++]=	(uint8)sSenSorData.TambCal;	
	pSystemData[u8RxNum++]=	(uint8)(sSenSorData.TambCal >> 8);
	
   pSystemData[u8RxNum++] = (uint8)ADPhy_GetFact(ADID_TAMB);
   pSystemData[u8RxNum++] = (uint8)(ADPhy_GetFact(ADID_TAMB) >> 8);
   
	pSystemData[u8RxNum++]=	(uint8)sSenSorData.TevpCal;	
	pSystemData[u8RxNum++]=	(uint8)(sSenSorData.TevpCal >> 8);
   
	pSystemData[u8RxNum++]=	(uint8)sSenSorData.TwaterCal;	
	pSystemData[u8RxNum++]=	(uint8)(sSenSorData.TwaterCal >> 8);
	
	pSystemData[u8RxNum++] = (uint8)sSenSorData.u8VehicleSpeed;
	
	pSystemData[u8RxNum++] = (uint8)lTDData.TDResult;//TD	
	pSystemData[u8RxNum++] = (uint8)(lTDData.TDResult >>8);//TD	
	
	pSystemData[u8RxNum++] = (uint8)((uint16)lTDData.i16TDRamb & 0x00ff);
	pSystemData[u8RxNum++] = (uint8)((uint16)lTDData.i16TDRamb >> 8) ;
	
	pSystemData[u8RxNum++] = (uint8)(lTDData.i16TDRsun);//TD DEP	    
	pSystemData[u8RxNum++] = (uint8)(lTDData.i16TDRsun >> 8);//TD DEP	 
	pSystemData[u8RxNum++] = (uint8)sSenSorData.u8ADSunR;
	
	pSystemData[u8RxNum++] = (uint8)(lAutoResult.aFanVolt/10)  ;//(uint8) (FanControl_GetCurrentVolt() / 10)
	pSystemData[u8RxNum++] = FanControl_GetCurrentLevel();	
	
	pSystemData[u8RxNum++] = sOutData.ACOut;
	
	pSystemData[u8RxNum++] = SystemControl.ncfMode;
	
	pSystemData[u8RxNum++] = SystemControl.mdMode;
	
	pSystemData[u8RxNum++] = Motor_GetPosition(MOTOR_ID_MIX);
	
	pSystemData[u8RxNum++] = Motor_GetSetPosition(MOTOR_ID_MIX) ;
	
	pSystemData[u8RxNum++] = Motor_GetPosition(MOTOR_ID_MODE);

	pSystemData[u8RxNum++] = Motor_GetSetPosition(MOTOR_ID_MODE);
	
	pSystemData[u8RxNum++] = Motor_GetPosition(MOTOR_ID_NCF);
	
	pSystemData[u8RxNum++] = Motor_GetSetPosition(MOTOR_ID_NCF) ;
	
	if (lAutoResult.CFanState)
	{
		pSystemData[u8RxNum] = lAutoResult.CFanState;
	}
	else if (lAutoResult.HFanState)
	{
		pSystemData[u8RxNum] = lAutoResult.HFanState;
		pSystemData[u8RxNum] += 0x10;
	}
	else
	{
		pSystemData[u8RxNum] = 0;
	}
	u8RxNum++;
	pSystemData[u8RxNum++] = (uint8)sSenSorData.TVentF;		
	pSystemData[u8RxNum++] = (uint8)(sSenSorData.TVentF >> 8);
	
	pSystemData[u8RxNum++] = (uint8)sSenSorData.TVentD;		
	pSystemData[u8RxNum++] = (uint8)(sSenSorData.TVentD >> 8);
	
	pSystemData[u8RxNum++] = (uint8)sSenSorData.TinSimul;		
	pSystemData[u8RxNum++] = (uint8)(sSenSorData.TinSimul >> 8);
	
	pSystemData[u8RxNum++] = (uint8)sSenSorData.Tddt;		
	pSystemData[u8RxNum++] = (uint8)(sSenSorData.Tddt >> 8);
	pSystemData[u8RxNum++] = (uint8)sSenSorData.Tadt;		
	pSystemData[u8RxNum++] = (uint8)(sSenSorData.Tadt >> 8);
#endif
}



/*******************************************************************************
 * Function:  void GetParameterData(uint8* rtx)
 * 
 * Description: 读取系统状态， 22服务的 IODID_SYSTEMPARMETER  0xFDDE（DID 读取）
 *
 * Returns:    none
 *
 * Notes:    由uds 可调用此函数     ，在上位机第2个界面中完成        
 *                         
 *                          
*******************************************************************************/
typedef struct
{
	void*  pData;
	uint8   u8Length;
}PC_PARAMETER_TYPE;
PC_PARAMETER_TYPE TBL_SetParameter[]=
{
#if _PROJECT_AUTO_
	{&TDSunAddTimer, 1},
	{&TDSunDecTimer, 1},
	{&u16KTset, 2},
	{&u16KTin, 2},
	{&u16KTamb, 2},
	{&u16KSun, 2},
	{TBL_SEGTAMB, 20},
	{TBL_OFFSETAMB, 20},
	{TBL_ADSUN, 6},
	{TBL_OFFSETSUN, 6},
	{&tACON_EVP, 2},
	{&tACOFF_EVP, 2},
	{TBL_ACTAMB, 4},
	{TBL_FANTD, 32},
	{&TBL_AFANVOLT[1], 16},
	{TBL_MODETD, 8},
	{TBL_NCFTD, 4},
	{&sMIXVentPar.i16Kp, 2},
	{&sMIXVentPar.i16Ki, 2},
	{&sMIXVentPar.i16Kd, 2},
	{&u8G_TD, 1},
	{&u8T1Per, 1},
	{&u8BV_FRESH, 1},
	{&fVoltBLR_VSMAX, 2},
#endif
	{PDATA_NULL, 1},   //最后一个标记
};
void GetParameterData(uint8* rtx)
{
	//TODO: 返回标定参数，参考模板
	int16 iNum;
	uint8* pDercData = rtx;
	for (iNum =0 ; TBL_SetParameter[iNum].pData != PDATA_NULL; iNum++)
	{
		MemCopy(pDercData, TBL_SetParameter[iNum].pData,  TBL_SetParameter[iNum].u8Length);
		pDercData += TBL_SetParameter[iNum].u8Length; 
	}
}

//设置并将数据记忆
BOOL  SetParameterData(uint8* rtx)
{
	//TODO: 设置标定参数，参考模板
	int16 iNum;
	uint8* pSrcData = rtx;
	for (iNum =0 ; TBL_SetParameter[iNum].pData != PDATA_NULL; iNum++)
	{
		MemCopy(TBL_SetParameter[iNum].pData, pSrcData, TBL_SetParameter[iNum].u8Length);
		pSrcData += TBL_SetParameter[iNum].u8Length; 
	}
	Save_Parameter(rtx);
	return TRUE;
}



/*******************************************************************************
 * Function:  void GetParameterData_SEC(uint8* rtx)
 * 
 * Description: 读取系统状态， 22服务的 IODID_SYSTEMPARMETER  0xFDDE（DID 读取）
 *
 * Returns:    none
 *
 * Notes:    由uds 可调用此函数     ，在上位机第2个界面中完成        
 *                         
 *                          
*******************************************************************************/
void GetParameterData_Sec(uint8* rtx)
{
	//TODO: 得到标定参数2，参考模板
}
//设置并将数据记忆
BOOL  SetParameterData_Sec(uint8* rtx)
{
	//TODO: 设置标定参数2，参考模板
	Save_Parameter_Sec(rtx);
	return TRUE;
}

/*******************************************************************************
 * Function:  void SetSetData(uint8* rtx)
 * 
 * Description: 读取系统状态， 22服务的 DID_SYSTMESET 实现
 *
 * Returns:    none
 *
 * Notes:    由uds 可调用此函数     ，在上位机的调试界面中完成        
 *                         
 *                          
*******************************************************************************/
BOOL  SetSetData(uint8* rtx)
{
	SystemControl.tset = (int16)((rtx[1]<<8) + rtx[0]);
	SystemControl.acMode = (ACModeType)rtx[2];
	SystemControl.ncfMode = (NcfModeType)rtx[3];
	SystemControl.mdMode = (MDModeType)rtx[4];	
	SystemControl.fanMode = rtx[5];

	SystemControl.RdefMode = (RdefModeType)rtx[6];

	sOutData_PC.ACOut = rtx[7];
	sOutData_PC.FANVolt = (rtx[8]) + (rtx[9]<<8);
	sOutData_PC.MIXAD = rtx[10];
	sOutData_PC.MODEAD = rtx[11];
	sOutData_PC.NCFAD = rtx[12];
	sOutData_PC.RHeatOut = rtx[13];
	
	MemCopy((uint8*)&sSenSorData_PC, &rtx[14],  18);
	
	MemCopy((uint8*)&PCControlEn, &rtx[230],  10);
	
	if (PCControlEn.Bits.b2)
	{
		SystemControl.AutoBits.AC = 0;
	}
	
	if (PCControlEn.Bits.b3)
	{
		SystemControl.AutoBits.NCF = 0;
	}
	
	if (PCControlEn.Bits.b4)
	{
		SystemControl.AutoBits.MODE = 0;
	}
	
	if (PCControlEn.Bits.b5)
	{
		SystemControl.AutoBits.FAN = 0;
	}else{}
	
	if (PCControlEn.Bits.b1)
	{
		SystemControl.AutoBits.AC = 0;
	}else{}

	if (PCControlEn.Bits.b2)
	{
		SystemControl.AutoBits.NCF = 0;
	}else{}

	if (PCControlEn.Bits.b3)
	{
		SystemControl.AutoBits.MODE = 0;
	}else{}
	
	if (PCControlEn.Bits.b4)
	{
		SystemControl.AutoBits.FAN = 0;
		if (SystemControl.fanMode == 0)
		{
			SystemControl.OffState = OFF_STATE_OFF;
		}
		else
		{
			SystemControl.OffState = OFF_STATE_ON;
		}
	}else{}
	return TRUE;
}

void GetSetData(uint8* rtx)
{
	rtx[0] = (uint8)SystemControl.tset;
	rtx[1] = (uint8)(SystemControl.tset >>8);
	rtx[2] = SystemControl.acMode;
	rtx[3] = SystemControl.ncfMode;
	rtx[4] = SystemControl.mdMode;
	rtx[5] = SystemControl.fanMode;
	rtx[6] = SystemControl.RdefMode;
	
	rtx[7] = sOutData.ACOut;
	rtx[8] = (uint8)sOutData.FANVolt;
	rtx[9] = (uint8)(sOutData.FANVolt >>8);
	rtx[10] = sOutData.MIXAD;
	rtx[11] = sOutData.MODEAD;
	rtx[12] = sOutData.NCFAD;
	rtx[13] = sOutData.RHeatOut;
	MemCopy(&rtx[14], (uint8*)&sSenSorData, 18);
	
	MemCopy(&rtx[230], (uint8*)&PCControlEn, 10);
}

/*******************************************************************************
 * Function:  void PCSet_SenSorData(void)
 * 
 * Description: 设置传感器的功能
 *
 * Returns:    none
 *
 * Notes:    由lowerdrive调用 
 *                         
 *                          
*******************************************************************************/
void PCSet_SenSorData(void)
{
	if (PCControlEn.Bits.b12)
	{
		sSenSorData.TambCal = sSenSorData_PC.TambCal;
	}
	
	if (PCControlEn.Bits.b13)
	{
		sSenSorData.TinCal = sSenSorData_PC.TinCal;
	}
	
	if (PCControlEn.Bits.b14)
	{
		sSenSorData.TevpCal = sSenSorData_PC.TevpCal;
	}
	
	if (PCControlEn.Bits.b15)
	{
		sSenSorData.TwaterCal = sSenSorData_PC.TwaterCal;
	}
	
	if (PCControlEn.Bits.b16)
	{
		sSenSorData.u8ADSunR = sSenSorData_PC.u8ADSunR;
	}
	
	if (PCControlEn.Bits.b17)
	{
		sSenSorData.u8VehicleSpeed = sSenSorData_PC.u8VehicleSpeed;
	}
	
	if (PCControlEn.Bits.b18)
	{
		sSenSorData.TVentF = sSenSorData_PC.TVentF;
	}
	
	if (PCControlEn.Bits.b19)
	{
		sSenSorData.TVentD = sSenSorData_PC.TVentD;
	}
	
	if (PCControlEn.Bits.b20)
	{
		sSenSorData.Tadt = sSenSorData_PC.Tadt;
	}
	
	if (PCControlEn.Bits.b21)
	{
		sSenSorData.Tddt = sSenSorData_PC.Tddt;
	}
}


/*******************************************************************************
 * Function:   void PCSet_OutData(void)
 *
 * Description: PC 控制
 *
 * Returns:     none
 *
 * Notes:      
 *
*******************************************************************************/
void PCSet_OutData(void)
{
	if (PCControlEn.Bits.b6)
	{
		sOutData.ACOut = sOutData_PC.ACOut;
	}else{}
	
	if (PCControlEn.Bits.b7)
	{
		sOutData.FANVolt = sOutData_PC.FANVolt;
	}else{}
	
	if (PCControlEn.Bits.b8)
	{
		sOutData.MIXAD = sOutData_PC.MIXAD;
	}else{}
	
	if (PCControlEn.Bits.b9)
	{
		sOutData.MODEAD = sOutData_PC.MODEAD;
	}else{}
	
	if (PCControlEn.Bits.b10)
	{
		sOutData.NCFAD = sOutData_PC.NCFAD;
	}else{}
	
	if (PCControlEn.Bits.b11)
	{
		sOutData.RHeatOut = sOutData_PC.RHeatOut;
	}else{}
}
#endif

