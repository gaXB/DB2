/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：InterLayMatrix.c
* 功能描述：InterLay层的变量声明文件，需要按照项目定义
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
* 此文件由PC 自动生成，PC 版本 V1.0, DBC 文件名：  CVTC_SV51-02&B1_CMX_20200409_BD_V4.81 - HCM.dbc
* 生成日期：  01/28/21 09:23:21
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：
* 完成日期：
*/
#include "InterLayMatrix.h"

/***  属性表结构  ***/ 


//发送数据变量声明
volatile CANMsg12F85351Union sCANMsg12F85351;
volatile uint8 CANMsg12F85351Status;

volatile CANMsg12F98851Union sCANMsg12F98851;
volatile uint8 CANMsg12F98851Status;

//接收数据变量声明
volatile CANMsg0EF87372Union sCANMsg0EF87372;
volatile uint8 CANMsg0EF87372Status;

volatile CANMsg12F81018Union sCANMsg12F81018;
volatile uint8 CANMsg12F81018Status;

volatile CANMsg12F85050Union sCANMsg12F85050;
volatile uint8 CANMsg12F85050Status;



//CAN2 的数据

//发送数据变量声明
volatile CANMsg410Union sCANMsg410;
volatile uint8 CANMsg410Status;

volatile CANMsg44EUnion sCANMsg44E;
volatile uint8 CANMsg44EStatus;

volatile CANMsg453Union sCANMsg453;
volatile uint8 CANMsg453Status;

volatile CANMsg454Union sCANMsg454;
volatile uint8 CANMsg454Status;

volatile CANMsg455Union sCANMsg455;
volatile uint8 CANMsg455Status;

volatile CANMsg456Union sCANMsg456;
volatile uint8 CANMsg456Status;

volatile CANMsg457Union sCANMsg457;
volatile uint8 CANMsg457Status;

volatile CANMsg551Union sCANMsg551;
volatile uint8 CANMsg551Status;

//接收数据变量声明
volatile CANMsg115Union sCANMsg115;
volatile uint8 CANMsg115Status;

volatile CANMsg131Union sCANMsg131;
volatile uint8 CANMsg131Status;

volatile CANMsg22FUnion sCANMsg22F;
volatile uint8 CANMsg22FStatus;

volatile CANMsg424Union sCANMsg424;
volatile uint8 CANMsg424Status;

volatile CANMsg432Union sCANMsg432;
volatile uint8 CANMsg432Status;

volatile CANMsg451Union sCANMsg451;
volatile uint8 CANMsg451Status;

volatile CANMsg452Union sCANMsg452;
volatile uint8 CANMsg452Status;

volatile CANMsg460Union sCANMsg460;
volatile uint8 CANMsg460Status;

volatile CANMsg470Union sCANMsg470;
volatile uint8 CANMsg470Status;

volatile CANMsg472Union sCANMsg472;
volatile uint8 CANMsg472Status;

volatile CANMsg490Union sCANMsg490;
volatile uint8 CANMsg490Status;

volatile CANMsg26DUnion sCANMsg26D;
volatile uint8 CANMsg26DStatus;
