/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：CANIDConfig.h
* 功能描述：CANID 配置
* 1  诊断ID,发送ID ,接收ID 
* 2  发送IF id 定义
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
* 更改说明： 文件按照规范更改
* 此文件由PC 自动生成，PC 版本 V1.0, DBC 文件名：  CVTC_SV51-02&B1_CMX_20200409_BD_V4.81 - HCM.dbc
* 生成日期：  01/28/21 09:23:21
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年7月22日

* 版本：1.1
* 说明：增加autosar
* 原作者：xb
* 完成日期：2019年10月23日
* 
* 版本：1.2
* 说明： CAN_STANDARD_ 的宏定义在此处设置， CANDrive.H 文件中这个宏定义屏蔽 
* 作者：XB
* 完成日期：2020年7月7日

* 版本：1.2
* 说明：此文件的CAN 滤波掩码，建议工程师重新定义 
* 作者：
* 完成日期：
*/

#ifndef CANIDCONFIG_H_
#define CANIDCONFIG_H_



#define   CAN_STANDARD_    1   //标准帧

 //CAN 接收的信号定义     
#define     R_ID_1F2     0x1F2
#define     R_ID_1F5     0x1F5
#define     ID_DIAG_FUNCT   0x7DF
#define     ID_DIAG_PHYSIC   0x721

#if   (CAN_STANDARD_ ==1)
#define  ID_DIAG_FUNCT_IF           ((uint8)(ID_DIAG_FUNCT&0xff))
#define  ID_DIAG_PHYSIC_IF           ((uint8)(ID_DIAG_PHYSIC&0xff))
#else
#define  ID_DIAG_FUNCT_IF            2
#define  ID_DIAG_PHYSIC_IF           1
#endif

//CAN 接收掩码定义， 初始化函数会调用,扩展帧只有2个标准帧有4个
//写入通道1 的掩码值
#define   RECEIVE_ID1_1        0x1F2
#define   RECEIVE_ID1_2        0x1F2
#define   RECEIVE_ID1_3        0x1F2
#define   RECEIVE_ID1_4        0x1F2


#define  RECEIVE_ID1     (RECEIVE_ID1_1 & RECEIVE_ID1_2 & RECEIVE_ID1_3 & RECEIVE_ID1_4) 
#define  RECEIVE_ID1_OR   (RECEIVE_ID1_1 | RECEIVE_ID1_2 | RECEIVE_ID1_3 | RECEIVE_ID1_4) 
#define  RECEIVE_ID1_MASK   (RECEIVE_ID1 ^ RECEIVE_ID1_OR)

//写入通道2 的掩码值
#define   RECEIVE_ID2_1        0x1F5
#define   RECEIVE_ID2_2        0x1F5
#define   RECEIVE_ID2_3        0x1F5
#define   RECEIVE_ID2_4        0x1F5


#define  RECEIVE_ID2     (RECEIVE_ID2_1 & RECEIVE_ID2_2 & RECEIVE_ID2_3 & RECEIVE_ID2_4) 
#define  RECEIVE_ID2_OR   (RECEIVE_ID2_1 | RECEIVE_ID2_2 | RECEIVE_ID2_3 | RECEIVE_ID2_4) 
#define  RECEIVE_ID2_MASK   (RECEIVE_ID2 ^ RECEIVE_ID2_OR)

//写入通道3 的掩码值
#define   RECEIVE_ID3_1        0x721
#define   RECEIVE_ID3_2        0x721
#define   RECEIVE_ID3_3        0x721
#define   RECEIVE_ID3_4        0x721


#define  RECEIVE_ID3     (RECEIVE_ID3_1 & RECEIVE_ID3_2 & RECEIVE_ID3_3 & RECEIVE_ID3_4) 
#define  RECEIVE_ID3_OR   (RECEIVE_ID3_1 | RECEIVE_ID3_2 | RECEIVE_ID3_3 | RECEIVE_ID3_4) 
#define  RECEIVE_ID3_MASK   (RECEIVE_ID3 ^ RECEIVE_ID3_OR)

//写入通道4 的掩码值
#define   RECEIVE_ID4_1        0x7DF
#define   RECEIVE_ID4_2        0x7DF
#define   RECEIVE_ID4_3        0x7DF
#define   RECEIVE_ID4_4        0x7DF


#define  RECEIVE_ID4     (RECEIVE_ID4_1 & RECEIVE_ID4_2 & RECEIVE_ID4_3 & RECEIVE_ID4_4) 
#define  RECEIVE_ID4_OR   (RECEIVE_ID4_1 | RECEIVE_ID4_2 | RECEIVE_ID4_3 | RECEIVE_ID4_4) 
#define  RECEIVE_ID4_MASK   (RECEIVE_ID4 ^ RECEIVE_ID4_OR)

//CAN 发送的信号定义
#define   ID_DIAG_CLM    0x7A1       //诊断ID
#define   ID_TX1    	   0x0C9
#define   ID_TX2    	   0x1F1

//CAN 发送的信号 CANIF 的逻辑号定义
#define   IDIF_TX1      2
#define   IDIF_TX2      3

//CAN 网络管理使用说明  
#define  _NEED_OSKNM_    0
#define  _NEED_AUTOSARNM_   0
#define  _INDIRECT_NM_      1
#if      _NEED_OSKNM_ || _NEED_AUTOSARNM_
#define   CANID_NETMANGE   0x403
#define   ID_R_NM_BIEGIN   0x400
#define  ID_R_NM_END  0x4FF
#else
#define  CANID_NETMANGE  0x403
#endif

// CAN IF 层的逻辑号数量及定义
#define MAX_SEND_NUM  4
static const uint32 TBL_SENDID[MAX_SEND_NUM] ={CANID_NETMANGE, ID_DIAG_CLM, ID_TX1, ID_TX2};

#endif /* CANIDCONFIG_H_ */
