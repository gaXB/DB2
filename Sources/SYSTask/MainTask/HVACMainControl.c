/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�HVACMainControl.c
* ����������ʵ��HVAC ���Ʋ��߼��� ���������������ʵ��
* 1 �����߼����ĵ���Ҫ�ļ�����ϵͳ���Ʊ��� SystemControl ����sDisplayData �� sOutData 
* 
* �������� codewarrior IDE 11.1
* MCU˵���� ��MCU�޹أ��������ø���
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��3��14��
* 
* �汾��1.1
* ˵��������LCD������
* ���ߣ�xb
* ������ڣ�2020��6��3��
*
* �汾��1.2
* ˵��������LED ģʽ��OFF ʱ����ҪΪ1
* ���ߣ�xb
* ������ڣ�2020��6��3��
* 
* �汾��1.3
* ˵������������ģ���Ż� SystemControl.fanmode �����ģ��õ�
* ���ߣ�xb
* ������ڣ�2020��7��6��
* 
* �汾��1.4
* ˵����
* 1 	offʱ //  SystemControl.fanMode = FAN_MODE_OFF; ���δ˾�  
* 2   �綯ʱ �Զ������ʱ�޷��������
* ���ߣ�xb
* ������ڣ�2020��11��21��
* 
* �汾��1.5
* ˵����
* 1 	���ӷ�����ʾ����ѡ��Ĺ���
* ���ߣ�xb
* ������ڣ�2021��4��6��
* 
* �汾��1.6
* ˵����
* 1 	ö�����͸�ֵ�Ż�
* ���ߣ�xb
* ������ڣ�2021��8��23��
*
*/
#include "APPDrive.h"
#include "KeyPadLogic.h"
#include "Parameter.h"

//����������ؼĴ���
static uint8 u8FanCurLevel;                  //��ǰ������λ
static uint16 vFanVotlOut;
//ac ������ؼĴ���
static uint16 TimerACInitDelay;    //ac ������ʱ����
static uint8 bACDelayEN=0;         //ac ����ʹ��
static uint8 bTevpAC;
       
//��ʱ�������
static uint16 nDelayDis, nDelayOut;
/**************************�����ò���****************************************/
#define  DISDELAY_TIMES      0         //*10ms
#define	OUTDELAY_TIMES     100        //*10ms



extern void EVACControl(void);

/*******************************************************************************
 * Function:   void  RdefControl(void)
 *
 * Description: ���˪����
 *
 * Returns:     none
 *
 * Notes:     1 ʵ�� ���˪�����߼�
 * 
*******************************************************************************/
static void RdefControl(void)
{
	//TODO:���˪�߼������ʵ��
	uint8 rdefState;
   static RdefModeType u8LastState;
   //rdef ������ؼĴ���
   static uint16 TimerRHeat;              //���˪����
   //�ձ��߼�Ϊ���˪���ƿ���15���ӣ��Զ������ ����Ϊ����
   //���˪�����ڰ�����ʵ�֣��л�ʱ��Ҫ���¶�ʱ  TimerRHeat
   
   if (SystemControl.RdefMode != u8LastState)
   {
   	u8LastState = SystemControl.RdefMode;
   	TimeOutChkSLong(&TimerRHeat, 0);
   }else{}
   
   if(SystemControl.RdefMode == RDEF_MODE_ON )
   {
      if(TimeOutChkSLong(&TimerRHeat, 900))
      {
         SystemControl.RdefMode = RDEF_MODE_OFF;
      }else{}
      rdefState = 1;
   }
   else
   {
      rdefState = 0;
   }
   
   sOutData.RHeatOut = rdefState;
}


