/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CoolSpeed_PID.c
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
#include "CoolSpeed_PID.h"
#include "APPDrive.h"



void CoolSpeedPID_GetState(void);
PIDDATA   sCoolSpeedPidData;
PIDPARAMETER  sCoolSpeedPar = {50, -50, -50, 0, 500, 10000, 0,5, 300, 800, 10000, CoolSpeedPID_GetState};
PIDPARAMETER  sBatCoolSpeedPar = {50, -50, -50, 0, 500, 10000, 0,5, 300, 800, 10000, CoolSpeedPID_GetState};

static BOOL bBathParameter;

/*******************************************************************************
 * Function:      int16 Deal_CoolSpeedPID(void)
 *
 * Description:    PID ����
 *
 * Returns:         none
 *
 * Notes:           
 *
********************************************************************************/
int16 Deal_CoolSpeedPID(void)
{
	sCoolSpeedPidData.i16OutInitValue = 3;
	if (sCoolSpeedPidData.u8RunState == PIDRUN_STATE_INIT && sCoolSpeedPidData.i16Get < sCoolSpeedPidData.i16Set)
	{//����ʱ �¶��Ѿ�����Ŀ��ֵ��ֱ�ӽ�������
		sCoolSpeedPidData.u8RunState = PIDRUN_STATE_RUN;
	}
	else{}
	if (bBathParameter)
	{
		return Deal_PIDFun(&sCoolSpeedPidData, &sBatCoolSpeedPar);
	}
	else
	{
		return Deal_PIDFun(&sCoolSpeedPidData, &sCoolSpeedPar);
	}
}

void CoolSpeedPID_GetState(void)
{
	//sCoolSpeedPidData.i16Get = sSenSorData.Tadt;
	//sCoolSpeedPidData.i16Set = sSenSorData.Tddt;
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
int16 CoolSpeedPID_Init(void)
{
	//��Ͽ��� ��ʼ��
	(void)TimeOutChkTenthSLong(&sCoolSpeedPidData.Timer01, 0);
	return 1;//GAMath_LineFuni16(TSET_LO, TSET_HI, 0, 1000, SystemControl.tset);
}

//int16 PIDCool_Revis(int16 CurrentData)
//{
//	return PID_Resvis(&sCoolSpeedPidData, CurrentData);
//}

void CoolSpeedPIDMode(uint8 bBathMode)
{
	bBathParameter = bBathMode;
}
