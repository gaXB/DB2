/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�HardWareDrive.c
* ����������lib �������ļ�
* ���� ad��timer �������2������������غ�����
* �������� codewarrior IDE 11.1
* MCU˵����Ϊ�ӿ�ģ�壬�ӿں������䣬���ݺ�MCU���
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��3��14��
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
 * Description: RTC �ж�
 *
 * Returns:     none
 *
 * Notes:     �������ܣ���Ҫ����
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
 * Description: 1ms �ж�
 *
 * Returns:     none
 *
 * Notes:     ͳһ��systick��ʵ��
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
		//��������
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
 * Description: �жϷ��������� 
 *
 * Returns:     none
 *
 * Notes:    ��ADֵͨ��ADHWEnd_Interrupt����ADģ�� 	
 *           ��MCU �̶���ʽ������
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
 * Description: ѡ��ͨ��������Ӳ������Ƭ��ѡ���Ӧ��ͨ��
 *
 * Returns:     none
 *
 * Notes:    �˴�.......����������Ӳ����д
			
 *
*******************************************************************************/ 
/***************���ݵ�Ƭ������AD�Ĵ���***********/
//uint8 TBL_ADCH[ADID_MAXNUM] = {0x84, 0x84, 0x8f, 0x8e, 0x89, 0x88, 0x81, 1, 0x86, 0x87,
//		0x82, 0x83, 0x8b, 0x8d, 0x00, 0x85, 0x8c, 0x8a};

uint8 TBL_ADCH[ADID_MAXNUM] = {0x01, 0x8c, 0x0, 0x0, 0x88, 0x8b, 0x80, 0x83,
		0x89, 0x8a, 0x81, 0x87, 0x82, 0x8f, 0x8e, 0x86};

void ADCH_SEL(uint8 adid)   
{//TODO:  ADCH ����
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
 * Description:  ����Ĺ����ѹ
 *
 * Returns:    �������ĵ�ѹ
 *
 * Notes:   
			
 *
*******************************************************************************/
int16 MotorHW_SupplyVolt(void)
{//TODO: ��������ѹ
	return ADPhy_GetFact(ADID_VOLT);
}
/***********************************************
����ʽ�����������ӿ�
����Ŀ��0�����ů
        1     ģʽ
************************************************/
//#define     O_FRE      GPIOB_PTG0
//#define     O_REC      GPIOB_PTG1

void MotorHW_Drive(uint8 mid,uint8 motor_stat)
{
	//TODO: �����������
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
 * Description: �õ������ADֵ�� mId�����
 *
 * Returns:     8λAD ֵ
 *
 * Notes:   
 *
*******************************************************************************/
uint8 MotorHW_GetAD(uint8 MotorId)
{
	//TODO: �����������
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
��������������ӿ�
����Ŀ��0���  ����ѭ�����
************************************************/
void NcfMotorHW_Drive(uint8 mid,uint8 motor_stat)
{
	//TODO: ������������д
	if (mid == NCFMOTOR_ID_NCF)
	{
		(void)Mcp_Drive_Motor(MOTOR7708_ID_NCF,motor_stat);
	}
}

/*******************************************************************************
 * Function:    TimerCounterHalt(void),  TimerCounterResume(void)
 *
 * Description: ��ʱ���ж�ֹͣ����������
 *
 * Returns:     none
 *
 * Notes:   ����Ա���ݵ�Ƭ��������ѡ��
 *
*******************************************************************************/
void  TimerCounterHalt(void)//frescal
{//16λ����MCU�ɲ���д
   // TPM1C0SC_CH0IE=0;		
}

void  TimerCounterResume(void)
{
   // TPM1C0SC_CH0IE=1;
}
/***************************************************
uint8 Read_KeyBoard_Signal(void)
����ģ�飬����������ӿ�
**************************************************/

uint16 KeyPadHW_ReadSignal(void)
{
	//TODO:  �����ɼ�����
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
 * Description: ��������ʱ��mcu��Ҫ������ش���
 *
 * Returns:     none
 *
 * Notes:   ����Ա���ݵ�Ƭ����ϵͳ�����Դ��������
 *          ��MCU �̶���ʽ
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
 * Description: ���ر�����״̬
 *
 * Returns:    CodeData �� 0λ a�ŵ�״̬�� ��1һλΪ b�ŵ�״̬    
 *
 * Notes:   
			
 *
*******************************************************************************/
//TODO: 28 35 �������ײ���������
#define	   ICODE_FAN2     GPIO_UNUSE   //��ů������1
#define	   ICODE_FAN1    	GPIO_UNUSE   //��ů������2
#define     ICODE_TEMP1    GPIO_UNUSE   //�絵������2
#define		ICODE_TEMP2    GPIO_UNUSE   //�絵������1
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

#define		ICD_FAN_A       GetPinV_CD4051(CD35_FAN_1_CH)   //�絲������
#define     ICD_FAN_C		 GetPinV_CD4051(CD35_FAN_2_CH)
#define     ICD_FAN_B       GetPinV_CD4051(CD35_FAN_3_CH)

#define		ICD_MIX_C		GetPinV_CD4051(CD35_TEMP_1_CH)   //�¶��趨������
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
 * Description:  spi MCU �ӿڣ�pin ����
 *
 * Returns:    none
 *
 * Notes:   SPI pin ��д	
 *           �̶���ʽ�����޴˶˿ڿ�����
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
 * Description:  7708 CS MCU �ӿڣ�pin ����
 *                 
 * parameter��       u8ICId: u8ICId оƬID,0~1,  BitSet: ���øߣ�1 �ͣ�0
 * 
 * Returns:      none
 *
 * Notes:   SPI pin ��д	
 *
*******************************************************************************/

void McpHW_CSSet(uint8 u8ICId, PinOutState BitSet)
{
	//TODO: MCP7708 cs����
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
 * Description:  IIC ����
 *
 * Returns:    none
 *
 * Notes:     	�밴����ģ���д
 *
*******************************************************************************/
#define   SDA  IGPIO_PTB10
#define   SCL  IGPIO_PTB9

#define   SDA_OUT  GPIO_PTB10
#define   SCL_OUT  GPIO_PTB9

#define   SDA_OUT_LOW      PTB->PDDR |= 0x400u;  SDA_OUT=0;//�������ݽ�����͵�ƽ
#define   SDA_OUT_HIGH     PTB->PDDR &= 0xFFFFFBFFu;   //�������ݽ�Ϊ���룬�����������

#define   SCL_OUT_LOW      PTB->PDDR |= 0x200u;    SCL_OUT=0; //����ʱ�ӽ�����͵�ƽ
#define   SCL_OUT_HIGH     PTB->PDDR &= 0xFFFFFDFFu;   //����ʱ�ӽ�Ϊ���룬�����������



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
 * Description:  ���عķ���ķ�����ѹ
 *
 * Returns:    ADVoltType
 *
 * Notes:    none
 *
*******************************************************************************/
ADVoltType FanHW_BackVolt(void)
{//TODO: ��д�ķ��������ѹ
	return ADPhy_GetFact(ADID_FAN);
}
ADVoltType FanHW_SupperVolt(void)
{//TODO: ��д�ķ�������ѹ
	return ADPhy_GetFact(ADID_VOLT);
}

//TODO: �ķ�������̶���ʽ
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
 * Description:  8860 ����
 *
 * Returns:    
 *
 * Notes:     8860 ����ģ��
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


