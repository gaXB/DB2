/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：motor.h
* 功能描述：电机模块的头文件。
* 为此模块能正常工作需要加入 需要定时器模块，和数学函数
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
* 说明：增加停机工作，当设定位置=255时 电机停机
* 原作者：xb
* 完成日期：2020年9月28日
* 
* * 版本：1.2
* 说明：
* 1 当电机抖动时未设置DriveState的bug修复，抖动AD MOTOR_STOPSHAKE_AD 由2->3
* 2 电机精度值-默认值由3改为5
* 原作者：xb
* 完成日期：2020年12月2日
* 
* 版本：1.3
* 说明：
* 1 精度值MOTOR_DRIVE_ACCURACY 由宏定义变为int16,默认5
* 2 提前量默认为1，可配置
* 3 增加MotorDriveInit 来设定精度和提前量
* 作者：xb
* 完成日期：2020年12月5日
*/
#ifndef MOTOR_H__
#define MOTOR_H__


#include "timerout.h"

#define  MAX_MOTOR_NUM     4            //最多支持电机个数
#define  MOTOR_RUN_TIME    2            //处理时间
//#define  MOTOR_SHORTERROR_SHORT        1
//#define  MOTOR_SHORTERROR_OPEN         2

//正常工作电机状态
#define  MOTOR_STATE_HALT             0x00   //电机停机
#define  MOTOR_STATE_FORWARD          0x01   //电机正传
#define  MOTOR_STATE_BACKWARD         0x02   //电机反转
#define  MOTOR_STATE_JAMMED_ERROR     0x03   //电机堵转故障
#define  MOTOR_STATE_RESET            0x05   //电机初始化
#define  MOTOR_STATE_WAITHALT         0x09   //正反转切换时停止1s
//#define  MOTOR_STATE_JUMP_ERROR       0x10   // 电机跳变故障 ,未用到，功能预留
#define  MOTOR_STATE_SHAKE            0x11   //电机跳变状态



//电机短路工作时的状态
#define  MOTOR_STATE_SHORT_ENTER       0x12      //短路工作进入
#define  MOTOR_STATE_SHORT_FORWARD     0x13      //电机短路正转
#define  MOTOR_STATE_SHORT_BACKWARD    0x14      //短路工作修正
#define  MOTOR_STATE_SHORT_GETPOSTION  0x15      //找到位置的状态
#define  MOTOR_STATE_SHORT_LOSE        0x16      //丢失步数


//电机自检工作时的状态
#define  MOTOR_STATE_TEST_ENTER           0x17   //自检进入
#define  MOTOR_STATE_TEST_FORWARD         0x18   //自检正转
#define  MOTOR_STATE_TEST_BACKWARD        0x19   //自检反转
#define  MOTOR_STATE_TEST_GOTOMIDDLE      0x20   //回到中间
#define  MOTOR_STATE_TEST_END             0x21   //自检退出
                                                           
//电机工作模式
#define  MOTOR_WORK_NORMAL    0x00     //正常工作
#define  MOTOR_WORK_SHORT     0x01      //短路工作
#define  MOTOR_WORK_SELFTEST  0x02      //自检工作
#define  MOTOR_WORK_STOP      0x03      //停止工作

//故障码
#define  MOTOR_ERROR_NONE     0x00   //电机正常
#define  MOTOR_ERROR_JAMMED   0x01   //电机堵转
#define  MOTOR_ERROR_TIMEOUT  0x02   //电机超时
#define  MOTOR_ERROR_SHORT    0x04   //电机短路
#define  MOTOR_ERROR_OPEN     0x08   //断路
#define  MOTOR_ERROR_SHAKE    0x10   //电机抖动故障
#define  MOTOR_ERROR_TEST     0x80   //测试故障
#define  MOTOR_ERROR_INVALID  0xff   //电机无效
#define  MOTOR_ERROR_BACKERR  (MOTOR_ERROR_SHORT | MOTOR_ERROR_OPEN)

