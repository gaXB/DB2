/************************文件说明*****************************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SystemApp.c
* 功能描述： 应用系统接口
* 1 提供外部接口函数
*  Init_System
*  SystemApp_Loop
*  SystemApp_PowerON
* 2 系统状态切换接口
* System_GOTODiagTask 
* System_GOTOInitTask
* 3 
* 应用层控制， 提供模板 ，需更改处按说明文件
* 编译器： codewarrior IDE 11.1
* MCU说明：和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月13日
* 
* 版本：2.0
* 说明： 增加_PROJECT_LDO_MODE
* 根据不同LDO 的模式来确定初始模式
* 
* 版本：3.0
* 说明： 记忆功能放在LOOP循环处理中
* 
* 版本：3.1
* 说明：更改枚举类型的赋值
* 完成日期：2021年8月23日
*/

#include "HVACMainControl.h"
#include "APPDrive.h"

/*******************************************************************************
 * 变量定义
 *
*******************************************************************************/
uint8	SystemState;

/*******************************************************************************
 * 系统工作任务 定义
 *
********************************************************************************/
#define   SYSTEM_STATE_INIT     0        //开机时初始化工作
#define   SYSTEM_STATE_NORMALWORK   1    //正常工作
#define   SYSTEM_STATE_DIAG   2          //自检工作
/*************************************************************************************************
外部函数声明
*********************************************************************************************/
extern void  Enter_InitTask(uint8 InitType);
extern void  DiagStepSet(uint8 step);
extern uint8 Run_Init_Task(void);
extern uint8 Run_Diag_Task(void);

//本模块实现函数
void System_GOTOInitTask(void);

/*******************************************************************************
 * Function:    void System_Default(void)
 *
 * Description: 系统进入默认模式
 *
 * Returns:     none
 *
 * Notes:       
*******************************************************************************/
static void System_Default(void)
{
	//TODO: 编写默认模式的值
	SystemControl.tset = _TSET_DEFAULT_;       //系统状态初始化
	SystemControl.OffState = OFF_STATE_OFF; 
	SystemControl.AutoBits.AC = 0;
	SystemControl.AutoBits.FAN = 0;
	SystemControl.AutoBits.MODE = 0;
	SystemControl.AutoBits.NCF = 0;
				
	SystemControl.acMode = AC_MODE_OFF;
	SystemControl.fanMode = FAN_MODE_1L;
	SystemControl.mdMode = MD_MODE_OSF;
	SystemControl.ncfMode = NCF_MODE_NEW;
	SystemControl.RdefMode = RDEF_MODE_OFF;
}


/*******************************************************************************
 * Function:    Init_Parameters
 *
 * Description:  实现如果第一次上电用默认值，之后需要用记忆值。并关闭所有输出及显示
 *
 * Returns:     none
 *
 * Notes:       变量初始化，
*******************************************************************************/
static void Init_Parameters(void)
{
   //读取EE
#if  _PROJECT_LDO_MODE == LDO_MODE_BATTIGN
	if (MCUPM_IsLPowerRest() == FALSE)   
	{//BATT有IGN上电的处理
		if (Load_Ign())
		{
			
		}  
		else
		{//载入错误使用默认值
			System_Default();
		}
		//此时ssensordata能保持
	}
	else
	{
    	//全部掉电时系统为初始化状态
		//TODO: 根据掉电模式确认
		System_Default();
		sSenSorData.TambCal = _TAMB_DEFAULT_;
		sSenSorData.TambCal_TD = _TAMB_DEFAULT_;
		sSenSorData.TwaterCal = _TWATER_DEFAULT_;
		sSenSorData.TinSimul = _TIN_DEFAULT_;
		sSenSorData.TinCal = _TIN_DEFAULT_;
	}
#else
	//其他模式不能判断是否有蓄电池
	if (Load_Ign())
	{
		
	}  
	else
	{//载入错误使用默认值
		System_Default();
	}
	
	sSenSorData.TambCal = _TAMB_DEFAULT_;
	sSenSorData.TambCal_TD = _TAMB_DEFAULT_;
	sSenSorData.TwaterCal = _TWATER_DEFAULT_;
	sSenSorData.TinSimul = _TIN_DEFAULT_;
	sSenSorData.TinCal = _TIN_DEFAULT_;
#endif

	if (SystemControl.tset > TSET_HI || SystemControl.tset < TSET_LO) 
	{
		SystemControl.tset = _TSET_DEFAULT_;
	}
	else{}
	
	if (SystemControl.fanMode == 0)
	{  
		SystemControl.fanMode = 1;
	}
	else{}
	
	SystemControl.RdefMode = RDEF_MODE_OFF;
	OtherControl.CHeatMode_L = 0;
	OtherControl.CHeatMode_R = 0;
	
	sDisplayData.AC = 0;
	sDisplayData.AUTO = 0;
	sDisplayData.CIRF = 0;
	sDisplayData.FANLevel = 0;
	sDisplayData.LCDDiagMode = 0;
	sDisplayData.LCDMode = 0;
	sDisplayData.LCheat = 0;
	sDisplayData.LEDMode = 0;
	sDisplayData.MAXAC = 0;
	sDisplayData.MODE_MAXDEF = 0;
	sDisplayData.NEW = 0;
	sDisplayData.RHEAT = 0;
	sDisplayData.RCheat = 0;
	sDisplayData.VentMode = MD_MODE_OSF;
	sDisplayData.i16Temp = 0;
	
	sOutData.ACOut = 0;
	sOutData.FANVolt = 0;
	sOutData.MIXAD = 255;
	sOutData.MODEAD = 255;
	sOutData.NCFAD = 255;
	sOutData.RHeatOut = 0;
	sOutData.u8LCheat = 0;
	sOutData.u8RCheat = 0;
}

