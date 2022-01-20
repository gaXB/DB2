/**************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：KeyPadLogic.c
* 功能描述：实现按键，命令，旋钮等命令
* 1 改变 SystemControl 
* 
* 编译器： codewarrior IDE 11.1
* MCU说明： 与MCU无关，基本不用更改
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
* 
* 
* * 版本：1.1
* 说明：led 显示版本号调用方式变更
* 作者:lyc
* 完成日期：2020年5月29日
*
*/
#include "APPDrive.h"
#include "KeyPadLogic.h"
#include "SystemSave.h"

//外部函数声明
extern void System_GOTODiagTask(void);
static uint8 bEnDebug;

void  TestMode_restrict(void)
{
	LevelModeType u8LevelMode;
	u8LevelMode = GetTSETLevelMode(SystemControl.u8TestLevel);

	switch(u8LevelMode)
	{
		default:
			break;
		case LEVELMODE_COOL:
			SystemControl.ptcMode = 0;
			break;
		case LEVELMODE_COOLDEF:
			SystemControl.ptcMode = 0;
			if (SystemControl.fanMode >= 4)
			{
				SystemControl.fanMode = 4;
			}else{}
			break;
		case LEVELMODE_HOTDEF:
			if (SystemControl.fanMode >= 4)
			{
				SystemControl.fanMode = 4;
			}else{}
			break;
		case LEVELMODE_HPMODE:
			SystemControl.ptcMode = 0;
			break;
		case LEVELMODE_WPTC:
			SystemControl.acMode = 0;
			break;

	}
}

void DealTestSwitch(void)
{
	if (SystemControl.u8TestLevel == TSETTEM_MODE_HOTDEF_1)
	{
		SystemControl.mdMode = MD_MODE_OSF;
	}
	else 	if (SystemControl.u8TestLevel == TSETTEM_MODE_HOTDEF_2)
	{
		SystemControl.mdMode = MD_MODE_OSFD;
	}
	else 	if (SystemControl.u8TestLevel == TSETTEM_MODE_HOTDEF_3)
	{
		SystemControl.mdMode = MD_MODE_OSD;
	}
}

