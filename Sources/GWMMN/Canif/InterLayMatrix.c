/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�InterLayMatrix.c
* ����������InterLay��ı��������ļ�����Ҫ������Ŀ����
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
* ���ļ���PC �Զ����ɣ�PC �汾 V1.0, DBC �ļ�����  CVTC_SV51-02&B1_CMX_20200409_BD_V4.81 - HCM.dbc
* �������ڣ�  01/28/21 09:23:21
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�
* ������ڣ�
*/
#include "InterLayMatrix.h"

/***  ���Ա�ṹ  ***/ 


//�������ݱ�������
volatile CANMsg0C9Union sCANMsg0C9;
volatile uint8 CANMsg0C9Status;

volatile CANMsg1F1Union sCANMsg1F1;
volatile uint8 CANMsg1F1Status;

//�������ݱ�������
volatile CANMsg1F2Union sCANMsg1F2;
volatile uint8 CANMsg1F2Status;

volatile CANMsg1F5Union sCANMsg1F5;
volatile uint8 CANMsg1F5Status;

