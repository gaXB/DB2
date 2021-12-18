/************************�ļ�˵��*****************************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�PowerMange.c
* ������������Դ����ģ���������ļ������ļ����ܱ�����
* 1  ��Դ״̬�л� 
* 2  ���ߣ�����
* 3  ����������
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * ���ļ����ܱ�����
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��19��
* 
* �汾��1.1
* ˵����
* 1 ����_PM_USECAN_��֧��û��CAN ����Ŀ
* 2 ����Դ�͹���ʱ���Ϊ2��
* 1�� PM_LIGN_ERR_TIME �͵�ѹʱ����ʱʱ��
* 2�� PM_HIGN_ERR_TIME �ߵ�ѹʱ����ʱʱ��
* 3�� ����ApplPM_SleepEnter��ApplPM_SleepReady�ص�����
* ���ߣ�xb
* ������ڣ�2019��3��30��
* 
* �汾��1.2
* ˵����
* 1) ���ӱ�ע˵��
* ���ߣ�xb
* ������ڣ�2019��10��21��
* 
* �汾��1.3
* ˵����
* 1) ͨ��ApplPM_StateChange ��֪ͨ�û���������ȥ
* ���ߣ�xb
* ������ڣ�2019��12��11��

* �汾��1.4
* ˵����
* 1) ֧��autosar NM
* 2) ����_PM_AUTOSARNM_ �궨��
* ���ߣ�xb
* ������ڣ�2019��12��11��
* 
* �汾��1.5
* ˵����
* 1) ��Դ�����ʼ��ʱ��Ҫ����CANWake_Begin
* 2) 
* ���ߣ�xb
* ������ڣ�2020��3��7��
* 
* �汾��1.6
* ˵����
* 1) ��Դ������standby�ɷ��ص�off
* ���ߣ�xb
* ������ڣ�2020��5��30��
*/

#include "LIBDriveDefine.h"
#include "PowerMange.h"

#if _PM_USECAN_
#include "CanDrive.h"
#if _PM_OSKNM_
	#include "OsekNm.h"
#endif
#if _PM_AUTOSARNM_
	#include "PMAssist_AutoSar.h"
#endif
#if _PM_INDIRECTNM_
	#include "IndirectNM.h"
#endif
#endif


static PMStateType u8PowerState;
static uint16 TimerIgnErr, TimerIgnOK;
static uint8 bIgnWorkState, bIgnWorkStateLast;
static uint8 bCANVoltState;
static uint8 u8AppSleepEn = 1;  //Ĭ�Ͽ���ֱ������
static uint16 TimerPowerState;
static uint8 u8CANSleepEn;


//�ⲿ�ӿں���
extern void  DealForWakeUp(void);
extern void  DealForSleep(void);
//��Դ����״̬ת���궨��
#define  PMStateChange(state)  ApplPM_StateChange(u8PowerState, state); \
											u8PowerState = state