/******************************************************************************
 * Function:   void  ACControl(void)
 *
 * Description: AC���������ָʾ��
 *
 * Returns:     none
 *
 * Notes:     ac ��������Ϊ
 *          1 ��������
 *          2 ������
 *          3 ������ʱ
 *          4 ѹ������
 *
*******************************************************************************/
static void ACControl(void)
{
	//TODO:AC�����߼������ʵ�� 
	static  uint8 sg_tambac=0;           //����ACʹ��
	
   //AC ������ʱ
   if(TimeOutChkTenthSLong(&TimerACInitDelay,35))
   {
      bACDelayEN=1;
   }
   
   //���±���
   if(sSenSorData.TambCal < tAC_TAMBPRO_ON)
   { 
	   sg_tambac=1;     //
   }else{}
   
   if(sSenSorData.TambCal > tAC_TAMBPRO_OFF)
   { 
	   sg_tambac=0;
   }else{}

   //ȷ��������˪��
   if (sSenSorData.TevpCal > tACON_EVP)
   {
   	  bTevpAC = 0;
   }
   
   if (sSenSorData.TevpCal < tACOFF_EVP)
   {
   	 bTevpAC = 1;
   }

   //����״̬�ж� AC������� ָʾ��
   if(SystemControl.OffState ||SystemControl.fanMode ==FAN_MODE_OFF)
   {  //
      sOutData.ACOut = 0;
   }
   else
   {
      if(SystemControl.acMode &&!sg_tambac && !bTevpAC && bACDelayEN)
      {
      	sOutData.ACOut = 1;
      }  
      else 
      {
      	sOutData.ACOut = 0;
      }
   }
}
/*******************************************************************************
 * Function:   void  NCFControl(void)
 *
 * Description: ����ѭ������
 *
 * Returns:     none
 *
 * Notes:     1 ���ݵ�ǰ����״̬��SystemControl.ncfMode���趨���
 *              λ�ü�ָʾ��
 *
*******************************************************************************/
static void NCFControl(void)
{
	//TODO:�����߼������ʵ��
	if(SystemControl.ncfMode == NCF_MODE_CIRF)	
	{
		sOutData.NCFAD = 1;
	}
	else
	{	
		sOutData.NCFAD = 0;
	}
}
/*******************************************************************************
 * Function:   void  MixMotorControl(void)
 *
 * Description: ��ϵ������
 *
 * Returns:     none
 *
 * Notes:      1 ��AUTOTDģ��õ���ǰ��λ��ֵMixAD
 *              ���趨���λ��
*******************************************************************************/
static void MixMotorControl(void)
{
	//TODO:����߼������ʵ��
   uint16 MixAD;
	if (!SystemControl.OffState)
   {
   	if (BUSCONFIG_USE_ATC == 0)
   	{
   		MixAD = GAMath_LineFuni16(TSET_LO, TSET_HI, u8MIXMOTOR_LO_AD, u8MIXMOTOR_HI_AD, SystemControl.tset);
   	}
   	else
   	{
#if  _PROJECT_AUTO_
   		MixAD =  AUTOTD_GetResult(AUTOTD_TYPE_MIX);      //�õ�ADֵ   	
   		MixAD = GAMath_LineFuni16(0, 1000, u8MIXMOTOR_LO_AD, u8MIXMOTOR_HI_AD, MixAD);//�õ�ǧ�ֱ� ,���ô˺���
#else
   		MixAD = GAMath_LineFuni16(TSET_LO, TSET_HI, u8MIXMOTOR_LO_AD, u8MIXMOTOR_HI_AD, SystemControl.tset);
#endif
   	}
   	
   	sOutData.MIXAD = (uint8)MixAD;
   }
}

/*******************************************************************************
 * Function:   void  MdMotorControl(void)
 *
 * Description: ��ϵ������
 *
 * Returns:     none
 *
 * Notes:     1 ���ݵ�ǰģʽ��SystemControl.mdMode)�趨�����λ��
 *            �� ָʾ�Ƶ�״̬
 *
*******************************************************************************/
static void MdMotorControl(void)
{
	//TODO:ģʽ�߼������ʵ��
	uint8    oset;

	if(SystemControl.mdMode <= MD_MODE_OST)
	{//SystemControl.mdMode off
   	oset = TBL_MODE_OSET[SystemControl.mdMode];
	}
	else
	{
		oset = TBL_MODE_OSET[MD_MODE_OST];
	}
	
	sOutData.MODEAD = oset;
}

/*******************************************************************************
 * Function:   void  FanControl(void)
 *
 * Description: ��������
 *
 * Returns:     none
 *
 * Notes:    1 �Զ����ƹ���,����ȷ籣������, ��AUTO_TD��ʵ��
 *           2 ���� FanVolt_Control() ģ���ȶ���ѹ
 *           3 ������ֻ��Ҫ���õ�ѹ�������ٶȣ�����ѹ�ȶ�ģ����ʵ�ֹ���
 *           
*******************************************************************************/
static void FanControl(void)
{
	//TODO:�����߼������ʵ��
	if(SystemControl.OffState)
	{  //off ֱ�ӵ�0
	  u8FanCurLevel = 0;
	//  SystemControl.fanMode = FAN_MODE_OFF; �����δ˾�  saveoff�� loadoff �����ܼ���
	  sOutData.FANVolt = 0;
	}
	else
	{
		if(!SystemControl.AutoBits.FAN)
		{//
			if(SystemControl.fanMode > MAX_FELVL) 
			{
				SystemControl.fanMode = MAX_FELVL;
			}else{}
			//�����Զ����ݵ�λ�ó�faset 
			vFanVotlOut = TBL_FANVOLT[SystemControl.fanMode];
	
			sOutData.FANVolt = vFanVotlOut; 
		}
		else
		{//�Զ�ʱ��faset ����
#if _PROJECT_AUTO_
			vFanVotlOut = AUTOTD_GetResult(AUTOTD_TYPE_FAN);
			
			SystemControl.fanMode = FanControl_GetCurrentLevel();  //�Զ�ʱ����λ�������λȷ��
			
			sOutData.FANVolt = vFanVotlOut;
			(void)FanControl_SetRate(FANVOLT_RATE_INIT);
#else 
			vFanVotlOut = TBL_FANVOLT[SystemControl.fanMode];
			sOutData.FANVolt = vFanVotlOut;
#endif
		}
#if	_APPCONFIG_FANDISPLAY_ == 1 
		u8FanCurLevel = FanControl_GetCurrentLevel(); 
#else
		u8FanCurLevel = SystemControl.fanMode; 
#endif
	}
}


