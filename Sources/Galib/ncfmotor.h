/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：ncfmotor.h
* 功能描述：电机模块的头文件。
* 为此模块能正常工作需要加入 需要定时器模块
*
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 *
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年5月20日
*
* 版本：1.1
* 说明：电机默认为不动作，设定为255时为不动作
* 作者：xb
* 完成日期：2020年9月29日
*/ 
#ifndef NCFMOTOR_H__
#define NCFMOTOR_H__

#include "timerout.h"

#define MAX_NCFMOTOR_NUM   3 //最多支持电机个数


//电机状态
#define NCFMOTOR_STATE_HALT   	0x00       //停止
#define NCFMOTOR_STATE_RESET  	0x01       //初始化
#define NCFMOTOR_STATE_RUNBYSET  0x02    	  //按照设定值运行

//内外电机设置 类型
typedef enum
{
	NCFMOTOR_POSITION_BACKWARD = 0,    //反转位置
	NCFMOTOR_POSITION_FORWARD,         //正转位置
	NCFMOTOR_POSITION_INVALID,         //错误设定
	NCFMOTOR_POSITION_STOP = 255,
}NcfMotorSetType;


BOOL NcfMotor_Init(uint16 outtime, uint8 nmotor);
void NcfMotor_Reset(void);
void NcfMotor_Control(void);
void NcfMotor_SetPosition(uint8 motorId, NcfMotorSetType mPostion);
void NcfMotor_ChangePosition(uint8 motorId);
NcfMotorSetType NcfMotor_GetPosition(uint8 motorId);
#endif

