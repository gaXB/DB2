/**************************文件说明*****************************************************
* Copyright (c) 2021 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：LIBDriveDefine.h
* 功能描述：底层驱动头文件整合，和对LIB的驱动号进行定义便于应用使用
* 1  galib
* 2  stepmotor
* 3  MCU_PORT
* 4  DTC
* 5  SYSTEMSAVE
* 6  CAN
* 7  显示输出驱动
* 编译器： codewarrior IDE 11.1
* MCU说明：此文件跟MCU 无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
* 
* 版本：2.0
* 说明：增加内外版本的键值
* 原作者：xb
* 完成日期：2020年6月3日
*/
#ifndef LIBDRIVEDEFINE_H_
#define LIBDRIVEDEFINE_H_

#include "Galib.h"
//#include "Stepmotor.h"
//#include "Drv8860_Stepmotor.h"
#include "HardWareDrive.h"
#include "MCU_PORT.h"
#include "MoudleDrive.h"
#include "SystemSaveFlash.h"
#include "GlobalDefine.h"
#include "Deal_DTC.h"
#include "OutANDDisplay.h"
#include "Signal_Refresh.h"

#if  _PROJECT_CAN_
#include "ApplayLay.h"
#include "IndirectNM.h"
#include "InterLayCan.h"
#include "UDS_APP.h"
#include "printf.h"
#endif

/*******************************************************************************
 *TODO: AD 号定义
 *
********************************************************************************/
#define ADID_VOLT       0
#define ADID_BATT       1     //和采集点火
#define ADID_FAN        2     //鼓风机
#define ADID_TIN        3      //车内
#define ADID_TAMB       4     //车外
#define ADID_TEVP       5     //蒸发
#define ADID_FVENT      6
#define ADID_DVENT      7
#define ADID_SUN        8     //阳光
#define ADID_PRESS      9    //压力传感器
#define ADID_MOTORMIX    10
#define ADID_MOTORMODE   11
#define ADID_MOTORNCF    12
//初始化配置字
#define ADID_MAXNUM      13
#define ADID_VNUM        3
#define ADID_TNUM        5
#define ADID_SYSIGN      ADVOLT_MODE_12V



/*******************************************************************************
 * TODO: 电机模块电机号分配
 *
********************************************************************************/
#define  MOTOR_ID_MIX      0
#define  MOTOR_ID_MODE     1
#define  MOTOR_ID_NCF      2

#define  NCFMOTOR_ID_NCF   0
//步进电机，电机号定义
/*#define  STEPMOTOR_ID_MODE_OSF     0
#define  STEPMOTOR_ID_MODE_DEF     1
#define	STEPMOTOR_ID_MIX          2
#define	STEPMOTOR_ID_MIX_ASSIANT  3
#define	STEPMOTOR_ID_NCF          4

#define  MODE_OSF_MAXSTEP         1500
#define  MODE_DEF_MAXSTEP         2000
#define	MIX_MAXSTEP              2000
#define	MIX_ASSIANT_MAXSTEP      3000
#define	NCF_MAXSTEP              3000

#define   BACK_STEP               48*/
/*******************************************************************************
 * TODO: 7708模块电机号分配
 *
********************************************************************************/
#define  MOTOR7708_ID_MIX     0
#define  MOTOR7708_ID_MODE    1
#define  MOTOR7708_ID_NCF     2

/*******************************************************************************
 * TODO: 编码器定义
 *
********************************************************************************/
#define  CODE_NUM_TSET       0
#define  CODE_NUM_FAN        1
#define  CODE_NUM_MAX     2


/*******************************************************************************
 *TODO:  按键宏定义
 *
********************************************************************************/
#define  KEY_DIAG       0x1800u
#define  KEY_AUTO       0x1000u    
#define  KEY_ONOFF      0x0800u
#define  KEY_CIRF       KEY_VER_H   //高位，固定键值为0x0002u，不可更改，其他键值定义需避开
#define  KEY_AC      	KEY_VER_L   //低位，固定键值为0x0001u，不可更改，其他键值定义需避开
#define  KEY_FANUP    	0x0004u
#define  KEY_FANDOWN    0x0020u
#define  KEY_FRE        0x2000u    //外循环
#define  KEY_MODE     	0x0008u
#define  KEY_FAN        0x8000u
#define  KEY_DEF       	0x0010u
#define  KEY_TEMP       0x6000u
#define  KEY_AIRCLEAN   0x0040u
#define  KEY_AIRCLEAN_LEVE 0x0060u
#define  KEY_AIRCLEAN_AUTO_ON 0x0400u
#define  KEY_AIRCLEAN_AUTO    0x0600u
#define  KEY_RHEAT    	0x0100u
#define  KEY_CHEAT_R    0x0200u
#define  KEY_CHEAT_L    0x0080u

#define  KEY_VER_SLEF_LCD  		KEY_CODE_COMBFOUR_VERH      //按住低位 4击高位 作为辅助LCD显示低位值
#define  KEY_VER_SLEF           	KEY_CODE_COMBFOUR_VERL      //按住高位 4击低位 显示内部版本号，
#define  KEY_CLIENT_VER        	KEY_VERH_LONG            //内循环长按5s

#define  KEY_OFF        KEY_ONOFF
#define  COMD_FAN_ADD           2
#define  COMD_FAN_DEC           1 
#define  COMD_TSET_ADD         16
#define  COMD_TSET_DEC         15


/*******************************************************************************
 * 标定参数个数定义
********************************************************************************/
#define   CAILBRATION_RAMDATA_NUM    200
#define   CAILBRATION_PARAMETER_NUM    240 

#endif /* LIBDRIVEDEFINE_H_ */


