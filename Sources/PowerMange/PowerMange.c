/************************文件说明*****************************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：PowerMange.c
* 功能描述：电源管理模块主功能文件，此文件不能被更改
* 1  电源状态切换 
* 2  休眠，唤醒
* 3  网络管理调用
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 此文件不能被更改
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月19日
* 
* 版本：1.1
* 说明：
* 1 增加_PM_USECAN_，支持没有CAN 的项目
* 2 将电源低故障时间分为2类
* 1） PM_LIGN_ERR_TIME 低电压时的延时时间
* 2） PM_HIGN_ERR_TIME 高电压时的延时时间
* 3） 增加ApplPM_SleepEnter，ApplPM_SleepReady回调函数
* 作者：xb
* 完成日期：2019年3月30日
* 
* 版本：1.2
* 说明：
* 1) 增加备注说明
* 作者：xb
* 完成日期：2019年10月21日
* 
* 版本：1.3
* 说明：
* 1) 通过ApplPM_StateChange 来通知用户，其他舍去
* 作者：xb
* 完成日期：2019年12月11日

* 版本：1.4
* 说明：
* 1) 支持autosar NM
* 2) 增加_PM_AUTOSARNM_ 宏定义
* 作者：xb
* 完成日期：2019年12月11日
* 
* 版本：1.5
* 说明：
* 1) 电源管理初始化时需要调用CANWake_Begin
* 2) 
* 作者：xb
* 完成日期：2020年3月7日
* 
* 版本：1.6
* 说明：
* 1) 电源管理在standby可返回到off
* 作者：xb
* 完成日期：2020年5月30日
*/

#include "LIBDriveDefine.h"
#include "PowerMange.h"

#if _PM_USECAN_
#include "CanDrive.h"
#if _PM_OSKNM_
	#include "OsekNm.h"
#endif
#if _PM_AUTOSARNM_
	#include "PMAssist_AutoSar.h"
#endif
#if _PM_INDIRECTNM_
	#include "IndirectNM.h"
#endif
#endif


static PMStateType u8PowerState;
static uint16 TimerIgnErr, TimerIgnOK;
static uint8 bIgnWorkState, bIgnWorkStateLast;
static uint8 bCANVoltState;
static uint8 u8AppSleepEn = 1;  //默认可以直接休眠
static uint16 TimerPowerState;
static uint8 u8CANSleepEn;


//外部接口函数
extern void  DealForWakeUp(void);
extern void  DealForSleep(void);
//电源管理状态转换宏定义
#define  PMStateChange(state)  ApplPM_StateChange(u8PowerState, state); \
											u8PowerState = state
