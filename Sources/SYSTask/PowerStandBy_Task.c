/************************文件说明*****************************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：PowerStandyBy_Task.c
* 功能描述：电源standby时的系统任务处理
* 提供模板 ，按实际应用编写
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年5月30日
*/

#include "PowerMange.h"
#include "APPDrive.h"



/*******************************************************************************
 * Function:        void PowerStandByTask(void)
 *
 * Description:     电源等待休眠的工作任务
 *
 * Returns:         none
 *
 * Notes:           主循环调用
 *
********************************************************************************/
void PowerStandByTask(void)
{
	//点火电压故障
	//TODO: 在此编写电源模式在standby时候的处理，此处可将电源任务切换到OFF，
	//比如在驻车模式时，电源OFF时，等待CANsleep 此时可切换到OFF
	/*if (有驻车按键时)
	{	
		PowerMN_AppSleepEn(0);
	}
	else
	{} */
	sPanleData.LCDONOFF = 0;
	sPanleData.AC = 0;
	sPanleData.CIRF = 0;
	sPanleData.FANLevel = 0;
	sPanleData.PTCn = 0;
	sPanleData.RHEAT = 0;
	sPanleData.VentMode = 0;
	sPanleData.VerClient = SwV_CLIENT *16 + SwsV_CLIENT;
	sPanleData.VerSelf = SwV_ *16 + SwsV_;
	sPanleData.i16Temp = SystemControl.u8TestLevel*10;
}

/*******************************************************************************
 * Function:        void PowerStandByTask_Enter(void)
 *
 * Description:     进入powerstandy  模式
 *
 * Returns:         none
 *
 * Notes:           进入时由pm_app外部调用
 *
********************************************************************************/
void PowerStandByTask_Enter(void)
{

}
