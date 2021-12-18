/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�mcp7708.c
* ����������HT16K23 ����ģ�飬�������¹���
* 1  Һ�������������10���ֽڵ�LCD������ʾ
* 2  ʵ��20ms ����һ�����ݣ�1s ��ʼ��һ��
* 3  ������ȡ����
* 
* ��ģ��ʹ����ҪIICģ��
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭģ���1.0��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��6��2��
* 
* �汾��1.1
* ˵������������size ���ܸ��ĵ�����
* ԭ���ߣ�xb
* ������ڣ�2020��6��2��
*/
#include"Ht16k23_Drive.h"
#include"timerout.h"
#include"IIC_Drive_Software.h"


static uint8* u8HtData = PDATA_NULL;         //LCD ��ָ��
static uint8 u8HtSize = HT16K23_MAXLCDNUM;   //��Ҫ��������ݳ���


extern void Ht16k23APP_LCDSet(void);
 /*******************************************************************************
 * Function:   Ht16k23_Init()
 *
 * Description: ��ʼ��16k23 ���ɷ���ϵͳ��ʼ������������������ÿ1s��ʼ��һ��
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
static void  Ht16k23_Init(void)
{
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(MODE_SEGMENGT); //����ģʽ������жϲɼ�����
	IIC_Stop_Bit();
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(NORMAL_DISPLAY_ON); //��Һ������
	IIC_Stop_Bit();
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(KEY_SCAN_PERIOD); //���ð���ɨ������
	IIC_Stop_Bit();
}
/*******************************************************************************
* Function:   Ht16k23_LCDDataSet(uint8* u8LCDData, uint8 u8Length)
*
* Description: �ϵ�����һ��
*
* Returns:     ������ȷ 1, ���� 0
*
* Notes:       none
*		
*******************************************************************************/
BOOL Ht16k23_LCDDataSet(uint8* u8LCDData, uint8 u8Length)
{
	BOOL bHt16k23_InitOK;
	if (u8Length > HT16K23_MAXLCDNUM)
	{
		bHt16k23_InitOK = FALSE;
	}
	else
	{
		u8HtData = u8LCDData;
		u8HtSize = u8Length;
		bHt16k23_InitOK = TRUE;
	}
	Ht16k23_Init();
	
	return bHt16k23_InitOK;
}
 /*******************************************************************************
 * Function:   Ht16k23_StandbyMode()
 *
 * Description: 16k23���ߣ����رմ���ʱ����
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
void  Ht16k23_StandbyMode(void)
{
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(STANDBY_DISPLAY_OFF); //��Һ������������
	IIC_Stop_Bit();
}

 /*******************************************************************************
 * Function:  void  Ht16k23_Drvie(void)
 *
 * Description: ����16k23,��u8HtData���ݴ���16k23
 *
 * Returns:     none
 *
 * Notes:       none
 *
*******************************************************************************/
static void  Ht16k23_Drvie(void)
{
	uint8 i=0;
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(LCD_FIRST_ADDRESS);
	
	for (i = 0; i < u8HtSize; i++)
	{
		IIC_Send_Byte(u8HtData[i]);
	}
	IIC_Stop_Bit();
}

 /*******************************************************************************
 * Function:    Ht16k23_Read_Key()
 *
 * Description: ��Read_KeyBoard_Signal�����е���
 *
 * Returns:     ����ֵ
 *
 * Notes:       K19-K0���ݷֱ����key_data��19-0λ
 *		
*******************************************************************************/
void  Ht16k23_Read_Key(uint32* pKeyData)
{
	static uint8	INT_SIGN,key_data[3];
	static uint16 timer_key16k23;
	 
	if (TimeOutChkMsLong(&timer_key16k23, 40)) //40ms��ȡһ��
	{
		IIC_Start_Bit();
		IIC_Send_Byte(SLAVE_WRITE);
		IIC_Send_Byte(INT_REG_ADDRESS); 
		IIC_Stop_Bit();
		
		IIC_Start_Bit();
		IIC_Send_Byte(SLAVE_READ);
		INT_SIGN=IIC_Receive_Byte(); //�������жϱ��
		IIC_Send_Ack(ACK_NO);
		IIC_Stop_Bit();
			 
		if(INT_SIGN!=0)
		{
			INT_SIGN =0; //�尴���жϱ��  
			
			IIC_Start_Bit();
			IIC_Send_Byte(SLAVE_WRITE); //����дģʽ
			IIC_Send_Byte(KEY_FRIST_ADDRESS); //д�������ݼĴ����׵�ַ0x20
			IIC_Stop_Bit();
			
			IIC_Start_Bit();
			IIC_Send_Byte(SLAVE_READ); //���ö�ģʽ
			key_data[0]=IIC_Receive_Byte(); //����ַΪ0x20�İ���ֵ8��
			IIC_Send_Ack(ACK_YES);
			key_data[1]=IIC_Receive_Byte(); //����ַΪ0x21�İ���ֵ8��
			IIC_Send_Ack(ACK_YES);
			key_data[2]=IIC_Receive_Byte(); //����ַΪ0x22�İ���ֵ4��
			IIC_Send_Ack(ACK_NO);
			IIC_Stop_Bit();
		}
		else 
		{//�ް�����Ӧʱ�尴������
			key_data[0] = 0;
			key_data[1] = 0;
			key_data[2] = 0;
		}
	}
	 
	//�����鰴��ֵ��������key_data�У�k0-k7����key_data[0]��k8-k15����key_data[1]��k16-k19����key_data[2]
	*pKeyData = ((uint32)key_data[2] << 16) + ((uint32)key_data[1] << 8) + key_data[0];
}


 /*******************************************************************************
 * Function:  Ht16k23_Updata()
 *
 * Description: ����16k23 �������������и���Һ������
 *
 * Returns:     none
 *
 * Notes:       ʵ��20ms ����һ�����ݣ�1s ��ʼ��һ��
 *		
*******************************************************************************/
void  Ht16k23_Updata(void)
{
	static uint16  timer_int16k23,timer_ref16k23;	
   
	if(TimeOutChkMsLong(&timer_int16k23,1000))//1s��ʼ��һ��
   {
		Ht16k23_Init(); //��ʼ��Ht16k23
   }
   
   if (TimeOutChkMsLong(&timer_ref16k23, 20))
   {
   	Ht16k23APP_LCDSet();
   	Ht16k23_Drvie(); //LCD���ݴ����ht16k23
   }  
   else{}
}

 /*******************************************************************************
 * Function:  Ht16k23_Clear()
 *
 * Description:  ����Ҫʱ����
 *
 * Returns:   none
 *
 * Notes:     ���16k23���������,����ִ��
 *
*******************************************************************************/
void  Ht16k23_Clear(void)
{
	uint8 i=0;
	
	IIC_Start_Bit();
	IIC_Send_Byte(SLAVE_WRITE);
	IIC_Send_Byte(LCD_FIRST_ADDRESS);
	
	for (i = 0; i < u8HtSize; i++)
	{
		IIC_Send_Byte(0);
	}
	IIC_Stop_Bit();
}
