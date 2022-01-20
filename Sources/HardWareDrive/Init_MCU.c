/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Init_MCU.c
* 功能描述：MCU 硬件初始化
* 1  时钟
* 2  rtc
* 3  AD， PWM， wdog, PMC, timer
* 编译器： codewarrior IDE 10.7
* MCU说明：此文件跟MCU 相关，适用于KEAZ128
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年3月14日
* 
* * 版本：1.1
* 说明：MCUDATA 的功能由MCUPMLogic 决定
* 原作者：xb
* 完成日期：2020年5月13日
*  
* 版本：1.2
* 说明：
* 1 时钟校准功能优化，PE和JLINK切换时不需要更改代码
* 2 外部时钟增加时钟稳定的判断
* 作者：xb
* 完成日期：2020年7月6日
*/
#include "cpu.h"
#include "DataType.h"
#include "MCUPMLogic.h"
#include "Cpu.h"
#include "GALib.h"
#include "Config.h"
#include "MCU_PORT.h"

//外部函数调用声明
extern void TrOut_5VEN(uint8 en);
extern void MCUPM_SetReset(uint32 u32ResonRegister);
/*******************************************************************************
* Function:      void Init_MCG_INT(void)
*
* Description:    配置总线时钟为10M，
                 
* return :       none
* Note  :        使用内部时钟，不能更改， KEZ调整到31.25khz， 
*                
************************************************************************************/
static void Init_MCG_INT(void)
{

}


void LPIT0_init (void)
{
	/*!
	 * LPIT Clocking:
	 * ==============================
	 */
  PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
  PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs 		*/

  /*!
   * LPIT Initialization:
   */
  LPIT0->MCR |= LPIT_MCR_M_CEN_MASK;  /* DBG_EN-0: Timer chans stop in Debug mode */
                              	  	  /* DOZE_EN=0: Timer chans are stopped in DOZE mode */
                              	  	  /* SW_RST=0: SW reset does not reset timer chans, regs */
                              	  	  /* M_CEN=1: enable module clk (allows writing other LPIT0 regs) */
  LPIT0->MIER = LPIT_MIER_TIE0_MASK;  /* TIE0=1: Timer Interrupt Enabled fot Chan 0 */
  LPIT0->TMR[0].TVAL = 40000;      /* Chan 0 Timeout period: 40M clocks */

  LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
  	  	  	  	  	  	  	  /* T_EN=1: Timer channel is enabled */
                              /* CHAIN=0: channel chaining is disabled */
                              /* MODE=0: 32 periodic counter mode */
                              /* TSOT=0: Timer decrements immediately based on restart */
                              /* TSOI=0: Timer does not stop after timeout */
                              /* TROT=0 Timer will not reload on trigger */
                              /* TRG_SRC=0: External trigger soruce */
                              /* TRG_SEL=0: Timer chan 0 trigger source is selected*/

}

#define RTC_INITIAL_SEC_VALUE	(1) /* Starting value for RTC, this should be set to non-zero value */
#define RTC_ALARM_PERIOD       	(5) /* Alarm Period Time */
void RTC_init(void)
{
	/*!
	 * RTC Clocking:
	 * ==========================
	 */
    uint32_t sim_lpoclks_mask = 0;

    /* Select 32kHz LPO clock for RTC_CLK */
    sim_lpoclks_mask = SIM->LPOCLKS|
            		   SIM_LPOCLKS_RTCCLKSEL(0b01) |
					   SIM_LPOCLKS_LPO32KCLKEN_MASK;

    SIM->LPOCLKS = sim_lpoclks_mask;	    /* Write once register */
    PCC->PCCn[PCC_RTC_INDEX] |= PCC_PCCn_CGC_MASK;      /* Enable clock for RTC */

    /*!
     * RTC Initialization:
     * ===========================
     */
    RTC->SR &= ~RTC_SR_TCE_MASK;		/* Disable RTC module */
    RTC->TPR &= 0;	    				/* Time prescaler register is reset to zero */
    RTC->TSR = RTC_INITIAL_SEC_VALUE;	/* Start count from any value different than 0 */

    RTC->TAR = RTC_INITIAL_SEC_VALUE + RTC_ALARM_PERIOD;	/* Set alarm time */

    /*!
     *  Configure RTC
     *  =====================
     */
    RTC->TCR &= 0;  			/* Set compensation to 0 */
    RTC->CR =
            RTC_CR_LPOS(0) |    /* Use RTC_CLK instead of LPO */
            RTC_CR_CPS(1);      /* Select TSIC output for RTC_CLKOUT */

    RTC->IER = RTC_IER_TSIC(5) |	/* Selects 1Hz output */
    		RTC_IER_TSIE(1);      /* Enable alarm interrupt */

    /* Run RTC */
    RTC->SR |= RTC_SR_TCE_MASK;
}
/*******************************************************************************
* Function:      void Init_MCG(void)
*
* Description:    外部4m配置总线时钟为10M，
                 
* return :       none
* Note  :       KEZ的配置方式，KEZN需要更改

************************************************************************************/
static void Init_MCG_EXT(void)
{
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                        g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    /* Initialize pins */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    LPIT0_init();
   // RTC_init();

   //
   // INT_SYS_EnableIRQ(RTC_Seconds_IRQn);	/* Enable RTC interrupt */
}
/*******************************************************************************
* Function:      void Init_RTC(void)
*
* Description:    用rtc生产0.1s 的中断，用来唤醒
* return :        none
* Note  :         时钟为内部1khz的时钟
************************************************************************************/ 
static void Init_RTC(void)
{

}

