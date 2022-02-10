/*
 * SRealy_IF.c
 *
 *  Created on: 2022Äê2ÔÂ9ÈÕ
 *      Author: Administrator
 */
#include "MCU_PORT.h"
#include "SpecialRealy.h"


uint8 SRealy_HWIF(uint8 id, uint8 setValue)
{
	uint8 bGetValue = 0;
	if (id == SREALY_NUM_SOV1)
	{
		O_SOV1 = setValue;
		bGetValue = I_SOV1_FB;
	}
	else 	if (id == SREALY_NUM_SOV2)
	{
		O_SOV2 = setValue;
		bGetValue = I_SOV2_FB;
	}
	else 	if (id == SREALY_NUM_TMU)
	{
		O_TMU = setValue;
		bGetValue = I_PTC4_FB;
	}
	else 	if (id == SREALY_NUM_RHEAT)
	{
		O_RHEAT = setValue;
		bGetValue = I_PTC3_FB;
	}
	else{}

	return bGetValue;
}
