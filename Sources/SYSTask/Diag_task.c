/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Diag_Task.c
* ����������SystemState == DIAG_TASKʱ �Լ��������д˳���
* �Լ칦��ģ����� ����ģ��ʵ��
* 1 ��ĻLCD ��LED,OUT ������
* 2 �������
* 3 �汾�Ų���
* 4 ����DTC ����
* �������� codewarrior IDE 10.7
* MCU˵���� ��MCU�޹�
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��3��14��
*
* �汾��1.1
* ˵����ö�����͸�ֵ�Ż� 
* ���ߣ�xb
* ������ڣ�2021��8��23��
*/
#include "APPDrive.h"


static  uint8  DiagStep=0;    //�Լ�Ĳ���
static  uint16 TimerDiag2s;   // �Լ춨ʱ����
static  uint8  XsErrCount;   //��ǰ��ʾ�Ĺ�����
static  uint8  u8DTCType = DTCB_TESTFAILED; 

/*******************************************************************************
 * �Լ첽���궨��
 *
 * ****************************************************************************/
#define   DIAG_STEP_ENTER 0       //�����Լ�
#define   DIAG_STEP_ONE  1        //�Լ첽��1
#define   DIAG_STEP_TWO  2
#define   DIAG_STEP_THREE  3
#define   DIAG_STEP_FOUR  4
#define   DIAG_STEP_XSERRCODE 5    //��ʾ������
#define   DIAG_STEP_OUT  255     //�˳��Լ�

/*******************************************************************************
 *��������
 *
 * ****************************************************************************/
static  uint8  SelectNext_DiagErrCode(uint8 u8ErrCount);
/*******************************************************************************
 * Function:  void  DiagStepSet(uint8 step)
 *
 * Description: �����Լ죬�趨��ǰ���Լ첽��
 *
 * Returns:     none
 *
 * Notes:
 *
*******************************************************************************/
void  DiagStepSet(uint8 step)
{
  DiagStep=step;
}

