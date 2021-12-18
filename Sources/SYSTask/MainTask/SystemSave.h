/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SystemSave.h
* 功能描述：SystemSave的头文件，
* 编译器： codewarrior IDE 10.7
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
* * 版本：1.2
* 说明：把flash相关的记忆放在SystemSaveFlash 模块,其他ram存储放在此区域
* 作者：xb
* 完成日期：2019年9月5日
*/
#ifndef SYSTEMSAVE_H_
#define SYSTEMSAVE_H_

#include "SystemData.h"

#define DEF_OPTYPE_AC   1
#define DEF_OPTYPE_NCF  2
#define DEF_OPTYPE_MODE 3
#define DEF_OPTYPE_FAN  4 


//外部接口函数
void Def_Operat(uint8 OPType);
void LoadMode_BeforDef(void);
void Save_Def (void);
void Load_Def (void);
void Save_Off (void);
void Load_Off (void);
void Save_Park (void);
void Load_Park (void);
void Save_LOHI (void);
void Load_LOHI (void);
void Save_MaxAC(void);
void Load_MaxAC(void);
#endif /* SYSTEMSAVE_H_ */