/*******************************************************************************
* Function:      void Init_SIM(void)
*
* Description:    SIM 配置，设置开启时钟
                 
* return :       none
* Note  :       

************************************************************************************/
static void Init_SIM(void)
{

}

/*******************************************************************************
* Function:      void Init_WDOG(void)
*
* Description:   使能看门狗 ，时钟为内部1khz的时钟
* 
* return :       none
* 
* Note  :       固定用法
************************************************************************************/ 
static void Init_WDOG(void)
{
	/* Initialize WDOG */
	//WDOG_DRV_Init(INST_WATCHDOG1, &watchdog1_Config0);
}

/*******************************************************************************
* Function:     void Init_PMC()
*
* Description:    使能电源管理模块
* 
* return :       none
* 
* Note  :        不用更改
************************************************************************************/ 
static void Init_PMC(void)
{

}

/*******************************************************************************
 * Function:    Init_GPIO_1
 *
 * Description: mcu GPIO settings
 *
 * Returns:     none
 *
 * Notes:     	上电端口初始化  ,此时mcu进入工作状态
 *
*******************************************************************************/
void  Init_GPIO_1(void)
{
//	  PTD->PDDR |= 1<<0;            		/* Port D0:  Data Direction= output */
//	  for (i = 0; i<18; i++)
//	  {
//		  PORTA->PCR[i] |=  PORT_PCR_MUX(1);
//		  PORTB->PCR[i] |=  PORT_PCR_MUX(1);
//		  PORTC->PCR[i] |=  PORT_PCR_MUX(1);
//		  PORTD->PCR[i] |=  PORT_PCR_MUX(1);
//		  PORTE->PCR[i] |=  PORT_PCR_MUX(1);
//	  }
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
//	TLE94112_CS = 1;
//	TLE9262_CS = 1;

    O_CAN_EN_1 = 0;
    O_CAN_EN_2 = 0;
    O_CAN_EN_3 = 0;
    LIN_EN = 1;
    TrOut_5VEN(1);
}

/*******************************************************************************
 * Function:    Init_GPIO_0
 *
 * Description: DZ60 GPIO settings
 *
 * Returns:     none
 *
 * Notes:       进入休眠之前的操作	
 *
*******************************************************************************/
void  Init_GPIO_0(void)
{

}

