/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Encoder35.c
* ����������35��������ȡģ�飬�������¹���
* 1 ͨ����ȡ 35������ 3��PIN��״ֵ̬���õ����������������ģ��
* 2 �û�ͨ������ģ���ȡ����
* 
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭģ���1.1��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��7��3��
* 
*/

#include "EnCoder35.h"
#include "ComdQueue.h"

typedef struct
{
   uint8 CodeCur ;  //��ǰ��״̬
   uint8 CodeOld;  //��һ�ε�״̬
   uint8 CodeErr;
   uint8 KeyCode;
}CODE35DATA;    //�������ݽṹ


static CODE35DATA Code35Data[MAX_CODE35NUM];
static uint8 CodeTotalNum = 0;



//��Ҫ���ⲿ���õĺ�������
//���ر�������ֵ ������ abc�� ��Ӧ������ 0λ��1λ ��2λ��״̬
extern uint8 Code35HW_ReadPin(uint8 num);

 /*******************************************************************************
 * Function:   BOOL Init_Code35��uint8 num ,uint8 *CodeKeyData)
 *
 * Description:  ������1���Ƴ�ʼ��,  
 *               CodeKeyData ����(��СΪ3) Ϊ��������ֵ(��ߵ�)��ţ��ұߵļ�ֵĬ��Ϊ��ߵ�--��
 *               �������������Ϊ0����Ч��
 *
 * Returns:      TRUE ������ȷ, FALSE ���ô���
 *
 * Notes:      ��������ʼ��
 *
*******************************************************************************/
 BOOL Init_Code35(uint8 num ,uint8 *CodeKeyData)
 {
	CODE35DATA *pCodeData = Code35Data;
	BOOL bInitCode35;
	if(num == 0 || num > MAX_CODE35NUM) 
	{
		bInitCode35 = FALSE;
	}
	else
	{
		bInitCode35 = TRUE;
		CodeTotalNum = num;
    
		for(num=0; num < CodeTotalNum; num++)
		{
			if(CodeKeyData[num] != 0) 
			{
				pCodeData->KeyCode = CodeKeyData[num];
				pCodeData->CodeErr = CODE35_ERROR_NONE;
			}
			else
			{
				pCodeData->KeyCode = 0;
				pCodeData->CodeErr = CODE35_ERROR_KEYCODE;
			}
			pCodeData++;
		}
		bInitCode35 = TRUE;
	}
	
	return bInitCode35;
}
 /*******************************************************************************
 * Function:  void Treat_Read_Code35(void)
 *
 * Description: ������35����
 *
 * Returns:     none
 *
 * Notes:    �ú�������main����ѭ������,����Ϊ4ms���ϵ�Ƶ�ʣ���������ж�λ�� 		
 *
*******************************************************************************/
void Treat_Read_Code35(void)
{
	//��ȡ�絵���ص�ƽ
	uint8 num ;
	uint8 KeyCode = 0; // 1 ++ ,2--
	CODE35DATA *pCodeData = Code35Data;
	
	for (num = 0; num < CodeTotalNum; num++)
	{
		pCodeData->CodeCur = Code35HW_ReadPin(num);  //��Ӳ��ģ���ж�ȡ��ǰ�Ķ˿�״̬
      
      if (pCodeData->CodeCur != pCodeData->CodeOld && pCodeData->CodeCur != 0x07)
      {
      	//��һ����ֵʱ��Ҫ����
      	switch(pCodeData->CodeOld)
      	{
      		default:
      			pCodeData->CodeErr |= CODE35_ERROR_HARDWARE;
               break;
            case  0b00000011:
               if(pCodeData->CodeCur == 0b00000101)
   				{
   					KeyCode = 1;
   				}
   				else if (pCodeData->CodeCur == 0b00000110)
   				{
   					KeyCode =2;
   				}
   				else{}
   				break;
            case  0b00000101:
               if (pCodeData->CodeCur == 0b00000110)
               {
                  KeyCode = 1;
               }
               else if(pCodeData->CodeCur == 0b00000011)
               {
               	KeyCode =2;
               } 
               else
               {
               }
               break;
            case  0b00000110:
               if (pCodeData->CodeCur == 0b00000011)
               {
                  KeyCode = 1;
               }
               else if (pCodeData->CodeCur == 0b00000101)
               {
                  KeyCode = 2;
               }
               else{}
               break; 
         }
         if (KeyCode == 1) 
         {  //�������
         	pCodeData->CodeErr &= ~CODE35_ERROR_HARDWARE;  
         	(void)InsertComd(pCodeData->KeyCode);
         }
         else if(KeyCode == 2)
         {
         	pCodeData->CodeErr &= ~CODE35_ERROR_HARDWARE;  
         	(void)InsertComd(pCodeData->KeyCode - 1);
         }
         else{}
         
         pCodeData->CodeOld = pCodeData->CodeCur;
      //��һ����ֵʱ��Ҫ����
      }
      pCodeData++;
	}
}








