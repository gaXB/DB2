/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ComdQueue.c
* �������������� ģ��
* 
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭ����ģ���1.0��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��4��29��
* 
* 
*/
#include "ComdQueue.h"


static   uint8  QueueComd[MAX_COMD_NUM];
static   uint8  QueueFront;      //��Ԫ���±�
static   uint8  QueueRear;       //βԪ���±�




/*******************************************************************************
 * Function:   void InitComdQueue(void)
 *
 * Description:  ������г�ʼ��
 *
 * Returns:    None
 *
 * Notes:    ������ݣ�����Ҫʱ����
 *
*******************************************************************************/
void InitComdQueue(void)
{
   QueueFront = 0;
   QueueRear  = 0;
}


 /*******************************************************************************
 * Function:   BOOL InsertComd(uint8 Comd)
 *
 * Description:  ����������
 *
 * Returns:    ����ʧ�� FALSE,�ɹ�TRUE;
 *
 * Notes:   ���������ʱ����
 *
*******************************************************************************/
BOOL  InsertComd(uint8 Comd)
{
   uint8 bInitOK;
	if(Comd == COMD_NONE)
   {
		bInitOK = FALSE;
   }
	else
	{
	   QueueComd[QueueRear++] = Comd;

	   if (QueueRear >= MAX_COMD_NUM)
	   {
	      QueueRear = 0;
	   }
	   else{}
	   
	   if (QueueRear == QueueFront)
	   {//��������������ȥ��һ�����������
	   	QueueFront++;
	   	if (QueueFront >= MAX_COMD_NUM)
	   	{
	   		QueueFront = 0;
	   	}
	   	else{}
	   }
	   else{}
	   
	   bInitOK = TRUE;
	}


   return bInitOK;
}



 /*******************************************************************************
 * Function:   BOOL InsertComd(uint8 Comd)
 *
 * Description:  ���������
 *
 * Returns:    ���˷��� TRUE;
 *
 * Notes:   Ӧ�ò���ã�һ����ڰ��������д���
 *
*******************************************************************************/
uint8  ReadComd(void)
{
   uint8   u8Comd;

   if (QueueFront != QueueRear)
   {//������������
   	u8Comd = QueueComd[QueueFront++];
      if (QueueFront >= MAX_COMD_NUM)
      {
         QueueFront = 0;
      }
      else{}
   }
   else
   {
   	u8Comd = COMD_NONE;
   } 
   
   return u8Comd;
}