/*******************************************************************************
 * Function:    Init_Ad_1
 *
 * Description: AD模块初始化
 *
 * Returns:     none
 *
 * Notes:       上电AD模块初始化
 *
*******************************************************************************/
static void  Init_Ad_1(void)
{
	/*!
	* ADC0 Clocking:
	* ===================================================
	*/
	PCC->PCCn[PCC_ADC0_INDEX] &=~ PCC_PCCn_CGC_MASK;  /* Disable clock to change PCS */
	PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_PCS(1);     /* PCS=1: Select SOSCDIV2 		*/
	PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_CGC_MASK;   /* Enable bus clock in ADC 	*/

	/*!
	* ADC0 Initialization:
	* ===================================================
	*/
	ADC0->SC1[0] |= ADC_SC1_ADCH_MASK
			| ADC_SC1_AIEN(1);	/* ADCH=1F: Module is disabled for conversions	*/
	/* AIEN=0: Interrupts are disabled 			*/
	ADC0->CFG1 |= ADC_CFG1_ADIV_MASK
			|ADC_CFG1_MODE(2);	/* ADICLK=0: Input clk=ALTCLK1=SOSCDIV2 	*/
	/* ADIV=0: Prescaler=1 					*/
	/* MODE=1: 12-bit conversion 				*/

	ADC0->CFG2 = ADC_CFG2_SMPLTS(12);	/* SMPLTS=12(default): sample time is 13 ADC clks 	*/
	ADC0->SC2 = 0x00000000;         	/* ADTRG=0: SW trigger 							*/
	/* ACFE,ACFGT,ACREN=0: Compare func disabled		*/
	/* DMAEN=0: DMA disabled 							*/
	/* REFSEL=0: Voltage reference pins= VREFH, VREEFL */
	ADC0->SC3 = 0x00000000;       	/* CAL=0: Do not start calibration sequence 		*/
	/* ADCO=0: One conversion performed 				*/
	/* AVGE,AVGS=0: HW average function disabled 		*/
	INT_SYS_EnableIRQ(ADC0_IRQn);


	PCC->PCCn[PCC_ADC1_INDEX] &=~ PCC_PCCn_CGC_MASK;  /* Disable clock to change PCS */
	PCC->PCCn[PCC_ADC1_INDEX] |= PCC_PCCn_PCS(1);     /* PCS=1: Select SOSCDIV2 		*/
	PCC->PCCn[PCC_ADC1_INDEX] |= PCC_PCCn_CGC_MASK;   /* Enable bus clock in ADC 	*/

	/*!
	* ADC0 Initialization:
	* ===================================================
	*/
	ADC1->SC1[0] |= ADC_SC1_ADCH_MASK
			| ADC_SC1_AIEN(1);	/* ADCH=1F: Module is disabled for conversions	*/
	/* AIEN=0: Interrupts are disabled 			*/
	ADC1->CFG1 |= ADC_CFG1_ADIV_MASK
			|ADC_CFG1_MODE(2);	/* ADICLK=0: Input clk=ALTCLK1=SOSCDIV2 	*/
	/* ADIV=0: Prescaler=1 					*/
	/* MODE=1: 12-bit conversion 				*/

	ADC1->CFG2 = ADC_CFG2_SMPLTS(12);	/* SMPLTS=12(default): sample time is 13 ADC clks 	*/
	ADC1->SC2 = 0x00000000;         	/* ADTRG=0: SW trigger 							*/
	/* ACFE,ACFGT,ACREN=0: Compare func disabled		*/
	/* DMAEN=0: DMA disabled 							*/
	/* REFSEL=0: Voltage reference pins= VREFH, VREEFL */
	ADC1->SC3 = 0x00000000;       	/* CAL=0: Do not start calibration sequence 		*/
	/* ADCO=0: One conversion performed 				*/
	/* AVGE,AVGS=0: HW average function disabled 		*/
	INT_SYS_EnableIRQ(ADC1_IRQn);
}

/*******************************************************************************
 * Function:    Init_TMR_1
 *
 * Description: 1ms定时初始
 *
 * Returns:     none
 *
 * Notes:       固定使用systick 不用改
 *
*******************************************************************************/
static void  Init_TMR_1(void)
{
	S32_SysTick->RVR = 16000000;//0xFFFFFFul;
	S32_SysTick->CVR = 0ul;
	S32_SysTick->CSR = 0u;

	S32_SysTick->CSR = S32_SysTick_CSR_TICKINT(1u) | S32_SysTick_CSR_ENABLE(1);
}

