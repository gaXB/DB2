/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：NetLaye.h
* 功能描述：网络层的头文件，实现此模块需要加入 CANIF层
* 部分宏定义需要更改
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日

 * 
* 版本：0.5
* 说明：跟随.c文件得版本
* 作者：xb
* 完成日期：2019年10月25日
*/

#ifndef NET_LAY_H__
#define NET_LAY_H__
#include "CanIf.h"


//定时参数
#define     TIMER_N_AR     70   //物理层发送的时间
#define     TIMER_N_AS     70
#define     TIMER_N_BS     150 // 发送放接受流控制帧的时间
#define     TIMER_N_CR     150 // 接受放接受连续帧的时间

//网络层参数
#define  NETLAY_BS      8     //fc  continue times
#define  NETLAY_STmin   20    // continue fram  send time must small than it
#define  NETLAY_N_WFTMAX      0  // The FC Receive times must small than it

//配置
#define  NETLAY_MAXLENGTH    255   //定义网络层所能接受的最大长度,和最大个数相关，支持dtc12 12*4+3=51
#define  MAX_NPDU_BUFFER     3   //网络层处理物理层的缓存
// result define
#define  N_Success      0
#define  N_Timeout_A    1
#define  N_Timeout_Bs   2
#define  N_Timeout_Cr   3
#define  N_Wrong_SN     4
#define  N_Invalid_FS   5   //非法的流控制帧
#define  N_Buffer_Overflow 6  
#define  N_UNEXP_PDU       7   //接收到非预期的PDU，接收方应忽略该报文；
#define  N_WFT_OVRN        8   //等待流控制帧数错误
#define  N_ERROR           9

//result of ChangeParameter
#define  N_Success                  0
#define  N_Undefined_Parameter      1
#define  N_Wrong_Value              2
#define  N_Busy                     3



#define   AID_PHYSIC     ID_DIAG_PHYSIC_IF
#define   AID_FUNIC      ID_DIAG_FUNCT_IF
#define  CANDIAG_IFID                         1       //can if 层的ID

 
/*******************************************************************************
 *数据结构定义
 *
********************************************************************************/
#define  N_AI     uint8
#define  N_DATA   uint8

//netlay fram pci struct
typedef  struct
{
   unsigned    Length_SN  :4;
   unsigned    nPciType   :4;   //    
}N_PCI;

//netlay protect data unit struct
typedef struct
{
   N_AI  nId;
   N_PCI Pci;
   N_DATA   Data[7];
}N_PDU;

//发送接受数据的 数据结构
typedef  struct
{
   N_AI     nId;             //发送时为 目标ID,接受时为发送者的ID
   uint8    Data[NETLAY_MAXLENGTH];
   uint16   Length;
   uint8    N_Result;
}N_USDATA_IND;


/// n_usdata_con to app  struct
typedef  struct
{
   N_AI     nId;
   uint8    N_Result;
}N_USDATA_CON;


//发送1帧的数据结构
typedef struct
{
   uint8    S_State;   //发送状态
   uint16   S_Length;  //长度
   uint8    S_mId;     //目标ID
   uint8    S_SN;     //SN 
   uint8    S_BS;       //BS 
   uint16    S_STmin;    //stmin 
   uint16   S_TimerSend;    
} N_SendData;


//接受1帧的数据结构
typedef struct
{
   uint8    R_State;   //接受状态
   uint16   R_Length;  //长度
   uint8    R_mId;     //接受的ID
   uint8    R_SN;     //SN 
   uint8    R_BS;       //BS 
   uint16    R_STmin;    //stmin   
} N_RecData;
/**********************************************************************
函数声明 
************************************************************************/

//外部调用函数
void Send_78NRC(uint8 SID);
//本模块实现，提供外部调用函数
uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target);
void  Deal_UDSNetLay(void);
#endif
