/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�motor.h
* �������������ģ���ͷ�ļ���
* Ϊ��ģ��������������Ҫ���� ��Ҫ��ʱ��ģ�飬����ѧ����
*
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 *
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��5��20��
*
* �汾��1.1
* ˵��������ͣ�����������趨λ��=255ʱ ���ͣ��
* ԭ���ߣ�xb
* ������ڣ�2020��9��28��
* 
* * �汾��1.2
* ˵����
* 1 ���������ʱδ����DriveState��bug�޸�������AD MOTOR_STOPSHAKE_AD ��2->3
* 2 �������ֵ-Ĭ��ֵ��3��Ϊ5
* ԭ���ߣ�xb
* ������ڣ�2020��12��2��
* 
* �汾��1.3
* ˵����
* 1 ����ֵMOTOR_DRIVE_ACCURACY �ɺ궨���Ϊint16,Ĭ��5
* 2 ��ǰ��Ĭ��Ϊ1��������
* 3 ����MotorDriveInit ���趨���Ⱥ���ǰ��
* ���ߣ�xb
* ������ڣ�2020��12��5��
*/
#ifndef MOTOR_H__
#define MOTOR_H__


#include "timerout.h"

#define  MAX_MOTOR_NUM     4            //���֧�ֵ������
#define  MOTOR_RUN_TIME    2            //����ʱ��
//#define  MOTOR_SHORTERROR_SHORT        1
//#define  MOTOR_SHORTERROR_OPEN         2

//�����������״̬
#define  MOTOR_STATE_HALT             0x00   //���ͣ��
#define  MOTOR_STATE_FORWARD          0x01   //�������
#define  MOTOR_STATE_BACKWARD         0x02   //�����ת
#define  MOTOR_STATE_JAMMED_ERROR     0x03   //�����ת����
#define  MOTOR_STATE_RESET            0x05   //�����ʼ��
#define  MOTOR_STATE_WAITHALT         0x09   //����ת�л�ʱֹͣ1s
//#define  MOTOR_STATE_JUMP_ERROR       0x10   // ���������� ,δ�õ�������Ԥ��
#define  MOTOR_STATE_SHAKE            0x11   //�������״̬



//�����·����ʱ��״̬
#define  MOTOR_STATE_SHORT_ENTER       0x12      //��·��������
#define  MOTOR_STATE_SHORT_FORWARD     0x13      //�����·��ת
#define  MOTOR_STATE_SHORT_BACKWARD    0x14      //��·��������
#define  MOTOR_STATE_SHORT_GETPOSTION  0x15      //�ҵ�λ�õ�״̬
#define  MOTOR_STATE_SHORT_LOSE        0x16      //��ʧ����


//����Լ칤��ʱ��״̬
#define  MOTOR_STATE_TEST_ENTER           0x17   //�Լ����
#define  MOTOR_STATE_TEST_FORWARD         0x18   //�Լ���ת
#define  MOTOR_STATE_TEST_BACKWARD        0x19   //�Լ췴ת
#define  MOTOR_STATE_TEST_GOTOMIDDLE      0x20   //�ص��м�
#define  MOTOR_STATE_TEST_END             0x21   //�Լ��˳�
                                                           
//�������ģʽ
#define  MOTOR_WORK_NORMAL    0x00     //��������
#define  MOTOR_WORK_SHORT     0x01      //��·����
#define  MOTOR_WORK_SELFTEST  0x02      //�Լ칤��
#define  MOTOR_WORK_STOP      0x03      //ֹͣ����

//������
#define  MOTOR_ERROR_NONE     0x00   //�������
#define  MOTOR_ERROR_JAMMED   0x01   //�����ת
#define  MOTOR_ERROR_TIMEOUT  0x02   //�����ʱ
#define  MOTOR_ERROR_SHORT    0x04   //�����·
#define  MOTOR_ERROR_OPEN     0x08   //��·
#define  MOTOR_ERROR_SHAKE    0x10   //�����������
#define  MOTOR_ERROR_TEST     0x80   //���Թ���
#define  MOTOR_ERROR_INVALID  0xff   //�����Ч
#define  MOTOR_ERROR_BACKERR  (MOTOR_ERROR_SHORT | MOTOR_ERROR_OPEN)

