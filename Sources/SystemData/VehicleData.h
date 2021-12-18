/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：VehicleData.h
* 功能描述： 整车变量的类型定义，默认值，外部变量声明
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
* 
*/
#ifndef VEHICLEDATA_H_
#define VEHICLEDATA_H_
#include "SystemType.h"




//Key state
enum{          
	VCUKEY_OFF=0,
	VCUKEY_ON,
	VCUKEY_START,
	VCU_KEY_ERROR,
}; 

//整车状态值，在此增加
typedef struct
{
	uint8   RHeatBack;              //后除霜反馈状态
	uint8   VCUKeyState;	
	uint16  EXVPostion;             //电子膨胀阀反馈位置
	uint16 u16EngSpeed;             //车辆发动机转速
}VEHICLEDATA;

//其他控制状态，在此增加
typedef struct
{
	uint8 CHeatMode_L;
	uint8 CHeatMode_R;    //左右座椅加热
}OTHERCONTROL ;



extern OTHERCONTROL OtherControl;
extern VEHICLEDATA  sVehicleData;




#define  _KEY_STATE_DEFAULT_  2            /****车钥匙默认状态  START*****/

#endif /* VEHICLEDATA_H_ */
