/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�MoudleDrive.c
* ����������ģ�������ӿ�
* 1  SignalFilterģ��ӿڸĵ��ź�ˢ��ģ��
* 2  DTC 
* 3  CAN UDS
* �������� codewarrior IDE 11.1
* MCU˵�������ļ���MCU ��أ�������KEAZ128
* ��Ҫ�ײ��Ӧ��״̬
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
* 
*  �汾��1.1
* ˵���������������ǿ�ƹ���
* ���ߣ�xb
* ������ڣ�2020��5��30��

*  �汾��1.2
* ˵�����Ż���ͬ�궨��ʱ�ı�������
* ���ߣ�xb
* ������ڣ�2020��9��27��
* 
*  �汾��1.3
* ˵�����������ĺ�����д�����ģ����ڸ���
* ���ߣ�xb
* ������ڣ�2021��3��12��
* 
* �汾��1.4
* ˵����ö�����͸�ֵ�Ż�
* ���ߣ�xb
* ������ڣ�2021��8��23��
*/
#include "MoudleDrive.h"
#include "APPDrive.h"
#include "ExtParameter.h"



/*******************************************************************************
 * Function:  uint8 GetDTCState(uint8 DtcNum)
 *
 * Description: ���ش�dtc �Ƿ����
 *
 * Returns:     ����Ϊ1 
 *
 * Notes:     ��Deal_dtc ʹ��
 *
*******************************************************************************/
eDTCRETSTATE GetDTCState(uint8 DtcNum)
{
	//TODO: DTCģ��������д���ɲο�ģ��
	eDTCRETSTATE u8DTCResult;
	switch(DtcNum)
	{
		default:
			//���඼��Ϊ�����ź�
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
	//TODO: DTCģ��������д
	DTCSnap[num].u8Volt =(uint8) (ADPhy_GetFact(ADID_VOLT) / 10); 
	DTCSnap[num].u32ODO = 0x0100;//Get_Odometer();
	DTCSnap[num].u8WaterTemp = 140;//(uint8)sSenSorData.TwaterCal;
	DTCSnap[num].IgnState = 0xff;
}


/*******************************************************************************
 * Function:  void GetSystemData(uint8* pSystemData)
 * 
 * Description: ��ȡϵͳ״̬�� 22����� FDFF ��DID ��ȡ��
 *
 * Returns:    none
 *
 * Notes:    ��uds �ɵ��ô˺���     ������λ����һ�����������        
 *                         
 *                          
*******************************************************************************/
#if _PROJECT_CAN_
void GetSystemData(uint8* pSystemData)
{
	//TODO: �궨���棬��ramdata.csv ��д�� �ɲο�ģ��
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
 * Description: ��ȡϵͳ״̬�� 22����� IODID_SYSTEMPARMETER  0xFDDE��DID ��ȡ��
 *
 * Returns:    none
 *
 * Notes:    ��uds �ɵ��ô˺���     ������λ����2�����������        
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
	{PDATA_NULL, 1},   //���һ�����
};
void GetParameterData(uint8* rtx)
{
	//TODO: ���ر궨�������ο�ģ��
	int16 iNum;
	uint8* pDercData = rtx;
	for (iNum =0 ; TBL_SetParameter[iNum].pData != PDATA_NULL; iNum++)
	{
		MemCopy(pDercData, TBL_SetParameter[iNum].pData,  TBL_SetParameter[iNum].u8Length);
		pDercData += TBL_SetParameter[iNum].u8Length; 
	}
}

//���ò������ݼ���
BOOL  SetParameterData(uint8* rtx)
{
	//TODO: ���ñ궨�������ο�ģ��
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
 * Description: ��ȡϵͳ״̬�� 22����� IODID_SYSTEMPARMETER  0xFDDE��DID ��ȡ��
 *
 * Returns:    none
 *
 * Notes:    ��uds �ɵ��ô˺���     ������λ����2�����������        
 *                         
 *                          
*******************************************************************************/
void GetParameterData_Sec(uint8* rtx)
{
	//TODO: �õ��궨����2���ο�ģ��
}
//���ò������ݼ���
BOOL  SetParameterData_Sec(uint8* rtx)
{
	//TODO: ���ñ궨����2���ο�ģ��
	Save_Parameter_Sec(rtx);
	return TRUE;
}

/*******************************************************************************
 * Function:  void SetSetData(uint8* rtx)
 * 
 * Description: ��ȡϵͳ״̬�� 22����� DID_SYSTMESET ʵ��
 *
 * Returns:    none
 *
 * Notes:    ��uds �ɵ��ô˺���     ������λ���ĵ��Խ��������        
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
 * Description: ���ô������Ĺ���
 *
 * Returns:    none
 *
 * Notes:    ��lowerdrive���� 
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
 * Description: PC ����
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

