/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�MIXVENT_PID.c
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
#include "MIXVENT_PID.h"
#include "APPDrive.h"



void MIXVENTPID_GetState(void);
PIDDATA_INC sMIXVentPidData;
PIDPARAMETER_INC sMIXVentPar = {50, 45, 20, 0, -100, 100, MIXVENTPID_GetState};



/*******************************************************************************
 * Function:      uint8 Deal_MIXVENTPID(void)
 *
 * Description:    PID ����
 *
 * Returns:         none
 *
 * Notes:           
 *
********************************************************************************/
int16 Deal_MIXVENTPID(void)
{
	return Deal_PIDFun_INC(&sMIXVentPidData, &sMIXVentPar);
}

void MIXVENTPID_GetState(void)
{
	sMIXVentPidData.i16Get = sSenSorData.Tadt;
	sMIXVentPidData.i16Set = sSenSorData.Tddt;
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
int16 HmixPID_Init(void)
{
	//��Ͽ��� ��ʼ��
	(void)TimeOutChkTenthSLong(&sMIXVentPidData.Timer01, 0);
	return GAMath_LineFuni16(TSET_LO, TSET_HI, 0, 1000, SystemControl.tset);
}
