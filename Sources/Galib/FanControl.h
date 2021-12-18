/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：FanControl.h
* 功能描述：按键 模块的头文件。
* 为此模块能正常工作需要加入 需要定时器模块
*
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 *
* 版本：1.1
* 说明：对应 .c 文件
* 原作者：xb
* 完成日期：2019年9月22日
*
*
* 版本：1.2
* 说明：对应 .c 文件
* 原作者：xb
* 完成日期：2019年10月12日
* 
* * 版本：1.3
* 说明：对应 .c 文件
* 原作者：xb
* 完成日期：2020年1月18日
* 
*版本：1.4  
* 说明：对应 .c 文件 
* 原作者：lyc
* 完成日期：2020年9月29日
* 
* *版本：1.5  
* 说明：对应 .c 文件 
* 原作者：zhupan
* 完成日期：2020年12月8日
*/

#ifndef	FAN_CONTROL_H_
#define	FAN_CONTROL_H_

#include "DataType.h"
#include "ADTreat.h"

//风量控制状态
#define    FAN_STATE_CLOSE      0     //刚开始的关闭状态
#define    FAN_STATE_START      1     //启动状态
#define    FAN_STATE_PID        2     //PID 调节
#define    FAN_STATE_SLOWADJ    3     //慢速调节
#define    FAN_STATE_STOP       4     //停止

//系统电压相关配置
#define    SYSTEM_VOLT_12              ACD_V(13)      	// 12系统电压
#define    SYSTEM_VOLT_24              ACD_V(25)      	// 24系统电压
#define    FANOUT_MAXVOLT12            ACD_V(13.5)    	//最大电压12
#define    FANOUT_MAXVOLT24            ACD_V(25.5)    	//最大电压24

#define    IGN_CLOSE_12VOLT            ACD_V(5)          //当点火电压小于此电压时 停止PID 控制
#define    IGN_CLOSE_24VOLT            ACD_V(10)         //24
#define    IGN_STOPADJ_12VOLT          ACD_V(7)          //当点火电压小于此电压时 关闭鼓风机输出
#define    IGN_STOPADJ_24VOLT          ACD_V(14)         //24

//故障电压配置定义
#define   FANSHORT_VOLT                ACD_V(1)          //当输出占空比为<= FANOUT_MINPWM，反馈电压 为FANSHORT_VOLT时短路故障  FAN_ERRCODE_SHORT
#define   FANOUT_OPENVOLT              ACD_V(1)          //当输出占空比>= FANOUT_MINPWM  ，但反馈电压接近电源电压，FAN_ERRCODE_OPEN
   		                                                //即 u16CurrentVolt <= FANOUT_OPENVOLT
//导线压降参数， 
#define    VOUT_OFFSET_DEFAULT          248     		      //Vfan /(Vign -Vback) * 256

//分阶段调整电压
#define    FAN_CONTROL_ACCURACY_VOLT   ACD_V(0.2)       //小于此值达到精度     
#define    FAN_CONTROL_SLOWADJ_VOLT    ACD_V(0.3)       //小于此值进入慢速调节

//pwm占空比相关配置
#define    FANOUT_MAXPWM               10000            //最大PWM值
#define    MAXOUT_MINPWM               8000             //最大风量时，需要到这个值才能到最大输出,
                                                        //用来判定鼓风机短路故障，此pwm 不会变成<1v的反馈电压                    

//PID 最后移位数
#define   PID_PARAMETER_SHIFTNUM          11            //PID 最后移位再去调节       
#define   PID_PARAMETER_SHIFTVALUE       ((uint16)0x1 << PID_PARAMETER_SHIFTNUM)
#define   PID_I_MAXPWMVALUE               2             //I 项调节最大值
#define   PID_I_MAXVALUE                  (PID_I_MAXPWMVALUE * PID_PARAMETER_SHIFTVALUE)        
//慢速调节的配置
#define   FAN_CONTROL_TIME                 10          //调节周期ms
#define   FANADJ_STOPTIMES                 20          //持续 FAN_CONTROL_TIME *n  ms 都是不在精度范围内，跳到慢速调节
#define   FAN_START_TIME                   40          //持续 FAN_CONTROL_TIME *n  ms 启动时间参数
#define   OPEN_ERR_TIME                    100         //持续FAN_CONTROL_TIME *n  ms  都会故障则认为故障
#define   SHORT_ERR_TIME                   100         //持续FAN_CONTROL_TIME *n  ms  都会故障则认为故障


/*****************************上面为生成LIB时可调节参数，以下为应用可调用参数 ********************************/
//PID 参数默认值
#define   FAN_PID_PFACTOR                  100         //比例
#define   FAN_PID_IFACTOR                  20          //积分
#define   FAN_PID_DFACTOR                  0           //微分，可为0
#define   FAN_SLOWADJ_FACTOR               20           //慢速调节的参数，   
//电压调节速率配置
#define   FAN_DEFAULT_RATE                 25          //默认调节速度

//鼓风机故障码定义
#define   FAN_ERRCODE_NONE                  0
#define   FAN_ERRCODE_SHORT                0x01        //短路故障,鼓风机反馈短路到GND
#define   FAN_ERRCODE_OPEN                 0x02        //断路故障，鼓风机反馈短路到电源电压
#define   FAN_ERRCODE_SUPPERVOLT_LOW       0x04        //供电电压不足
#define   FAN_ERRCODE_ACCURACY             0x08        //精度未到

#define   FAN_ERRCODE_CIRTERR      (FAN_ERRCODE_OPEN | FAN_ERRCODE_SHORT)



//风量配置字
typedef struct
{
	BOOL   bSystemVolt24;       //0 为12v，1为24v	
	BOOL   bFanRelay;           //控制鼓风机继电器
	uint8  u8FanMaxLevel;	    //最大挡位
	uint8  u8SlowAdjFactor;     //慢速调节的参数，越大调节速度越快，但不稳定， 不能为0 
	int16  i16FanPFactor;       //比例因子
	int16  i16FanIFactor;       //积分因子
	int16  i16FanDFactor;       //微分因子，可为0
	uint16 u16InitRate;         //默认调节速度，当0档切换成其他档时，会自动将调节参数改为此值
	uint16 u16MinPwm;           //最小pwm，pwm调节时会以此值开始
	ADVoltType *TBL_FanVolt;    //风量电压表，符合从小到大
	uint8  u8VoltOffSet;        //因线上的损耗，实际电压 = u8VoltOffSet /256 *（vSuppler- vBack）必须在200~256 之间
}FANCONTROLCONFIG;

//应用可调用函数
BOOL FanControl_Init(FANCONTROLCONFIG* pFConfig);
void FanControl_SetVolt(ADVoltType avSetValue);
void FanControl_SetRate(uint16 u16Rate);
void FanVolt_Control(void);
uint8 FanControl_GetCurrentLevel(void);
ADVoltType FanControl_GetCurrentVolt(void);
uint8 FanControl_ErrCodeGet(void);
#endif
