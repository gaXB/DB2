/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�timerout.c
* ����������ʵ�ֶ�ʱ�����ܣ�
* 1  ms��0.1S, 1S ��ʱ��
* 2 ����loopѭ��������
*
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭ��ʱ��ģ��0.0.6��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��4��25��
* 
*
* �汾��1.1
* ˵��������uint16 TimeOut_GetMsData(void)
* ���ߣ�xb
* ������ڣ�2020��1��6��
*/
#include "timerout.h"

 
static uint16 mSdata;           //ÿ1ms��1
static uint16 TenthSdata;       //ÿ0.1s��1
static uint16 Sdata;            //ÿ1s��1
 
static uint16 MaxLoopTime;  
static uint32 u32AvrLoopFreq;


 /***********************************
 ͨ��Ӳ��������Ҫ���õĺ�������
 ************************************/
 //��ʱ��ģ��
#if   TIMEOUT_CPU_8BIT
void  TimerCounterHalt(void) ;
void  TimerCounterResume(void);
#endif
/*******************************************************************************
 * Function:   void MsCounterUpadat(void) 
 * 
 * Description:ÿ��һ��ʱ��+1
 *
 * Returns:     noe
 *
 * Notes:     �ж�ʱ����
 *
*******************************************************************************/
void MsCounterUpadat(void) 
{
	mSdata++;
}

/*******************************************************************************
 * Function:   void TenthSCounterUpadat(void) 
 *             void SCounterUpadat(void) 
 * Description:���غ���
 *
 * Returns:     noe
 *
 * Notes:     main-loop����
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
 * Description: ms��ʱ������ *time Ϊ��ʱ�Ĳ�����timeOutValΪ��ʱʱ�䣬Ϊ0���ʼ��*time
 *
 * Returns:     ������ʱ��ʱ����1�� �� *time��ʼ��������Ϊ0
 *
 * Notes:    ���ô˺����鿴��ʱ�Ƿ񵽣����ǵ�ʵ����������Ϊ65525ms
			
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
	else if(diftime >= timeOutVal)    //�ж��Ƿ�ʱ
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
 * Description: ms��ʱ������ *time Ϊ��ʱ�Ĳ�����timeOutValΪ��ʱʱ�䣬Ϊ0���ʼ��*time
 *
 * Returns:     ������ʱ��ʱ����1�� �� *time��ʼ��������Ϊ0
 *
 * Notes:    ��Ҫ�������ڽ���Ĺ����ϣ��ɷ�ֹ��ν���������ۻ����
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
	else if(diftime >= timeOutVal)    //�ж��Ƿ�ʱ
	{
		if (diftime >= (timeOutVal + 10))
		{//�˴�ֱ�ӵ��ڵ�ǰʱ�䣬��Ϊ�˷�ֹ�ж�ʱ��ûȥ��ѯ֮�󣬶�ν��������
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
 * Description: 0.1s��ʱ������ *time Ϊ��ʱ�Ĳ�����timeOutValΪ��ʱʱ�䣬Ϊ0���ʼ��*time
 *
 * Returns:     ������ʱ��ʱ����1�� �� *time��ʼ��������Ϊ0
 *
 * Notes:    ���ô˺����鿴��ʱ�Ƿ񵽣����ǵ�ʵ����������Ϊ65530/2 s
			
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
	else if (diftime >= timeOutVal)    //�ж��Ƿ�ʱ
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
 * Description: 1s��ʱ������ *time Ϊ��ʱ�Ĳ�����timeOutValΪ��ʱʱ�䣬Ϊ0���ʼ��*time
 *
 * Returns:     ������ʱ��ʱ����1�� �� *time��ʼ��������Ϊ0
 *
 * Notes:    ���ô˺����鿴��ʱ�Ƿ񵽣����ǵ�ʵ����������Ϊ65530 s
			
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
	else if(diftime >= timeOutVal)    //�ж��Ƿ�ʱ
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
 * Description: �����ã�������ѭ��������Ƶ��
 *
 * Returns:    ����1s�ڿɵ��õĴ���
 *
 * Notes:    ���Կ� ʹ��
			
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
 * Description: �����ã�������ѭ�������LOOpʱ��  
 *
 * Returns:    �������loopʱ��
 *
 * Notes:    ���Կ� ʹ��
			
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
 * Description: �õ���ǰ��msdata
 *
 * Returns:    ��ǰ��msdata
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
