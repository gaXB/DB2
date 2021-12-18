/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：HVACMainControl.c
* 功能描述：实现HVAC 控制层逻辑， 及正常工作任务的实现
* 1 程序逻辑更改的主要文件，由系统控制变量 SystemControl 设置sDisplayData 和 sOutData 
* 
* 编译器： codewarrior IDE 11.1
* MCU说明： 与MCU无关，基本不用更改
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年3月14日
* 
* 版本：1.1
* 说明：增加LCD的驱动
* 作者：xb
* 完成日期：2020年6月3日
*
* 版本：1.2
* 说明：增加LED 模式在OFF 时仍需要为1
* 作者：xb
* 完成日期：2020年6月3日
* 
* 版本：1.3
* 说明：风量处理模块优化 SystemControl.fanmode 由输出模块得到
* 作者：xb
* 完成日期：2020年7月6日
* 
* 版本：1.4
* 说明：
* 1 	off时 //  SystemControl.fanMode = FAN_MODE_OFF; 屏蔽此句  
* 2   电动时 自动标记有时无法输出风量
* 作者：xb
* 完成日期：2020年11月21日
* 
* 版本：1.5
* 说明：
* 1 	增加风量显示跟随选择的功能
* 作者：xb
* 完成日期：2021年4月6日
* 
* 版本：1.6
* 说明：
* 1 	枚举类型赋值优化
* 作者：xb
* 完成日期：2021年8月23日
*
*/
#include "APPDrive.h"
#include "KeyPadLogic.h"
#include "Parameter.h"

//风量控制相关寄存器
static uint8 u8FanCurLevel;                  //当前风量档位
static uint16 vFanVotlOut;
//ac 控制相关寄存器
static uint16 TimerACInitDelay;    //ac 开机延时参数
static uint8 bACDelayEN=0;         //ac 开机使能
static uint8 bTevpAC;
       
//延时输出参数
static uint16 nDelayDis, nDelayOut;
/**************************可配置参数****************************************/
#define  DISDELAY_TIMES      0         //*10ms
#define	OUTDELAY_TIMES     100        //*10ms



extern void EVACControl(void);

/*******************************************************************************
 * Function:   void  RdefControl(void)
 *
 * Description: 后除霜控制
 *
 * Returns:     none
 *
 * Notes:     1 实现 后除霜控制逻辑
 * 
*******************************************************************************/
static void RdefControl(void)
{
	//TODO:后除霜逻辑及输出实现
	uint8 rdefState;
   static RdefModeType u8LastState;
   //rdef 控制相关寄存器
   static uint16 TimerRHeat;              //后除霜控制
   //普遍逻辑为后除霜控制开启15分钟，自动灭掉， 以下为范例
   //后除霜控制在按键在实现，切换时需要重新定时  TimerRHeat
   
   if (SystemControl.RdefMode != u8LastState)
   {
   	u8LastState = SystemControl.RdefMode;
   	TimeOutChkSLong(&TimerRHeat, 0);
   }else{}
   
   if(SystemControl.RdefMode == RDEF_MODE_ON )
   {
      if(TimeOutChkSLong(&TimerRHeat, 900))
      {
         SystemControl.RdefMode = RDEF_MODE_OFF;
      }else{}
      rdefState = 1;
   }
   else
   {
      rdefState = 0;
   }
   
   sOutData.RHeatOut = rdefState;
}


