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
#define     AD_FANBACK 			     	IGPIO_PTA0       //adc0_se0
#define	    AD_VOLT		      	IGPIO_PTA1              //adc0_se1
#define   	ADT_PTL1    			IGPIO_PTA2           //ADC1_SE0
#define		AD_PRESS_PTL1  		IGPIO_PTA3         //ADC1_seg1
#define		JTAG_TMS	     	    IGPIO_PTA4
#define     SWD_RESET               IGPIO_PTA5
#define     UN_USA6     	        GPIO_PTA6
#define     LIN_EN                 GPIO_PTA7
#define		I_PTC2_FB             	GPIO_PTA8
#define		I_SOV4_FB              	IGPIO_PTA9
#define  	JTAG_TDO     		    IGPIO_PTA10
#define		PWM_PUMP2             	GPIO_PTA11      //ftm1_ch5
#define		PWM_PUMP1              	GPIO_PTA12      //ftm1_ch6
#define		SPI_CLK               	GPIO_PTA13
#define		SPI_OUT               	GPIO_PTA14
#define     AD_BATT                 IGPIO_PTA15     //adc1_se12
#define     CAN_KBI_1               IGPIO_PTA16     //adc1_s13
#define     PWM_PUMP3                GPIO_PTA17    //ftm0_ch6

//0x027b44
//
//ptb
#define		LIN_KBI	     	           IGPIO_PTB0
#define		O_5V_EN	         	     GPIO_PTB1
#define		CD4051_B_1	         	  GPIO_PTB2
#define		CD4051_C_1	         	  GPIO_PTB3
#define     O_KEY3                   GPIO_PTB4
#define     O_PTC2                   GPIO_PTB5    //FANREALY
#define		UN_USB6             	 IGPIO_PTB6   //xtal
#define		UN_USB7             	 IGPIO_PTB7
#define		O_CAN_EN_2           	 GPIO_PTB8
#define		IIC_SCL           	 GPIO_PTB9
//#define		IIC_SDA 	                 IGPIO_PTC11
#define		IIC_SDA             	 GPIO_PTB10
#define		I_PUMP1_FB               IGPIO_PTB11
#define     AD_PRESS_PTL2            IGPIO_PTB12   //ADC1_SE7
#define     ADT_TAMB                 IGPIO_PTB13   //ADC1_SE8
#define     ADM_TEMP                 IGPIO_PTB14   //ADC1_SE9
#define     ADM_NCF                  IGPIO_PTB15   //ADC1_SE14
#define     ADM_MODE                 IGPIO_PTB16   //ADC1_SE15
#define     I_PUMP3_FB               IGPIO_PTB17   //FTM0_CH5

//ptc
#define		CD4051_C_2      	       GPIO_PTC0
#define		OL_LCD      	          GPIO_PTC1
#define		CAN3_RX	         	     IGPIO_PTC2   //can0
#define		CAN3_TX	         	     IGPIO_PTC3
#define     JTAG_TCLK                IGPIO_PTC4
#define     JTAG_TDI                 IGPIO_PTC5
#define     CAN1_RX                   IGPIO_PTC6    // ADC1_SE4
#define     CAN1_TX                   IGPIO_PTC7    // ADC1_SE5
#define     LIN_RX                    IGPIO_PTC8    //    LPUART1
#define     LIN_TX                    GPIO_PTC9    //

#define		O_RHSENSOR_CLK    	     GPIO_PTC10
//#define		IIC_SDA 	                 IGPIO_PTC11
#define		O_CAN_EN_1 	               GPIO_PTC11
#define		AD_CD4051_1   	           IGPIO_PTC12
#define		CD4051_A_1	              GPIO_PTC13
#define		AD_CD4051_2	              IGPIO_PTC14
#define		CAN_KBI_2 	              IGPIO_PTC15
#define     CAN2_RX                   IGPIO_PTC16
#define		CAN2_TX	                 IGPIO_PTC17

