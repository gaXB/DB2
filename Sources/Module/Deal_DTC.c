#include "Deal_DTC.h"

/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Deal_DTC.c
* ����������DTC ����ʵ��
* 1  ʵ��DTC ���ܣ�һ�����Ϊ8ms*30/2 = 120ms
* 2  ʵ��DTCNUM_VOLTAGE_HIGH��DTCNUM_VOLTAGE_LOW ��2��DTC�Զ��ж�
* 3  ���ݵ���ѹ�ķ�Χ�����Ƿ���DTC����
* 4  ����DTC �ϻ����ܣ�DTC ���չ���
* 5  ��ʷ������Ҫ���   DTC_CONFIRM_TIMES ��
* 6  ����������ԣ���DTC �仯�Ҵﵽһ��ʱ����ٽ��м���
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * ���ļ����ܸ���
* �汾��1.0
* ˵������ԭ1.3 �汾���Ĺ��������ӵ�ѹ�жϹ��ܣ�
* ԭ���ߣ�xb
* ������ڣ�2019��10��25��
* 
* �汾��2.0
* ˵������Begin_DTCCycle/End_DTCCycle �ɱ�ģ��ʵ�֣��ⲿ����Ҫ����
* ԭ���ߣ�xb
* ������ڣ�2019��10��29��
*
* �汾��3.0
* ˵���� �ߵ�ѹ���Ϸ�
* ���ߣ�xb
* ������ڣ�2020��1��7��
*/



static int8 nErrTimes[DTC_MAXNUM];     //���ϴ���
uint8 DTC[DTC_MAXNUM];                 //������
DTCSNAPDATA DTCSnap[DTC_MAXNUM];

static uint8 DTCTotalNum;

#if   _SAVE_DTC_
static uint8 bSaveDTC = 1;
#endif

static uint8 u8DTCCycState;   //

#define  DTCCYC_STATE_INIT      0
#define  DTCCYC_STATE_WORK      0
#define  DTCCYC_STATE_OVER      1
typedef enum
{ 
	DTC_VOLT_INIT = 0,              //��ʼ��
	DTC_VOLT_OK = 1,                //����,ֻ���ڴ˽׶����жϹ�����
	DTC_VOLT_ERR = 2,               //����
	DTC_VOLT_TIME = 3,              //��ʱ
}eDTCVoltState;
//��ѹ״̬����
static eDTCVoltState  DTCVoltState = DTC_VOLT_INIT;
static eDTCRETSTATE  u8VoltLow, u8VoltHigh;
//�ⲿ��������
extern void Save_DTC(void);
extern uint8 Load_DTC(void);
extern uint8 DTCStorGet(void);

