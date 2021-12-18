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
volatile CANMsg0C9Union sCANMsg0C9;
volatile uint8 CANMsg0C9Status;

volatile CANMsg1F1Union sCANMsg1F1;
volatile uint8 CANMsg1F1Status;

//接收数据变量声明
volatile CANMsg1F2Union sCANMsg1F2;
volatile uint8 CANMsg1F2Status;

volatile CANMsg1F5Union sCANMsg1F5;
volatile uint8 CANMsg1F5Status;

