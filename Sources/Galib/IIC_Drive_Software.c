/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�IIC_Drive_SoftWare.c
* ����������IIC ���ģ��������������ϵͳ���ʱֻ����1·IIC����
* ����IIC�Ĺ������£�
* 1 IIC �Ŀ�ʼ��ֹͣ,ACK
* 2 IIC ���ݷ��ͺͽ���
* 3 ����Э���͵ײ�MCU�ӿ��޹�
* 
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭIICģ���1.2��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��6��2��
* 
* 
*/
#include"IIC_Drive_Software.h"


//mcu �ײ�ӿں��� 
extern void IICHW_SDAWrite(uint8 bSetState);
extern BOOL IICHW_SDARead(void);
extern void IICHW_SCLWrite(uint8 bSetState);
/*************************************************************************
 *Function:     IIC_Bit_Delay()
 *
 *Description:  8M����ʱ��ʱ�����5us
 *
 *Return:       none
**************************************************************************/
static void IIC_Bit_Delay(void)
{  
	uint8 i;
	for (i=0; i < IIC_DELAY; i++)
	{
		MCU_NOP;
	}
}

/*************************************************************************
 *Function:     IIC_Start_Bit()
 *
 *Description:  I2C��ʼλ����
 *
 *Return:       no
 *************************************************************************/
void IIC_Start_Bit(void)
{
	IICHW_SDAWrite(1);
	IICHW_SCLWrite(1);
	IIC_Bit_Delay();
	
	IICHW_SDAWrite(0);
	IIC_Bit_Delay();
	IICHW_SCLWrite(0);
}

/*************************************************************************
 *Function:     IIC_Stop_Bit()
 *
 *Description:  I2Cֹͣλ����
 *
 *Return:       no
 *************************************************************************/
void IIC_Stop_Bit(void)
{
	IICHW_SDAWrite(0);
	IICHW_SCLWrite(1);
	IIC_Bit_Delay();
	
	IICHW_SDAWrite(1);
	IIC_Bit_Delay();
	IICHW_SDAWrite(0);
}

/*************************************************************************
 *Function:     IIC_Send_Ack()
 *
 *Description:  I2CӦ���źţ�data=1����Ӧ���źţ�data=0���ͷ�Ӧ���ź�
 *              
 *Return:       no
 *************************************************************************/
void IIC_Send_Ack(uint8 data) 
{
	if (data == 1) //������Ӧ�ź�
	{
		IICHW_SDAWrite(0);
	}
	else //���Ͳ���Ӧ�ź�
	{
		IICHW_SDAWrite(1);
	}
	IIC_Bit_Delay();
	
	IICHW_SCLWrite(1);
	IIC_Bit_Delay();
	
	IICHW_SCLWrite(0);
	IIC_Bit_Delay();
}

/*************************************************************************
 *Function:     IIC_Send_Byte()
 *
 *Description:  I2C��������
 *
 *Return:       1���ͳɹ���0����ʧ��
 *************************************************************************/
uint8 IIC_Send_Byte(uint8 data)
{
	uint8 i=0;
	 
   for (i=0; i < 8; i++) //��������
   {
		if ( (data<<i) & 0x80) //��������ֵ
		{
			IICHW_SDAWrite(1);
		}
		else 
		{
			IICHW_SDAWrite(0);
		}
		//ʱ�Ӹߵ�ƽʱ�����ݴ���оƬ
		IIC_Bit_Delay();
		IICHW_SCLWrite(1);
		IIC_Bit_Delay();
		IICHW_SCLWrite(0);
   }
   
   //�����������ʱ������
   IIC_Bit_Delay();
   IICHW_SDAWrite(1); //������Ӧλ
   IIC_Bit_Delay();
   IICHW_SCLWrite(1);
   IIC_Bit_Delay();
   
   if (IICHW_SDARead() == 0) 
   {
      IICHW_SCLWrite(0);
      IIC_Bit_Delay();
      return 1; 
   }
   else 
   {
      IICHW_SCLWrite(0);
      IIC_Bit_Delay();
      return  0;   
   }
}

/*************************************************************************
 *Function:     IIC_Receive_Byte()
 *
 *Description:  I2C��������
 *
 *Return:       ���ؽ��յ�����
 *************************************************************************/
uint8 IIC_Receive_Byte(void) 
{
	uint8 data=0,m=0;
  
	IICHW_SDAWrite(1);
  
	for (m = 0; m < 8; m++) //��������
	{
		IICHW_SCLWrite(0);
		IIC_Bit_Delay();
    
		IICHW_SCLWrite(1);
		IIC_Bit_Delay();
    
		data <<= 1;
		
		if (IICHW_SDARead() == 1) 
		{
			data |= 0b00000001;
		}
		else{}
  }
  
  //�������ݽ��� ��ʱ������
  IICHW_SCLWrite(0);
  IIC_Bit_Delay();
  
  return (data); //��������
}
