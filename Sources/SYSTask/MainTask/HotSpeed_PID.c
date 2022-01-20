/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�HotSpeed_PID.c
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
#include "HotSpeed_PID.h"
#include "APPDrive.h"



void HotSpeedPID_GetState(void);
PIDDATA   sHotSpeedPidData;
PIDPARAMETER  sHotSpeedPar = {50, 50, 50, 0, 500, 10000, 0,5, 300, 800, 10000, HotSpeedPID_GetState};
PIDPARAMETER  sBatHotSpeedPar = {50, 50, 50, 0, 500, 10000, 0,5, 300, 800, 10000, HotSpeedPID_GetState};

static BOOL bBathEn;

/*******************************************************************************
 * Function:      int16 Deal_HotSpeedPID(void)
 *
 * Description:    PID ����
 *
 * Returns:         none
 *
 * Notes:           
 *
********************************************************************************/
int16 Deal_HotSpeedPID(void)
{
	if (sHotSpeedPidData.u8RunState == PIDRUN_STATE_INIT && sHotSpeedPidData.i16Get > sHotSpeedPidData.i16Set)
	{//����ʱ �¶��Ѿ�����Ŀ��ֵ��ֱ�ӽ�������
		sHotSpeedPidData.u8RunState = PIDRUN_STATE_RUN;
	}
	else{}

	if (bBathEn)
	{
		return Deal_PIDFun(&sHotSpeedPidData, &sBatHotSpeedPar);
	}
	else
	{
		return Deal_PIDFun(&sHotSpeedPidData, &sHotSpeedPar);
	}
}

void HotSpeedPID_GetState(void)
{
	//sHotSpeedPidData.i16Get = sSenSorData.Tadt;
	//sHotSpeedPidData.i16Set = sSenSorData.Tddt;
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
int16 HotSpeedPID_Init(void)
{
	//��Ͽ��� ��ʼ��
	(void)TimeOutChkTenthSLong(&sHotSpeedPidData.Timer01, 0);
	return 1;//GAMath_LineFuni16(TSET_LO, TSET_HI, 0, 1000, SystemControl.tset);
}


void HotSpeedPIDMode(uint8 bBathMode)
{
	bBathEn = bBathMode;
}
