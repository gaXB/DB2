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
volatile CANMsg12F85351Union sCANMsg12F85351;
volatile uint8 CANMsg12F85351Status;

volatile CANMsg12F98851Union sCANMsg12F98851;
volatile uint8 CANMsg12F98851Status;

//�������ݱ�������
volatile CANMsg0EF87372Union sCANMsg0EF87372;
volatile uint8 CANMsg0EF87372Status;

volatile CANMsg12F81018Union sCANMsg12F81018;
volatile uint8 CANMsg12F81018Status;

volatile CANMsg12F85050Union sCANMsg12F85050;
volatile uint8 CANMsg12F85050Status;



//CAN2 ������

//�������ݱ�������
volatile CANMsg410Union sCANMsg410;
volatile uint8 CANMsg410Status;

volatile CANMsg44EUnion sCANMsg44E;
volatile uint8 CANMsg44EStatus;

volatile CANMsg453Union sCANMsg453;
volatile uint8 CANMsg453Status;

volatile CANMsg454Union sCANMsg454;
volatile uint8 CANMsg454Status;

volatile CANMsg455Union sCANMsg455;
volatile uint8 CANMsg455Status;

volatile CANMsg456Union sCANMsg456;
volatile uint8 CANMsg456Status;

volatile CANMsg457Union sCANMsg457;
volatile uint8 CANMsg457Status;

volatile CANMsg551Union sCANMsg551;
volatile uint8 CANMsg551Status;

//�������ݱ�������
volatile CANMsg115Union sCANMsg115;
volatile uint8 CANMsg115Status;

volatile CANMsg131Union sCANMsg131;
volatile uint8 CANMsg131Status;

volatile CANMsg22FUnion sCANMsg22F;
volatile uint8 CANMsg22FStatus;

volatile CANMsg424Union sCANMsg424;
volatile uint8 CANMsg424Status;

volatile CANMsg432Union sCANMsg432;
volatile uint8 CANMsg432Status;

volatile CANMsg451Union sCANMsg451;
volatile uint8 CANMsg451Status;

volatile CANMsg452Union sCANMsg452;
volatile uint8 CANMsg452Status;

volatile CANMsg460Union sCANMsg460;
volatile uint8 CANMsg460Status;

volatile CANMsg470Union sCANMsg470;
volatile uint8 CANMsg470Status;

volatile CANMsg472Union sCANMsg472;
volatile uint8 CANMsg472Status;

volatile CANMsg490Union sCANMsg490;
volatile uint8 CANMsg490Status;

volatile CANMsg26DUnion sCANMsg26D;
volatile uint8 CANMsg26DStatus;
