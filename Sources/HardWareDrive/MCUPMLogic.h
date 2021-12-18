/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：MCUPMLogic.h
* 功能描述：MCU 掉电逻辑头文件
* 编译器： codewarrior IDE 11.1
* MCU说明：跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年5月13日
* 
* 
*/

#ifndef MCUPMLOGIC_H_
#define MCUPMLOGIC_H_

//外部可查看的寄存器，及其含义
extern uint32 u32MCUData[7];
#define   MCUDATA_NUM_REST             0
#define   MCUDATA_NUM_IGNCOUNT         1
#define   MCUDATA_NUM_REST_1           2
#define   MCUDATA_NUM_RESETCOUNT       3
#define   MCUDATA_NUM_IGNWAKE          4
#define   MCUDATA_NUM_FLG              5 

//LDO 的模式， 由整体宏定义_PROJECT_LDO_MODE 来决定
#define   LDO_MODE_BATTIGN        0
#define   LDO_MODE_2575           1
#define   LDO_MODE_IGN            2

void MCUPM_SetReset(uint32 u32ResonRegister);
void MCUPM_IgnOn(void);
void MCUPM_Over(void);
void MCUPM_ReadEE(void);
BOOL MCUPM_IsLPowerRest(void);
BOOL MCUPM_IsLastIgnOFFNormal(void);
BOOL MCUPM_IgnCountReset(uint32 u32times);
uint32 MCUPM_PowerOnResetTimes(void);

#endif /* MCUPMLOGIC_H_ */
