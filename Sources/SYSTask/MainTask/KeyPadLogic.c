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
static uint8 bEnDebug;

void  TestMode_restrict(void)
{
	LevelModeType u8LevelMode;
	u8LevelMode = GetTSETLevelMode(SystemControl.u8TestLevel);

	switch(u8LevelMode)
	{
		default:
			break;
		case LEVELMODE_COOL:
			SystemControl.ptcMode = 0;
			break;
		case LEVELMODE_COOLDEF:
			SystemControl.ptcMode = 0;
			if (SystemControl.fanMode >= 4)
			{
				SystemControl.fanMode = 4;
			}else{}
			break;
		case LEVELMODE_HOTDEF:
			if (SystemControl.fanMode >= 4)
			{
				SystemControl.fanMode = 4;
			}else{}
			break;
		case LEVELMODE_HPMODE:
			SystemControl.ptcMode = 0;
			break;
		case LEVELMODE_WPTC:
			SystemControl.acMode = 0;
			break;

	}
}

void DealTestSwitch(void)
{
	if (SystemControl.u8TestLevel == TSETTEM_MODE_HOTDEF_1)
	{
		SystemControl.mdMode = MD_MODE_OSF;
	}
	else 	if (SystemControl.u8TestLevel == TSETTEM_MODE_HOTDEF_2)
	{
		SystemControl.mdMode = MD_MODE_OSFD;
	}
	else 	if (SystemControl.u8TestLevel == TSETTEM_MODE_HOTDEF_3)
	{
		SystemControl.mdMode = MD_MODE_OSD;
	}
}

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
uint8 GetCurrentKey(void);
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
				u8LevelMode = GetTSETLevelMode(u8LevelSet);
				if (u8LevelSet > TSETTEM_MODE_DEBUG)
				{
					//SystemControl.u8TestLevel = TSETTEM_MODE_COOL_1;
				}
				else if (sVehicleControl.bBattCoolReq || sVehicleControl.bBattHotReq)
				{//���������ʱ
				   if (u8LevelMode == SystemControl.LevelMode)
					{//ͬһģʽ���л�������
						SystemControl.u8TestLevel = u8LevelSet;
					}else{}
				}
				else if (u8LevelMode == LEVELMODE_DEBUG)
				{//�������ģʽ
					if (bEnDebug)
					{
						SystemControl.u8TestLevel = u8LevelSet;
					}else{}
				}
				else
				{
					SystemControl.u8TestLevel = u8LevelSet;
				}
				DealTestSwitch();
			}
			break;
		case COMD_TSET_DEC:
			if(!SystemControl.OffState)
			{
				if (SystemControl.u8TestLevel > 1)
				{
					u8LevelSet = SystemControl.u8TestLevel - 1;
				}
				else
				{
					u8LevelSet = 1;
				}

				u8LevelMode = GetTSETLevelMode(u8LevelSet);
				if (u8LevelSet < TSETEM_MODE_JIAZHU)
				{
					SystemControl.u8TestLevel = TSETTEM_MODE_COOL_1;
				}
				else if (sVehicleControl.bBattCoolReq || sVehicleControl.bBattHotReq)
				{//���������ʱ
				   if (u8LevelMode == SystemControl.LevelMode)
					{//ͬһģʽ���л�������
						SystemControl.u8TestLevel = u8LevelSet;
					}else{}
				}
				else if (u8LevelMode == LEVELMODE_DEBUG)
				{//�������ģʽ
					if (bEnDebug)
					{
						SystemControl.u8TestLevel = u8LevelSet;
					}else{}
				}
				else
				{
					SystemControl.u8TestLevel = u8LevelSet;
				}
				DealTestSwitch();
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
	TestMode_restrict();
	extern uint16 KeyPadHW_ReadSignal(void);

	static uint16 TimerDebugKey;
	//keywd = GetCurrentKey();
#if 0//PCDEBUG_CONFIG > 1
	bEnDebug = 1;
#else
	if (sSenSorData.u8VehicleSpeed >= 5)
	{
		if (bEnDebug == 1)
		{
			bEnDebug = 0;
		}else{}

		if  (SystemControl.u8TestLevel != 1 && SystemControl.u8TestLevel != 2 && SystemControl.u8TestLevel != 23)
		{

		}
		else
		{
			SystemControl.u8TestLevel = TSETTEM_MODE_COOL_1;
		}
	}
	else{}


	if  (SystemControl.u8TestLevel != 1 && SystemControl.u8TestLevel != 2 && SystemControl.u8TestLevel != 23)
	{
		if (TimeOutChkTenthSLong(&TimerDebugKey, 20))
		{
			if (bEnDebug == 1)
			{
			//	SystemControl.u8TestLevel = TSETTEM_MODE_COOL_1;
				bEnDebug = 0;
			}else{}
		}
	}
	else{}

#endif

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
			//��Դ����
		case KEY_OFF	:
			if(!SystemControl.OffState)
			{//����off
				Save_Off();
				SystemControl.OffState = 1;
			}
		 break;
		case KEY_ON	:
			if(SystemControl.OffState == OFF_STATE_OFF)
			{//����off
				Load_Off();
				SystemControl.OffState = 0;
			}
		 break;
		case 	KEY_DEBUG:
			bEnDebug = 1;
			(void)TimeOutChkTenthSLong(&TimerDebugKey, 0);
			break;
		case 	KEY_RHEAT:
			if (SystemControl.RdefMode == 1)
			{
				SystemControl.RdefMode = 0;
			}
			else
			{
				SystemControl.RdefMode = 1;
			}
			break;
		//****************************************
		//rec/fre
		case 	KEY_NCF:
			if (SystemControl.ncfMode==NCF_MODE_NEW)
			{
				SystemControl.ncfMode = NCF_MODE_CIRF;
			}
			else
			{
				SystemControl.ncfMode = NCF_MODE_NEW;
			}
			SystemControl.AutoBits.NCF =0;
			SystemControl.MaxACMode = AC_MAX_MODE_OFF ; //Acmax �رգ�
			Def_Operat(DEF_OPTYPE_NCF);
			break;
		case KEY_PTC	:
		  if(!SystemControl.OffState)
		  {
			  if(SystemControl.ptcMode == 1)
			  {
					SystemControl.ptcMode = 0;
			  }
			  else
			  {
					SystemControl.ptcMode = 1;
			  }
		  }
		  else
		  {
			  //ac ����������
			  Load_Off();
			  SystemControl.ptcMode = 1;
			}
			break;
		case KEY_AC	:
		  if(!SystemControl.OffState)
		  {
			  if(SystemControl.acMode == AC_MODE_ON)
			  {
					SystemControl.acMode = AC_MODE_OFF;
			  }
			  else
			  {
					SystemControl.acMode = AC_MODE_ON;
			  }
			  SystemControl.AutoBits.AC = 0;
		  }
		  else
		  {
			  //ac ����������
			  Load_Off();
			  SystemControl.AutoBits.AC = 0;
			  SystemControl.acMode = AC_MODE_ON;
			}
			Def_Operat(DEF_OPTYPE_AC);
			//SystemControl.MaxACMode = AC_MAX_MODE_OFF ; //Acmax �رգ�
			break;
		case KEY_MODE:
			if(SystemControl.OffState )
			{
				  //offʱ�л�ģʽ
				  //offʱ��˪״̬������
				Load_Off();
				SystemControl.AutoBits.FAN =0;
				SystemControl.fanMode = FAN_MODE_1L;

				SystemControl.mdMode++;
				if (SystemControl.mdMode >= MD_MODE_OST)
				{
					SystemControl.mdMode = MD_MODE_OSF;
				}
				else{}
			}
			else
			{
				if(SystemControl.mdMode == MD_MODE_OST)       //out ost
				{
					Load_Def();
				}
				else
				{
					SystemControl.mdMode++;
					if (SystemControl.mdMode >= MD_MODE_OST)
					{
						SystemControl.mdMode = MD_MODE_OSF;
					}
					else{}
				}
			}
			Def_Operat(DEF_OPTYPE_MODE);
			SystemControl.AutoBits.MODE = 0;
			break;

		//****************************************
		//def ��˪
			 //****************************************
		case KEY_OST :
			if(SystemControl.OffState )
			{
				//�ػ������
				Load_Off();
				if(SystemControl.mdMode == MD_MODE_OST)
				{
					SystemControl.mdMode = MD_MODE_OSF;
				}

				Save_Def();

				SystemControl.mdMode  = MD_MODE_OST;
				SystemControl.ncfMode = NCF_MODE_NEW;
				SystemControl.acMode  = AC_MODE_ON;
				SystemControl.AutoBits.AC = 0;
				//SystemControl.AutoBits.NCF = 0;
				SystemControl.AutoBits.MODE = 0;
				SystemControl.AutoBits.FAN = 0;

				if (SystemControl.fanMode < FAN_MODE_5L)
				{
					SystemControl.fanMode = FAN_MODE_5L;
					(void)FanControl_SetRate(FANVOLT_RATE_INIT);
				}
				else{}
			}
			else
			{
				//���������
				if(SystemControl.mdMode == MD_MODE_OST)
				{
					Load_Def();
				}
				else
				{
					Save_Def();
					if (SystemControl.fanMode < FAN_MODE_5L)
					{
						SystemControl.fanMode = FAN_MODE_5L;
						(void)FanControl_SetRate(FANVOLT_RATE_INIT);
					}

					SystemControl.mdMode = MD_MODE_OST;
					SystemControl.ncfMode = NCF_MODE_NEW;
					SystemControl.acMode = AC_MODE_ON;
					SystemControl.AutoBits.AC = 0;
					SystemControl.AutoBits.NCF = 0;
					SystemControl.AutoBits.MODE = 0;
					SystemControl.AutoBits.FAN = 0;
				}
			}
			//sHPOutControl.Byte = 0;
			SystemControl.MaxACMode = AC_MAX_MODE_OFF ; //Acmax �رգ�
			break;
		//*********************��ʾ******************	     
	#if  _PROJECT_LCD_
//		case  KEY_AC + 0xff:
//		  //��ʾ ���� ʱ��
//			if(!SystemControl.OffState && SystemControl.tset == TSET_HI)
//			{
//				DisTsetShakeSet(sSenSorData.TevpCal) ;
//			}
//			break;
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
	TestMode_restrict();
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
