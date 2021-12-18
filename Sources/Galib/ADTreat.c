/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ADTreat.c
* ����������AD ģ��ɼ�����ת������ֵ���ж��Ƿ����
* 1  AD �ɼ�ʹ���ж�ģʽ
* 2  ��������ת������3·��ѹ��8·�¶�
* 3  �ȴ�ģʽ�ɼ�һ·����ѹ�Ĺ���
* 
* ��ģ��ʹ����Ҫ��ʱ��ģ�鼰��ѧ��������ģ��
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭADģ���1.2.6��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��5��14��
* 
* �汾��1.1
* �޸� BOOL ADInit(uint8 u8MaxNum, uint8 u8VoltRegisterType) �����ò�ͬ�ĵ���ֵ����
* 0 : 10/3.6 ��Ϊ 1 : 100/33 
* ���ߣ�xb
* ������ڣ�2020��3��28��
*/


#include "ADTreat.h"
#include "mathGA.h"


//AD������������
typedef struct
{
	uint16 AdVData;          		  //�˲����ֵ
	uint16 AdCData;         		  //��ǰ�ɼ�ֵ
	uint16 AdVBuff[AD_FILTER_NUM];    //buffer	
}ADDATA;

//ʵ������������������
typedef struct
{
	int16 Value;               //�¶�ֵ,���ߵ�ѹ
	const uint16  *TBL_TempAD;  //�¶ȱ���ѹʱ����
	uint8 ErrCode;             //������
}ADPHYDATA;

//AD ����������ֵ������������ݶ���
static ADDATA sADData[AD_MAXNUM];
static ADPHYDATA sADPhyData[ADVOLT_MAXNUM+ADTEMP_MAXNUM];

static uint8 u8ADTotalNum, u8ADTUseNum, u8ADVUseNum;  //ʵ������
static uint8 u8SupVoltMode;          //0 12v, 1:24v

static uint8 u8RBuffTimes;           //��ȡbuff ���� ��8���˲�
static uint8 u8ConverState;          //ADת��״̬
static uint8 u8AdPollPort;           //��ѯ�ɼ�ֵ

static uint8 u8ADModuleErrCode, u8ADErrChanle;   //�����뼰����ͨ��
static uint16 u16HWBackData;        // Ӳ���ɼ�ֵ
static uint8 u8RegisterType = 0;         // 1: 100k 30k��ѹ     0:10k/3.6k
 /**************************************************
 ͨ��Ӳ��������Ҫ���õĺ�������
 ****************************************************/
//�ⲿ���õĺ���
extern void   ADCH_SEL(uint8 adid);
//��ģ��ʵ�ֵĺ��������ޱ�ģ��ʹ��
static void Deal_ADToVolt(ADPHYDATA *pVoltSignal, uint16 u16ADVolt);
static ADTempType Get_Ad_T (uint16 adcal, const uint16 *tbl_tad);
static void Deal_ADToTemp(ADPHYDATA *pTempSignal, uint16 u16ADValue);
/*******************************************************************************
* Function:      BOOL ADInit(uint8 u8MaxNum)
*
* Description:   adģ���ʼ���� ʹ�õ����AD����
*
*  parameter ��    u8MaxNum ���֧������Ϊ25�� 
* return :       ������ȷTRUE

* Note  :       �ϵ��ʼ��ʱ����

************************************************************************************/
BOOL ADInit(uint8 u8MaxNum, uint8 u8VoltRegisterType)
{
	BOOL bADInitResult;
	if (u8MaxNum > AD_MAXNUM)
	{
		bADInitResult = FALSE;
		u8ADTotalNum = 0;
	}
	else
	{
		bADInitResult = TRUE;
		u8ADTotalNum = u8MaxNum;
		u8ConverState = ADCONVER_STATE_INIT;  
		u8ADModuleErrCode = 0;
		u8AdPollPort = 0;
		u8RBuffTimes = 0;
	}
	
	u8RegisterType = u8VoltRegisterType;
	return bADInitResult;
}
/*******************************************************************************
* Function:      BOOL ADPhy_Init(uint8 u8VNum, uint8 u8IgnMode, uint8 u8TNum, const uint16** pTBL)
*
* Description:   adģ����������ת�����ܳ�ʼ��
*
* paramter ��   u8VNum��  ��ѹ��֧�����������3
*             u8IgnMode��  ϵͳ��ѹת����ʽ�� 1��24v�� 0��12v
*             u8TNum�� ��Ҫ�����¶�ת�������������8
*             **pTBL�� �¶ȱ������
*             
* return :     ������ȷTRUE; 
* 
* Note  :      �ϵ��ʼ��ʱ����
*
************************************************************************************/
BOOL ADPhy_Init(uint8 u8VNum, uint8 u8IgnMode, uint8 u8TNum, const uint16** pTBL)
{
	BOOL bADVInitResult = TRUE;
	uint8 u8i;
	
	if (u8VNum > ADVOLT_MAXNUM || u8TNum > ADTEMP_MAXNUM || u8IgnMode >= ADVOLT_MODE_INVAILED)
	{
		bADVInitResult = FALSE;
		u8ADTUseNum = 0;
		u8ADVUseNum = 0;
	}
	else
	{
		u8ADTUseNum = u8TNum;
		u8ADVUseNum = u8VNum;
		u8SupVoltMode = u8IgnMode;
		
		for (u8i = 0; u8i < u8ADTUseNum; u8i++)
		{
			if (pTBL[u8i] == PDATA_NULL)
			{
				bADVInitResult = FALSE;
			}
			else
			{
				sADPhyData[u8i +ADID_TEMP_BEGIN].TBL_TempAD = pTBL[u8i];
			}
		}
	}
	return bADVInitResult;
}

