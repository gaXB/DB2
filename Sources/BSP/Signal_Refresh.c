/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Signal_Refresh.c
* �����������ź�ˢ��ģ�飬�ڴ�ʵ��
* 1  �¶ȸ��¡�ʵ�����¸����߼�
* 2  �����źŸ���
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹أ���Ϊģ����Ҫ����ʵ����ĿӦ���������
* ������ƿɼ��ο��ļ�
*/
/**************************���ļ�¼******************************************************
��
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
* 
* �汾��1.1
* ˵�����Ż���ʼ���������߼���
* ��û�г����¶ȴ�����ʱ����2s���˳�
* ���ߣ�xb
* ������ڣ�2020��7��6��
* 
* �汾��1.2
* ˵����1 �Ż���ʼ���������߼���
* ��û�г����¶ȴ�����ʱ����2s���˳�,ûʵ��û����
* 2 ���Զ�ʱ�����ڣ����⣬���⣬ΪĬ��ֵ ȥ���˹���
* ���ߣ�xb
* ������ڣ�2020��10��22��
* 
* �汾��1.3
* ˵����1 ���ڳ������SigF_GetResult05���õ��¶ȣ��Լ������¶Ȼز�µĲ���
* ���ߣ�xb
* ������ڣ�2020��12��19��
* 
* �汾��1.4
* ˵����1 ��ģ���ļ�ֻʵ���ϵ������ȶ���Ž�������ˢ��ģʽ��������Ҫ�Լ�����뿴�ο��ļ�
* ���ߣ�xb
* ������ڣ�2021��8��20��
*/

#include "Signal_TempGet.h"
#include "APPDrive.h"
#include "Deal_DTC.h"


//ˢ��״̬
static uint8 u8SigRefState;    //��ʼ��ʱΪ1�������˲�ʱ0
static uint16 TimerInit;
//���¸���ˢ���߼�
static TempType tAmbCal_Init, tAmbCal_Init_1;   //��ʼֵ��ǰһ��ֵ
static int16 i16TambSum;    //��Ȩ����
static uint8 u8AddNum;
static TempType tWaterCal_Last;  //�����׶ε� ˮ�£���ʼ��ʱ������

#if  NOINIT_TYPE ==  NOINIT_TYPE_KEA
__attribute__ ((section(".noinit"))) uint8 bInitTamb;   //��һ���ϵ��ǣ�֮��0x5a��������MCU_IsBattRest
#elif NOINIT_TYPE == NOINIT_TYPE_RL78
#pragma section bss NOINIT
uint8 bInitTamb;   //��һ���ϵ��ǣ�֮��0x5a��������MCU_IsBattRest
#pragma section 
#define __GOLBAL_VAR_
#endif

