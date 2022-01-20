/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Signal_Refresh.c
* �����������ź�ˢ��ģ�飬�ڴ�ʵ��
* 1  �¶ȸ��¡�ʵ�����¸����߼�
* 2  �����źŸ���
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹أ���Ϊģ����Ҫ����ʵ����ĿӦ���������
* ������ƿɼ��ο��ļ�
*/
/**************************���ļ�¼******************************************************
��
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
* 
* �汾��1.1
* ˵�����Ż���ʼ���������߼���
* ��û�г����¶ȴ�����ʱ����2s���˳�
* ���ߣ�xb
* ������ڣ�2020��7��6��
* 
* �汾��1.2
* ˵����1 �Ż���ʼ���������߼���
* ��û�г����¶ȴ�����ʱ����2s���˳�,ûʵ��û����
* 2 ���Զ�ʱ�����ڣ����⣬���⣬ΪĬ��ֵ ȥ���˹���
* ���ߣ�xb
* ������ڣ�2020��10��22��
* 
* �汾��1.3
* ˵����1 ���ڳ������SigF_GetResult05���õ��¶ȣ��Լ������¶Ȼز�µĲ���
* ���ߣ�xb
* ������ڣ�2020��12��19��
* 
* �汾��1.4
* ˵����1 ��ģ���ļ�ֻʵ���ϵ������ȶ���Ž�������ˢ��ģʽ��������Ҫ�Լ�����뿴�ο��ļ�
* ���ߣ�xb
* ������ڣ�2021��8��20��
*/

#include "Signal_TempGet.h"
#include "APPDrive.h"
#include "Deal_DTC.h"

const uint16  TBL_PTC1_JAC1[] =
{
	991 , 964 , 922 , 861 , 776 ,
	675 , 566 , 454 , 354 , 270 ,
	203 , 152 , 113 , 85 ,  64 ,
	49 , 37 , 29 ,
};

const uint16  TBL_PRESSL_JAC1[] =
{
	965 , 922 , 861 , 778 ,
	680 , 568 , 457 , 356 , 273 ,
	207 , 155 , 117 , 88 ,  67 ,
	51, 39, 31, 23
};

const uint16  TBL_PTH[] =
{
	976 , 942 , 889 , 816 ,
	724 , 617 , 507 , 404 , 315 ,
	241 , 183 , 139 , 105 ,  80 ,
};

static ADTGETTYPE ADT_PTH_TEMP =
{
		TBL_PTH,
	   14,
	   -300,
	   100,
};


static ADTGETTYPE ADT_PressTemp =
{
		TBL_PTC1_JAC1,
	   17,
	   -400,
	   100,
};

static ADTGETTYPE ADT_PressLTemp =
{
		TBL_PRESSL_JAC1,
	   17,
	   -300,
	   100,
};
//ˢ��״̬
static uint8 u8SigRefState;    //��ʼ��ʱΪ1�������˲�ʱ0
static uint16 TimerInit;
//���¸���ˢ���߼�
static TempType tAmbCal_Init, tAmbCal_Init_1;   //��ʼֵ��ǰһ��ֵ
static int16 i16TambSum;    //��Ȩ����
static uint8 u8AddNum;
static TempType tWaterCal_Last;  //�����׶ε� ˮ�£���ʼ��ʱ������

#if  NOINIT_TYPE ==  NOINIT_TYPE_KEA
__attribute__ ((section(".noinit"))) uint8 bInitTamb;   //��һ���ϵ��ǣ�֮��0x5a��������MCU_IsBattRest
#elif NOINIT_TYPE == NOINIT_TYPE_RL78
#pragma section bss NOINIT
uint8 bInitTamb;   //��һ���ϵ��ǣ�֮��0x5a��������MCU_IsBattRest
#pragma section 
#define __GOLBAL_VAR_
#endif

