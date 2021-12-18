/**************************�ļ�˵��*****************************************************
* Copyright (c) 2021  ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CAUTO_Vent.c
*   ��ģ��Ϊ�Զ��������Զ������Ľӿڶ�����Ϊһ������Ҫʵ�����¹���
*   1 AUTOTD_Init ϵͳ�����ʼ��״̬�����
*   
*   2 Cal_TD()  ����td���ֵ�������ݷ��� Cal_TD  sAUTOResult
*   
*   3 AUTOTD_COntrol �����Զ�����
*   1�� ac ����ֱ�Ӹı� systemconrol ����
*   2�� NCF���� ֱ�Ӹı�systemconrol ����
*   3��fan ���������ѹֵ�������������
*   4�� ģʽֱ�Ӹı�modeֵ
*   5����ϰѼ���õ� %�����������
*   
*   4 ���Զ����ܽ����AUTOTD_GetResult���
 *   
*
* �������� codewarrior IDE 11.1-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ �Ӵ�ԭCAUTO_Vent.c 1.4 �汾����������Ӧ�����Զ��㷨1.0 �汾
* ԭ���ߣ�xb
* ������ڣ�2021��3��8��
* 
* �汾��1.1
* ˵�����Ż� AUTOTD_GetResult
* ԭ���ߣ�xb
* ������ڣ�2021��8��23��
* 
* �汾��1.2
* ˵����TD ����ʱ�� ��Ҫint32
* ԭ���ߣ�xb
* ������ڣ�2021��11��22��
*/
#include "CAUTO.h"
#include "MIXVENT_PID.h"
#include "CAUTO_Vent_APP.h"

#define   _TD_TAMB_SWITCH__   0   //ʵ�����º�TD���²���ͬһ������ʱʹ��
#define   MAX_AFELVL          8   //�Զ�ʱ�̶�����8��



TDDATA	sTDData;                // TDֵ����
AUTORESULT sAUTOResult;          //�Զ��������ṹ�壬
static  uint8   StartHfan, StartCfan;      //��籣�����Ƿ��ǿ����ȷ籣�����ȷ籣��
static  uint16  u16StartTimes;     //��ʼʱ���ʱ��ÿ0.1S +1��������0
static AutoFanVoltType i16ProFanVolt; 
static uint8 bOffState;

#if _CFAN_PROTECT_ == 3
static uint8 bCFanTwater, bCFanTamb;
#endif

