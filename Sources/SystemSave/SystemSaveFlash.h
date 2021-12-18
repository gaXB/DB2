/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SystemSaveFlash.h
* 功能描述：SystemSaveFlash的头文件，
* 编译器： codewarrior IDE 11.1
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年9月5日

* 版本：2.0
* 说明：跟随.c
* 原作者：xb
* 完成日期：2019年12月30日
* 
* ** * 版本：3.1
* 说明：1 增加 MCUdata的存储
* 作者：xb
* 完成日期：2020年5月13日
* 
* 版本：4.0
* 说明：1 跟随.c
* 作者：xb
* 完成日期：2020年12月9日
*/

#ifndef SYSTEMSAVEMD_H_
#define SYSTEMSAVEMD_H_
#include "DataType.h"

#define  SAVE_IGNDEALY   5 //*0.1


//外部可调用函数
void FLASH_EE_Init(void);
BOOL Load_ALLCycEE(void);
//标定参数
void Save_Parameter(uint8 *rtx);
BOOL Load_Parameter(void);
void  Save_Parameter_Sec(uint8 *rtx);
BOOL  SetParameterData_Sec(uint8* rtx);
//系统记忆
void MemoryControl(void);
BOOL Load_Ign(void);
//programdata
void Load_ProgramData(void);
void Save_ProgramData(void);
//UDS 接口
void Save_UDSDID(void);
void Save_UDSConfig(void);
void Save_ScurityTimes(void);
#endif /* SYSTEMSAVEMD_H_ */
