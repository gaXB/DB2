/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：UDS_APP.h
* 1 定义PROGRAMDATA 的数据
* 2 F110 的长度和数据
* 3 DID 的结构体PID_DATA ，DID 总个数和写入的个数
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 

2019.1.17
ver0.1
1 初始版本
2 模板程序

2019.10.25
ver0.2
1 F110 DID长度和内容的宏定义模板

2021.8.23
ver0.3
1 对PD_NUM_xx 进行备注说明
*/
#ifndef UDS_APP_H_
#define UDS_APP_H_

#define  PD_NUM_INTEGRITY        0   //完整性 
#define  PD_NUM_DEPENDCE         1   //独立性
#define  PD_NUM_SESSIONMODE      2   //模式切换时候使用
#define  PD_NUM_PROCHECKH        3   //程序校验码
#define  PD_NUM_PROCHECKL        4
#define  PD_NUM_PROTIMESH        5   //刷写次数
#define  PD_NUM_PROTIMESL        6
#define  PD_NUM_EECHECK          7   //check sum 
#define  PD_NUM_EESTATE          8   //状态功能未用到

//声明的变量，外部声明，可供其他模块使用
extern uint8 ProgramData[];
extern uint8 F110SaveNumber;
extern uint8 u8ConfigData[];
#define  PID_F110_LENGTH     16        //不能超过16
extern uint8 TBL_F110ARRY[15][PID_F110_LENGTH];
extern uint8 TBL_F110[];
#define F110_DEFAULT_DATA   {0x43, 0x12, 0x34, 0x56, 0x78, 0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x01, 0x38,0x80, 0xff, 0xff}


//did 数据结构
typedef struct
{
   uint16 PID_ID;         //ID
   const uint8* pData;    //数据
   uint8  PIDLength;      //长度
}PID_DATA;

//DID 号定义
#define MAX_PID_NUM   13
#define WRITE_PID_NUM  3

extern PID_DATA PID[];
#endif /* UDS_APP_H_ */
