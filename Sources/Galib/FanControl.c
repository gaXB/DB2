/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：FanControl.c
* 功能描述：鼓风机驱动，具有以下功能
* 1 通过采集鼓风机的端电压和反馈电压，得到当前鼓风机输出电压
* 2 通过PID控制，设置PWM，继电器来达到电压的稳定
* 3 可设置电压调节
* 4 进行故障判断
* 
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原模块的1.2移植
* 原作者：xb
* 完成日期：2019年6月9日
* 
* 版本：1.1
* 说明： 1 增加电压误差配置字
*     
* 原作者：xb
* 完成日期：2019年9月23日
* 
* 版本：1.2
* 说明： 1 故障码判断策略更改
*     
* 版本：1.3
* 说明： 1 修改鼓风机慢速调节的默认ki 由4改为20，进入慢速由0.5v改为0.4
*        2 不能调节慢速的I值的BUG修改
* 原作者：xb
* 完成日期：2020年1月18日
*
*版本：1.4  
* 说明： 
* 1.Init初始化时，清除定时参数sFanData.ShortErrTime = 0;
* 2.FanVolt_Control，当IGN电压＜5V，清除定时参数；
* 原作者：lyc
* 完成日期：2020年9月29日
* 
* 版本：1.5  
* 说明： 
* 1.目标电压与当前电压差值小于等于0.3V时，速度会减慢,进入PID“I”相调节，上一版本是0.5V；
* 原作者：zhupan
* 完成日期：2020年12月8日
*/
#include "FanControl.h"
#include "mathGA.h"
#include "DealString.h"

typedef struct
{
	ADVoltType  avSetVolt;             //设置电压
	ADVoltType  avLastVolt;            //反馈的
	ADVoltType  avCurrentSetVolt;      //当前应该达到的电压，即理论电压
	uint16  u16SetRate;             //目标变化电压 单位（0.1v/s）, 即范围 0.1v/s - 25.5v/s
   int16   i16ErrSum;              //误差积分项
   int16   i16Err_1;               //前次误差,差分项
	uint16  TimerFanAdj;            //风量调节处理
	uint16  u16FanPWM;              //风量PWM = D /10000	
	BOOL    bFanRealy;              //继电器
	uint8   OpenErrTime;            //故障码计数器      
	uint8   ShortErrTime;
	uint8   u8AdjState;             //调节状态	
	uint8   u8AdjStateTime;         //调节计数器
	uint8   u8ErrCode;              //故障码
	uint8   u8CurrentLevel;         //是理论值avCurrentSetVolt。，得出来的档位
}FANCONTROLDATA;   //风量控制字


static FANCONTROLDATA  sFanData;
static FANCONTROLCONFIG sFanConfig;
static ADVoltType avMaxFanVolt;        //最大挡位的电压

