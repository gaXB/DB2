/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�PTCPower_PID.c
* ������������ϵ��PID
* ������¶�PID ����,���ø���
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��11��21��
* 
*/

#include "TempFilter.h"
#include "PTCPower_PID.h"
#include "APPDrive.h"



void PTCPowerPID_GetState(void);
PIDDATA   sPTCPowerPidData;
PIDPARAMETER  sPTCPowerPar = {50, 50, 50, 0, 500, 10000, 0,5, 300, 200, 5000, PTCPowerPID_GetState};
PIDPARAMETER  sBatPTCPowerPar = {50, 50, 50, 0, 500, 10000, 0,5, 300, 200, 5000, PTCPowerPID_GetState};
static BOOL bBathEn;


/*******************************************************************************
 * Function:      int16 Deal_PTCPowerPID(void)
 *
 * Description:    PID ����
 *
 * Returns:         none
 *
 * Notes:           
 *
********************************************************************************/
int16 Deal_PTCPowerPID(void)
{
	if (sPTCPowerPidData.u8RunState == PIDRUN_STATE_INIT && sPTCPowerPidData.i16Get > sPTCPowerPidData.i16Set)
	{//����ʱ �¶��Ѿ�����Ŀ��ֵ��ֱ�ӽ�������
		sPTCPowerPidData.u8RunState = PIDRUN_STATE_RUN;
	}
	else{}

	if (bBathEn)
	{
		return Deal_PIDFun(&sPTCPowerPidData, &sPTCPowerPar);
	}
	else
	{
		return Deal_PIDFun(&sPTCPowerPidData, &sBatPTCPowerPar);
	}
}

void PTCPowerPID_GetState(void)
{
	//sPTCPowerPidData.i16Get = sSenSorData.Tadt;
	//sPTCPowerPidData.i16Set = sSenSorData.Tddt;
}

/*******************************************************************************
 * Function:        void HmixPID_Init(void)
 *
 * Description:    ���PID iniit
 *
 * Returns:         none
 *
 * Notes:           �õ� 
 *
********************************************************************************/	
int16 PTCPowerPID_Init(void)
{
	//��Ͽ��� ��ʼ��
	(void)TimeOutChkTenthSLong(&sPTCPowerPidData.Timer01, 0);
	return 1;//GAMath_LineFuni16(TSET_LO, TSET_HI, 0, 1000, SystemControl.tset);
}


void  PTCPowerPIDMode(uint8 bBathMode)
{
	bBathEn = bBathMode;
}