/*******************************************************************************
 * Function:   void  Deal_Keypad_Code (void)
 *
 * Description: 将按键缓存数据读取并处理
 *
 * Returns:     none
 *
 * Notes:  主任务的，按键切换功能。
 *         根据按键及客户提供的逻辑功能, 改变系统主要控制变量 Systemcontrol
 *           
*******************************************************************************/
uint8 GetCurrentKey(void);
void Deal_Keypad_Normal(void)
{
	uint16 keywd;
	uint8 u8LevelMode;
	uint8 u8LevelSet;
	//uint8 u8FanLevel = sDisplayData.FANLevel;
	//TODO 按照编码器的值进行设置，请参考模板
	uint8 u8FanLevel = sDisplayData.FANLevel;
	
	keywd = ReadComd();
	
	switch((uint8)keywd)
	{
		default:
			break;
		case COMD_TSET_ADD:
			if(!SystemControl.OffState)
			{
				u8LevelSet = SystemControl.u8TestLevel + 1;
				u8LevelMode = GetTSETLevelMode(u8LevelSet);
				u8LevelMode = GetTSETLevelMode(u8LevelSet);
				if (u8LevelSet > TSETTEM_MODE_DEBUG)
				{
					//SystemControl.u8TestLevel = TSETTEM_MODE_COOL_1;
				}
				else if (sVehicleControl.bBattCoolReq || sVehicleControl.bBattHotReq)
				{//电池有需求时
				   if (u8LevelMode == SystemControl.LevelMode)
					{//同一模式下切换可以切
						SystemControl.u8TestLevel = u8LevelSet;
					}else{}
				}
				else if (u8LevelMode == LEVELMODE_DEBUG)
				{//进入调试模式
					if (bEnDebug)
					{
						SystemControl.u8TestLevel = u8LevelSet;
					}else{}
				}
				else
				{
					SystemControl.u8TestLevel = u8LevelSet;
				}
				DealTestSwitch();
			}
			break;
		case COMD_TSET_DEC:
			if(!SystemControl.OffState)
			{
				if (SystemControl.u8TestLevel > 1)
				{
					u8LevelSet = SystemControl.u8TestLevel - 1;
				}
				else
				{
					u8LevelSet = 1;
				}

				u8LevelMode = GetTSETLevelMode(u8LevelSet);
				if (u8LevelSet < TSETEM_MODE_JIAZHU)
				{
					SystemControl.u8TestLevel = TSETTEM_MODE_COOL_1;
				}
				else if (sVehicleControl.bBattCoolReq || sVehicleControl.bBattHotReq)
				{//电池有需求时
				   if (u8LevelMode == SystemControl.LevelMode)
					{//同一模式下切换可以切
						SystemControl.u8TestLevel = u8LevelSet;
					}else{}
				}
				else if (u8LevelMode == LEVELMODE_DEBUG)
				{//进入调试模式
					if (bEnDebug)
					{
						SystemControl.u8TestLevel = u8LevelSet;
					}else{}
				}
				else
				{
					SystemControl.u8TestLevel = u8LevelSet;
				}
				DealTestSwitch();
			}
			break;
		case COMD_FAN_ADD:
			  if(!SystemControl.OffState)
			  {
				 u8FanLevel++;
				 if(u8FanLevel>MAX_FELVL) u8FanLevel= MAX_FELVL;
				 SystemControl.fanMode =  u8FanLevel;
			  }
			  else
			  {
		   //      Load_Off();
				 SystemControl.OffState = 0;
				 if(SystemControl.mdMode == MD_MODE_OST)
				 {//退出off时，没有def状态
					Load_Def();
				 }
				// SystemControl.fanMode = FAN_MODE_1L;
			  }
			  SystemControl.AutoBits.FAN =0;
			  (void)FanControl_SetRate(FANVOLT_RATE_MAN);
			  Def_Operat(DEF_OPTYPE_FAN);
			  break;
		case  COMD_FAN_DEC:
			  if(!SystemControl.OffState)
			  {
				 if(u8FanLevel > MIN_FELVL )
				 {
					u8FanLevel--;
					SystemControl.fanMode = u8FanLevel;
				 }
				 else
				 {    //关机
				  //  Save_Off();
					 SystemControl.fanMode = MIN_FELVL;
				 //   SystemControl.OffState = 1;
				 }
			  }
			  else
			  {
				 SystemControl.OffState = 0;
				 if(SystemControl.mdMode == MD_MODE_OST)
				 {//退出off时，没有def状态
					Load_Def();
				 }

				 //SystemControl.fanMode = MIN_FELVL;
				// SystemControl.fanMode = FAN_MODE_1L;
			  }
			  (void)FanControl_SetRate(FANVOLT_RATE_MAN);
			  SystemControl.AutoBits.FAN =0;
			  Def_Operat(DEF_OPTYPE_FAN);
			  break;
	}
	TestMode_restrict();
	extern uint16 KeyPadHW_ReadSignal(void);

	static uint16 TimerDebugKey;
	//keywd = GetCurrentKey();
#if 0//PCDEBUG_CONFIG > 1
	bEnDebug = 1;
#else
	if (sSenSorData.u8VehicleSpeed >= 5)
	{
		if (bEnDebug == 1)
		{
			bEnDebug = 0;
		}else{}

		if  (SystemControl.u8TestLevel != 1 && SystemControl.u8TestLevel != 2 && SystemControl.u8TestLevel != 23)
		{

		}
		else
		{
			SystemControl.u8TestLevel = TSETTEM_MODE_COOL_1;
		}
	}
	else{}


	if  (SystemControl.u8TestLevel != 1 && SystemControl.u8TestLevel != 2 && SystemControl.u8TestLevel != 23)
	{
		if (TimeOutChkTenthSLong(&TimerDebugKey, 20))
		{
			if (bEnDebug == 1)
			{
			//	SystemControl.u8TestLevel = TSETTEM_MODE_COOL_1;
				bEnDebug = 0;
			}else{}
		}
	}
	else{}

#endif

	//TODO:读取按键,并按键值进行状态切换，请参考模板
	keywd = Read_Keydate();
	if(!keywd)
	{
		return;
	}else{}

	switch(keywd)
	{
		default:
			break;
			//电源按键
		case KEY_OFF	:
			if(!SystemControl.OffState)
			{//进入off
				Save_Off();
				SystemControl.OffState = 1;
			}
		 break;
		case KEY_ON	:
			if(SystemControl.OffState == OFF_STATE_OFF)
			{//进入off
				Load_Off();
				SystemControl.OffState = 0;
			}
		 break;
		case 	KEY_DEBUG:
			bEnDebug = 1;
			(void)TimeOutChkTenthSLong(&TimerDebugKey, 0);
			break;
		case 	KEY_RHEAT:
			if (SystemControl.RdefMode == 1)
			{
				SystemControl.RdefMode = 0;
			}
			else
			{
				SystemControl.RdefMode = 1;
			}
			break;
		//****************************************
		//rec/fre
		case 	KEY_NCF:
			if (SystemControl.ncfMode==NCF_MODE_NEW)
			{
				SystemControl.ncfMode = NCF_MODE_CIRF;
			}
			else
			{
				SystemControl.ncfMode = NCF_MODE_NEW;
			}
			SystemControl.AutoBits.NCF =0;
			SystemControl.MaxACMode = AC_MAX_MODE_OFF ; //Acmax 关闭；
			Def_Operat(DEF_OPTYPE_NCF);
			break;
		case KEY_PTC	:
		  if(!SystemControl.OffState)
		  {
			  if(SystemControl.ptcMode == 1)
			  {
					SystemControl.ptcMode = 0;
			  }
			  else
			  {
					SystemControl.ptcMode = 1;
			  }
		  }
		  else
		  {
			  //ac 按键不开机
			  Load_Off();
			  SystemControl.ptcMode = 1;
			}
			break;
		case KEY_AC	:
		  if(!SystemControl.OffState)
		  {
			  if(SystemControl.acMode == AC_MODE_ON)
			  {
					SystemControl.acMode = AC_MODE_OFF;
			  }
			  else
			  {
					SystemControl.acMode = AC_MODE_ON;
			  }
			  SystemControl.AutoBits.AC = 0;
		  }
		  else
		  {
			  //ac 按键不开机
			  Load_Off();
			  SystemControl.AutoBits.AC = 0;
			  SystemControl.acMode = AC_MODE_ON;
			}
			Def_Operat(DEF_OPTYPE_AC);
			//SystemControl.MaxACMode = AC_MAX_MODE_OFF ; //Acmax 关闭；
			break;
		case KEY_MODE:
			if(SystemControl.OffState )
			{
				  //off时切换模式
				  //off时除霜状态不记忆
				Load_Off();
				SystemControl.AutoBits.FAN =0;
				SystemControl.fanMode = FAN_MODE_1L;

				SystemControl.mdMode++;
				if (SystemControl.mdMode >= MD_MODE_OST)
				{
					SystemControl.mdMode = MD_MODE_OSF;
				}
				else{}
			}
			else
			{
				if(SystemControl.mdMode == MD_MODE_OST)       //out ost
				{
					Load_Def();
				}
				else
				{
					SystemControl.mdMode++;
					if (SystemControl.mdMode >= MD_MODE_OST)
					{
						SystemControl.mdMode = MD_MODE_OSF;
					}
					else{}
				}
			}
			Def_Operat(DEF_OPTYPE_MODE);
			SystemControl.AutoBits.MODE = 0;
			break;

		//****************************************
		//def 除霜
			 //****************************************
		case KEY_OST :
			if(SystemControl.OffState )
			{
				//关机的情况
				Load_Off();
				if(SystemControl.mdMode == MD_MODE_OST)
				{
					SystemControl.mdMode = MD_MODE_OSF;
				}

				Save_Def();

				SystemControl.mdMode  = MD_MODE_OST;
				SystemControl.ncfMode = NCF_MODE_NEW;
				SystemControl.acMode  = AC_MODE_ON;
				SystemControl.AutoBits.AC = 0;
				//SystemControl.AutoBits.NCF = 0;
				SystemControl.AutoBits.MODE = 0;
				SystemControl.AutoBits.FAN = 0;

				if (SystemControl.fanMode < FAN_MODE_5L)
				{
					SystemControl.fanMode = FAN_MODE_5L;
					(void)FanControl_SetRate(FANVOLT_RATE_INIT);
				}
				else{}
			}
			else
			{
				//开机的情况
				if(SystemControl.mdMode == MD_MODE_OST)
				{
					Load_Def();
				}
				else
				{
					Save_Def();
					if (SystemControl.fanMode < FAN_MODE_5L)
					{
						SystemControl.fanMode = FAN_MODE_5L;
						(void)FanControl_SetRate(FANVOLT_RATE_INIT);
					}

					SystemControl.mdMode = MD_MODE_OST;
					SystemControl.ncfMode = NCF_MODE_NEW;
					SystemControl.acMode = AC_MODE_ON;
					SystemControl.AutoBits.AC = 0;
					SystemControl.AutoBits.NCF = 0;
					SystemControl.AutoBits.MODE = 0;
					SystemControl.AutoBits.FAN = 0;
				}
			}
			//sHPOutControl.Byte = 0;
			SystemControl.MaxACMode = AC_MAX_MODE_OFF ; //Acmax 关闭；
			break;
		//*********************显示******************	     
	#if  _PROJECT_LCD_
//		case  KEY_AC + 0xff:
//		  //显示 蒸发 时间
//			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
//			{
//				DisTsetShakeSet(sSenSorData.TevpCal) ;
//			}
//			break;
		case  KEY_DIAG:
			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
			{
				System_GOTODiagTask();
			}
			break;
	#endif
		case  KEY_VER_SLEF_LCD:
			//模板不显示客户版本，具体按照需项目客户定义
			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
			{
#if  _PROJECT_LCD_
				//液晶屏显示内部版本高两位（客户更改次数）
				DisTsetShakeSet(SwsV_*10);
#else
#endif			
			}
			break;
		case  KEY_VER_SLEF:
			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
			{
#if  _PROJECT_LCD_
				//液晶屏显示内部版本低两位（内部更改次数）
				DisTsetShakeSet(SwV_*10 + SwsV_%10);	
#endif
				//AC、循环闪灯  一次性显示完整内部版本号
				LedDisVerEn(SwV_*100+SwsV_);
			}
			break;
		case KEY_CLIENT_VER:
			//客户版本号显示
			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
			{
#if  _PROJECT_LCD_
				//液晶屏显示内部版本低两位（内部更改次数）
				DisTsetShakeSet(SwV_CLIENT*10 + SwsV_CLIENT%10);	
#endif
				//AC、循环闪灯  一次性显示完整内部版本号
				LedDisVerEn(SwV_CLIENT*100+SwsV_CLIENT);
			}
			break;
	}
	TestMode_restrict();
}

/*******************************************************************************
 * Function:   void KeyLogic_Respone(uint8 u8KeyIMDType, int16 i16Data)
 *
 * Description: Deal_Keypad_Code 是放入缓存的按键进行读取，此功能为外部由信号时直接设置时使用
 *
 * Returns:     none
 *
 * Notes:      
 *           
*******************************************************************************/
void KeyLogic_Respone(uint8 u8KeyIMDType, int16 i16Data)
{
   //TODO: 根据实际应用需求编写,可参考模板
	if (u8KeyIMDType == IMKEY_FAN)
	{
		if (i16Data < 8)
		{
			SystemControl.fanMode = (uint8)i16Data;
		}
	}
	else
	{}
}

/*******************************************************************************
 * Function:   void  PanleState_Logic (void)
 *
 * Description: 旋钮，自锁按键等控制状态在实现
 *
 * Returns:     none
 *
 * Notes:  初始化结束时，优先调用
 *           
*******************************************************************************/
void PanleState_Logic(void)
{
	
}
