/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Init_Task.c
* 功能描述：SystemState == INIT_TASK时 初始化任务运行此程序
* 本模块实现
* 1 上电 1-2s后退出初始化，并清除按键，命令缓存值
* 编译器： codewarrior IDE 10.7
* MCU说明： 与MCU无关，基本不用更改
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年3月14日
*
*/
#include  "APPDrive.h"

static  uint8   InitState=0;     //是否是第一次进入本任务 0:y 1:n
static  uint16  TimerInitTask;
static  uint8   InitFlg=1;  // 点火初始化，和全掉电初始化的标记， 1为 全掉电

#define  INIT_STATE_ALL   0      //全部初始化为10s
#define  INIT_STATE_IGN   1      //IGN 初始化为2s
#define  INIT_STATE_OVER   2    //退出
/*******************************************************************************
 * Function:  void Enter_InitTask(uint8 InitType)
 *
 * Description:  进入初始化任务时调用，供主程序调用
 *
 * Returns:     none
 *
 * Notes:     当Initflg=1 则进入完全初始化的状态，其他为ign初始化的过程。
 *            InitType  1 全部初始化  0 ：按照默认的InitFlg来运行
*******************************************************************************/
void Enter_InitTask(uint8 InitType)
{
   if(InitType)
   { 
	   InitFlg=1;
   }else{}
   
   if(InitFlg)
   {
      InitState =INIT_STATE_ALL;   
   }
   else
   {
      InitState =INIT_STATE_IGN;
   }

   (void)TimeOutChkTenthSLong(&TimerInitTask,0); 
}

/*******************************************************************************
 * Function:   static  void Deal_Keypad_Code(void)
 *
 * Description: 当前任务的按键处理
 *
 * Returns:     none
 *
 * Notes:       此任务时只需要清按键		
 *
*******************************************************************************/
static  void Deal_Keypad_Code(void)
{

  (void)Read_Keydate();        //初始化时任何按键无效
  (void)   ReadComd();   
   return;
}

/*******************************************************************************
 * Function:    uint8 Run_Init_Task(void)
 *
 * Description: 执行初始化的相关任务
 *
 * Returns:     需要退出时返回1 ，否则返回0
 *
 * Notes:       初始化任务，1 清按键 2 关闭输出2s 后进入普通工作模式
 *
*******************************************************************************/
uint8 Run_Init_Task(void)
{
	uint8 u8RunEndFlg;
	sPanleData.LCDONOFF = 0;
	sPanleData.AC = 0;
	sPanleData.CIRF = 0;
	sPanleData.FANLevel = 0;
	sPanleData.PTCn = 0;
	sPanleData.RHEAT = 0;
	sPanleData.VentMode = 0;
	sPanleData.VerClient = 0;
	sPanleData.VerSelf = 0;
	sPanleData.i16Temp = 0;
   switch(InitState)
   {
      default:
      case  INIT_STATE_OVER:
         InitFlg=0;
         u8RunEndFlg = 1;
		 break;
      case  INIT_STATE_ALL:
         //如果只需要2s后退出，只需要将以下到break 屏蔽
       /*  if(TimeOutChkTenthSLong(&TimerInitTask,100))
         {
            InitState =INIT_STATE_OVER;

         }
         SetLcdDisplayMode(LCD_MODE_ALL);
         TrOut_BLed(1);
         break;*/
   case   INIT_STATE_IGN:
         if(TimeOutChkTenthSLong(&TimerInitTask,10))
         {
            InitState =INIT_STATE_OVER;  
         }
         SetLcdDisplayMode(LCD_MODE_CLEAR);
         u8RunEndFlg = 0;
         break;
   }
 
   Deal_Keypad_Code();
   return u8RunEndFlg;
}












