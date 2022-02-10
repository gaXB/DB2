/**************************�ļ�˵��*****************************************************
* Copyright (c) 2021 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�LowerDrive.c
* �����������ײ�������Ҫ���õĺ���
* 1  Init_LowerDrive ��ʼ���ײ�ģ��
* 2  LowDrive_Loop  �ײ�������ѭ���������غ���
* 3  LowDrive_PowerON / LowDrive_PowerOFF �ײ������ڵ�Դ����ʱ�õ��õ����غ���
* 
* �������� codewarrior IDE 11.1
* MCU˵������MCU �޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��7��25��
* 
* �汾��1.1
* ˵����1  ����MPULOGIC ������
*       2  �����������ǿ�ƹ���
* ���ߣ�xb
* ������ڣ�2020��6��1��
* 
* 
* �汾��1.2
* ˵����1 ���ӹķ������ֵ�Ĺ���
* ���ߣ�xb
* ������ڣ�2020��7��6��

* �汾��1.3
* ˵����1 �Ż���ͬ�궨�������������
* ���ߣ�xb
* ������ڣ�2020��9��27��
* 
* �汾��1.4
* ˵����1 loop��poweroff ����ȡ��
* ���ߣ�xb
* ������ڣ�2020��9��27��
* 
* �汾��1.5
* ˵����1 �������ǰ���;�������
* ���ߣ�xb
* ������ڣ�2020��12��19��
* 
* �汾��1.6
* ˵����1 �жϷŵ�hardwarddrive
* ���ߣ�xb
* ������ڣ�2021��8��23��
*/
#include "LibDriveDefine.h"

/*******************************************************************************
 * ��������
 *
*******************************************************************************/
static uint32 u32TestLoopFreq;        //�������е�Ƶ�ʣ�/1s
static uint16 u16TestLoopTime;        //�������е����ʱ��ms
static uint8 u8InitErrNum; 

volatile uint8 u8TestByte;
extern VoltType  TBL_FANVOLT[MAX_FELVL+1];

extern void InterLay2Init(void);
/*************************************************************************************************
�¶���ֵ��
****************************************************************************************************/
//TODO: ���ò�ͬ���¶���ֵ����˵����������
static const uint16  TBL_TAMB_SAIC_51[] = //�Ѹ���
{
		998, 988, 974, 957, 934, 907, 873, 834, 788, 738,
		684, 627, 569, 512, 456, 403, 354, 310, 270, 235,
		204, 177, 153, 133, 115,
};


static const uint16 TBL_SENSOR3K[] =
{
		986, 971, 952, 927, 896, 858, 813, 763, 707, 648,
		587, 525, 466, 409, 357, 309, 267, 230, 197, 169,
		145, 125, 107, 93, 80,
};

static const uint16 TBL_TIN_SAIC_51[] =
{
	922, 900, 866, 826, 782, 732, 678, 622, 565,
	508, 453, 401, 353, 309, 269, 234, 203, 176, 
	152, 131, 115, 99, 86, 74, 65
}; 

static const uint16  TBL_TEVP_SAIC_51[] =
{
	954,930,902,867,827,780,730,675,618,561,
	504,450,398,351,308,269,235,204,178,155,
	135,118,103,90,79
};  

static const  uint16  TBL_INTAKE_SAIC_51[] =
{
		954, 930, 902, 867, 827, 780, 730, 675, 618, 561,
		504, 450, 398, 351, 308, 269, 235, 204, 178, 155,
		135, 118, 103, 90, 79,
};


int16 AD3KValue;


//�ⲿ��������
#if _PROJECT_CAN_
extern void PCSet_SenSorData(void);
extern void PCSet_OutData(void);
extern void InterLay2Task(void);
#endif
/*******************************************************************************
 * Function:   void Sys_Clk(void)
 *
 * Description: ��ʱ����tm ģ����Ҫ�������
 *
 * Returns:     none
 *
 * Notes:     ����Ҫ����
*******************************************************************************/
static void Sys_Clk(void)
{
	TenthSCounterUpadat();
	SCounterUpadat();
}
/*******************************************************************************
 * Function:    void Init_LowerDrive(void)
 *
 * Description: �ײ�������ʼ��
 *
 * Returns:     none
 *
 * Notes:      ����ʵ��Ӧ�ñ�д��������
*******************************************************************************/
static void Moudle_InitErr(void)
{
	u8InitErrNum++;
}


