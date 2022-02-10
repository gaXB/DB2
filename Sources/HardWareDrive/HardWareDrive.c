/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：HardWareDrive.c
* 功能描述：lib 的驱动文件
* 包括 ad，timer ，电机（2），按键。相关函数。
* 编译器： codewarrior IDE 11.1
* MCU说明：为接口模板，接口函数不变，内容和MCU相关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年3月14日
* 
*/
#include "HardWareDrive.h"
#include "LibDriveDefine.h"
#include "MCU_Port.h"
#include "ADC.h"


uint8 GetPinV_CD4051(uint8 chValue)
{
	uint8 ch = chValue & 0x0f;
	uint8 ICNum= chValue>>4;
	uint8 IPinValue = 0;
	if (ICNum == 1)
	{
		GPIO_BIT_EQUAL( CD4051_A_1, ch&0x1);
		GPIO_BIT_EQUAL( CD4051_B_1, ch&0x2);
		GPIO_BIT_EQUAL( CD4051_C_1, ch&0x4);
		GaLib_MCUNOP(10);
		MCU_NOP;
		MCU_NOP;
		MCU_NOP;
		IPinValue = AD_CD4051_1;
	}
	else if (ICNum == 2)
	{
		GPIO_BIT_EQUAL( CD4051_A_2, ch&0x1);
		GPIO_BIT_EQUAL( CD4051_B_2, ch&0x2);
		GPIO_BIT_EQUAL( CD4051_C_2, ch&0x4);
		GaLib_MCUNOP(10);
		MCU_NOP;
		MCU_NOP;
		MCU_NOP;
		IPinValue = AD_CD4051_2;
	}

	return IPinValue;
}

int i16GPIO, i16GPION, i16ADCH;

/*******************************************************************************
 * Function:  void  RTC_IRQHandler(void)
 *
 * Description: RTC 中断
 *
 * Returns:     none
 *
 * Notes:     基本功能，不要更改
 *
*******************************************************************************/
void  RTC_IRQHandler1(void)
{
	static uint32 nRtcTime;
	//RTC_ClrFlags();
	//GPIO_Toggle(GPIOA, GPIO_PTC0_MASK);
    //GPIOA_PTC0 = 1;
#if _OPENSDA_CAN
	OGPIOA->Bits.PTC1 = ~OGPIOA->Bits.PTC1;
#endif
	//GPIOA_PTD0 = ~GPIOA_PTD0;
	nRtcTime++;
}
uint32 u32SysticNum;
/*******************************************************************************
 * Function: 	void SysTick_Handler(void)
 *
 * Description: 1ms 中断
 *
 * Returns:     none
 *
 * Notes:     统一用systick来实现
 *
*******************************************************************************/
void SysTick_Handler1(void)
{
	u32SysticNum++;
}

//interrupt
static int  ADCCont, SystickTimes, RTCTimes, adcResultInMv;
void LPIT0_Ch0_IRQHandler (void)
{
	static uint32 nTimes;
	LPIT0->MSR |= LPIT_MSR_TIF0_MASK; /* Clear LPIT0 timer flag 0 */

	MsCounterUpadat();
	nTimes++;
	if (nTimes >= 5)
	{
		nTimes = 0;
		//编码器调
	}
	else{}
}

void RTC_Seconds_IRQHandler (void)
{
    RTCTimes++;
}

/*******************************************************************************
 * Function:   void ADC0_IRQHandler(void)
 *
 * Description: 中断服务函数调用 
 *
 * Returns:     none
 *
 * Notes:    将AD值通过ADHWEnd_Interrupt传给AD模块 	
 *           此MCU 固定格式，不动
*******************************************************************************/
void ADC0_IRQHandler(void)
{
	uint16 u16ADResult;

	u16ADResult = ADC0->R[0];
	ADHWEnd_Interrupt(u16ADResult);
} 

void ADC1_IRQHandler(void)
{
	uint16 u16ADResult;

	u16ADResult = ADC1->R[0];
	ADHWEnd_Interrupt(u16ADResult);
}

