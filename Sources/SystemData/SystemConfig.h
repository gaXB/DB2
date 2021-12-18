/************************文件说明*****************************************************
* Copyright (c) 2019,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SystemConfig.h
* 功能描述：系统配置字：
* 1 是否为双温区
* 2 系统温度换算定义
* 3 温度默认值
* 4 设置温度范围，风量范围
* 此文件会为模板，按照系统应用要求更改，和MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月15日
*/

#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_

//系统设定
#define   DOUBLE_CLM       0  //是否为双温驱程序

/*******************************************************************************
 *系统温度换算定义
 *
********************************************************************************/
#define  SCD_T(temp)          ((int16)((temp) *10))             //温度转换的方式
#define  DCS_T(TempData)      (( (TempData) + (TempData)%10)/10)   // 10位的温度转化为实际温度
#define  SCD_DIFFT(DiffTemp)   ((int16)((DiffTemp) *10))   

/******************************************************************************
 *系统电压换算定义
********************************************************************************/
#define  SCD_V(volt)          ((int16)((volt) *100))  

/*******************************************************************************
 * 系统故障时的默认温度
 *
********************************************************************************/
#define  _TSET_DEFAULT_     SCD_T(24)
#define  _TAMB_DEFAULT_     SCD_T(20)
#define  _TIN_DEFAULT_      SCD_T(25)
#define  _TEVP_DEFAULT_     SCD_T(-2)
#define  _VENT_DEFAULT_     SCD_T(25)
#define  _FOOT_DEFAULT_     SCD_T(25)
#define  _TWATER_DEFAULT_   SCD_T(90)        // 水温默认80
/*******************************************************************************
 * HP SENSOR 的默认值
 *
********************************************************************************/
#define  _TPRESS_DEFAULT_     SCD_T(25)
#define  _PRESS_DEFAULT_      2000
#define  _TPTC_DEFAULT_       SCD_T(100)
#define  _TOIPTC_DEFAULT_       SCD_T(100)
/*******************************************************************************
 * 设置温度及风量定义
 *
********************************************************************************/
#define  TSET_LO        SCD_T(17)       //最低为17
#define  TSET_HI        SCD_T(32)       //最高为32
#define  TSET_STEP      SCD_T(1)        //旋钮每次温度改变
//将设置温度转换成CAN信号
#define  _TT05(Data)    ((Data)/10 + 14)   //设置温度转换
#define  TSET_ZERO      0               //温度值0
#define  MAX_FELVL      8               //最大风挡
#define  MIN_FELVL      1               //最小风挡 非off

/*******************************************************************************
 * 配置字
 *
********************************************************************************/
#define BUSCONFIG_RCP_MASK     0X01    //SUV 默认为1
#define BUSCONFIG_APM_MASK     0x02    
#define BUSCONFIG_ATC_MASK     0x04
#define BUSCONFIG_FHM_MASK     0x08

#define BUSCONFIG_USE_RCP      (!(u8ConfigData[0]&BUSCONFIG_RCP_MASK))    //为1 为配置
#define BUSCONFIG_USE_APM      (!(u8ConfigData[0]&BUSCONFIG_APM_MASK))
#define BUSCONFIG_USE_ATC      (!(u8ConfigData[0]&BUSCONFIG_ATC_MASK))
#define BUSCONFIG_USE_FHM      (!(u8ConfigData[0]&BUSCONFIG_FHM_MASK))

#endif /* SYSTEMCONFIG_H_ */
