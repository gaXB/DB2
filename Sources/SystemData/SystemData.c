/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SystemData.c
* ���������� ϵͳ����
* 1 Ӧ�ÿ��Ʊ���������
* 2 ����������
* 3 ���/��ʾ����
* �������� codewarrior IDE 11.1
* MCU˵������MCU �޹�
*  ���ļ���Ϊģ�壬�������ø���
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��13��
* 
* �汾��1.1
* ˵���������������ǿ�ƹ���
* ���ߣ�xb
* ������ڣ�2020��5��30��
* 
* �汾��1.2
* ˵����OtherControl �ŵ�VehicleData ��֤���ļ��Ķ���С
* ���ߣ�xb
* ������ڣ�2020��11��21��
* 
* �汾��1.3
* ˵����NOINIT ��������Renesas
* ���ߣ�xb
* ������ڣ�2021��8��23��
*/
#include "SystemData.h"

/*************************************************************************************************4
 * ϵͳ״̬��������
****************************************************************************************************/

#if  NOINIT_TYPE ==  NOINIT_TYPE_KEA
__attribute__ ((section(".noinit"))) SYSTEMCONTROL  SystemControl;    //ϵͳ���� 
__attribute__ ((section(".noinit"))) SENSORDATA     sSenSorData;      //�������ź� �ṹ��
VEHICLECONTROL  sVehicleControl;
EVSENSORDATA sEVSensorData;
ACTORDATA sActorData;
PANLEDATA sPanleData;
#elif NOINIT_TYPE == NOINIT_TYPE_RL78
#pragma section bss NOINIT
SYSTEMCONTROL  SystemControl;    //ϵͳ���� 
SENSORDATA     sSenSorData;      //�������ź� �ṹ��
#pragma section 
#define __GOLBAL_VAR_
#endif


uint8 u8ConfigData[3];  //������
OUTDATA sOutData;
EVOUTDATA sEVOutData;
DISPLAYDATA sDisplayData;

#if _PROJECT_CAN_
//pc ���ñ���
PCCONTROLEN_TYPE PCControlEn;
SENSORDATA     sSenSorData_PC;
EVSENSORDATA sEVSensorData_PC;
OUTDATA sOutData_PC;
EVOUTDATA sEVOutData_PC;
#endif
/*******************************************************************************
 * Function:   SYSTEMCONTROL* SystemControlGet(void)
 *
 * Description: ����ϵͳ����״̬
 *
 * Returns:     SYSTEMCONTROL*
 *
 * Notes:     �ⲿģ�����
 *
*******************************************************************************/
SYSTEMCONTROL* SystemControlGet(void)
{
	return &SystemControl;
}