//底层驱动驱动函数
extern ADVoltType FanHW_BackVolt(void);
extern ADVoltType FanHW_SupperVolt(void);
extern void FanHW_OutPWM(uint16 pwm);
extern void FanHW_OutFanRealy(uint8 bSetState);
/******************************************************************************
* Function:   BOOL FanControl_Init(FANCONTROLCONFIG* pFConfig)
* 
* Description:   鼓风机控制初始化
*
* param1 ：       pFConfig

* return :    BOOL 
* 
* Note  :     鼓风机初始化函数
*
************************************************************************************/
BOOL FanControl_Init(FANCONTROLCONFIG* pFConfig)
{
	BOOL bFanInitOK = TRUE;
	uint8 i;
	ADVoltType avFanLastVolt = 1;

	MemCopy(&sFanConfig, pFConfig, sizeof(FANCONTROLCONFIG));
	avMaxFanVolt = pFConfig->TBL_FanVolt[pFConfig->u8FanMaxLevel];

	sFanData.avCurrentSetVolt = 0;
	sFanData.u16FanPWM = 0;
	sFanData.avLastVolt = 0;
	sFanData.u16SetRate = pFConfig->u16InitRate;
	sFanData.avSetVolt = 0;
	sFanData.u8CurrentLevel = 0;
	sFanData.u8ErrCode = FAN_ERRCODE_NONE;
	sFanData.u8AdjState = FAN_STATE_CLOSE;
	sFanData.OpenErrTime = 0;
    sFanData.ShortErrTime = 0;
	
	//rate 不能为0
	if (pFConfig->u16InitRate == 0 && pFConfig->u8SlowAdjFactor == 0)
	{
		sFanConfig.u16InitRate = FAN_DEFAULT_RATE;
		sFanConfig.u8SlowAdjFactor = FAN_SLOWADJ_FACTOR;
		bFanInitOK = FALSE;
	}
	else
	{

	}

	//风量tbl 不能有错
	for (i = 1; i <= pFConfig->u8FanMaxLevel; i++)
	{
		if (pFConfig->TBL_FanVolt[i] <= avFanLastVolt)
		{
			bFanInitOK = FALSE;
		}
		else
		{
			avFanLastVolt = pFConfig->TBL_FanVolt[i];
		}
	}
	
	// v offset 必须在200~256 之间
	if (sFanConfig.u8VoltOffSet >= 200)
	{
		
	}
	else
	{
		sFanConfig.u8VoltOffSet = VOUT_OFFSET_DEFAULT;
		bFanInitOK = FALSE;
	}
	return bFanInitOK;
}
/******************************************************************************
* Function:   void FanControl_SetVolt(ADVoltType u16Volt)
* 				
* Description:   设置鼓风机目标电压
*
* param1 ：   ADVoltType  目标电压

* return :    NONE
* 
* Note  : 
*
************************************************************************************/
void FanControl_SetVolt(ADVoltType avSetValue)
{	
	
	if ( GAMath_abs(sFanData.avSetVolt - avSetValue) > ACD_V(0.1) )
	{
		if (sFanData.avSetVolt <= 0)
		{//增加，当原来为0档时，到其他档就为初始速度
			sFanData.u16SetRate = sFanConfig.u16InitRate;
		}
		else
		{
		}		
		
		//sFanData.u8AdjState = FAN_STATE_PID;
		sFanData.i16ErrSum = 0;
		sFanData.i16Err_1 = 0;
		sFanData.avSetVolt = avSetValue;
	}
	else
	{
		//sFanData.avSetVolt = avSetValue;
	}
}
/******************************************************************************
* Function:   void FanControl_SetRate(uint16 u16Rate)
* 				
* Description:   设置鼓风机变化速度
*
* param ：           u16Rate 变化速率 单位为 0.1v/s, 不能为0

* return :     none
* 
* Note  :    
*
************************************************************************************/
void FanControl_SetRate(uint16 u16Rate)
{
	if (sFanData.u16SetRate != u16Rate)
	{
		sFanData.u16SetRate = u16Rate;	
		sFanData.i16ErrSum = 0;
		sFanData.i16Err_1 = 0;
	}
	else
	{
		//设置相同则不处理
	}
}


/******************************************************************************
* Function:   uint8 Fan_CurrentLevelGet(void)
* 
* Description:   得到当前的风档档位
*
* return :    挡位
* 
* Note  :  
*
************************************************************************************/
uint8 FanControl_GetCurrentLevel(void)
{
	return sFanData.u8CurrentLevel;
}
/******************************************************************************
* Function:   ADVoltType FanControl_GetCurrentVolt(void)
* 
* Description:   得到当前的鼓风机的电压
*
* return :    ADVoltType 
* 
* Note  :  
*
************************************************************************************/
ADVoltType FanControl_GetCurrentVolt(void)
{
	return (sFanData.avLastVolt);
}

/******************************************************************************
* Function:   uint8 Fan_ErrCode(void)
* 
* Description:   得到风量控制故障码
*
* return :   故障码
* 
* Note  :    具体定义见 故障码宏定义
*
************************************************************************************/
uint8 FanControl_ErrCodeGet(void)
{
	return sFanData.u8ErrCode;
}

