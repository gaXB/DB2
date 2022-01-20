/************************�ļ�˵��*****************************************************
* Copyright (c) 2015 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Signal_TempGet.c
* �����������¶��ź����ݵõ��ĸ�������
*
* �������� codewarrior IDE 11.1
* MCU˵���� ��
*/
/**************************���ļ�¼******************************************************
��
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
* 
* * �汾��2.0
* ˵�����ж�ĩβ������BUG ,
*   tbl_tad[u8TBLLength - 1] ������Ϊu8TBLLength��
* ���ߣ�xb
* ������ڣ�2020��7��6��
*/

#include "LibDriveDefine.h"
#include "Signal_TempGet.h"

uint16 u16Testdata;
/*const uint16  TBL_TEVP_JAC1[] =
{
	898, 864, 826, 782, 733, 681, 626, 570, 514,
	460, 408, 360, 315, 275, 239, 207, 180, 155,
	137, 120, 104, 91, 80, 70, 62
};  

const ADTGETTYPE ADT_PressTemp =
{
		TBL_TEVP_JAC1,
	   34,
	   -400,
	   50,
};*/
 ADTempType Signal_GetAdT (uint16 adcal, ADTGETTYPE *pADTemp)
{
   int16 i;
   ADTempType tBackTemp;
   const uint16 *tbl_tad = pADTemp->tbl_tad;
   const uint8 u8TBLLength = pADTemp->u8TBLLength;
   const ADTempType atOffsetTemp = pADTemp->atOffsetTemp;
   const ADTempType atAccurate = pADTemp->atAccurate;

	if (adcal >= tbl_tad[0]) 
	{
		tBackTemp = atOffsetTemp;
	}
	else if (adcal <= tbl_tad[u8TBLLength - 1]) 
	{ 
		tBackTemp = u8TBLLength * atAccurate + atOffsetTemp;
	}
	else 
	{
		for (i = 0; adcal < tbl_tad[i]; i++)
		{// ����ǰ����жϣ����� i �� 1 - u8TBLLength ֮��
		}
		if (i > 0)
		{ 
			i--; //���i������0��  ֮�� tbl[i] > adcal ;  tbl[i+1] <= adcal
		}
		else{}
		//  t   adcal   ( i*50 - 400 )      tbl_tad[i]      
		//              ((i+1)*50 -400)     tbl_tad[i+1]   
		//                                    adcal
		if (tbl_tad[i] == tbl_tad[i+1]) 
		{
			tBackTemp = (i*atAccurate) + atOffsetTemp;
		}
		else
		{                  
			tBackTemp = GAMath_LineFuni16(tbl_tad[i], tbl_tad[i+1],  (i*atAccurate + atOffsetTemp), (i*atAccurate + atOffsetTemp +atAccurate), adcal);
			//tTempCal = (adcal - tbl_tad[i]) * 50 / (tbl_tad[i+1] - tbl_tad[i]) + i * 50 -400;
		}
	}
	return tBackTemp; 
}

//��ADֵ�������Ӧ��ѹ��ֵ
 /*******************************************************************************
  * Function:  uint16 Signal_GetAd_Press(uint8 hrol,uint16 adcal)
  *
  * Description: ��ADֵ�������Ӧ��ѹ��ֵ
  * 
  * hrol  1��high   0��low  (press1 ��; press2  ��)
  *
  * Returns:     ʵ��ѹ��ֵ ��λKPa 1Bar = 0.1MPa = 100KPa
  *
  * Notes:     4.069AD - 232 = 
  *
 *******************************************************************************/
uint16 Signal_GetAd_Press(uint8 hrol,uint16 adcal)
{
	uint16 tbackPress;
	if(hrol == 0)
	{
		if(adcal < 71)
		{
			tbackPress = 0;
		}
		else if(adcal > 921)
		{
			tbackPress = 3516;
		}
		else
		{
			tbackPress = (uint16)((uint32)adcal*521/128 - 232);
		}
	}
	else
	{
		if(adcal < 102)
		{
			tbackPress = 60;
		}
		else if(adcal > 921)
		{
			tbackPress = 1025;
		}
		else
		{
			tbackPress = (uint16)((uint32)adcal*151/128 -60);
		}
	}
	return tbackPress;
}