/*******************************************************************************
 * Function:   uint8  Run_Diag_Task(void)
 *
 * Description: ִ���Լ���������
 *
 * Returns:     ��Ҫ�˳�ʱ����1 �����򷵻�0
 *
 * Notes:       ���տͻ���������
 *
*******************************************************************************/
uint8  Run_Diag_Task(void)
{
	static  uint8 XsErrBit;
	static  uint8 nWholeLcdLampTimes,nDiag0;
	static  uint8 bDisErrMode;
	uint8 u8RunEndFlg = 0;
	uint16   keywd;
	
	keywd = ReadComd();
	keywd=Read_Keydate();

	switch(keywd)
	{
		default:
			break;
		case  KEY_OFF:
		//off ��������ֱ���˳��Լ�
			DiagStep=DIAG_STEP_OUT;
		break;
		//�����������ܿ��ڴ˴�����
	}
  
	switch(DiagStep)
	{
		default:
		case  DIAG_STEP_OUT:
			//outdiag  ȫAUTO,
			
			SystemControl.AutoBits.AC  = 1;
			SystemControl.AutoBits.NCF  = 1;
			SystemControl.AutoBits.FAN = 1;
			SystemControl.AutoBits.MODE = 1;
			SetLcdDisplayMode(LCD_MODE_NORMAL);
			u8RunEndFlg = 1;
			break;
		case  DIAG_STEP_ENTER:
			(void)TimeOutChkTenthSLong(&TimerDiag2s,0);
			nWholeLcdLampTimes=0;
			u8DTCType = DTCB_TESTFAILED; 
			SetLcdDisplayMode(LCD_MODE_NORMAL);
			DiagStep=DIAG_STEP_ONE;					
			break;
		case  DIAG_STEP_ONE:
			//XS WHOLE LCD
			//XS WHOLE LCD
			if(TimeOutChkTenthSLong(&TimerDiag2s,10))
			{
				nWholeLcdLampTimes++;   
				if(nWholeLcdLampTimes>=6)
				{
					DiagStep=DIAG_STEP_TWO;
					XsErrCount=0;
					nWholeLcdLampTimes = 0;
					break;
				}
			}
			
			//ʵ��ȫ��ȫ�� 5��
			if (nWholeLcdLampTimes&0x01)
			{
				LED_AllClose();
				OUT_AllClose();
				SetLcdDisplayMode(LCD_MODE_CLEAR);
			}
			else
			{
				LED_AllOpen();
				OUT_AllOpen();
				SetLcdDisplayMode(LCD_MODE_ALL);
			}
			break;
		case  DIAG_STEP_TWO:
			//����Լ�
			if(TimeOutChkTenthSLong(&TimerDiag2s,10))
			{
				nWholeLcdLampTimes++;   
				if(nWholeLcdLampTimes >= 16)
				{
					DiagStep = DIAG_STEP_THREE;
					nWholeLcdLampTimes = 0;
					SetLcdDisplayMode(LCD_MODE_NORMAL);
				}
			}
		
			if (nWholeLcdLampTimes > 8)
			{
				(void)Motor_SetPosition(0, 15);
				(void)Motor_SetPosition(1, 15);
				(void)Motor_SetPosition(2, 15);
				(void)NcfMotor_SetPosition(0, NCFMOTOR_POSITION_BACKWARD);
			}
			else
			{
				(void)Motor_SetPosition(0, 240);
				(void)Motor_SetPosition(1, 240);
				(void)Motor_SetPosition(2, 240);
				(void)NcfMotor_SetPosition(0, NCFMOTOR_POSITION_FORWARD);
			}
			break;
		case  DIAG_STEP_THREE:
			//�汾����ʾ
			DisTsetShakeSet(SwV_ *100 + SwsV_ * 10);
			LedDisVerEn(SwV_ *100 + SwsV_);
			DiagStep = DIAG_STEP_FOUR;
			break;
		case  DIAG_STEP_FOUR:
      // OSD,NEWF,FAN7
			if (LED_GetDisVer() == 0)
			{
	         DiagStep = DIAG_STEP_XSERRCODE;
	         XsErrCount=0;
	         nDiag0=0;
	         bDisErrMode = 0;
	         XsErrBit = SelectNext_DiagErrCode(0);
			}
         break;
		case  DIAG_STEP_XSERRCODE:
      //��˸������
   	 
			if(TimeOutChkTenthSLong(&TimerDiag2s,5))
			{//0.5 ��  0.5s �� 0.5s ��  1s�� ��һ��
				//nDiag0 =0,2 ����134�� 0-4ѭ��
				nDiag0++;
				if(nDiag0>=5)
				{
					nDiag0=0;
					if (!bDisErrMode)
					{
						XsErrBit =SelectNext_DiagErrCode(XsErrBit);
					}else{}
				}
			}
			
			if (nDiag0 & 0x01)
			{
				SetLcdDisplayMode(LCD_MODE_NORMAL);
				DisTsetShakeSet(XsErrBit *10);
			}
			else
			{
				SetLcdDisplayMode(LCD_MODE_CLEAR);
			}

			switch(keywd)
			{
				default:
					break;
				case KEY_AC:
					XsErrBit =SelectNext_DiagErrCode(XsErrBit);
					break;
				case  KEY_DEF:
					if (u8DTCType == DTCB_TESTFAILED)
					{
						u8DTCType = DTCB_CONFIRM; 
						XsErrBit = 0;
					}
					else
					{
						u8DTCType = DTCB_TESTFAILED; 
					} 
					break;
				case KEY_AC+KEY_DEF:
					ClearDTC(0XFF);
					break;
			}
			break;
   }
   return u8RunEndFlg;
}



/*******************************************************************************
 * Function:  uint8  DiagErrCode(uint8* ErrCount)
 *
 * Description: ѡ����һ�������룬ErrCount Ϊ��ǰ�Ĺ�����
 *
 * Returns:     ��һ��������
 *
 * Notes:
 *
*******************************************************************************/
 uint8  SelectNext_DiagErrCode(uint8 u8ErrCount)
 {
	 uint8  u8NoErrNum = 0;
  
	 if (u8ErrCount == 0)
	 {
		 u8ErrCount = 1;
	 }			 	 
	 else{}
	 
	 while (u8NoErrNum < DTC_MAXNUM)
	 {
		 u8ErrCount++;
		 if (u8ErrCount > DTC_MAXNUM)
		 {
			 u8ErrCount = 1;
		 }
		 else{}
		 
		 if (DTC[u8ErrCount-1] & u8DTCType)
		 {
			 break;
		 }
		 else
		 {
			 u8NoErrNum++;
		 }
	 }
	 
	 if (u8NoErrNum >= DTC_MAXNUM)
	 {//û�й�����
		 u8ErrCount = 0;
	 }
	 else{}
	 
	 return u8ErrCount;
 }