/*******************************************************************************
 * Function:   void  DisplayDataSet(void)
 *
 * Description: DisplayData Set
 *
 * Returns:     none
 *
 * Notes:     ��sDisplayData ��������
 *
*******************************************************************************/
static void DisplayDataSet(void)
{	
	//TODO: �ڴ˱�д����sDisplayData
	sDisplayData.LEDMode = 1;   //�ر�֮��led ������Ҫ��������
	if (SystemControl.OffState)
	{//OFF STATE
		sDisplayData.FANLevel = 0;
		sDisplayData.AC = 0;
		sDisplayData.AUTO = 0;
		sDisplayData.LCDDiagMode = 0;
		sDisplayData.LCDMode = 0;
		sDisplayData.MAXAC = 0;
		sDisplayData.MODE_MAXDEF = 0;
		sDisplayData.VentMode = MD_MODE_OSF;
	}
	else
	{
		sDisplayData.FANLevel = u8FanCurLevel;
      if (SystemControl.acMode == AC_MODE_ON || SystemControl.AutoBits.AC)
      {
      	sDisplayData.AC = 1;
      }
      else 
      {
      	sDisplayData.AC = 0;
      }
		if (SystemControl.AutoBits.AC && SystemControl.AutoBits.FAN && 
				SystemControl.AutoBits.MODE)
		{
			sDisplayData.AUTO = 1;
		}
		else
		{
			sDisplayData.AUTO = 0;
		}

		sDisplayData.LCDDiagMode = 0;
		sDisplayData.LCDMode = 1;
		if (SystemControl.MaxACMode)
		{
			sDisplayData.MAXAC = 1;
		}
		else
		{
			sDisplayData.MAXAC = 0;
		}
		if (SystemControl.MaxDefMode)
		{
			sDisplayData.MODE_MAXDEF = 1;
		}
		else
		{
			sDisplayData.MODE_MAXDEF = 0;
		}

		sDisplayData.VentMode = SystemControl.mdMode;
	
	}
			
	sDisplayData.i16Temp = SystemControl.tset;
	if (SystemControl.RdefMode == RDEF_MODE_ON)
	{
		sDisplayData.RHEAT = 1;
	}
	else
	{
		sDisplayData.RHEAT = 0;
	}
	//sDisplayData.RHEAT = sSenSorData.RHeat_FB;
	
	if (SystemControl.ncfMode == NCF_MODE_CIRF)
	{
		sDisplayData.CIRF = 1;
		sDisplayData.NEW = 0;
	}
	else
	{
		sDisplayData.CIRF = 0;
		sDisplayData.NEW = 1;
	}
}


/*******************************************************************************
 * Function:   void  OtherOutControl(void)
 *
 * Description: �����������
 *
 * Returns:     none
 *
 * Notes:     ������Ҫ����Ӧ�ò�����н���loop�ĺ���
 *
*******************************************************************************/
static inline void LCDSetControl(void)
{	
	//TODO: �ڴ˱�д����LCD_Dsiplayģ��
#if  _PROJECT_LCD_
	LCDDATA lLCDData; 
	if(SystemControl.OffState)
	{
		SetLcdDisplayMode(LCD_MODE_CLEAR);
	}
	else
	{
		//AUTO״̬����
		if(SystemControl.OffState ==0  &&                 //ȫ�Զ��Ҳ���off
		SystemControl.AutoBits.AC   &&
		SystemControl.AutoBits.MODE && SystemControl.AutoBits.FAN)
		{ //auto led
			lLCDData.SegAuto = 1;
		}
		else
		{
			lLCDData.SegAuto = 0; 
		}
		SetLcdDisplayMode(LCD_MODE_NORMAL);
		lLCDData.SegAC = SystemControl.acMode;
		lLCDData.SegFanLevel = u8FanCurLevel;
		lLCDData.SegMode = SystemControl.mdMode;
		lLCDData.SegNCF = SystemControl.ncfMode;
		
		if (SystemControl.tset >= TSET_HI)
		{
			lLCDData.i16Temp = TSET_LCD_HI;
		}
		else if (SystemControl.tset <= TSET_LO)
		{
			lLCDData.i16Temp = TSET_LCD_LO;
		}
		else
		{
			lLCDData.i16Temp = SystemControl.tset;
		}
		SetLcdDisplayData(&lLCDData);
	}
#endif
}

