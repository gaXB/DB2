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
		case DTCNUM_TIN:
			u8DTCResult = DTC_RETSTATE_ERR;
			break;
		case  DTCNUM_TAMB:
			if (ADPhy_GetErr(ADID_TAMB))
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
			break;
		case DTCNUM_TEVP:
			if (ADPhy_GetErr(ADID_TEVP))
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
			break;
		//md motor
		case DTCNUM_MDMOTOR_BACKERR:
			if ((Motor_GetErrorCode(MOTOR_ID_MODE) & MOTOR_ERROR_BACKERR) != 0)
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
			break;
//		case DTCNUM_MDMOTOR_JAMMED:
//			//return (Motor_GetErrorCode(MOTOR_ID_MODE) & MOTOR_ERROR_JAMMED);
//			u8DTCResult = DTC_RETSTATE_OK;
//			break;
//		//mix motor
//		case DTCNUM_MIXMOTOR_JAMMED:
//			u8DTCResult = DTC_RETSTATE_OK;
//			break;
		case DTCNUM_MIXMOTOR_BACKERR:
			if ((Motor_GetErrorCode(MOTOR_ID_MIX) & MOTOR_ERROR_BACKERR) != 0)
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
			break;
//		case DTCNUM_NCFMOTOR_JAMMED:
//			//return (Motor_GetErrorCode(MOTOR_ID_NCF) & MOTOR_ERROR_JAMMED);
//			u8DTCResult = DTC_RETSTATE_OK;
//			break;
		case DTCNUM_NCFMOTOR_BACKERR:
			if ((Motor_GetErrorCode(MOTOR_ID_NCF) & MOTOR_ERROR_BACKERR) != 0)
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
			break;
//		case DTCNUM_FAN_BACKERR://�ķ�� short or open
//			if (ADValue_Get(ADID_FAN, ADVALUE_GETTYPE_8AD) < 25)
//			{
//				u8DTCResult = DTC_RETSTATE_ERR;
//			}
//			else
//			{
//				u8DTCResult = DTC_RETSTATE_OK;
//			}
//			break;
		case  DTCNUM_PT1TEMP:
			if((ADValue_Get(ADID_TPTL1, ADVALUE_GETTYPE_10AD) < 10)  || (ADValue_Get(ADID_TPTL1, ADVALUE_GETTYPE_10AD)) > 990)
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
			break;
		case  DTCNUM_PT2TEMP:
			if((ADValue_Get(ADID_TPTL2, ADVALUE_GETTYPE_10AD) < 10)  || (ADValue_Get(ADID_TPTL2, ADVALUE_GETTYPE_10AD)) > 990)
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
			break;
		case  DTCNUM_PTHTEMP:
			if((ADValue_Get(ADID_TPTH, ADVALUE_GETTYPE_10AD) < 10)  || (ADValue_Get(ADID_TPTH, ADVALUE_GETTYPE_10AD)) > 990)
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
			break;
		case  DTCNUM_PT1PRESS:
			if((ADValue_Get(ADID_PPTL1, ADVALUE_GETTYPE_10AD) < 10)  || (ADValue_Get(ADID_PPTL1, ADVALUE_GETTYPE_10AD)) > 990)
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
			break;
		case  DTCNUM_PT2PRESS:
			if((ADValue_Get(ADID_PPTL2, ADVALUE_GETTYPE_10AD) < 10)  || (ADValue_Get(ADID_PPTL2, ADVALUE_GETTYPE_10AD)) > 990)
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
			break;
		case  DTCNUM_PTHPRESS:
			if((ADValue_Get(ADID_PPTH, ADVALUE_GETTYPE_10AD) < 10)  || (ADValue_Get(ADID_PPTH, ADVALUE_GETTYPE_10AD)) > 990)
			{
				u8DTCResult = DTC_RETSTATE_ERR;
			}
			else
			{
				u8DTCResult = DTC_RETSTATE_OK;
			}
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