/*******************************************************************************
 * Function:   void ADCH_SEL(uint8 adid)   
 *
 * Description: 选择通道，根据硬件及单片机选择对应的通道
 *
 * Returns:     none
 *
 * Notes:    此处.......处须程序根据硬件编写
			
 *
*******************************************************************************/ 
/***************根据单片机定义AD寄存器***********/
//uint8 TBL_ADCH[ADID_MAXNUM] = {0x84, 0x84, 0x8f, 0x8e, 0x89, 0x88, 0x81, 1, 0x86, 0x87,
//		0x82, 0x83, 0x8b, 0x8d, 0x00, 0x85, 0x8c, 0x8a};

uint8 TBL_ADCH[ADID_MAXNUM] = {0x01, 0x8c, 0x0, 0x0, 0x88, 0x8b, 0x80, 0x83,
		0x89, 0x8a, 0x81, 0x87, 0x82, 0x8f, 0x8e, 0x86};

void ADCH_SEL(uint8 adid)   
{//TODO:  ADCH 设置
	uint8 MCU_ADCH;
	uint8 u8ADID;
	if (adid < ADID_MAXNUM)
	{
		MCU_ADCH = TBL_ADCH[adid] &0x7f;
	}
	else
	{
		MCU_ADCH = 29;
	}
	if (TBL_ADCH[adid] & 0x80)
	{
		u8ADID  = 1;
	}
	else
	{
		u8ADID = 0;
	}

	convertAdcChan(u8ADID ,MCU_ADCH);
}
/*******************************************************************************
 * Function:     uint16 MotorHW_SupplyVolt(void)
 *
 * Description:  电机的供电电压
 *
 * Returns:    电机输出的电压
 *
 * Notes:   
			
 *
*******************************************************************************/
int16 MotorHW_SupplyVolt(void)
{//TODO: 电机供电电压
	return ADPhy_GetFact(ADID_VOLT);
}
/***********************************************
反馈式电机驱动物理接口
本项目中0电机冷暖
        1     模式
************************************************/
//#define     O_FRE      GPIOB_PTG0
//#define     O_REC      GPIOB_PTG1

void MotorHW_Drive(uint8 mid,uint8 motor_stat)
{
	//TODO: 电机驱动设置
	switch(mid)
	{
		default:
        break;
		case MOTOR_ID_MIX:
			(void)Mcp_Drive_Motor(MOTOR7708_ID_MIX,motor_stat);

			break;
		case MOTOR_ID_MODE:
			(void)Mcp_Drive_Motor(MOTOR7708_ID_MODE, motor_stat);
        break;
		case MOTOR_ID_NCF:
			(void)Mcp_Drive_Motor(MOTOR7708_ID_NCF,motor_stat);
		break;
  }
}



/*******************************************************************************
 * Function:    uint8 MotorHW_GetAD(uint8 MotorId)
 *
 * Description: 得到电机的AD值， mId电机号
 *
 * Returns:     8位AD 值
 *
 * Notes:   
 *
*******************************************************************************/
uint8 MotorHW_GetAD(uint8 MotorId)
{
	//TODO: 电机反馈设置
	uint8 u8BackAD;
	switch(MotorId)
	{
		default:
			u8BackAD = 0;
			break;
		case MOTOR_ID_MIX:
			u8BackAD = ADValue_Get(ADID_MOTORMIX, ADVALUE_GETTYPE_8ADFAST);
			break;
		case MOTOR_ID_MODE:
			u8BackAD = ADValue_Get(ADID_MOTORMODE, ADVALUE_GETTYPE_8ADFAST);
			break;
		case MOTOR_ID_NCF:
			u8BackAD = ADValue_Get(ADID_MOTORNCF, ADVALUE_GETTYPE_8ADFAST);
			break;
	}
	
	return u8BackAD;
}
/***********************************************
内外电机驱动物理接口
本项目中0电机  内外循环电机
************************************************/
void NcfMotorHW_Drive(uint8 mid,uint8 motor_stat)
{
	//TODO: 内外电机驱动编写
	if (mid == NCFMOTOR_ID_NCF)
	{
		(void)Mcp_Drive_Motor(MOTOR7708_ID_NCF,motor_stat);
	}
}

