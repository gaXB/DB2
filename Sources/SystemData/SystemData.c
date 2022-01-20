/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SystemData.c
* 功能描述： 系统变量
* 1 应用控制变量及其他
* 2 传感器变量
* 3 输出/显示变量
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
* 版本：1.2
* 说明：OtherControl 放到VehicleData 保证此文件改动较小
* 作者：xb
* 完成日期：2020年11月21日
* 
* 版本：1.3
* 说明：NOINIT 变量兼容Renesas
* 作者：xb
* 完成日期：2021年8月23日
*/
#include "SystemData.h"

/*************************************************************************************************4
 * 系统状态变量定义
****************************************************************************************************/

#if  NOINIT_TYPE ==  NOINIT_TYPE_KEA
__attribute__ ((section(".noinit"))) SYSTEMCONTROL  SystemControl;    //系统变量 
__attribute__ ((section(".noinit"))) SENSORDATA     sSenSorData;      //传感器信号 结构体
VEHICLECONTROL  sVehicleControl;
EVSENSORDATA sEVSensorData;
ACTORDATA sActorData;
PANLEDATA sPanleData;
#elif NOINIT_TYPE == NOINIT_TYPE_RL78
#pragma section bss NOINIT
SYSTEMCONTROL  SystemControl;    //系统变量 
SENSORDATA     sSenSorData;      //传感器信号 结构体
#pragma section 
#define __GOLBAL_VAR_
#endif


uint8 u8ConfigData[3];  //配置字
OUTDATA sOutData;
EVOUTDATA sEVOutData;
DISPLAYDATA sDisplayData;

#if _PROJECT_CAN_
//pc 设置变量
PCCONTROLEN_TYPE PCControlEn;
SENSORDATA     sSenSorData_PC;
EVSENSORDATA sEVSensorData_PC;
OUTDATA sOutData_PC;
EVOUTDATA sEVOutData_PC;
#endif
/*******************************************************************************
 * Function:   SYSTEMCONTROL* SystemControlGet(void)
 *
 * Description: 返回系统控制状态
 *
 * Returns:     SYSTEMCONTROL*
 *
 * Notes:     外部模块调用
 *
*******************************************************************************/
SYSTEMCONTROL* SystemControlGet(void)
{
	return &SystemControl;
}
