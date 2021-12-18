/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SystemSave.c
* 功能描述：本模块具有实现系统应用相关存储记忆功能,所有存储都不能放在flash区,只用noint 变量来实现,
*  和MCU 相关。具有如下功能:
* 1  off记忆（放在ram中）
* 2  def 记忆（放在ram中），具有除霜被操作不变功能void Def_Operat(uint8 OPType)
* 3  lohi前后 记忆（ram） 
* 
* 编译器： codewarrior IDE 11.1
* MCU说明：与MCU 无关,此模块只作为模板
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年3月14日
* 
* 版本：1.1
* 说明：更新saveDID 的方式改为通过PID 自动存储
* 作者：xb
* 完成日期：2019年3月14日
* 
* * 版本：1.2
* 说明：把flash相关的记忆放在SystemSaveFlash 模块,其他ram存储放在此区域
* 作者：xb
* 完成日期：2019年9月5日
* 
* 版本：1.3
* 说明：增加默认值功能
* 作者：xb
* 完成日期：2020年11月21日
* 
* 版本：1.4
* 说明：1 枚举类型赋值优化
* 2 兼容renesas
* 作者：xb
* 完成日期：2021年8月23日
*/
//TODO: 应用记忆的功能放在此函数，本模板提供了off def park等记忆的模板，请按实际需求进行编写
#include "SystemSave.h"


//相关变量
#if  NOINIT_TYPE ==  NOINIT_TYPE_KEA
__attribute__ ((section(".noinit"))) static SYSTEMCONTROL SystemOffState, SystemDefState, SystemLOHIState, SystemMaxACState;   //off时的记忆和def时的记忆
__attribute__ ((section(".noinit"))) static SYSTEMCONTROL SystemParkState;
__attribute__ ((section(".noinit"))) static uint8 OffChkSum, DefChkSum, LOHIChkSum, MaxACChkSum;      //将存储放入ram 中进行存储
__attribute__ ((section(".noinit"))) static uint8 ParkChksum;

__attribute__ ((section(".noinit"))) static struct
{
	uint8 MODE;
	uint8 FAN;
	uint8 AC;
	uint8 NCF;
}SaveDefFlg;
#elif NOINIT_TYPE == NOINIT_TYPE_RL78
#pragma section bss NOINIT
static SYSTEMCONTROL SystemOffState, SystemDefState, SystemLOHIState, SystemMaxACState;   //off时的记忆和def时的记忆
static SYSTEMCONTROL SystemParkState;
static uint8 OffChkSum, DefChkSum, LOHIChkSum, MaxACChkSum;      //将存储放入ram 中进行存储
static uint8 ParkChksum;
static struct
{
	uint8 MODE;
	uint8 FAN;
	uint8 AC;
	uint8 NCF;
}SaveDefFlg;
#pragma section 
#define __GOLBAL_VAR_
#endif


/*******************************************************************************
 * Function:      static  void SaveErr_GotoDefault(void)
 *
 * Description:    记忆错误返回默认转台
 *
 * Returns:         none
 *
 * Notes:        none
 *
********************************************************************************/
static  void SaveErr_GotoDefault(void)
{
#if _PROJECT_AUTO_
   SystemControl.AutoBits.AC = 1;
   SystemControl.AutoBits.NCF = 1;
   SystemControl.AutoBits.FAN = 1;
   SystemControl.AutoBits.MODE= 1;
#endif
}
/*******************************************************************************
 * Function:        BOOL Chkeck_SaveSystemControl(SYSTEMCONTROL *pSystem, uint8 ChkSum) 
 *
 * Description:     判断校验码是否正确
 *
 * Returns:         正确 TRUE  错误 FALSE
 *
 * Notes:         pSystem 为记忆的系统状态字，ChkSum  pSystem的校验码  
 *
********************************************************************************/
static BOOL Chkeck_SaveSystemControl(SYSTEMCONTROL *pSystem, uint8 ChkSum) 
{
   uint8  Num=0;
   uint8*  pSaveChar = (uint8*)pSystem;
   uint8 bCheck;
   for(Num=0; Num< sizeof(SYSTEMCONTROL);Num++)
   {
      ChkSum += *pSaveChar;
      pSaveChar++;  
   }
   if (ChkSum == 255)
   {//记忆正确
   	bCheck = TRUE;
   }
   else
   {
   	bCheck = FALSE;
   }
   
   return bCheck;
}

