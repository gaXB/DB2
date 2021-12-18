/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：InterLayCAN.h
* 功能描述：InterLay层的头文件，实现此模块需要加入 CANIF层
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日

* 版本：2.0
* 说明：新LIB 的电压范围
* 原作者：xb
* 完成日期：2019年1月9日

* 版本：2.3
* 说明：跟随 .c
* 原作者：xb
* 完成日期：2019年12月24日
* 
* 版本：2.4
* 说明：跟随 .c
* 原作者：zhupan
* 完成日期：2021年08月31日
* 
*/

#ifndef INTERLAYCAN_H_
#define INTERLAYCAN_H_
#include "InterLayMatrix.h"

/******************************************************************************
发送管理的功能配置位
******************************************************************************/		
#define  _TX_USENM_   1    //是否使用网络管理，直接接间都行
#define  _TX_USEUDS_  1    //UDS 诊断功能
#define  _TX_USEVOLT_ 1    //电源管理can ，如果不带OSK 网络管理则此项一定要为1


/******************************************************************************
 //CAN 发送，及busoff相关设置
******************************************************************************/
#define   BUSOFF_CLEAR_TIME     5000         //busoff 故障多少时间后可清0。* ms
#define   CANTX_VOLT_HIGHERR    ACD_V(16.1)        //can发送电压范围
#define   CANTX_VOLT_HIGHOK     ACD_V(15.9)
#define   CANTX_VOLT_LOWERR     ACD_V(8.9)
#define   CANTX_VOLT_LOWOK      ACD_V(9.1)

#define  BUSOFF_DTC_TIMES   3            //busoff 多少次后进入BUSOFF状态

/******************************************************************************
CAN通信收发使能状态
******************************************************************************/
#define   InCAN_RX_DISABLE                     0      
#define   InCAN_RX_ENABLE                      1
#define   InCAN_PSEUDO_DISABLE                 2   //此状态时不进行测试接收超时

/******************************************************************************
CAN报文发送请求状态
******************************************************************************/
#define   InCAN_TX_DISABLE                  0
#define   InCAN_TX_ENABLE                   1

/******************************************************************************
CAN报文初始功能预留
******************************************************************************/
#define   InCAN_MODE_NORMAL                 0
#define   InCAN_MODE_INIT                   1

#define   CAN_INITMODE_TIME              100    

/***  报文状态预定义  ***/
/*  ====================================================================================
            Bit 7       |    Bit 6 ~ Bit 2   |        Bit 1       |        Bit 0       
    -----------------------------------------------------------------------------------
          CAN_TX_EN     |        Rsvd        |     CAN_UPDATE     |    CAN_MSG_LOST
                        |                    |                    |      报文丢失
          发送使能      |        保留        |    报文更新标志    |   (报文接收超时)
    ===================================================================================*/
#define   CAN_TX_EN                         0x80
#define   CAN_UPDATE                        0x02
#define   CAN_MSG_LOST                      0x01


//应用层功能函数
void InterLayTask(void);
void InterLayInit(void);
uint8 InterLay_GetRxMode(void);
uint8 InterLay_GetTxMode(void);
uint8 InCAN_GetBusOffDTC(void);
//需要CANIF 层调用的函数
void InCAN_BusOff(void);
void InCAN_TxConfirmation(void);
void InCAN_L_Data_Indication(uint32 Identifier, uint8 DLC, uint8 *Data);
void InCAN_ClearTXBuffer(void);
void InCAN_TXImmediate(uint8 u8IfId);
/*Basic INTER CallBack function for the Application*/
/*indicate nm normal*/
extern void APPlIL_RXComplete(uint32 u32Id);
extern void APPlIL_TXComplete(uint8 u8IfId);
extern void APPlIL_RecTimeOut(uint32 u32Id);
extern BOOL APPlIL_TXStart(uint8 u8IfId);
#endif /* INTERLAYCAN_H_ */
