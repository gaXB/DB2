/************************�ļ�˵��*****************************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SystemApp.c
* ���������� Ӧ��ϵͳ�ӿ�
* 1 �ṩ�ⲿ�ӿں���
*  Init_System
*  SystemApp_Loop
*  SystemApp_PowerON
* 2 ϵͳ״̬�л��ӿ�
* System_GOTODiagTask 
* System_GOTOInitTask
* 3 
* Ӧ�ò���ƣ� �ṩģ�� ������Ĵ���˵���ļ�
* �������� codewarrior IDE 11.1
* MCU˵������MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��13��
* 
* �汾��2.0
* ˵���� ����_PROJECT_LDO_MODE
* ���ݲ�ͬLDO ��ģʽ��ȷ����ʼģʽ
* 
* �汾��3.0
* ˵���� ���书�ܷ���LOOPѭ��������
* 
* �汾��3.1
* ˵��������ö�����͵ĸ�ֵ
* ������ڣ�2021��8��23��
*/

#include "HVACMainControl.h"
#include "APPDrive.h"

/*******************************************************************************
 * ��������
 *
*******************************************************************************/
uint8	SystemState;

/*******************************************************************************
 * ϵͳ�������� ����
 *
********************************************************************************/
#define   SYSTEM_STATE_INIT     0        //����ʱ��ʼ������
#define   SYSTEM_STATE_NORMALWORK   1    //��������
#define   SYSTEM_STATE_DIAG   2          //�Լ칤��
/*************************************************************************************************
�ⲿ��������
*********************************************************************************************/
extern void  Enter_InitTask(uint8 InitType);
extern void  DiagStepSet(uint8 step);
extern uint8 Run_Init_Task(void);
extern uint8 Run_Diag_Task(void);

//��ģ��ʵ�ֺ���
void System_GOTOInitTask(void);

/*******************************************************************************
 * Function:    void System_Default(void)
 *
 * Description: ϵͳ����Ĭ��ģʽ
 *
 * Returns:     none
 *
 * Notes:       
*******************************************************************************/
static void System_Default(void)
{
	//TODO: ��дĬ��ģʽ��ֵ
	SystemControl.tset = _TSET_DEFAULT_;       //ϵͳ״̬��ʼ��
	SystemControl.OffState = OFF_STATE_OFF; 
	SystemControl.AutoBits.AC = 0;
	SystemControl.AutoBits.FAN = 0;
	SystemControl.AutoBits.MODE = 0;
	SystemControl.AutoBits.NCF = 0;
				
	SystemControl.acMode = AC_MODE_OFF;
	SystemControl.fanMode = FAN_MODE_1L;
	SystemControl.mdMode = MD_MODE_OSF;
	SystemControl.ncfMode = NCF_MODE_NEW;
	SystemControl.RdefMode = RDEF_MODE_OFF;
}


/*******************************************************************************
 * Function:    Init_Parameters
 *
 * Description:  ʵ�������һ���ϵ���Ĭ��ֵ��֮����Ҫ�ü���ֵ�����ر������������ʾ
 *
 * Returns:     none
 *
 * Notes:       ������ʼ����
*******************************************************************************/
static void Init_Parameters(void)
{
   //��ȡEE
#if  _PROJECT_LDO_MODE == LDO_MODE_BATTIGN
	if (MCUPM_IsLPowerRest() == FALSE)   
	{//BATT��IGN�ϵ�Ĵ���
		if (Load_Ign())
		{
			
		}  
		else
		{//�������ʹ��Ĭ��ֵ
			System_Default();
		}
		//��ʱssensordata�ܱ���
	}
	else
	{
    	//ȫ������ʱϵͳΪ��ʼ��״̬
		//TODO: ���ݵ���ģʽȷ��
		System_Default();
		sSenSorData.TambCal = _TAMB_DEFAULT_;
		sSenSorData.TambCal_TD = _TAMB_DEFAULT_;
		sSenSorData.TwaterCal = _TWATER_DEFAULT_;
		sSenSorData.TinSimul = _TIN_DEFAULT_;
		sSenSorData.TinCal = _TIN_DEFAULT_;
	}
#else
	//����ģʽ�����ж��Ƿ�������
	if (Load_Ign())
	{
		
	}  
	else
	{//�������ʹ��Ĭ��ֵ
		System_Default();
	}
	
	sSenSorData.TambCal = _TAMB_DEFAULT_;
	sSenSorData.TambCal_TD = _TAMB_DEFAULT_;
	sSenSorData.TwaterCal = _TWATER_DEFAULT_;
	sSenSorData.TinSimul = _TIN_DEFAULT_;
	sSenSorData.TinCal = _TIN_DEFAULT_;
#endif

	if (SystemControl.tset > TSET_HI || SystemControl.tset < TSET_LO) 
	{
		SystemControl.tset = _TSET_DEFAULT_;
	}
	else{}
	
	if (SystemControl.fanMode == 0)
	{  
		SystemControl.fanMode = 1;
	}
	else{}
	
	SystemControl.RdefMode = RDEF_MODE_OFF;
	OtherControl.CHeatMode_L = 0;
	OtherControl.CHeatMode_R = 0;
	
	sDisplayData.AC = 0;
	sDisplayData.AUTO = 0;
	sDisplayData.CIRF = 0;
	sDisplayData.FANLevel = 0;
	sDisplayData.LCDDiagMode = 0;
	sDisplayData.LCDMode = 0;
	sDisplayData.LCheat = 0;
	sDisplayData.LEDMode = 0;
	sDisplayData.MAXAC = 0;
	sDisplayData.MODE_MAXDEF = 0;
	sDisplayData.NEW = 0;
	sDisplayData.RHEAT = 0;
	sDisplayData.RCheat = 0;
	sDisplayData.VentMode = MD_MODE_OSF;
	sDisplayData.i16Temp = 0;
	
	sOutData.ACOut = 0;
	sOutData.FANVolt = 0;
	sOutData.MIXAD = 255;
	sOutData.MODEAD = 255;
	sOutData.NCFAD = 255;
	sOutData.RHeatOut = 0;
	sOutData.u8LCheat = 0;
	sOutData.u8RCheat = 0;
}