/*******************************************************************************
 * Function:        void  Cal_TD(void)
 *
 * Description:     TD����
 *
 * Returns:         none
 *
 * Notes:           1 ����TDֵ�� ��MAIN-LOOP�е���
 *
********************************************************************************/
void  Cal_TD(void)
{
   uint8  u8NSeg;
   static int16 i16TDSunL;  //��ǰAD��tdsun��
	static  uint16 TimerRefSun;
	static  uint16 TimerMainTask;

	int32 i32TDResult;
  // SENSORDATA lSenSorData = Get_SenSor();
   //ÿ1s ����1��
  // if(!TimeOutChkMsLong(&TimerDealCal,200)) return ;
#if _PROJECT_AUTO_ == 0
   SystemControl.AutoBits.AC = 0;
   SystemControl.AutoBits.FAN = 0;
   SystemControl.AutoBits.MODE = 0;
   SystemControl.AutoBits.NCF = 0;
   return;
#endif

  // Cal_SimulTin();
   
   sSenSorData.TinSimul = sSenSorData.TinCal;
#if _TD_TAMB_SWITCH__
   lSenSorData.TambCal = lSenSorData.TambCal_TD;   //GetAd_Fact();
#endif
   
   if (!TimeOutChkMsLong(&TimerMainTask, 500))
   {//500ms ����һ��
   	return;
   }
   
   //�������²���
   sTDData.i16TDRamb = MathG_GetSegLinei16(sSenSorData.TambCal, TBL_SEGTAMB, TBL_OFFSETAMB, TD_TAMBSEG_MAXNUM);
   
   //�������ⲹ�����˲�
   u8NSeg = GAMath_CalDataSegU8(sSenSorData.u8ADSunR, TBL_ADSUN, TD_SUNSEG_MAXNUM, 0);
   if (u8NSeg >= TD_SUNSEG_MAXNUM)
   {
   	i16TDSunL = TBL_OFFSETSUN[TD_SUNSEG_MAXNUM-1];
   }
   else if (u8NSeg ==0)
   {
   	i16TDSunL = TBL_OFFSETSUN[0];
   }
   else
   {
   	i16TDSunL = GAMath_LineFuni16(TBL_ADSUN[u8NSeg-1], TBL_ADSUN[u8NSeg], 
		TBL_OFFSETSUN[u8NSeg-1], TBL_OFFSETSUN[u8NSeg], sSenSorData.u8ADSunR);
   }
   //��������
   if (i16TDSunL > sTDData.i16TDRsun)
   {
   	if ( TimeOutChkTenthSLong(&TimerRefSun,TDSunAddTimer) )
   	{
   		sTDData.i16TDRsun++;
   	}
   }
   else if (i16TDSunL < sTDData.i16TDRsun)
   {
   	if ( TimeOutChkTenthSLong(&TimerRefSun,TDSunAddTimer) )
   	{
   		sTDData.i16TDRsun--;
   	}
   }
   else
   {}
   
   int16 i16TDtset = SystemControl.tset;
   if (SystemControl.tset == TSET_LO)
   {
   	i16TDtset = SCD_T(17);
   }
   else if (SystemControl.tset == TSET_HI)
   {
   	i16TDtset = SCD_T(32);
   }
   else{}
   
   i32TDResult = (int32)u16KTin * (sSenSorData.TinSimul - AUTO_BASE_TSET) ;
   i32TDResult += (int32)u16KTamb * (sSenSorData.TambCal - AUTO_BASE_TSET);
   i32TDResult += (int32)u16KSun * sTDData.i16TDRsun;
   i32TDResult -= (int32)u16KTset * (i16TDtset - AUTO_BASE_TSET);
   
   i32TDResult += sTDData.i16TDRamb * 10;
   
   
   //��Χ�;�������
   if ( (i32TDResult % 10) >= 5) 
   {
   	sTDData.TDResult += 10;
   }
   else
   {
   	
   }
   i32TDResult = i32TDResult/10;
   
   if (i32TDResult>=3000) 
   {
   	i32TDResult = 3000;
   }
   else if (i32TDResult <= -3000)
   {
   	i32TDResult = -3000;
   }
   else{}
   	
   sTDData.TDResult= (int16)i32TDResult;
}


