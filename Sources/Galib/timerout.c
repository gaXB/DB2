/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：timerout.c
* 功能描述：实现定时器功能：
* 1  ms，0.1S, 1S 定时器
* 2 测试loop循环的性能
*
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原定时器模块0.0.6移植
* 原作者：xb
* 完成日期：2019年4月25日
* 
*
* 版本：1.1
* 说明：增加uint16 TimeOut_GetMsData(void)
* 作者：xb
* 完成日期：2020年1月6日
*/
#include "timerout.h"

 
static uint16 mSdata;           //每1ms加1
static uint16 TenthSdata;       //每0.1s加1
static uint16 Sdata;            //每1s加1
 
static uint16 MaxLoopTime;  
static uint32 u32AvrLoopFreq;


 /***********************************
 通过硬件驱动需要调用的函数声明
 ************************************/
 //定时器模块
#if   TIMEOUT_CPU_8BIT
void  TimerCounterHalt(void) ;
void  TimerCounterResume(void);
#endif
/*******************************************************************************
 * Function:   void MsCounterUpadat(void) 
 * 
 * Description:每隔一定时间+1
 *
 * Returns:     noe
 *
 * Notes:     中断时调用
 *
*******************************************************************************/
void MsCounterUpadat(void) 
{
	mSdata++;
}

/*******************************************************************************
 * Function:   void TenthSCounterUpadat(void) 
 *             void SCounterUpadat(void) 
 * Description:主控函数
 *
 * Returns:     noe
 *
 * Notes:     main-loop调用
 *
*******************************************************************************/
void TenthSCounterUpadat(void) 
{
	static uint16 TenthSecTimer;
	if (TimeOutChkMsLong(&TenthSecTimer, 100)) 
	{
		TenthSdata++;
	}
	else{}
}

void SCounterUpadat(void) 
{
	static uint16 SecTimer;
	if(TimeOutChkMsLong(&SecTimer, 1000))
	{
		Sdata++;
	}
	else{}
}
/*******************************************************************************
 * Function:    BOOL TimeOutChkMsLong(uint16 *time ,uint16 timeOutVal) 
 *
 * Description: ms定时函数， *time 为定时的参数。timeOutVal为定时时间，为0则初始化*time
 *
 * Returns:     当到达时间时返回1， 且 *time初始化。否则为0
 *
 * Notes:    调用此函数查看定时是否到，考虑到实际情况，最大为65525ms
			
 *
*******************************************************************************/
BOOL TimeOutChkMsLong(uint16 *timer, uint16 timeOutVal)
{
	uint16 shadow,diftime;
	BOOL bTimeOut;
#if   TIMEOUT_CPU_8BIT
	TimerCounterHalt();
#endif
	shadow = mSdata;
#if  TIMEOUT_CPU_8BIT
	TimerCounterResume();
#endif   
	diftime = shadow - *timer;
    
	if(timeOutVal == 0)
	{
		*timer = shadow;
		bTimeOut = TRUE;
	}
	else if(diftime >= timeOutVal)    //判断是否到时
	{
		*timer += diftime;
		bTimeOut = TRUE; 
	}
	else 
	{ 
		bTimeOut = FALSE;
	}
	return bTimeOut;
}
  
/*******************************************************************************
 * Function:    BOOL TimeOutChkTMsLong(uint16 *time ,uint16 timeOutVal) 
 *
 * Description: ms定时函数， *time 为定时的参数。timeOutVal为定时时间，为0则初始化*time
 *
 * Returns:     当到达时间时返回1， 且 *time初始化。否则为0
 *
 * Notes:    主要用在周期进入的功能上，可防止多次进入产生的累积误差
 *
*******************************************************************************/
BOOL TimeOutChkTMsLong(uint16 *timer, uint16 timeOutVal)
{
	uint16 shadow,diftime;
	BOOL bTimeOut;
#if   TIMEOUT_CPU_8BIT
   TimerCounterHalt();
#endif
	shadow = mSdata;
#if  TIMEOUT_CPU_8BIT
	TimerCounterResume();
#endif   
	diftime = shadow - *timer;
    
	if(timeOutVal == 0)
	{
		*timer = shadow;
		bTimeOut = TRUE;
	}
	else if(diftime >= timeOutVal)    //判断是否到时
	{
		if (diftime >= (timeOutVal + 10))
		{//此处直接等于当前时间，是为了防止有段时间没去查询之后，多次进入的问题
			*timer += diftime;
		}
		else
		{
			*timer += timeOutVal;
		}
		bTimeOut = TRUE; 
	}
	else
	{  
		bTimeOut = FALSE;
	}
	return bTimeOut;
}
  