//ˢ��״̬
#define  SIGREF_STATE_INIT    0
#define  SIGREF_STATE_RUN     1
#define  SIGREF_STATE_STOP    2
//��ģ�龲̬��������
static void Signal_Refresh_EndInit(void);
static void Signal_Refresh_Normal(void);
/*******************************************************************************
 * Function:        Deal_Signal_Refresh
 *
 * Description:    �¶�ˢ�����س��򣬷���lowerdrive-loop����
 *
 * Returns:         none
 *
 * Notes:           �˴�Ϊ�¶�ˢ�£����ڣ��⣬���⣬�ڴ�ˢ��
 *                  �����߼���Ҫ����ʵ���߼������޸�
 *                 
********************************************************************************/
void Deal_Signal_Refresh(void)
{
	switch (u8SigRefState)
	{
		default:
		case SIGREF_STATE_INIT:
		  //TODO: ��ʼ���׶εĴ���
			//����ʵ�������ȶ���Ž����ʼ��ģʽ����û�и���1s���������ģʽ
			if (TimeOutChkTenthSLong(&TimerInit, 1))
			{
				u8AddNum++;
#ifdef  ADID_TAMB
				//����ʵ���¶��ȶ�����ÿ0.2s�ɼ�һ�βɼ�10�Σ��ϴε��¶�ֵ�ͱ��β�0.5����˵���ź��ȶ�������������ģʽ
				i16TambSum += ADPhy_GetFact(ADID_TAMB);
				if (u8AddNum >= 10)
				{
					tAmbCal_Init = i16TambSum/10;
					if (GAMath_abs(tAmbCal_Init - tAmbCal_Init_1) < SCD_T(0.5))
					{//�¶��ȶ������ˢ��ģʽ
						u8SigRefState = SIGREF_STATE_RUN;
						Signal_Refresh_EndInit();
					}
					else
					{//���¼���2s
						u8AddNum = 0;
						i16TambSum = 0;
						tAmbCal_Init_1 = tAmbCal_Init;
					}
				}
				else{}
#else
//ADID_TAMB ��2s���˳���ʼ��
				if (u8AddNum >= 20)
				{
					Signal_Refresh_EndInit();
					u8SigRefState = SIGREF_STATE_RUN;
				}
				else{}
#endif
			}
			break;
		case SIGREF_STATE_RUN:
			Signal_Refresh_Normal();
			break;
		case SIGREF_STATE_STOP:
			break;
	}

	(void)Deal_DTC();
}
/*******************************************************************************
 * Function:       static void Signal_Refresh_Normal(void)
 *
 * Description:    ����ˢ������
 *
 * Returns:         none
 *
 * Notes:          none  
 *                 
********************************************************************************/
static void Signal_Refresh_Normal(void)
{
	//TODO: ��������µ��¶ȸ��£���ο�ģ��
	static  uint16 TimerReftin;
	static  uint16 TimerRefAdd, TimerRefDec;
	static  uint16 TimerRefAdd_TD, TimerRefDec_TD;
	int16	tAmbfact, tInfact; //����ʵ��ֵ
	int16 tAmbfactdiff;      //tAmbfactdiff = ʵ������ - ��ʾ����
	//�����ź�ˢ��
	//�����ź�ˢ��
	if(DTCTFAILED(DTCNUM_TAMB))
	{
		sSenSorData.TambCal = _TAMB_DEFAULT_;         //����������ʱĬ��Ϊ25��
		sSenSorData.TambCal_TD = _TAMB_DEFAULT_;
	}
	else
	{
		tAmbfact = SigF_GetResult(SIG_NUM_TAMB);  //ʹ���˲�������½���ˢ��
//		tAmbfact = ADPhy_GetFact(ADID_TAMB);  //��ʱʹ�òɼ�ֵ
		tAmbfactdiff = tAmbfact - sSenSorData.TambCal;
		if(1)
		{
			if(0 == sSenSorData.u8VehicleSpeed)
			{
				if(tAmbfact < sSenSorData.TambCal)
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,1))
					{
					   sSenSorData.TambCal -= 5;
					   if  (sSenSorData.TambCal <= tAmbfact)
					   {
					     sSenSorData.TambCal = tAmbfact;
					   }
					}
					(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
				}
				else
				{
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}

			}
			else if((0 < sSenSorData.u8VehicleSpeed) && (sSenSorData.u8VehicleSpeed <= 20))
			{
				if(tAmbfact > sSenSorData.TambCal)
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,90))
					{  //���ܼ�2 ֻ�ܼ�1
					   sSenSorData.TambCal += 5;
					   if  (sSenSorData.TambCal >= tAmbfact)
					   {
					     sSenSorData.TambCal = tAmbfact;
					   }
					 }
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}
				else if(sSenSorData.TambCal!=tAmbfact)
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,1))
					{
					   sSenSorData.TambCal -= 5;
					   if  (sSenSorData.TambCal <= tAmbfact)
					   {
					      sSenSorData.TambCal = tAmbfact;
					    }
					}
					(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
				}
				else
				{
					(void)TimeOutChkTenthSLong(&TimerRefAdd_TD, 0);
					(void)TimeOutChkTenthSLong(&TimerRefDec_TD, 0);
				}

			}
			else if((20 < sSenSorData.u8VehicleSpeed) && (sSenSorData.u8VehicleSpeed <= 60))
			{
				if(tAmbfact > sSenSorData.TambCal)
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,60))
					{  //���ܼ�2 ֻ�ܼ�1
						sSenSorData.TambCal += 5;
						if  (sSenSorData.TambCal >= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
				   }
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}
				else if(sSenSorData.TambCal!=tAmbfact)
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,1))
					{
						sSenSorData.TambCal -= 5;
						if  (sSenSorData.TambCal <= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
				}
				else
				{
					(void)TimeOutChkTenthSLong(&TimerRefAdd_TD, 0);
					(void)TimeOutChkTenthSLong(&TimerRefDec_TD, 0);
				}
			}
			else
			{
				if(tAmbfact > sSenSorData.TambCal)
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,30))
					{  //���ܼ�2 ֻ�ܼ�1
						sSenSorData.TambCal += 5;
						if(sSenSorData.TambCal >= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}
				else if(sSenSorData.TambCal!=tAmbfact)
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,1))
					{
						sSenSorData.TambCal -= 5;
						if(sSenSorData.TambCal <= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
				  (void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
			   }
			   else
			   {
					(void)TimeOutChkTenthSLong(&TimerRefAdd_TD, 0);
					(void)TimeOutChkTenthSLong(&TimerRefDec_TD, 0);
				}
			}

		}
		else
		{
			if((0 < sSenSorData.u8VehicleSpeed) && (sSenSorData.u8VehicleSpeed <= 50))
			{
				if(tAmbfactdiff > SCD_T(5))
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,90))
					{  //���ܼ�2 ֻ�ܼ�1
						sSenSorData.TambCal += 5;
						if(sSenSorData.TambCal >= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);

				}
				/*else if(tAmbfactdiff < SCD_T(-5))
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,50))
					{
						sSenSorData.TambCal -= 5;
						if(sSenSorData.TambCal <= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);

				}*/
				else
				{
					if(tAmbfact < sSenSorData.TambCal)
					{
						if(TimeOutChkTenthSLong(&TimerRefDec,90))
						{
							sSenSorData.TambCal -= 5;
							if(sSenSorData.TambCal <= tAmbfact)
							{
								sSenSorData.TambCal = tAmbfact;
							}
						}
						(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
					}

				}
			}
			else if((50 < sSenSorData.u8VehicleSpeed) && (sSenSorData.u8VehicleSpeed < 70))
			{
				if(tAmbfactdiff > SCD_T(3))
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,60))
					{  //���ܼ�2 ֻ�ܼ�1
						sSenSorData.TambCal += 5;
						/*if(sSenSorData.TambCal >= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}*/
					}
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}
				else
				{
					if(tAmbfact < sSenSorData.TambCal)
					{
						if(TimeOutChkTenthSLong(&TimerRefDec,60))
						{
							sSenSorData.TambCal -= 5;
							if(sSenSorData.TambCal <= tAmbfact)
							{
								sSenSorData.TambCal = tAmbfact;
							}
						}
						(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
					}
				}
			}
			else
			{
				if(tAmbfact > sSenSorData.TambCal)
				{
					if(TimeOutChkTenthSLong(&TimerRefAdd,30))
					{  //���ܼ�2 ֻ�ܼ�1
						sSenSorData.TambCal += 5;
					   if(sSenSorData.TambCal >= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefDec, 0);
				}
				else if(sSenSorData.TambCal!=tAmbfact)
				{
					if(TimeOutChkTenthSLong(&TimerRefDec,30))
					{
						sSenSorData.TambCal -= 5;
						if(sSenSorData.TambCal <= tAmbfact)
						{
							sSenSorData.TambCal = tAmbfact;
						}
					}
					(void)TimeOutChkTenthSLong(&TimerRefAdd, 0);
				}
				else
				{
					(void)TimeOutChkTenthSLong(&TimerRefAdd_TD, 0);
					(void)TimeOutChkTenthSLong(&TimerRefDec_TD, 0);
				}
			}
		}
	}
	if(sSenSorData.TambCal > SCD_T(500))
	{
		sSenSorData.TambCal =  SCD_T(500);
	}
	else{}
	sSenSorData.TambCal_TD = sSenSorData.TambCal;