/*******************************************************************************
 * Function:       void PowerMange(void)
 *
 * Description:    电源管理主控程序，
 *
 * Returns:         none
 *
 * Notes:           放在主循环
 *
********************************************************************************/
void PowerMange(void)
{
	int16 i16IgnVolt;
	static uint8 bIgnErr = PM_VOLT_OK;
	i16IgnVolt = ADPhy_GetFact(ADID_VOLT);
	uint8 u8IgnOkTimes;
	uint16 u16IgnErrTime;
	//CAN 唤醒状态处理
#if _PM_USECAN_
	Deal_CANWake();
#endif
	if (i16IgnVolt > IGNVOLT_WORK_ERRU || i16IgnVolt < IGNVOLT_WORK_ERRD)
	{
	   bIgnErr = PM_VOLT_ERR;
	}
	if (i16IgnVolt < IGNVOLT_WORK_OKU && i16IgnVolt > IGNVOLT_WORK_OKD)
	{
		bIgnErr = PM_VOLT_OK;
	}
	bIgnErr = PM_VOLT_OK;
   //300ms  延时关机逻辑， 
   if (bIgnErr == PM_VOLT_OK)
   { 
   	(void)TimeOutChkMsLong(&TimerIgnErr, 0);
      if (TimeOutChkMsLong(&TimerIgnOK, PM_IGN_OK_TIME))
      {
      	bIgnWorkState = PM_VOLT_OK;
      }
      
      bIgnWorkStateLast = PM_VOLT_OK;
   }
   else
   {
      if (i16IgnVolt > IGNVOLT_WORK_OKU)
      {//高压的时候	
      	u16IgnErrTime = PM_HIGN_ERR_TIME;
      }
      else
      {//低压的时候
      	u16IgnErrTime = PM_LIGN_ERR_TIME;
      }

   	if (TimeOutChkMsLong(&TimerIgnErr, u16IgnErrTime))
      {
      	bIgnWorkState = PM_VOLT_ERR;
      }
   	else{}
   	
      (void)TimeOutChkMsLong(&TimerIgnOK, 0);
      
      if (bIgnWorkStateLast != PM_VOLT_ERR)
      {
      	bIgnWorkStateLast = PM_VOLT_ERR;
      }
   }
#if _BATT_COLLECT_ 
	int16 i16BattVolt;
	i16BattVolt = Get_Ad_Fact(ADID_VOLT);
   if(i16BattVolt>CANVOLT_HIGHERR ||i16BattVolt<CANVOLT_LOWERR)//保护状态时暂时定义不发CAN
   {
   	bCANVoltState=0;
   }
   if(i16BattVolt < CANVOLT_HIGHOK && i16BattVolt > CANVOLT_LOWOK)
   {
   	bCANVoltState=1;
   }
#else
   bCANVoltState = 1;
#endif
   
   switch (u8PowerState)
   {
	default:
   	case POWER_STATE_INIT:
   		if (TimeOutChkMsLong(&TimerPowerState, 500))
   		{
				#if _PM_USECAN_
   				CANWake_Begin(1);
				#endif
   			if (bIgnWorkState == PM_VOLT_OK)
				{
					PMStateChange(POWER_STATE_WORK);
				}
   			else
   			{ 
					PMStateChange(POWER_STATE_OFF);
   			}
   		}
   		break;
   	case  POWER_STATE_WORK:
#if  _OPENSDA_CAN
#else
   		if (bIgnWorkState == PM_VOLT_ERR)
   		{
				PMStateChange(POWER_STATE_OFF);
   		}else{}
#endif
   		break;
   	case  POWER_STATE_OFF:
   		if (bIgnWorkState == PM_VOLT_OK)
   		{
   			PMStateChange(POWER_STATE_WORK);
   		}
   		else
   		{
   			
   		}
   		
   		if (u8AppSleepEn)
   		{//应用以完成,可以睡眠
   			if (i16IgnVolt < IGN_VOLT_SLEEP)
   			{
#if   _PM_OSKNM_
   				CanNm_GotoMode(0, GOTO_BUSSLEEP);
#elif _PM_AUTOSARNM_
   				CanNm_NetworkRelease(0);
#endif   	
   				PMStateChange(POWER_STATE_STANDBY);
   			}
   			else{}
   		}
   		else{}
   		break;
   	case POWER_STATE_STANDBY:
   		if (bIgnWorkState == PM_VOLT_OK)
   		{
#if _PM_OSKNM_
   			CanNm_GotoMode(0, GOTO_AWAKE);
#elif _PM_AUTOSARNM_
   			CanNm_NetworkRequest(0);	
#endif
   			u8CANSleepEn = 0;  //应该等待重新指示位
   			PMStateChange(POWER_STATE_WORK);
   		}
   		else
   		{
   			
   		}

   		if (u8AppSleepEn == 0)
   		{//应用有唤醒请求,yanz
#if  _PM_AUTOSARNM_
   			CanNm_NetworkRequest(0);	
#endif
   			u8CANSleepEn = 0;  //应该等待重新指示位
   			PMStateChange(POWER_STATE_OFF);
   		}
   		else
   		{
   			
   		}

#if (_PM_OSKNM_	|| _PM_AUTOSARNM_)
   		if (u8CANSleepEn)
   		{
   			PMStateChange(POWER_STATE_STANDBY_WAIT);
   		   (void)TimeOutChkMsLong(&TimerPowerState, 0);
   			//进入休眠模式的处理
   			CANWake_Begin(0);
   			CANDrive_GotoSleep();   //candrive 切换为sleep模式
   		}
   		else
   		{//一直等待睡眠指示位，或者点火正常后正常工作
   			
   		}
#elif   _PM_USECAN_
//没有osek时，休眠标记位有无，由CAN 信号来确定
   		if (GetCanWakeEn() == 0)
   		{//没有报文
   			PMStateChange(POWER_STATE_STANDBY_WAIT);
   		   (void)TimeOutChkMsLong(&TimerPowerState, 0);
   			CANWake_Begin(0);
   			CANDrive_GotoSleep();
   		}
#else
			PMStateChange(POWER_STATE_STANDBY_WAIT);
			(void)TimeOutChkMsLong(&TimerPowerState, 0);
#endif
   		break;
   	case POWER_STATE_STANDBY_WAIT:
   		if (TimeOutChkMsLong(&TimerPowerState, 100))
   		{
#if _PM_USECAN_
   			CANWake_Begin(1);
#endif
   			PMStateChange(POWER_STATE_SLEEP);
   			//直接进入sleep loop
   		}
   		else
   		{
   			break;
   		}
   	case POWER_STATE_SLEEP:
   		u8IgnOkTimes = 0;
   		do{
      		//(void)DisableInterrupt();
   			
   			DealForSleep();  //进入休眠 ，硬件驱动层实现
#if _PM_USECAN_		
   			Deal_CANWake();
      		if (GetCanWakeEn())
				{//can唤醒
      			break;
				}
      		else
      		{

      		}
#endif
   			if(ReadIgn_Only(ADID_VOLT) > IGN_VOLT_WAKEUP)
   			{//只读取点火ad ，正常则大于休眠电压返回1
   				u8IgnOkTimes++;
   			}
   			else
   			{
   				u8IgnOkTimes=0;
   			}     //连续5次信号有效才能恢复点火，时间为64*5=320ms
   		}while(u8IgnOkTimes<1);
#if _PM_USECAN_
   		if (GetCanWakeEn())
   		{//如果是CAN 唤醒
#if   _POWERM_CANWAKE_RESET_
				DealForWakeUp();    //此处上电直接复位，可能应用时间
#else
				PMStateChange(POWER_STATE_INIT);
				CANDrive_GotoAWake();
				PowerNM_Init();
				(void)TimeOutChkMsLong(&TimerPowerState, 0);
#endif
   		}
   		else
#endif
   		{//点火唤醒  			
   			DealForWakeUp();
   		}
   		break;
   }
   
}

