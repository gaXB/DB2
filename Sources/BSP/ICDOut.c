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
	}
	else
	{
		O_5VEN = GPIO_CLEAR;
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
  
	(void)FanControl_SetVolt(sOutData.FANVolt); 
}