//	if (bTambDelayFlg)
//	{
//		if (TimeOutChkSLong(&TimerTambDelay, 1800))
//		{
//			bTambDelayFlg = 0;
//		}
//		sSenSorData.TambCal = tAmbCal_Init;
//	}
//	else
//	{
//
//	}

	//sSenSorData.TinCal = SCD_T(25);
	//sSenSorData.TambCal = SCD_T(15);
   //�����ź�ˢ��
	if(DTC[DTCNUM_TIN] & 0x01)
	{
		sSenSorData.TinCal = _TIN_DEFAULT_;//2014-2-16 12:42:07���ģ����ڴ��������ϣ����Ĭ��Ϊ20��
	}
	else
	{
		tInfact = SigF_GetResult(SIG_NUM_TIN);
//			tInfact = ADPhy_GetFact(ADID_TIN);  //��ʱʹ�òɼ�ֵ
		if(TimeOutChkSLong(&TimerReftin, 2))
		{   //refresh tin 0.5 every10S  0.1 2S
			if(tInfact > sSenSorData.TinCal)
			{
				sSenSorData.TinCal++;
			}
			else  if(tInfact != sSenSorData.TinCal)
			{
				sSenSorData.TinCal--;
			}
			else{}
		}else{}
	}
   if (sSenSorData.TinCal >= SCD_T(75))
   {
   	sSenSorData.TinCal = SCD_T(75);
   }else{}

   sSenSorData.TinSimul = sSenSorData.TinCal;

   //�����¶�
	if(DTC[DTCNUM_TEVP] & 0x01)
	{
		sSenSorData.TevpCal = _TEVP_DEFAULT_;
	}
	else
	{
		sSenSorData.TevpCal = SigF_GetResult(SIG_NUM_TEVP);
//		sSenSorData.TevpCal = ADPhy_GetFact(ADID_TEVP);  //��ʱʹ�òɼ�ֵ
	}

	//�����ź�ˢ��
