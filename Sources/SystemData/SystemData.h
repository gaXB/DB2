/************************�ļ�˵��*****************************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SystemData.h
* ���������� 1 ϵͳ�������ⲿ����˵������APPʹ��
* 2 SystemApp.c ϵͳ״̬�л��ĺ�������
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
*/

#ifndef   SYSTEMDATA_H__
#define   SYSTEMDATA_H__

#include "SystemType.h"
#include "SystemConfig.h"
#include "VehicleData.h"
/*******************************************************************************
 * ����ȫ�ֽṹ�嶨�� ,�����붨��
 *
********************************************************************************/
extern SYSTEMCONTROL  SystemControl;
extern SENSORDATA sSenSorData; 
extern OUTDATA sOutData;
extern DISPLAYDATA sDisplayData;
extern uint8  u8ConfigData[3];
extern VEHICLECONTROL  sVehicleControl;
extern EVOUTDATA sEVOutData;

EVSENSORDATA sEVSensorData;
ACTORDATA sActorData;
#if _PROJECT_CAN_
extern PCCONTROLEN_TYPE PCControlEn;
extern SENSORDATA     sSenSorData_PC;
extern EVSENSORDATA     sEVSensorData_PC;
extern OUTDATA sOutData_PC;
extern EVOUTDATA sEVOutData_PC;
extern PANLEDATA sPanleData;
#endif

SYSTEMCONTROL* SystemControlGet(void);


#endif
