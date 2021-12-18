/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�FanControl.h
* �������������� ģ���ͷ�ļ���
* Ϊ��ģ��������������Ҫ���� ��Ҫ��ʱ��ģ��
*
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 *
* �汾��1.1
* ˵������Ӧ .c �ļ�
* ԭ���ߣ�xb
* ������ڣ�2019��9��22��
*
*
* �汾��1.2
* ˵������Ӧ .c �ļ�
* ԭ���ߣ�xb
* ������ڣ�2019��10��12��
* 
* * �汾��1.3
* ˵������Ӧ .c �ļ�
* ԭ���ߣ�xb
* ������ڣ�2020��1��18��
* 
*�汾��1.4  
* ˵������Ӧ .c �ļ� 
* ԭ���ߣ�lyc
* ������ڣ�2020��9��29��
* 
* *�汾��1.5  
* ˵������Ӧ .c �ļ� 
* ԭ���ߣ�zhupan
* ������ڣ�2020��12��8��
*/

#ifndef	FAN_CONTROL_H_
#define	FAN_CONTROL_H_

#include "DataType.h"
#include "ADTreat.h"

//��������״̬
#define    FAN_STATE_CLOSE      0     //�տ�ʼ�Ĺر�״̬
#define    FAN_STATE_START      1     //����״̬
#define    FAN_STATE_PID        2     //PID ����
#define    FAN_STATE_SLOWADJ    3     //���ٵ���
#define    FAN_STATE_STOP       4     //ֹͣ

//ϵͳ��ѹ�������
#define    SYSTEM_VOLT_12              ACD_V(13)      	// 12ϵͳ��ѹ
#define    SYSTEM_VOLT_24              ACD_V(25)      	// 24ϵͳ��ѹ
#define    FANOUT_MAXVOLT12            ACD_V(13.5)    	//����ѹ12
#define    FANOUT_MAXVOLT24            ACD_V(25.5)    	//����ѹ24

#define    IGN_CLOSE_12VOLT            ACD_V(5)          //������ѹС�ڴ˵�ѹʱ ֹͣPID ����
#define    IGN_CLOSE_24VOLT            ACD_V(10)         //24
#define    IGN_STOPADJ_12VOLT          ACD_V(7)          //������ѹС�ڴ˵�ѹʱ �رչķ�����
#define    IGN_STOPADJ_24VOLT          ACD_V(14)         //24

//���ϵ�ѹ���ö���
#define   FANSHORT_VOLT                ACD_V(1)          //�����ռ�ձ�Ϊ<= FANOUT_MINPWM��������ѹ ΪFANSHORT_VOLTʱ��·����  FAN_ERRCODE_SHORT
#define   FANOUT_OPENVOLT              ACD_V(1)          //�����ռ�ձ�>= FANOUT_MINPWM  ����������ѹ�ӽ���Դ��ѹ��FAN_ERRCODE_OPEN
   		                                                //�� u16CurrentVolt <= FANOUT_OPENVOLT
//����ѹ�������� 
#define    VOUT_OFFSET_DEFAULT          248     		      //Vfan /(Vign -Vback) * 256

//�ֽ׶ε�����ѹ
#define    FAN_CONTROL_ACCURACY_VOLT   ACD_V(0.2)       //С�ڴ�ֵ�ﵽ����     
#define    FAN_CONTROL_SLOWADJ_VOLT    ACD_V(0.3)       //С�ڴ�ֵ�������ٵ���

//pwmռ�ձ��������
#define    FANOUT_MAXPWM               10000            //���PWMֵ
#define    MAXOUT_MINPWM               8000             //������ʱ����Ҫ�����ֵ���ܵ�������,
                                                        //�����ж��ķ����·���ϣ���pwm ������<1v�ķ�����ѹ                    