/*******************************************************************************
 * Function:    TimerCounterHalt(void),  TimerCounterResume(void)
 *
 * Description: 定时器中断停止和启动函数
 *
 * Returns:     none
 *
 * Notes:   程序员根据单片机，自行选择
 *
*******************************************************************************/
void  TimerCounterHalt(void)//frescal
{//16位以上MCU可不编写
   // TPM1C0SC_CH0IE=0;		
}

void  TimerCounterResume(void)
{
   // TPM1C0SC_CH0IE=1;
}
/***************************************************
uint8 Read_KeyBoard_Signal(void)
按键模块，物理层驱动接口
**************************************************/

uint16 KeyPadHW_ReadSignal(void)
{
	//TODO:  按键采集驱动
	uint16  keyword =0;
	
	O_KEY1 = 0;
	O_KEY2 = 1;
	O_KEY3 = 1;
	O_KEY4 = 1;
	MCU_NOP;
	MCU_NOP;
	MCU_NOP;
	MCU_NOP;
	if (GetPinV_CD4051(IKEY1_CH) == 0)
	{
		keyword |= KEY_AC;
	}
	if (GetPinV_CD4051(IKEY2_CH) == 0)
	{
		keyword |= KEY_ON;
	}

	O_KEY1 = 1;
	O_KEY2 = 0;
	O_KEY3 = 1;
	O_KEY4 = 1;
	MCU_NOP;
	MCU_NOP;
	MCU_NOP;
	MCU_NOP;
	if (GetPinV_CD4051(IKEY1_CH) == 0)
	{
		keyword |= KEY_OST;
	}
	if (GetPinV_CD4051(IKEY2_CH) == 0)
	{
		keyword |= KEY_NCF;
	}

	O_KEY1 = 1;
	O_KEY2 = 1;
	O_KEY3 = 0;
	O_KEY4 = 1;
	MCU_NOP;
	MCU_NOP;
	MCU_NOP;
	MCU_NOP;
	if (GetPinV_CD4051(IKEY1_CH) == 0)
	{
		keyword |= KEY_PTC;
	}
	if (GetPinV_CD4051(IKEY2_CH) == 0)
	{
		keyword |= KEY_MODE;
	}

	O_KEY1 = 1;
	O_KEY2 = 1;
	O_KEY3 = 1;
	O_KEY4 = 0;
	MCU_NOP;
	MCU_NOP;
	MCU_NOP;
	MCU_NOP;
	if (GetPinV_CD4051(IKEY1_CH) == 0)
	{
		keyword |= KEY_OFF;
	}
	if (GetPinV_CD4051(IKEY2_CH) == 0)
	{
		keyword |= KEY_RHEAT;
	}
	return keyword;
}
/*******************************************************************************
 * Function:   void  DealForSleep(void) void  DealForWakeUp(void)
 *
 * Description: 进入休眠时，mcu需要做的相关处理
 *
 * Returns:     none
 *
 * Notes:   程序员根据单片机和系统相关资源进行配置
 *          此MCU 固定格式
*******************************************************************************/
void  DealForSleep(void)
{
	//PMC_SetMode(PMC,PmcModeStop3);
	MCU_NOP;
   Clear_WatchDog();
}


void sSW_Reset(void) 
{ 
//   __DSB();
//   SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEYSTAT_Pos)|SCB_AIRCR_SYSRESETREQ_Msk);
//   __DSB();
	SystemSoftwareReset();
    while(1)
    {};
}

void  DealForWakeUp(void)
{
//   Clear_WatchDog();
   sSW_Reset();
}

void WDOG_Feed(void)
{
	WDOG_DRV_Trigger(INST_WATCHDOG1);
}
/*******************************************************************************
 * Function:   uint8 Read_Code28Data(uint8 num)
 *
 * Description: 返回编码器状态
 *
 * Returns:    CodeData 第 0位 a脚的状态， 第1一位为 b脚的状态    
 *
 * Notes:   
			
 *
*******************************************************************************/
//TODO: 28 35 编码器底层驱动设置
#define	   ICODE_FAN2     GPIO_UNUSE   //冷暖编码器1
#define	   ICODE_FAN1    	GPIO_UNUSE   //冷暖编码器2
#define     ICODE_TEMP1    GPIO_UNUSE   //风档编码器2
#define		ICODE_TEMP2    GPIO_UNUSE   //风档编码器1
uint8 Code28HW_ReadPin(uint8 num)
{
   uint8 CodeData = 0;
   if (num == CODE_NUM_TSET)
   {
      if (ICODE_TEMP1)
      {
         CodeData |= 0x01;   
      }
      if (ICODE_TEMP2)
      {
         CodeData |= 0x02;
      }
   }
   else if (num == CODE_NUM_FAN)
   {
      if (ICODE_FAN1)
      {
         CodeData |= 0x01;   
      }
      if (ICODE_FAN2)
      {
         CodeData |= 0x02;
      }
   }
   else{}
   
   return CodeData;
}

