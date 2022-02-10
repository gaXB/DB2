/*
 * SpecialRealy.c
 *
 *  Created on: 2022Äê2ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef MODULE_SPECIALREALY_C_
#define MODULE_SPECIALREALY_C_

#include "DataType.h"
#include "SpecialRealy.h"


typedef struct
{
	uint8 bSet;
	uint8 bGet;
	uint8 ErrCode;
	uint8 u8ErrTime;
}SREALYDATA;

SREALYDATA  sSRealyData[SREAL_MAXNUM];


extern uint8 SRealy_HWIF(uint8 id, uint8 setValue);


void SRealyControl(void)
{
	uint8 num;
	SREALYDATA* pRealyData = sSRealyData;

	for (num = 0; num < SREAL_MAXNUM; num++)
	{
		pRealyData->bGet = SRealy_HWIF(num, pRealyData->bSet);
		pRealyData->u8ErrTime++;

		if (pRealyData->u8ErrTime > ERRSET_TIMES)
		{
			pRealyData->u8ErrTime = 0;
			if (pRealyData->bSet == 0 && pRealyData->bGet == 1)
			{
				pRealyData->ErrCode = SREALY_ERR_OPENLOAD;
			}
			else if (pRealyData->bSet == 1 && pRealyData->bGet == 0)
			{
				pRealyData->ErrCode = SREALY_ERR_OVERCUNRRENT;
			}
			else
			{
				pRealyData->ErrCode = 0;
			}
		}
		pRealyData++;
	}

}


void SRealySet(uint8 id, uint8 bSetValue)
{
	if (bSetValue != sSRealyData[id].bSet)
	{
		sSRealyData[id].bSet = bSetValue;
		sSRealyData[id].u8ErrTime = 0;
	}
	else{}
}

uint8 SRealyGetErr(uint8 id)
{
	return sSRealyData[id].ErrCode;
}
#endif /* MODULE_SPECIALREALY_C_ */
