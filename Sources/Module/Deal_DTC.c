#include "Deal_DTC.h"

/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Deal_DTC.c
* 功能描述：DTC 功能实现
* 1  实现DTC 功能，一般故障为8ms*30/2 = 120ms
* 2  实现DTCNUM_VOLTAGE_HIGH，DTCNUM_VOLTAGE_LOW 这2个DTC自动判定
* 3  根据点火电压的范围设置是否开启DTC功能
* 4  具有DTC 老化功能，DTC 快照功能
* 5  历史故障需要点火   DTC_CONFIRM_TIMES 次
* 6  缓慢记忆策略，当DTC 变化且达到一定时间后再进行记忆
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 此文件不能更改
* 版本：1.0
* 说明：由原1.3 版本更改过来，增加电压判断功能，
* 原作者：xb
* 完成日期：2019年10月25日
* 
* 版本：2.0
* 说明：由Begin_DTCCycle/End_DTCCycle 由本模块实现，外部不需要调用
* 原作者：xb
* 完成日期：2019年10月29日
*
* 版本：3.0
* 说明： 高低压故障反
* 作者：xb
* 完成日期：2020年1月7日
*/



static int8 nErrTimes[DTC_MAXNUM];     //故障次数
uint8 DTC[DTC_MAXNUM];                 //故障码
DTCSNAPDATA DTCSnap[DTC_MAXNUM];

static uint8 DTCTotalNum;

#if   _SAVE_DTC_
static uint8 bSaveDTC = 1;
#endif

static uint8 u8DTCCycState;   //

#define  DTCCYC_STATE_INIT      0
#define  DTCCYC_STATE_WORK      0
#define  DTCCYC_STATE_OVER      1
typedef enum
{ 
	DTC_VOLT_INIT = 0,              //初始化
	DTC_VOLT_OK = 1,                //运行,只有在此阶段在判断故障码
	DTC_VOLT_ERR = 2,               //故障
	DTC_VOLT_TIME = 3,              //延时
}eDTCVoltState;
//电压状态设置
static eDTCVoltState  DTCVoltState = DTC_VOLT_INIT;
static eDTCRETSTATE  u8VoltLow, u8VoltHigh;
//外部函数声明
extern void Save_DTC(void);
extern uint8 Load_DTC(void);
extern uint8 DTCStorGet(void);

