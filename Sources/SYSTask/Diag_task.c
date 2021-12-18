/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Diag_Task.c
* 功能描述：SystemState == DIAG_TASK时 自检任务运行此程序
* 自检功能模块调用 ，本模块实现
* 1 屏幕LCD ，LED,OUT ，跳闪
* 2 电机测试
* 3 版本号测试
* 4 部分DTC 设置
* 编译器： codewarrior IDE 10.7
* MCU说明： 与MCU无关
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年3月14日
*
* 版本：1.1
* 说明：枚举类型赋值优化 
* 作者：xb
* 完成日期：2021年8月23日
*/
#include "APPDrive.h"


static  uint8  DiagStep=0;    //自检的步数
static  uint16 TimerDiag2s;   // 自检定时参数
static  uint8  XsErrCount;   //当前显示的故障码
static  uint8  u8DTCType = DTCB_TESTFAILED; 

/*******************************************************************************
 * 自检步数宏定义
 *
 * ****************************************************************************/
#define   DIAG_STEP_ENTER 0       //进入自检
#define   DIAG_STEP_ONE  1        //自检步骤1
#define   DIAG_STEP_TWO  2
#define   DIAG_STEP_THREE  3
#define   DIAG_STEP_FOUR  4
#define   DIAG_STEP_XSERRCODE 5    //显示故障码
#define   DIAG_STEP_OUT  255     //退出自检

/*******************************************************************************
 *函数声明
 *
 * ****************************************************************************/
static  uint8  SelectNext_DiagErrCode(uint8 u8ErrCount);
/*******************************************************************************
 * Function:  void  DiagStepSet(uint8 step)
 *
 * Description: 进入自检，设定当前的自检步数
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
 * Description: 执行自检的相关任务
 *
 * Returns:     需要退出时返回1 ，否则返回0
 *
 * Notes:       按照客户方案进行
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
		//off 按键可以直接退出自检
			DiagStep=DIAG_STEP_OUT;
		break;
		//其他按键功能可在此处增加
	}
  
	switch(DiagStep)
	{
		default:
		case  DIAG_STEP_OUT:
			//outdiag  全AUTO,
			
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
			
			//实现全亮全面 5次
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
			//电机自检
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
			//版本号显示
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
      //闪烁故障码
   	 
			if(TimeOutChkTenthSLong(&TimerDiag2s,5))
			{//0.5 开  0.5s 灭 0.5s 开  1s灭 下一个
				//nDiag0 =0,2 亮，134灭， 0-4循环
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
 * Description: 选择下一个故障码，ErrCount 为当前的故障码
 *
 * Returns:     下一个故障码
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
	 {//没有故障码
		 u8ErrCount = 0;
	 }
	 else{}
	 
	 return u8ErrCount;
 }