void Init_LowerDrive(void)
{
	//TODO: ��дLIB�ĵײ�����
	const uint16 *TBL_SENSOR[ADID_TNUM] = {0};
	TBL_SENSOR[ADID_TIN-3] = TBL_TIN_SAIC_51;
	TBL_SENSOR[ADID_TAMB-3] = TBL_TAMB_SAIC_51;
	TBL_SENSOR[ADID_TEVP-3] = TBL_TEVP_SAIC_51;
	TBL_SENSOR[ADID_FVENT-3] = TBL_INTAKE_SAIC_51;
	TBL_SENSOR[ADID_DVENT-3] = TBL_INTAKE_SAIC_51;

	
	//eeprom
	FLASH_EE_Init();   //����������ǰ��
	
	(void)ADInit(ADID_MAXNUM, 1);  //�ڶ�������Ϊ��ѹ���������
	if ( ADPhy_Init(ADID_VNUM, ADID_SYSIGN, ADID_TNUM, TBL_SENSOR) )
	{
		//������ȷ
	} 	
	else
	{
		Moudle_InitErr();
	}
	
	
	//motor
	if (Motor_Init(100,1,3))
	{
		
	}
	else
	{
		Moudle_InitErr();
	}
	
	MotorDriveInit(5, 1);  //����Ĭ�ϵĵ�����Ⱥ���ǰ��
	
	if (NcfMotor_Init(150,1))
	{
		
	}
	else
	{
		Moudle_InitErr();
	}
	
	//7708 ��ʼ��
	MCPComPinType TBL_Set7708[6] = {M7708_COMBINPIN2, M7708_COMBINPIN1, M7708_COMBINPIN3, M7708_COMBINPIN4, M7708_COMBINPIN5, M7708_COMBINPIN6};  //7708 �������
	if (Mcp_MotorInit(3, TBL_Set7708))
	{
	}
	else
	{
		Moudle_InitErr();
	}
	
	//SIG
	if (SigF_Init(SIG_MAXNUM))
	{
		
	}
	else
	{
		Moudle_InitErr();
	}
	
	(void)Init_DealDTC(DTC_MAXNUM);

   //fancontorl
	FANCONTROLCONFIG lConfig;
	lConfig.TBL_FanVolt = TBL_FANVOLT;
	lConfig.bSystemVolt24 = 0;
	lConfig.i16FanDFactor = FAN_PID_DFACTOR;
	lConfig.i16FanPFactor = FAN_PID_PFACTOR;
	lConfig.i16FanIFactor = FAN_PID_IFACTOR;
	lConfig.u8SlowAdjFactor = FAN_SLOWADJ_FACTOR;
	lConfig.u16InitRate = 20;
	lConfig.u16MinPwm = 1000;  //10%
	lConfig.u8FanMaxLevel = MAX_FELVL;
	lConfig.u8VoltOffSet = VOUT_OFFSET_DEFAULT;
	
	if (FanControl_Init(&lConfig))
	{
		
	}
	else
	{
		//Moudle_Init();
	}
	
#if  _PROJECT_LCD_	
	Init_LCD();
#endif 	
	
	InitComdQueue();   //���ڿ��ж�ǰ�棬���ģ���ʼ������Ӧ�ÿ��ж�

#if  _PROJECT_CAN_
   InterLayInit();
   InterLay2Init();
#endif
   
/* uint8 TBL_DrivePriode[5] = {16, 16, 16, 12, 0xff};  //8,7,6,5,4 ����������
   STEPMOTOR_INITDATA TBL_Init[5];

	TBL_Init[STEPMOTOR_ID_MODE_OSF].u16MaxStep = MODE_OSF_MAXSTEP;
	TBL_Init[STEPMOTOR_ID_MODE_OSF].u16BackStep = BACK_STEP;
	TBL_Init[STEPMOTOR_ID_MODE_OSF].bForwdReserve = 0;
	
	TBL_Init[STEPMOTOR_ID_MIX].u16MaxStep = MIX_MAXSTEP;
	TBL_Init[STEPMOTOR_ID_MIX].u16BackStep = BACK_STEP;
	TBL_Init[STEPMOTOR_ID_MIX].bForwdReserve = 0;
	
	TBL_Init[STEPMOTOR_ID_MIX_ASSIANT].u16MaxStep = MIX_ASSIANT_MAXSTEP;
	TBL_Init[STEPMOTOR_ID_MIX_ASSIANT].u16BackStep = BACK_STEP;
	TBL_Init[STEPMOTOR_ID_MIX_ASSIANT].bForwdReserve = 0;
	
	TBL_Init[STEPMOTOR_ID_NCF].u16MaxStep = NCF_MAXSTEP;
	TBL_Init[STEPMOTOR_ID_NCF].u16BackStep = BACK_STEP;
	TBL_Init[STEPMOTOR_ID_NCF].bForwdReserve = 0;
	
	TBL_Init[STEPMOTOR_ID_MODE_DEF].u16MaxStep = MODE_DEF_MAXSTEP;
	TBL_Init[STEPMOTOR_ID_MODE_DEF].u16BackStep = BACK_STEP;
	TBL_Init[STEPMOTOR_ID_MODE_DEF].bForwdReserve = 0;
	
	
	Drv8860_Init(2);
	(void)StepMotorInit(4, TBL_Init, TBL_DrivePriode, 5)	;
	StepMotor_IgnRest(0xff);
*/	
	
   //����ΪCAN UDS����Ĺ̶��÷����ö�
#if  _PROJECT_CAN_
	(void)Load_Parameter();    //�������״̬
#endif
	(void)Load_ALLCycEE();     //������������-UDS����
	
#if	_PROJECT_CAN_
	Load_ProgramData();
	Diagnostic_IgnReset();  
	
	if (ProgramData[PD_NUM_SESSIONMODE] == 1)
	{
		ProgramData[PD_NUM_SESSIONMODE] = 0;
		Save_ProgramData();  
		Enter_DefaultSession();
	}
#endif
	MCUPM_ReadEE();

	uint8  codedata[2];
	codedata[CODE_NUM_TSET] = COMD_TSET_ADD;
	codedata[CODE_NUM_FAN] = COMD_FAN_ADD;
	Init_Code35(CODE_NUM_MAX, codedata);
}