/*******************************************************************************
 * Function:        uint8 CalSys_ChkSum(SYSTEMCONTROL *pSystem) 
 *
 * Description:     计算校验码
 *
 * Returns:         校验码
 *
 * Notes:         
 *
********************************************************************************/
static uint8 CalSys_ChkSum(SYSTEMCONTROL *pSystem) 
{
   uint8  SaveNum=0;
   uint8*  pSaveChar = (uint8*)pSystem;
   
   uint8 ChkSum =0;
   for(SaveNum=0; SaveNum<sizeof(SYSTEMCONTROL);SaveNum++)
   {
      ChkSum += *pSaveChar;
      pSaveChar++;  
   }
   ChkSum = ~ChkSum;
   return ChkSum;
}

/*******************************************************************************
 * Function:        void Save_Off (void) 
 *
 * Description:     off 时记忆系统
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Save_Off (void)
{
	SystemOffState = SystemControl;

   OffChkSum = CalSys_ChkSum(&SystemOffState);
}

/*******************************************************************************
 * Function:       void Load_Off (void)
 *
 * Description:     off 退出时记忆
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Load_Off (void)
{
   if( Chkeck_SaveSystemControl(&SystemOffState, OffChkSum))
   {  //记忆正确，没有丢失
      SystemControl.acMode  =  SystemOffState.acMode;
      SystemControl.fanMode =  SystemOffState.fanMode;
      //SystemControl.mdMode =   SystemOffState.mdMode;  //不需要导入mode状态，
      SystemControl.ncfMode =  SystemOffState.ncfMode;
      SystemControl.AutoBits = SystemOffState.AutoBits;
   }
   else
   {  //记忆错误，丢失
   	//SaveErr_GotoDefault();
   }
   SystemControl.OffState = OFF_STATE_ON;
}

/*******************************************************************************
 * Function:        void Save_Off (void) 
 *
 * Description:     Park 时记忆系统
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Save_Park (void)
{
	SystemParkState = SystemControl;

	ParkChksum = CalSys_ChkSum(&SystemParkState);
}

/*******************************************************************************
 * Function:       void Load_Off (void)
 *
 * Description:     Park 退出时记忆
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Load_Park (void)
{
	 if( Chkeck_SaveSystemControl(&SystemParkState, ParkChksum))
	   {  //记忆正确，没有丢失
	      SystemControl.acMode  =  SystemParkState.acMode;
	      SystemControl.fanMode =  SystemParkState.fanMode;
	      SystemControl.mdMode =   SystemParkState.mdMode; 
	      SystemControl.ncfMode =  SystemParkState.ncfMode;
	      SystemControl.AutoBits = SystemParkState.AutoBits;
	   }
	   else
	   {  //记忆错误，丢失
	   	SaveErr_GotoDefault();
	   }
	   SystemControl.ParkAC = 0;
	
}

/*******************************************************************************
 * Function:        void Save_Def (void)
 *
 * Description:     def 时记忆系统
 *
 * Returns:         none
 *
 * Notes:           none
 *
********************************************************************************/
void Save_Def (void)
{
   SystemDefState = SystemControl;

   DefChkSum = CalSys_ChkSum(&SystemDefState);
   
   SaveDefFlg.AC = 1;
   SaveDefFlg.FAN = 1;
   SaveDefFlg.MODE = 1;
   SaveDefFlg.NCF = 1;
}

