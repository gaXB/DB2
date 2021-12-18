/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：ADTreat.h
* 功能描述：AD 模块的头文件。
* 为此模块能正常工作需要加入 需要定时器模块及数学函数处理模块
* 
* 编译器： codewarrior IDE 11.1-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年5月14日
* 
* 版本：1.1
* 说明：
* 修改 BOOL ADInit(uint8 u8MaxNum, uint8 u8VoltRegisterType) 来调用不同的电阻值类型
* 0 : 10/3.6 改为 1 : 100/33 
* 100/33 为1
* 原作者：xb
* 完成日期：2020年3月28日
* 
*/


#ifndef READ_AD__
#define READ_AD__

#include "timerout.h"


//项目支持AD个数
#define   AD_MAXNUM         25      //AD 模块最大支持路数
#define   AD_FILTER_NUM     8       //AD 率波buf数量
#define   ADTEMP_MAXNUM     8       //最大支持的温度转换数量
#define   ADVOLT_MAXNUM     3       //最大支持的电压转换数量
#define   ADID_TEMP_BEGIN   3       //温度的起始ID值
#define   ADTREAT_READTIME   2      //读取AD模块的时间
#define   ADTREAT_UNCOMPLETE_TIMES   5   //当在 ADTREAT_READTIME内无法读取到指定AD 时，等待 此次数，则为UNCOMPLETE 故障

//READ IGN ONLY 时的等待时间，这2个参数需要根据MCU适当调节
#define  RIGN_INTCONVER_TIMES    1000     //AD中断完成需要等待的时间
#define  RIGN_WAITAD_TIMES        200     //单个AD转换完成的等待时间

 //只读取AD时，读取AD的次数
#define   READ_ADIGN_TIMES        2 

//温度信号短路和断路判断说明
#define    AD_TEMPERR_OPEN     1002  // >= 温度断路AD 
#define    AD_TEMPERR_SHORT    21    //<= 短路AD 
//电压故障判断范围
#define    AD_VOLT12_HIGHERR    1610
#define    AD_VOLT12_HIGHOK     1590
#define    AD_VOLT12_LOWERR     890
#define    AD_VOLT12_LOWOK      910

#define    AD_VOLT24_HIGHERR    3210
#define    AD_VOLT24_HIGHOK     3190
#define    AD_VOLT24_LOWERR     1790
#define    AD_VOLT24_LOWOK      1810
/*********************不更改变量分界************************/
//AD 转换的状态
#define  ADCONVER_STATE_INTREAD        0           //中断模式     
#define  ADCONVER_STATE_INTOK          1           
#define  ADCONVER_STATE_INIT           2
#define  ADCONVER_STATE_WAITREAD       0x10        //等待模式
#define  ADCONVER_STATE_WAITOK         0x11

//本模块故障码
#define  ADMODULE_ERRCODE_OK         0x00 
#define  ADMODULE_ERRCODE_INT        0x01      //误触发中断
#define  ADMODULE_ERRCODE_UNCOMPETE  0x02      //1个周期内没有转换完成

//电源模式
#define   ADVOLT_MODE_12V       0
#define   ADVOLT_MODE_24V       1
#define   ADVOLT_MODE_INVAILED  2

//ADPHY故障类型
#define ADPHYERR_TEMP_OK       0
#define ADPHYERR_TEMP_SHORT    1        //温度传感器短路
#define ADPHYERR_TEMP_OPEN     2        //温度传感器断路
#define ADPHYERR_TEMP_INVAILED 0xff     //无效值  

#define ADPHYERR_VOLT_OK       0
#define ADPHYERR_VOLT_HIGH     1         //电压过高
#define ADPHYERR_VOLT_LOW      2         //电压过低
#define ADPHYERR_VOLT_INVAILED 0xff      //无效值  


typedef int16 ADTempType;        //AD 温度形式， temp = data/10
typedef int16 ADVoltType;       //AD 的电压转换模式， V =data/100

#define ACD_V(volt)        ((int16)(volt *100))    //实际电压v转换为 数字量
#define ACD_T(temp)        ((int16)(temp *10))     //实际温度℃转换为 数字量


typedef struct
{
	uint16 u16ADValue;            
	int16 i16PhyValue;            
}ADTESTDATA;                     //测试返回数据结构体定义

//AD_ValueGet 得到的类型设置 
 typedef enum
 {
	 ADVALUE_GETTYPE_8AD = 0,        //8位ad
	 ADVALUE_GETTYPE_8ADFAST,        //快速8位ad
	 ADVALUE_GETTYPE_10AD,           //10位ad
	 ADVALUE_GETTYPE_10ADFAST,       //快速10位ad
	 ADVALUE_GETTYPE_INVAILED,      //大于等于此值为错误
 }ADValueGetType;  
 

 
//底层必须调用
void ADHWEnd_Interrupt(uint16 u16ReadResult);
//上层可调用函数声明
BOOL ADInit(uint8 u8MaxNum, uint8 u8VoltRegisterType);
BOOL ADPhy_Init(uint8 u8VNum, uint8 u8IgnMode, uint8 u8TNum, const uint16** pTBL);
uint16  ADValue_Get(uint8 adid, ADValueGetType u8ADGetType);
int16 ADPhy_GetFact(uint8 adid);
uint8 ADPhy_GetErr(uint8 adid);
uint8 ADModule_ErrCode (void);
void Treat_Read_Ad(void);
ADVoltType ReadIgn_Only(uint8 VoltId);
uint8 ADModule_Test(ADTESTDATA* pTestTable);
#endif
