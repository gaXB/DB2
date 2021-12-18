/************************�ļ�˵��********************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�PowerMange.h
* ����������PowerMange ��ͷ�ļ���Ϊʵ�ִ˹�����Ҫ���������ļ����������ð���ĿҪ����ĺ궨��
1 ADģ�� 
2 �����Ҫʹ����������������������ļ���
��������λ _PM_INDIRECTNM_��  _PM_OSKNM_ 
*
*/
/**************************���ļ�¼********************************************
* �汾��1.2
* ˵��������.c�ļ�
* * ���ߣ�xb
* ������ڣ�2019��10��21��
* 
* �汾��1.3
* ˵��������.c�ļ�
* * ���ߣ�xb
* ������ڣ�2019��12��11��
*
* �汾��1.4
* ˵��������.c�ļ��� ����_PM_AUTOSARNM_
* * ���ߣ�xb
* ������ڣ�2019��12��24��
* 
* �汾��1.5
* ˵��������.c�ļ�
* * ���ߣ�xb
* ������ڣ�2020��3��7��
*  
* �汾��1.6
* ˵��������.c�ļ�
* * ���ߣ�xb
* ������ڣ�2020��5��30��
* 
*/
#ifndef POWERMANGE_H_
#define POWERMANGE_H_

#include "DataType.h"
//��������
#define  _PM_AUTOSARNM_           0 
#define  _PM_OSKNM_               0              //�Ƿ���OSEK
#define  _PM_INDIRECTNM_          1              //�Ƿ��ýӼ��������
#define  _PM_USECAN_              1     //�Ƿ�ʹ��CAN ����
#define  _BATT_COLLECT_           0     //��δʵ��
#define  _POWERM_CANWAKE_RESET_   1     //can ����ʱֱ�Ӹ�λ��Ϊ1��Ӱ����Ϊ��һ֡ʱ�����

#define   IGNVOLT_WORK_ERRU    1640     //��Դ���ϵĸ�ֵ
#define   IGNVOLT_WORK_ERRD    860      //��Դ���ϵĵ�ֵ
#define   IGNVOLT_WORK_OKU     1600     //��Դ�ָ��ĸ�ֵ
#define   IGNVOLT_WORK_OKD     900      //��Դ�ָ��ĵ�ֵ
#define   IGN_VOLT_SLEEP       600      //���ߵĵ�ѹֵ
#define   IGN_VOLT_WAKEUP      700      //���ѵ�ѹ

#define   PM_VOLT_ERR          1
#define   PM_VOLT_OK           0

//�����ʱʱ��
#define   PM_LIGN_ERR_TIME    200         //��ѹ��ʱ�����ϵ�ʱ�䣬 *MS��
#define   PM_HIGN_ERR_TIME    500         //��ѹ��ʱ�����ϵ�ʱ�䣬 *MS������5bʵ��Ҫ��Ӧ��400����
#define   PM_IGN_OK_TIME      30          //��ԴOff ��ON ����ʱʱ��

//ϵͳ��Դ״̬
typedef enum
{
	POWER_STATE_INIT = 0,                //��ʼ��
	POWER_STATE_SLEEP,                   //��ʱMCU ��������״̬,�ڴ˴�ѭ��
	POWER_STATE_WORK,                    //������������ѹ 9- 16v
	POWER_STATE_STANDBY,                 //��Դ�Ѿ�û�ˣ�Ӧ���Ѿ�����Ҫ���������ȴ�CAN ����
	POWER_STATE_OFF,                     //��ԴOFF״̬���Կɷ� CAN,
	POWER_STATE_STANDBY_WAIT,            //��CAN �Ѿ����ߵ����ߵĹ��Ƚ׶Σ�ע���ʱӦ���������ٴ���
}PMStateType;

void PowerNM_Init(void);
void PowerMange(void);
void PowerMN_AppSleepEn(uint8 bEn);
uint8 PowerMN_GetPowerState(void);
uint8 PowerMN_GetIgnState(void);
void PowerMN_CanSleepEn(uint8 bEn);
/*Basic PM CallBack function for the Application*/
extern void ApplPM_StateChange(const PMStateType pmPreviousState, const PMStateType pmCurrentState);
#endif /* POWERMANGE_H_ */