#define  TEST_ERROR_NONE     		0x00
#define  TEST_ERROR_SHAKE     		0x01
#define  TEST_ERROR_NOTGETPOSTION   0x02
#define  TEST_ERROR_RANGESMALL        0x04      //范围不够大
#define  TEST_ERROR_RANGEOUT        0x08     //范围超出
#define  TEST_ERROR_DOING     		0x80
//系统相关
#define  MOTOR_SET_AD                  3  // 2次电机设置位置大于等此值才能改变set值，写成0 不一样就动
#define  MOTOR_JUMP_AD                 5  //2次电机ad采集值差此值以上
#define  MOTOR_RUNHATL_LEASTTIME       5  // 电机从转动到停止再到转动0.1*
#define  DEFAULT_MOTOR_BEFORDATA       1  // 电机提前量 
#define  DEFAULT_MOTOR_DRIVE_ACCURACY  5  //ad 电机的精度

//故障恢复处理
#define  MOTOR_SHORTERROR_WAITTIME   5   //短路故障后过多少时间（此时信号稳定）才开始动作。
#define  MOTOR_SHORT_FORWARD_TIME    100   //故障时的正反转时间
#define  MOTOR_SHORT_BACKWARD_TIME   20
#define  MOTORERR_SHORT_TIMES      20     //5ms周期的话就是100ms为故障

#define  MOTOR_STOP_AD			255
#define  MOTOR_OPEN_AD        250         //断路的ad
#define  MOTOR_SHORT_AD       5           //短路的ad
#define  DEAL_SHORTRECOVE_AD  10          //短路工作时如果位置差大于此值才进行处理

//抖动时相关宏定义
#define   MOTOR_SHAKEERROR_WAITTIME   10  //shake 故障恢复需要的时间
#define   MOTOR_RUNSHAKE_AD         1     //运动的情况反向shake 的ad
#define   MOTOR_STOPSHAKE_AD     3     //停止的情况下大于此值则算抖动
#define   MAX_ERECOVE_TIMES      10      //故障恢复的次数,未用
#define   MOTOR_JUMP_AD          5   //2次电机ad采集值差此值以上


//自检
#define  TEST_FORWARD_TIME          100         // 测试正反转   
#define  TEST_FORWARD_AD            240         // 正转的时到达此ad停止并返回
#define  TEST_BACKWARD_TIME         100         // 反转时 到 此时间 *0.1 
#define  TEST_BACKWARD_AD           20          // 自检时 到达此ad 返回
#define  TEST_MIDDLE_AD            128          // 必须大于MOTOR_DRIVE_ACCURACY




//系统需要功能可配置，以下为预定义
#define   NEEDFUNCTION_ELESMALL          0   // 最小电动模式，此模式时电机没有故障保护功能，
//只有按照电机电压进行动作，有堵转保护功能，此时代码最小

#if    NEEDFUNCTION_ELESMALL 
	#define   SHORT_AUTORECOVEY_MAXTIMES     0
	#define   NEEDFUNCTION_SELF_TSET         0     //自检功能
	#define   NEEDFUNCTION_VOLTMANGE         1     //电源管理功能，改变电机提前量
	#define   NEEDFUNCTION_DEALSHAKE         0     //是否需要抖动功能
	#define   NEEDFUNCTION_SHORT_AUTORECOVEY 0     //短路自动恢复功能
#else
	#define   SHORT_AUTORECOVEY_MAXTIMES     2
	#define   NEEDFUNCTION_SELF_TSET         1     //自检功能
	#define   NEEDFUNCTION_VOLTMANGE         0     //电源管理功能，改变电机提前量
	#define   NEEDFUNCTION_DEALSHAKE         1     //是否需要抖动功能 ， 此处1 只处理停止时的抖动，2 运动时也处理
	#define   NEEDFUNCTION_SHORT_AUTORECOVEY 1     //短路自动恢复功能
#endif


uint8 Motor_Init(uint16 outtime,uint8 njame,uint8 nmotor);
uint8 Motor_SelfTset(uint8 id);
void  Motor_Reset(void);
void  Motor_Control(void);
//void Motor_SetError(uint8 mode);
uint8 Motor_GetPosition(uint8 motorId);
BOOL  Motor_SetPosition(uint8 motorId,uint8 mPosition); 
uint8 Motor_GetErrorCode(uint8 motorId);
uint8 Motor_GetSeltTestCode(uint8 motorId);
uint8 Motor_GetSetPosition(uint8 motorId);
uint8 Motor_RunControl(uint8 motorId,uint8 motorState);
BOOL MotorDriveInit(uint8 u8AccData, uint8 u8BeforData);   //1.3增加此函数
#endif