//	if(DTC[DTCNUM_FVENT_BACKERR] & 0x01)
//	{
//		sSenSorData.TVentF = _VENT_DEFAULT_;
//	}
//	else
//	{
//		sSenSorData.TVentF = SigF_GetResult(SIG_NUM_VENTF);
////		sSenSorData.TVentF = ADPhy_GetFact(ADID_VENTF);  //��ʱʹ�òɼ�ֵ
//	}

	//�����ź�ˢ��
//	if(DTC[DTCNUM_DVENT_BACKERR] & 0x01)
//	{
//		sSenSorData.TVentD = _FOOT_DEFAULT_;
//	}
//	else
//	{
//		sSenSorData.TVentD = SigF_GetResult(SIG_NUM_VENTD);
////		sSenSorData.TVentD = ADPhy_GetFact(ADID_VENTD);  //��ʱʹ�òɼ�ֵ
//	}
   //�����ź�ˢ��        0-125 60s
	//uint8 adsun = Get_8ad(ADID_SUN);
	sSenSorData.u8ADSunR = ADValue_Get(ADID_SUN, ADVALUE_GETTYPE_8AD);
	//sSenSorData.TwaterCal = _TWATER_DEFAULT_;


	if(DTC[DTCNUM_PT1TEMP] & 0x01)
	{
		sEVSensorData.TPL1_Temp = _TPRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPL1_Temp = SigF_GetResult(SIG_NUM_PT1TEMP);
	}

	if(DTC[DTCNUM_PT2TEMP] & 0x01)
	{
		sEVSensorData.TPL2_Temp = _TPRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPL2_Temp = SigF_GetResult(SIG_NUM_PT2TEMP);
	}

	if(DTC[DTCNUM_PTHTEMP] & 0x01)
	{
		sEVSensorData.TPH_Temp = _TPRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPH_Temp = SigF_GetResult(SIG_NUM_PT3TEMP);
	}