//��ģ��ʵ�ֺ���
static void DTC_AgeDTC(void);
static void Begin_DTCCycle(void);
static void End_DTCCycle(void);
/*******************************************************************************
* Function:   void DeaVolt_DTC(void)
*
* Description:  ������ĵ�ѹ����
*
* Returns:      none
*
* Notes:       
*
*******************************************************************************/
static void DeaVolt_DTC(void)
{
	int16 i16Volt = ADPhy_GetFact(ADID_VOLT);
   static uint16 TimerVoltLow, TimerVoltHigh;
   static uint16 TimerVoltDTC;
   
	switch(DTCVoltState)
	{
	default:
	case  DTC_VOLT_INIT:
		if (TimeOutChkMsLong(&TimerVoltDTC, DTCVOLT_INIT_DELAY))
		{  //��ʼ��1500ms
			if (i16Volt > DTC_HVOLT_DISABLE || i16Volt < DTC_LVOLT_DISABLE)
			{
				DTCVoltState = DTC_VOLT_ERR; 
			}
			else
			{
				DTCVoltState = DTC_VOLT_OK;
			}
		}
		else{}
		break;
	case  DTC_VOLT_OK:
		if (i16Volt > DTC_HVOLT_DISABLE || i16Volt < DTC_LVOLT_DISABLE)
		{
			DTCVoltState = DTC_VOLT_ERR; 
		}
		else{}
		break;
	case  DTC_VOLT_ERR:
		if(i16Volt < DTC_HVOLT_ENABLE && i16Volt > DTC_LVOLT_ENABLE)
		{
			(void)TimeOutChkMsLong(&TimerVoltDTC, 0) ;
			DTCVoltState = DTC_VOLT_TIME; 
		}
		else{}
		break;
	case  DTC_VOLT_TIME:
		if (i16Volt > DTC_HVOLT_DISABLE || i16Volt < DTC_LVOLT_DISABLE)
		{
			(void)TimeOutChkMsLong(&TimerVoltDTC,0) ;  
		}
		else{}
		if(TimeOutChkMsLong(&TimerVoltDTC, DTCVOLT_RECOVER_DELAY)) 
		{
			DTCVoltState = DTC_VOLT_OK;  
		}
		else{}
		break;
	}
	
	if (i16Volt < DTC_EVOLT_LOW_L)
	{//��Ϊ��Դ�رգ�DTC �������жϣ�
		DTCVoltState = DTC_VOLT_INIT;
		if (u8DTCCycState == DTCCYC_STATE_WORK)
		{
			u8DTCCycState = DTCCYC_STATE_OVER;
			End_DTCCycle();
		}
		else{}
	}
	else if (i16Volt > DTC_LVOLT_ENABLE)
	{
		if (u8DTCCycState == DTCCYC_STATE_OVER)
		{
			u8DTCCycState = DTCCYC_STATE_WORK;
			Begin_DTCCycle();
		}
		else{}
	}
	else{}
	
	
	//����ʵ�ֵ͵�ѹ���ϵĹ���
	i16Volt = ADPhy_GetFact(DTC_EVOLT_ADID);
	if (i16Volt < DTC_EVOLT_LOW_H && i16Volt > DTC_EVOLT_LOW_L)
	{
		if (TimeOutChkMsLong(&TimerVoltLow, DTC_EVOLT_TIME))
		{
			u8VoltLow = DTC_RETSTATE_ERR;
		}else{}
	}
	else
	{
		(void)TimeOutChkMsLong(&TimerVoltLow, 0);
		
		if (i16Volt > DTC_EVOLT_LOW_OK)
		{
			u8VoltLow = DTC_RETSTATE_OK;
		}else{}
	}
	
	//����ʵ�ָߵ�ѹ���ϵĹ���
	if (i16Volt > DTC_EVOLT_HIGH)
	{
		if (TimeOutChkMsLong(&TimerVoltHigh, DTC_EVOLT_TIME))
		{
			u8VoltHigh = DTC_RETSTATE_ERR;
		}
		else{}
	}
	else
	{
		(void)TimeOutChkMsLong(&TimerVoltHigh, 0);
		
		if (i16Volt < DTC_EVOLT_HIGH_OK)
		{
			u8VoltHigh = DTC_RETSTATE_OK;
		}
		else{}
	}
}
 /*******************************************************************************
 * Function:   BOOL Init_DealDTC(uint8 MaxNum)
 *
 * Description:  �����봦���ʼ������
 *
 * Returns:      �趨�ɹ� ���أ�1
 *
 * Notes:       MaxNum ������  
 *
*******************************************************************************/
BOOL Init_DealDTC(uint8 MaxNum)
{
   BOOL bInitOK;
	if (MaxNum <= DTC_MAXNUM)
   {
      DTCTotalNum = MaxNum ;
      u8DTCCycState = DTCCYC_STATE_WORK;
      Begin_DTCCycle();
      bInitOK = TRUE;
   }
	else
	{
		bInitOK = FALSE;
	}
	DTCVoltState = DTC_VOLT_INIT;
	
	u8DTCCycState = 0;
	return bInitOK;
}


