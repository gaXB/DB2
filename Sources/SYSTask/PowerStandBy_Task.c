/************************�ļ�˵��*****************************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�PowerStandyBy_Task.c
* ������������Դstandbyʱ��ϵͳ������
* �ṩģ�� ����ʵ��Ӧ�ñ�д
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��5��30��
*/

#include "PowerMange.h"
#include "APPDrive.h"



/*******************************************************************************
 * Function:        void PowerStandByTask(void)
 *
 * Description:     ��Դ�ȴ����ߵĹ�������
 *
 * Returns:         none
 *
 * Notes:           ��ѭ������
 *
********************************************************************************/
void PowerStandByTask(void)
{
	//����ѹ����
	//TODO: �ڴ˱�д��Դģʽ��standbyʱ��Ĵ����˴��ɽ���Դ�����л���OFF��
	//������פ��ģʽʱ����ԴOFFʱ���ȴ�CANsleep ��ʱ���л���OFF
	/*if (��פ������ʱ)
	{	
		PowerMN_AppSleepEn(0);
	}
	else
	{} */
	sPanleData.LCDONOFF = 0;
	sPanleData.AC = 0;
	sPanleData.CIRF = 0;
	sPanleData.FANLevel = 0;
	sPanleData.PTCn = 0;
	sPanleData.RHEAT = 0;
	sPanleData.VentMode = 0;
	sPanleData.VerClient = SwV_CLIENT *16 + SwsV_CLIENT;
	sPanleData.VerSelf = SwV_ *16 + SwsV_;
	sPanleData.i16Temp = SystemControl.u8TestLevel*10;
}

/*******************************************************************************
 * Function:        void PowerStandByTask_Enter(void)
 *
 * Description:     ����powerstandy  ģʽ
 *
 * Returns:         none
 *
 * Notes:           ����ʱ��pm_app�ⲿ����
 *
********************************************************************************/
void PowerStandByTask_Enter(void)
{

}
