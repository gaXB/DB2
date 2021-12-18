/************************文件说明*****************************************************
* Copyright (c) 2019,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：MCU_PORT.h
* 功能描述：MCU 的端口定义，所有端口定义放在此处，除CANEN
* 此文件会被所有文件调用
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日
* 
* * 版本：1.1
* 说明：增加MCUPMLogic
* 原作者：xb
* 完成日期：2020年5月13日
*/

#ifndef MCU_PORT_H_
#define MCU_PORT_H_
#include "cpu.h"/* for EnableInterrupts macro */
#include "MCUPMLogic.h"
#include "gpio.h"

extern int i16GPIO, i16GPION, i16ADCH;
#define GPIOA_PTD1 i16GPIO
#define GPIO_UNUSE i16GPION
#define  ADCH   i16ADCH
//注意： 所有端口定义除CANEN 都放在此处，没用端口定义为UN_USXn（比如B5）
//PTAD
#define     ADM_NCF 			   	IGPIO_PTA0       //adc0_se0
#define	    ADT_CHILLEROUT		  	IGPIO_PTA1       //adc0_se1
#define   	UN_USA2    				GPIO_PTA2
#define		TEMP_Dsicharge  		IGPIO_PTA3         //ADC1_seg1
#define		JTAG_TMS	     	    IGPIO_PTA4
#define     SWD_RESET               IGPIO_PTA5
#define     UN_USA6     	        GPIO_PTA6
#define     TLE9262_WK2             IGPIO_PTA7
#define		UN_USA8             	GPIO_PTA8
#define		UN_USA9              	GPIO_PTA9
#define  	JTAG_TDO     		    IGPIO_PTA10
#define		UN_USA11             	GPIO_PTA11
#define		UN_USA12              	GPIO_PTA12
#define		UN_USA13             	GPIO_PTA13
#define		UN_USA14              	GPIO_PTA14
#define     AD_PRESS_H              IGPIO_PTA15     //adc1_se12
#define     ADM_MODE_FRONT          IGPIO_PTA16     //adc1_s13
#define     UN_USA17                GPIO_PTA17

//0x027b44
//
//ptb
#define		TLE94112_CS	     	     GPIO_PTB0
#define		SPI_OUT	         	     GPIO_PTB1
#define		SPI_CLK	         	     GPIO_PTB2
#define		SPI_IN	         	     IGPIO_PTB3
#define     UN_USB4                  GPIO_PTB4
#define     TLE9262_CS               GPIO_PTB5
#define		UN_USB6             	 IGPIO_PTB6   //xtal
#define		UN_USB7             	 IGPIO_PTB7
#define		LDO2_OUT_FB           	 IGPIO_PTB8
#define		LDO1_OUT_FB           	 IGPIO_PTB9
#define		UN_USB10             	 GPIO_PTB10
#define		TLE94112_EN              GPIO_PTB11
#define     ADT_PTC                  IGPIO_PTB12   //ADC1_SE7
#define     ADT_TEVP                 IGPIO_PTB13   //ADC1_SE8
#define     ADT_TAMB                 IGPIO_PTB14   //ADC1_SE9
#define     ADT_TIN                  IGPIO_PTB15   //ADC1_SE14
#define     ADV_FAN                  IGPIO_PTB16   //ADC1_SE15
#define     PWM_FANOUT               GPIO_PTB17   //FTM0_CH5

//ptc
#define		A4980_STEP_COOL 	     GPIO_PTC0
#define		A4980_DIR_COOL	         GPIO_PTC1
#define		CAN0_RX	         	     IGPIO_PTC2
#define		CAN0_TX	         	     IGPIO_PTC3
#define     JTAG_TCLK                IGPIO_PTC4
#define     JTAG_TDI                 IGPIO_PTC5
#define     ADV_FAN_BATT             IGPIO_PTC6    // ADC1_SE4
#define     ADM_MODE_REAR            IGPIO_PTC7    // ADC1_SE5
#define		A4980_DIAG_CHILER 	     IGPIO_PTC8
#define		A4980_EN_CHILER	         GPIO_PTC9
#define		A4980_DIR_CHILER	     GPIO_PTC10
#define		A4980_STEP_CHILER 	     GPIO_PTC11
#define		A4980_DIR_HEAT   	     GPIO_PTC12
#define		A4980_EN_HEAT	         GPIO_PTC13
#define		A4980_STEP_HEAT	         GPIO_PTC14
#define		A4980_DIAG_HEAT 	     IGPIO_PTC15
#define     UN_USC16                 GPIO_PTC16
#define		A4980_EN_COOL	         GPIO_PTC17