/******************************************************************************
 * Function:   void  ACControl(void)
 *
 * Description: AC控制输出及指示灯
 *
 * Returns:     none
 *
 * Notes:     ac 开启条件为
 *          1 外温满足
 *          2 蒸发器
 *          3 开机延时
 *          4 压力控制
 *
*******************************************************************************/
static void ACControl(void)
{
	//TODO:AC控制逻辑及输出实现 
	static  uint8 sg_tambac=0;           //外温AC使能
	
   //AC 开机延时
   if(TimeOutChkTenthSLong(&TimerACInitDelay,35))
   {
      bACDelayEN=1;
   }
   
   //外温保护
   if(sSenSorData.TambCal < tAC_TAMBPRO_ON)
   { 
	   sg_tambac=1;     //
   }else{}
   
   if(sSenSorData.TambCal > tAC_TAMBPRO_OFF)
   { 
	   sg_tambac=0;
   }else{}

   //确定蒸发除霜点
   if (sSenSorData.TevpCal > tACON_EVP)
   {
   	  bTevpAC = 0;
   }
   
   if (sSenSorData.TevpCal < tACOFF_EVP)
   {
   	 bTevpAC = 1;
   }

   //根据状态判定 AC的输出和 指示灯
   if(SystemControl.OffState ||SystemControl.fanMode ==FAN_MODE_OFF)
   {  //
      sOutData.ACOut = 0;
   }
   else
   {
      if(SystemControl.acMode &&!sg_tambac && !bTevpAC && bACDelayEN)
      {
      	sOutData.ACOut = 1;
      }  
      else 
      {
      	sOutData.ACOut = 0;
      }
   }
}
/*******************************************************************************
 * Function:   void  NCFControl(void)
 *
 * Description: 内外循环控制
 *
 * Returns:     none
 *
 * Notes:     1 根据当前内外状态（SystemControl.ncfMode）设定电机
 *              位置及指示灯
 *
*******************************************************************************/
static void NCFControl(void)
{
	//TODO:内外逻辑及输出实现
	if(SystemControl.ncfMode == NCF_MODE_CIRF)	
	{
		sOutData.NCFAD = 1;
	}
	else
	{	
		sOutData.NCFAD = 0;
	}
}
/*******************************************************************************
 * Function:   void  MixMotorControl(void)
 *
 * Description: 混合电机控制
 *
 * Returns:     none
 *
 * Notes:      1 由AUTOTD模块得到当前的位置值MixAD
 *              并设定电机位置
*******************************************************************************/
static void MixMotorControl(void)
{
	//TODO:混合逻辑及输出实现
   uint16 MixAD;
	if (!SystemControl.OffState)
   {
   	if (BUSCONFIG_USE_ATC == 0)
   	{
   		MixAD = GAMath_LineFuni16(TSET_LO, TSET_HI, u8MIXMOTOR_LO_AD, u8MIXMOTOR_HI_AD, SystemControl.tset);
   	}
   	else
   	{
#if  _PROJECT_AUTO_
   		MixAD =  AUTOTD_GetResult(AUTOTD_TYPE_MIX);      //得到AD值   	
   		MixAD = GAMath_LineFuni16(0, 1000, u8MIXMOTOR_LO_AD, u8MIXMOTOR_HI_AD, MixAD);//得到千分比 ,采用此函数
#else
   		MixAD = GAMath_LineFuni16(TSET_LO, TSET_HI, u8MIXMOTOR_LO_AD, u8MIXMOTOR_HI_AD, SystemControl.tset);
#endif
   	}
   	
   	sOutData.MIXAD = (uint8)MixAD;
   }
}

/*******************************************************************************
 * Function:   void  MdMotorControl(void)
 *
 * Description: 混合电机控制
 *
 * Returns:     none
 *
 * Notes:     1 根据当前模式（SystemControl.mdMode)设定电机的位置
 *            及 指示灯的状态
 *
*******************************************************************************/
static void MdMotorControl(void)
{
	//TODO:模式逻辑及输出实现
	uint8    oset;

	if(SystemControl.mdMode <= MD_MODE_OST)
	{//SystemControl.mdMode off
   	oset = TBL_MODE_OSET[SystemControl.mdMode];
	}
	else
	{
		oset = TBL_MODE_OSET[MD_MODE_OST];
	}
	
	sOutData.MODEAD = oset;
}

/*******************************************************************************
 * Function:   void  FanControl(void)
 *
 * Description: 风量控制
 *
 * Returns:     none
 *
 * Notes:    1 自动控制功能,冷风热风保护功能, 在AUTO_TD中实现
 *           2 调用 FanVolt_Control() 模块稳定电压
 *           3 本进程只需要设置电压和上升速度，最后电压稳定模块来实现功能
 *           
*******************************************************************************/
static void FanControl(void)
{
	//TODO:风量逻辑及输出实现
	if(SystemControl.OffState)
	{  //off 直接到0
	  u8FanCurLevel = 0;
	//  SystemControl.fanMode = FAN_MODE_OFF; 不屏蔽此句  saveoff， loadoff 将不能记忆
	  sOutData.FANVolt = 0;
	}
	else
	{
		if(!SystemControl.AutoBits.FAN)
		{//
			if(SystemControl.fanMode > MAX_FELVL) 
			{
				SystemControl.fanMode = MAX_FELVL;
			}else{}
			//不是自动根据档位得出faset 
			vFanVotlOut = TBL_FANVOLT[SystemControl.fanMode];
	
			sOutData.FANVolt = vFanVotlOut; 
		}
		else
		{//自动时由faset 决定
#if _PROJECT_AUTO_
			vFanVotlOut = AUTOTD_GetResult(AUTOTD_TYPE_FAN);
			
			SystemControl.fanMode = FanControl_GetCurrentLevel();  //自动时，档位由输出档位确定
			
			sOutData.FANVolt = vFanVotlOut;
			(void)FanControl_SetRate(FANVOLT_RATE_INIT);
#else 
			vFanVotlOut = TBL_FANVOLT[SystemControl.fanMode];
			sOutData.FANVolt = vFanVotlOut;
#endif
		}
#if	_APPCONFIG_FANDISPLAY_ == 1 
		u8FanCurLevel = FanControl_GetCurrentLevel(); 
#else
		u8FanCurLevel = SystemControl.fanMode; 
#endif
	}
}