//PID �����λ��
#define   PID_PARAMETER_SHIFTNUM          11            //PID �����λ��ȥ����       
#define   PID_PARAMETER_SHIFTVALUE       ((uint16)0x1 << PID_PARAMETER_SHIFTNUM)
#define   PID_I_MAXPWMVALUE               2             //I ��������ֵ
#define   PID_I_MAXVALUE                  (PID_I_MAXPWMVALUE * PID_PARAMETER_SHIFTVALUE)        
//���ٵ��ڵ�����
#define   FAN_CONTROL_TIME                 10          //��������ms
#define   FANADJ_STOPTIMES                 20          //���� FAN_CONTROL_TIME *n  ms ���ǲ��ھ��ȷ�Χ�ڣ��������ٵ���
#define   FAN_START_TIME                   40          //���� FAN_CONTROL_TIME *n  ms ����ʱ�����
#define   OPEN_ERR_TIME                    100         //����FAN_CONTROL_TIME *n  ms  �����������Ϊ����
#define   SHORT_ERR_TIME                   100         //����FAN_CONTROL_TIME *n  ms  �����������Ϊ����


/*****************************����Ϊ����LIBʱ�ɵ��ڲ���������ΪӦ�ÿɵ��ò��� ********************************/
//PID ����Ĭ��ֵ
#define   FAN_PID_PFACTOR                  100         //����
#define   FAN_PID_IFACTOR                  20          //����
#define   FAN_PID_DFACTOR                  0           //΢�֣���Ϊ0
#define   FAN_SLOWADJ_FACTOR               20           //���ٵ��ڵĲ�����   
//��ѹ������������
#define   FAN_DEFAULT_RATE                 25          //Ĭ�ϵ����ٶ�

//�ķ�������붨��
#define   FAN_ERRCODE_NONE                  0
#define   FAN_ERRCODE_SHORT                0x01        //��·����,�ķ��������·��GND
#define   FAN_ERRCODE_OPEN                 0x02        //��·���ϣ��ķ��������·����Դ��ѹ
#define   FAN_ERRCODE_SUPPERVOLT_LOW       0x04        //�����ѹ����
#define   FAN_ERRCODE_ACCURACY             0x08        //����δ��

#define   FAN_ERRCODE_CIRTERR      (FAN_ERRCODE_OPEN | FAN_ERRCODE_SHORT)



//����������
typedef struct
{
	BOOL   bSystemVolt24;       //0 Ϊ12v��1Ϊ24v	
	BOOL   bFanRelay;           //���ƹķ���̵���
	uint8  u8FanMaxLevel;	    //���λ
	uint8  u8SlowAdjFactor;     //���ٵ��ڵĲ�����Խ������ٶ�Խ�죬�����ȶ��� ����Ϊ0 
	int16  i16FanPFactor;       //��������
	int16  i16FanIFactor;       //��������
	int16  i16FanDFactor;       //΢�����ӣ���Ϊ0
	uint16 u16InitRate;         //Ĭ�ϵ����ٶȣ���0���л���������ʱ�����Զ������ڲ�����Ϊ��ֵ
	uint16 u16MinPwm;           //��Сpwm��pwm����ʱ���Դ�ֵ��ʼ
	ADVoltType *TBL_FanVolt;    //������ѹ�����ϴ�С����
	uint8  u8VoltOffSet;        //�����ϵ���ģ�ʵ�ʵ�ѹ = u8VoltOffSet /256 *��vSuppler- vBack��������200~256 ֮��
}FANCONTROLCONFIG;

//Ӧ�ÿɵ��ú���
BOOL FanControl_Init(FANCONTROLCONFIG* pFConfig);
void FanControl_SetVolt(ADVoltType avSetValue);
void FanControl_SetRate(uint16 u16Rate);
void FanVolt_Control(void);
uint8 FanControl_GetCurrentLevel(void);
ADVoltType FanControl_GetCurrentVolt(void);
uint8 FanControl_ErrCodeGet(void);
#endif
