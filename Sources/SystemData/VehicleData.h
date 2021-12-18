/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�VehicleData.h
* ���������� �������������Ͷ��壬Ĭ��ֵ���ⲿ��������
* 
* �������� codewarrior IDE 11.1
* MCU˵������MCU �޹�
* ����Ӧ��Ҫ������
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��13��
* 
* 
*/
#ifndef VEHICLEDATA_H_
#define VEHICLEDATA_H_
#include "SystemType.h"




//Key state
enum{          
	VCUKEY_OFF=0,
	VCUKEY_ON,
	VCUKEY_START,
	VCU_KEY_ERROR,
}; 

//����״ֵ̬���ڴ�����
typedef struct
{
	uint8   RHeatBack;              //���˪����״̬
	uint8   VCUKeyState;	
	uint16  EXVPostion;             //�������ͷ�����λ��
	uint16 u16EngSpeed;             //����������ת��
}VEHICLEDATA;

//��������״̬���ڴ�����
typedef struct
{
	uint8 CHeatMode_L;
	uint8 CHeatMode_R;    //�������μ���
}OTHERCONTROL ;



extern OTHERCONTROL OtherControl;
extern VEHICLEDATA  sVehicleData;




#define  _KEY_STATE_DEFAULT_  2            /****��Կ��Ĭ��״̬  START*****/

#endif /* VEHICLEDATA_H_ */