/*******************************************************************************
 * Function:       void PowerMange(void)
 *
 * Description:    ��Դ�������س���
 *
 * Returns:         none
 *
 * Notes:           ������ѭ��
 *
********************************************************************************/
void PowerMange(void)
{
	int16 i16IgnVolt;
	static uint8 bIgnErr = PM_VOLT_OK;
	i16IgnVolt = ADPhy_GetFact(ADID_VOLT);
	uint8 u8IgnOkTimes;
	uint16 u16IgnErrTime;
	//CAN ����״̬����
#if _PM_USECAN_
	Deal_CANWake();
#endif
	if (i16IgnVolt > IGNVOLT_WORK_ERRU || i16IgnVolt < IGNVOLT_WORK_ERRD)
	{
	   bIgnErr = PM_VOLT_ERR;
	}
	if (i16IgnVolt < IGNVOLT_WORK_OKU && i16IgnVolt > IGNVOLT_WORK_OKD)
	{
		bIgnErr = PM_VOLT_OK;
	}
	bIgnErr = PM_VOLT_OK;
   //300ms  ��ʱ�ػ��߼��� 
   if (bIgnErr == PM_VOLT_OK)
   { 
   	(void)TimeOutChkMsLong(&TimerIgnErr, 0);
      if (TimeOutChkMsLong(&TimerIgnOK, PM_IGN_OK_TIME))
      {
      	bIgnWorkState = PM_VOLT_OK;
      }
      
      bIgnWorkStateLast = PM_VOLT_OK;
   }
   else
   {
      if (i16IgnVolt > IGNVOLT_WORK_OKU)
      {//��ѹ��ʱ��	
      	u16IgnErrTime = PM_HIGN_ERR_TIME;
      }
      else
      {//��ѹ��ʱ��
      	u16IgnErrTime = PM_LIGN_ERR_TIME;
      }

   	if (TimeOutChkMsLong(&TimerIgnErr, u16IgnErrTime))
      {
      	bIgnWorkState = PM_VOLT_ERR;
      }
   	else{}
   	
      (void)TimeOutChkMsLong(&TimerIgnOK, 0);
      
      if (bIgnWorkStateLast != PM_VOLT_ERR)
      {
      	bIgnWorkStateLast = PM_VOLT_ERR;
      }
   }
#if _BATT_COLLECT_ 
	int16 i16BattVolt;
	i16BattVolt = Get_Ad_Fact(ADID_VOLT);
   if(i16BattVolt>CANVOLT_HIGHERR ||i16BattVolt<CANVOLT_LOWERR)//����״̬ʱ��ʱ���岻��CAN
   {
   	bCANVoltState=0;
   }
   if(i16BattVolt < CANVOLT_HIGHOK && i16BattVolt > CANVOLT_LOWOK)
   {
   	bCANVoltState=1;
   }
#else
   bCANVoltState = 1;
#endif
   
   switch (u8PowerState)
   {
	default:
   	case POWER_STATE_INIT:
   		if (TimeOutChkMsLong(&TimerPowerState, 500))
   		{
				#if _PM_USECAN_
   				CANWake_Begin(1);
				#endif
   			if (bIgnWorkState == PM_VOLT_OK)
				{
					PMStateChange(POWER_STATE_WORK);
				}
   			else
   			{ 
					PMStateChange(POWER_STATE_OFF);
   			}
   		}
   		break;
   	case  POWER_STATE_WORK:
#if  _OPENSDA_CAN
#else
   		if (bIgnWorkState == PM_VOLT_ERR)
   		{
				PMStateChange(POWER_STATE_OFF);
   		}else{}
#endif
   		break;
   	case  POWER_STATE_OFF:
   		if (bIgnWorkState == PM_VOLT_OK)
   		{
   			PMStateChange(POWER_STATE_WORK);
   		}
   		else
   		{
   			
   		}
   		
   		if (u8AppSleepEn)
   		{//Ӧ�������,����˯��
   			if (i16IgnVolt < IGN_VOLT_SLEEP)
   			{
#if   _PM_OSKNM_
   				CanNm_GotoMode(0, GOTO_BUSSLEEP);
#elif _PM_AUTOSARNM_
   				CanNm_NetworkRelease(0);
#endif   	
   				PMStateChange(POWER_STATE_STANDBY);
   			}
   			else{}
   		}
   		else{}
   		break;
   	case POWER_STATE_STANDBY:
   		if (bIgnWorkState == PM_VOLT_OK)
   		{
#if _PM_OSKNM_
   			CanNm_GotoMode(0, GOTO_AWAKE);
#elif _PM_AUTOSARNM_
   			CanNm_NetworkRequest(0);	
#endif
   			u8CANSleepEn = 0;  //Ӧ�õȴ�����ָʾλ
   			PMStateChange(POWER_STATE_WORK);
   		}
   		else
   		{
   			
   		}

   		if (u8AppSleepEn == 0)
   		{//Ӧ���л�������,yanz
#if  _PM_AUTOSARNM_
   			CanNm_NetworkRequest(0);	
#endif
   			u8CANSleepEn = 0;  //Ӧ�õȴ�����ָʾλ
   			PMStateChange(POWER_STATE_OFF);
   		}
   		else
   		{
   			
   		}

#if (_PM_OSKNM_	|| _PM_AUTOSARNM_)
   		if (u8CANSleepEn)
   		{
   			PMStateChange(POWER_STATE_STANDBY_WAIT);
   		   (void)TimeOutChkMsLong(&TimerPowerState, 0);
   			//��������ģʽ�Ĵ���
   			CANWake_Begin(0);
   			CANDrive_GotoSleep();   //candrive �л�Ϊsleepģʽ
   		}
   		else
   		{//һֱ�ȴ�˯��ָʾλ�����ߵ����������������
   			
   		}
#elif   _PM_USECAN_
//û��osekʱ�����߱��λ���ޣ���CAN �ź���ȷ��
   		if (GetCanWakeEn() == 0)
   		{//û�б���
   			PMStateChange(POWER_STATE_STANDBY_WAIT);
   		   (void)TimeOutChkMsLong(&TimerPowerState, 0);
   			CANWake_Begin(0);
   			CANDrive_GotoSleep();
   		}
#else
			PMStateChange(POWER_STATE_STANDBY_WAIT);
			(void)TimeOutChkMsLong(&TimerPowerState, 0);
#endif
   		break;
   	case POWER_STATE_STANDBY_WAIT:
   		if (TimeOutChkMsLong(&TimerPowerState, 100))
   		{
#if _PM_USECAN_
   			CANWake_Begin(1);
#endif
   			PMStateChange(POWER_STATE_SLEEP);
   			//ֱ�ӽ���sleep loop
   		}
   		else
   		{
   			break;
   		}
   	case POWER_STATE_SLEEP:
   		u8IgnOkTimes = 0;
   		do{
      		//(void)DisableInterrupt();
   			
   			DealForSleep();  //�������� ��Ӳ��������ʵ��
#if _PM_USECAN_		
   			Deal_CANWake();
      		if (GetCanWakeEn())
				{//can����
      			break;
				}
      		else
      		{

      		}
#endif
   			if(ReadIgn_Only(ADID_VOLT) > IGN_VOLT_WAKEUP)
   			{//ֻ��ȡ���ad ��������������ߵ�ѹ����1
   				u8IgnOkTimes++;
   			}
   			else
   			{
   				u8IgnOkTimes=0;
   			}     //����5���ź���Ч���ָܻ����ʱ��Ϊ64*5=320ms
   		}while(u8IgnOkTimes<1);
#if _PM_USECAN_
   		if (GetCanWakeEn())
   		{//�����CAN ����
#if   _POWERM_CANWAKE_RESET_
				DealForWakeUp();    //�˴��ϵ�ֱ�Ӹ�λ������Ӧ��ʱ��
#else
				PMStateChange(POWER_STATE_INIT);
				CANDrive_GotoAWake();
				PowerNM_Init();
				(void)TimeOutChkMsLong(&TimerPowerState, 0);
#endif
   		}
   		else
#endif
   		{//�����  			
   			DealForWakeUp();
   		}
   		break;
   }
   
}

