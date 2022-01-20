/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ICDOut.c
* ���������� ͨ��Ӳ������� OutTask��ʵ��
* ����Ҫʵ��
* 1 TrOut_5VEN 
* 2 OUT_AllClose/OUT_AllOpen
* �������� codewarrior IDE 11.1
* MCU˵������MCU �޹�
*  
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��13��
* 
*  �汾��1.1
* ˵�����������0xffΪͣ��
* ԭ���ߣ�xb
* ������ڣ�2020��10��21��
*/
#include "ICDOut.h"




/*******************************************************************************
 * Function: void TrOut_5VEN(uint8 en)
 *
 * Description: �����Դ����
 *
 * Returns:     none
 *
 * Notes:    �˺�������ɾ��,O_5VEN Ϊ�̶��궨��
 *
*******************************************************************************/
void TrOut_5VEN(uint8 en)
{
	if (en)
	{
		O_5VEN = GPIO_SET;
		O_12V_EN = 1;
		O_TMU = 1;

	}
	else
	{
		O_5VEN = GPIO_CLEAR;
		O_12V_EN =0;
		O_TMU = 0;
	}
}


/*******************************************************************************
 * Function: void	Out_AllClose(void)
 *
 * Description: ��������ر�
 *
 * Returns:     none
 *
 * Notes:
 *
*******************************************************************************/
void	OUT_AllClose(void)
{
	//TODO: ��д��������ر�,�������ܱ���
	OUTFAN_PWM = GPIO_CLEAR;
	O_FAN_RELAY = GPIO_CLEAR;
}

/*******************************************************************************
 * Function: void	Out_AllOpen(void)
 *
 * Description: �����������,�Լ����ʹ��
 *
 * Returns:     none
 *
 * Notes:    ����5ven
 *
*******************************************************************************/
void	OUT_AllOpen(void)
{
	//TODO: ��д�����������,�������ܱ���
	OUTFAN_PWM = PWMMODE_FAN/2;
	O_FAN_RELAY = GPIO_SET;
}


/*******************************************************************************
 * Function: void	OutTask(void)
 *
 * Description: ���������
 *
 * Returns:     none
 *
 * Notes:
 *
*******************************************************************************/
void OutTask(void)
{
	//TODO: icd ����ڴ�ʵ��,�����͵�����Ʊ����������ɼ�ģ��
	(void)Motor_SetPosition(MOTOR_ID_MODE, sOutData.MODEAD);//��ֱ�����ã���λ0xffʱΪֹͣ��ע����ģ��Ϊ1.1����
	(void)Motor_SetPosition(MOTOR_ID_MIX, sOutData.MIXAD);
	(void)Motor_SetPosition(MOTOR_ID_NCF, sOutData.NCFAD);
	(void)FanControl_SetVolt(sOutData.FANVolt); 
	O_INCARFAN_2 = sEVOutData.IncarFAN_H;
	O_INCARFAN_1 = sEVOutData.IncarFAN_L;
	O_SOV1 = sEVOutData.SOV1;
	O_SOV2 = sEVOutData.SOV2;
	O_RHEAT = sOutData.RHeatOut;

	EXVSetPostion(EXV_NUM_EXV  ,sEVOutData.EXV_Postion);
	EXVSetPostion(EXV_NUM_BEXV  ,sEVOutData.BEXV_Postion);
	EXVSetPostion(EXV_NUM_THREEWAY1  ,sEVOutData.ThreeWay1_Postion);
	EXVSetPostion(EXV_NUM_THREEWAY2  ,sEVOutData.ThreeWay2_Postion);
	EXVSetPostion(EXV_NUM_FOURWAY  ,sEVOutData.FourWay_Postion);
	EXVSetPostion(EXV_NUM_AGS_A  ,sEVOutData.AGS_Postion);
	EXVSetPostion(EXV_NUM_AGS_B  ,sEVOutData.AGS_Postion);

	WPumpSetDuty(0, sEVOutData.EWPT1);
	WPumpSetDuty(1, sEVOutData.EWPT2);
	WPumpSetDuty(2, sEVOutData.EWPT3);
}