/*******************************************************************************
 * Function:    Init_System
 *
 * Description: ϵͳ��ʼ�� main-loop ����
 *
 * Returns:     none
 *
 * Notes:      ���ø���
 *           
*******************************************************************************/
void Init_System(void)
{
	Init_Parameters();
	//����ȫ����ʼ������
	System_GOTOInitTask();
}

/*******************************************************************************
 * Function:    void System_Loop(void)
 *
 * Description: Ӧ��LOOP ����
 *
 * Returns:     none
 *
 * Notes:      ��Դ����ʱҲ������
 *            
*******************************************************************************/
void SystemApp_Loop(void)
{
	if (SystemState != SYSTEM_STATE_DIAG)
	{//ѭ�����䣬���ڴ˴�������ر�֮����Ҫ���������SystemApp_PowerON
		MemoryControl();
	}else{}
	
}

/*******************************************************************************
 * Function:    void SystemState_InitToNormal(void)
 *
 * Description: �ص�������������ʼ������ʱ������һ��
 *
 * Returns:     none
 *
 * Notes:      �ڴ˱�д������ʼ��ʱ��Ҫ���������
 *
*******************************************************************************/
static void SystemState_InitToNormal(void)
{	
#if _PROJECT_AUTO_
	//�Զ����Ƴ�ʼ��
	AUTOTD_Init();   	
#endif 	
	HVACControl_Init();
}

/*******************************************************************************
 * Function:    void SystemApp_PowerON(void)
 *
 * Description: Ӧ�õ�Դ��������
 *
 * Returns:     none
 *
 * Notes:       ʵ�ֳ�ʼ�����������Լ�3�ֹ���ģʽ�������⹦�������ø���
 *            
*******************************************************************************/
void SystemApp_PowerON(void)
{
	switch(SystemState) 
	{
		case  SYSTEM_STATE_INIT:
			if(Run_Init_Task()) //��ϵͳΪ��ʼ��ϵͳ2s
			{// �Ƿ���Ҫ�˳���ʼ��
				SystemState = SYSTEM_STATE_NORMALWORK;
				//lcd ģ����������
				SystemState_InitToNormal();
			}  
			break;
			
		case  SYSTEM_STATE_NORMALWORK:   
			Run_Normal_Task();  
			break;
	
		case  SYSTEM_STATE_DIAG:		
			
			if(Run_Diag_Task())
			{  //�Լ�ģʽ�˳�����
				SystemState =SYSTEM_STATE_NORMALWORK;
				SystemControl.tset = _TSET_DEFAULT_;
				SystemControl.AutoBits.AC  = 1;
				SystemControl.AutoBits.NCF  = 1;
				SystemControl.AutoBits.FAN = 1;
				SystemControl.AutoBits.MODE = 1;
			}
			//����Ϊ��Ҫ��ͨ�ã����Ʊ����Ŀ��ƣ��粻��Ҫ��ɾ��
			break;		
		default:
			SystemState = SYSTEM_STATE_INIT;
			Enter_InitTask(0);
			break;
	}
} 
/*******************************************************************************
 * Function:   void System_GOTOInitTask(void)
 * 
 * Description: ��������ʼ������
 *
 * Returns:     none
 *
 * Notes:      �����ʼ��ʱ����            
 *                         
 *                          
*******************************************************************************/
void System_GOTOInitTask(void)
{
   Enter_InitTask(0);  //�����ʼ������
   SystemState = SYSTEM_STATE_INIT;
   //TODO: ��д�����ʼ��ʱ����Ҫ�ı��״̬
}

/*******************************************************************************
 * Function:   void System_GOTODiagTask(void)
 * 
 * Description: �����Լ�״̬
 *
 * Returns:     none
 *
 * Notes:                  
 *                         
 *                          
*******************************************************************************/
void System_GOTODiagTask(void)
{
	SystemState = SYSTEM_STATE_DIAG;
	DiagStepSet(0);
}