//ˢ��״̬
#define  SIGREF_STATE_INIT    0
#define  SIGREF_STATE_RUN     1
#define  SIGREF_STATE_STOP    2
//��ģ�龲̬��������
static void Signal_Refresh_EndInit(void);
static void Signal_Refresh_Normal(void);
/*******************************************************************************
 * Function:        Deal_Signal_Refresh
 *
 * Description:    �¶�ˢ�����س��򣬷���lowerdrive-loop����
 *
 * Returns:         none
 *
 * Notes:           �˴�Ϊ�¶�ˢ�£����ڣ��⣬���⣬�ڴ�ˢ��
 *                  �����߼���Ҫ����ʵ���߼������޸�
 *                 
********************************************************************************/
void Deal_Signal_Refresh(void)
{
	switch (u8SigRefState)
	{
		default:
		case SIGREF_STATE_INIT:
		  //TODO: ��ʼ���׶εĴ���
			//����ʵ�������ȶ���Ž����ʼ��ģʽ����û�и���1s���������ģʽ
			if (TimeOutChkTenthSLong(&TimerInit, 1))
			{
				u8AddNum++;
#ifdef  ADID_TAMB
				//����ʵ���¶��ȶ�����ÿ0.2s�ɼ�һ�βɼ�10�Σ��ϴε��¶�ֵ�ͱ��β�0.5����˵���ź��ȶ�������������ģʽ
				i16TambSum += ADPhy_GetFact(ADID_TAMB);
				if (u8AddNum >= 10)
				{
					tAmbCal_Init = i16TambSum/10;
					if (GAMath_abs(tAmbCal_Init - tAmbCal_Init_1) < SCD_T(0.5))
					{//�¶��ȶ������ˢ��ģʽ
						u8SigRefState = SIGREF_STATE_RUN;
						Signal_Refresh_EndInit();
					}
					else
					{//���¼���2s
						u8AddNum = 0;
						i16TambSum = 0;
						tAmbCal_Init_1 = tAmbCal_Init;
					}
				}
				else{}
#else
//ADID_TAMB ��2s���˳���ʼ��
				if (u8AddNum >= 20)
				{
					Signal_Refresh_EndInit();
					u8SigRefState = SIGREF_STATE_RUN;
				}
				else{}
#endif
			}
			break;
		case SIGREF_STATE_RUN:
			Signal_Refresh_Normal();
			break;
		case SIGREF_STATE_STOP:
			break;
	}

	(void)Deal_DTC();
}
/*******************************************************************************
 * Function:       static void Signal_Refresh_Normal(void)
 *
 * Description:    ����ˢ������
 *
 * Returns:         none
 *
 * Notes:          none  
 *                 
********************************************************************************/
static void Signal_Refresh_Normal(void)
{
	//TODO: ��������µ��¶ȸ��£���ο�ģ��
}
/*******************************************************************************
 * Function:        void Signal_Refresh_EndInit(void)
 *
 * Description:     ������ʼ������
 *
 * Returns:         none
 *
 * Notes:           ��ʼ�����̽���ʱ����
 *                 
********************************************************************************/
static void Signal_Refresh_EndInit(void)
{
	//TODO: ��д��ʼ��������� �������̣�ģ��ʵ�����³�ʼ���߼����粻��Ҫ��ɾ��
	if (bInitTamb == 0x5a)
	{

	}
	else
	{//�ǵ�һ���ϵ�ı��
		bInitTamb = 0x5a;
	}
}

/*******************************************************************************
 * Function:    SigRefresh_IgnON /SigRefresh_IgnOFF
 *
 * Description: �ź�ˢ��֪ͨ��������Դ�ɴ���/��������ʱ����һ��
 *
 * Returns:     none
 *
 * Notes:       ����Ӧ��Ҫ���д
 *
*******************************************************************************/
void SigRefresh_IgnON(void)
{
	//TODO: ��һ���ϵ�Ĳ���
	tWaterCal_Last = sSenSorData.TwaterCal;
	u8SigRefState = SIGREF_STATE_INIT;
	(void)TimeOutChkTenthSLong(&TimerInit, 0);
	i16TambSum = 0;
	tAmbCal_Init_1 = ADPhy_GetFact(ADID_TAMB);
	u8AddNum = 0;
}


void SigRefresh_IgnOFF(void)
{
	//u8SigRefState = SIGREF_STATE_STOP; //���IGN����󣬲���Ҫ�ɼ�ˢ���¶�
}

/*******************************************************************************
 * Function:  void SignalCAN_RXComplete(uint32 u32Id)
 *
 * Description:  can rxcomplete
 *
 * Returns:      none
 *
 * Notes:     u32Id�� ���յ���CAN ID
 *        
*******************************************************************************/
void SignalCAN_RXComplete(uint32 u32Id)
{
	//TODO: Add code to read Frame singal here
	switch(u32Id)
	{		
		default: 
			break;
	}
}

/*******************************************************************************
 * Function:  void SignalCAN_RecTimeOut(uint32 u32Id)
 *
 * Description:  can rx timeout
 *
 * Returns:      none
 *
 * Notes:     u32Id�� ���յ���CAN ID
 *        
*******************************************************************************/
void SignalCAN_RecTimeOut(uint32 u32Id)
{
	//TODO:���źų�ʱ�Ĵ���
	switch(u32Id)
	{
		default:
			break;
	}
}


/*******************************************************************************
* Function:      int16 SigF_GetCurrentState(uint8 u8Num)
*
* Description:   sigfilter ģ���������� 
                 
* return :       ���ص�ǰ״̬
* Note  :        �밴ģ��˵���ļ���д

************************************************************************************/
int16 SigF_GetCurrentState(uint8 u8Num)
{
	//TODO:sigfilter ģ����������
	int16 i16SigData = 0;
	switch(u8Num)
	{
		default:
			i16SigData = 0xffff;
			break;
		case SIG_NUM_TIN:
			i16SigData = ADPhy_GetFact(ADID_TIN);
			break;
	}
	
	return i16SigData;
}

