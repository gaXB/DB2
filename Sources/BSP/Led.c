/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Led.c
* ���������� LED �������
* 1 �汾����ʾ����
* MCU˵������MCU �޹�
*  
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��13��
* 
*  �汾��2.0
* ˵����LED ��ʾ�汾�Ź�������
* ���ߣ�lyc
* ������ڣ�2020��5��29��
* 
*  �汾��2.1
* ˵����LED ���Ӱ�һ���Ľӿ�
* ���ߣ�xb
* ������ڣ�2020��11��21��
*/

#include "Led.h"

static uint8 u8LEDVerL, u8LEDVerH;

//ָʾ�ư汾����ʾ����
static uint8   VerDataH,VerDataL;    //��ʾ������
static uint8   DisVerEn;   //��ʾ�汾��ʹ��  ʹ��ʱָʾ�� ��otherControl����ʾ
static uint16  TimerDisVer;  //��ʾ��˸ʱ��
static uint8   DisVerState;  //״̬

//��ʾ�汾�ŵ�״̬
#define  DISVER_STATE_BEGIN      0     //��ʼ�ر�2s
#define  DISVER_STATE_DISHW      1     //��ʾ�߸�λ
#define  DISVER_STATE_DISLW      2     //��ʾ�ߵ�λ
#define  DISVER_STATE_OFF        3     //��ʾ��λʱoff
#define  DISVER_STATE_DISsHW     4    // ��ʾ�͸�λ
#define  DISVER_STATE_DISsLW     5    // ��ʾ�͵�λ
#define  DISVER_STATE_OFF_L      6    // ��ʾ��λʱoff
#define  DISVER_STATE_OVER       7    // ���ֹͣ2s ���˳���ʾ�汾�Ź���״̬

#define  DISVER_STATE_PUASE1     8    // �߸�λ���ߵ�λ�м���ͣ��ʾ��off
#define  DISVER_STATE_PUASE2     9    // �ߵ�λ���͸�λ�м���ͣ��ʾ��off
#define  DISVER_STATE_PUASE3     10    //�͸�λ���͵�λ�м���ͣ��ʾ��off

/*******************************************************************************
 * Function:   void  LEDControl(void)
 *
 * Description:  led control
 *
 * Returns:     none
 *
 * Notes:       ����displaydata ���led
 *
*******************************************************************************/
static void LEDControl(void)
{
	//TODO: ����sDisplayData��ֵ��дLED �����
	OL_LCD = 1;
	if (sDisplayData.LEDMode > 0)
	{//����ͬģʽ����
		GPIO_BIT_EQUAL(OL_AC, sDisplayData.AC);
		GPIO_BIT_EQUAL(OL_CIRF, sDisplayData.CIRF);
		GPIO_BIT_EQUAL(OL_PTC, sDisplayData.ptc);
		GPIO_BIT_EQUAL(OL_RHEAT, sDisplayData.RHEAT);
	}
	else
	{//ȫ��Ϩ��
		GPIO_BIT_EQUAL(OL_AC, 0);
		GPIO_BIT_EQUAL(OL_CIRF, 0);
		GPIO_BIT_EQUAL(OL_PTC, 0);
		GPIO_BIT_EQUAL(OL_RHEAT, 0);
	}
}
/*******************************************************************************
 * Function: void	Led_AllClose(void)
 *           void	LED_AllOpen(void)
 * Description:  led ȫ���رգ������� ��Ҫʱʱ���ã�
 *
 * Returns:     none
 *
 * Notes:       ��Ҫ���д
 *
*******************************************************************************/
void	LED_AllClose(void)
{
	//TODO: ��дȫ�أ� ��ע��Ӱ��LCD,COM ��
	sDisplayData.LEDMode = 0;
	LEDControl();
}


void	LED_AllOpen(void)
{
	//TODO: ��дȫ�����Լ���
	sDisplayData.LEDMode = 1;
	sDisplayData.AC = 1;
	LEDControl();
}
/*******************************************************************************
 * Function:   void  LEDDisVer_Control(void)
 *
 * Description:  led ��ʾ�汾��,	�˹��ܿ���ʱ ���led�������ر�
 *              �汾����ʾ����Ϊ��1 ȫ����2s  2 ��λָʾ�� ��0.5s ��0.5s������Ϊ��λ��
 *              ֵ��3 ��λָʾ�� ��0.5s ��0.5s������Ϊ��λ��ֵ�� 4 ȫ��2s 5��������
 *
 * Returns:     NONE
 *
 * Notes:      ���ø���
*******************************************************************************/
static void  LEDDisVer_Control(void)
{
   //�汾����ʾ����
   if(DisVerEn)
   {
      switch(DisVerState)
      {
         default:
         case  DISVER_STATE_OVER:
            //2s ��ʾ���
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
            //��ʾ�ߵ�λ
            if(TimeOutChkMsLong(&TimerDisVer,500))
            {
               DisVerState = DISVER_STATE_OFF;
            }
            u8LEDVerL = 0;
            u8LEDVerH = 1;
            break;
         case  DISVER_STATE_DISHW:
            //��ʾ�߸�λ
         	if(TimeOutChkMsLong(&TimerDisVer,500))
				{
					if(VerDataH>=10)
					{
						DisVerState = DISVER_STATE_OFF;
					}
					else if (VerDataH>0)
					{//�߸���ʾ��֮����ʾ�ߵ�ʱȫ��1s
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
				//��ʾ�͵�λ
				if(TimeOutChkMsLong(&TimerDisVer,500))
				{
					DisVerState = DISVER_STATE_OFF_L;
				}
				u8LEDVerH = 0;
				u8LEDVerL = 1;
				break;
			case  DISVER_STATE_DISsHW:
				//��ʾ�ߵ�λ
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
 * Description:  led ��ʾ�������߹ر�
 *
 * Returns:     none
 *
 * Notes:       ��Ҫ����ʱ���ã���ʾ������	LEDDisVer_Control
 *              ���ܸ���
*******************************************************************************/
void  LedDisVerEn(uint16 u16VerData)
{
   if(DisVerEn)
   {
      //���������ʾ��ر�
      DisVerEn =0;
   }
   else
   { //����
      DisVerEn =1;
      DisVerState = DISVER_STATE_BEGIN;  //���뿪ʼ��ʾ�Ĺ���
      (void)TimeOutChkMsLong(&TimerDisVer,0);
      VerDataH = u16VerData/100;
      VerDataL = u16VerData%100;
   }
}
/*******************************************************************************
 * Function:   void LEDSetTask(void)
 *
 * Description:  led �����ʾ����LowerDrive- LOOP ����
 *
 * Returns:     none
 *
 * Notes:      ���Ķ�
 *
*******************************************************************************/
void LEDSetTask(void)
{
	BOOL bVerHOld, bVerLOld;
	
	LEDDisVer_Control();
	
	if(DisVerEn) 
	{
		//��ʾ������ʱ�˺���û��
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
 * Description:  ����led �汾�ŵ���ʾ״̬,��Ҫʱ���ã�
 *
 * Returns:     none
 *
 * Notes:       ���Ķ�
 *
*******************************************************************************/
BOOL  LED_GetDisVer(void)
{
	return DisVerEn;
}