/*******************************************************************************
 * Function:    Init_PWM_1
 *
 * Description: DZ32 PWM settings
 *
 * Returns:     none
 *
 * Notes:       上电PWM初始化(f=4KHZ,占空比0~255)
 *
*******************************************************************************/
static void Init_PWM_1(void)
{
	/**
	 * FTM0 Clocking:
	 * ==================================================
	 */
	PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config 	*/
	PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_PCS(0b010)	/* Clock Src=1, 8 MHz SIRCDIV1_CLK */
                              |  PCC_PCCn_CGC_MASK;  	/* Enable clock for FTM regs 		*/

	/*!
	 * FTM0 Initialization:
	 * ===================================================
	 */
	FTM0->MODE |= FTM_MODE_WPDIS_MASK;  /* Write protect to registers disabled (default) 				*/
	FTM0->SC	=	FTM_SC_PWMEN6_MASK	/* Enable PWM channel 0 output									*/
					 /* Enable PWM channel 1 output									*/
					|FTM_SC_PS(4);     	/* TOIE (Timer Overflow Interrupt Ena) = 0 (default) 			*/
										/* CPWMS (Center aligned PWM Select) = 0 (default, up count) 	*/
										/* CLKS (Clock source) = 0 (default, no clock; FTM disabled) 	*/
										/* PS (Prescaler factor) = 7. Prescaler = 128 					*/

	FTM0->COMBINE = 0x00000000;	/* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0 */
	FTM0->POL = 0x00000000;    	/* Polarity for all channels is active high (default) 		*/
	FTM0->MOD = 5000 -1 ;     	/* FTM1 counter final value (used for PWM mode) 			*/
								/* FTM1 Period = MOD-CNTIN+0x0001 ~= 62500 ctr clks  		*/
								/* 8MHz /128 = 62.5kHz ->  ticks -> 1Hz 					*/

	/**
	 * FTM0, Channel 1 in PWM Mode:
	 * ==================================================
	 */
	FTM0->CONTROLS[6].CnSC = FTM_CnSC_MSB_MASK
							|FTM_CnSC_ELSB_MASK;  	/* FTM0 ch1: edge-aligned PWM, low true pulses 		*/
													/* CHIE (Chan Interrupt Ena) = 0 (default) 			*/
													/* MSB:MSA (chan Mode Select)=0b10, Edge Align PWM		*/
													/* ELSB:ELSA (chan Edge/Level Select)=0b10, low true 	*/

	FTM0->CONTROLS[6].CnV =  0;	/* FTM0 ch1 compare value (~75% duty cycle) */

	FTM0->SC |= FTM_SC_CLKS(3);
	/* Start FTM0 counter with clk source = external clock (SOSCDIV1_CLK)*/


	/**
	 * FTM1 Clocking:
	 * ==================================================
	 */
	PCC->PCCn[PCC_FTM1_INDEX] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config 	*/
	PCC->PCCn[PCC_FTM1_INDEX] |= PCC_PCCn_PCS(0b010)	/* Clock Src=1, 8 MHz SIRCDIV1_CLK */
                              |  PCC_PCCn_CGC_MASK;  	/* Enable clock for FTM regs 		*/

	/*!
	 * FTM1 Initialization:
	 * ===================================================
	 */
	FTM1->MODE |= FTM_MODE_WPDIS_MASK;  /* Write protect to registers disabled (default) 				*/
	FTM1->SC	=	FTM_SC_PWMEN6_MASK | FTM_SC_PWMEN5_MASK	/* Enable PWM channel 0 output									*/
					 /* Enable PWM channel 1 output									*/
					|FTM_SC_PS(4);     	/* TOIE (Timer Overflow Interrupt Ena) = 0 (default) 			*/
										/* CPWMS (Center aligned PWM Select) = 0 (default, up count) 	*/
										/* CLKS (Clock source) = 0 (default, no clock; FTM disabled) 	*/
										/* PS (Prescaler factor) = 7. Prescaler = 128 					*/

	FTM1->COMBINE = 0x00000000;	/* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0 */
	FTM1->POL = 0x00000000;    	/* Polarity for all channels is active high (default) 		*/
	FTM1->MOD = 5000 -1 ;     	/* FTM1 counter final value (used for PWM mode) 			*/
								/* FTM1 Period = MOD-CNTIN+0x0001 ~= 62500 ctr clks  		*/
								/* 8MHz /128 = 62.5kHz ->  ticks -> 1Hz 					*/

	/**
	 * FTM1, Channel 1 in PWM Mode:
	 * ==================================================
	 */
	FTM1->CONTROLS[6].CnSC = FTM_CnSC_MSB_MASK
							|FTM_CnSC_ELSB_MASK;  	/* FTM0 ch1: edge-aligned PWM, low true pulses 		*/
													/* CHIE (Chan Interrupt Ena) = 0 (default) 			*/
													/* MSB:MSA (chan Mode Select)=0b10, Edge Align PWM		*/
													/* ELSB:ELSA (chan Edge/Level Select)=0b10, low true 	*/

	FTM1->CONTROLS[6].CnV =  0;	/* FTM0 ch1 compare value (~75% duty cycle) */

	FTM1->CONTROLS[5].CnSC = FTM_CnSC_MSB_MASK
							|FTM_CnSC_ELSB_MASK;  	/* FTM0 ch1: edge-aligned PWM, low true pulses 		*/
													/* CHIE (Chan Interrupt Ena) = 0 (default) 			*/
													/* MSB:MSA (chan Mode Select)=0b10, Edge Align PWM		*/
													/* ELSB:ELSA (chan Edge/Level Select)=0b10, low true 	*/

	FTM1->CONTROLS[5].CnV =  0;	/* FTM0 ch1 compare value (~75% duty cycle) */


	FTM1->SC |= FTM_SC_CLKS(3);
	/* Start FTM1 counter with clk source = external clock (SOSCDIV1_CLK)*/


	/**
	 * FTM2 Clocking:
	 * ==================================================
	 */
	PCC->PCCn[PCC_FTM2_INDEX] &= ~PCC_PCCn_CGC_MASK; 	/* Ensure clk disabled for config 	*/
	PCC->PCCn[PCC_FTM2_INDEX] |= PCC_PCCn_PCS(0b010)	/* Clock Src=1, 8 MHz SIRCDIV1_CLK */
										|  PCC_PCCn_CGC_MASK;  	/* Enable clock for FTM regs 		*/

	/*!
	 * FTM2 Initialization:
	 * ===================================================
	 */
	FTM2->MODE |= FTM_MODE_WPDIS_MASK;  /* Write protect to registers disabled (default) 				*/
	FTM2->SC	=	FTM_SC_PWMEN2_MASK	/* Enable PWM channel 0 output									*/
					 /* Enable PWM channel 1 output									*/
					|FTM_SC_PS(0);     	/* TOIE (Timer Overflow Interrupt Ena) = 0 (default) 			*/
										/* CPWMS (Center aligned PWM Select) = 0 (default, up count) 	*/
										/* CLKS (Clock source) = 0 (default, no clock; FTM disabled) 	*/
										/* PS (Prescaler factor) = 7. Prescaler = 128 					*/

	FTM2->COMBINE = 0x00000000;	/* FTM mode settings used: DECAPENx, MCOMBINEx, COMBINEx=0 */
	FTM2->POL = 0x00000000;    	/* Polarity for all channels is active high (default) 		*/
	FTM2->MOD = 4000 -1 ;     	/* FTM1 counter final value (used for PWM mode) 			*/
								/* FTM1 Period = MOD-CNTIN+0x0001 ~= 62500 ctr clks  		*/
								/* 8MHz /128 = 62.5kHz ->  ticks -> 1Hz 					*/

	/**
	 * FTM2, Channel 1 in PWM Mode:
	 * ==================================================
	 */
	FTM2->CONTROLS[2].CnSC = FTM_CnSC_MSB_MASK
							|FTM_CnSC_ELSB_MASK;  	/* FTM0 ch1: edge-aligned PWM, low true pulses 		*/
													/* CHIE (Chan Interrupt Ena) = 0 (default) 			*/
													/* MSB:MSA (chan Mode Select)=0b10, Edge Align PWM		*/
													/* ELSB:ELSA (chan Edge/Level Select)=0b10, low true 	*/

	FTM2->CONTROLS[2].CnV =  2000;	/* FTM0 ch1 compare value (~75% duty cycle) */


	FTM2->SC |= FTM_SC_CLKS(3);
	/* Start FTM0 counter with clk source = external clock (SOSCDIV1_CLK)*/
}

