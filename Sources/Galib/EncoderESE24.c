/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�EncoderESE24.c
* ����������ESE24��������ȡģ�飬�������¹���
* 1 ͨ����ȡ ESE24������ 2��PIN��״ֵ̬���õ����������������ģ��
* 2 �û�ͨ������ģ���ȡ����
* 
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��7��3��
* 
*/

#include "EncoderESE24.h"
#include "ComdQueue.h"

typedef struct
{
   uint8 CodeCur ;  //��ǰ��״̬
   uint8 CodeOld;  //��һ�ε�״̬
   uint8 CodeErr;
   uint8 KeyCode;
}CODEDATAKEY;    //�������ݽṹ

static CODEDATAKEY  CodeDataKey[MAX_CODEESE24NUM];
static uint8 CodeKeyTotalNum = 0;

//��Ҫ���ⲿ���õĺ�������
//���ر�������ֵ 2���� a, b �� ��Ӧ������ 0λ��1λ��״̬
extern uint8 CodeESE24HW_ReadPin(uint8 num);
 
 /*******************************************************************************
 * Function:     BOOL Init_CodeESE24(uint8 num ,uint8 *CodeKeyData)
 *
 * Description:  ������ese24���Ƴ�ʼ��,  
 *               CodeKeyData ����(��СΪ3) Ϊ��������ֵ(��ߵ�)��ţ��ұߵļ�ֵĬ��Ϊ��ߵ�--��
 *               �������������Ϊ0����Ч��
 *
 * Returns:      TRUE ������ȷ, FALSE ���ô���
 *
 * Notes:   ��������ʼ��
 *
*******************************************************************************/
 BOOL Init_CodeESE24(uint8 num ,uint8 *CodeKeyData)
 {
	CODEDATAKEY  *pCodeData = CodeDataKey;
	BOOL bInitCKResult;
	if(num == 0 || num > MAX_CODEESE24NUM) 
	{
		bInitCKResult = FALSE;
	}
	else
	{
		bInitCKResult = TRUE;
		CodeKeyTotalNum = num;
    
		for(num = 0; num < CodeKeyTotalNum; num++)
		{
			if(CodeKeyData[num] !=0) 
			{
				pCodeData->KeyCode = CodeKeyData[num];
				pCodeData->CodeErr = CODEESE24_ERROR_NONE;
			}
			else
			{
				pCodeData->KeyCode = 0;
				pCodeData->CodeErr = CODEESE24_ERROR_KEYCODE;
			}
			pCodeData++;
		}
	}
	
	return bInitCKResult;
 }
 
 
 /*******************************************************************************
 * Function:  void Treat_Read_CodeESE24(void)
 *
 * Description: ������ESE24 ����
 *
 * Returns:     none
 *
 * Notes:    �ú�������main����ѭ������
 *
*******************************************************************************/
void Treat_Read_CodeESE24(void)
{
	//��ȡ�絵���ص�ƽ
	uint8 num ;

	CODEDATAKEY *pCodeData=CodeDataKey;
	
	for(num=0; num < CodeKeyTotalNum; num++)
	{
		pCodeData->CodeCur = CodeESE24HW_ReadPin(num);  //��Ӳ��ģ���ж�ȡ��ǰ�Ķ˿�״̬
		
		if (pCodeData->CodeCur == 1 && pCodeData->CodeCur != pCodeData->CodeOld)
		{
			(void)InsertComd(pCodeData->KeyCode);
		}
		else  if (pCodeData->CodeCur==2 && pCodeData->CodeCur!=pCodeData->CodeOld)
		{
			(void)InsertComd(pCodeData->KeyCode-1);
		}
		else{}
		
		pCodeData->CodeOld = pCodeData->CodeCur;

		pCodeData++;
   }
    
}