/*******************************************************************************
* Function:       uint16  ADValue_Get(uint8 adid, ADValueGetType u8ADGetType)
*                
*           
* Description:    adid�� ��Ҫ�ɼ�ad��ID�����ش��źŵ�ad��
*                 u8ADGetType �� ADֵ ���ͣ� ��enum ADValueGetType
*                 
* return :         0---255

* Note  :         ��adid����ʱ ������0

************************************************************************************/
uint16  ADValue_Get(uint8 adid, ADValueGetType u8ADGetType)
{
   uint16 u16AD;
   
   if(adid >= u8ADTotalNum || u8ADGetType >= ADVALUE_GETTYPE_INVAILED)
   {
   	u16AD = 0;
   }
   else
   {
      if (u8ADGetType == ADVALUE_GETTYPE_8AD || u8ADGetType == ADVALUE_GETTYPE_10AD)
      {
      	u16AD = sADData[adid].AdVData;
      }
      else
      {
      	u16AD = sADData[adid].AdCData;
      }
      
      if (u8ADGetType == ADVALUE_GETTYPE_8AD || u8ADGetType == ADVALUE_GETTYPE_8ADFAST)
      {//��8λ ��λ����
   		if ((u16AD & 0x3) == 0x03)
   		{
   			u16AD++;
   		}else{}
   		
   		if (u16AD >= 1023)
   		{
   			u16AD = 1023;
   		}else{}
   		u16AD >>=2;
      }
      else{}
   }
   
   return  (u16AD);
}
/*******************************************************************************
* Function:        int16 ADPhy_GetFact(uint8 adid)
*						 
* Description:     ����adidͨ����ʵ������ֵ��
* 
* return :         �¶� -40--80  ʵ���¶ȣ�data/10  
                                      ��ѹ  0- 8000  ʵ�ʵ�ѹ�� data/100    ���ȣ� 0.05v
                   
* Note  :      

************************************************************************************/
int16 ADPhy_GetFact(uint8 adid)
{
	int16 i16Value;
	if(adid >= (ADTEMP_MAXNUM + ADVOLT_MAXNUM))
	{
		i16Value = 0;
	}
	else
	{
		i16Value = sADPhyData[adid].Value;
	}
	
	return i16Value;
}

/*******************************************************************************
* Function:        uint8 ADPhy_GetErr(uint8 adid)
*						 
* Description:     ����adidͨ���Ĺ�����
* 
* return :         ADPHY��������  ADPHYERR_XXX
                   
* Note  :      

************************************************************************************/
uint8 ADPhy_GetErr(uint8 adid)
{
	uint8 u8ErrCode;
	if(adid >= (ADTEMP_MAXNUM + ADVOLT_MAXNUM))
	{
		u8ErrCode = 0xff;
	}
	else
	{
		u8ErrCode = sADPhyData[adid].ErrCode;
	}
	
	return u8ErrCode;
}
/*******************************************************************************
* Function:     uint8 ADModule_ErrCode (void)
*
* Description:    ���ش�ģ��Ĺ�����
* 
* return :         
* Note  :      
************************************************************************************/
uint8 ADModule_ErrCode (void)
{
	return u8ADModuleErrCode;
}