static void OtherOutControl(void)
{	
	//TODO:LCD���ƣ� �������Ʒ��ڴ˴�
	LCDSetControl();
}


/*******************************************************************************
 * Function:   void  DelayBSPOut(void)
 *
 * Description: �����������
 *
 * Returns:     none
 *
 * Notes:     ��ʾ����ӳ��߼���
 *
*******************************************************************************/
void  DelayBSPOut(void)
{
	nDelayDis++;
	nDelayOut++;
	if (nDelayDis < DISDELAY_TIMES)
	{//TODO: �ڴ˱�д�ر���ʾ����Ŀ���������Ҫ�ɲ�д
		//sDisplayData.AC = 0;
	}
	else
	{
		nDelayDis = DISDELAY_TIMES;
	}
	
	
	if (nDelayOut < OUTDELAY_TIMES)
	{//TODO: �ڴ˱�д�ر��������Ŀ���������Ҫ�ɲ�д
		//sOutData.ACOut = 0;
	}
	else
	{
		nDelayOut = OUTDELAY_TIMES;
	}
}




/*******************************************************************************
 * Function:   void Run_Normal_Task(void)
 *
 * Description: Ӧ����������systemApp�ڵ�Դ����ʱLOOP����
 *
 * Returns:     none
 *
 * Notes:      ��������������ڴ�����
 *
*******************************************************************************/
void Run_Normal_Task(void)
{
	static uint16 TimerNoramalTask;

#if _PROJECT_AUTO_
	if (BUSCONFIG_USE_ATC)
	{
		Cal_TD();     //td���������綯ʱ�Ὣ autobits =0
		AUTOTD_Control();  //��Ҫ��ع���ʱ�ڽ�����ӣ����ʱ��Ҫע��
	}
	else
#endif
	{
		SystemControl.AutoBits.AC = 0;
		SystemControl.AutoBits.MODE = 0;
		SystemControl.AutoBits.FAN = 0;
		SystemControl.AutoBits.NCF = 0;
	}
	
	if (TimeOutChkMsLong(&TimerNoramalTask, 10))
	{
//		Park_HVAC();         		//פ�����Ƽ�������
		EVACControl();
		
		RdefControl();       		//���˪��������߼������
		
		ACControl();        		//ac��������߼������
		
		NCFControl();       			//�����������߼������
		
		MixMotorControl();  			//��ϵ����������߼�����������˴�Ϊ2����ů��������ڴ˴�
		
		MdMotorControl();   			//ģʽ�����������߼������
		
		FanControl();       			//������������߼������
		
		DisplayDataSet();
/******����Ϊ��Ҫ��ͨ�ã����Ʊ����Ŀ���******/
		
		OtherOutControl();  			//��������
		
		DelayBSPOut();            //�������
	}else{}

	PanleState_Logic();
	Deal_Keypad_Normal();			//���������߼�����
	
}

/*******************************************************************************
 * Function:   void HVACControl_Init(void)
 *
 * Description: ��������Ƶĳ�ʼ������ϵͳ��������ģʽʱ����
 *
 * Returns:     none
 *
 * Notes:      none
 *
*******************************************************************************/
void HVACControl_Init(void)
{
   //TODO: ��дHVACCONTORL ��ʼ��������
	OtherControl.CHeatMode_L = 0;
	OtherControl.CHeatMode_R = 0;
   SystemControl.RdefMode = RDEF_MODE_OFF;
	(void)TimeOutChkTenthSLong(&TimerACInitDelay,0);  //ac ������ʱ
	bACDelayEN = 0;
//	bACProtect = 0;       //AC������ʱ�ĳ�ʼ��Ϊ���Կ���
//	bTevpAC = 1;          //��˪�����ĳ�ʼֵ��Ϊ��ac
	//�ķ��Ϊ0�������
	u8FanCurLevel =0;  
	PanleState_Logic();
	nDelayDis = 0;
	nDelayOut = 0;
}