//ptD
#define		UN_USD0          	     GPIO_PTD0
#define		UN_USD1          	     GPIO_PTD1
#define     ADT_ICOND_OUTLET         IGPIO_PTD2   //ADC1_SE2
#define     ADT_OCOND_OUTLET         IGPIO_PTD3   //ADC1_SE3
#define     ADT_EVAP_PUTLET          IGPIO_PTD4   //ADC1_SE6
#define     TLE9262_INT              IGPIO_PTD5
#define     LIN_RX                   IGPIO_PTD6    //UART2
#define     LIN_TX                   GPIO_PTD7
#define		A4980_DIAG_COOL 	     IGPIO_PTD8
#define		UN_USD9          	     GPIO_PTD9
#define		LDO1_IN          	     GPIO_PTD10   //LDO1 的输出
#define		LDO1_FB          	     IGPIO_PTD11   //LDO1 的FB
#define		LDO2_IN          	     GPIO_PTD12   //LDO1 的输出
#define		UN_USD13          	     GPIO_PTD13
#define		UN_USD14          	     GPIO_PTD14
#define		UN_USD15          	     GPIO_PTD15
#define		UN_USD16          	     GPIO_PTD16
#define		UN_USD17          	     GPIO_PTD17


//ptD
#define		UN_USE0          	     GPIO_PTE0
#define		UN_USE1          	     GPIO_PTE1
#define		AD_PRESS_L          	 IGPIO_PTE2  //ADC1_SE10
#define		UN_USE3          	     GPIO_PTE3
#define		UN_USE4          	     GPIO_PTE4
#define		UN_USE5          	     GPIO_PTE5
#define		ADM_MIX          	     IGPIO_PTE6  //adc1_SE11
#define		UN_USE7          	     GPIO_PTE7
#define		LDO2_FB          	     IGPIO_PTE8   //LDO2 的FB
#define		UN_USE9          	     GPIO_PTE9
#define		UN_USE10          	     GPIO_PTE10
#define		UN_USE11          	     GPIO_PTE11
#define		UN_USE12         	     GPIO_PTE12
#define		UN_USE13          	     GPIO_PTE14
#define		UN_USE15          	     GPIO_PTE15
#define		UN_USE16          	     GPIO_PTE16

#define     O_5VEN                   GPIO_PTB1
#define     O_FAN_RELAY              GPIO_PTB1
/*******************************************************************************
 *定义pwm 信号的最大值和 pwm 口定义 
 *
********************************************************************************/
#define   PWMMODE_FAN      5000     //鼓风机


//pwm 口定义 
#define		LED_PWM           FTM2->CONTROLS[3].CnV   
#define     OUTFAN_PWM			FTM2->CONTROLS[3].CnV   //鼓风机风量输出


//#define   _BIT_BME_BFI_   //BME 功能使能
 
#ifdef   _BIT_BME_BFI_
#define  GPIO_SET      0xFFFFFFFFu
#define  GPIO_CLEAR    0
#else
#define  GPIO_SET      1
#define  GPIO_CLEAR    0
#endif


/*******************************************************************************
 * 地址定义
 *
********************************************************************************/
#if  BOOT_MCU_KEA64
#define CODE_END_ADDR   0x000F000u         // 64K 0xffff  最后 4k 作为flash 模拟ee
#else
#define CODE_END_ADDR   0x00040000u
#endif

#define CODE_START_ADDR 0x00004000u         // 0 -0x3fff   16k 为bootloader 区域
#define FLASH_PAGE_SIZE 0x0200u

/*******************************************************************************
 * 总线时钟定义，及1ms定时需要每一步的值
 *
********************************************************************************/
#define   BUS_CLK_HZ      10000000
/*******************************************************************************
 *MCU 固定接口不能动
 *
********************************************************************************/


void	Init_MCU(void);
void  Init_GPIO_0(void);
void  Init_GPIO_1(void);
#endif /* MCU_PORT_H_ */
