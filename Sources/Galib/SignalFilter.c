/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SignalFilter.c
* �����������ź��˲��ȶ�ģ�飬���� ad���¶ȵ��źŲ��ȶ���ƽ������
* ����ʱ��Ϊ ��  COLLECT_PRIDE_TIME �� COLLECT_TIMES ��ms��
* ����ʽ��   ����ֵ =  ����ƽ�� �� COLLECT_WEIGHT% +  ��һ��* (1-COLLECT_WEIGHT)
* ע�� ����ʼ��ʱ��һ���˲���Ҫ500ms���Ҳ��ܲɼ���� 
* 
* ��ģ��ʹ����Ҫ��ʱ��ģ��
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭģ���1.0��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��5��14��
* 
* �汾��1.1
* ˵��������SIG_MAX_NUM ��10��Ϊ15
* ���ߣ�xb
* ������ڣ�2020��1��6��
* 
* �汾��1.2
* ˵����
* 1 ����05���˲������¶Ȼز��
* 2 �Ż�-��ʱ����������
* ���ߣ�xb
* ������ڣ�2020��12��4��
*/
#include "SignalFilter.h"



//�ź��˲��ṹ�嶨��
typedef struct
{
	int16  i16SigData[COLLECT_TIMES];      //������
	int16  i16SigResult;                   //������
	int16  i16Result05;
}SIGNALDATA;


static SIGNALDATA sSigData[SIG_MAX_NUM];
static uint8 u8TotalNum = SIG_MAX_NUM;

//�ײ������ӿں���
extern int16 SigF_GetCurrentState(uint8 u8Num);
/*******************************************************************************
* Function:      BOOL SigF_Init(uint8 u8MaxNum)
*
* Description:   ��ʼ�������嵱ǰ������
                 
* return :       TRUE : �ɹ��� false ʧ��
* Note  :        ��ʼ������������Ұѽ��i16SigResult =0

************************************************************************************/
BOOL SigF_Init(uint8 u8MaxNum)
{
	uint8 i;
	BOOL bInitOK;
	SIGNALDATA *pSigData;
	
	if (u8MaxNum <= SIG_MAX_NUM)
	{
		u8TotalNum = u8MaxNum;
		bInitOK = TRUE;
		
		pSigData = sSigData;
		for(i = 0; i < u8TotalNum; i++)
		{
			pSigData->i16SigResult = 0;
			pSigData++;
		}
	}	
	else
	{
		bInitOK = FALSE;
	}
			
	return bInitOK;
}

/*******************************************************************************
* Function:     int16 SigF_GetResult(uint8 u8SigNum)
*
* Description:   u8SigNum  �ź�ͨ��
                 
* return :       �õ���ǰ�ź�ֵ���Ѿ��˲�
* Note  :        

************************************************************************************/
int16 SigF_GetResult(uint8 u8SigNum)
{
	int16 i16Result;
	if (u8SigNum < SIG_MAX_NUM)
	{
		i16Result = sSigData[u8SigNum].i16SigResult;
	}
	else
	{
		i16Result = 0;
	}
	
	return i16Result;
}

/*******************************************************************************
* Function:     int16 SigF_GetResult(uint8 u8SigNum)
*
* Description:   u8SigNum  �ź�ͨ��
                 
* return :       �õ���ǰ�ź�ֵ���Ѿ��˲�
* Note  :        

************************************************************************************/
int16 SigF_GetResult05(uint8 u8SigNum)
{
	int16 i16Result;
	if (u8SigNum < SIG_MAX_NUM)
	{
		i16Result = sSigData[u8SigNum].i16Result05;
	}
	else
	{
		i16Result = 0;
	}
	
	return i16Result;
}

/*******************************************************************************
* Function:      static void SigF_CalcResult(uint8 u8SigNum)
*
* Description:  �Ե�u8SigNum �źŽ����˲��㷨
                 i16SigResult = AVG(i16SigData) COLLECT_WEIGHT% +  i16SigResult* (1-COLLECT_WEIGHT)
* return :       none
* Note  :        

************************************************************************************/
static void SigF_CalcResult(uint8 u8SigNum)
{
	uint8 i;
	int32 i32AvgSig = 0;
	SIGNALDATA *pSigData = &sSigData[u8SigNum];
	
	for (i = 0; i < COLLECT_TIMES; i++)
	{
		i32AvgSig += pSigData->i16SigData[i];
	}
	
	i32AvgSig /= COLLECT_TIMES;
	
	i32AvgSig = i32AvgSig * COLLECT_WEIGHT + pSigData->i16SigResult * (100-COLLECT_WEIGHT);
	
	if (i32AvgSig % 100 >= 50)
	{//4 �� 5��
		i32AvgSig += 100;
	}
	else if (i32AvgSig % 100 <= -50)
	{
		i32AvgSig -= 100;
	}
	else{}

	pSigData->i16SigResult = (int16)(i32AvgSig / 100);
	if ( (pSigData->i16SigResult - pSigData->i16Result05) >= 4)
	{
		pSigData->i16Result05 = (pSigData->i16SigResult+1)/5;
		pSigData->i16Result05 = pSigData->i16Result05 *5;
	}
	else if ((pSigData->i16SigResult - pSigData->i16Result05) <= -4)
	{
		pSigData->i16Result05 = (pSigData->i16SigResult-1)/5;
		pSigData->i16Result05 = pSigData->i16Result05 *5;
	}
	else
	{//����֮�䲻��
		
	}
}
/*******************************************************************************
* Function:      void SigFControl(void)
*
* Description:    ���غ���������loop�е���
                 
* return :       NONE
* Note  :        

************************************************************************************/
void SigFControl(void)
{
	static uint16 TimerCollect;
	static uint8  u8CollectTimes;
	SIGNALDATA *pSigData;
	uint8 i;
	
	if (TimeOutChkMsLong(&TimerCollect, COLLECT_PRIDE_TIME))
	{
		pSigData = sSigData;
		for (i = 0; i < u8TotalNum; i++)
		{
			pSigData->i16SigData[u8CollectTimes] = SigF_GetCurrentState(i);
			pSigData++;
		}
		
		u8CollectTimes++;
		if (u8CollectTimes >= COLLECT_TIMES)
		{
			u8CollectTimes = 0;
			for (i = 0; i < u8TotalNum; i++)
			{
				SigF_CalcResult(i);
			}	
		}
		else{}
	}
}