//本模块实现函数
static void DTC_AgeDTC(void);
static void Begin_DTCCycle(void);
static void End_DTCCycle(void);
/*******************************************************************************
* Function:   void DeaVolt_DTC(void)
*
* Description:  故障码的电压处理
*
* Returns:      none
*
* Notes:       
*
*******************************************************************************/
static void DeaVolt_DTC(void)
{
	int16 i16Volt = ADPhy_GetFact(ADID_VOLT);
   static uint16 TimerVoltLow, TimerVoltHigh;
   static uint16 TimerVoltDTC;
   
	switch(DTCVoltState)
	{
	default:
	case  DTC_VOLT_INIT:
		if (TimeOutChkMsLong(&TimerVoltDTC, DTCVOLT_INIT_DELAY))
		{  //初始化1500ms
			if (i16Volt > DTC_HVOLT_DISABLE || i16Volt < DTC_LVOLT_DISABLE)
			{
				DTCVoltState = DTC_VOLT_ERR; 
			}
			else
			{
				DTCVoltState = DTC_VOLT_OK;
			}
		}
		else{}
		break;
	case  DTC_VOLT_OK:
		if (i16Volt > DTC_HVOLT_DISABLE || i16Volt < DTC_LVOLT_DISABLE)
		{
			DTCVoltState = DTC_VOLT_ERR; 
		}
		else{}
		break;
	case  DTC_VOLT_ERR:
		if(i16Volt < DTC_HVOLT_ENABLE && i16Volt > DTC_LVOLT_ENABLE)
		{
			(void)TimeOutChkMsLong(&TimerVoltDTC, 0) ;
			DTCVoltState = DTC_VOLT_TIME; 
		}
		else{}
		break;
	case  DTC_VOLT_TIME:
		if (i16Volt > DTC_HVOLT_DISABLE || i16Volt < DTC_LVOLT_DISABLE)
		{
			(void)TimeOutChkMsLong(&TimerVoltDTC,0) ;  
		}
		else{}
		if(TimeOutChkMsLong(&TimerVoltDTC, DTCVOLT_RECOVER_DELAY)) 
		{
			DTCVoltState = DTC_VOLT_OK;  
		}
		else{}
		break;
	}
	
	if (i16Volt < DTC_EVOLT_LOW_L)
	{//认为电源关闭，DTC 不用在判断，
		DTCVoltState = DTC_VOLT_INIT;
		if (u8DTCCycState == DTCCYC_STATE_WORK)
		{
			u8DTCCycState = DTCCYC_STATE_OVER;
			End_DTCCycle();
		}
		else{}
	}
	else if (i16Volt > DTC_LVOLT_ENABLE)
	{
		if (u8DTCCycState == DTCCYC_STATE_OVER)
		{
			u8DTCCycState = DTCCYC_STATE_WORK;
			Begin_DTCCycle();
		}
		else{}
	}
	else{}
	
	
	//以下实现低电压故障的功能
	i16Volt = ADPhy_GetFact(DTC_EVOLT_ADID);
	if (i16Volt < DTC_EVOLT_LOW_H && i16Volt > DTC_EVOLT_LOW_L)
	{
		if (TimeOutChkMsLong(&TimerVoltLow, DTC_EVOLT_TIME))
		{
			u8VoltLow = DTC_RETSTATE_ERR;
		}else{}
	}
	else
	{
		(void)TimeOutChkMsLong(&TimerVoltLow, 0);
		
		if (i16Volt > DTC_EVOLT_LOW_OK)
		{
			u8VoltLow = DTC_RETSTATE_OK;
		}else{}
	}
	
	//以下实现高电压故障的功能
	if (i16Volt > DTC_EVOLT_HIGH)
	{
		if (TimeOutChkMsLong(&TimerVoltHigh, DTC_EVOLT_TIME))
		{
			u8VoltHigh = DTC_RETSTATE_ERR;
		}
		else{}
	}
	else
	{
		(void)TimeOutChkMsLong(&TimerVoltHigh, 0);
		
		if (i16Volt < DTC_EVOLT_HIGH_OK)
		{
			u8VoltHigh = DTC_RETSTATE_OK;
		}
		else{}
	}
}
 /*******************************************************************************
 * Function:   BOOL Init_DealDTC(uint8 MaxNum)
 *
 * Description:  故障码处理初始化函数
 *
 * Returns:      设定成功 返回：1
 *
 * Notes:       MaxNum 最大个数  
 *
*******************************************************************************/
BOOL Init_DealDTC(uint8 MaxNum)
{
   BOOL bInitOK;
	if (MaxNum <= DTC_MAXNUM)
   {
      DTCTotalNum = MaxNum ;
      u8DTCCycState = DTCCYC_STATE_WORK;
      Begin_DTCCycle();
      bInitOK = TRUE;
   }
	else
	{
		bInitOK = FALSE;
	}
	DTCVoltState = DTC_VOLT_INIT;
	
	u8DTCCycState = 0;
	return bInitOK;
}


