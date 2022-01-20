/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Led.c
* 功能描述： LED 设置输出
* 1 版本号显示功能
* MCU说明：和MCU 无关
*  
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月13日
* 
*  版本：2.0
* 说明：LED 显示版本号功能升级
* 作者：lyc
* 完成日期：2020年5月29日
* 
*  版本：2.1
* 说明：LED 增加暗一级的接口
* 作者：xb
* 完成日期：2020年11月21日
*/

#include "Led.h"

static uint8 u8LEDVerL, u8LEDVerH;

//指示灯版本号显示变量
static uint8   VerDataH,VerDataL;    //显示的数据
static uint8   DisVerEn;   //显示版本号使能  使能时指示灯 在otherControl中显示
static uint16  TimerDisVer;  //显示闪烁时间
static uint8   DisVerState;  //状态

//显示版本号的状态
#define  DISVER_STATE_BEGIN      0     //开始关闭2s
#define  DISVER_STATE_DISHW      1     //显示高高位
#define  DISVER_STATE_DISLW      2     //显示高低位
#define  DISVER_STATE_OFF        3     //显示高位时off
#define  DISVER_STATE_DISsHW     4    // 显示低高位
#define  DISVER_STATE_DISsLW     5    // 显示低低位
#define  DISVER_STATE_OFF_L      6    // 显示低位时off
#define  DISVER_STATE_OVER       7    // 最后停止2s ，退出显示版本号功能状态

#define  DISVER_STATE_PUASE1     8    // 高高位至高低位中间暂停显示，off
#define  DISVER_STATE_PUASE2     9    // 高低位至低高位中间暂停显示，off
#define  DISVER_STATE_PUASE3     10    //低高位至低低位中间暂停显示，off

/*******************************************************************************
 * Function:   void  LEDControl(void)
 *
 * Description:  led control
 *
 * Returns:     none
 *
 * Notes:       根据displaydata 输出led
 *
*******************************************************************************/
static void LEDControl(void)
{
	//TODO: 根据sDisplayData的值编写LED 的输出
	OL_LCD = 1;
	if (sDisplayData.LEDMode > 0)
	{//按不同模式设置
		GPIO_BIT_EQUAL(OL_AC, sDisplayData.AC);
		GPIO_BIT_EQUAL(OL_CIRF, sDisplayData.CIRF);
		GPIO_BIT_EQUAL(OL_PTC, sDisplayData.ptc);
		GPIO_BIT_EQUAL(OL_RHEAT, sDisplayData.RHEAT);
	}
	else
	{//全部熄灭
		GPIO_BIT_EQUAL(OL_AC, 0);
		GPIO_BIT_EQUAL(OL_CIRF, 0);
		GPIO_BIT_EQUAL(OL_PTC, 0);
		GPIO_BIT_EQUAL(OL_RHEAT, 0);
	}
}
/*******************************************************************************
 * Function: void	Led_AllClose(void)
 *           void	LED_AllOpen(void)
 * Description:  led 全部关闭，开启， 需要时时调用，
 *
 * Returns:     none
 *
 * Notes:       按要求编写
 *
*******************************************************************************/
void	LED_AllClose(void)
{
	//TODO: 编写全关， 备注会影响LCD,COM 等
	sDisplayData.LEDMode = 0;
	LEDControl();
}


