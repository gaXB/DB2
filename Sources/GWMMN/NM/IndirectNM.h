/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：IndirectNM.h
* 功能描述：接间网络管理功能头文件，部分宏定义请按说明更改。 如果需要实现此模块，需要加入其他CAN通讯文件
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日
* 
* 版本：2.0
* 说明：跟随2.0的.c
* 作者：xb
* 完成日期：2019年9月22日
* 
* * 版本：2.1
* 说明：跟随2.1的.c
* 作者：xb
* 完成日期：2019年12月16日
*/

#ifndef INDIRECTNM_H_
#define INDIRECTNM_H_
#include "canIf.h"

#define  NM_MAINFUNCTION_PERIOD                 (2)     //IndirectNMTask 在loop调用的周期ms
#define  CANNM_TXATTRID        0        //用来发送数据使用的ID的在应用发送列表中的序号，一般选择应用报文中第一个报文，

#define   NM_SENDFF_TIME            10    //发送第一帧的时间

#define   BUSOFF_QUICKRECOVER_TIME    50   //快恢复的时间
#define   BUSOFF_SLOWRECOVER_TIME    200   //慢恢复的时间
 
typedef uint16 nmIndexType;
typedef uint8 nmNetIdType;

//本模块实现的功能
void  IndirectNMTask(void);
void	IndirectNM_Init(void);
//底层驱动，由CANIF 调用以下函数
void CanNm_BusOff(uint8 u8Chanle);
void CanNm_TxConfirmation(uint8 u8Chanle);
/*Basic NM CallBack function for the Application*/
/*indicate nm normal*/
extern void ApplNmCanStart(const nmIndexType nmChannelHandle);
/*This callback sets the communication hardware (CAN controller,bus transceiver)
* to a state from which they can be woken up by the CAN bus.*/
extern void ApplNmCanSleep(const nmIndexType nmChannelHandle);
/*indicates bus off error occured*/
extern void ApplNmBusOff(const nmIndexType nmChannelHandle);
/*indicates can bus wake up*/
extern void ApplNmBusOffEnd(const nmIndexType nmChannelHandle);
extern void ApplNmBusOffRecoverySuccess(const nmIndexType nmChannelHandle);

#define NmCanInterruptRestore()              \
   CANIF_LEAVE_CRITICAL_SECTION()
#define NmCanInterruptDisable()              \
    CANIF_ENTER_CRITICAL_SECTION()
#define NMBusErrorCanInit(NmChanneld)        \
    Can_InitController(NmChanneld, (const Can_ControllerBaudrateConfigType*)(0))

#endif /* INDIRECTNM_H_ */