/*******************************************************************************
 * Function:        StartUp_Strategies
 *
 * Description:     ����������������
 *
 * Returns:         none
 *
 * Notes:       
********************************************************************************/
static void StartUp_Strategies(void)
{

#if _CFAN_PROTECT_ == 3
	if (sSenSorData.TwaterCal >= SCD_T(53) )
	{
		bCFanTwater = 0;
	}
	if (sSenSorData.TwaterCal <= SCD_T(50))
	{
		bCFanTwater = 1;
	}
	if (sSenSorData.TambCal >= SCD_T(15) )
	{
		bCFanTamb = 0;
	}
	if (sSenSorData.TambCal <= SCD_T(10))
	{
		bCFanTamb = 1;
	}

	if (sSenSorData.TwaterCal <= SCD_T(15))
	{
		sAUTOResult.CFanState = 0x31;
		i16ProFanVolt = TBL_CFAN[0]; 
	}
	else if (sSenSorData.TwaterCal <= SCD_T(30))
	{
		sAUTOResult.CFanState = 0x32;
		i16ProFanVolt = GAMath_LineFuni16(SCD_T(15), SCD_T(30), TBL_CFAN[0], TBL_CFAN[1], sSenSorData.TwaterCal);
	}
	else if (sSenSorData.TwaterCal <= SCD_T(45))
	{
		sAUTOResult.CFanState = 0x33;
		i16ProFanVolt = GAMath_LineFuni16(SCD_T(30), SCD_T(45), TBL_CFAN[1], TBL_CFAN[2], sSenSorData.TwaterCal);
	}
	else if (sSenSorData.TwaterCal <= SCD_T(60))
	{
		sAUTOResult.CFanState = 0x34;
		i16ProFanVolt = GAMath_LineFuni16(SCD_T(45), SCD_T(60), TBL_CFAN[2], TBL_CFAN[3], sSenSorData.TwaterCal);
	}
	else if (sSenSorData.TwaterCal <= SCD_T(70))
	{
		sAUTOResult.CFanState = 0x35;
		i16ProFanVolt = GAMath_LineFuni16(SCD_T(60), SCD_T(70), TBL_CFAN[3], TBL_CFAN[4], sSenSorData.TwaterCal);
	}
	else
	{
		sAUTOResult.CFanState = 0;
		i16ProFanVolt = TBL_CFAN[4]; 
		StartCfan = 0;
	}
	if (SystemControl.AutoBits.FAN == 0 || bCFanTamb == 0)
	{
		sAUTOResult.CFanState = 0;
		i16ProFanVolt = TBL_CFAN[4]; 
		StartCfan = 0;
	}
#elif _CFAN_PROTECT_ == 1
	TempType Tadt_HI, Tadt_LO;
	TempType TdiffTset;   //��ͬ�����¶ȴ�����ƫ��
	static uint8 u8StedyTimes =0;

	TdiffTset = (SystemControl.tset - TSET_LO) * TADT_DIFF_LOHI/ (TSET_HI - TSET_LO);
	if (sSenSorData.TambCal <= SCD_T(-10))
	{
		Tadt_LO = Tadt_Lo1;
	}
	else if (sSenSorData.TambCal >= SCD_T(15))
	{
		Tadt_LO = Tadt_Lo2;
	}
	else
	{
		Tadt_LO = GAMath_LineFuni16(SCD_T(-10), SCD_T(15), Tadt_Lo1, Tadt_Lo2, sSenSorData.TambCal);
	}
	
	Tadt_HI = Tadt_LO + TdiffTset;
	
	if (StartCfan && sSenSorData.TambCal < Tamb_STHOT && sSenSorData.TwaterCal < Tadt_HI &&
			sTDData.TDResult < TD_STHOT  && SystemControl.AutoBits.FAN)
	{//������籣��������
		switch (sAUTOResult.CFanState)
		{
		default:
		case 0:
			//��һ�׶�
			sAUTOResult.CFanState = 0x01;
		case 1:
			if (u16StartTimes >= u8HOTSEQ_1)
			{
				sAUTOResult.CFanState = 0x02;
			}
			i16ProFanVolt = 0;
			break;
		case 2:
			if (sSenSorData.TwaterCal >= Tadt_LO)
			{
				sAUTOResult.CFanState = 0x03;	
				u8StedyTimes = 0;
			}
			i16ProFanVolt = fVoltBLR_1A;
			break;
		case 3:
			if (i16ProFanVolt >= fVoltBLR_2A && sSenSorData.TwaterCal >= Tadt_LO)
			{
				u8StedyTimes++;
				if (u8StedyTimes >= 5)
				{
					sAUTOResult.CFanState = 0x04;
				}
			}
			i16ProFanVolt = fVoltBLR_2A;
			break;
		case 4:
			if (sSenSorData.TwaterCal >= Tadt_HI)
			{
				i16ProFanVolt += CFAN_SEG4_PERVOLT;
			}else{}
			if (i16ProFanVolt >= sAUTOResult.aFanVolt)
			{//�˳���籣��
				sAUTOResult.CFanState = 0;
				StartCfan = 0;
			} 
			else{}
			break;
		}
	}
	else
	{
		sAUTOResult.CFanState = 0;
		StartCfan = 0;
		i16ProFanVolt = TBL_AFANVOLT[MAX_AFELVL];
	}
#else
	sAUTOResult.CFanState = 0;
	StartCfan = 0;
#endif
	
#if _HFAN_PROTECT_ == 1
	if(StartHfan &&  sSenSorData.TambCal > Tamb_STCOOL && sTDData.TDResult > TD_STCOOL 
			&& SystemControl.AutoBits.FAN)
	{
		switch(sAUTOResult.HFanState)
		{
			default:
			case 0:
				sAUTOResult.HFanState=1;//�ȷ籣���ĵڶ�����
			case 1:
				if (u16StartTimes > u8COOLSEQ_1)
				{
					sAUTOResult.HFanState=2;
				}
				break;
			case 2:
				if (u16StartTimes > (u8COOLSEQ_1 + u8COOLSEQ_2))
				{
					StartHfan = 0;
					sAUTOResult.HFanState=0;
				}
				break;
		}
	}
	else
	{
		StartHfan = 0;
		sAUTOResult.HFanState = 0;
	}
#else
	StartHfan = 0;
	sAUTOResult.HFanState = 0;
#endif
	 
	u16StartTimes++;
}