void	LED_AllOpen(void)
{
	//TODO: 编写全开，自检用
	sDisplayData.LEDMode = 1;
	sDisplayData.AC = 1;
	LEDControl();
}
/*******************************************************************************
 * Function:   void  LEDDisVer_Control(void)
 *
 * Description:  led 显示版本号,	此功能开启时 相关led驱动被关闭
 *              版本号显示过程为，1 全部灭2s  2 高位指示灯 灭0.5s 亮0.5s。次数为高位的
 *              值，3 低位指示灯 灭0.5s 亮0.5s。次数为低位的值。 4 全灭2s 5正常工作
 *
 * Returns:     NONE
 *
 * Notes:      不用更改
*******************************************************************************/
static void  LEDDisVer_Control(void)
{
   //版本号显示控制
   if(DisVerEn)
   {
      switch(DisVerState)
      {
         default:
         case  DISVER_STATE_OVER:
            //2s 显示完毕
            if(TimeOutChkMsLong(&TimerDisVer, 2000))
            { 
               DisVerState = DISVER_STATE_OVER;
               DisVerEn =0;
            }
            u8LEDVerL = 0;
            u8LEDVerH = 0;
            break;
         case  DISVER_STATE_BEGIN:
            if(TimeOutChkMsLong(&TimerDisVer, 2000))
            {
               DisVerState = DISVER_STATE_OFF;
            }
            u8LEDVerL = 0;
            u8LEDVerH = 0;    
            break;
         case  DISVER_STATE_PUASE1:
				if(TimeOutChkMsLong(&TimerDisVer,1000))
				{
					DisVerState = DISVER_STATE_OFF;
				}
				u8LEDVerL = 0;
				u8LEDVerH = 0;    
				break;
			case  DISVER_STATE_PUASE2:
				if(TimeOutChkMsLong(&TimerDisVer,1000))
				{
					DisVerState = DISVER_STATE_OFF_L;
				}
				u8LEDVerL = 0;
				u8LEDVerH = 0;    
				break;
			case  DISVER_STATE_PUASE3:
				if(TimeOutChkMsLong(&TimerDisVer,1000))
				{
					DisVerState = DISVER_STATE_OFF_L;
				}
				u8LEDVerL = 0;
				u8LEDVerH = 0;   
				break;
         case  DISVER_STATE_OFF:
            if(TimeOutChkMsLong(&TimerDisVer,500))
            {
               if(VerDataH>=10)
               {
                  VerDataH = VerDataH-10;
                  DisVerState = DISVER_STATE_DISHW;
                  u8LEDVerL = 1;
               }
               else if(VerDataH)
               {
                  VerDataH--;
                  DisVerState = DISVER_STATE_DISLW;
                  u8LEDVerL = 0;
               }
               else
               {
                  DisVerState = DISVER_STATE_PUASE2;
               }
            }
            u8LEDVerH = 0;
            break;
         case  DISVER_STATE_DISLW:
            //显示高低位
            if(TimeOutChkMsLong(&TimerDisVer,500))
            {
               DisVerState = DISVER_STATE_OFF;
            }
            u8LEDVerL = 0;
            u8LEDVerH = 1;
            break;
         case  DISVER_STATE_DISHW:
            //显示高高位
         	if(TimeOutChkMsLong(&TimerDisVer,500))
				{
					if(VerDataH>=10)
					{
						DisVerState = DISVER_STATE_OFF;
					}
					else if (VerDataH>0)
					{//高高显示完之后，显示高低时全关1s
						DisVerState = DISVER_STATE_PUASE1;
					}
					else
					{
						DisVerState = DISVER_STATE_PUASE2;
					}
				}
         	
            u8LEDVerL = 1;
            u8LEDVerH = 1;
            break;
         case  DISVER_STATE_OFF_L:
				if(TimeOutChkMsLong(&TimerDisVer,500))
				{
					if(VerDataL>=10)
					{
						VerDataL = VerDataL-10;
						DisVerState = DISVER_STATE_DISsHW;
						u8LEDVerH = 1;
					}
					else if(VerDataL)
					{
						VerDataL--;
						DisVerState = DISVER_STATE_DISsLW;
						u8LEDVerH = 0;
					}
					else
					{
						DisVerState = DISVER_STATE_OVER;
					}
				}
				u8LEDVerL = 0;
				break;
			  case  DISVER_STATE_DISsLW:
				//显示低低位
				if(TimeOutChkMsLong(&TimerDisVer,500))
				{
					DisVerState = DISVER_STATE_OFF_L;
				}
				u8LEDVerH = 0;
				u8LEDVerL = 1;
				break;
			case  DISVER_STATE_DISsHW:
				//显示高低位
				if(TimeOutChkMsLong(&TimerDisVer,500))
				{
					if(VerDataL>=10)
					{
						DisVerState = DISVER_STATE_OFF_L;
					}
					else if(VerDataL>0)
					{
						DisVerState = DISVER_STATE_PUASE3;
					}
					else
					{
						DisVerState = DISVER_STATE_OVER;
					}
				}
				u8LEDVerH = 1;
				u8LEDVerL = 1;
				break;
      }
   }
}
/*******************************************************************************
 * Function:   void  LedDisVerEn(void)
 *
 * Description:  led 显示启动或者关闭
 *
 * Returns:     none
 *
 * Notes:       需要启动时调用，显示控制在	LEDDisVer_Control
 *              不能更改
*******************************************************************************/
void  LedDisVerEn(uint16 u16VerData)
{
   if(DisVerEn)
   {
      //如果正在显示则关闭
      DisVerEn =0;
   }
   else
   { //启动
      DisVerEn =1;
      DisVerState = DISVER_STATE_BEGIN;  //进入开始显示的工作
      (void)TimeOutChkMsLong(&TimerDisVer,0);
      VerDataH = u16VerData/100;
      VerDataL = u16VerData%100;
   }
}
/*******************************************************************************
 * Function:   void LEDSetTask(void)
 *
 * Description:  led 输出显示，被LowerDrive- LOOP 调用
 *
 * Returns:     none
 *
 * Notes:      不改动
 *
*******************************************************************************/
void LEDSetTask(void)
{
	BOOL bVerHOld, bVerLOld;
	
	LEDDisVer_Control();
	
	if(DisVerEn) 
	{
		//显示故障码时此函数没用
		bVerHOld = SWVERH_LED;
		bVerLOld = SWVERL_LED;
		
		SWVERH_LED = u8LEDVerH;
		SWVERL_LED = u8LEDVerL;
		
		LEDControl();
		
		SWVERH_LED = bVerHOld;
		SWVERL_LED = bVerLOld;
	}
	else
	{
		LEDControl();
	}
}


/*******************************************************************************
 * Function:    BOOL  LED_GetDisVer(void)
 *
 * Description:  返回led 版本号的显示状态,需要时调用，
 *
 * Returns:     none
 *
 * Notes:       不改动
 *
*******************************************************************************/
BOOL  LED_GetDisVer(void)
{
	return DisVerEn;
}
