/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：main.c
* 功能描述：主函数入口，此main定义了整体系统架构
*
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为MAIN-LOOP上层应用，
*        此文件不能更改
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
* 
* 版本：2.0
* 说明：增加standby task
* 原作者：xb
* 完成日期：2020年5月30日
*/
#include "APPDrive.h"
#include "PowerMange.h"


/*************************************************************************************************
外部函数声明
*********************************************************************************************/
extern void Init_LowerDrive(void);
extern void LowDrive_Loop(void);
extern void LowDrive_PowerON(void);
//app 接口
extern void Init_System(void);
extern void SystemApp_Loop(void);
extern void SystemApp_PowerON(void);
extern void PowerOffTask(void);
extern void PowerStandByTask(void);
/*******************************************************************************
 * Function:        void main()
 *
 * Description:    主函数入口
 *
 * Returns:         none
 *
 * Notes:           none
 *
********************************************************************************/

int main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/
	(void)DisableInterrupt();
	//int counter = 0;
	Init_MCU();
		
	Init_LowerDrive();   //此处要先放，因为此处有初始化FASH
	
	Init_System();   //系统的相关配置，主要是模块的相关配置。Ad，电机等
		
   PowerNM_Init();


	(void)EnableInterrupt();

	for(;;)
	{
		//counter++;
		Clear_WatchDog(); 
		
		LowDrive_Loop(); 
		
		SystemApp_Loop();
		
		PowerMange();           //电源管理，算出当前电源的状态。
	
		//以上系统底层驱动
		//以下为电源正常时才能运行
		if (PowerMN_GetPowerState() == POWER_STATE_WORK)     //ACC电压正常
		{
			//更新 1621 包含 更新1621 数据字
			LowDrive_PowerON();
			//根据系统状态，进行相关任务处理
			SystemApp_PowerON();
		}
		else
		{//点火电压故障
			if (PowerMN_GetPowerState() == POWER_STATE_OFF)
			{
				PowerOffTask();
			}
			else if (PowerMN_GetPowerState() == POWER_STATE_STANDBY)
			{
				PowerStandByTask();
			}
			else
			{//在其他状态时（POWER_STATE_INIT，POWER_STATE_SLEEP，POWER_STATE_STANDBY_WAIT，不能做任何任务，由powermn来处理
				
			}		
		}
	}
	return 0;
}


