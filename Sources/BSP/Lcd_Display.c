/************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：LCD_Display.c
* 功能描述：本模块实现LCD 刷新显示功能， 实现。为lCD驱动的驱动应用接口，此驱动接口适合LCDDATA的显示数字和状态，
* 如有不同建立不同应用接口。
* 实现如下功能：
 * 1 为lCD模块和APP之间提供基本驱动接口，按照当前系统状态进行刷新 
 * 1.1 全屏显示，全部熄灭
 * 1.2 可显示温度区的显示状态 ①  显示设置温度 ② 其他温度并闪烁
 * 
 * 2 具体参考外部函数调用
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为控制层和MCU 之间的接口
*/        
/**************************更改记录******************************************************
 * 
* 
* 版本：1.0
* 作者：xb
* 完成日期：2019年3月14日
* 
* 版本：1.2
* 作者：xb
* 完成日期：20190920
* 1 模板更新，显示闪烁功能更新
*
* 版本：2.0
* 作者：xb 完成日期：20200406
* 1 模板更新，增加备注说明
* 
* 版本：2.1
* 作者：xb 完成日期：20200603
* 1 LCD 开机清空
* 2 优化LCD显示
* 
* * 
* 版本：2.2
* 说明：优化 SetLcdDisplayData(LCDDATA *pLCDData)
* 作者：xb
* 完成日期：2020年11月19日
* 
* 版本：2.3
* 说明：适应HT1621和16K23
* 作者：xb
* 完成日期：2021年3月15日
*/
#include "OutANDDisplay.h"
#include "timerout.h"
#include "LCD_MAP.h"
#include "SystemConfig.h"
#ifdef _HT_1621_
	#include "ht1621_Drive.h"
#endif 
#ifdef _HT_16K23_	
	#include "ht16k23_Drive.h"
#endif