#define		ICD_FAN_A       GetPinV_CD4051(CD35_FAN_1_CH)   //风挡编码器
#define     ICD_FAN_C		 GetPinV_CD4051(CD35_FAN_2_CH)
#define     ICD_FAN_B       GetPinV_CD4051(CD35_FAN_3_CH)

#define		ICD_MIX_C		GetPinV_CD4051(CD35_TEMP_1_CH)   //温度设定编码器
#define		ICD_MIX_B		GetPinV_CD4051(CD35_TEMP_2_CH)
#define		ICD_MIX_A		GetPinV_CD4051(CD35_TEMP_3_CH)

//uint8 Code35HW_ReadPin(uint8 num)
uint8 Code35HW_ReadPin(uint8 num)
{
	uint8 CodeData = 0;
	if (num == CODE_NUM_FAN)
	{
		if (ICD_FAN_A)
		{
			CodeData |= 0x01;
		}
		
		if (ICD_FAN_B)
		{
			CodeData |= 0x02;
		}
		
		if (ICD_FAN_C)
		{
			CodeData |= 0x04;
		}
	}
	else if (num == CODE_NUM_TSET)
	{
		if (ICD_MIX_A)
		{
			CodeData |= 0x01;
		}
		
		if (ICD_MIX_B)
		{
			CodeData |= 0x02;
		}
		
		if (ICD_MIX_C)
		{
			CodeData |= 0x04;
		}
	}
	else
	{
		CodeData = 0;
	}
	
	return CodeData;
}

uint8 CodeKeyHW_ReadPin(uint8 num)
{
   uint8 CodeData = 0;
//   if (num == CODE_NUM_FAN)
//   {
//      if (ICDKEY_TEMP_A)
//      {
//         CodeData |= 0x01;   
//      }
//      if (ICDKEY_TEMP_B)
//      {
//         CodeData |= 0x02;
//      }
//   }
//   else if (num == CODE_NUM_TSET)
//   {
//      if (ICDKEY_FAN_A)
//      {
//         CodeData |= 0x01;   
//      }
//      if (ICDKEY_FAN_B)
//      {
//         CodeData |= 0x02;
//      }
//   }
//   else{}
   
   return CodeData;
}
/*******************************************************************************
 * Function:    void SPIHW_ClkWrite(uint8 bSetState);
 *              void SPIHW_DataWrite(uint8 bSetState)
 *              uint8 SPIHW_DataRead(void)
 * Description:  spi MCU 接口，pin 设置
 *
 * Returns:    none
 *
 * Notes:   SPI pin 读写	
 *           固定格式，如无此端口可屏蔽
*******************************************************************************/
void SPIHW_ClkWrite(uint8 bSetState)
{
	//GaLib_MCUNOP(1);
	if (bSetState)
	{
		SPI_CLK = GPIO_SET;
	}
	else
	{
		SPI_CLK = GPIO_CLEAR;
	}
	//GaLib_MCUNOP(1);
}

void SPIHW_DataWrite(uint8 bSetState)
{
	//GaLib_MCUNOP(1);
	if (bSetState)
	{
		SPI_OUT = GPIO_SET;
	}
	else
	{
		SPI_OUT = GPIO_CLEAR;
	}
	//GaLib_MCUNOP(1);
}


uint8 SPIHW_DataRead(void)
{
	//GaLib_MCUNOP(2);
	return 1;
}


/*******************************************************************************
 * Function:    void McpHW_CSSet(uint8 u8ICId, uint8 BitSet)
 *              
 * Description:  7708 CS MCU 接口，pin 设置
 *                 
 * parameter：       u8ICId: u8ICId 芯片ID,0~1,  BitSet: 设置高：1 低：0
 * 
 * Returns:      none
 *
 * Notes:   SPI pin 读写	
 *
*******************************************************************************/

