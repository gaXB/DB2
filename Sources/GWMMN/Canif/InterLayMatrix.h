/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：InterLayMatrix.h
* 功能描述：InterLay层数据发送，默认值，timeout值，发送接收表的定义。请按规范编写
* 编译器： codewarrior IDE 11.1 
* MCU说明：此模块为上层应用，和MCU 无关
* 
*此文件由PC 自动生成，PC 版本 V1.0, DBC 文件名：  CVTC_SV51-02&B1_CMX_20200409_BD_V4.81 - HCM.dbc
* 生成日期：  01/28/21 09:23:21
*/
/**************************更改记录******************************************************
 *
* 版本：1.0
* 说明：初始版本建立
* 原作者：
* 完成日期：
*/
#ifndef _INTERLAY_MATRIX_H_
#define _INTERLAY_MATRIX_H_
#include "timerout.h"
#include "CANIDConfig.h"


//CAN发送数据结构体
typedef struct
{
           uint8   IfID;            //if层ID
  volatile uint8   *pMsg;        //报文名称(报文存储地址)
           uint16  Cycle;        //报文发送周期(发)
  volatile uint8   *pStatus;     //报文状态
  const    uint8   *pInit;       //报文初始化值
}CANMsgTXAttrStruct;

//CAN接收数据结构体
typedef struct
{
           uint32  ID;           //接收为报文ID
  volatile uint8   *pMsg;        //报文名称(报文存储地址)
           uint16  Cycle;        //报文超时周期(收)
  volatile uint8   *pStatus;     //报文状态
  const    uint8   *pInit;       //报文初始化值
  const    uint8   *pTimeOut;    //报文掉线值
  const    uint8   *pMask;       //报文屏蔽位(决定某一位在掉线时是否更新为掉线值)
}CANMsgRXAttrStruct;