/*******************************************************************************
 * Function:      uint8 PowerMN_GetPowerState(void)
 *
 * Description:    返回电源管理的状态
 *
 * Returns:         none
 *
 * Notes:           app调用
 *
********************************************************************************/
uint8 PowerMN_GetPowerState(void)
{
	return u8PowerState;
}
/*******************************************************************************
 * Function:      uint8 PowerMN_GetPowerState(void)
 *
 * Description:    返回电源状态,经过滤波延时之后的结果
 *
 * Returns:         none
 *
 * Notes:           app有需要时调用
 *
********************************************************************************/
uint8 PowerMN_GetIgnState(void)
{
	return bIgnWorkState;
}

/*******************************************************************************
 * Function:       void PowerNM_Init(void)
 *
 * Description:    电源管理初始化
 *
 * Returns:         none
 *
 * Notes:           app调用
 *
********************************************************************************/
void PowerNM_Init(void)
{
	PMStateChange(POWER_STATE_INIT);
#if _PM_USECAN_
	CANDrive_GotoAWake();
#endif
#if _INDIRECT_NM_
	IndirectNM_Init();
#endif
#if _PM_OSKNM_
	CanNm_Init();
	CanNm_GotoMode(0, GOTO_AWAKE);
#endif
#if _PM_AUTOSARNM_
	PMAssistAutosar_InitNM();
#endif
	u8CANSleepEn = 0;
}

/*******************************************************************************
 * Function:       void PowerMN_AppSleepEn(uint8 bEn)
 *
 * Description:    设置app 是否已经可以休眠
 *
 * Returns:         none
 *
 * Notes:           app调用
 *
********************************************************************************/
void PowerMN_AppSleepEn(uint8 bEn)
{
	u8AppSleepEn = bEn;	
}
/*******************************************************************************
 * Function:      void PowerMN_CanSleepEn(uint8 bEn)
 *
 * Description:    设置CAN是否已经可以休眠
 *
 * Returns:         none
 *
 * Notes:           app调用
 *
********************************************************************************/
void PowerMN_CanSleepEn(uint8 bEn)
{
	u8CANSleepEn = bEn;	
}