/*******************************************************************************
 * Function:      uint8 PowerMN_GetPowerState(void)
 *
 * Description:    ���ص�Դ�����״̬
 *
 * Returns:         none
 *
 * Notes:           app����
 *
********************************************************************************/
uint8 PowerMN_GetPowerState(void)
{
	return u8PowerState;
}
/*******************************************************************************
 * Function:      uint8 PowerMN_GetPowerState(void)
 *
 * Description:    ���ص�Դ״̬,�����˲���ʱ֮��Ľ��
 *
 * Returns:         none
 *
 * Notes:           app����Ҫʱ����
 *
********************************************************************************/
uint8 PowerMN_GetIgnState(void)
{
	return bIgnWorkState;
}

/*******************************************************************************
 * Function:       void PowerNM_Init(void)
 *
 * Description:    ��Դ�����ʼ��
 *
 * Returns:         none
 *
 * Notes:           app����
 *
********************************************************************************/
void PowerNM_Init(void)
{
	PMStateChange(POWER_STATE_INIT);
#if _PM_USECAN_
	CANDrive_GotoAWake();
#endif
#if _INDIRECT_NM_
	IndirectNM_Init();
#endif
#if _PM_OSKNM_
	CanNm_Init();
	CanNm_GotoMode(0, GOTO_AWAKE);
#endif
#if _PM_AUTOSARNM_
	PMAssistAutosar_InitNM();
#endif
	u8CANSleepEn = 0;
}

/*******************************************************************************
 * Function:       void PowerMN_AppSleepEn(uint8 bEn)
 *
 * Description:    ����app �Ƿ��Ѿ���������
 *
 * Returns:         none
 *
 * Notes:           app����
 *
********************************************************************************/
void PowerMN_AppSleepEn(uint8 bEn)
{
	u8AppSleepEn = bEn;	
}
/*******************************************************************************
 * Function:      void PowerMN_CanSleepEn(uint8 bEn)
 *
 * Description:    ����CAN�Ƿ��Ѿ���������
 *
 * Returns:         none
 *
 * Notes:           app����
 *
********************************************************************************/
void PowerMN_CanSleepEn(uint8 bEn)
{
	u8CANSleepEn = bEn;	
}
