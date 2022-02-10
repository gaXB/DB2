/**************************文件说明*****************************************************
* Copyright (c) 2021 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：LowerDrive.c
* 功能描述：底层驱动需要调用的函数
* 1  Init_LowerDrive 初始化底层模块
* 2  LowDrive_Loop  底层驱动的循环调用主控函数
* 3  LowDrive_PowerON / LowDrive_PowerOFF 底层驱动在电源正常时该调用的主控函数
* 
* 编译器： codewarrior IDE 11.1
* MCU说明：于MCU 无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年7月25日
* 
* 版本：1.1
* 说明：1  增加MPULOGIC 的驱动
*       2  增加输入输出强制功能
* 作者：xb
* 完成日期：2020年6月1日
* 
* 
* 版本：1.2
* 说明：1 增加鼓风机补偿值的功能
* 作者：xb
* 完成日期：2020年7月6日

* 版本：1.3
* 说明：1 优化不同宏定义编译出错的问题
* 作者：xb
* 完成日期：2020年9月27日
* 
* 版本：1.4
* 说明：1 loop的poweroff 功能取消
* 作者：xb
* 完成日期：2020年9月27日
* 
* 版本：1.5
* 说明：1 电机的提前量和精度设置
* 作者：xb
* 完成日期：2020年12月19日
* 
* 版本：1.6
* 说明：1 中断放到hardwarddrive
* 作者：xb
* 完成日期：2021年8月23日
*/
#include "LibDriveDefine.h"

/*******************************************************************************
 * 变量定义
 *
*******************************************************************************/
static uint32 u32TestLoopFreq;        //程序运行的频率，/1s
static uint16 u16TestLoopTime;        //程序运行的最大时间ms
static uint8 u8InitErrNum; 

volatile uint8 u8TestByte;
extern VoltType  TBL_FANVOLT[MAX_FELVL+1];

extern void InterLay2Init(void);
/*************************************************************************************************
温度阻值表
****************************************************************************************************/
//TODO: 设置不同的温度阻值表，并说明上拉电阻
static const uint16  TBL_TAMB_SAIC_51[] = //已更改
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


//外部函数调用
#if _PROJECT_CAN_
extern void PCSet_SenSorData(void);
extern void PCSet_OutData(void);
extern void InterLay2Task(void);
#endif
/*******************************************************************************
 * Function:   void Sys_Clk(void)
 *
 * Description: 定时器，tm 模块需要必须调用
 *
 * Returns:     none
 *
 * Notes:     不需要更改
*******************************************************************************/
static void Sys_Clk(void)
{
	TenthSCounterUpadat();
	SCounterUpadat();
}
/*******************************************************************************
 * Function:    void Init_LowerDrive(void)
 *
 * Description: 底层驱动初始化
 *
 * Returns:     none
 *
 * Notes:      按照实际应用编写驱动函数
*******************************************************************************/
static void Moudle_InitErr(void)
{
	u8InitErrNum++;
}


void Init_LowerDrive(void)
{
	//TODO: 编写LIB的底层驱动
	const uint16 *TBL_SENSOR[ADID_TNUM] = {0};
	TBL_SENSOR[ADID_TIN-3] = TBL_TIN_SAIC_51;
	TBL_SENSOR[ADID_TAMB-3] = TBL_TAMB_SAIC_51;
	TBL_SENSOR[ADID_TEVP-3] = TBL_TEVP_SAIC_51;
	TBL_SENSOR[ADID_FVENT-3] = TBL_INTAKE_SAIC_51;
	TBL_SENSOR[ADID_DVENT-3] = TBL_INTAKE_SAIC_51;

	
	//eeprom
	FLASH_EE_Init();   //此项建议放在最前，
	
	(void)ADInit(ADID_MAXNUM, 1);  //第二个参数为分压电阻的类型
	if ( ADPhy_Init(ADID_VNUM, ADID_SYSIGN, ADID_TNUM, TBL_SENSOR) )
	{
		//设置正确
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
	
	MotorDriveInit(5, 1);  //设置默认的电机精度和提前量
	
	if (NcfMotor_Init(150,1))
	{
		
	}
	else
	{
		Moudle_InitErr();
	}
	
	//7708 初始化
	MCPComPinType TBL_Set7708[6] = {M7708_COMBINPIN2, M7708_COMBINPIN1, M7708_COMBINPIN3, M7708_COMBINPIN4, M7708_COMBINPIN5, M7708_COMBINPIN6};  //7708 电机配置
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
	
	InitComdQueue();   //放在开中断前面，软件模块初始化，不应该开中断

#if  _PROJECT_CAN_
   InterLayInit();
   InterLay2Init();
#endif
   
/* uint8 TBL_DrivePriode[5] = {16, 16, 16, 12, 0xff};  //8,7,6,5,4 的驱动步数
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
	
   //以下为CAN UDS载入的固定用法不用动
#if  _PROJECT_CAN_
	(void)Load_Parameter();    //载入参数状态
#endif
	(void)Load_ALLCycEE();     //载入其他参数-UDS参数
	
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


//以下为中断函数

/*************************************************************************
 * Function:  void LowDrive_Loop(void)
 *
 * Description: lowdrvie 的loop调用
 *
 * Returns:     none
 *
 * Notes:    基本不用更改，可按实际需要增减
 *
*******************************************************************************/
void LowDrive_Loop(void)
{
	Sys_Clk();           //时钟函数
	
	Treat_Read_Ad();     // ad处理函数。模块相关
	
	SigFControl();       //信号滤波模块
	
//	Treat_Read_Keypad();
	
	u32TestLoopFreq = TimeOut_TestLoop();
	u16TestLoopTime = TimeOut_GetMaxLoop();

	//发送can，电源管理内部处理
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
 * Description: lowdrvie 的不同电源时调用的主控函数
 *
 * Returns:     none
 *
 * Notes:     此处放置底层驱动函数需要在电源为ON /OFF时调用的主控函数
 *
*******************************************************************************/
void LowDrive_PowerON(void)
{
#if _PROJECT_LCD_
//	Updata_LCD();
#endif
}

