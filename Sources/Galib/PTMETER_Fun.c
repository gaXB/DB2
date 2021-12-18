/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�PTMETER_Fun.c
* ������������λ������ģ�飬ͨ����ǰ��λ����AD�õ���λ���ĵ�ѹ
* 
*
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 *
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��6��17��
*
*/ 
#include "PTMETER_Fun.h"
#include "MathGa.h"
#include "timerout.h"

static PTMETERCONFIG PTConfig[MAX_PTMETER_NUM];   //���ñ���

uint8 u8PTLevel[MAX_PTMETER_NUM];  //����ĵ�λ������λֵ
uint8 u8PTTotalNum; //������


//�ⲿ��������
extern uint16 PTMETERHW_Read(uint8 num);
/*******************************************************************************
 * Function:    BOOL PTMETER_Init(uint8 u8Num, PTMETERCONFIG ConfigSet[])
 *
 * Description:  ��λ����ʼ��
 *
 * Returns:     �Ƿ�ɹ�
 *
 * Notes:      
*******************************************************************************/
BOOL PTMETER_Init(uint8 u8Num, PTMETERCONFIG ConfigSet[])
{
	uint8 i;
	BOOL ptInitRes;

	if (u8Num <= MAX_PTMETER_NUM)
	{
		ptInitRes = TRUE;
		
		for (i = 0; i< u8Num; i++)
		{
			PTConfig[i] = ConfigSet[i];
		}
		u8PTTotalNum = u8Num;
	}
	else
	{
		ptInitRes = FALSE;
	}
	
	return ptInitRes;
}
/*******************************************************************************
 * Function:    void PTMETER_Control(void)
 *
 * Description:  ���غ���
 *
 * Returns:     none 
 *
 * Notes:      loop ����
*******************************************************************************/
void PTMETER_Control(void)
{
	static uint16 TimerColt;
	uint8 u8Id;
	uint16 u16AD;
	int16 i16lData;
	uint8 u8Level;
	int16 i16HystPer;
	PTMETERCONFIG* pConfig;
	pConfig = PTConfig;
	
	if (TimeOutChkMsLong(&TimerColt, 10))
	{
		for (u8Id = 0; u8Id < u8PTTotalNum; u8Id++)
		{
			u16AD = PTMETERHW_Read(u8Id);
			
			i16lData = GAMath_LineFuni16(pConfig->i16StartAD, pConfig->i16EndAD, 0, pConfig->MaxLevel*100, u16AD);
			if (i16lData <= 0)
			{
				u8Level = 0;
			}
			else
			{
				u8Level = (uint8)(i16lData/100); 
				i16lData = i16lData%100;
				if (pConfig->MaxLevel >= 19)
				{
					i16HystPer = 0;
				}
				else
				{
					i16HystPer = PTMETER_HYSTERESIS_PER;
				}
				if (i16lData > (50+i16HystPer))
				{
					u8Level++;
				}
				else if (i16lData < (50-i16HystPer))
				{
					
				}
				else
				{
					if (u8PTLevel[u8Id] > u8Level)
					{//��ǰ��λ����
						u8Level++;
					}
					else
					{
					}
				}
				
				if (u8Level >= pConfig->MaxLevel)
				{
					u8Level = pConfig->MaxLevel;
				}
				else{}
			}//calc level
			u8PTLevel[u8Id] = u8Level;
			pConfig++;
		}//for
	}//time
	else
	{
		
	}
}

/*******************************************************************************
 * Function:      uint8 PTMETER_GetLevel(uint8 u8PTId)
 *
 * Description:   �õ���ǰ��id��λ���ĵ�λֵ
 *
 * Returns:     none 
 *
 * Notes:       Ӧ�õ���
*******************************************************************************/
uint8 PTMETER_GetLevel(uint8 u8PTId)
{
	return u8PTLevel[u8PTId];
}