/*******************************************************************************
* Function:       uint8 ADModule_Test(ADTESTDATA* pTestTable)
*
* Description:    ģ�����
* parameter :     ��ADֵ���������� ���ص� pTestTable
* return :        ���ع�����  
* Note  :      
************************************************************************************/
uint8 ADModule_Test(ADTESTDATA* pTestTable)
{
	uint8 u8ADIDNum;
	ADDATA *pADData = sADData;
	ADPHYDATA *pPhyData = sADPhyData;
	ADTESTDATA* pADTest = pTestTable;
	
	for (u8ADIDNum = 0; u8ADIDNum < u8ADTotalNum; u8ADIDNum++)
	{
		pADTest->u16ADValue = pADData->AdVData;
		pADTest->i16PhyValue = pPhyData->Value;
		
		pPhyData++;
		pADData++;
		pADTest++;
	}

	return u8ADModuleErrCode;		
}

/*******************************************************************************
* Function:        void ADHWEnd_Interrupt(uint16 u16ReadResult)
*
* Description:    ad �ж�ʱ��ͨ���˺�����adֵ������ģ��
*
* parameter :  u16ReadResult ������ADֵ
* 
* return :       none
* Note   :       AD�жϺ�������
************************************************************************************/
void ADHWEnd_Interrupt(uint16 u16ReadResult)
{
	u16HWBackData = u16ReadResult;
	if (u8ConverState == ADCONVER_STATE_INTREAD)
	{
		if (u8AdPollPort < u8ADTotalNum)
		{
			sADData[u8AdPollPort++].AdCData = u16HWBackData;
			
			if (u8AdPollPort >= u8ADTotalNum)
			{//����ת�����
				u8ConverState = ADCONVER_STATE_INTOK;
			}
			else
			{
				ADCH_SEL(u8AdPollPort);
			}
		}
		else
		{
			u8ADModuleErrCode |= ADMODULE_ERRCODE_INT;
		}
	}
	else if (u8ConverState == ADCONVER_STATE_WAITREAD)
	{
		u8ConverState = ADCONVER_STATE_WAITOK;
	}
	else
	{
		u8ADModuleErrCode |= ADMODULE_ERRCODE_INT;
	}
}

/*******************************************************************************
 * Function:   static void ADConverPhysic(void)
 *
 * Description: ��AD ֵ��ת��Ϊ����ʵ������
 *
 * Returns:     none
 *
 * Notes:   
 *
*******************************************************************************/
static void ADConverPhysic(void)
{
	uint8 u8ADIDNum;
	ADDATA *pADData;
	ADPHYDATA *pPhyData;
	
	pADData = sADData;
	pPhyData = sADPhyData;
	for (u8ADIDNum = 0; u8ADIDNum < u8ADVUseNum; u8ADIDNum++)
	{
		Deal_ADToVolt(pPhyData, pADData->AdVData);
		pADData++;
		pPhyData++;
	}
	
	pADData = &sADData[ADID_TEMP_BEGIN];
	pPhyData = &sADPhyData[ADID_TEMP_BEGIN];
	for (u8ADIDNum = ADID_TEMP_BEGIN; u8ADIDNum < (ADID_TEMP_BEGIN + u8ADTUseNum); u8ADIDNum++)
	{
		Deal_ADToTemp(pPhyData, pADData->AdVData);
		pADData++;
		pPhyData++;
	}
}


