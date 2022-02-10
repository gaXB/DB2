/*
 * LINEXV.c
 *
 *  Created on: 2021��12��29��
 *      Author: Administrator
 */


#include "LINEXV.h"
#include "timerout.h"
EXVDATA  sEXVData[EXVNUM];



#define INIT_IN_PROCESS  0x00  //���ڳ�ʼ��
#define INIT_INITIALIZED 0x01  //��ʼ�����
#define IN_CONTROL       0x02    //״̬�ı�


uint8 EXVIF_GetRecFlag(EXVDATA* pEXV, uint8 num);


void EXVControl(void)
{
	uint8 num;
	EXVDATA* pExvData = sEXVData;

	for (num = 0; num < EXVNUM; num++)
	{
		if (EXVIF_GetRecFlag(pExvData, num))
		{
			(void)TimeOutChkMsLong(&pExvData->TimerRec, 0);
			pExvData->bLostErr = 0;
			switch (pExvData->u8State)
			{
				default:
				case INIT_IN_PROCESS:
					(void)TimeOutChkTenthSLong(&pExvData->TimerControl, 0);
					pExvData->InitReq = 1;
					pExvData->bEnable = 1;
					pExvData->u16ComPostion = 0;
					pExvData->u8State = INIT_INITIALIZED;
					break;
				case INIT_INITIALIZED:
					if (TimeOutChkTenthSLong(&pExvData->TimerControl, 20))
					{
						pExvData->InitReq = 2;
						pExvData->bEnable = 1;
						pExvData->u16ComPostion = 0;
						pExvData->u8State = IN_CONTROL;
					}
					break;
				case IN_CONTROL:// &&
					if(pExvData->InitRet)
					{
						pExvData->InitReq = 0;
						pExvData->bEnable = 1;
						pExvData->u16ComPostion = pExvData->u16SetValue;
					}
					break;
			}
		}
		else
		{
			if (TimeOutChkMsLong(&pExvData->TimerRec, 5000))
			{
				pExvData->bLostErr = 1;
			}else{}
		}
		pExvData++;
	}
}


void EXVSetPostion(uint8 num, uint16 postion)
{
	sEXVData[num].u16SetValue = postion;
}


uint8 EXVGetLostErr(uint8 num)
{
	return sEXVData[num].bLostErr;
}