// 本模块变量定义
static const uint8 bcd[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

static uint16  TimerDisTset;     //设置温度区显示的时间
static uint8   nDisplayTset;     //设置温度区显示的次数
typedef struct
{
	int16 i16TempShake;           //温度区闪烁时显示的温度
	uint8 LCDTShakeMode;          //温度区显示   0: 正常  1：闪烁
	LCDModeType LCDDisplayMode;   //显示模式  LCDDISMODE
}LCDCONTROL;

static LCDCONTROL sLContorl;
static LCDDATA    sLDisplayData;



//内部实现的函数
static   void	SetLcd_Tset(int16 i16Temp);
static   void	SetLcd_Whole(uint8 lcds);
static	void	LcdDisplayNormal(void);

/*******************************************************************************
 * Function:   void	SetLcd_Tset(uint8 tset)
 *
 * Description: 设置LCD test 区显示的数字
 *
 * Returns:     none，
 *
 * Notes:      显示为数字的模板
 *		
*******************************************************************************/
static void	SetLcd_Tset(int16 i16Temp)
{
	uint8 ltdl,ltdm,ltdr ;
	
	LCD_1A = 0;
	LCD_1B = 0;
	LCD_1C = 0;
	LCD_1D = 0;
	LCD_1E = 0;
	LCD_1F = 0;
	LCD_1G = 0;
	
	LCD_2A = 0;
	LCD_2B = 0;
	LCD_2C = 0;
	LCD_2D = 0;
	LCD_2E = 0;
	LCD_2F = 0;
	LCD_2G = 0;
	
	LCD_3A = 0;
	LCD_3B = 0;
	LCD_3C = 0;
	LCD_3D = 0;
	LCD_3E = 0;
	LCD_3F = 0;
	LCD_3G = 0;
	
	LCD_DOIT = 0;
	LCD_CEL = 0;
	
	if (i16Temp == TSET_LCD_OFF)   //off      
	{
		
	}
	else if (i16Temp == TSET_LCD_LO)     //lo  
	{
      LCD_1A = 0;
      LCD_1B = 0;
      LCD_1C = 0;
      LCD_1D = 1;
      LCD_1E = 1;
      LCD_1F = 1;
      LCD_1G = 0;

      LCD_2A = 0;
      LCD_2B = 0;
      LCD_2C = 1;
      LCD_2D = 1;
      LCD_2E = 1;
      LCD_2F = 0;
      LCD_2G = 1;
  }
  else if (i16Temp == TSET_LCD_HI)     //Hi 
  {
      LCD_1A = 0;
      LCD_1B = 1;
      LCD_1C = 1;
      LCD_1D = 0;
      LCD_1E = 1;
      LCD_1F = 1;
      LCD_1G = 1;

      LCD_2A = 0;
      LCD_2B = 1;
      LCD_2C = 1;
      LCD_2D = 0;
      LCD_2E = 0;
      LCD_2F = 0;
      LCD_2G = 0;
	}
	else//正常情况下
	{
		if (i16Temp < 0)
		{
			//负号显示
			i16Temp = -i16Temp;
		}else{}
		ltdl = (unsigned char)(i16Temp / 100);
		i16Temp = i16Temp % 100;
		ltdm = (unsigned char)(i16Temp / 10);
		ltdr = (unsigned char)(i16Temp % 10);
		
		if(ltdl)
		{ //首位为0不显示
			ltdl = bcd[ltdl];
		}
		
		ltdm = bcd[ltdm];
		ltdr = bcd[ltdr];
		
		LCD_DOIT = 1;
		LCD_CEL = 1;
		

		if( ltdl & 0x01 )	LCD_1A = 1;
		if( ltdl & 0x02 )	LCD_1B = 1;
		if( ltdl & 0x04 )	LCD_1C = 1;
		if( ltdl & 0x08 )	LCD_1D = 1;
		if( ltdl & 0x10 )	LCD_1E = 1;
		if( ltdl & 0x20 )	LCD_1F = 1;
		if( ltdl & 0x40 )	LCD_1G = 1;
		
		if( ltdm & 0x01 )	LCD_2A = 1;
		if( ltdm & 0x02 )	LCD_2B = 1;
		if( ltdm & 0x04 )	LCD_2C = 1;
		if( ltdm & 0x08 )	LCD_2D = 1;
		if( ltdm & 0x10 )	LCD_2E = 1;
		if( ltdm & 0x20 )	LCD_2F = 1;
		if( ltdm & 0x40 )	LCD_2G = 1;
		
		if( ltdr & 0x01 )	LCD_3A = 1;
		if( ltdr & 0x02 )	LCD_3B = 1;
		if( ltdr & 0x04 )	LCD_3C = 1;
		if( ltdr & 0x08 )	LCD_3D = 1;
		if( ltdr & 0x10 )	LCD_3E = 1;
		if( ltdr & 0x20 )	LCD_3F = 1;
		if( ltdr & 0x40 )	LCD_3G = 1;
	}
}
/*******************************************************************************
 * Function:    void	SetLcd_OtherData(void)
 *
 * Description: 其他字符设置
 *
 * Returns:     none，
 *
 * Notes:      
 *		
*******************************************************************************/
static void	SetLcd_OtherData(void)
{
	//TODO: 根据sLDisplayData 编写除Tset以外的字符
}
/*******************************************************************************
 * Function:  void  SetLcd_Whole(uint8 lcds)
 *
 * Description: 全部显示 0： 全灭   1：全屏
 *
 * Returns:     none
 *
 * Notes:   不能改动
 *
*******************************************************************************/
static void  SetLcd_Whole(uint8 lcds)
{
	uint8 i;
	if(lcds)       //全屏
	{
		for(i=0;i<HTDATA_FACTNUM;i++)
		htdata[i].Byte=0xFF;
	}
	else     //全灭
	{
		for(i=0;i<HTDATA_FACTNUM;i++)
		htdata[i].Byte=0;
	}
}

/*************************************************************
 * Function:   void LcdDisplayNormal(void)
 *
 * Description:  刷新方式
 *
 * Returns:     NONE   
 *
 * Notes:    不能改动
 *          
***************************************************************/
static   void LcdDisplayNormal(void)
{
	if (sLContorl.LCDDisplayMode == LCD_MODE_CLEAR)
	{
		SetLcd_Whole(0); //如有需要 可考虑不加 
	}
	else if (sLContorl.LCDDisplayMode == LCD_MODE_ALL)
	{
		SetLcd_Whole(1); //如有需要 可考虑不加 
	}
	else
	{
		//按照设置温度区的显示方式进行显示，特殊的显示基本 0.5s 亮灭3次
		if (sLContorl.LCDTShakeMode)
		{//闪烁模式
			if (TimeOutChkTenthSLong(&TimerDisTset,5)) nDisplayTset++;
			else{}
			
			if (nDisplayTset >= 6)
			{
				sLContorl.LCDTShakeMode = 0;
			}
			else if (nDisplayTset & 0x01)
			{
				SetLcd_Tset(sLContorl.i16TempShake);
			}
			else
			{
				SetLcd_Tset(TSET_LCD_OFF);
			}
		}
		else
		{//正常模式
			SetLcd_Tset(sLDisplayData.i16Temp);
		}
		SetLcd_OtherData();
	}
}

/*************************************************************
 * Function:   void Ht16k23APP_LCDSet(void)
 *
 * Description:  被HT16K23等驱动IC的驱动程序调用,设置LCD的控制字
 *
 * Returns:     none
 *
 * Notes:      不能被改变
 *
***************************************************************/
#ifdef _HT_1621_
void Ht1621APP_LCDSet(void)
{
	LcdDisplayNormal();
}
#endif
#ifdef _HT_16K23_	
void Ht16k23APP_LCDSet(void)
{
	LcdDisplayNormal();
}
#endif
/*************************************************************
 * Function:  void DisTsetShakeSet(int16 i16Temp)
 *
 * Description: 设置温度区闪烁显示i16Temp的值
 *
 * Returns:     none
 *
 * Notes:    不能被改动
***************************************************************/
void DisTsetShakeSet(int16 i16Temp)
{
	sLContorl.LCDTShakeMode = 1;
	sLContorl.i16TempShake = i16Temp;
	nDisplayTset = 0;
}
/*************************************************************
 * Function:  void SetLcdDisplayMode(uint8 mode)
 *
 * Description:  设置整体显示方式，可供应用程序调用
 *
 * Returns:    none
 *
 * Notes:    不能被改动
 *           
***************************************************************/
void SetLcdDisplayMode(LCDModeType mode)
{
	sLContorl.LCDDisplayMode = mode;
}
/*************************************************************
 * Function:  void  SetLcdDisplayData(LCDDATA *pLCDData)
 *
 * Description:  设置LCD 显示数据。有需要时调用
 *
 * Returns:   none
 *
 * Notes:     不能改动
 *
***************************************************************/
void  SetLcdDisplayData(LCDDATA *pLCDData)
{
	MemCopy(&sLDisplayData, pLCDData, sizeof(LCDDATA));
}
/*************************************************************
 * Function:  void Clear_LCD(void)
 *
 * Description:  清楚LCD 控制字
 *
 * Returns:    none
 *
 * Notes:    不能改动
***************************************************************/
void Clear_LCD(void)
{
#if _PROJECT_LCD_
	#ifdef _HT_1621_
		Ht1621_Clear();
	#endif 
	#ifdef _HT_16K23_	
		Ht16k23_Clear();
	#endif
#endif
}
/*************************************************************
 * Function:  void Updata_LCD(void)
 *
 * Description:  驱动LCD 的IC
 *
 * Returns:    none
 *
 * Notes:    不能改动
***************************************************************/
void Updata_LCD(void)
{
#if _PROJECT_LCD_
	#ifdef _HT_1621_
		Ht1621_Updata();
	#endif 
	#ifdef _HT_16K23_	
		Ht16k23_Updata();
	#endif	
#endif

}

/*************************************************************
 * Function:  void Init_LCD(void)
 *
 * Description:  初始化LCD 的IC
 *
 * Returns:    none
 *
 * Notes:    不能改动
***************************************************************/
void Init_LCD(void)
{
	
#if _PROJECT_LCD_
	#ifdef  _HT_1621_
		Ht1621_LCDDataSet((uint8*)htdata, HTDATA_FACTNUM);
	#endif
	#ifdef  _HT_16K23_
		Ht16k23_LCDDataSet((uint8*)htdata, HTDATA_FACTNUM);
	#endif	
#endif
	Clear_LCD();	
	sLContorl.LCDDisplayMode = LCD_MODE_CLEAR;
	sLContorl.LCDTShakeMode = 0;
}