void Def_Operat(uint8 OPType)
{
	switch(OPType)
	{
		default:
			break;
		case DEF_OPTYPE_AC:
			SaveDefFlg.AC = 0;
			break;
		case DEF_OPTYPE_NCF:
			SaveDefFlg.NCF = 0;
			break;
		case DEF_OPTYPE_MODE:
			SaveDefFlg.MODE = 0;
			break;
		case DEF_OPTYPE_FAN:
			SaveDefFlg.FAN = 0;
			break;
	}
} 
/*******************************************************************************
 * Function:       void Load_Def (void)
 *
 * Description:     def 退出时记忆
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Load_Def (void)
{
   if( Chkeck_SaveSystemControl(&SystemDefState, DefChkSum))
   {  //记忆正确，没有丢失
      if (SaveDefFlg.AC)
      {
      	SystemControl.acMode  =  SystemDefState.acMode;
      	SystemControl.AutoBits.AC = SystemDefState.AutoBits.AC;
      }
      
      if (SaveDefFlg.MODE)
      {
      	SystemControl.mdMode  =  SystemDefState.mdMode;
      	SystemControl.AutoBits.MODE = SystemDefState.AutoBits.MODE;
      }
      
      if (SaveDefFlg.FAN)
      {
      	SystemControl.fanMode  =  SystemDefState.fanMode;
      	SystemControl.AutoBits.FAN = SystemDefState.AutoBits.FAN;
      }
      
      if (SaveDefFlg.NCF)
      {
      	SystemControl.ncfMode  =  SystemDefState.ncfMode;
      	SystemControl.AutoBits.NCF = SystemDefState.AutoBits.NCF;
      } 
   }
   else
   {  //记忆错误，丢失
   	SaveErr_GotoDefault();
   }
   if (SystemControl.mdMode == MD_MODE_OST)
   {//防错
   	SystemControl.mdMode = MD_MODE_OSF;
   	SystemControl.AutoBits.MODE = 0;  
   }
   SystemControl.OffState = OFF_STATE_ON;
}

/*******************************************************************************
 * Function:       void LoadMode_BeforDef(void)
 *
 * Description:     载入进入除霜前的模式，
 *
 * Returns:         none
 *
 * Notes:           退出off，时需要使用此函数			
 *
********************************************************************************/
void LoadMode_BeforDef(void)
{
	if( Chkeck_SaveSystemControl(&SystemDefState, DefChkSum))
	{
		SystemControl.mdMode = SystemDefState.mdMode;
	}
	else
	{
		SystemControl.mdMode = MD_MODE_OSF;
	}
}

/*******************************************************************************
 * Function:        void Save_LOHI (void) 
 *
 * Description:     LOHI 时记忆系统
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Save_LOHI (void) 
{
   SystemLOHIState = SystemControl; 

   LOHIChkSum = CalSys_ChkSum(&SystemLOHIState);  
}

/*******************************************************************************
 * Function:       void Load_LOHI (void)
 *
 * Description:     LOHI 退出时记忆
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Load_LOHI (void)
{  
   if( Chkeck_SaveSystemControl(&SystemLOHIState, LOHIChkSum) )
   {  //记忆正确，没有丢失
      SystemControl.acMode  =  SystemLOHIState.acMode;  
      SystemControl.fanMode =  SystemLOHIState.fanMode;  
      SystemControl.mdMode =   SystemLOHIState.mdMode;
      SystemControl.ncfMode =  SystemLOHIState.ncfMode;
      SystemControl.AutoBits = SystemLOHIState.AutoBits;
   }        
   else
   {  //记忆错误，丢失
   	SaveErr_GotoDefault();
   }
   SystemControl.OffState = OFF_STATE_ON;   
  
}

void Save_MaxAC(void)
{
	SystemMaxACState = SystemControl;
	MaxACChkSum = CalSys_ChkSum(&SystemMaxACState);
}

void Load_MaxAC(void)
{
   if( Chkeck_SaveSystemControl(&SystemMaxACState, MaxACChkSum))
   {  //记忆正确，没有丢失
	  SystemControl.tset  = SystemMaxACState.tset;
	  SystemControl.acMode  =  SystemMaxACState.acMode;
	  SystemControl.fanMode =  SystemMaxACState.fanMode;
	  SystemControl.mdMode  =   SystemMaxACState.mdMode; 
	  SystemControl.ncfMode =  SystemMaxACState.ncfMode;
	  SystemControl.AutoBits = SystemMaxACState.AutoBits;
   }
   else
   {  //记忆错误，丢失
   	SaveErr_GotoDefault();
   }
   SystemControl.OffState = OFF_STATE_ON;
   SystemControl.MaxACMode = 0;
}



