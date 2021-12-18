/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SystemSaveFlash.h
* ����������SystemSaveFlash��ͷ�ļ���
* �������� codewarrior IDE 11.1
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��9��5��

* �汾��2.0
* ˵��������.c
* ԭ���ߣ�xb
* ������ڣ�2019��12��30��
* 
* ** * �汾��3.1
* ˵����1 ���� MCUdata�Ĵ洢
* ���ߣ�xb
* ������ڣ�2020��5��13��
* 
* �汾��4.0
* ˵����1 ����.c
* ���ߣ�xb
* ������ڣ�2020��12��9��
*/

#ifndef SYSTEMSAVEMD_H_
#define SYSTEMSAVEMD_H_
#include "DataType.h"

#define  SAVE_IGNDEALY   5 //*0.1


//�ⲿ�ɵ��ú���
void FLASH_EE_Init(void);
BOOL Load_ALLCycEE(void);
//�궨����
void Save_Parameter(uint8 *rtx);
BOOL Load_Parameter(void);
void  Save_Parameter_Sec(uint8 *rtx);
BOOL  SetParameterData_Sec(uint8* rtx);
//ϵͳ����
void MemoryControl(void);
BOOL Load_Ign(void);
//programdata
void Load_ProgramData(void);
void Save_ProgramData(void);
//UDS �ӿ�
void Save_UDSDID(void);
void Save_UDSConfig(void);
void Save_ScurityTimes(void);
#endif /* SYSTEMSAVEMD_H_ */