/*******************************************************************************
* Function:   void DTC_AgeDTC(void))
*
* Description:  �����ϻ�һ��
*
* Returns:      void
*
* Notes:     ģ�龲̬����
*
*******************************************************************************/
static void DTC_AgeDTC(void)
{
  uint8 i;
  
	for (i = 0; i< DTC_MAXNUM; i++)
	{
     //DTC[num] |= DTCB_FAILED_THISOC;
		if ( (DTC[i] & DTCB_TESTNOTCOMPLETED) == 0)
		{//�Ѳ������
			if (DTC[i] & DTCB_FAILED_THISOC) 
			{ //�й���
				DTCSnap[i].DTCAgeTimes = 0; 
			}
			else
			{//ageing
				if (DTCSnap[i].DTCAgeTimes >= DTC_AGE_TIMES)
				{
					DTC[i] &= ~DTCB_TESTFAILED_SINCECLEAR;
					DTC[i] &= ~DTCB_CONFIRM;  //ageing dtc
				}
				else
				{
					DTCSnap[i].DTCAgeTimes++;
				}
			  
				DTCSnap[i].DTCPendTimes = 0;
				DTC[i] &= ~DTCB_PENDING;
		  }
		}
		else
		{
   	  
		}
	}

#if   _SAVE_DTC_
   bSaveDTC = 1;       //���¼���
#else
	Save_DTC() ;    
#endif 
}

 /*******************************************************************************
 * Function:   void Begin_NewCycle(void)
 *
 * Description:  ��ʼ�µ�ѭ��
 *
 * Returns:      none
 *
 * Notes:     ��ʼֵ�� DTCB_TESTFAILED = 0    DTCB_FAILED_THISOC=0
 *            DTCB_PENDING  DTCB_CONFIRM ����ֵ     DTCB_TESTNOTCOMPLETED_SINCECLEAR = 1����������߼���
 *            DTCB_TESTFAILED_SINCECLEAR ����ֵ     DTCB_TESTNOTCOMPLETED = 1
*******************************************************************************/
static void Begin_DTCCycle(void)
{
	uint8 i;

	if (Load_DTC())
	{	//������ȷ��
#if   _SAVE_DTC_
		//�����µ�洢�����ϵ��ϻ�һ��
		DTC_AgeDTC();	
#endif
	}
	else
	{
	}
	
	for (i = 0; i < DTCTotalNum; i++)
	{
		DTC[i] &= ~DTCB_TESTFAILED;  
		DTC[i] &= ~DTCB_FAILED_THISOC; 
		DTC[i] |= DTCB_TESTNOTCOMPLETED;   
		nErrTimes[i] = 0;      
	}  
}

 /*******************************************************************************
 * Function:   void  ClearDTC(uint8 nId)
 *
 * Description:  ���dtc
 *
 * Returns:      none
 *
 * Notes:     nId ��һ��Id
*******************************************************************************/
void  ClearDTC(uint8 nId)
{
   uint8 i;
   if (nId == 0xff)
   {
      for(i = 0; i < DTCTotalNum; i++)
      {
         DTC[i] = DTCB_TESTNOTCOMPLETED_SINCECLEAR | DTCB_TESTNOTCOMPLETED;   
         nErrTimes[i] = 0;
         DTCSnap[i].DTCPendTimes = 0;
      }
#if   _SAVE_DTC_
      bSaveDTC = 1;       //���¼���
#endif
   }
   else if (nId < DTCTotalNum) 
   {
      DTC[nId] = DTCB_TESTNOTCOMPLETED_SINCECLEAR | DTCB_TESTNOTCOMPLETED;   
      nErrTimes[nId] = 0;
      DTCSnap[nId].DTCPendTimes = 0;
#if   _SAVE_DTC_
      bSaveDTC = 1;       //���¼���
#endif
   }
   else{}
}

 /*******************************************************************************
 * Function:   uint8 Deal_DTC(void))
 *
 * Description:  �����봦�����˺�������main-loop ��
 *
 * Returns:      �����˷��أ�1 �� ʱ��δ������0�� û�д�����0xff
 *
 * Notes:   
 *
*******************************************************************************/
uint8 Deal_DTC(void)
{
   static uint16  TimerDealDTC;
   static uint16 u16SaveTimes;
   uint8 num;
   eDTCRETSTATE u8RetState;
	uint8 u8DealResult = 0;
   if (TimeOutChkMsLong(&TimerDealDTC, DTCDEAL_TIME))
   {	
   	u16SaveTimes++;
   	DeaVolt_DTC();
   	if (DTCVoltState == DTC_VOLT_INIT)
   	{
   		u8DealResult = 0xfe;
   	}
   	else
   	{
	#if   _PROJECT_CAN_   
			if (!DTCStorGet())
			{//�����������.���򷵻�0xff
				u8DealResult = 0xff;
			}
			else
	#endif
			{
				for(num = 0; num < DTCTotalNum; num++)
				{
					if (num == DTCNUM_VOLTAGE_LOW)
					{
						u8RetState = u8VoltLow;
					} 
					else if (num == DTCNUM_VOLTAGE_HIGH)
					{
						u8RetState = u8VoltHigh;
					}
					else
					{
						if (DTCVoltState == DTC_VOLT_OK)
						{
							u8RetState = GetDTCState(num);
						}
						else
						{
							u8RetState = DTC_RETSTATE_WAITE;
						}
					}
					//u8RetState get end
					
					if (u8RetState == DTC_RETSTATE_ERR || u8RetState == DTC_RETSTATE_IMMEDI_ERR || u8RetState == DTC_RETSTATE_COMFIRM_ERR) 
					{
						if ( (nErrTimes[num] >= (DTC_FAILED_TIMES - DTC_FAILED_STEP)) ||  u8RetState == DTC_RETSTATE_IMMEDI_ERR || u8RetState == DTC_RETSTATE_COMFIRM_ERR) 
						{  //���β���ʧ���Ѵﵽ 
							nErrTimes[num] = DTC_FAILED_TIMES;
							if ( !(DTC[num] & DTCB_FAILED_THISOC))
							{//����ѭ����һ�ι���
								DTC[num] |= DTCB_FAILED_THISOC;      //���β��Թ���
								
								DTCSnap[num].DTCPendTimes++;
								if (DTCSnap[num].DTCPendTimes >= DTC_CONFIRM_TIMES || u8RetState == DTC_RETSTATE_COMFIRM_ERR)
								{
									DTCSnap[num].DTCPendTimes = DTC_CONFIRM_TIMES;
									DTC[num] |= DTCB_CONFIRM;
									ReCord_DTCSnap(num);  
								}
								else{}
								
								DTC[num] |= DTCB_PENDING; 
								  
							#if   _SAVE_DTC_
								bSaveDTC = 1 ;       //���¼���
							#endif
							}
							else
							{//�Ѿ����Ϲ���
								
							}
							
							DTC[num] |= DTCB_TESTFAILED;         //��ǰ����ʧ��
							DTC[num] &= ~DTCB_TESTNOTCOMPLETED;  //�����
							
							if (DTC[num] & DTCB_TESTNOTCOMPLETED_SINCECLEAR)
							{
								DTC[num] &= ~DTCB_TESTNOTCOMPLETED_SINCECLEAR ; //����ɣ���0 ����Ҫ���� 
							#if   _SAVE_DTC_
								bSaveDTC = 1 ;       //���¼���
							#endif  
							}
				
							if (!(DTC[num] & DTCB_TESTFAILED_SINCECLEAR) )
							{//�Դ�������ֹ���
								DTC[num] |= DTCB_TESTFAILED_SINCECLEAR;
							#if   _SAVE_DTC_
								bSaveDTC = 1 ;       //���¼���
							#endif 
							}
						}
						else
						{ 
							nErrTimes[num]+= DTC_FAILED_STEP; 
						}
					}
					else if (u8RetState == DTC_RETSTATE_OK || u8RetState == DTC_RETSTATE_IMMEDI_OK) 
					{//û�й���
						if ( nErrTimes[num] <= (DTC_PASSED_TIMES + DTC_PASSED_STEP) || u8RetState == DTC_RETSTATE_IMMEDI_OK )    // DTC_PASSED_TIMES ����Ϊ-
						{//û�й����Ѿ���һ����
							nErrTimes[num] = DTC_PASSED_TIMES;
							// DTCB_FAILED_THISOC;DTCB_PENDING,DTCB_CONFIRM,DTCB_TESTFAILED_SINCECLEAR   //�������
							DTC[num] &= ~DTCB_TESTFAILED; 
							DTC[num] &= ~DTCB_TESTNOTCOMPLETED;//�����,��0
							if (DTC[num] & DTCB_TESTNOTCOMPLETED_SINCECLEAR)
							{ //����ɣ���0 ����Ҫ����
								DTC[num] &= ~DTCB_TESTNOTCOMPLETED_SINCECLEAR ;
							#if   _SAVE_DTC_
								bSaveDTC = 1 ;       //���¼���
							#endif  
							}
						}
						else
						{
							nErrTimes[num] -= DTC_PASSED_STEP;  
						}
					}
					else
					{//u8RetState == DTC_RETSTATE_WAITE �����������
						
					}
				}//for
				u8DealResult = 1;
			}//UDS DTC not get
   	}//dtc state init
   }//8ms
   
#if   _SAVE_DTC_
   if (u16SaveTimes >= SAVE_CYC_TIMES)
   {
   	u16SaveTimes = 0;
      if (bSaveDTC && DTCVoltState == DTC_VOLT_OK)
      {//��ѹ�����²Ž��д洢
      	bSaveDTC = 0;
      	Save_DTC();
      }
   }
   else{}
#endif
   return u8DealResult;
}



 /*******************************************************************************
 * Function:   void End_DTCCycle(void))
 *
 * Description:  ����ѭ����������ʼ�ϻ�
 *
 * Returns:      void
 *
 * Notes:   
 *
*******************************************************************************/
static void End_DTCCycle(void)
{
#if   _SAVE_DTC_
	//���ʹ���ϵ��ϻ�����Ҫ���д洢
#else
	DTC_AgeDTC();
#endif
}