//����Ϊ�жϺ���

/*************************************************************************
 * Function:  void LowDrive_Loop(void)
 *
 * Description: lowdrvie ��loop����
 *
 * Returns:     none
 *
 * Notes:    �������ø��ģ��ɰ�ʵ����Ҫ����
 *
*******************************************************************************/
void LowDrive_Loop(void)
{
	Sys_Clk();           //ʱ�Ӻ���
	
	Treat_Read_Ad();     // ad��������ģ�����
	
	SigFControl();       //�ź��˲�ģ��
	
//	Treat_Read_Keypad();
	
	u32TestLoopFreq = TimeOut_TestLoop();
	u16TestLoopTime = TimeOut_GetMaxLoop();

	//����can����Դ�����ڲ�����
#if _PROJECT_CAN_	
	InterLayTask();
	InterLay2Task();
	Deal_UDSNetLay();
	AppLay_DiagControl();	
	
	static uint16 TimerNM;
	if (TimeOutChkTMsLong(&TimerNM, NM_MAINFUNCTION_PERIOD))
	{
		IndirectNMTask();
	}
#endif
#if _PROJECT_CAN_
	PCSet_OutData();
#endif
	OutTask();
	CommSetTask();
//	LEDSetTask();
	Deal_Signal_Refresh();
#if _PROJECT_CAN_
	PCSet_SenSorData();
#endif
	Motor_Control();
//	NcfMotor_Control();
	FanVolt_Control();
	WPumpControl();
	EXVControl();
	//StepMotor_Control();
	//ICDrvie_StepMotor();
	//StepMotor_ICErrRead();
//	Updata_LCD();
	Treat_Read_Code35();
	ReadTouch_MainControl();
	SRealyControl();
	Deal_ActorErr();

	extern int16 MathG_GetSegLinei16_rev(int16 Cvalue,int16* DataX, int16 *Datay, uint8 Num);
	AD3KValue = ADValue_Get(ADID_TAMB, ADVALUE_GETTYPE_10AD);
	AD3KValue = MathG_GetSegLinei16_rev(AD3KValue, (int16*)TBL_TAMB_SAIC_51, (int16*)TBL_SENSOR3K, 25);

}

/*******************************************************************************
 * Function:  void LowDrive_PowerON(void)
 *             void LowDrive_PowerOFF(void)
 * Description: lowdrvie �Ĳ�ͬ��Դʱ���õ����غ���
 *
 * Returns:     none
 *
 * Notes:     �˴����õײ�����������Ҫ�ڵ�ԴΪON /OFFʱ���õ����غ���
 *
*******************************************************************************/
void LowDrive_PowerON(void)
{
#if _PROJECT_LCD_
//	Updata_LCD();
#endif
}

