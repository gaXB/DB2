/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CommSet.c
* ���������� CAN LIN �ź����ã��� CommSetTask()��ʵ��
* 1 100ms����һ�Σ��������⹦����Ҫ��InterLay_App.c��ʵ��
* �������� codewarrior IDE 11.1
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
*/
#include "CommSet.h"
#include "APPDrive.h"
#include "OutANDDisplay.h"
#if _PROJECT_CAN_
#include "InterLayMatrix.h"
#endif



/*******************************************************************************
 * Function:        void CommSetTask(void)
 *
 * Description:     �ź����ã��ײ�����loop���� 
 *
 * Returns:        none
 *
 * Notes:           		
 *
********************************************************************************/
void CommSetTask(void)
{
	static uint16 TimerSetMessage;
	
	if (TimeOutChkMsLong(&TimerSetMessage, 10))
	{//TODO: �ź�ֵ�����ڴ˴���д

	}
	else{}
	
	
	//ʵ�ֻػ�ģʽ���л��������ͼ�ģ��
}
