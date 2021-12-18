/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�InterLay_App.c
* ����������InterLay�� �Ļص�����ʵ�֣�����������Ҫ����ʵ����Ŀ����
* 1  InterLay�� �Ļص�����
* 2  InterlAY ����������ã� 
* 3   ��ʱ��������
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��
* 
* �汾��1.2
* ˵����������1.2��ģ��
* ԭ���ߣ�xb
* ������ڣ�2019��10��25��

* �汾��2.3
* ˵����������InterLaycan 2.3��ģ��
* ԭ���ߣ�xb
* ������ڣ�2020��1��2��
* 
* �汾��2.4
* ˵����
* 1 ����id ���ǳ���ID ����ɾ����
* 2 �������ܵ���ɾ��
* 3 ѭ������DTC ����ɾ��
* �÷����ο��ļ�
* ԭ���ߣ�xb
* ������ڣ�2020��7��14��
* 
* * �汾��2.5
* ˵����
* 1 timeout����  SignalCAN_RecTimeOut(u32Id);
* 
* ԭ���ߣ�xb
* ������ڣ�2020��7��14��
* 
*/

#include "InterLayMatrix.h"
#include "APPDrive.h"
#include "AppLayInterface.h"


volatile uint8 u8APPTest;
volatile uint8 u8Test1;

/*******************************************************************************
*   Call back functions for INTERLAY Module
*******************************************************************************/
void MP5Logic_RecTask(void);
void MP5Logic_TimeOutTask(void);

 /*******************************************************************************
 * Function:  void APPlIL_RXComplete(uint8 u8RT, uint32 u32Id)
 *
 * Description:  app ���ݷ������֪ͨapp
 *
 * Returns:      none
 *
 * Notes:     u32Id�� ���յ���CAN ID
 *        
*******************************************************************************/
void SignalCAN_RXComplete(uint32 u32Id);
void SignalCAN_RecTimeOut(uint32 u32Id);
void APPlIL_RXComplete(uint32 u32Id)
{
	//TODO: Add code to read Frame singal here
	SignalCAN_RXComplete(u32Id);
	switch(u32Id)
	{
		default: 
			break;

	}
}
/*******************************************************************************
* Function:  void APPlIL_TXComplete(uint8 u8IfId)
*
* Description:  app ���ݷ������֪ͨapp
*
* Returns:      none
*
* Notes:     u8IfId�����͵�CAN IFID
*        
*******************************************************************************/
void APPlIL_TXComplete(uint8 u8IfId)
{
	
}
/*******************************************************************************
* Function:  BOOL APPlIL_TXStart(uint8 u8IfId)
*
* Description:  app ���ݿ�ʼ���͵Ļص�����
*
* Returns:      TRUE ���Է��ͣ� FALSE ������
*
* Notes:     NONE
*        
*******************************************************************************/
BOOL APPlIL_TXStart(uint8 u8IfId)
{
	//TODO: �źŷ���ʱ���ã������Ƿ��ͣ��ɼ��ο��ļ�
	BOOL bCANSend = TRUE; //Ĭ�Ͽɿ���
	
	
	return bCANSend;
}
/*******************************************************************************
* Function:  void APPlIL_RecTimeOut(uint32 u32Id)
*
* Description:  ���ݽ��ճ�ʱ֪ͨapp
*
* Returns:      none
*
* Notes:       u32Id �� id
*           
*******************************************************************************/
void APPlIL_RecTimeOut(uint32 u32Id)
{
	SignalCAN_RecTimeOut(u32Id);
	switch(u32Id)
	{
		default:
			break;
	}
}

