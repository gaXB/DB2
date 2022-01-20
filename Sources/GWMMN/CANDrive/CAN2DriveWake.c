/************************�ļ�˵��************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾   All rights reserved.
*
* �ļ����ƣ�CANDriveWake.c
* ����������ʵ��CAN ���ߣ����ѹ���
* �������� codewarrior IDE 10.7
* MCU˵����TJA1040 �ײ�����, KEAZ128 kbi����
*/
/**************************���ļ�¼******************************************************
 *  ���ļ�û����������ܱ�����
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��
* 
* �汾��1.1
* ˵����KBI �˿�ʹ�ú궨����CANDrive.h ��ʵ�֡���ʹ��KEAZ128��TJ1040 ϵ�� ʱ���ļ����ø���
* ԭ���ߣ�xb
* ������ڣ�2019��10��22��
*
* �汾��1.2
* ˵����  CANDrive_SleepIO(0);	
	      Init_Mscan();  ����һ��
* ԭ���ߣ�xb
* ������ڣ�2019��12��24��

*/
#include "CAN2Drive.h"


//CAN �жϻ��� �ײ�������������
static	uint8	bCANWake;            //��ǰcan�Ƿ���ڹ���״̬�� 1: Ϊ���ڹ��������߱����ѣ� 0��can�����Ѿ�ֹͣ�������� 
static  	uint8	bCANWakeBegin;       //�Ƿ����ʹ�ܻ��� 1�����ԣ� 0��������
static  	uint8	bCANKBIInd;          //�жϱ��
#if _CANWAKE_FUN_
static uint16 TimerCANWake;     //canwke��ʱ��
static uint8  u8CANWakeTimes;      //��ʱ100ms����һ��
#endif
/*******************************************************************************
 * Function:     void CANDrive_SleepIO(uint8 En)
 *  *
 * Description:    CAN ˯�߿�״̬
 *
 * Returns:         none
 *
 * Notes:           En: 1 Ϊ����    0:Ϊ����
 *
********************************************************************************/
void CAN2Drive_SleepIO(uint8 En)
{
	if (En)
	{
		O_CAN2EN = GPIO_SET;
	}
	else
	{
		O_CAN2EN = GPIO_CLEAR;
	}
}
/*******************************************************************************
* Function:     void Init_KBI_1()
*
* Description:    �ߵ͵�ƽ�ж�����
* 
* return :       none
* 
* Note  :        
************************************************************************************/ 
void Init_KBI2_1(void)
{

	//KBI_SetCallback(KBI0, &KBI0_Task);
	//KBI_SetCallback(KBI1, &KBI1_Task);
}

/*******************************************************************************
* Function:     void Init_KBI_0()
*
* Description:    �ߵ͵�ƽ�ж�����
* 
* return :       none
* 
* Note  :        
************************************************************************************/ 
void Init_KBI2_0(void)
{

}

/*******************************************************************************
 * Function:        uint8 GetCanWakeEn(void)
 *
 * Description:     ���ص�ǰCAN �Ƿ��ڹ���1 :Ϊ�����ѡ� 0�� û�б�����
 *                  //�������can ���ѹ��ܣ�ֱ�ӷ���0����
 * Returns:         none
 *
 * Notes:           ��Դ����ģ�����
 *
********************************************************************************/
uint8 GetCan2WakeEn(void)
{
#if _CANWAKE_FUN_
	return bCANWake;
#else
	return 0;
#endif
}

/*******************************************************************************
 * Function:      void KBI1_IRQHandler(void)
 *  *
 * Description:     KBI �ж�
 *
 * Returns:         none
 *
 * Notes:           none
 *
********************************************************************************/
void KBI21_IRQHandler(void)
{

}

void KBI20_IRQHandler(void)
{

}
/*******************************************************************************
 * Function:        void Deal_CANWake(void)
 *  *
 * Description:     �жϵ�ǰ��wakeen ״̬
 *
 * Returns:         none
 *
 * Notes:           ��Դ����ģ����ã�ע����sleepѭ��ʱ��Ҳ��Ҫ����
 *                ʵ��:
 *                һ ����ģʽʱ��
 *                1 ͨ��ind�ź��жϵ�ǰ�Ƿ����ж�
 *                2 ��ind�ź�û�е�ʱ�����2s����Ϊ����CAN
 *                �� ����ģʽʱ
 *                1 ͨ��ind�ź��жϵ�ǰ�Ƿ����ж�
 *
********************************************************************************/
void Deal_CAN2Wake(void)
{
#if _CANWAKE_FUN_
	if (bCANKBIInd && bCANWakeBegin)
	{
		u8CANWakeTimes = 0;
		bCANWake = 1;
		(void)TimeOutChkMsLong(&TimerCANWake, 0);
	}
	else
	{
		if (TimeOutChkMsLong(&TimerCANWake, 100))
		{
			if (u8CANWakeTimes >= CANUNWAKE_TIME)
			{
				bCANWake = 0;
				bCANWakeBegin = 1;
			}
			else
			{
				u8CANWakeTimes++;
			}
		}
	}
	bCANKBIInd = 0;
#endif
}
/*******************************************************************************
 * Function:       void CANWake_Begin(uint8 bEN)
 *  *
 * Description:    �Ƿ�ʹ��canwake��1��ʹ�� 0����ֹ
 *
 * Returns:         none
 *
 * Notes:           ��Դ����ģ�����
 *
********************************************************************************/
void CAN2Wake_Begin(uint8 bEN)
{
	bCANWakeBegin = bEN;
	bCANWake = 0;
	bCANKBIInd = 0;
}

/*******************************************************************************
 * Function:       void CANDrive_GotoSleep(void)
 *  *
 * Description:    can ��������sleep ģʽ
 *
 * Returns:         none
 *
 * Notes:           ��Դ����ģ�����
 *
********************************************************************************/
void CAN2Drive_GotoSleep(void)
{
	CANDrive_SleepIO(1);
	CANDrive_Close();
#if _CANWAKE_FUN_
	Init_KBI2_1();
#endif
}
/*******************************************************************************
 * Function:      void CANDrive_GotoAWake(void)
 *  *
 * Description:    can ����׼�������������� ģʽ
 *
 * Returns:         none
 *
 * Notes:           ��Դ����ģ�����
 *
********************************************************************************/
void CAN2Drive_GotoAWake(void)
{
	CAN2Drive_SleepIO(0);	
	Init_Mscan2();  
#if _CAN2WAKE_FUN_
	Init_KBI2_0();
#endif
}
/*******************************************************************************
 * Function:     void CANGet_Fram(void)
 *  *
 * Description:    ���յ�can֡
 *
 * Returns:         none
 *
 * Notes:           �ײ�candrive����
 *
********************************************************************************/
void CAN2Get_Fram(void)
{
	bCANKBIInd = 1;
}