//ptD
#define		I_INCARFAN1_FB            IGPIO_PTD0
#define		I_SOV3_FB          	     IGPIO_PTD1
#define     AD_PRESS_PTH              IGPIO_PTD2   //ADC1_SE2
#define     ADT_PTL2                  IGPIO_PTD3   //ADC1_SE3
#define     ADT_PTH                   IGPIO_PTD4   //ADC1_SE6
#define     O_PTC3                     GPIO_PTD5     //O_RDEF
#define     O_12V_EN                   GPIO_PTD6    //UART2
#define     O_PUMP3                    GPIO_PTD7     //不用
#define		CD4051_A_2 	              GPIO_PTD8
#define		CD4051_B_2          	     GPIO_PTD9
#define		O_CAN_EN_3          	      GPIO_PTD10   //LDO1 的输出
#define		O_PUMP1           	      GPIO_PTD11   //LDO1 的FB    
#define		O_PUMP2          	      GPIO_PTD12   //LDO1 的输出  
#define		O_INCARFAN_2              GPIO_PTD13
#define		O_RHEAT          	     GPIO_PTD14              //O_PTC3
#define		O_PTC1          	     GPIO_PTD15             //O_APTC1  O_PTC1
#define		O_FAN_RELAY              GPIO_PTD16             //O_PTC2
#define		O_SOV1          	     GPIO_PTD17


//ptD
#define		O_MCP_CS                GPIO_PTE0     //6836 CS
#define		I_PUMP2_FB          	   IGPIO_PTE1
#define		AD_SUN             	   IGPIO_PTE2  //ADC1_SE10
#define     O_SOV3                  GPIO_PTE3
#define		PWM_FAN          	      GPIO_PTE4     //FTM2_CH2
#define		I_PTC1_FB          	     GPIO_PTE5
#define		AD_TEVP          	     IGPIO_PTE6  //adc1_SE11
#define		O_PTC4           	       GPIO_PTE7  //TMU继电器 O_TMU
#define		O_INCARFAN_1           GPIO_PTE8   //LDO2 的FB
#define		O_TMU       	       GPIO_PTE9   //O_PTC4
#define		I_PTC3_FB          	    GPIO_PTE10
#define		I_INCARFAN2_FB          IGPIO_PTE11
#define		O_SOV2         	     GPIO_PTE12
#define		I_PTC4_FB          	    IGPIO_PTE13
#define		O_SOV4          	     GPIO_PTE14
#define		I_SOV2_FB          	     IGPIO_PTE15
#define		I_SOV1_FB          	     IGPIO_PTE16


#define     O_5VEN                   GPIO_PTB1
//#define     O_FAN_RELAY              GPIO_PTB1

//#define		O_PTC4          	     UN_USB6
#define    O_KEY4             I_PTC1_FB
#define    O_KEY1             O_PUMP2
#define    O_KEY2             O_PUMP1

#define         OL_RHEAT        O_PTC1     //改成后除霜
#define         OL_AC           O_PTC2     //AC
#define    	    OL_CIRF         O_PTC3      //外循环
#define         OL_PTC          O_PTC4    //PTC
/*******************************************************************************
 *定义pwm 信号的最大值和 pwm 口定义 
 *
********************************************************************************/
#define   PWMMODE_FAN      4000     //鼓风机


//pwm 口定义 
#define		LED_PWM           FTM2->CONTROLS[3].CnV   
#define     OUTFAN_PWM			FTM2->CONTROLS[2].CnV   //鼓风机风量输出


#define   PWMMODE_PUMP     5000
#define     PUMP1_PWM			FTM1->CONTROLS[6].CnV
#define     PUMP2_PWM			FTM1->CONTROLS[5].CnV
#define     PUMP3_PWM			FTM0->CONTROLS[6].CnV  
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
//4051_1   0- CD35_FAN_B   1- CD35_FAN_C   2- I_RHDATA   3-CD35_FAN_A

//4051_1	            	4051_2
//Net_IO(FANswich2)	0	Net_IO(TEMP3)	2
//Net_IO(FANswich3)	1	Net_SW(X-2) 	3
//Net_IO(FANswich1)	3	Net_SW(X-1)	4
//Net_IO(TEMP1)	   5
//Net_IO(TEMP2)	   7

#define  CD35_FAN_1_CH       0x15
#define  CD35_FAN_2_CH       0x17
#define  CD35_FAN_3_CH       0x22
#define  CD35_TEMP_1_CH      0x13
#define  CD35_TEMP_2_CH      0x10
#define  CD35_TEMP_3_CH      0x11
#define  IKEY1_CH            0x24
#define  IKEY2_CH            0x23

void	Init_MCU(void);
void  Init_GPIO_0(void);
void  Init_GPIO_1(void);
#endif /* MCU_PORT_H_ */
