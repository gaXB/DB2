/*
 * WaterPumpIF.c
 *
 *  Created on: 2021Äê12ÔÂ29ÈÕ
 *      Author: Administrator
 */

#include "WaterPump.h"
#include "MCU_PORT.h"

uint8 WPumpIf_GetFB(uint8 num)
{
	uint8 PinValue = 0;
	if (num == 0)
	{
		PinValue = I_PUMP1_FB;
	}
	else 	if (num == 1)
	{
		PinValue = I_PUMP2_FB;
	}
	else 	if (num == 2)
	{
		PinValue = I_PUMP3_FB;
	}

	return PinValue;
}
void  WPumpIf_OutPWM(uint8 num, uint16 duty)
{
	if (num == 0)
	{
		PUMP1_PWM = PWMMODE_PUMP * duty/ 100;
	}
	else 	if (num == 1)
	{
		PUMP2_PWM = PWMMODE_PUMP * duty/ 100;
	}
	else
	{
		PUMP3_PWM = PWMMODE_PUMP * duty/ 100;
	}
}