/*******************************************************************************
 * Function:   static void ADC_INT_Begin(void)
 *
 * Description: �жϲɼ�����ʼ
 *
 * Returns:     none
 *
 * Notes:   
 *
*******************************************************************************/
static void ADC_INT_Begin(void)
{
	u8AdPollPort = 0;
	ADCH_SEL(0);
	u8ConverState = ADCONVER_STATE_INTREAD;
}
/*******************************************************************************
 * Function:   void Treat_Read_Ad(void)
 *
 * Description: �ɼ�����AD������ad�������ƽ���������������������
 *
 * Returns:     none
 *
 * Notes:   �˴�ΪAD�ɼ����Ƶĺ��ĺ�����ÿ1ms�ɼ�1������
 *          �ú�������main�����е�������ѭ������
 *
 *
*******************************************************************************/
void Treat_Read_Ad(void)
{ 
   static uint16 TimerADC;
   static uint8 u8UNCompleteTimes;
   ADDATA *pADData;
   uint8 u8i, u8j;
   
	if (TimeOutChkMsLong(&TimerADC, ADTREAT_READTIME))
	{
		switch (u8ConverState)
		{
			default:
			case ADCONVER_STATE_INIT:
				u8RBuffTimes = 0;
				ADC_INT_Begin();
				break;
			case ADCONVER_STATE_INTREAD:
				//δת�����,���¿�ʼת��
				u8UNCompleteTimes++;
				if (u8UNCompleteTimes >= ADTREAT_UNCOMPLETE_TIMES)
				{//2�ζ�ʧ�ܣ����¼����
					u8UNCompleteTimes = 0;
					u8RBuffTimes = 0;
					u8ADModuleErrCode |= ADMODULE_ERRCODE_UNCOMPETE;
					u8ADErrChanle = u8AdPollPort;
					ADC_INT_Begin();
				}
				break;
			case ADCONVER_STATE_INTOK:
				//ת�����
				//���˵���ADֵ����ad����
				u8UNCompleteTimes = 0;
				pADData = sADData;
				for (u8i = 0; u8i < u8ADTotalNum; u8i++)
				{
					pADData->AdVBuff[u8RBuffTimes] = pADData->AdCData;
					pADData++;
				}
				
				u8RBuffTimes++;
				
				if (u8RBuffTimes >= AD_FILTER_NUM)
				{//8���˲���ɣ�������ƽ��������������ת��
					u8RBuffTimes = 0;
					pADData = sADData;
					for (u8i = 0; u8i < u8ADTotalNum; u8i++)
					{
						pADData->AdVData = 0;
						for (u8j = 0; u8j < AD_FILTER_NUM; u8j++)
						{
							pADData->AdVData += pADData->AdVBuff[u8j];
						}
						pADData->AdVData /= AD_FILTER_NUM;
						pADData++;
					}
					//����ת��
					ADConverPhysic();
				}
				else{}
				ADC_INT_Begin();//��ʼ�»����ת��
				break;

		}
	}
	else
	{}
}
/*******************************************************************************
* Function:        static uint8 Get_Ad_T ( uint8 adcal, const uint8*tbl_tad)
*
* Description:    ͨ��adֵ�õ��¶�.
                   adcal :��ǰ��ad��   *tbl_tad�� ��Ӧ���¶ȱ�
* return :       -40�ȵ�80��  i -400---800
* Note  :        �¶Ȳɼ�ͨ�����ɼ�������Ϊ0.1��
                 5��һ��
************************************************************************************/
static ADTempType Get_Ad_T (uint16 adcal, const uint16 *tbl_tad)
{
   int16 i;
   ADTempType tBackTemp;
   

	if (adcal >= tbl_tad[0]) 
	{
		tBackTemp = -400;
	}
	else if (adcal <= tbl_tad[24]) 
	{ 
		tBackTemp = 800;
	}
	else 
	{
		for (i = 0; adcal < tbl_tad[i]; i++)
		{// ����ǰ����жϣ����� i �� 1 - 24 ֮��
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
			tBackTemp = (i*50) - 400;
		}
		else
		{                  
			tBackTemp = GAMath_LineFuni16(tbl_tad[i], tbl_tad[i+1],  (i*50 - 400), (i*50 - 350), adcal);
			//tTempCal = (adcal - tbl_tad[i]) * 50 / (tbl_tad[i+1] - tbl_tad[i]) + i * 50 -400;
		}
	}
	return tBackTemp; 
}
//*/
/*******************************************************************************
 * Function:   static void Deal_TempSignal(ADDATA *pTempSignal)
 *
 * Description: ����  pTempSignal �ĵ�ѹ�ź�ֵ  ���ж����Ƿ����
 *
 * Returns:     none
 *
 * Notes:
 *
*******************************************************************************/
static void Deal_ADToTemp(ADPHYDATA *pTempSignal, uint16 u16ADValue)
{
	pTempSignal->Value = Get_Ad_T(u16ADValue, pTempSignal->TBL_TempAD);    

   //�õ�������
   if (u16ADValue >= AD_TEMPERR_OPEN)
   {
   	pTempSignal->ErrCode = ADPHYERR_TEMP_OPEN;
   }
   else if(u16ADValue <= AD_TEMPERR_SHORT)
   {
   	pTempSignal->ErrCode = ADPHYERR_TEMP_SHORT;
   }
   else
   {
   	pTempSignal->ErrCode = ADPHYERR_TEMP_OK;
   }
}
/*******************************************************************************
 * Function:   static void Deal_VoltSignal(ADDATA *pVoltSignal)
 *
 * Description: ����  pVoltSignal �ĵ�ѹ�ź�ֵ�����ж����Ƿ����
 *
 * Returns:     none
 *
 * Notes:
 *
*******************************************************************************/
static void Deal_ADToVolt(ADPHYDATA *pVoltSignal, uint16 u16ADVolt)
{
	if (u8SupVoltMode == ADVOLT_MODE_12V)
	{//12v ,��100k��30k��ѹ
		if (u8RegisterType)
		{
			pVoltSignal->Value = (int16)((252 * (int32)u16ADVolt) >> 7);
		}
		else
		{
			pVoltSignal->Value = (59 * u16ADVolt) >> 5;
		}
		//�����ж�
		if (pVoltSignal->Value > AD_VOLT12_HIGHERR)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_HIGH;
		}
		else if (pVoltSignal->Value < AD_VOLT12_LOWERR)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_LOW;
		}
		else if (pVoltSignal->Value > AD_VOLT12_LOWOK && pVoltSignal->Value < AD_VOLT12_HIGHOK)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_OK;
		}
		else
		{//����
			
		}
	}
	else if (u8SupVoltMode == ADVOLT_MODE_24V)
	{//24 ��200k��30k��ѹ
		pVoltSignal->Value = (int16)((479 * (int32)u16ADVolt) >> 7);
		//�����ж�
		if (pVoltSignal->Value > AD_VOLT24_HIGHERR)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_HIGH;
		}
		else if (pVoltSignal->Value < AD_VOLT24_LOWERR)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_LOW;
		}
		else if (pVoltSignal->Value > AD_VOLT24_LOWOK && pVoltSignal->Value < AD_VOLT24_HIGHOK)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_OK;
		}
		else
		{//����
			
		}
	}
	else
	{
		pVoltSignal->Value = 0;
		pVoltSignal->ErrCode = ADPHYERR_VOLT_INVAILED;
	}
}