#define  SYSTEMDATA_SET_16(i16Data)  	pSystemData[u8RxNum++] = (uint8)(i16Data & 0x00ff);\
	pSystemData[u8RxNum++] = (uint8)(i16Data >> 8) ;

#define  SYSTEMDATA_SET_8(i16Data)  	pSystemData[u8RxNum++] = (uint8)(i16Data & 0x00ff);
void GetSystemData(uint8* pSystemData)
{
	//TODO: �궨���棬��ramdata.csv ��д�� �ɲο�ģ��
#if _PROJECT_AUTO_  
	//TDDATA lTDData;
	//AUTORESULT lAutoResult;
	uint8 u8RxNum;
	
	//lTDData = AUTOTD_GetTD();
	//lAutoResult = AUTOTD_GetARes();

	pSystemData[0] = (uint8)SystemControl.u8TestLevel;
	pSystemData[1] = (uint8)SystemControl.systemMode;
	
	
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

	pSystemData[u8RxNum++]=	(uint8)sEVSensorData.TevpTarge;
	pSystemData[u8RxNum++]=	(uint8)(sEVSensorData.TevpTarge >> 8);
   
	pSystemData[u8RxNum++]=	(uint8)sSenSorData.TwaterCal;	
	pSystemData[u8RxNum++]=	(uint8)(sSenSorData.TwaterCal >> 8);
	
	pSystemData[u8RxNum++]=	(uint8)sEVSensorData.TwaterTarge;
	pSystemData[u8RxNum++]=	(uint8)(sEVSensorData.TwaterTarge >> 8);

	pSystemData[u8RxNum++] = (uint8)sSenSorData.u8VehicleSpeed;
	
	pSystemData[u8RxNum++] = (uint8)sEVSensorData.TPL1_Press;
	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.TPL1_Press >>8);

	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.TPL1_Temp & 0x00ff);
	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.TPL1_Temp >> 8) ;

	pSystemData[u8RxNum++] = (uint8)sEVSensorData.TPL2_Press;
	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.TPL2_Press >>8);

	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.TPL2_Temp & 0x00ff);
	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.TPL2_Temp >> 8) ;

	pSystemData[u8RxNum++] = (uint8)sEVSensorData.TPH_Press;
	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.TPH_Press >>8);
	
	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.TPH_Temp & 0x00ff);
	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.TPH_Temp >> 8) ;

	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.BattInWater & 0x00ff);
	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.BattInWater >> 8) ;

	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.BattOutWater & 0x00ff);
	pSystemData[u8RxNum++] = (uint8)(sEVSensorData.BattOutWater >> 8) ;
	

	SYSTEMDATA_SET_16(sEVSensorData.BattTargetWater);
	SYSTEMDATA_SET_16(sEVSensorData.BattTemp);

	pSystemData[u8RxNum++] = (uint8) (sOutData.FANVolt / 10);
	pSystemData[u8RxNum++] = FanControl_GetCurrentLevel();	
	
	pSystemData[u8RxNum++] = sOutData.ACOut;
	pSystemData[u8RxNum++] = Motor_GetPosition(MOTOR_ID_MIX);

	pSystemData[u8RxNum++] = Motor_GetSetPosition(MOTOR_ID_MIX) ;

	pSystemData[u8RxNum++] = Motor_GetPosition(MOTOR_ID_MODE);

	pSystemData[u8RxNum++] = Motor_GetSetPosition(MOTOR_ID_MODE);

	pSystemData[u8RxNum++] = Motor_GetPosition(MOTOR_ID_NCF);

	pSystemData[u8RxNum++] = Motor_GetSetPosition(MOTOR_ID_NCF) ;

	SYSTEMDATA_SET_16(sEVOutData.EACSpeed);
	SYSTEMDATA_SET_16(sEVOutData.PTCPower);
	SYSTEMDATA_SET_16(sVehicleControl.i16PowerLimte);

	pSystemData[u8RxNum++] = (uint8)sEVOutData.ThreeWay1_Postion;
	pSystemData[u8RxNum++] = (uint8)sEVOutData.ThreeWay2_Postion;
	pSystemData[u8RxNum++] = (uint8)sEVOutData.FourWay_Postion;

	SYSTEMDATA_SET_16(sEVOutData.EXV_Postion);
	SYSTEMDATA_SET_16(sEVOutData.BEXV_Postion);
	SYSTEMDATA_SET_16(sEVOutData.AGS_Postion);
	SYSTEMDATA_SET_8(sEVOutData.SOV1);
	SYSTEMDATA_SET_8(sEVOutData.SOV2);
	SYSTEMDATA_SET_8(sEVOutData.EWPT1);
	SYSTEMDATA_SET_8(sEVOutData.EWPT2);
	SYSTEMDATA_SET_8(sEVOutData.EWPT3);
	SYSTEMDATA_SET_8(sEVOutData.IncarFAN_H);
	SYSTEMDATA_SET_8(sEVOutData.IncarFAN_L);
	extern uint16 EACStopReason;
	SYSTEMDATA_SET_16(EACStopReason);

	if (u8RxNum > CAILBRATION_PARAMETER_NUM)
	{
		asm("BKPT");
	}else{}
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
uint8 TBL_LEVEL[24] = {0 ,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

typedef struct
{
	void*  pData;
	uint8   u8Length;
}PC_PARAMETER_TYPE;
PC_PARAMETER_TYPE TBL_SetParameter[]=
{
#if _PROJECT_AUTO_
	{&TBL_COOLInitSpeed, 6},
	{&TBL_LEVEL[4], 3},
	{&i16SignalCarMaxSpeed, 2},
	{&i16EXVCharge, 2},
	{&i16EXVDrive, 2},
	{&u8EXVAdjStep, 1},
	{&u8EXVAdjTime, 1},
	{&i16EXVMin, 2},
	{&u8Bat3WayAdjStep, 1},
	{&Adj3wayTime, 2},
	{&i163Way2Min, 2},
	{&i16BatCInitSpeed, 2},
	{&i16BothCoolMaxSpeed, 2},
	{&tACOFF_EVP, 2},
	{&tACON_EVP, 2},
	{TBL_CoolDefInitSpeed, 8},
	{TBL_HotDefInitSpeed, 6},
	{TBL_HotDefInitPTC, 6},
	{&TBL_LEVEL[8], 7},
	{TBL_CoolDefMix, 8},
	{TBL_HotDefMix, 6},
	{TBL_HPInitSpeed, 6},
	{&TBL_LEVEL[16], 3},
	{&i16BatPTC_InitPower, 2},
	{&i16BatPTC_MaxPower, 2},
	{&TBLWPTC_InitPower[0], 6},
	{&TBL_LEVEL[20], 3},
	{&Parameter_A, 2},
	{&Parameter_B, 2},
	{&Parameter_C, 2},
	{&Parameter_D, 2},
	{&i16BEXVCool, 2},
	{&TBL_BEXV_HOTY, 4},
	{&TBL_BEXV_HOTX, 4},
	{&i16SignalCarMaxSpeed_HP, 2},
	{&i16BatHInitPSpeed, 2},
	{&i16BatHMaxSpeed, 2},
	{&i16BothMaxSpeed_HP, 2},
	{&HPreePro_T1, 2},
	{&TBL_AFANVOLT[1], 14},
	{TBL_MODE_OSET, 5},
	{&u8MIXMOTOR_HI_AD, 1},
	{&u8MIXMOTOR_LO_AD, 1},
	{&TBL_NCFAD[1], 1},
	{&TBL_NCFAD[0], 1},
#endif
	{PDATA_NULL, 1},   //���һ�����
};
void GetParameterData(uint8* rtx)
{
	//TODO: ���ر궨�������ο�ģ��
	int16 iNum;
	uint16 u8SendLength = 0;
	uint8* pDercData = rtx;
	for (iNum =0 ; TBL_SetParameter[iNum].pData != PDATA_NULL; iNum++)
	{
		MemCopy(pDercData, TBL_SetParameter[iNum].pData,  TBL_SetParameter[iNum].u8Length);
		pDercData += TBL_SetParameter[iNum].u8Length; 
		u8SendLength += TBL_SetParameter[iNum].u8Length;
	}

	if (u8SendLength > CAILBRATION_PARAMETER_NUM)
	{
		asm("BKPT");
	}else{}
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
PC_PARAMETER_TYPE TBL_SetParameter_Sec[]=
{
#if _PROJECT_AUTO_
	{&sCoolSpeedPar.u16Period, 2},
	{&sCoolSpeedPar.i16Kp, 2},
	{&sCoolSpeedPar.i16PMax, 2},
	{&sCoolSpeedPar.i16IMax, 2},
	{&sCoolSpeedPar.i16Ki, 2},
	{&sBatCoolSpeedPar.i16Kp, 2},
	{&sBatCoolSpeedPar.i16PMax, 2},
	{&sBatCoolSpeedPar.i16IMax, 2},
	{&sBatCoolSpeedPar.i16Ki, 2},
	//
	{&sHotSpeedPar.u16Period, 2},
	{&sHotSpeedPar.i16Kp, 2},
	{&sHotSpeedPar.i16PMax, 2},
	{&sHotSpeedPar.i16IMax, 2},
	{&sHotSpeedPar.i16Ki, 2},
	{&sBatHotSpeedPar.i16Kp, 2},
	{&sBatHotSpeedPar.i16PMax, 2},
	{&sBatHotSpeedPar.i16IMax, 2},
	{&sBatHotSpeedPar.i16Ki, 2},
	//
	{&sPTCPowerPar.u16Period, 2},
	{&sPTCPowerPar.i16Kp, 2},
	{&sPTCPowerPar.i16PMax, 2},
	{&sPTCPowerPar.i16IMax, 2},
	{&sPTCPowerPar.i16Ki, 2},
	{&sBatPTCPowerPar.i16Kp, 2},
	{&sBatPTCPowerPar.i16PMax, 2},
	{&sBatPTCPowerPar.i16IMax, 2},
	{&sBatPTCPowerPar.i16Ki, 2},
#endif
	{PDATA_NULL, 1},   //���һ�����
};
void GetParameterData_Sec(uint8* rtx)
{
	//TODO: �õ��궨����2���ο�ģ��
	int16 iNum;
	uint16 u8SendLength = 0;
	uint8* pDercData = rtx;
	for (iNum =0 ; TBL_SetParameter_Sec[iNum].pData != PDATA_NULL; iNum++)
	{
		MemCopy(pDercData, TBL_SetParameter_Sec[iNum].pData,  TBL_SetParameter_Sec[iNum].u8Length);
		pDercData += TBL_SetParameter_Sec[iNum].u8Length;
		u8SendLength += TBL_SetParameter_Sec[iNum].u8Length;
	}

	if (u8SendLength > CAILBRATION_PARAMETER_NUM)
	{
		asm("BKPT");
	}else{}
}
//���ò������ݼ���
BOOL  SetParameterData_Sec(uint8* rtx)
{
	//TODO: ���ñ궨����2���ο�ģ��
	int16 iNum;
	uint8* pSrcData = rtx;
	for (iNum =0 ; TBL_SetParameter_Sec[iNum].pData != PDATA_NULL; iNum++)
	{
		MemCopy(TBL_SetParameter_Sec[iNum].pData, pSrcData, TBL_SetParameter_Sec[iNum].u8Length);
		pSrcData += TBL_SetParameter_Sec[iNum].u8Length;
	}
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
	SystemControl.u8TestLevel = (rtx[0]);
	SystemControl.acMode = (ACModeType)rtx[1];
	SystemControl.ncfMode = (NcfModeType)rtx[2];
	SystemControl.mdMode = (MDModeType)rtx[3];
	SystemControl.fanMode = rtx[4];

	SystemControl.RdefMode = (RdefModeType)rtx[5];
	SystemControl.ptcMode = rtx[6];

	sOutData_PC.ACOut = rtx[7];
	sOutData_PC.FANVolt = (rtx[8]) + (rtx[9]<<8);
	sOutData_PC.MIXAD = rtx[10];
	sOutData_PC.MODEAD = rtx[11];
	sOutData_PC.NCFAD = rtx[12];
	sOutData_PC.RHeatOut = rtx[13];


	MemCopy((uint8*)&sSenSorData_PC, &rtx[14],  9);
	MemCopy((uint8*)&sEVSensorData_PC, &rtx[23],  34);
	
	
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
	rtx[0] = SystemControl.u8TestLevel;
	rtx[1] = SystemControl.acMode;
	rtx[2] = SystemControl.ncfMode;
	rtx[3] = SystemControl.mdMode;
	rtx[4] = SystemControl.fanMode;
	rtx[5] = SystemControl.RdefMode;
	rtx[6] = SystemControl.ptcMode;
	rtx[7] = sOutData.ACOut;
	rtx[8] = (uint8)sOutData.FANVolt;
	rtx[9] = (uint8)(sOutData.FANVolt >>8);
	rtx[10] = sOutData.MIXAD;
	rtx[11] = sOutData.MODEAD;
	rtx[12] = sOutData.NCFAD;
	rtx[13] = sOutData.RHeatOut;
	
	MemCopy(&rtx[14], (uint8*)&sSenSorData, 9);
	MemCopy(&rtx[23], (uint8*)&sEVSensorData, 34);

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
	if (PCControlEn.Bits.b13) //������ +1
	{
		sSenSorData.TambCal = sSenSorData_PC.TambCal;
	}
	
	if (PCControlEn.Bits.b14)
	{
		sSenSorData.TinCal = sSenSorData_PC.TinCal;
	}
	
	if (PCControlEn.Bits.b15)
	{
		sSenSorData.TevpCal = sSenSorData_PC.TevpCal;
	}
	
	if (PCControlEn.Bits.b16)
	{
		sSenSorData.TwaterCal = sSenSorData_PC.TwaterCal;
	}
	
	if (PCControlEn.Bits.b17)
	{
		sSenSorData.u8VehicleSpeed = sSenSorData_PC.u8VehicleSpeed;
	}
	
	uint8 bitNum;
	uint32 maskData;
	uint8 ByteNum;
	int16* pData = (int16*)&sEVSensorData;
	int16* pData_PC = (int16*)&sEVSensorData_PC;
	for (bitNum = 18; bitNum <=34; bitNum++)
	{
		ByteNum = (bitNum)/8;
		maskData = 1;
		maskData <<= bitNum -ByteNum*8;
		if (PCControlEn.Data[ByteNum] & maskData)
		{
			*pData = *pData_PC;
		}
		pData++;
		pData_PC++;
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
	if (PCControlEn.Bits.b7)
	{
		sOutData.ACOut = sOutData_PC.ACOut;
	}else{}
	
	if (PCControlEn.Bits.b8)
	{
		sOutData.FANVolt = sOutData_PC.FANVolt;
	}else{}
	
	if (PCControlEn.Bits.b9)
	{
		sOutData.MIXAD = sOutData_PC.MIXAD;
	}else{}
	
	if (PCControlEn.Bits.b10)
	{
		sOutData.MODEAD = sOutData_PC.MODEAD;
	}else{}
	
	if (PCControlEn.Bits.b11)
	{
		sOutData.NCFAD = sOutData_PC.NCFAD;
	}else{}
	
	if (PCControlEn.Bits.b12)
	{
		sOutData.RHeatOut = sOutData_PC.RHeatOut;
	}else{}

	extern uint8  u8MIXMOTOR_HI_AD, u8MIXMOTOR_LO_AD;
	extern uint8 TBL_NCFAD[2];
	if (SystemControl.systemMode == SYSTEM_MODE_DEBUG)
	{
		SystemControl.mdMode = sCANMsg451.Bits.TMU_HVAC_MODE_1*4 + sCANMsg451.Bits.TMU_HVAC_MODE_2;
		sOutData.FANVolt = sCANMsg451.Bits.TMU_HVAC_BLW_V*100;
		sOutData.NCFAD = (sCANMsg451.Bits.TMU_HVAC_RF_1 << 3) + sCANMsg451.Bits.TMU_HVAC_RF_2;
		sOutData.NCFAD = 		sOutData.NCFAD * (TBL_NCFAD[1] - TBL_NCFAD[0])/20 + TBL_NCFAD[0] ;

		sOutData.MIXAD = (sCANMsg451.Bits.TMU_HVAC_AM_1 << 3) + sCANMsg451.Bits.TMU_HVAC_AM_2;
		sOutData.MIXAD = sOutData.MIXAD *(u8MIXMOTOR_HI_AD - u8MIXMOTOR_LO_AD) /20 + u8MIXMOTOR_LO_AD;

		sEVOutData.PTCPower =(sCANMsg451.Bits.TMU_WPTC_HTR_OUTPUT_1 <<3) +  sCANMsg451.Bits.TMU_WPTC_HTR_OUTPUT_2;
		sEVOutData.PTCPower = sEVOutData.PTCPower*50;
		sEVOutData.EACSpeed = (sCANMsg451.Bits.TMU_ECOMPR_RPM_1 <<3) +  sCANMsg451.Bits.TMU_ECOMPR_RPM_2;
		sEVOutData.EACSpeed = sEVOutData.EACSpeed*500;
		if (sEVOutData.PTCPower > 0)
		{
			sEVOutData.ThemalSysRequest = 1;
			sEVOutData.HeatConfirmation = 1;
		}
		else		if (sEVOutData.EACSpeed > 0)
		{
			sEVOutData.ThemalSysRequest = 1;
			sEVOutData.CoolConfirmation = 1;
		}
		else
		{
			sEVOutData.ThemalSysRequest = 0;
			sEVOutData.CoolConfirmation = 0;
			sEVOutData.HeatConfirmation = 0;
		}
		sEVOutData.SOV1 = sCANMsg451.Bits.TMU_SOV1_OPEN_CLOSE;
		sEVOutData.SOV2 = sCANMsg451.Bits.TMU_SOV2_OPEN_CLOSE;
		sEVOutData.AGS_Postion = sCANMsg451.Bits.TMU_AGS_POSITION*100/15;
		sEVOutData.EXV_Postion = (sCANMsg452.Bits.TMU_EXV_PULSE_NUM_1 <<7) + sCANMsg452.Bits.TMU_EXV_PULSE_NUM_2;
		sEVOutData.BEXV_Postion = (sCANMsg452.Bits.TMU_FBEV_PULSE_NUM_1 <<5) + sCANMsg452.Bits.TMU_FBEV_PULSE_NUM_2;
		if (sCANMsg452.Bits.TMU_RAD_FAN_LV == 2)
		{
			sEVOutData.IncarFAN_H = 1;
			sEVOutData.IncarFAN_L = 0;
		}
		else 	if (sCANMsg452.Bits.TMU_RAD_FAN_LV == 1)
		{
			sEVOutData.IncarFAN_H = 0;
			sEVOutData.IncarFAN_L = 1;
		}
		else
		{
			sEVOutData.IncarFAN_H = 0;
			sEVOutData.IncarFAN_L = 0;
		}

		sEVOutData.FourWay_Postion =  sCANMsg452.Bits.TMU_4WAY_POSITION;
		sEVOutData.ThreeWay1_Postion =  sCANMsg452.Bits.TMU_3WAY1_POSITION;
		sEVOutData.ThreeWay2_Postion =  sCANMsg452.Bits.TMU_3WAY2_POSITION;
		sEVOutData.EWPT1 =  sCANMsg452.Bits.TMU_DUTY_MOTOR_PUMP;
		sEVOutData.EWPT2 =  sCANMsg452.Bits.TMU_DUTY_BATT_PUMP;
		sEVOutData.EWPT3 =  sCANMsg452.Bits.TMU_DUTY_WCPTC_PUMP;
	}
}
#endif

