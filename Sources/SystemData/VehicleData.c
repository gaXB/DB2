/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：VehicleData.c
* 功能描述： 整车变量，可分为2部分， 整车的反馈和can的部分数据可根据实际应用增加，本项目中预留
* 
* 编译器： codewarrior IDE 11.1
* MCU说明：和MCU 无关
* 按照应用要求增加
*/
/**************************更改记录******************************************************
* 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月13日
* 
* * 版本：1.1
* 说明：OtherControl 放到VehicleData 
* 作者：xb
* 完成日期：2020年11月21日
*/
#include "VehicleData.h"

VEHICLEDATA   sVehicleData;
OTHERCONTROL OtherControl;
