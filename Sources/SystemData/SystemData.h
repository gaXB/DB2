/************************文件说明*****************************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SystemData.h
* 功能描述： 1 系统变量的外部变量说明，以APP使用
* 2 SystemApp.c 系统状态切换的函数声明
* 编译器： codewarrior IDE 11.1
* MCU说明：和MCU 无关
*  此文件会为模板，基本不用更改
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月13日
* 
* 版本：1.1
* 说明：增加输入输出强制功能
* 作者：xb
* 完成日期：2020年5月30日
* 
*/

#ifndef   SYSTEMDATA_H__
#define   SYSTEMDATA_H__

#include "SystemType.h"
#include "SystemConfig.h"
#include "VehicleData.h"
/*******************************************************************************
 * 其他全局结构体定义 ,故障码定义
 *
********************************************************************************/
extern SYSTEMCONTROL  SystemControl;
extern SENSORDATA sSenSorData; 
extern OUTDATA sOutData;
extern DISPLAYDATA sDisplayData;
extern uint8  u8ConfigData[3];
extern VEHICLECONTROL  sVehicleControl;
extern EVOUTDATA sEVOutData;

EVSENSORDATA sEVSensorData;
ACTORDATA sActorData;
#if _PROJECT_CAN_
extern PCCONTROLEN_TYPE PCControlEn;
extern SENSORDATA     sSenSorData_PC;
extern EVSENSORDATA     sEVSensorData_PC;
extern OUTDATA sOutData_PC;
extern EVOUTDATA sEVOutData_PC;
extern PANLEDATA sPanleData;
#endif

SYSTEMCONTROL* SystemControlGet(void);


#endif