#define  TEST_ERROR_NONE     		0x00
#define  TEST_ERROR_SHAKE     		0x01
#define  TEST_ERROR_NOTGETPOSTION   0x02
#define  TEST_ERROR_RANGESMALL        0x04      //��Χ������
#define  TEST_ERROR_RANGEOUT        0x08     //��Χ����
#define  TEST_ERROR_DOING     		0x80
//ϵͳ���
#define  MOTOR_SET_AD                  3  // 2�ε������λ�ô��ڵȴ�ֵ���ܸı�setֵ��д��0 ��һ���Ͷ�
#define  MOTOR_JUMP_AD                 5  //2�ε��ad�ɼ�ֵ���ֵ����
#define  MOTOR_RUNHATL_LEASTTIME       5  // �����ת����ֹͣ�ٵ�ת��0.1*
#define  DEFAULT_MOTOR_BEFORDATA       1  // �����ǰ�� 
#define  DEFAULT_MOTOR_DRIVE_ACCURACY  5  //ad ����ľ���

//���ϻָ�����
#define  MOTOR_SHORTERROR_WAITTIME   5   //��·���Ϻ������ʱ�䣨��ʱ�ź��ȶ����ſ�ʼ������
#define  MOTOR_SHORT_FORWARD_TIME    100   //����ʱ������תʱ��
#define  MOTOR_SHORT_BACKWARD_TIME   20
#define  MOTORERR_SHORT_TIMES      20     //5ms���ڵĻ�����100msΪ����

#define  MOTOR_STOP_AD			255
#define  MOTOR_OPEN_AD        250         //��·��ad
#define  MOTOR_SHORT_AD       5           //��·��ad
#define  DEAL_SHORTRECOVE_AD  10          //��·����ʱ���λ�ò���ڴ�ֵ�Ž��д���

//����ʱ��غ궨��
#define   MOTOR_SHAKEERROR_WAITTIME   10  //shake ���ϻָ���Ҫ��ʱ��
#define   MOTOR_RUNSHAKE_AD         1     //�˶����������shake ��ad
#define   MOTOR_STOPSHAKE_AD     3     //ֹͣ������´��ڴ�ֵ���㶶��
#define   MAX_ERECOVE_TIMES      10      //���ϻָ��Ĵ���,δ��
#define   MOTOR_JUMP_AD          5   //2�ε��ad�ɼ�ֵ���ֵ����


//�Լ�
#define  TEST_FORWARD_TIME          100         // ��������ת   
#define  TEST_FORWARD_AD            240         // ��ת��ʱ�����adֹͣ������
#define  TEST_BACKWARD_TIME         100         // ��תʱ �� ��ʱ�� *0.1 
#define  TEST_BACKWARD_AD           20          // �Լ�ʱ �����ad ����
#define  TEST_MIDDLE_AD            128          // �������MOTOR_DRIVE_ACCURACY




//ϵͳ��Ҫ���ܿ����ã�����ΪԤ����
#define   NEEDFUNCTION_ELESMALL          0   // ��С�綯ģʽ����ģʽʱ���û�й��ϱ������ܣ�
//ֻ�а��յ����ѹ���ж������ж�ת�������ܣ���ʱ������С

#if    NEEDFUNCTION_ELESMALL 
	#define   SHORT_AUTORECOVEY_MAXTIMES     0
	#define   NEEDFUNCTION_SELF_TSET         0     //�Լ칦��
	#define   NEEDFUNCTION_VOLTMANGE         1     //��Դ�����ܣ��ı�����ǰ��
	#define   NEEDFUNCTION_DEALSHAKE         0     //�Ƿ���Ҫ��������
	#define   NEEDFUNCTION_SHORT_AUTORECOVEY 0     //��·�Զ��ָ�����
#else
	#define   SHORT_AUTORECOVEY_MAXTIMES     2
	#define   NEEDFUNCTION_SELF_TSET         1     //�Լ칦��
	#define   NEEDFUNCTION_VOLTMANGE         0     //��Դ�����ܣ��ı�����ǰ��
	#define   NEEDFUNCTION_DEALSHAKE         1     //�Ƿ���Ҫ�������� �� �˴�1 ֻ����ֹͣʱ�Ķ�����2 �˶�ʱҲ����
	#define   NEEDFUNCTION_SHORT_AUTORECOVEY 1     //��·�Զ��ָ�����
#endif


uint8 Motor_Init(uint16 outtime,uint8 njame,uint8 nmotor);
uint8 Motor_SelfTset(uint8 id);
void  Motor_Reset(void);
void  Motor_Control(void);
//void Motor_SetError(uint8 mode);
uint8 Motor_GetPosition(uint8 motorId);
BOOL  Motor_SetPosition(uint8 motorId,uint8 mPosition); 
uint8 Motor_GetErrorCode(uint8 motorId);
uint8 Motor_GetSeltTestCode(uint8 motorId);
uint8 Motor_GetSetPosition(uint8 motorId);
uint8 Motor_RunControl(uint8 motorId,uint8 motorState);
BOOL MotorDriveInit(uint8 u8AccData, uint8 u8BeforData);   //1.3���Ӵ˺���
#endif