/*******************************************************************************
 * Function:        AUTOTD_Control
 *
 * Description:     TD ����
 *
 * Returns:         none
 *
 * Notes:         ����TDֵ��SENSORDATA��SYSTEMCONTROL ��״̬ʵ���Զ�����
 *               1  �Զ�TD���ߵĿ���
 *               2  ����ȷ籣������
 *               3 ��ʱ������ѭ�����
 *               4  �Զ����ƹ���ʵ�֣�
444ystemCOntrol4.1     4.1  AC ֱ�Ӹ��� SystemCOntrol.AC
                       4.2  ģʽ ֱ�Ӹ��� SystemCOntrol.mdMode
                       4.3  ���� ֱ�Ӹ��� SystemCOntrol.ncfMode
                       4.4  ��ϼ������ϰٷֱȣ�sAUTOResult.i16Hmix
                       4.5  ���� �����������ѹ  sAUTOResult.aFanVolt
********************************************************************************/
void AUTOTD_Control(void)
{
	SYSTEMCONTROL *pSystemControl;
	static uint16 TimerDealAUTO;
	static  uint8 TambProOsdt;	//mode ����
	AutoFanVoltType i16AutoFanVolt;
	uint8 u8NSeg;
	
	
	if (TimeOutChkTMsLong(&TimerDealAUTO, 100) || (1 == bOffState))
	{//0.1s����һ�Σ����ʱ����ʱ���������
		pSystemControl = SystemControlGet();
		if (SystemControl.mdMode == MD_MODE_OSF || SystemControl.mdMode == MD_MODE_OST)
		{
			sSenSorData.Tadt = sSenSorData.TVentF;
		}
		else if (SystemControl.mdMode == MD_MODE_OSFD)
		{
			sSenSorData.Tadt = (u8T1Per*sSenSorData.TVentF + (100-u8T1Per)*sSenSorData.TVentD) / 100;
		}
		else
		{
			sSenSorData.Tadt = sSenSorData.TVentD;
		}
		sSenSorData.Tddt = AUTO_BASE_TSET - sTDData.TDResult * u8G_TD /10;
		
		if (!SystemControl.OffState)
		{
			StartUp_Strategies(); 
			//auto ac 
			if(SystemControl.AutoBits.AC)
		   {
		      if(SystemControl.tset == TSET_LO || SystemControl.tset == TSET_HI)//
		      {
		         if (SystemControl.tset == TSET_LO)
		         	SystemControl.acMode = AC_MODE_ON;   //AC
		         else
		         	SystemControl.acMode = AC_MODE_OFF; //
		      }
		      else
		      {
		      	if (sSenSorData.TambCal >= TBL_ACTAMB[1]) 	SystemControl.acMode = AC_MODE_ON;
		      	if (sSenSorData.TambCal <= TBL_ACTAMB[0]) 	SystemControl.acMode = AC_MODE_OFF;
		      }
		   }


			//auto ncf 
			if (pSystemControl->AutoBits.NCF)
			{
#if _CFAN_PROTECT_ == 1
				if (sAUTOResult.CFanState || sAUTOResult.HFanState)
				{//��籣���׶ζ�����ѭ��
					SystemControl.ncfMode = NCF_MODE_NEW;				
				}
				else
#endif
				{
					if(SystemControl.tset == TSET_LO)  SystemControl.ncfMode = NCF_MODE_CIRF;
					else if(SystemControl.tset == TSET_HI) SystemControl.ncfMode = NCF_MODE_NEW;
					else 
					{  
						if (sTDData.TDResult < TBL_NCFTD[0]) SystemControl.ncfMode = NCF_MODE_NEW;
						if (sTDData.TDResult > TBL_NCFTD[1]) SystemControl.ncfMode = NCF_MODE_CIRF;	
					}
				}
			}
			//�Զ�ģʽ 
			if (sSenSorData.TambCal > tLOTamb_ProMode[1]) TambProOsdt=0;     //
			else if ((sSenSorData.TambCal <= tLOTamb_ProMode[1]) && (sSenSorData.TambCal >= tLOTamb_ProMode[0])) 
			{
				if((SystemControl.mdMode != MD_MODE_OSD) && (SystemControl.mdMode != MD_MODE_OSDT))TambProOsdt=1;
				else if(SystemControl.mdMode == MD_MODE_OSD)TambProOsdt=1;//
				else if(SystemControl.mdMode == MD_MODE_OSDT)TambProOsdt=2;//
			}
			else
			{  //TambCal < tLOTamb_ProMode[0]
				TambProOsdt = 2;
			} 
			
			if(pSystemControl->AutoBits.MODE)
			{
#if  _CFAN_PROTECT_ == 3
				if ( (bCFanTamb && bCFanTwater) != 0)
				{
					SystemControl.mdMode = MD_MODE_OSDT;
				}
#else
				if (sAUTOResult.CFanState == 1 || sAUTOResult.CFanState == 2)  
				{
					SystemControl.mdMode = MD_MODE_OST;    
				}
				else if (sAUTOResult.CFanState == 3)
				{
					SystemControl.mdMode = MD_MODE_OSDT;    
				}
				else if (sAUTOResult.CFanState == 4)
				{
					SystemControl.mdMode = MD_MODE_OSD;    
				}
#endif
				else if (sAUTOResult.HFanState == 1) 
				{
					SystemControl.mdMode = MD_MODE_OST; 
				}
				else if (SystemControl.tset == TSET_LO)  
				{   
					SystemControl.mdMode = MD_MODE_OSF;    
				}
				else if(SystemControl.tset == TSET_HI) 
				{   
					SystemControl.mdMode = MD_MODE_OSD;    
				}
				else if(TambProOsdt == 1)
				{
					pSystemControl->mdMode = MD_MODE_OSD;    //
				}
				else if(TambProOsdt == 2)
				{
					pSystemControl->mdMode = MD_MODE_OSDT;   //    
				}
				else
				{
					if (sTDData.TDResult <= TBL_MODETD[0]) 
					{   
						SystemControl.mdMode = MD_MODE_OSD;
					}
					else if (sTDData.TDResult >= TBL_MODETD[3])    
					{
						pSystemControl->mdMode = MD_MODE_OSF;
					}
					else if (sTDData.TDResult > TBL_MODETD[0] && sTDData.TDResult < TBL_MODETD[1])
					{
						if (SystemControl.mdMode >= MD_MODE_OSD)
						{
							SystemControl.mdMode = MD_MODE_OSD; 
						}
						else
						{//���� 
							SystemControl.mdMode = MD_MODE_OSFD; 
						}
					}
					else if (sTDData.TDResult >= TBL_MODETD[1] && sTDData.TDResult < TBL_MODETD[2])
					{
				   	SystemControl.mdMode =MD_MODE_OSFD;
					}
					else
					{
						if (SystemControl.mdMode == MD_MODE_OSF)
						{
							SystemControl.mdMode = MD_MODE_OSF; 
						}
						else
						{//���� 
							SystemControl.mdMode = MD_MODE_OSFD; 
						}
					}
				}
		   }
			
			
		   //�����Զ�ʱ���� td ���ƺͱ������ƣ� �ó�����ʵ�������fvolt
			if (SystemControl.AutoBits.FAN)
			{
				//����i16ProFanVolt �Զ�����ֵ
				if (SystemControl.tset == TSET_HI)
				{
					i16AutoFanVolt = TBL_AFANVOLT[MAX_AFELVL];
				}
				else if (SystemControl.tset == TSET_LO)
				{
					i16AutoFanVolt = TBL_AFANVOLT[MAX_AFELVL];
				}
				else
				{
					u8NSeg = GAMath_CalDataSegI16(sTDData.TDResult, (int16*)TBL_FANTD, MAX_AFELVL + MAX_AFELVL, 0);
	
					if (u8NSeg < MAX_AFELVL)
					{
						if (u8NSeg == 0)
						{
							i16AutoFanVolt = TBL_AFANVOLT[MAX_AFELVL];
						}
						else
						{
							i16AutoFanVolt = GAMath_LineFuni16(TBL_FANTD[u8NSeg-1], TBL_FANTD[u8NSeg], 
							TBL_AFANVOLT[MAX_AFELVL+1 - u8NSeg], TBL_AFANVOLT[MAX_AFELVL - u8NSeg], sTDData.TDResult); //������ѹ ��Ӧ��Ϊ MAX_AFELVL - ��u8NSeg-1�� ~  MAX_AFELVL - u8NSeMAX_AFELVL
						}
					}
					else if (u8NSeg > MAX_AFELVL)
					{
						//u8NSeg -= MAX_AFELVL;
						if (u8NSeg >= MAX_AFELVL + MAX_AFELVL)
						{
							i16AutoFanVolt = TBL_AFANVOLT[MAX_AFELVL];
						}
						else
						{
							i16AutoFanVolt = GAMath_LineFuni16(TBL_FANTD[u8NSeg-1], TBL_FANTD[u8NSeg], 
							TBL_AFANVOLT[u8NSeg - MAX_AFELVL], TBL_AFANVOLT[u8NSeg + 1 - MAX_AFELVL], sTDData.TDResult); 
						}
					}
					else
					{//==MAX_AFELVL
						i16AutoFanVolt = TBL_AFANVOLT[1];
					}
				}
				//ȡ���Զ�����ķ����ĵ�ѹֵ
				if(sAUTOResult.CFanState != 0)//��籣��
				{
					if (i16ProFanVolt < i16AutoFanVolt)
					{
						sAUTOResult.aFanVolt = i16ProFanVolt;
					}
					else 
					{
						sAUTOResult.aFanVolt = i16AutoFanVolt;
					}
				}
				else if(sAUTOResult.HFanState == 1)
				{//�ȷ籣����һ�׶�Ϊ
					sAUTOResult.aFanVolt = fVoltBLR_1A; 
				}
				else
				{//lohi ���Զ�ģʽ��
					sAUTOResult.aFanVolt = i16AutoFanVolt;
				}
				//���٣�����ѭ������
				if (SystemControl.ncfMode == NCF_MODE_NEW)
				{
					sAUTOResult.aFanVolt = (u8BV_FRESH * sAUTOResult.aFanVolt)/ 100;
					AutoFanVoltType fVoltBVSPEED;
					if (sSenSorData.u8VehicleSpeed >= 100)
					{
						fVoltBVSPEED = fVoltBLR_VSMAX;
					}
					else if (sSenSorData.u8VehicleSpeed <= 10)
					{
						fVoltBVSPEED = 0;
					}
					else
					{
						fVoltBVSPEED = (sSenSorData.u8VehicleSpeed - 10) * fVoltBLR_VSMAX / 90;
					}
					
					if (sAUTOResult.aFanVolt >= fVoltBVSPEED)
					{
						sAUTOResult.aFanVolt -= fVoltBVSPEED;
					}else{}
				}
				else{}
				
				//����
				if (sAUTOResult.aFanVolt > TBL_AFANVOLT[MAX_AFELVL]) sAUTOResult.aFanVolt = TBL_AFANVOLT[MAX_AFELVL];
				if (sAUTOResult.CFanState != 1)
				{
					if (sAUTOResult.aFanVolt <= TBL_AFANVOLT[1]) 
					{
						sAUTOResult.aFanVolt = TBL_AFANVOLT[1];	
					}
					else{}
				}else{}	
			}
			else{} //����auto�������������	
			
			//��ϵ��������OFFʱ������
	#if   HMIX_VENTPID_EN == 0
			if (SystemControl.tset == TSET_LO)
			{
				sAUTOResult.i16Hmix = 0;
			}
			else if (SystemControl.tset == TSET_HI)
			{
				sAUTOResult.i16Hmix = 1000;
			}
			else
			{
				sAUTOResult.i16Hmix = MathG_GetSegLinei16(sTDData.TDResult, TBL_TEMPMOTOR_TD, TBL_TEMPMOTOR_POSTION, TD_MIXSEG_MAXNUM);
			}
	#else	
			if (bOffState)
			{
				bOffState = 0;//��һ�ν���ʱ��ʼ��һ��
				sAUTOResult.i16Hmix = HmixPID_Init(); 
			}
			else{}
			
			sAUTOResult.i16Hmix -= Deal_MIXVENTPID();
			if (sAUTOResult.i16Hmix <= 0)
			{
				sAUTOResult.i16Hmix = 0;
			}
			else if (sAUTOResult.i16Hmix >= 1000)
			{
				sAUTOResult.i16Hmix = 1000;
			}else{}
	#endif
		}
		else
		{//offʱ ����
			bOffState = 1;
		}

		sAUTOResult.AC = pSystemControl->acMode;
		sAUTOResult.NCF = pSystemControl->ncfMode;
		sAUTOResult.MODE = pSystemControl->mdMode;	
	}//100ms����һ��
	else
	{
	}
}

