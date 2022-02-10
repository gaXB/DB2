/*
 * WaterPump.c
 *
 *  Created on: 2021年12月29日
 *      Author: Administrator
 */

#include "WaterPump.h"

typedef struct
{
	uint16 SetDuty;
	uint16 RunDuty;
	uint16 ColletLowNum;
	uint16 LowStartNum;
	uint16 LowEndNum;
	uint16 LastErrLowEndTime;
	uint16 LastErrStartTime;
	uint8  ErrCode;
	uint8  ControlState;
}WPUMPDATA;


WPUMPDATA  sWPumpData[MAX_PUMP_NUM];

#define     PUMP_COLLET_TIME       10

#define     PUMP_OTHER_ERR_TIMES       (2400/PUMP_COLLET_TIME)
#define     PUMP_OVERTEMP_ERR_TIMES       (1900/PUMP_COLLET_TIME)
#define     PUMP_OVERCURRENT_ERR_TIMES       (1400/PUMP_COLLET_TIME)
#define     PUMP_ERR_TIMES       (900/PUMP_COLLET_TIME)
#define     PUMP_OK_TIMES_MIN       (400/PUMP_COLLET_TIME)
#define     PUMP_OK_TIMES_MAX       (600/PUMP_COLLET_TIME)
uint8 WPumpIf_GetFB(uint8 num);
void  WPumpIf_OutPWM(uint8 num, uint16 duty);

void  WPumpControl(void)
{
	static uint16 TimerPump;
	static uint16 ColletNum;
	uint8 num;
	WPUMPDATA* pData = sWPumpData;

	if (TimeOutChkMsLong(&TimerPump, PUMP_COLLET_TIME))
	{
		ColletNum++;

		for (num = 0; num < MAX_PUMP_NUM; num++)
		{
			if (WPumpIf_GetFB(num) == 1)
			{//外面为低
				if (pData->ColletLowNum == 0)
				{
					pData->LowStartNum = ColletNum;
				}
				pData->ColletLowNum++;
			}
			else
			{
				if (pData->ColletLowNum > PUMP_ERR_TIMES)
				{
					if (pData->ControlState == 0)
					{
						pData->ControlState = 1;
					}
					else
					{
						pData->ControlState = 2;
					}
					if (pData->ColletLowNum > PUMP_OTHER_ERR_TIMES)
					{
						pData->ErrCode = PUMP_ERROR_OTHER;
					}
					else	if (pData->ColletLowNum > PUMP_OVERTEMP_ERR_TIMES)
					{
						pData->ErrCode = PUMP_ERROR_OVERTEMP;
					}
					else	if (pData->ColletLowNum > PUMP_OVERCURRENT_ERR_TIMES)
					{
						pData->ErrCode = PUMP_ERROR_OVERCURRENT;
					}
					else
					{
						pData->ErrCode = PUMP_ERROR_UNLOAD;
					}
					pData->LastErrStartTime = pData->LowStartNum;
					pData->LastErrLowEndTime = ColletNum;
				}
				else if (pData->ColletLowNum > PUMP_OK_TIMES_MIN && pData->ColletLowNum > PUMP_OK_TIMES_MAX)
				{
					pData->ControlState = 0;
					pData->ErrCode = PUMP_ERROR_NULL;
				}
				else{}
				pData->ColletLowNum = 0;
			}
			WPumpIf_OutPWM(num, pData->SetDuty);
			pData++;
		}
	}
}

void  WPumpSetDuty(uint8 id, uint8 duty)
{
	if (duty >= 93)
	{
		sWPumpData[id].SetDuty = 93;
	}
	else
	{
		sWPumpData[id].SetDuty = duty;
	}
}


uint8  WPumpGetErr(uint8 id)
{
	return sWPumpData[id].ErrCode;
}
