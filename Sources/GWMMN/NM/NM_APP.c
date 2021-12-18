/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�NM_APP.c
* ����������CAN ͨѶ�Ƿ�ʹ�ܹ���ģ�飬ΪӦ�ò㣬��Ϊģ�壬�����ø���
* �����ڽӼ��osek���������ʹ�ô��ļ�
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��

* �汾��1.1 
* ˵����֧��autosar
* ԭ���ߣ�xb
* ������ڣ�2019��12��24��
*/


#include "NM_APP.h"
#include "CanDrive.h"

static uint8 bCANCOMRecEn, bCANCOMSendEn;  //Ӧ�ñ��Ľ���ʹ��
static uint8 bCANCOMDelay;
static uint16 TimertApp;

/*******************************************************************************
 * Function:       void Com_RxStart(void)  void Com_RxStop(void)
 *                 void Com_TxStart(void)   void Com_TxStop(void)
 * Description:     com ���ͣ����� ʹ�ܹر�
 *
 * Returns:         none
 *
 * Notes:          Ӧ�ò���� 
 *
********************************************************************************/
void Com_RxStart(void)
{
	bCANCOMRecEn = 1;
}

void Com_RxStop(void)
{
	bCANCOMRecEn = 0;
}

void Com_TxStart(void)
{
	bCANCOMSendEn = 1;
}

void Com_TxStop(void)
{
	bCANCOMSendEn = 0;
}
/*******************************************************************************
 * Function:       void Com_TxEnDealy(void)
 *                
 * Description:     com tx ������ʱ
 *
 * Returns:         none
 *
 * Notes:          Ӧ�ò���� 
 *
********************************************************************************/
void Com_TxEnDealy(void)
{
	bCANCOMDelay = 1;
	(void)TimeOutChkMsLong(&TimertApp, 0);
}

/*******************************************************************************
 * Function:      uint8 ComGetTxEn(void)
 *                
 * Description:   com ����ʹ��ֵ ������
 *
 * Returns:         none
 *
 * Notes:          ��������� 
 *
********************************************************************************/
uint8 AutoSarNM_TxEn(void);
uint8 ComGetTxEn(void)
{
	if (bCANCOMDelay)
	{
		if (TimeOutChkMsLong(&TimertApp, APPSEND_DELAY_TIME))
		{
			bCANCOMDelay = 0;
		}
		return 0;
	}
#if _NEED_AUTOSARNM_ == 0
	return bCANCOMSendEn;
#else
	return (AutoSarNM_TxEn() && bCANCOMSendEn);
#endif
	
}