/*******************************************************************************
 * Function:        uint16 Deal_AUTOTD(enum  AUTOTYPE u8TDType)
 *
 * Description:     �Զ����ƣ������Զ��Ľ��
 *
 * Returns:         AUTOTD_TYPE_AC       �Զ��ı� pSystemControl  ���� ac  ״̬
 * 					  AUTOTD_TYPE_NCF      �Զ��ı� pSystemControl  ���� ncf ״̬
 *                  AUTOTD_TYPE_MODE     �Զ��ı� pSystemControl  ���� mode ״̬
 *                  AUTOTD_TYPE_MIX      ���ص�����ưٷֱȣ� ��ǧ��֮����
 *                  AUTOTD_TYPE_FAN      ���عķ����ѹ  �� ��ѹ��10
 * Notes:          
 *
********************************************************************************/
int16 AUTOTD_GetResult(AUTOTYPE u8TDType)
{
	int16 i16RetData = 0;
	switch(u8TDType)
	{
		default:
			break;
		case	AUTOTD_TYPE_AC:
			//ac �Զ�����
			i16RetData = sAUTOResult.AC;
			break;
		case	AUTOTD_TYPE_NCF:
			i16RetData = sAUTOResult.NCF;
			break;
		case	AUTOTD_TYPE_MODE:
			i16RetData = sAUTOResult.MODE;
			break;
		case AUTOTD_TYPE_MIX:
			i16RetData = sAUTOResult.i16Hmix;
			break;
		case AUTOTD_TYPE_FAN:
			i16RetData = sAUTOResult.aFanVolt;
			break;
	}
	return i16RetData;
}