//	if(DTC[DTCNUM_PTCTEMP] & 0x01)
//	{
//		sHPSenSorData.tPTCTemp = _TPTC_DEFAULT_;
//	}
//	else
//	{
//		sHPSenSorData.tPTCTemp = SigF_GetResult(SIG_NUM_PTCTEMP) + 800;
//	}

	if(DTC[DTCNUM_PT1PRESS] & 0x01)
	{
		sEVSensorData.TPL1_Press = _PRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPL1_Press = SigF_GetResult(SIG_NUM_PT1PRESS);
	}

	if(DTC[DTCNUM_PT2PRESS] & 0x01)
	{
		sEVSensorData.TPL2_Press = _PRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPL2_Press = SigF_GetResult(SIG_NUM_PT2PRESS);
	}

	if(DTC[DTCNUM_PTHPRESS] & 0x01)
	{
		sEVSensorData.TPH_Press = _PRESS_DEFAULT_;
	}
	else
	{
		sEVSensorData.TPH_Press = SigF_GetResult(SIG_NUM_PT3PRESS);
	}

	//if(DTC[DTCNUM_SUN_BACKERR] & 0x01)
	//{
	//	sSenSorData.u8ADSunR = 0xff;
	//}
	//else
	//{
		sSenSorData.u8ADSunR = SigF_GetResult(SIG_NUM_SUN);
	//}
//�ź�����

	sVehicleControl.i16BackEACSpeed = (sCANMsg490.Bits.COMPSPD_1 <<8) + sCANMsg490.Bits.COMPSPD_2;
	sVehicleControl.i16PTCUsePower = sCANMsg432.Bits.AIRPTC_W_HVHTR_POWER *50;

}
/*******************************************************************************
 * Function:        void Signal_Refresh_EndInit(void)
 *
 * Description:     ������ʼ������
 *
 * Returns:         none
 *
 * Notes:           ��ʼ�����̽���ʱ����
 *                 
********************************************************************************/
static void Signal_Refresh_EndInit(void)
{
	//TODO: ��д��ʼ��������� �������̣�ģ��ʵ�����³�ʼ���߼����粻��Ҫ��ɾ��
	if (bInitTamb == 0x5a)
	{

	}
	else
	{//�ǵ�һ���ϵ�ı��
		bInitTamb = 0x5a;
	}
}

/*******************************************************************************
 * Function:    SigRefresh_IgnON /SigRefresh_IgnOFF
 *
 * Description: �ź�ˢ��֪ͨ��������Դ�ɴ���/��������ʱ����һ��
 *
 * Returns:     none
 *
 * Notes:       ����Ӧ��Ҫ���д
 *
*******************************************************************************/
void SigRefresh_IgnON(void)
{
	//TODO: ��һ���ϵ�Ĳ���
	tWaterCal_Last = sSenSorData.TwaterCal;
	u8SigRefState = SIGREF_STATE_INIT;
	(void)TimeOutChkTenthSLong(&TimerInit, 0);
	i16TambSum = 0;
	tAmbCal_Init_1 = ADPhy_GetFact(ADID_TAMB);
	u8AddNum = 0;
}