///初始化为0，不改变的值，如果没有定义可以先用这个为默认值
static const uint8  CANMsgZEROInit[8]    = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8  CANMsgUNCTimeout[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8  CANMsgUNCMask[8]    = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static const uint8  CANMsgZEROMask[8]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//
//MSG名称:  GW_VCU_0C9h
//MSGC9 定义
typedef union{ 
 	struct
	{
		unsigned	Reserved_0  :8;	 //预留_0
		unsigned	Reserved_1  :8;	 //预留_1
		unsigned	Reserved_2  :8;	 //预留_2
		unsigned	Reserved_3  :8;	 //预留_3
		unsigned	Reserved_4  :8;	 //预留_4
		unsigned	Reserved_5  :8;	 //预留_5
		unsigned	Reserved_6  :3;	 //预留_6
		unsigned	PrplsnSysAtv  :1;	 //Power Status: Ready or Not Ready法规强制（Propulsion System Active） 
		unsigned	Reserved_7  :4;	 //预留_7
		unsigned	Reserved_8  :8;	 //预留_8
	}Bits;
	uint8 data[8];
}CANMsg0C9Union;
extern volatile CANMsg0C9Union sCANMsg0C9;
extern volatile uint8 CANMsg0C9Status;
#define TXIFID_0C9   IDIF_TX1
static const uint8  CANMsg0C9Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG名称:  BCM_1F1h
//MSG1F1 定义
typedef union{ 
 	struct
	{
		unsigned	SysPwrMd  :2;	 // System Power Mode 
		unsigned	Reserved_0  :2;	 //预留_0
		unsigned	SysOpnlMd  :3;	 // System Operational Mode先预留，默认发0 
		unsigned	SysVolMdV  :1;	 //  
		unsigned	Reserved_1  :8;	 //预留_1
		unsigned	Reserved_2  :8;	 //预留_2
		unsigned	Reserved_3  :8;	 //预留_3
		unsigned	SysVolV  :1;	 //  
		unsigned	Reserved_4  :7;	 //预留_4
		unsigned	SysVol  :8;	 // Battery Voltage 
		unsigned	Reserved_5  :8;	 //预留_5
		unsigned	Reserved_6  :1;	 //预留_6
		unsigned	SysVolMd  :2;	 // System Voltage Mode （9-16V） 
		unsigned	Reserved_7  :5;	 //预留_7
	}Bits;
	uint8 data[8];
}CANMsg1F1Union;
extern volatile CANMsg1F1Union sCANMsg1F1;
extern volatile uint8 CANMsg1F1Status;
#define TXIFID_1F1   IDIF_TX2
static const uint8  CANMsg1F1Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//以下为接收信号

//MSG名称:  GW_IPK_1F2h
//MSG1F2 定义
typedef union{ 
 	struct
	{
		unsigned	Reserved_0  :4;	 //预留_0
		unsigned	SysBPM  :2;	 //System Backup Power Mode  
		unsigned	SysBPMEnbd  :1;	 //System Backup Power Mode Enabled  
		unsigned	Reserved_1  :1;	 //预留_1
		unsigned	Reserved_2  :8;	 //预留_2
		unsigned	Reserved_3  :8;	 //预留_3
		unsigned	Reserved_4  :8;	 //预留_4
		unsigned	Reserved_5  :8;	 //预留_5
		unsigned	Reserved_6  :8;	 //预留_6
		unsigned	Reserved_7  :8;	 //预留_7
		unsigned	Reserved_8  :8;	 //预留_8
	}Bits;
	uint8 data[8];
}CANMsg1F2Union;
extern volatile CANMsg1F2Union sCANMsg1F2;
extern volatile uint8 CANMsg1F2Status;
#define R_ID_1F2   0x1F2
static const uint8  CANMsg1F2Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG名称:  GW_VCU_1F5h
//MSG1F5 定义
typedef union{ 
 	struct
	{
		unsigned	Reserved_0  :8;	 //预留_0
		unsigned	Reserved_1  :8;	 //预留_1
		unsigned	Reserved_2  :8;	 //预留_2
		unsigned	TrShftLvrPos  :4;	 //Transmission Shift Lever Position（Forward Range A为D档状态）tbox法规项 
		unsigned	TrShftLvrPosV  :1;	 //Transmission Shift Lever Position Validity 
		unsigned	Reserved_3  :3;	 //预留_3
		unsigned	Reserved_4  :8;	 //预留_4
		unsigned	Reserved_5  :8;	 //预留_5
		unsigned	Reserved_6  :8;	 //预留_6
		unsigned	Reserved_7  :8;	 //预留_7
	}Bits;
	uint8 data[8];
}CANMsg1F5Union;
extern volatile CANMsg1F5Union sCANMsg1F5;
extern volatile uint8 CANMsg1F5Status;
#define R_ID_1F5   0x1F5
static const uint8  CANMsg1F5Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};




//定义发送矩阵表
static const CANMsgTXAttrStruct CANMsgTX_AttrTable[] =
{
	{ TXIFID_0C9,  sCANMsg0C9.data,  10,  &CANMsg0C9Status,  CANMsg0C9Init},
	{ TXIFID_1F1,  sCANMsg1F1.data,  50,  &CANMsg1F1Status,  CANMsg1F1Init},
};
#define   CANMSG_TXATTR    (sizeof(CANMsgTX_AttrTable)/sizeof(CANMsgTXAttrStruct))


//定义接收矩阵表
static const CANMsgRXAttrStruct CANMsgRX_AttrTable[] =
{
	{ R_ID_1F2,  sCANMsg1F2.data,  500,  &CANMsg1F2Status,  CANMsg1F2Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_1F5,  sCANMsg1F5.data,  250,  &CANMsg1F5Status,  CANMsg1F5Init,  CANMsgUNCTimeout, CANMsgUNCMask},
};
#define   CANMSG_RXATTR    (sizeof(CANMsgRX_AttrTable)/sizeof(CANMsgRXAttrStruct))

#endif
