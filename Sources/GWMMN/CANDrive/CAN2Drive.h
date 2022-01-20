/************************文件说明********************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：CANDrive.h
* 功能描述：CANDrive， CANDriveWake 的头文件，为实现此此功能需要以下文件
* 1 GALIB 库文件（derivative.h mcu底层驱动文件）
* 2 CanIf.h  及common 头文件
* 3 CANIDConfig.h  应用CAN ID 配置文件
* MCU说明：keaz128 此文件中跟MCU 相关。
* 更改说明： 部分宏定义需要按照规范要求更改
*/
/**************************更改记录********************************************
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日
* 
* * 版本：2.0
* 说明：增加CAN_STANDARD_ 宏定义
* 原作者：xb
* 完成日期：2019年7月22日
* 
* * * 版本：4.0
* 说明：增加EnableCanErrTxInterrupt 
* DisableCanErrTxInterrupt 函数
* 2 对用4.0 .c 文件
* 原作者：xb 
* 完成日期：2019年9月22日
*
* * * 版本：4.1
* 1 对用4.1 .c 文件
* 原作者：xb 
* 完成日期：2019年12月24日


* * * 版本：4.2
* 1 CAN_STANDARD_      此宏定义由CONFIG定义
* 原作者：xb 
* 完成日期：2020年7月8日
*/


#ifndef CAN2DRIVE_H_
#define CAN2DRIVE_H_

#include "LIBDriveDefine.h"
#include "CANIDConfig.h"
#include "CanIf.h"

//#define   CAN_STANDARD_     1    //标准帧为1
#define   _CAN2WAKE_FUN_     1      //CAN唤醒功能是否需要
#define   CAN2UNWAKE_TIME    50     //*100ms   多少时间CAN没唤醒则认为已没有CAN 信号可以休眠

#define   O_CAN2EN   GPIOA_PTD1     //如果没有CANWAKE 功能则设置为GPIO_UNUSE,也可以更改CANDrive_SleepIO
#define   CAN2WAKE_KBI_NUM       0    // kbi 的号及通道，如无此功能，可随意定义
#define   CAN2WAKE_KBI_CHANEL    27

/*************以下不能更改*************************************************/
//can 错误和接收中断控制宏定义
#define EnableCan2ErrTxInterrupt()      NVIC_DisableIRQ(MSCAN_TX_IRQn)
#define DisableCan2ErrTxInterrupt()     NVIC_EnableIRQ(MSCAN_TX_IRQn)
#define DisableCan2RxInterrupt()			MCU_NOP//CAN_ReceiverFullIntDisable(MSCAN)
#define EnableCan2RxInterrupt()        MCU_NOP// CAN_ReceiverFullIntEn(MSCAN)

//can 相关功能函数
void CAN2Driver_BusOffInit(void);
void CAN2Driver_ClearTransmitBuffer(void);
BOOL CAN2Drive_TxBufferEmptemp(void);
BOOL CAN2Drive_SendFram(uint32 id, uint8* u8SendData);
void CAN2Drive_Close(void);
void CAN2Drive_SleepIO(uint8 En);
void Init_Mscan2(void);

//wakeup 相关功能函数
uint8 GetCan2WakeEn(void);
void Deal_CAN2Wake(void);
void CAN2Wake_Begin(uint8 bEN);
void CAN2Get_Fram(void);
void CAN2Drive_GotoAWake(void);
void CAN2Drive_GotoSleep(void);

#endif /* CAN2DRIVE_H_ */