/*******************************************************************************
 * Function:    BOOL TimeOutChkHsLong(uint16 *time ,uint16 timeOutVal) 
 *
 * Description: 0.1s定时函数， *time 为定时的参数。timeOutVal为定时时间，为0则初始化*time
 *
 * Returns:     当到达时间时返回1， 且 *time初始化。否则为0
 *
 * Notes:    调用此函数查看定时是否到，考虑到实际情况，最大为65530/2 s
			
 *
*******************************************************************************/
BOOL TimeOutChkTenthSLong(uint16 *timer, uint16 timeOutVal)
{	
	BOOL bTimeOut;
	uint16 diftime = TenthSdata - *timer;

	if(timeOutVal == 0)
	{
		*timer = TenthSdata;
		bTimeOut = TRUE;
	}
	else if (diftime >= timeOutVal)    //判断是否到时
	{
      *timer += diftime;
      bTimeOut = TRUE; 
	}
	else
	{  
		bTimeOut = FALSE;
	}
	return bTimeOut;
}
      
      
  
/*******************************************************************************
 * Function:    BOOL TimeOutChkHsLong(uint16 *time ,uint16 timeOutVal) 
 *
 * Description: 1s定时函数， *time 为定时的参数。timeOutVal为定时时间，为0则初始化*time
 *
 * Returns:     当到达时间时返回1， 且 *time初始化。否则为0
 *
 * Notes:    调用此函数查看定时是否到，考虑到实际情况，最大为65530 s
			
 *
*******************************************************************************/
BOOL TimeOutChkSLong(uint16 *timer, uint16 timeOutVal)
{
	BOOL bTimeOut;
	uint16 diftime = Sdata - *timer;
    
	if(timeOutVal == 0)
	{
		*timer = Sdata;
		bTimeOut = TRUE;
	}
	else if(diftime >= timeOutVal)    //判断是否到时
	{
		*timer += diftime;
		bTimeOut = TRUE; 
	}
	else
	{  
		bTimeOut = FALSE;
	}
	
	return bTimeOut;
}


/*******************************************************************************
 * Function:   uint32 TimeOut_TestLoop(void)
 *
 * Description: 测试用，测试主循环的运行频率
 *
 * Returns:    返回1s内可调用的次数
 *
 * Notes:    调试可 使用
			
 *
*******************************************************************************/
uint32 TimeOut_TestLoop(void)
{
	static uint32 u32LoopTimes;
	static uint16 TimerMeasure;
	
   u32LoopTimes++;
   if (TimeOutChkMsLong(&TimerMeasure, LOOP_MEASURE_TIME))
   {
   	u32AvrLoopFreq = u32LoopTimes;
   	u32LoopTimes = 0;
   	MaxLoopTime = 0;
   }
   else{}  
   
   return u32AvrLoopFreq;
}
/*******************************************************************************
 * Function:   uint16 TimeOut_GetMaxLoop(void)
 *
 * Description: 测试用，测试主循环的最大LOOp时间  
 *
 * Returns:    返回最大loop时间
 *
 * Notes:    调试可 使用
			
 *
*******************************************************************************/
uint16 TimeOut_GetMaxLoop(void)
{
	static uint16   LastmSdata;
   uint16 shadow, CurLoopTime;
   
#if   TIMEOUT_CPU_8BIT
   TimerCounterHalt();
#endif
   shadow = mSdata;
#if   TIMEOUT_CPU_8BIT
   TimerCounterResume();
#endif
   
   CurLoopTime = shadow - LastmSdata;
   LastmSdata = mSdata;
   if(MaxLoopTime < CurLoopTime)
   {
   	MaxLoopTime = CurLoopTime;
   }
   else{}
   

   return MaxLoopTime;
}
  
/*******************************************************************************
 * Function:   uint16 TimeOut_GetMsData(void)
 *
 * Description: 得到当前的msdata
 *
 * Returns:    当前的msdata
 *
 * Notes:    
			
 *
*******************************************************************************/
uint16 TimeOut_GetMsData(void)
{
   uint16 shadow;
#if   TIMEOUT_CPU_8BIT
   TimerCounterHalt();
#endif
   shadow = mSdata;
#if   TIMEOUT_CPU_8BIT
   TimerCounterResume();
#endif
   return shadow;
}
