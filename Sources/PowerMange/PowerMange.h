/************************文件说明********************************************
* Copyright (c) 2015 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：PowerMange.h
* 功能描述：PowerMange 的头文件，为实现此功能需要包含以下文件。具体配置按项目要求更改宏定义
1 AD模块 
2 如果需要使用网络管理，加入网络管理的文件并
更改配置位 _PM_INDIRECTNM_和  _PM_OSKNM_ 
*
*/
/**************************更改记录********************************************
* 版本：1.2
* 说明：跟随.c文件
* * 作者：xb
* 完成日期：2019年10月21日
* 
* 版本：1.3
* 说明：跟随.c文件
* * 作者：xb
* 完成日期：2019年12月11日
*
* 版本：1.4
* 说明：跟随.c文件， 增加_PM_AUTOSARNM_
* * 作者：xb
* 完成日期：2019年12月24日
* 
* 版本：1.5
* 说明：跟随.c文件
* * 作者：xb
* 完成日期：2020年3月7日
*  
* 版本：1.6
* 说明：跟随.c文件
* * 作者：xb
* 完成日期：2020年5月30日
* 
*/
#ifndef POWERMANGE_H_
#define POWERMANGE_H_

#include "DataType.h"
//功能配置
#define  _PM_AUTOSARNM_           0 
#define  _PM_OSKNM_               0              //是否用OSEK
#define  _PM_INDIRECTNM_          1              //是否用接间网络管理
#define  _PM_USECAN_              1     //是否使用CAN 功能
#define  _BATT_COLLECT_           0     //暂未实现
#define  _POWERM_CANWAKE_RESET_   1     //can 唤醒时直接复位则为1，影响结果为第一帧时间变慢

#define   IGNVOLT_WORK_ERRU    1640     //电源故障的高值
#define   IGNVOLT_WORK_ERRD    860      //电源故障的低值
#define   IGNVOLT_WORK_OKU     1600     //电源恢复的高值
#define   IGNVOLT_WORK_OKD     900      //电源恢复的低值
#define   IGN_VOLT_SLEEP       600      //休眠的电压值
#define   IGN_VOLT_WAKEUP      700      //唤醒电压

#define   PM_VOLT_ERR          1
#define   PM_VOLT_OK           0

//点火延时时间
#define   PM_LIGN_ERR_TIME    200         //电压低时，故障的时间， *MS；
#define   PM_HIGN_ERR_TIME    500         //电压高时，故障的时间， *MS；根据5b实验要求应该400以上
#define   PM_IGN_OK_TIME      30          //电源Off 到ON 的延时时间

//系统电源状态
typedef enum
{
	POWER_STATE_INIT = 0,                //初始化
	POWER_STATE_SLEEP,                   //此时MCU 进入休眠状态,在此处循环
	POWER_STATE_WORK,                    //控制器工作电压 9- 16v
	POWER_STATE_STANDBY,                 //电源已经没了（应用已经不需要工作），等待CAN 休眠
	POWER_STATE_OFF,                     //电源OFF状态，仍可发 CAN,
	POWER_STATE_STANDBY_WAIT,            //从CAN 已经休眠到休眠的过度阶段，注意此时应用任务不能再处理
}PMStateType;

void PowerNM_Init(void);
void PowerMange(void);
void PowerMN_AppSleepEn(uint8 bEn);
uint8 PowerMN_GetPowerState(void);
uint8 PowerMN_GetIgnState(void);
void PowerMN_CanSleepEn(uint8 bEn);
/*Basic PM CallBack function for the Application*/
extern void ApplPM_StateChange(const PMStateType pmPreviousState, const PMStateType pmCurrentState);
#endif /* POWERMANGE_H_ */
