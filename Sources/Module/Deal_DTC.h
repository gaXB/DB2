/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Deal_DTC.h
* 功能描述：DTC 处理的头文件，实现此模块需要加入 LIBDriveDefine.h
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
* 更改说明： 请按照规范进行更改
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年10月26日
* 
* * 版本：2.0
* 说明：由Begin_DTCCycle/End_DTCCycle 由本模块实现，外部不需要调用
* 原作者：xb
* 完成日期：2019年10月29日
*/



#ifndef DEAL_DTC_H__
#define DEAL_DTC_H__

#include "LIBDriveDefine.h"

//项目需要实现的DTC 定义，需要按照项目更改
#define  DTCNUM_TIN          0               
#define  DTCNUM_TAMB         1             
#define  DTCNUM_TEVP         2        
#define  DTCNUM_PRESS        3

#define  DTCNUM_MDMOTOR_BACKERR         4   
#define  DTCNUM_MIXMOTOR_BACKERR        5        
#define  DTCNUM_NCFMOTOR_BACKERR        6      
#define  DTCNUM_VOLTAGE_HIGH            8         
#define  DTCNUM_VOLTAGE_LOW             7        
#define  DTCNUM_EMSCAN_NORESPONE        9   
#define  DTCNUM_IPKCAN_NORESPONE        10    
#define  DTCNUM_ESCCAN_NORESPONE        11   
#define  DTCNUM_BCMCAN_NORESPONE        12 
#define  DTCNUM_ICGMCAN_NORESPONE       13   
#define  DTCNUM_FICMCAN_NORESPONE       14 
#define  DTCNUM_TCUCAN_NORESPONE        15 
#define  DTCNUM_LINACSW_NORESPONE       16
#define  DTCNUM_BUSOFF                  17 
#define  DTC_MAXNUM                     18

//dtc 判断故障的电压
#define   DTCVOLT_INIT_DELAY            1500        //电压启动延时 ms
#define   DTCVOLT_RECOVER_DELAY         500        //电压从故障到恢复的时间 ms
#define   DTC_LVOLT_DISABLE             ACD_V(9)
#define   DTC_LVOLT_ENABLE              ACD_V(9.5)
#define   DTC_HVOLT_DISABLE             ACD_V(16)
#define   DTC_HVOLT_ENABLE              ACD_V(15.5)

//DTCNUM_VOLTAGE_HIGH /LOW 判断
#define   DTC_EVOLT_ADID                ADID_VOLT       //判断电压故障码的ADID来源 
#define   DTC_EVOLT_LOW_L               ACD_V(5)        //在此范围内低压故障
#define   DTC_EVOLT_LOW_H               ACD_V(9)
#define   DTC_EVOLT_LOW_OK              ACD_V(9.5)      //L恢复电压
#define   DTC_EVOLT_HIGH                ACD_V(16)     //在此范围内高压故障
#define   DTC_EVOLT_HIGH_OK             ACD_V(15.5)       //H恢复电压
#define   DTC_EVOLT_TIME                1300            //故障持续时间为此值
//dtc snap 结构体定义，当项目有其他需要时需要更改
typedef struct
{
	uint8  DTCAgeTimes;  //32bit 
	uint8  DTCPendTimes;
	uint8  u8Volt;   // *0.1
	uint8  IgnState;
	uint8  u8WaterTemp;  // 40
	uint32 u32ODO;    //odometer
}DTCSNAPDATA;
//DTC 读取参数定义， 一般不需要更改
#define  DTCDEAL_TIME   8                // dtc采集时间  *ms
#define  DTC_FAILED_TIMES      30        // 故障限制
#define  DTC_PASSED_TIMES      -30		  // 正确限制
#define  DTC_FAILED_STEP       2         // 每次错误+的值 
#define  DTC_PASSED_STEP       1         // 每次正确-的值 
#define  DTC_AGE_TIMES         10        //老化次数
#define  _SAVE_DTC_            1         //增加dtc 改变需要存储时置为1，为0则需要掉电记忆
#define  SAVE_CYC_TIMES        250       //当SAVE_DTC == 1 时有效，表示 记忆间隔时间不大于SAVE_CYC_TIMES *DTCDEAL_TIME， 最大可设到65535
#define  DTC_CONFIRM_TIMES     2         //历史故障需要循环的次数

//以下不能改
//DTC 每一位的定义
#define  DTCB_TESTFAILED                   0x01
#define  DTCB_FAILED_THISOC                0x02
#define  DTCB_PENDING                      0x04
#define  DTCB_CONFIRM                      0x08
#define  DTCB_TESTNOTCOMPLETED_SINCECLEAR  0x10
#define  DTCB_TESTFAILED_SINCECLEAR        0x20
#define  DTCB_TESTNOTCOMPLETED             0x40

//GetDTCState 返回值定义
typedef enum
{
        DTC_RETSTATE_OK = 0,             //无故障
        DTC_RETSTATE_ERR = 1,            //有故障
        DTC_RETSTATE_WAITE = 2,          //故障等待，即不判断
        DTC_RETSTATE_IMMEDI_OK = 0x80,   //立即无故障
        DTC_RETSTATE_IMMEDI_ERR  = 0x81,  //立即有故障
        DTC_RETSTATE_COMFIRM_ERR = 0xC1  //立即确认为历史故障
}eDTCRETSTATE;



//外部可调用变量
extern DTCSNAPDATA DTCSnap[DTC_MAXNUM];
extern uint8 DTC[DTC_MAXNUM];
#define DTCTFAILED(DtcNum)    (DTC[DtcNum] & DTCB_TESTFAILED)

//本模块实现函数
uint8 Deal_DTC(void);
BOOL Init_DealDTC(uint8 MaxNum);
void ClearDTC(uint8 nId);
//外部应用函数实现的函数
extern eDTCRETSTATE GetDTCState(uint8 DtcNum);    //查看DTC的状态，由应用程序提供
extern void ReCord_DTCSnap(uint8 num);
#endif