/*******************************************************************************
 * Function:        void AUTOTD_Init(void)
 *
 * Description:     �Զ����� ��ʼ��
 *
 * Returns:         none
 * Notes:          
 *
********************************************************************************/
void AUTOTD_Init(void)
{
	StartCfan = 1;   //��翪��׼�������ж��Ƿ����
	StartHfan = 1;   //�ȷ翪��׼�������ж��Ƿ����
	bOffState = 1;
	u16StartTimes = 0;
		
	sAUTOResult.CFanState = 0;
	sAUTOResult.HFanState =0;
	sAUTOResult.i16Hmix = HmixPID_Init(); 
}

/*******************************************************************************
 * Function:        TDDATA AUTOTD_GetTD(void)
 *
 * Description:     �Զ�����TDֵ
 *
 * Returns:         none
 * Notes:          
 *
********************************************************************************/
TDDATA AUTOTD_GetTD(void)
{
	return sTDData;
}

/*******************************************************************************
 * Function:        AUTORESULT AUTOTD_GetARes(void)
 *
 * Description:     �õ��Զ�������궨ģ��ʹ��
 *
 * Returns:         none
 * Notes:          
 *
********************************************************************************/
AUTORESULT AUTOTD_GetARes(void)
{
	return sAUTOResult;
}
