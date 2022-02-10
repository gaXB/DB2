/*
 * WaterPump.h
 *
 *  Created on: 2021Äê12ÔÂ29ÈÕ
 *      Author: Administrator
 */

#ifndef MODULE_WATERPUMP_H_
#define MODULE_WATERPUMP_H_

#include "timerout.h"

#define  PUMP_ERROR_NULL           0x0
#define  PUMP_ERROR_UNLOAD         0x01
#define  PUMP_ERROR_OVERCURRENT    0x02
#define  PUMP_ERROR_OVERTEMP       0x03
#define  PUMP_ERROR_OTHER          0x04


#define   MAX_PUMP_NUM      3

void  WPumpSetDuty(uint8 id, uint8 duty);
void  WPumpControl(void);
uint8  WPumpGetErr(uint8 id);
#endif /* MODULE_WATERPUMP_H_ */
