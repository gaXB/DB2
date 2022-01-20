/************************�ļ�˵��*****************************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�PowerOff_Task.c
* ������������ԴOFFʱ��ϵͳ������
* �ṩģ�� ����ʵ��Ӧ�ñ�д
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��19��
* 
* �汾��1.1
* ˵����ö�����͸�ֵ�Ż� 
* ���ߣ�xb
* ������ڣ�2021��8��23��
*/

#include "PowerMange.h"
#include "APPDrive.h"



static uint16 TimerPowerDelay;
static uint8 bDelayEn;    //bDelayEn ����Ӧ���Ѿ������������������


/*******************************************************************************
 * Function:        void PowerOffTask(void)
 *
 * Description:     ��Դ����ʱ�Ĺ�������
 *
 * Returns:         none
 *
 * Notes:           ��ѭ������
 *
********************************************************************************/
void PowerOffTask(void)
{
	//����ѹ����
	//TODO: �ڴ˱�д��ԴOFFʱ��Ĵ���
	Clear_LCD();
	sDisplayData.AC = 0;
	sDisplayData.AUTO = 0;
	sDisplayData.CIRF = 0;
	sDisplayData.FANLevel = 0;
	sDisplayData.LCDDiagMode = 0;
	sDisplayData.LCDMode = 0;
	sDisplayData.LCheat = 0;
	sDisplayData.LEDMode = 0;
	sDisplayData.MAXAC = 0;
	sDisplayData.MODE_MAXDEF = 0;
	sDisplayData.NEW = 0;
	sDisplayData.RHEAT = 0;
	sDisplayData.RCheat = 0;
	sDisplayData.VentMode = MD_MODE_OSF;
	sDisplayData.i16Temp = 0;
	
	sOutData.ACOut = 0;
	sOutData.FANVolt = 0;
	//sOutData.MIXAD
	//sOutData.MODEAD
	//sOutData.NCFAD
	sOutData.RHeatOut = 0;
	sOutData.u8LCheat = 0;
	sOutData.u8RCheat = 0;

	sEVOutData.AGS_Postion = 0;
	sEVOutData.SOV1 = 0;
	sEVOutData.SOV2 = 0;
	sEVOutData.EWPT1 = 0;
	sEVOutData.EWPT2 = 0;
	sEVOutData.EWPT3 = 0;
	sEVOutData.EACSpeed = 0;
	sEVOutData.PTCPower = 0;
	sEVOutData.IncarFAN_L = 0;
	sEVOutData.IncarFAN_H = 0;
	sEVOutData.ThemalSysRequest = 0;
	sEVOutData.HeatConfirmation = 0;
	sEVOutData.CoolConfirmation = 0;
	sEVOutData.TMU_Realay = 0;
	//(void)NcfMotor_SetPosition(0,NCF_MODE_CIRF);
	//NcfMotor_Control();

   //	LinControl();
	
	if (TimeOutChkMsLong(&TimerPowerDelay, 4000))
	{
		bDelayEn = 1;
	}
	if ( bDelayEn)//NcfMotor_GetState(0) == NCFMOTOR_STATE_HALT &&
	{	
		PowerMN_AppSleepEn(1);
	}
	else
	{}  //��������
}

/*******************************************************************************
 * Function:        void PowerOffTask_Enter(void)
 *
 * Description:     ����poweroff ģʽ
 *
 * Returns:         none
 *
 * Notes:           ����ʱ��pm_app�ⲿ����
 *
********************************************************************************/
void PowerOffTask_Enter(void)
{
	(void)TimeOutChkMsLong(&TimerPowerDelay, 0);
	bDelayEn = 0;
	PowerMN_AppSleepEn(0);
}