void SigRefresh_IgnOFF(void)
{
	//u8SigRefState = SIGREF_STATE_STOP; //���IGN����󣬲���Ҫ�ɼ�ˢ���¶�
}

/*******************************************************************************
 * Function:  void SignalCAN_RXComplete(uint32 u32Id)
 *
 * Description:  can rxcomplete
 *
 * Returns:      none
 *
 * Notes:     u32Id�� ���յ���CAN ID
 *        
*******************************************************************************/
void SignalCAN_RXComplete(uint32 u32Id)
{
	//TODO: Add code to read Frame singal here
	switch(u32Id)
	{		
		default: 
			break;
		case R_ID_12F85050:
			sSenSorData.u8VehicleSpeed = sCANMsg12F85050.Bits.C_VSP;
			break;
	}
}

void SignalCAN2_RXComplete(uint32 u32Id)
{
	//TODO: Add code to read Frame singal here
	switch(u32Id)
	{
		default:
			break;
		case R_ID_22F:
			sVehicleControl.i16PowerLimte = (sCANMsg22F.Bits.VCU_TO_TMU_TMS_POWER_LIMIT_1 << 8) + sCANMsg22F.Bits.VCU_TO_TMU_TMS_POWER_LIMIT_2;
			break;
		case R_ID_424:
			sVehicleControl.bPump = sCANMsg424.Bits.PUMPEN;
			if (sCANMsg424.Bits.BMSHEATREQUEST >= 2)
			{
				sVehicleControl.bBattHotReq = 1;
			}
			else
			{
				sVehicleControl.bBattHotReq = 0;
			}

			sEVSensorData.BattInWater = sCANMsg424.Bits.INLETCURRENTTEMP*5 -400;
			sEVSensorData.BattOutWater = sCANMsg424.Bits.OUTLETCURRENTTEMP*5 -400;
			sEVSensorData.BattTargetWater = sCANMsg424.Bits.INLETTARGETTEMP*5 -400;
			sEVSensorData.BattTemp = sCANMsg424.Bits.AVERAGEBATTEMP*5 -400;
			sVehicleControl.bBattCoolReq = sCANMsg424.Bits.BMSCOOLREQUEST;
			break;
		case R_ID_26D:
			sSenSorData.TwaterCal = sCANMsg26D.Bits.HVHTR_OUTLET_TEMP *10 -400;
			break;
		case R_ID_131:
			sEVSensorData.CurrentMotorTemp = sCANMsg131.Bits.CURRENTMOTORTEMP*10 -400;
			sEVSensorData.CurrentIGBTemp = sCANMsg131.Bits.CURRENTIGBTTEMP *10 -400;
			break;
		case R_ID_460:
			sEVSensorData.OBCtemp = sCANMsg460.Bits.OBCTEMP*10 -550;
			break;
		case R_ID_470:
			sEVSensorData.DCDCbodyTemp = sCANMsg470.Bits.DCDCBODYTEMP*10 -550;
			break;
	}
}
/*******************************************************************************
 * Function:  void SignalCAN_RecTimeOut(uint32 u32Id)
 *
 * Description:  can rx timeout
 *
 * Returns:      none
 *
 * Notes:     u32Id�� ���յ���CAN ID
 *        
*******************************************************************************/
#define   BATTWATER_DEFAULT_TEMP   SCD_T(25)
#define   SENSOR_DEFAULT_TEMP   SCD_T(25)
void SignalCAN_RecTimeOut(uint32 u32Id)
{
	//TODO:���źų�ʱ�Ĵ���
	switch(u32Id)
	{
		default:
			break;
		case R_ID_12F85050:
			sSenSorData.u8VehicleSpeed = 0;
			break;
	}
}

