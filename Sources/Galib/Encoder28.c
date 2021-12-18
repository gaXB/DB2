/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Encoder28.c
* ����������28��������ȡģ�飬�������¹���
* 1 ͨ����ȡ 28������ 2��PIN��״ֵ̬���õ�28�����������������ģ��
* 2 �û�ͨ������ģ���ȡ����
* 
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭģ���0.0.0��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��5��24��
* 
*/
#include "Encoder28.h"


typedef struct
{
   uint8 CodeCur ;  //��ǰ��״̬
   uint8 CodeOld;  //��һ�ε�״̬
   uint8 CodeErr;
   uint8 KeyCode;
}CODEDATA28;    //�������ݽṹ

static CODEDATA28 CodeData28[MAX_CODE28NUM];
static uint8 Code28TotalNum = 0;

//��Ҫ���ⲿ���õĺ�������
//���ر�������ֵ 2���� a, b �� ��Ӧ������ 0λ��1λ��״̬
extern uint8 Code28HW_ReadPin(uint8 num);
 
 /*******************************************************************************
 * Function:   BOOL Init_Code28(uint8 num ,uint8 *CodeKeyData)
 *
 * Description:  ������28���Ƴ�ʼ��,  
 *               CodeKeyData ����(��СΪ3) Ϊ��������ֵ(��ߵ�)��ţ��ұߵļ�ֵĬ��Ϊ��ߵ�--��
 *               �������������Ϊ0����Ч��
 *
 * Returns:      TRUE ������ȷ, FALSE ���ô���
 *
 * Notes:   ��������ʼ��
 *
*******************************************************************************/
 BOOL Init_Code28(uint8 num ,uint8 *CodeKeyData)
 {
	CODEDATA28  *pCodeData = CodeData28;
   BOOL bInitCode28; 
	if(num==0 || num>MAX_CODE28NUM) 
	{
		bInitCode28 = FALSE;
		Code28TotalNum = 0;
	}
	else
	{
		bInitCode28 = TRUE;
		Code28TotalNum = num;
    
		for(num=0; num < Code28TotalNum; num++)
		{
			if(CodeKeyData[num] != 0) 
			{
				pCodeData->KeyCode = CodeKeyData[num];
				pCodeData->CodeErr = CODE28_ERROR_NONE;
			}
			else
			{
				pCodeData->KeyCode = 0;
				pCodeData->CodeErr = CODE28_ERROR_KEYCODE;
			}
			pCodeData++;
		}
	}
	
	return bInitCode28;
 }
 
 
 /*******************************************************************************
 * Function:  void Read_Code28(void)
 *
 * Description: ������28����
 *
 * Returns:     none
 *
 * Notes:    �ú�������main����ѭ������,����Ϊ4ms���ϵ�Ƶ�ʣ�Ҳ�ɷ����ж�λ�� 		
 *
*******************************************************************************/
void Treat_Read_Code28(void)
{
	//��ȡ�絵���ص�ƽ
	uint8 num;
	uint8 KeyCode = 0; // 1 ++ ,2--
	uint8 ReadCode;
	CODEDATA28 *pCodeData = CodeData28;
	
	for(num = 0; num < Code28TotalNum; num++)
	{
      ReadCode = Code28HW_ReadPin(num);     //��Ӳ��ģ���ж�ȡ��ǰ�Ķ˿�״̬
      pCodeData->CodeCur = ReadCode&0x01;  //a �ŵĵ�ѹ��Ҫ֪����һ�ε�ֵ��b�Ų���Ҫ
      ReadCode &= 0x02;                    //b �ŵ�״̬
      
      if (pCodeData->CodeCur != pCodeData->CodeOld)
      {
         //��һ����ֵʱ��Ҫ����
      	switch (pCodeData->CodeCur)
         {
            default:
               pCodeData->CodeErr |=CODE28_ERROR_HARDWARE;
               break;
            case  0:
               if (ReadCode)
   				{
               	KeyCode = 1;
   				}
   				else
   				{
   					KeyCode = 2;
   				}
   				break;
            case  1:
               if (ReadCode)
               {
                  KeyCode = 2;
               }
               else
               {
               	KeyCode = 1;
               } 
               break;
         }
         if (KeyCode == 1) 
         {  //�������
            pCodeData->CodeErr &= ~CODE28_ERROR_HARDWARE;  
            (void)InsertComd(pCodeData->KeyCode);
         }
         else if (KeyCode == 2)
         {
         	pCodeData->CodeErr &= ~CODE28_ERROR_HARDWARE;  
         	(void)InsertComd(pCodeData->KeyCode-1);
         }
         pCodeData->CodeOld = pCodeData->CodeCur;
      //��һ����ֵʱ��Ҫ����
      }
      pCodeData++;
   }
}
