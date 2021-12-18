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
				if (u8LevelSet > TSETTEM_MODE_DEBUG)
				{
					SystemControl.u8TestLevel = TSETTEM_MODE_DEBUG;
				}
				else if (u8LevelMode == SystemControl.TestMode)
				{//同一模式下切换可以切
					SystemControl.u8TestLevel = u8LevelSet;
				}
				else if (sVehicleControl.bBattCoolReq || sVehicleControl.bBattHotReq)
				{//不能切换

				}
				else
				{
					SystemControl.u8TestLevel = u8LevelSet;
				}
			}
			break;
		case COMD_TSET_DEC:
			if(!SystemControl.OffState)
			{
				if (SystemControl.u8TestLevel > 0)
				{
					u8LevelSet = SystemControl.u8TestLevel - 1;
				}
				else
				{
					u8LevelSet = 0;
				}

				u8LevelMode = GetTSETLevelMode(u8LevelSet);
				if (u8LevelSet < TSETEM_MODE_JIAZHU)
				{
					SystemControl.u8TestLevel = TSETTEM_MODE_DEBUG;
				}
				else if (u8LevelMode == SystemControl.TestMode)
				{//同一模式下切换可以切
					SystemControl.u8TestLevel = u8LevelSet;
				}
				else if (sVehicleControl.bBattCoolReq || sVehicleControl.bBattHotReq)
				{//不能切换

				}
				else
				{
					SystemControl.u8TestLevel = u8LevelSet;
				}
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
		//*********************显示******************	     
	#if  _PROJECT_LCD_
		case  KEY_AC + 0xff:
		  //显示 蒸发 时间
			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
			{
				DisTsetShakeSet(sSenSorData.TevpCal) ;	
			}
			break;
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