/*******************************************************************************
 * Function:   ADVoltType ReadIgn_Only(uint8 VoltId)
 *
 * Description:  ֻ��ȡ����ѹ��
 *
 * Returns:     none
 *
 * Notes:  ��ģ��ֻ��������ʱʹ��

 *
*******************************************************************************/
ADVoltType ReadIgn_Only(uint8 VoltId)
{
	static uint16 u16WaiteTimes = 0;
	uint8 u8ADIgnTimes, u8ConverOKTimes;
	ADVoltType avBackVolt = 0;
	
	while (u8ConverState != ADCONVER_STATE_INTOK && u16WaiteTimes < RIGN_INTCONVER_TIMES)
	{
		u16WaiteTimes++;
	};//�жϲɼ���û��ɣ��ȴ���������Ƿ���ɣ���תΪ�ȴ��ɼ�ģʽ
	
	u8ADIgnTimes = 0;
	u8ConverOKTimes = 0;
	
	do 
	{
		u16WaiteTimes = 0;
		u8ConverState = ADCONVER_STATE_WAITREAD;
		ADCH_SEL(VoltId);
		while (u16WaiteTimes < RIGN_WAITAD_TIMES)
		{
			u16WaiteTimes++;
		};
		
		if (u8ConverState != ADCONVER_STATE_WAITOK)
		{
			
		}
		else
		{
			sADData[VoltId].AdVData = u16HWBackData;
			u8ConverOKTimes++;
		}
		u8ADIgnTimes++;
	}while(u8ADIgnTimes < READ_ADIGN_TIMES);

	if (u8ConverOKTimes > 0)
	{//��һ�γɹ�����
		Deal_ADToVolt(&sADPhyData[VoltId], sADData[VoltId].AdVData);
		avBackVolt = sADPhyData[VoltId].Value;
	}
	else
	{
		avBackVolt = 0;
	}
	
	u8ConverState = ADCONVER_STATE_INTOK;
	return avBackVolt;
}