/*******************************************************************************
 * Function:   void  DisplayDataSet(void)
 *
 * Description: DisplayData Set
 *
 * Returns:     none
 *
 * Notes:     对sDisplayData 进行设置
 *
*******************************************************************************/
static void DisplayDataSet(void)
{	
	//TODO: 在此编写设置sDisplayData
	sDisplayData.LEDMode = 1;   //关闭之后led 还是需要正常工作
	if (SystemControl.OffState)
	{//OFF STATE
		sDisplayData.FANLevel = 0;
		sDisplayData.AC = 0;
		sDisplayData.AUTO = 0;
		sDisplayData.LCDDiagMode = 0;
		sDisplayData.LCDMode = 0;
		sDisplayData.MAXAC = 0;
		sDisplayData.MODE_MAXDEF = 0;
		sDisplayData.VentMode = MD_MODE_OSF;
	}
	else
	{
		sDisplayData.FANLevel = u8FanCurLevel;
      if (SystemControl.acMode == AC_MODE_ON || SystemControl.AutoBits.AC)
      {
      	sDisplayData.AC = 1;
      }
      else 
      {
      	sDisplayData.AC = 0;
      }
		if (SystemControl.AutoBits.AC && SystemControl.AutoBits.FAN && 
				SystemControl.AutoBits.MODE)
		{
			sDisplayData.AUTO = 1;
		}
		else
		{
			sDisplayData.AUTO = 0;
		}

		sDisplayData.LCDDiagMode = 0;
		sDisplayData.LCDMode = 1;
		if (SystemControl.MaxACMode)
		{
			sDisplayData.MAXAC = 1;
		}
		else
		{
			sDisplayData.MAXAC = 0;
		}
		if (SystemControl.MaxDefMode)
		{
			sDisplayData.MODE_MAXDEF = 1;
		}
		else
		{
			sDisplayData.MODE_MAXDEF = 0;
		}

		sDisplayData.VentMode = SystemControl.mdMode;
	
	}
			
	sDisplayData.i16Temp = SystemControl.tset;
	if (SystemControl.RdefMode == RDEF_MODE_ON)
	{
		sDisplayData.RHEAT = 1;
	}
	else
	{
		sDisplayData.RHEAT = 0;
	}
	//sDisplayData.RHEAT = sSenSorData.RHeat_FB;
	
	if (SystemControl.ncfMode == NCF_MODE_CIRF)
	{
		sDisplayData.CIRF = 1;
		sDisplayData.NEW = 0;
	}
	else
	{
		sDisplayData.CIRF = 0;
		sDisplayData.NEW = 1;
	}
}


/*******************************************************************************
 * Function:   void  OtherOutControl(void)
 *
 * Description: 其他输出控制
 *
 * Returns:     none
 *
 * Notes:     其他需要在主应用层程序中进行loop的函数
 *
*******************************************************************************/
static inline void LCDSetControl(void)
{	
	//TODO: 在此编写设置LCD_Dsiplay模块
#if  _PROJECT_LCD_
	LCDDATA lLCDData; 
	if(SystemControl.OffState)
	{
		SetLcdDisplayMode(LCD_MODE_CLEAR);
	}
	else
	{
		//AUTO状态发送
		if(SystemControl.OffState ==0  &&                 //全自动且不是off
		SystemControl.AutoBits.AC   &&
		SystemControl.AutoBits.MODE && SystemControl.AutoBits.FAN)
		{ //auto led
			lLCDData.SegAuto = 1;
		}
		else
		{
			lLCDData.SegAuto = 0; 
		}
		SetLcdDisplayMode(LCD_MODE_NORMAL);
		lLCDData.SegAC = SystemControl.acMode;
		lLCDData.SegFanLevel = u8FanCurLevel;
		lLCDData.SegMode = SystemControl.mdMode;
		lLCDData.SegNCF = SystemControl.ncfMode;
		
		if (SystemControl.tset >= TSET_HI)
		{
			lLCDData.i16Temp = TSET_LCD_HI;
		}
		else if (SystemControl.tset <= TSET_LO)
		{
			lLCDData.i16Temp = TSET_LCD_LO;
		}
		else
		{
			lLCDData.i16Temp = SystemControl.tset;
		}
		SetLcdDisplayData(&lLCDData);
	}
#endif
}

