/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SystemSave.h
* ����������SystemSave��ͷ�ļ���
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
* * �汾��1.2
* ˵������flash��صļ������SystemSaveFlash ģ��,����ram�洢���ڴ�����
* ���ߣ�xb
* ������ڣ�2019��9��5��
*/
#ifndef SYSTEMSAVE_H_
#define SYSTEMSAVE_H_

#include "SystemData.h"

#define DEF_OPTYPE_AC   1
#define DEF_OPTYPE_NCF  2
#define DEF_OPTYPE_MODE 3
#define DEF_OPTYPE_FAN  4 


//�ⲿ�ӿں���
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
