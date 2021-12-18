/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�main.c
* ������������������ڣ���main����������ϵͳ�ܹ�
*
* �������� codewarrior IDE 11.1
* MCU˵������ģ��ΪMAIN-LOOP�ϲ�Ӧ�ã�
*        ���ļ����ܸ���
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
* 
* �汾��2.0
* ˵��������standby task
* ԭ���ߣ�xb
* ������ڣ�2020��5��30��
*/
#include "APPDrive.h"
#include "PowerMange.h"


/*************************************************************************************************
�ⲿ��������
*********************************************************************************************/
extern void Init_LowerDrive(void);
extern void LowDrive_Loop(void);
extern void LowDrive_PowerON(void);
//app �ӿ�
extern void Init_System(void);
extern void SystemApp_Loop(void);
extern void SystemApp_PowerON(void);
extern void PowerOffTask(void);
extern void PowerStandByTask(void);
/*******************************************************************************
 * Function:        void main()
 *
 * Description:    ���������
 *
 * Returns:         none
 *
 * Notes:           none
 *
********************************************************************************/

int main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/
	(void)DisableInterrupt();
	//int counter = 0;
	Init_MCU();
		
	Init_LowerDrive();   //�˴�Ҫ�ȷţ���Ϊ�˴��г�ʼ��FASH
	
	Init_System();   //ϵͳ��������ã���Ҫ��ģ���������á�Ad�������
		
   PowerNM_Init();


	(void)EnableInterrupt();

	for(;;)
	{
		//counter++;
		Clear_WatchDog(); 
		
		LowDrive_Loop(); 
		
		SystemApp_Loop();
		
		PowerMange();           //��Դ���������ǰ��Դ��״̬��
	
		//����ϵͳ�ײ�����
		//����Ϊ��Դ����ʱ��������
		if (PowerMN_GetPowerState() == POWER_STATE_WORK)     //ACC��ѹ����
		{
			//���� 1621 ���� ����1621 ������
			LowDrive_PowerON();
			//����ϵͳ״̬���������������
			SystemApp_PowerON();
		}
		else
		{//����ѹ����
			if (PowerMN_GetPowerState() == POWER_STATE_OFF)
			{
				PowerOffTask();
			}
			else if (PowerMN_GetPowerState() == POWER_STATE_STANDBY)
			{
				PowerStandByTask();
			}
			else
			{//������״̬ʱ��POWER_STATE_INIT��POWER_STATE_SLEEP��POWER_STATE_STANDBY_WAIT���������κ�������powermn������
				
			}		
		}
	}
	return 0;
}