void McpHW_CSSet(uint8 u8ICId, PinOutState BitSet)
{
	//TODO: MCP7708 cs设置
	if (u8ICId == 0)
	{
		if (BitSet)
		{
			O_MCP_CS = GPIO_SET;
		}
		else
		{
			O_MCP_CS = GPIO_CLEAR;
		}
	}
	else
	{	}
}

/*******************************************************************************
 * Function:    void IICHW_SDAWrite(uint8 bSetState)
 *              BOOL IICHW_SDARead(void)
 *              void IICHW_SCLWrite(uint8 bSetState)
 * Description:  IIC 驱动
 *
 * Returns:    none
 *
 * Notes:     	请按以下模板编写
 *
*******************************************************************************/
#define   SDA  IGPIO_PTB10
#define   SCL  IGPIO_PTB9

#define   SDA_OUT  GPIO_PTB10
#define   SCL_OUT  GPIO_PTB9

#define   SDA_OUT_LOW      PTB->PDDR |= 0x400u;  SDA_OUT=0;//设置数据脚输出低电平
#define   SDA_OUT_HIGH     PTB->PDDR &= 0xFFFFFBFFu;   //设置数据脚为输入，外接上拉电阻

#define   SCL_OUT_LOW      PTB->PDDR |= 0x200u;    SCL_OUT=0; //设置时钟脚输出低电平
#define   SCL_OUT_HIGH     PTB->PDDR &= 0xFFFFFDFFu;   //设置时钟脚为输入，外接上拉电阻



void IICHW_SDAWrite(uint8 bSetState)
{
	if (bSetState)
	{
		SDA_OUT_HIGH
	}
	else
	{
		SDA_OUT_LOW
	}
	GaLib_MCUNOP(30);
}

BOOL IICHW_SDARead(void)
{
	GaLib_MCUNOP(3);
	return SDA;
}
void IICHW_SCLWrite(uint8 bSetState)
{
	uint8 bOK= 0;
	while(bOK < 30)
	{
		if (bSetState)
		{
			SCL_OUT_HIGH
		}
		else
		{
			SCL_OUT_LOW
		}
		if (SCL == bSetState)
		{
			bOK = 30;
		}
		else
		{
			bOK++;
		}
	}
	//GaLib_MCUNOP(30);
}

/*******************************************************************************
 * Function:    ADVoltType FanHW_BackVolt(void);
 *             
 * Description:  返回鼓风机的反馈电压
 *
 * Returns:    ADVoltType
 *
 * Notes:    none
 *
*******************************************************************************/
ADVoltType FanHW_BackVolt(void)
{//TODO: 编写鼓风机反馈电压
	return ADPhy_GetFact(ADID_FAN);
}
ADVoltType FanHW_SupperVolt(void)
{//TODO: 编写鼓风机供电电压
	return ADPhy_GetFact(ADID_VOLT);
}

//TODO: 鼓风机驱动固定格式
void  FanHW_OutPWM(uint16 pwm)
{
	OUTFAN_PWM =  (uint32)pwm * PWMMODE_FAN / 10000;
}

void FanHW_OutFanRealy(uint8 bSetState)
{
	if (bSetState) 
	{	
		//O_FAN_RELAY = GPIO_SET;
	}
	else 
	{
		//O_FAN_RELAY = GPIO_CLEAR;
	}
}


/*******************************************************************************
 * Function:     void DRV8860HW_LanthWrite(uint8 bSetState)
 *             
 * Description:  8860 驱动
 *
 * Returns:    
 *
 * Notes:     8860 驱动模板
 *
*******************************************************************************/
 void DRV8860HW_LanthWrite(uint8 bSetState)
 {
	 //DRV8860_LANTH = bSetState;
 }


 void DRV8860HW_ENWrite(uint8 bSetState)
 {
	 //DRV8860_EN = bSetState;
 }

 uint8 DRV8860HW_FaultRead(uint8 u8ICNum)
 {
	 //return DRV8860_I_FAULT;
	 return 1;
 }