void SignalCAN2_RecTimeOut(uint32 u32Id)
{
	//TODO:���źų�ʱ�Ĵ���
	switch(u32Id)
	{
		default:
			break;
		case R_ID_22F:
#if PCDEBUG_CONFIG > 0
			sVehicleControl.i16PowerLimte = 5000;
#else
			sVehicleControl.i16PowerLimte = 1000;
#endif

			break;
		case R_ID_26D:
			sSenSorData.TwaterCal = _TWATER_DEFAULT_;
			break;
		case R_ID_424:
			sVehicleControl.bPump = 0;
			sVehicleControl.bBattHotReq = 0;
			sEVSensorData.BattInWater = SCD_T(25);
			sEVSensorData.BattOutWater = SCD_T(25);
			sEVSensorData.BattTargetWater = SCD_T(25);
			sEVSensorData.BattTemp = SCD_T(25);
#if PCDEBUG_CONFIG > 0
			sVehicleControl.bBattCoolReq = 0;
			sVehicleControl.bBattHotReq = 0;
#endif
			break;
		case R_ID_131:
			sEVSensorData.CurrentMotorTemp = SENSOR_DEFAULT_TEMP;
			sEVSensorData.CurrentIGBTemp = SENSOR_DEFAULT_TEMP;
			break;
		case R_ID_460:
			sEVSensorData.OBCtemp = SENSOR_DEFAULT_TEMP;
			break;
		case R_ID_470:
			sEVSensorData.DCDCbodyTemp = SENSOR_DEFAULT_TEMP;
			break;
	}
}
/*******************************************************************************
* Function:      int16 SigF_GetCurrentState(uint8 u8Num)
*
* Description:   sigfilter ģ���������� 
                 
* return :       ���ص�ǰ״̬
* Note  :        �밴ģ��˵���ļ���д

************************************************************************************/
int16 SigF_GetCurrentState(uint8 u8Num)
{
	//TODO:sigfilter ģ����������
	int16 i16SigData;
	switch(u8Num)
	{
		default:
			i16SigData = 0xffff;
			break;
		case SIG_NUM_TIN:
			i16SigData = SCD_T(25);//ADPhy_GetFact(ADID_TIN);
			break;
		case SIG_NUM_TAMB:
			i16SigData =  ADPhy_GetFact(ADID_TAMB);
			break;
		case SIG_NUM_TEVP:
			i16SigData = ADPhy_GetFact(ADID_TEVP);
			break;
//		case SIG_NUM_VENTF:
//			i16SigData =  ADPhy_GetFact(ADID_VENTF);
//			break;
//		case SIG_NUM_VENTD:
//			i16SigData =  ADPhy_GetFact(ADID_VENTD);
//			break;
		case SIG_NUM_PT1TEMP:
			i16SigData =  Signal_GetAdT (ADValue_Get(ADID_TPTL1, ADVALUE_GETTYPE_10AD), &ADT_PTH_TEMP);
			break;
		case SIG_NUM_PT2TEMP:
			i16SigData =  Signal_GetAdT (ADValue_Get(ADID_TPTL2, ADVALUE_GETTYPE_10AD), &ADT_PTH_TEMP);
			break;
		case SIG_NUM_PT3TEMP:
			i16SigData =  Signal_GetAdT (ADValue_Get(ADID_TPTH, ADVALUE_GETTYPE_10AD), &ADT_PTH_TEMP);
			break;
//		case SIG_NUM_PTCTEMP:
//			i16SigData =  ADPhy_GetFact(ADID_PTC);
//			break;

		case SIG_NUM_PT1PRESS:
			i16SigData = Signal_GetAd_Press(1,ADValue_Get(ADID_PPTL1, ADVALUE_GETTYPE_10AD));
			break;
		case SIG_NUM_PT2PRESS:
			i16SigData = Signal_GetAd_Press(1,ADValue_Get(ADID_PPTL2, ADVALUE_GETTYPE_10AD));
			break;
		case SIG_NUM_PT3PRESS:
			i16SigData = Signal_GetAd_Press(0,ADValue_Get(ADID_PPTH, ADVALUE_GETTYPE_10AD));
			break;
		case SIG_NUM_SUN:
			i16SigData = ADValue_Get(ADID_SUN, ADVALUE_GETTYPE_8AD);
			break;
	}
	return i16SigData;
}