/*******************************************************************************
 * Function:    void Init_MCU(void)
 *
 * Description: 对mcu 内部进行配置
 *
 * Returns:     none
 *
 * Notes:     	 1 看门狗  2 时钟  3 SIM  4 RTC  5 PMC
 *
*******************************************************************************/

void LPTMR_ISR(void)
{
	/* Static variable, used to count if the timeout has passed to
	 * provide the LIN scheme tick.
	 */
	static uint32_t interruptCount = 0UL;
    /* Timer Interrupt Handler */
    lin_lld_timeout_service(LI0);

    /* If 5 ms have passed, provide the required tick */
    if(++interruptCount == 10UL)
    {
    	l_sch_tick(LI0);
    	interruptCount = 0UL;
    }
	/* Clear compare flag */
	LPTMR_DRV_ClearCompareFlag(INST_LPTMR1);
}


void Init_MCU(void)
{	
	MCUPM_SetReset(RCM->SRS);//(SIM->SRSID);
   //此项建议放在最前，
	Init_WDOG();
	Init_MCG_EXT();
//	Init_SIM();
//	Init_PMC();
//	Init_RTC();
//
	Init_Ad_1(); //ad 配置
//
	//Init_TMR_1();  //pwm 配置（1ms中断配置）(名字更改TMR)
//
	Init_PWM_1();
//
	Init_GPIO_1(); //gpi 启动配置

	/* Initialize LPTMR */
	LPTMR_DRV_Init(INST_LPTMR1, &lpTmr1_config0, false);
	INT_SYS_InstallHandler(LPTMR0_IRQn, LPTMR_ISR, (isr_t *)NULL);
	//INT_SYS_EnableIRQ(LPTMR0_IRQn);
   INT_SYS_EnableIRQ(LPIT0_Ch0_IRQn);
	INT_SYS_EnableIRQ(LPTMR0_IRQn);
	LPTMR_DRV_StartCounter(INST_LPTMR1);
//
//	(void)FLASH_Init(10000000);
}