/******************************************************************************
* Function:    void FanVolt_Control(void)
* 
* Description:   鼓风机控制 主控函数
*
* return :     none
* Note  :   
*
************************************************************************************/
void FanVolt_Control(void)
{
	static uint16	TimerFanControl;
	int16  i16Delta;                          //本次误差
	ADVoltType avCurrentVolt;                 //本次输出电压
	uint16 u16DiffVolt;                       //与目标电压的差距
	int32  i32Pid;
	ADVoltType avSupperVolt, avBackVolt;      //鼓风机端电压，反馈电压
	ADVoltType avSystemVolt;                 //系统的电压12，24
	int16 i16VoltOffset;
	//PID 控制应该满足电压范围内
	ADVoltType avCloseVolt; 
	ADVoltType avStopAdjVolt;	
	uint8  u8FanAdjState;                    //0 不调节， 1 向上， 2向下
	
	if (TimeOutChkMsLong(&TimerFanControl, FAN_CONTROL_TIME))
	{  //10ms 调整一次
		//计算鼓风机端电压
		avSupperVolt = FanHW_SupperVolt();
		if (sFanData.bFanRealy)
		{
			avBackVolt = FanHW_BackVolt();
		}
		else
		{
			avBackVolt = avSupperVolt;
		}
		
		
   	if (sFanConfig.bSystemVolt24)
   	{
   		avCloseVolt = IGN_CLOSE_24VOLT;
   		avStopAdjVolt = IGN_STOPADJ_24VOLT;
   		avSystemVolt = SYSTEM_VOLT_24;
   	}
   	else
   	{
   		avCloseVolt = IGN_CLOSE_12VOLT;
   		avStopAdjVolt = IGN_STOPADJ_12VOLT;
   		avSystemVolt = SYSTEM_VOLT_12;
   	}
   	
   	//当电源电压低于5v，则故障计时清除
      if (avSupperVolt <= avCloseVolt)
   	{
      	sFanData.ShortErrTime = 0;
      	sFanData.OpenErrTime  = 0;
      	sFanData.u8ErrCode = FAN_ERRCODE_NONE;
   	}
		
		if (avSupperVolt >= avBackVolt)
		{
			//此处为线速压降的处理，得到当前的鼓风机实际电压avCurrentVolt
			avCurrentVolt = avSupperVolt - avBackVolt;
			if (avCurrentVolt > avSystemVolt)
			{
				i16VoltOffset = sFanConfig.u8VoltOffSet;
			}
			else 
			{
				i16VoltOffset = GAMath_LineFuni16(0, avSystemVolt, 256, sFanConfig.u8VoltOffSet, avCurrentVolt);
			}
			
			if (i16VoltOffset > 256 || i16VoltOffset < sFanConfig.u8VoltOffSet) i16VoltOffset = sFanConfig.u8VoltOffSet;
			else {}
			
			avCurrentVolt = (uint16)((uint32)avCurrentVolt * i16VoltOffset >>8); 
		}
		else
		{//当电源电压小于反馈电压， FAN_ERRCODE_BACK
			avCurrentVolt = 0;
		}

		if (avSupperVolt <= avCloseVolt)
		{//当电源电压低于关闭电压（5v），则输出为0
			sFanData.u16FanPWM = 0;
			FanHW_OutPWM(0);	
			FanHW_OutFanRealy(0);
		}
		else if (avSupperVolt <= avStopAdjVolt)
		{//供电电压变化时暂停调节，PID 重新计算
			sFanData.u8AdjState = FAN_STATE_PID;
			sFanData.i16ErrSum = 0;
			FanHW_OutPWM(sFanData.u16FanPWM);	
			FanHW_OutFanRealy(sFanData.bFanRealy);
		}
		else
		{
			//以下为计算当前的应该的风量上升速度
			if (sFanData.avSetVolt > sFanData.avCurrentSetVolt)
			{
				u8FanAdjState = 1;
			}
			else if (sFanData.avSetVolt < sFanData.avCurrentSetVolt)
			{
				u8FanAdjState = 2;
			}
			else
			{
				u8FanAdjState = 0;
			}
			//计算理论设置电压，即当前应该到的电压值
			int16 i16FanAdjSetV;   //计算当前目标的电压以调整多少
			i16FanAdjSetV = sFanData.u16SetRate * FAN_CONTROL_TIME / 100;
			if (i16FanAdjSetV == 0)
			{
				i16FanAdjSetV = 1;
			}else{}
			
			//按照调节速度更改目标电压，且计算出当前挡位
			if (sFanData.avSetVolt <= 0)
			{
				sFanData.u8AdjState = FAN_STATE_CLOSE;
				sFanData.avCurrentSetVolt = 0;
				sFanData.u16SetRate = sFanConfig.u16InitRate; //
			}
//			else if (sFanData.u8AdjState == FAN_STATE_START)
//			{
//				if (sFanData.avSetVolt > sFanConfig.TBL_FanVolt[1])
//				{
//					sFanData.avCurrentSetVolt = sFanConfig.TBL_FanVolt[1];
//				}
//				else 
//				{
//					sFanData.avCurrentSetVolt = sFanData.avSetVolt;
//				}
//			}
			else if (u8FanAdjState == 1)
			{
				sFanData.avCurrentSetVolt += i16FanAdjSetV; 
				if (sFanData.avCurrentSetVolt >= sFanData.avSetVolt)
				{
					sFanData.avCurrentSetVolt = sFanData.avSetVolt;
				}else{}
			}
			else if (u8FanAdjState == 2)
			{
				sFanData.avCurrentSetVolt -= i16FanAdjSetV;    
				if (sFanData.avCurrentSetVolt <= sFanData.avSetVolt)
				{
					sFanData.avCurrentSetVolt = sFanData.avSetVolt;
				}else{}
			}
			else{}

			//将电压转换为档位
			sFanData.u8CurrentLevel = GAMath_CalTBLi16Level(sFanData.u8CurrentLevel, sFanData.avCurrentSetVolt,
						(int16*)sFanConfig.TBL_FanVolt, sFanConfig.u8FanMaxLevel);
			
			if (sFanData.u8CurrentLevel >= sFanConfig.u8FanMaxLevel )
			{
				sFanData.u8CurrentLevel = sFanConfig.u8FanMaxLevel;
			}
			else
			{}
			
			if (sFanData.avSetVolt > 0)
			{//当设置不为0时，档位肯定>=1 
				if (sFanData.u8CurrentLevel == 0)
				{
					sFanData.u8CurrentLevel = 1;
				}
				else
				{}
			}
			else
			{
				sFanData.u8CurrentLevel = 0;
			}
			
			
			//求当前的误差
			if (sFanData.avSetVolt >= avCurrentVolt)
			{
				u16DiffVolt = sFanData.avSetVolt - avCurrentVolt;	
			}
			else
			{
				u16DiffVolt = avCurrentVolt - sFanData.avSetVolt;		
			}	
			//故障判断
			if (u16DiffVolt >= ACD_V(0.5))
			{
				if (sFanData.avSetVolt < (avMaxFanVolt - ACD_V(1)) && sFanData.avSetVolt != 0 
						&& avBackVolt <= FANSHORT_VOLT)
				{//当不是最大档时和0档，反馈电压小于1V
					sFanData.ShortErrTime++;
					if (sFanData.ShortErrTime >= SHORT_ERR_TIME)
					{//故障持续1s, 后确认故障
						sFanData.u8ErrCode = FAN_ERRCODE_SHORT;
					}
					else{}
				}
				else
				{
					sFanData.ShortErrTime = 0;
				}
			
				if (sFanData.u16FanPWM >= sFanConfig.u16MinPwm && avCurrentVolt <= FANOUT_OPENVOLT)
				{ //当输出占空比>= FANOUT_MINPWM  ，但反馈电压接近电源电压，FAN_ERRCODE_OPEN
					//即 avCurrentVolt <= FANOUT_OPENVOLT， 即有输出时 反馈电压不会接近电源电压，或者鼓风机端电压不会太小
					sFanData.OpenErrTime++;
					if (sFanData.OpenErrTime >= OPEN_ERR_TIME)
					{//次故障持续1s 后确认故障
						sFanData.u8ErrCode = FAN_ERRCODE_OPEN;
					}
					else{}
				}
				else
				{
					sFanData.OpenErrTime = 0;
				}
			}
			else
			{
				sFanData.ShortErrTime = 0;
				sFanData.OpenErrTime = 0;
				if (u16DiffVolt <= FAN_CONTROL_ACCURACY_VOLT)
				{
					sFanData.u8ErrCode = FAN_ERRCODE_NONE;
				}
				else{}
			}
	//故障判断end

			
			//pid 控制
			i16Delta = sFanData.avCurrentSetVolt - avCurrentVolt;  //比例项， 与目标速度的差
			
			sFanData.i16ErrSum += i16Delta;   //积分项
			if (sFanData.i16ErrSum  >  PID_I_MAXVALUE) 
			{
				sFanData.i16ErrSum =  PID_I_MAXVALUE;
			}
			else if (sFanData.i16ErrSum  < -PID_I_MAXVALUE) 
			{
				sFanData.i16ErrSum = -PID_I_MAXVALUE;
			}
			else{}
	
			//根据状态调节状态，计算调节速度
			switch (sFanData.u8AdjState)
			{
				default:
				case FAN_STATE_CLOSE:
					if (sFanData.avSetVolt <= 0)
					{
						sFanData.bFanRealy = 0;
					}
					else
					{
						sFanData.bFanRealy = 1;
						sFanData.u8AdjState = FAN_STATE_START;
						sFanData.avCurrentSetVolt = sFanConfig.TBL_FanVolt[1];
					}
					sFanData.u16FanPWM = 0;
					i32Pid = 0;
					sFanData.u8AdjStateTime =0;
					sFanData.i16ErrSum = 0;
					break;
				case FAN_STATE_START:
					sFanData.u8AdjStateTime++;
					if (sFanData.u8AdjStateTime > FAN_START_TIME)
					{
						sFanData.u8AdjStateTime = 0;
						sFanData.u8AdjState = FAN_STATE_PID;
					}
					else{}
					//此状态下直接运行PID，不break
				case FAN_STATE_PID:	
					if ( (u8FanAdjState == 1 && i16Delta < 0) || (u8FanAdjState == 2 && i16Delta > 0) )
					{//向上只能为+，向下只能为-，相等时，可正负
						i32Pid = 0;
						sFanData.i16ErrSum = 0;
					}
					else
					{
						//正常PID调节
						if (u16DiffVolt <= FAN_CONTROL_SLOWADJ_VOLT)
						{
							sFanData.u8AdjStateTime = 0;
							sFanData.u8AdjState = FAN_STATE_SLOWADJ;
						}
						else{}
						
						i32Pid = i16Delta * (int32)sFanConfig.i16FanPFactor;
						i32Pid += sFanData.i16ErrSum * (int32)sFanConfig.i16FanIFactor;
						i32Pid += (i16Delta - sFanData.i16Err_1) * (int32)sFanConfig.i16FanDFactor;//+微分项
					}
					sFanData.bFanRealy = 1;
					break;
				case FAN_STATE_SLOWADJ:
					//低速调节,每次调节1
					if (u16DiffVolt > FAN_CONTROL_SLOWADJ_VOLT)
					{
						sFanData.u8AdjStateTime++;
						if (sFanData.u8AdjStateTime > FANADJ_STOPTIMES)
						{//如果200ms内都有问题，则退出
							sFanData.u8AdjStateTime = 0;
							sFanData.u8AdjState = FAN_STATE_PID;
						}else{}
					}
					else if (u16DiffVolt > FAN_CONTROL_ACCURACY_VOLT)
					{
						if (sFanData.u8AdjStateTime > 0)
						{
							sFanData.u8AdjStateTime--;
						}else{}
					}
					else
					{
						sFanData.u8AdjStateTime = 0;
					}
					
					if (sFanData.i16ErrSum * sFanConfig.u8SlowAdjFactor >= PID_PARAMETER_SHIFTVALUE)
					{
						i32Pid = PID_PARAMETER_SHIFTVALUE;
						sFanData.i16ErrSum = 0;
					}
					else if (sFanData.i16ErrSum * sFanConfig.u8SlowAdjFactor <= -PID_PARAMETER_SHIFTVALUE)
					{
						i32Pid = -PID_PARAMETER_SHIFTVALUE;
						sFanData.i16ErrSum = 0;
					}
					else
					{
						i32Pid = 0;
					}
					sFanData.bFanRealy = 1;
					break;
				case FAN_STATE_STOP:
					//停止
					i32Pid = 0;
					break;	
			}
			
			sFanData.i16Err_1 = i16Delta; //保存上一次的误差,电压

			if (i32Pid < 0 && i32Pid > -PID_PARAMETER_SHIFTVALUE)
			{
				i32Pid = 0;
			}
			else
			{
				i32Pid >>= PID_PARAMETER_SHIFTNUM;
			}
	
			if (sFanData.u8AdjState != FAN_STATE_CLOSE)
			{
				i32Pid += sFanData.u16FanPWM;	
				//pwm 的范围	
				if (i32Pid < sFanConfig.u16MinPwm)
				{
					sFanData.u16FanPWM = sFanConfig.u16MinPwm;
				}
				else if (i32Pid >= FANOUT_MAXPWM)
				{
					sFanData.u16FanPWM = FANOUT_MAXPWM;
				}
				else
				{
					sFanData.u16FanPWM = i32Pid;
				}
			}
			else
			{
				sFanData.u16FanPWM = 0;
			}
			
			sFanData.avLastVolt = avCurrentVolt;
			// piD 控制结束
			
			if (sFanData.avSetVolt == avMaxFanVolt && sFanData.avLastVolt >= (avMaxFanVolt - ACD_V(0.5))
					&& sFanData.u16FanPWM > MAXOUT_MINPWM )
			{//当鼓风机为最大挡时，且鼓风机反馈电压已经快达到设置电压时则pwm为最大值
				sFanData.u16FanPWM = FANOUT_MAXPWM;
			}
			else{}
			
			FanHW_OutPWM(sFanData.u16FanPWM);//输出
			FanHW_OutFanRealy(sFanData.bFanRealy);
		}
	}
	else
	{
		//10ms未到
	}
}
