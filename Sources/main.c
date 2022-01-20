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
#include "fault_exceptions.h"


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
l_u8 Motor1_temp;
#define TIMER_COMPARE_VAL (uint16_t)(2000U)
#define TIMER_TICKS_1US   (uint16_t)(4U)

#define MOTOR_SELECTION_INCREASE (1u)
#define MOTOR_SELECTION_DECREASE (2u)
#define MOTOR_SELECTION_STOP     (3u)

#define MOTOR1_OVER_TEMP   (200u)
#define MOTOR1_MAX_TEMP    (100u)
#define MOTOR1_MIN_TEMP    (30u)

void lin_master_task(void)
{

 /* Infinite loop */

	/* Check if information about the Motor1 Temp has been received */
//		if (l_flg_tst_LI0_Motor1Temp_flag())
//		{
//		  /* Clear this flag... */
//			l_flg_clr_LI0_Motor1Temp_flag();
//
//		  /* Store temperature data */
//			Motor1_temp = l_u8_rd_LI0_Motor1Temp();
//
//		  /* Request stop motor by power off */
//			if (MOTOR1_OVER_TEMP < Motor1_temp)
//			{
//				l_u8_wr_LI0_Motor1Selection(MOTOR_SELECTION_STOP);
//			}
//		  /* Request to reduce motor speed */
//			else if ((MOTOR1_MAX_TEMP < Motor1_temp) && (MOTOR1_OVER_TEMP > Motor1_temp))
//			{
//				l_u8_wr_LI0_Motor1Selection(MOTOR_SELECTION_DECREASE);
//			}
//		  /* Request to increase motor speed if users request */
//			else
//			{
//				l_u8_wr_LI0_Motor1Selection(MOTOR_SELECTION_INCREASE);
//			}
//		}
//	   /* Check node state */
//		if (LIN_NODE_STATE_SLEEP_MODE == lin_lld_get_state(LI0))
//		{
//		/* Turn off all LEDs */
//		}

}

void LIN_Init(void)
{
	/* Initialize LIN network interface */
	l_sys_init();
	l_ifc_init(LI0);
	/* Set Schedule table to Normal */
	l_sch_set(LI0, LI0_NormalTable, 0u);
}


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
		
   enable_fault_handlers();        // If not set, all fault are handled by HardFault_Handler
   set_fault_handlers_priority();  // priority: MemManage (1), BusFault (1), UsageFault(1)

	Init_LowerDrive();   //此处要先放，因为此处有初始化FASH
	
	Init_System();   //系统的相关配置，主要是模块的相关配置。Ad，电机等
		
   PowerNM_Init();

   LIN_Init();

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


//void HardFault_Handler(void)
//{
//	while(1)
//	{
//		MCU_NOP;
//	}
//}

int16 i16HartErrNum;
void HardFault_Handler44(void)
{
// LR provides information of the return stack PSP/MSP
	i16HartErrNum++;
	asm("MOVS R0, #4");
	asm("MOV R1, LR");
	asm("TST R0, R1");
	asm("BEQ _MSP");
	asm("MRS R0, PSP");
	asm("B getStackFrame");
	asm("_MSP:");
	asm("MRS R0, MSP");
	asm("B getStackFrame");
}

uint32_t r0,r1,r2,r3,r12,lr,pc,psr;
void getStackFrame(uint32_t *stackFrame)
{

	r0  = stackFrame[0];
	r1  = stackFrame[1];
	r2  = stackFrame[2];
	r3  = stackFrame[3];
	r12 = stackFrame[4];
	lr  = stackFrame[5];
	pc  = stackFrame[6];
	psr = stackFrame[7];	/* 确保已经运行 */
	asm("BKPT");
}
