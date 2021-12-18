/************************�ļ�˵��*****************************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�PowerMange��_App.c
* ������������Դ����ص������ӿ�
* �ṩ�ӿں������������ݰ�Ӧ��Ҫ���д
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
* 
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��19��
* 
* * �汾��1.1
* ˵�������� ApplPM_SleepEnter �� ApplPM_SleepReady �ص�����
* ���ߣ�xb
* ������ڣ�2019��3��30��
* 
*  �汾��1.2
*  ���ߣ�xb
* ������ڣ�2019��10��21��
* 
* �汾��1.3
* ˵���� ����pm 1.3�汾
*  ���ߣ�xb
* ������ڣ�2019��10��21��
*
* �汾��1.4
* ˵���� ����pm 1.4�汾
*  ���ߣ�xb
* ������ڣ�2019��12��24��
* 
* * * �汾��1.5
* ˵��������.c�ļ�
* * ���ߣ�xb
* ������ڣ�2020��3��7��
* 
* * * * �汾��1.5.1
* ˵����1 ���� MCUPMLogic ģ�������
* 2 ��2575��·ʱ��IGN ON ʱ���ܿ���5ven
* ���ߣ�xb
* ������ڣ�2020��5��13��
* 
* �汾��1.6
* ˵����
* 1) ���1.6 ��pm
* ���ߣ�xb
* ������ڣ�2020��5��30��
*/
#include "DataType.h"
#include "APPDrive.h"
#include "PowerMange.h"

/*******************************************************************************
��Ҫ�ⲿ���ú�������
********************************************************************************/
extern void PowerOffTask_Enter(void);
extern void PowerStandByTask_Enter(void);
extern void SigRefresh_IgnOFF(void);
extern void SigRefresh_IgnON(void);
extern void System_GOTOInitTask(void);
//extern void Lin_Init(void);

/*******************************************************************************
 * Function:       void ApplPM_IgnOFF(void)
 *
 * Description:    ��Դ�����POWER_STATE_WORK->POWER_STATE_OFFʱ����һ��
 *
 * Returns:         none
 *
 * Notes:          �ص�������ͬ�� ��д�������Ҫ��ִ�е�����
 *
********************************************************************************/
void ApplPM_IgnOFF(void)
{
	PowerOffTask_Enter();       //����offtask��֪ͨtask
	SigRefresh_IgnOFF();       //֪ͨ�ź�ˢ��IgnOFF��
}
/*******************************************************************************
 * Function:      void ApplPM_IgnON(void)
 *
 * Description:    ��Դ�����POWER_STATE_INIT/POWER_STATE_OFF/ POWER_STATE_STANDBY
 *                                                  ->POWER_STATE_ON ʱ����һ��
 *
 * Returns:         none
 *
 * Notes:        ��Դ��OFF��ON ʱ�Ĵ�������
 *
********************************************************************************/
void ApplPM_IgnON(void)
{
#if (_PROJECT_LDO_MODE == LDO_MODE_2575)
	TrOut_5VEN(1);                    //����2575
#endif
	System_GOTOInitTask();            //֪ͨϵͳ״̬�����ʼ��ģʽ
	Motor_Reset();                    //�����ʼ��
	SigRefresh_IgnON();               //֪ͨ�ź�ˢ��ģ��
	MCUPM_IgnOn();                    //MCU PM ģ��̶��÷�
#if _PM_AUTOSARNM_
	CanNm_NetworkRequest(0);
#endif
}


/*******************************************************************************
 * Function:      void ApplPM_SleepReady(void)
 *
 * Description:     ����STANDBY_WAIT ״̬��׼��˯�ߵ�֪ͨAPP
 *
 * Returns:         none
 *
 * Notes:       ��ʱCANEN�Ѿ��ص�����ȴ�1s�����sleepģʽ����1s���Դ���һЩ��Ҫ��ʱ������ɵ�����
 *              ���磺 ����1��LIN˯���źš�
 *
********************************************************************************/
void ApplPM_SleepReady(void)
{

}

/*******************************************************************************
 * Function:      void ApplPM_SleepEnter(void)
 *
 * Description:     ����SLEEP ǰ֪ͨAPP
 *
 * Returns:         none
 *
 * Notes:       ��Ҫ�ر�������������͹���
 *
********************************************************************************/
void ApplPM_SleepEnter(void)
{
	Mcp_Close();
	MCUPM_Over();
	Init_GPIO_0(); 
}
/*******************************************************************************
 * Function:      void ApplPM_WakeUp(void)
 *
 * Description:    ��Դ�������ߵ�����ʱ����һ��
 *
 * Returns:         none
 *
 * Notes:        CAN�͵���Ѷ������ ���ֱ�Ӹ�λ���������
 *
********************************************************************************/
void ApplPM_WakeUp(void)
{
	Init_GPIO_1(); 
	//Lin_Init();
	
	System_GOTOInitTask();
}
/*******************************************************************************
 * Function:      void ApplPM_WakeUp(void)
 *
 * Description:    ��Դ��������CAN����ʱ����һ��
 *
 * Returns:         none
 *
 * Notes:        ���ֱ�Ӹ�λ���������
 *
********************************************************************************/
void ApplPM_CANWakeUp(void)
{

}


void ApplPM_StateChange(const PMStateType pmPreviousState, const PMStateType pmCurrentState)
{
	
	switch (pmCurrentState)
	{//�л����״̬
		default:
		case	POWER_STATE_INIT:
			break;
		case POWER_STATE_WORK:
			ApplPM_IgnON();
			break;
		case POWER_STATE_OFF:
			if (pmPreviousState != POWER_STATE_STANDBY)
			{
				ApplPM_IgnOFF();
			}
			else{}
			break;
		case POWER_STATE_SLEEP:
			ApplPM_SleepEnter();
			break;
		case POWER_STATE_STANDBY:
			PowerStandByTask_Enter();
			break;
		case POWER_STATE_STANDBY_WAIT:
			ApplPM_SleepReady();
			break;	
	}
	
	if (pmPreviousState == POWER_STATE_SLEEP && pmCurrentState == POWER_STATE_INIT)
	{
		ApplPM_WakeUp();
	}
	
}