static void OtherOutControl(void)
{	
	//TODO:LCD控制， 其他控制放在此处
	LCDSetControl();
}


/*******************************************************************************
 * Function:   void  DelayBSPOut(void)
 *
 * Description: 其他输出控制
 *
 * Returns:     none
 *
 * Notes:     显示输出延迟逻辑，
 *
*******************************************************************************/
void  DelayBSPOut(void)
{
	nDelayDis++;
	nDelayOut++;
	if (nDelayDis < DISDELAY_TIMES)
	{//TODO: 在此编写关闭显示的项目，如果不需要可不写
		//sDisplayData.AC = 0;
	}
	else
	{
		nDelayDis = DISDELAY_TIMES;
	}
	
	
	if (nDelayOut < OUTDELAY_TIMES)
	{//TODO: 在此编写关闭输出的项目，如果不需要可不写
		//sOutData.ACOut = 0;
	}
	else
	{
		nDelayOut = OUTDELAY_TIMES;
	}
}




/*******************************************************************************
 * Function:   void Run_Normal_Task(void)
 *
 * Description: 应用正常任务，systemApp在电源正常时LOOP调用
 *
 * Returns:     none
 *
 * Notes:      如其他特殊控制在此增加
 *
*******************************************************************************/
void Run_Normal_Task(void)
{
	static uint16 TimerNoramalTask;

#if _PROJECT_AUTO_
	if (BUSCONFIG_USE_ATC)
	{
		Cal_TD();     //td处理函数，电动时会将 autobits =0
		AUTOTD_Control();  //需要相关功能时在进行添加，添加时需要注意
	}
	else
#endif
	{
		SystemControl.AutoBits.AC = 0;
		SystemControl.AutoBits.MODE = 0;
		SystemControl.AutoBits.FAN = 0;
		SystemControl.AutoBits.NCF = 0;
	}
	
	if (TimeOutChkMsLong(&TimerNoramalTask, 10))
	{
//		Park_HVAC();         		//驻车控制及相关输出
		EVACControl();
		
		RdefControl();       		//后除霜控制相关逻辑及输出
		
		ACControl();        		//ac控制相关逻辑及输出
		
		NCFControl();       			//内外控制相关逻辑及输出
		
		MixMotorControl();  			//混合电机控制相关逻辑及输出。，此处为2个冷暖电机都放在此处
		
		MdMotorControl();   			//模式电机控制相关逻辑及输出
		
		FanControl();       			//风量控制相关逻辑及输出
		
		DisplayDataSet();
/******以上为主要（通用）控制变量的控制******/
		
		OtherOutControl();  			//其他控制
		
		DelayBSPOut();            //放在最后
	}else{}

	PanleState_Logic();
	Deal_Keypad_Normal();			//按键功能逻辑处理
	
}

/*******************************************************************************
 * Function:   void HVACControl_Init(void)
 *
 * Description: 主任务控制的初始化，在系统进入正常模式时调用
 *
 * Returns:     none
 *
 * Notes:      none
 *
*******************************************************************************/
void HVACControl_Init(void)
{
   //TODO: 编写HVACCONTORL 初始化的内容
	OtherControl.CHeatMode_L = 0;
	OtherControl.CHeatMode_R = 0;
   SystemControl.RdefMode = RDEF_MODE_OFF;
	(void)TimeOutChkTenthSLong(&TimerACInitDelay,0);  //ac 开机延时
	bACDelayEN = 0;
//	bACProtect = 0;       //AC开关延时的初始，为可以开启
//	bTevpAC = 1;          //结霜保护的初始值，为关ac
	//鼓风机为0档无输出
	u8FanCurLevel =0;  
	PanleState_Logic();
	nDelayDis = 0;
	nDelayOut = 0;
}