/*******************************************************************************
* Function:   void DTC_AgeDTC(void))
*
* Description:  进行老化一次
*
* Returns:      void
*
* Notes:     模块静态函数
*
*******************************************************************************/
static void DTC_AgeDTC(void)
{
  uint8 i;
  
	for (i = 0; i< DTC_MAXNUM; i++)
	{
     //DTC[num] |= DTCB_FAILED_THISOC;
		if ( (DTC[i] & DTCB_TESTNOTCOMPLETED) == 0)
		{//已测试完成
			if (DTC[i] & DTCB_FAILED_THISOC) 
			{ //有故障
				DTCSnap[i].DTCAgeTimes = 0; 
			}
			else
			{//ageing
				if (DTCSnap[i].DTCAgeTimes >= DTC_AGE_TIMES)
				{
					DTC[i] &= ~DTCB_TESTFAILED_SINCECLEAR;
					DTC[i] &= ~DTCB_CONFIRM;  //ageing dtc
				}
				else
				{
					DTCSnap[i].DTCAgeTimes++;
				}
			  
				DTCSnap[i].DTCPendTimes = 0;
				DTC[i] &= ~DTCB_PENDING;
		  }
		}
		else
		{
   	  
		}
	}

#if   _SAVE_DTC_
   bSaveDTC = 1;       //重新记忆
#else
	Save_DTC() ;    
#endif 
}

 /*******************************************************************************
 * Function:   void Begin_NewCycle(void)
 *
 * Description:  开始新的循环
 *
 * Returns:      none
 *
 * Notes:     初始值： DTCB_TESTFAILED = 0    DTCB_FAILED_THISOC=0
 *            DTCB_PENDING  DTCB_CONFIRM 记忆值     DTCB_TESTNOTCOMPLETED_SINCECLEAR = 1（清除）或者记忆
 *            DTCB_TESTFAILED_SINCECLEAR 记忆值     DTCB_TESTNOTCOMPLETED = 1
*******************************************************************************/
static void Begin_DTCCycle(void)
{
	uint8 i;

	if (Load_DTC())
	{	//载入正确，
#if   _SAVE_DTC_
		//不用下电存储，则上电老化一次
		DTC_AgeDTC();	
#endif
	}
	else
	{
	}
	
	for (i = 0; i < DTCTotalNum; i++)
	{
		DTC[i] &= ~DTCB_TESTFAILED;  
		DTC[i] &= ~DTCB_FAILED_THISOC; 
		DTC[i] |= DTCB_TESTNOTCOMPLETED;   
		nErrTimes[i] = 0;      
	}  
}

 /*******************************************************************************
 * Function:   void  ClearDTC(uint8 nId)
 *
 * Description:  清除dtc
 *
 * Returns:      none
 *
 * Notes:     nId 第一个Id
*******************************************************************************/
void  ClearDTC(uint8 nId)
{
   uint8 i;
   if (nId == 0xff)
   {
      for(i = 0; i < DTCTotalNum; i++)
      {
         DTC[i] = DTCB_TESTNOTCOMPLETED_SINCECLEAR | DTCB_TESTNOTCOMPLETED;   
         nErrTimes[i] = 0;
         DTCSnap[i].DTCPendTimes = 0;
      }
#if   _SAVE_DTC_
      bSaveDTC = 1;       //重新记忆
#endif
   }
   else if (nId < DTCTotalNum) 
   {
      DTC[nId] = DTCB_TESTNOTCOMPLETED_SINCECLEAR | DTCB_TESTNOTCOMPLETED;   
      nErrTimes[nId] = 0;
      DTCSnap[nId].DTCPendTimes = 0;
#if   _SAVE_DTC_
      bSaveDTC = 1;       //重新记忆
#endif
   }
   else{}
}

 /*******************************************************************************
 * Function:   uint8 Deal_DTC(void))
 *
 * Description:  故障码处理，将此函数放在main-loop 中
 *
 * Returns:      处理了返回：1 ， 时间未到返回0， 没有处理返回0xff
 *
 * Notes:   
 *
*******************************************************************************/
uint8 Deal_DTC(void)
{
   static uint16  TimerDealDTC;
   static uint16 u16SaveTimes;
   uint8 num;
   eDTCRETSTATE u8RetState;
	uint8 u8DealResult = 0;
   if (TimeOutChkMsLong(&TimerDealDTC, DTCDEAL_TIME))
   {	
   	u16SaveTimes++;
   	DeaVolt_DTC();
   	if (DTCVoltState == DTC_VOLT_INIT)
   	{
   		u8DealResult = 0xfe;
   	}
   	else
   	{
	#if   _PROJECT_CAN_   
			if (!DTCStorGet())
			{//如果不能设置.，则返回0xff
				u8DealResult = 0xff;
			}
			else
	#endif
			{
				for(num = 0; num < DTCTotalNum; num++)
				{
					if (num == DTCNUM_VOLTAGE_LOW)
					{
						u8RetState = u8VoltLow;
					} 
					else if (num == DTCNUM_VOLTAGE_HIGH)
					{
						u8RetState = u8VoltHigh;
					}
					else
					{
						if (DTCVoltState == DTC_VOLT_OK)
						{
							u8RetState = GetDTCState(num);
						}
						else
						{
							u8RetState = DTC_RETSTATE_WAITE;
						}
					}
					//u8RetState get end
					
					if (u8RetState == DTC_RETSTATE_ERR || u8RetState == DTC_RETSTATE_IMMEDI_ERR || u8RetState == DTC_RETSTATE_COMFIRM_ERR) 
					{
						if ( (nErrTimes[num] >= (DTC_FAILED_TIMES - DTC_FAILED_STEP)) ||  u8RetState == DTC_RETSTATE_IMMEDI_ERR || u8RetState == DTC_RETSTATE_COMFIRM_ERR) 
						{  //本次测试失败已达到 
							nErrTimes[num] = DTC_FAILED_TIMES;
							if ( !(DTC[num] & DTCB_FAILED_THISOC))
							{//本次循环第一次故障
								DTC[num] |= DTCB_FAILED_THISOC;      //本次测试故障
								
								DTCSnap[num].DTCPendTimes++;
								if (DTCSnap[num].DTCPendTimes >= DTC_CONFIRM_TIMES || u8RetState == DTC_RETSTATE_COMFIRM_ERR)
								{
									DTCSnap[num].DTCPendTimes = DTC_CONFIRM_TIMES;
									DTC[num] |= DTCB_CONFIRM;
									ReCord_DTCSnap(num);  
								}
								else{}
								
								DTC[num] |= DTCB_PENDING; 
								  
							#if   _SAVE_DTC_
								bSaveDTC = 1 ;       //重新记忆
							#endif
							}
							else
							{//已经故障过了
								
							}
							
							DTC[num] |= DTCB_TESTFAILED;         //当前测试失败
							DTC[num] &= ~DTCB_TESTNOTCOMPLETED;  //已完成
							
							if (DTC[num] & DTCB_TESTNOTCOMPLETED_SINCECLEAR)
							{
								DTC[num] &= ~DTCB_TESTNOTCOMPLETED_SINCECLEAR ; //已完成，清0 ，需要记忆 
							#if   _SAVE_DTC_
								bSaveDTC = 1 ;       //重新记忆
							#endif  
							}
				
							if (!(DTC[num] & DTCB_TESTFAILED_SINCECLEAR) )
							{//自从清除后又故障
								DTC[num] |= DTCB_TESTFAILED_SINCECLEAR;
							#if   _SAVE_DTC_
								bSaveDTC = 1 ;       //重新记忆
							#endif 
							}
						}
						else
						{ 
							nErrTimes[num]+= DTC_FAILED_STEP; 
						}
					}
					else if (u8RetState == DTC_RETSTATE_OK || u8RetState == DTC_RETSTATE_IMMEDI_OK) 
					{//没有故障
						if ( nErrTimes[num] <= (DTC_PASSED_TIMES + DTC_PASSED_STEP) || u8RetState == DTC_RETSTATE_IMMEDI_OK )    // DTC_PASSED_TIMES 此数为-
						{//没有故障已经到一个点
							nErrTimes[num] = DTC_PASSED_TIMES;
							// DTCB_FAILED_THISOC;DTCB_PENDING,DTCB_CONFIRM,DTCB_TESTFAILED_SINCECLEAR   //不能清除
							DTC[num] &= ~DTCB_TESTFAILED; 
							DTC[num] &= ~DTCB_TESTNOTCOMPLETED;//已完成,请0
							if (DTC[num] & DTCB_TESTNOTCOMPLETED_SINCECLEAR)
							{ //已完成，清0 ，需要记忆
								DTC[num] &= ~DTCB_TESTNOTCOMPLETED_SINCECLEAR ;
							#if   _SAVE_DTC_
								bSaveDTC = 1 ;       //重新记忆
							#endif  
							}
						}
						else
						{
							nErrTimes[num] -= DTC_PASSED_STEP;  
						}
					}
					else
					{//u8RetState == DTC_RETSTATE_WAITE 或者其他情况
						
					}
				}//for
				u8DealResult = 1;
			}//UDS DTC not get
   	}//dtc state init
   }//8ms
   
#if   _SAVE_DTC_
   if (u16SaveTimes >= SAVE_CYC_TIMES)
   {
   	u16SaveTimes = 0;
      if (bSaveDTC && DTCVoltState == DTC_VOLT_OK)
      {//电压正常下才进行存储
      	bSaveDTC = 0;
      	Save_DTC();
      }
   }
   else{}
#endif
   return u8DealResult;
}



 /*******************************************************************************
 * Function:   void End_DTCCycle(void))
 *
 * Description:  本次循环结束，开始老化
 *
 * Returns:      void
 *
 * Notes:   
 *
*******************************************************************************/
static void End_DTCCycle(void)
{
#if   _SAVE_DTC_
	//如果使用上电老化则不需要进行存储
#else
	DTC_AgeDTC();
#endif
}