/*******************************************************************************
 * Function:    Init_System
 *
 * Description: 系统初始化 main-loop 调用
 *
 * Returns:     none
 *
 * Notes:      不用更改
 *           
*******************************************************************************/
void Init_System(void)
{
	Init_Parameters();
	//进入全部初始化工作
	System_GOTOInitTask();
}

/*******************************************************************************
 * Function:    void System_Loop(void)
 *
 * Description: 应用LOOP 任务
 *
 * Returns:     none
 *
 * Notes:      电源故障时也会运行
 *            
*******************************************************************************/
void SystemApp_Loop(void)
{
	if (SystemState != SYSTEM_STATE_DIAG)
	{//循环记忆，放在此处，如点火关闭之后不需要记忆则放在SystemApp_PowerON
		MemoryControl();
	}else{}
	
}

/*******************************************************************************
 * Function:    void SystemState_InitToNormal(void)
 *
 * Description: 回调函数，结束初始化任务时，调用一次
 *
 * Returns:     none
 *
 * Notes:      在此编写结束初始化时需要处理的内容
 *
*******************************************************************************/
static void SystemState_InitToNormal(void)
{	
#if _PROJECT_AUTO_
	//自动控制初始化
	AUTOTD_Init();   	
#endif 	
	HVACControl_Init();
}

/*******************************************************************************
 * Function:    void SystemApp_PowerON(void)
 *
 * Description: 应用电源正常工作
 *
 * Returns:     none
 *
 * Notes:       实现初始化，正常，自检3种工作模式，无特殊功能需求不用更改
 *            
*******************************************************************************/
void SystemApp_PowerON(void)
{
	switch(SystemState) 
	{
		case  SYSTEM_STATE_INIT:
			if(Run_Init_Task()) //此系统为初始化系统2s
			{// 是否需要退出初始化
				SystemState = SYSTEM_STATE_NORMALWORK;
				//lcd 模块的相关设置
				SystemState_InitToNormal();
			}  
			break;
			
		case  SYSTEM_STATE_NORMALWORK:   
			Run_Normal_Task();  
			break;
	
		case  SYSTEM_STATE_DIAG:		
			
			if(Run_Diag_Task())
			{  //自检模式退出进入
				SystemState =SYSTEM_STATE_NORMALWORK;
				SystemControl.tset = _TSET_DEFAULT_;
				SystemControl.AutoBits.AC  = 1;
				SystemControl.AutoBits.NCF  = 1;
				SystemControl.AutoBits.FAN = 1;
				SystemControl.AutoBits.MODE = 1;
			}
			//以下为主要（通用）控制变量的控制，如不需要可删减
			break;		
		default:
			SystemState = SYSTEM_STATE_INIT;
			Enter_InitTask(0);
			break;
	}
} 
/*******************************************************************************
 * Function:   void System_GOTOInitTask(void)
 * 
 * Description: 处理进入初始化流程
 *
 * Returns:     none
 *
 * Notes:      进入初始化时调用            
 *                         
 *                          
*******************************************************************************/
void System_GOTOInitTask(void)
{
   Enter_InitTask(0);  //进入初始化流程
   SystemState = SYSTEM_STATE_INIT;
   //TODO: 编写进入初始化时，需要改变的状态
}

/*******************************************************************************
 * Function:   void System_GOTODiagTask(void)
 * 
 * Description: 进入自检状态
 *
 * Returns:     none
 *
 * Notes:                  
 *                         
 *                          
*******************************************************************************/
void System_GOTODiagTask(void)
{
	SystemState = SYSTEM_STATE_DIAG;
	DiagStepSet(0);
}
