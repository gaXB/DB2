/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�LCD_Display.c
* ������������ģ��ʵ��LCD ˢ����ʾ���ܣ� ʵ�֡�ΪlCD����������Ӧ�ýӿڣ��������ӿ��ʺ�LCDDATA����ʾ���ֺ�״̬��
* ���в�ͬ������ͬӦ�ýӿڡ�
* ʵ�����¹��ܣ�
 * 1 ΪlCDģ���APP֮���ṩ���������ӿڣ����յ�ǰϵͳ״̬����ˢ�� 
 * 1.1 ȫ����ʾ��ȫ��Ϩ��
 * 1.2 ����ʾ�¶�������ʾ״̬ ��  ��ʾ�����¶� �� �����¶Ȳ���˸
 * 
 * 2 ����ο��ⲿ��������
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ���Ʋ��MCU ֮��Ľӿ�
*/        
/**************************���ļ�¼******************************************************
 * 
* 
* �汾��1.0
* ���ߣ�xb
* ������ڣ�2019��3��14��
* 
* �汾��1.2
* ���ߣ�xb
* ������ڣ�20190920
* 1 ģ����£���ʾ��˸���ܸ���
*
* �汾��2.0
* ���ߣ�xb ������ڣ�20200406
* 1 ģ����£����ӱ�ע˵��
* 
* �汾��2.1
* ���ߣ�xb ������ڣ�20200603
* 1 LCD �������
* 2 �Ż�LCD��ʾ
* 
* * 
* �汾��2.2
* ˵�����Ż� SetLcdDisplayData(LCDDATA *pLCDData)
* ���ߣ�xb
* ������ڣ�2020��11��19��
* 
* �汾��2.3
* ˵������ӦHT1621��16K23
* ���ߣ�xb
* ������ڣ�2021��3��15��
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


// ��ģ���������
static const uint8 bcd[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

static uint16  TimerDisTset;     //�����¶�����ʾ��ʱ��
static uint8   nDisplayTset;     //�����¶�����ʾ�Ĵ���
typedef struct
{
	int16 i16TempShake;           //�¶�����˸ʱ��ʾ���¶�
	uint8 LCDTShakeMode;          //�¶�����ʾ   0: ����  1����˸
	LCDModeType LCDDisplayMode;   //��ʾģʽ  LCDDISMODE
}LCDCONTROL;

static LCDCONTROL sLContorl;
static LCDDATA    sLDisplayData;



//�ڲ�ʵ�ֵĺ���
static   void	SetLcd_Tset(int16 i16Temp);
static   void	SetLcd_Whole(uint8 lcds);
static	void	LcdDisplayNormal(void);

/*******************************************************************************
 * Function:   void	SetLcd_Tset(uint8 tset)
 *
 * Description: ����LCD test ����ʾ������
 *
 * Returns:     none��
 *
 * Notes:      ��ʾΪ���ֵ�ģ��
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
	else//���������
	{
		if (i16Temp < 0)
		{
			//������ʾ
			i16Temp = -i16Temp;
		}else{}
		ltdl = (unsigned char)(i16Temp / 100);
		i16Temp = i16Temp % 100;
		ltdm = (unsigned char)(i16Temp / 10);
		ltdr = (unsigned char)(i16Temp % 10);
		
		if(ltdl)
		{ //��λΪ0����ʾ
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
 * Description: �����ַ�����
 *
 * Returns:     none��
 *
 * Notes:      
 *		
*******************************************************************************/
static void	SetLcd_OtherData(void)
{
	//TODO: ����sLDisplayData ��д��Tset������ַ�
}
/*******************************************************************************
 * Function:  void  SetLcd_Whole(uint8 lcds)
 *
 * Description: ȫ����ʾ 0�� ȫ��   1��ȫ��
 *
 * Returns:     none
 *
 * Notes:   ���ܸĶ�
 *
*******************************************************************************/
static void  SetLcd_Whole(uint8 lcds)
{
	uint8 i;
	if(lcds)       //ȫ��
	{
		for(i=0;i<HTDATA_FACTNUM;i++)
		htdata[i].Byte=0xFF;
	}
	else     //ȫ��
	{
		for(i=0;i<HTDATA_FACTNUM;i++)
		htdata[i].Byte=0;
	}
}

/*************************************************************
 * Function:   void LcdDisplayNormal(void)
 *
 * Description:  ˢ�·�ʽ
 *
 * Returns:     NONE   
 *
 * Notes:    ���ܸĶ�
 *          
***************************************************************/
static   void LcdDisplayNormal(void)
{
	if (sLContorl.LCDDisplayMode == LCD_MODE_CLEAR)
	{
		SetLcd_Whole(0); //������Ҫ �ɿ��ǲ��� 
	}
	else if (sLContorl.LCDDisplayMode == LCD_MODE_ALL)
	{
		SetLcd_Whole(1); //������Ҫ �ɿ��ǲ��� 
	}
	else
	{
		//���������¶�������ʾ��ʽ������ʾ���������ʾ���� 0.5s ����3��
		if (sLContorl.LCDTShakeMode)
		{//��˸ģʽ
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
		{//����ģʽ
			SetLcd_Tset(sLDisplayData.i16Temp);
		}
		SetLcd_OtherData();
	}
}

/*************************************************************
 * Function:   void Ht16k23APP_LCDSet(void)
 *
 * Description:  ��HT16K23������IC�������������,����LCD�Ŀ�����
 *
 * Returns:     none
 *
 * Notes:      ���ܱ��ı�
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
 * Description: �����¶�����˸��ʾi16Temp��ֵ
 *
 * Returns:     none
 *
 * Notes:    ���ܱ��Ķ�
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
 * Description:  ����������ʾ��ʽ���ɹ�Ӧ�ó������
 *
 * Returns:    none
 *
 * Notes:    ���ܱ��Ķ�
 *           
***************************************************************/
void SetLcdDisplayMode(LCDModeType mode)
{
	sLContorl.LCDDisplayMode = mode;
}
/*************************************************************
 * Function:  void  SetLcdDisplayData(LCDDATA *pLCDData)
 *
 * Description:  ����LCD ��ʾ���ݡ�����Ҫʱ����
 *
 * Returns:   none
 *
 * Notes:     ���ܸĶ�
 *
***************************************************************/
void  SetLcdDisplayData(LCDDATA *pLCDData)
{
	MemCopy(&sLDisplayData, pLCDData, sizeof(LCDDATA));
}
/*************************************************************
 * Function:  void Clear_LCD(void)
 *
 * Description:  ���LCD ������
 *
 * Returns:    none
 *
 * Notes:    ���ܸĶ�
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
 * Description:  ����LCD ��IC
 *
 * Returns:    none
 *
 * Notes:    ���ܸĶ�
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
 * Description:  ��ʼ��LCD ��IC
 *
 * Returns:    none
 *
 * Notes:    ���ܸĶ�
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


