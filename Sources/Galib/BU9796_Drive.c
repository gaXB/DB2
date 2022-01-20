/**************************�ļ�˵��*****************************************************
* Copyright (c) 2021 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�BU9796_Drive.c
* ����������BU9796 ����ģ�飬�������¹���
* 1  Һ�������������10���ֽڵ�LCD������ʾ
* 2  ʵ��20ms ����һ������
* 
* ��ģ��ʹ����ҪIICģ��
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xwl
* ������ڣ�2021��10��21��
* 
*/
#include"BU9796_Drive.h"
#include"timerout.h"
#include"IIC_Drive_Software.h"


static uint8* u8HtData = PDATA_NULL;         //LCD ��ָ��
static uint8 u8HtSize = BU9796_MAXLCDNUM;   //��Ҫ��������ݳ���


extern void BU9796APP_LCDSet(void);
 /*******************************************************************************
 * Function:   BU9796_Init()
 *
 * Description: ��ʼ��BU9796 ���ɷ���ϵͳ��ʼ������������������ÿ1s��ʼ��һ��
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
static void  BU9796_Init(void)
{
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(ICSET_RESET); 
	IIC_Send_Byte(MODE_SET_OFF); 	
	IIC_Stop_Bit();
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(ICSET); 
	IIC_Send_Byte(DISPLAY_CONTROL);  //��Һ������
	IIC_Send_Byte(BLINK_CONTROL);
	IIC_Send_Byte(AP_CONTROL);
	IIC_Send_Byte(MODE_SET_ON);
	IIC_Stop_Bit();

}
/*******************************************************************************
* Function:   BU9796_LCDDataSet(uint8* u8LCDData, uint8 u8Length)
*
* Description: �ϵ�����һ��
*
* Returns:     ������ȷ 1, ���� 0
*
* Notes:       none
*		
*******************************************************************************/
BOOL BU9796_LCDDataSet(uint8* u8LCDData, uint8 u8Length)
{
	uint8 i=0;
	BOOL BU9796_InitOK;
	if (u8Length > BU9796_MAXLCDNUM)
	{
		BU9796_InitOK = FALSE;
	}
	else
	{
		u8HtData = u8LCDData;
		u8HtSize = u8Length;
		BU9796_InitOK = TRUE;
	}
	BU9796_Init();
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(LCD_FIRST_ADDRESS);
	
	for (i = 0; i < 20; i++)
	{
		IIC_Send_Byte(0);
	}
	IIC_Stop_Bit();
	
	return BU9796_InitOK;
}
 /*******************************************************************************
 * Function:   BU9796_StandbyMode()
 *
 * Description: BU9796���ߣ����رմ���ʱ����
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
void  BU9796_StandbyMode(void)
{
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(ICSET);
	IIC_Send_Byte(MODE_SET_OFF); //��Һ������������
	IIC_Stop_Bit();
}

 /*******************************************************************************
 * Function:  void  BU9796_Drvie(void)
 *
 * Description: ����BU9796,��u8HtData���ݴ���9796
 *
 * Returns:     none
 *
 * Notes:       none
 *
*******************************************************************************/
static void  BU9796_Drvie(void)
{
	uint8 i=0;
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(ICSET);
	IIC_Send_Byte(DISPLAY_CONTROL);
	IIC_Send_Byte(BLINK_CONTROL);
	IIC_Send_Byte(AP_CONTROL);
	IIC_Send_Byte(MODE_SET_ON);
	IIC_Send_Byte(LCD_FIRST_ADDRESS);
	
	for (i = 0; i < u8HtSize; i++)
	{
		IIC_Send_Byte(u8HtData[i]);
	}
	IIC_Stop_Bit();
	
}

 /*******************************************************************************
 * Function:  BU9796_Updata()
 *
 * Description: ����9796 �������������и���Һ������
 *
 * Returns:     none
 *
 * Notes:       ʵ��20ms ����һ�����ݣ�1s ��ʼ��һ��
 *		
*******************************************************************************/
void  BU9796_Updata(void)
{
	static uint16  timer_int9796,timer_ref9796;	
   
//	if(TimeOutChkMsLong(&timer_int9796,1000))//1s��ʼ��һ��
//   {
//		BU9796_Init(); //��ʼ��BU9796
//   }
   
   if (TimeOutChkMsLong(&timer_ref9796, 20))
   {
   	BU9796APP_LCDSet();
   	BU9796_Drvie(); //LCD���ݴ����BU9796
   }  
   else{}
}

 /*******************************************************************************
 * Function:  BU9796_Clear()
 *
 * Description:  ����Ҫʱ����
 *
 * Returns:   none
 *
 * Notes:     ���BU9796���������,����ִ��
 *
*******************************************************************************/
void  BU9796_Clear(void)
{
	uint8 i=0;
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_ADDRESS);
	IIC_Send_Byte(ICSET); 
	IIC_Send_Byte(MODE_SET_OFF); 	
	IIC_Send_Byte(LCD_FIRST_ADDRESS);
	
	for (i = 0; i < 20; i++)
	{
		IIC_Send_Byte(0);
	}
	IIC_Stop_Bit();
}
