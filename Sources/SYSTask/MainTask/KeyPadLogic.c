/**************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�KeyPadLogic.c
* ����������ʵ�ְ����������ť������
* 1 �ı� SystemControl 
* 
* �������� codewarrior IDE 11.1
* MCU˵���� ��MCU�޹أ��������ø���
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
* 
* 
* * �汾��1.1
* ˵����led ��ʾ�汾�ŵ��÷�ʽ���
* ����:lyc
* ������ڣ�2020��5��29��
*
*/
#include "APPDrive.h"
#include "KeyPadLogic.h"
#include "SystemSave.h"

//�ⲿ��������
extern void System_GOTODiagTask(void);
/*******************************************************************************
 * Function:   void  Deal_Keypad_Code (void)
 *
 * Description: �������������ݶ�ȡ������
 *
 * Returns:     none
 *
 * Notes:  ������ģ������л����ܡ�
 *         ���ݰ������ͻ��ṩ���߼�����, �ı�ϵͳ��Ҫ���Ʊ��� Systemcontrol
 *           
*******************************************************************************/
void Deal_Keypad_Normal(void)
{
	uint16 keywd;
	uint8 u8LevelMode;
	uint8 u8LevelSet;
	//uint8 u8FanLevel = sDisplayData.FANLevel;
	//TODO ���ձ�������ֵ�������ã���ο�ģ��
	uint8 u8FanLevel = sDisplayData.FANLevel;
	
	keywd = ReadComd();
	
	switch((uint8)keywd)
	{
		default:
			break;
		case COMD_TSET_ADD:
			if(!SystemControl.OffState)
			{
				u8LevelSet = SystemControl.u8TestLevel + 1;
				u8LevelMode = GetTSETLevelMode(u8LevelSet);
				if (u8LevelSet > TSETTEM_MODE_DEBUG)
				{
					SystemControl.u8TestLevel = TSETTEM_MODE_DEBUG;
				}
				else if (u8LevelMode == SystemControl.TestMode)
				{//ͬһģʽ���л�������
					SystemControl.u8TestLevel = u8LevelSet;
				}
				else if (sVehicleControl.bBattCoolReq || sVehicleControl.bBattHotReq)
				{//�����л�

				}
				else
				{
					SystemControl.u8TestLevel = u8LevelSet;
				}
			}
			break;
		case COMD_TSET_DEC:
			if(!SystemControl.OffState)
			{
				if (SystemControl.u8TestLevel > 0)
				{
					u8LevelSet = SystemControl.u8TestLevel - 1;
				}
				else
				{
					u8LevelSet = 0;
				}

				u8LevelMode = GetTSETLevelMode(u8LevelSet);
				if (u8LevelSet < TSETEM_MODE_JIAZHU)
				{
					SystemControl.u8TestLevel = TSETTEM_MODE_DEBUG;
				}
				else if (u8LevelMode == SystemControl.TestMode)
				{//ͬһģʽ���л�������
					SystemControl.u8TestLevel = u8LevelSet;
				}
				else if (sVehicleControl.bBattCoolReq || sVehicleControl.bBattHotReq)
				{//�����л�

				}
				else
				{
					SystemControl.u8TestLevel = u8LevelSet;
				}
			}
			break;
		case COMD_FAN_ADD:
			  if(!SystemControl.OffState)
			  {
				 u8FanLevel++;
				 if(u8FanLevel>MAX_FELVL) u8FanLevel= MAX_FELVL;
				 SystemControl.fanMode =  u8FanLevel;
			  }
			  else
			  {
		   //      Load_Off();
				 SystemControl.OffState = 0;
				 if(SystemControl.mdMode == MD_MODE_OST)
				 {//�˳�offʱ��û��def״̬
					Load_Def();
				 }
				// SystemControl.fanMode = FAN_MODE_1L;
			  }
			  SystemControl.AutoBits.FAN =0;
			  (void)FanControl_SetRate(FANVOLT_RATE_MAN);
			  Def_Operat(DEF_OPTYPE_FAN);
			  break;
		case  COMD_FAN_DEC:
			  if(!SystemControl.OffState)
			  {
				 if(u8FanLevel > MIN_FELVL )
				 {
					u8FanLevel--;
					SystemControl.fanMode = u8FanLevel;
				 }
				 else
				 {    //�ػ�
				  //  Save_Off();
					 SystemControl.fanMode = MIN_FELVL;
				 //   SystemControl.OffState = 1;
				 }
			  }
			  else
			  {
				 SystemControl.OffState = 0;
				 if(SystemControl.mdMode == MD_MODE_OST)
				 {//�˳�offʱ��û��def״̬
					Load_Def();
				 }

				 //SystemControl.fanMode = MIN_FELVL;
				// SystemControl.fanMode = FAN_MODE_1L;
			  }
			  (void)FanControl_SetRate(FANVOLT_RATE_MAN);
			  SystemControl.AutoBits.FAN =0;
			  Def_Operat(DEF_OPTYPE_FAN);
			  break;
	}
   
	//TODO:��ȡ����,������ֵ����״̬�л�����ο�ģ��
	keywd = Read_Keydate();
	if(!keywd)
	{ 
		return;
	}else{}
	switch(keywd)
	{
		default:
			break;
		//*********************��ʾ******************	     
	#if  _PROJECT_LCD_
		case  KEY_AC + 0xff:
		  //��ʾ ���� ʱ��
			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
			{
				DisTsetShakeSet(sSenSorData.TevpCal) ;	
			}
			break;
		case  KEY_DIAG:
			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
			{
				System_GOTODiagTask();
			}
			break;
	#endif
		case  KEY_VER_SLEF_LCD:
			//ģ�岻��ʾ�ͻ��汾�����尴������Ŀ�ͻ�����
			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
			{
#if  _PROJECT_LCD_
				//Һ������ʾ�ڲ��汾����λ���ͻ����Ĵ�����
				DisTsetShakeSet(SwsV_*10);
#else
#endif			
			}
			break;
		case  KEY_VER_SLEF:
			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
			{
#if  _PROJECT_LCD_
				//Һ������ʾ�ڲ��汾����λ���ڲ����Ĵ�����
				DisTsetShakeSet(SwV_*10 + SwsV_%10);	
#endif
				//AC��ѭ������  һ������ʾ�����ڲ��汾��
				LedDisVerEn(SwV_*100+SwsV_);
			}
			break;
		case KEY_CLIENT_VER:
			//�ͻ��汾����ʾ
			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
			{
#if  _PROJECT_LCD_
				//Һ������ʾ�ڲ��汾����λ���ڲ����Ĵ�����
				DisTsetShakeSet(SwV_CLIENT*10 + SwsV_CLIENT%10);	
#endif
				//AC��ѭ������  һ������ʾ�����ڲ��汾��
				LedDisVerEn(SwV_CLIENT*100+SwsV_CLIENT);
			}
			break;
	}
}

/*******************************************************************************
 * Function:   void KeyLogic_Respone(uint8 u8KeyIMDType, int16 i16Data)
 *
 * Description: Deal_Keypad_Code �Ƿ��뻺��İ������ж�ȡ���˹���Ϊ�ⲿ���ź�ʱֱ������ʱʹ��
 *
 * Returns:     none
 *
 * Notes:      
 *           
*******************************************************************************/
void KeyLogic_Respone(uint8 u8KeyIMDType, int16 i16Data)
{
   //TODO: ����ʵ��Ӧ�������д,�ɲο�ģ��
	if (u8KeyIMDType == IMKEY_FAN)
	{
		if (i16Data < 8)
		{
			SystemControl.fanMode = (uint8)i16Data;
		}
	}
	else
	{}
}

/*******************************************************************************
 * Function:   void  PanleState_Logic (void)
 *
 * Description: ��ť�����������ȿ���״̬��ʵ��
 *
 * Returns:     none
 *
 * Notes:  ��ʼ������ʱ�����ȵ���
 *           
*******************************************************************************/
void PanleState_Logic(void)
{
	
}
