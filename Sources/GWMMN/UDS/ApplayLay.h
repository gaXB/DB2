/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：ApplayLay.h
* 功能描述：网络层的头文件，实现此模块需要加入 netlay, systemsave,dealdtc
* 此文件不需要更改
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日

* 版本：1.1
* 说明：加入预编译RC
* 原作者：xb
* 完成日期：2019年10月25日

* 版本：1.2
* 说明：
* 1 CAILBRATION_RAMDATA_NUM/CAILBRATION_PARAMETER_NUM 标定参数个数放在LIBDriveDefine.h
* 2 DID_MAXNUM 直接定义为250
* 作者：xb
* 完成日期：2020年9月27日
*/


#ifndef  __APPLAY_LAY_H__
#define  __APPLAY_LAY_H__
#include  "NetLaye.h"
/*******************************************************************************
 * 应用层定时宏定义
 *
********************************************************************************/

#define  P2CAN_Sever    50
#define  P2CAN_Sever1   500      //本系统中不用  除以10
#define  S3_Sever       5000

//服务定义，及子功能定义
#define  SEVER_SID_SESSIONCONTROL      0X10
//子功能
#define  SESSIONCONTROL_TYPE_RESVER    0x00
#define  SESSIONCONTROL_TYPE_DS        0x01
#define  SESSIONCONTROL_TYPE_PRGS      0x02
#define  SESSIONCONTROL_TYPE_EXTDSE    0x03
#define  SESSIONCONTROL_TYPE_EOLDS     0x04
// reset sever
#define  SEVER_SID_RESET      0x11
#define  RESET_TYPE_HARDRESET 0x01
#define  RESET_TYPE_KOFFONR   0x02
#define  RESET_TYPE_SOFTRESET  0x03
//security sever
#define  SEVER_SID_SECURITY   0x27
#define  SECURITY_REQSD_LEV1  0x01
#define  SECURITY_REQSD_LEV2  0x05
#define  SECURITY_SENDK_LEV1  0x02
#define  SECURITY_SENDK_LEV2  0x06
#define  SECURITY_REQSD_LEV3  0x09
#define  SECURITY_SENDK_LEV3  0x0a


#define  SEVER_SID_TP         0X3E     //TEST PRESENT   
#define  SEVER_SID_COMMUNICATIONCONTROL   0x28      //COMM CONTROL
//子功能
#define  CC_TYPE_ERXTX    0x00
#define  CC_TYPE_ERXDTX   0x01
#define  CC_TYPE_DRXETX   0x02
#define  CC_TYPE_DRXTX    0x03

//DTCCONTROL
#define  SEVER_SID_DTCCONTROL       0x85

//子功能
#define  DTCCONTROL_TYPE_STORON     0x01
#define  DTCCONTROL_TYPE_STOROFF    0x02

#define  SEVER_SID_READDATABYINF    0x22
#define  SEVER_SID_WRITEDATABYINF   0x2E
#define  SEVER_SID_IOCONTROL        0x2F

#define  SEVER_SID_READBYADDR       0x23
#define  SEVER_SID_WRITEBYADDR       0x3D

#define  SEVER_SID_CLEARDIAGINF     0x14

#define  SEVER_SID_READDTC    0x19
//子功能
#define  READDTC_SID_NUM      0X01
#define  READDTC_SID_READ     0X02
#define  READDTC_SID_SNAPSHORT  0x04
#define  READDTC_SID_ALLREAD  0x0A


#define  SEVER_SID_RC      0x31   //routine control

#define  RC_SID_START      0x01
#define  RC_SID_STOP       0x02
#define  RC_SID_RESULT     0x03


#define  FUN_NEED_PRECONDITIONS     1
#define  ROUTINE_ID_PRECONDITIONS  0xFF02
#define  ROUTINE_ID_ERASE          0xFF00
#define  ROUTINE_ID_INTEGRITY      0xDFFF
#define  ROUTINE_ID_DEPENDENCE     0xFF01


#define  SEVER_SID_REQDOWNLOAD      0x34
#define  SEVER_SID_TRANSDATA      0x36
#define  SEVER_SID_EXITTRANS      0x37
//服务定义结束


//DTC 相关定义
#define  DTC_HIGHTBYTE     0x96      //AC默认为96
#define  DTC_MIDLLE_TYPE   0X00      //00--0a 为DTC
#define  DTC_LOWBTYE       0x00      //没用  0x00
#define  SUPPORT_DTC       0x4B        //





//否定响应码
#define  NRC_SNS        0x11
#define  NRC_SFNS       0x12
#define  NRC_IMLOIF     0x13   //格式不对
#define  NRC_CNC        0X22   //子功能为保留值，或者条件不满足
#define  NRC_ROOR       0x31
#define  NRC_SFNSIAS    0x7e   //subFunctionNotSupportedInActiveSession
#define  NRC_SNSIAS     0x7f   //serviceNotSupportedInActiveSession
#define  NRC_78         0x78


#define  NRC_RSE        0x24   //requestSequenceError
#define  NRC_SAD        0x33   //securityAccessDenied
#define  NRC_IK         0x35   //invalid key
#define  NRC_ENOA       0x36   // exceededNumberOfAttempts
#define  NRC_RTDNE      0x37   // requiredTimeDelayNotExpired
#define  NRC_WBSC       0x73   //wrongBlockSequenceCounter
#define  NRC_GPF        0x72   //generalProgrammingFailure

#ifdef  __NRC22__    //不在范围内的反馈是22和12 的宏定义 
#define NRC_CNC1   0x22   
#else
#define NRC_CNC1   0x12
#endif


//发送状态定义
#define  APP_STATE_IDLE      0
#define  APP_STATE_RESPONE   1
#define  APP_STATE_DEALREQUEST    2




#define  SESSION_MODE_DEFAULT     0       //default session
#define  SESSION_MODE_PROGRAM     2       //program session
#define  SESSION_MODE_EXTERN      3

#define  SECURITY_STATE_LOCK      0      //LOCK STATE INIT IS this
#define  SECURITY_STATE_SSEED1    1      //发送1种子状态
#define  SECURITY_STATE_LEVEL1    2      //等级1解锁
#define  SECURITY_STATE_SSEED2    3      //发送2种子状态 
#define  SECURITY_STATE_LEVEL2    4      //等级2解锁
#define  SECURITY_STATE_TIMELOCK  5      //访问10s 后才能解锁  


#define   APPDATA_MAXLENGTH        255
typedef struct
{
   uint8  A_AI;                        //address idefine
   uint8  A_PCI;                       //pci is sid
   uint8  A_Data[APPDATA_MAXLENGTH];    //?
   uint16  A_Length;                   //lenth a_data's length +1
}A_PDU;


#define   IOCONTROL_STATE_LENGTH  2     //2F sever 上汽规定状态字固定为2

/*******************************************************************************
 * DID最大个数定义
 *
********************************************************************************/
#define	DID_MAXNUM  250

/**********************************************************************

   总线状态相关定义  ,  NM_IND(XXX) 
   ************************************************************************/

/***********************************************************************
应用层可调用函数
**********************************************************************/
void Diagnostic_IgnReset(void) ;
void Enter_DefaultSession(void);
void AppLay_DiagControl(void);
BOOL Diag_GetIsDiag(void);
uint8 Diag_GetSession(void);
uint8 NmControlEnGet(void);
/***********************************************************************
需要调用外部函数声明
**********************************************************************/
extern void ClearDTC(uint8 nId);
extern void ApplUDS_GotoProgramMode(void);
extern void ApplUDS_Reset(void);	
#endif
