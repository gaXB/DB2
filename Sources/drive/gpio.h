/******************************************************************************
 * ver0.04
 * 20190325
 * 1 加入GPIO_UNUSE
 * 2 BME 功能使能时要加入此句，否则屏蔽
 *  #define   _BIT_BME_GPIO_   BME 功能使能    注意此位和 _BIT_BME_BFI_ 要一样
 * 
 * ver0.0.3 
*2017/8/27
*1适应有BME 功能
*1 
*1 
 * *ver0.0.2 
*2016/10/8
*1 keil 的<<31 会报错，所以就取消此块代码没实际用处


 * *ver0.0.1 
*2015/9/6
*1 增加输入引脚的未定义
*1 端口为输入时需要用  IGPIOx->PTn0
*

*ver0.0.0 
*2015/8/23
*1 为了便于端口更改，和符合原来模块中直接更改端口的方式
*使用GPIOX->PDOR 来更改端口电平。 并提供位定义例如 GPIOA->PTA0
*
***************************************************************************/
/*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************//*!
*
* @file GPIO.h
*
* @version 0.0.1
*
* @date Jun 12, 2013
*
* @brief provide commond GPIO utilities. 
*
*******************************************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_


#include "device_registers.h"
//#include "stdint.h"


#define   _BIT_BME_GPIO_   //BME 功能使能    注意此位和 _BIT_BME_BFI_ 要一样
#define GPIO_BIT_EQUAL(GPIO_BIT, value) \
{\
	if (value) \
	{\
		GPIO_BIT = GPIO_SET;\
	}\
	else\
	{\
		GPIO_BIT = GPIO_CLEAR;\
	}\
}
/******************************************************************************
*define gpio pin name
*
*******************************************************************************/
typedef volatile union {
	uint32_t Byte;
  struct {
    unsigned PTA0        :1;                                       
    unsigned PTA1        :1;                          
    unsigned PTA2	     :1; 
    unsigned PTA3        :1; 
    unsigned PTA4        :1; 
    unsigned PTA5   	 :1;                                
    unsigned PTA6   	 :1;                                      
    unsigned PTA7	     :1;        
    unsigned PTA8        :1;
    unsigned PTA9        :1;
    unsigned PTA10	     :1;
    unsigned PTA11        :1;
    unsigned PTA12        :1;
    unsigned PTA13   	 :1;
    unsigned PTA14   	 :1;
    unsigned PTA15	     :1;
    unsigned PTA16        :1;
    unsigned PTA17        :1;
    unsigned PTA18	     :1;
    unsigned PTA19        :1;
    unsigned PTA20        :1;
    unsigned PTA21   	 :1;
    unsigned PTA22   	 :1;
    unsigned PTA23	     :1;
    unsigned PTA24        :1;
    unsigned PTA25        :1;
    unsigned PTA26	     :1;
    unsigned PTA27        :1;
    unsigned PTA28        :1;
    unsigned PTA29   	 :1;
    unsigned PTA30   	 :1;
    unsigned PTA31	     :1;
  } Bits;
} PORTASTR;
#define  OGPIOA  ((PORTASTR*)PTA_BASE)

#define  GPIO_PTA0    OGPIOA->Bits.PTA0
#define  GPIO_PTA1    OGPIOA->Bits.PTA1
#define  GPIO_PTA2    OGPIOA->Bits.PTA2
#define  GPIO_PTA3    OGPIOA->Bits.PTA3
#define  GPIO_PTA4    OGPIOA->Bits.PTA4
#define  GPIO_PTA5    OGPIOA->Bits.PTA5
#define  GPIO_PTA6    OGPIOA->Bits.PTA6
#define  GPIO_PTA7    OGPIOA->Bits.PTA7

#define  GPIO_PTA8    OGPIOA->Bits.PTA8
#define  GPIO_PTA9    OGPIOA->Bits.PTA9
#define  GPIO_PTA10   OGPIOA->Bits.PTA10
#define  GPIO_PTA11   OGPIOA->Bits.PTA11
#define  GPIO_PTA12   OGPIOA->Bits.PTA12
#define  GPIO_PTA13   OGPIOA->Bits.PTA13
#define  GPIO_PTA14   OGPIOA->Bits.PTA14
#define  GPIO_PTA15   OGPIOA->Bits.PTA15

#define  GPIO_PTA16   OGPIOA->Bits.PTA16
#define  GPIO_PTA17   OGPIOA->Bits.PTA17
#define  GPIO_PTA18   OGPIOA->Bits.PTA18
#define  GPIO_PTA19   OGPIOA->Bits.PTA19
#define  GPIO_PTA20   OGPIOA->Bits.PTA20
#define  GPIO_PTA21   OGPIOA->Bits.PTA21
#define  GPIO_PTA22   OGPIOA->Bits.PTA22
#define  GPIO_PTA23   OGPIOA->Bits.PTA23

#define  GPIO_PTA24   OGPIOA->Bits.PTA24
#define  GPIO_PTA25   OGPIOA->Bits.PTA25
#define  GPIO_PTA26   OGPIOA->Bits.PTA26
#define  GPIO_PTA27   OGPIOA->Bits.PTA27
#define  GPIO_PTA28   OGPIOA->Bits.PTA28
#define  GPIO_PTA29   OGPIOA->Bits.PTA29
#define  GPIO_PTA30   OGPIOA->Bits.PTA30
#define  GPIO_PTA31   OGPIOA->Bits.PTA31
    
/*! @} End of gpio_api_list */

/******************************************************************************
*define gpio pin name
*
*******************************************************************************/
typedef volatile union {
	uint32_t Byte;
  struct {
    unsigned PTB0        :1;
    unsigned PTB1        :1;
    unsigned PTB2	     :1;
    unsigned PTB3        :1;
    unsigned PTB4        :1;
    unsigned PTB5   	 :1;
    unsigned PTB6   	 :1;
    unsigned PTB7	     :1;
    unsigned PTB8        :1;
    unsigned PTB9        :1;
    unsigned PTB10	     :1;
    unsigned PTB11        :1;
    unsigned PTB12        :1;
    unsigned PTB13   	 :1;
    unsigned PTB14   	 :1;
    unsigned PTB15	     :1;
    unsigned PTB16        :1;
    unsigned PTB17        :1;
    unsigned PTB18	     :1;
    unsigned PTB19        :1;
    unsigned PTB20        :1;
    unsigned PTB21   	 :1;
    unsigned PTB22   	 :1;
    unsigned PTB23	     :1;
    unsigned PTB24        :1;
    unsigned PTB25        :1;
    unsigned PTB26	     :1;
    unsigned PTB27        :1;
    unsigned PTB28        :1;
    unsigned PTB29   	 :1;
    unsigned PTB30   	 :1;
    unsigned PTB31	     :1;
  } Bits;
} PORTBSTR;
#define  OGPIOB  ((PORTBSTR*)PTB_BASE)

#define  GPIO_PTB0    OGPIOB->Bits.PTB0
#define  GPIO_PTB1    OGPIOB->Bits.PTB1
#define  GPIO_PTB2    OGPIOB->Bits.PTB2
#define  GPIO_PTB3    OGPIOB->Bits.PTB3
#define  GPIO_PTB4    OGPIOB->Bits.PTB4
#define  GPIO_PTB5    OGPIOB->Bits.PTB5
#define  GPIO_PTB6    OGPIOB->Bits.PTB6
#define  GPIO_PTB7    OGPIOB->Bits.PTB7

#define  GPIO_PTB8    OGPIOB->Bits.PTB8
#define  GPIO_PTB9    OGPIOB->Bits.PTB9
#define  GPIO_PTB10   OGPIOB->Bits.PTB10
#define  GPIO_PTB11   OGPIOB->Bits.PTB11
#define  GPIO_PTB12   OGPIOB->Bits.PTB12
#define  GPIO_PTB13   OGPIOB->Bits.PTB13
#define  GPIO_PTB14   OGPIOB->Bits.PTB14
#define  GPIO_PTB15   OGPIOB->Bits.PTB15

#define  GPIO_PTB16   OGPIOB->Bits.PTB16
#define  GPIO_PTB17   OGPIOB->Bits.PTB17
#define  GPIO_PTB18   OGPIOB->Bits.PTB18
#define  GPIO_PTB19   OGPIOB->Bits.PTB19
#define  GPIO_PTB20   OGPIOB->Bits.PTB20
#define  GPIO_PTB21   OGPIOB->Bits.PTB21
#define  GPIO_PTB22   OGPIOB->Bits.PTB22
#define  GPIO_PTB23   OGPIOB->Bits.PTB23

#define  GPIO_PTB24   OGPIOB->Bits.PTB24
#define  GPIO_PTB25   OGPIOB->Bits.PTB25
#define  GPIO_PTB26   OGPIOB->Bits.PTB26
#define  GPIO_PTB27   OGPIOB->Bits.PTB27
#define  GPIO_PTB28   OGPIOB->Bits.PTB28
#define  GPIO_PTB29   OGPIOB->Bits.PTB29
#define  GPIO_PTB30   OGPIOB->Bits.PTB30
#define  GPIO_PTB31   OGPIOB->Bits.PTB31

//END B

/******************************************************************************
*define gpioc pin name
*
*******************************************************************************/
typedef volatile union {
	uint32_t Byte;
  struct {
    unsigned PTC0        :1;
    unsigned PTC1        :1;
    unsigned PTC2	     :1;
    unsigned PTC3        :1;
    unsigned PTC4        :1;
    unsigned PTC5   	 :1;
    unsigned PTC6   	 :1;
    unsigned PTC7	     :1;
    unsigned PTC8        :1;
    unsigned PTC9        :1;
    unsigned PTC10	     :1;
    unsigned PTC11        :1;
    unsigned PTC12        :1;
    unsigned PTC13   	 :1;
    unsigned PTC14   	 :1;
    unsigned PTC15	     :1;
    unsigned PTC16        :1;
    unsigned PTC17        :1;
    unsigned PTC18	     :1;
    unsigned PTC19        :1;
    unsigned PTC20        :1;
    unsigned PTC21   	 :1;
    unsigned PTC22   	 :1;
    unsigned PTC23	     :1;
    unsigned PTC24        :1;
    unsigned PTC25        :1;
    unsigned PTC26	     :1;
    unsigned PTC27        :1;
    unsigned PTC28        :1;
    unsigned PTC29   	 :1;
    unsigned PTC30   	 :1;
    unsigned PTC31	     :1;
  } Bits;
} PORTCSTR;
#define  OGPIOC  ((PORTCSTR*)PTC_BASE)

#define  GPIO_PTC0    OGPIOC->Bits.PTC0
#define  GPIO_PTC1    OGPIOC->Bits.PTC1
#define  GPIO_PTC2    OGPIOC->Bits.PTC2
#define  GPIO_PTC3    OGPIOC->Bits.PTC3
#define  GPIO_PTC4    OGPIOC->Bits.PTC4
#define  GPIO_PTC5    OGPIOC->Bits.PTC5
#define  GPIO_PTC6    OGPIOC->Bits.PTC6
#define  GPIO_PTC7    OGPIOC->Bits.PTC7

#define  GPIO_PTC8    OGPIOC->Bits.PTC8
#define  GPIO_PTC9    OGPIOC->Bits.PTC9
#define  GPIO_PTC10   OGPIOC->Bits.PTC10
#define  GPIO_PTC11   OGPIOC->Bits.PTC11
#define  GPIO_PTC12   OGPIOC->Bits.PTC12
#define  GPIO_PTC13   OGPIOC->Bits.PTC13
#define  GPIO_PTC14   OGPIOC->Bits.PTC14
#define  GPIO_PTC15   OGPIOC->Bits.PTC15

#define  GPIO_PTC16   OGPIOC->Bits.PTC16
#define  GPIO_PTC17   OGPIOC->Bits.PTC17
#define  GPIO_PTC18   OGPIOC->Bits.PTC18
#define  GPIO_PTC19   OGPIOC->Bits.PTC19
#define  GPIO_PTC20   OGPIOC->Bits.PTC20
#define  GPIO_PTC21   OGPIOC->Bits.PTC21
#define  GPIO_PTC22   OGPIOC->Bits.PTC22
#define  GPIO_PTC23   OGPIOC->Bits.PTC23

#define  GPIO_PTC24   OGPIOC->Bits.PTC24
#define  GPIO_PTC25   OGPIOC->Bits.PTC25
#define  GPIO_PTC26   OGPIOC->Bits.PTC26
#define  GPIO_PTC27   OGPIOC->Bits.PTC27
#define  GPIO_PTC28   OGPIOC->Bits.PTC28
#define  GPIO_PTC29   OGPIOC->Bits.PTC29
#define  GPIO_PTC30   OGPIOC->Bits.PTC30
#define  GPIO_PTC31   OGPIOC->Bits.PTC31

/******************************************************************************
*define gpioD pin name
*
*******************************************************************************/
typedef volatile union {
	uint32_t Byte;
  struct {
    unsigned PTD0        :1;
    unsigned PTD1        :1;
    unsigned PTD2	     :1;
    unsigned PTD3        :1;
    unsigned PTD4        :1;
    unsigned PTD5   	 :1;
    unsigned PTD6   	 :1;
    unsigned PTD7	     :1;
    unsigned PTD8        :1;
    unsigned PTD9        :1;
    unsigned PTD10	     :1;
    unsigned PTD11        :1;
    unsigned PTD12        :1;
    unsigned PTD13   	 :1;
    unsigned PTD14   	 :1;
    unsigned PTD15	     :1;
    unsigned PTD16        :1;
    unsigned PTD17        :1;
    unsigned PTD18	     :1;
    unsigned PTD19        :1;
    unsigned PTD20        :1;
    unsigned PTD21   	 :1;
    unsigned PTD22   	 :1;
    unsigned PTD23	     :1;
    unsigned PTD24        :1;
    unsigned PTD25        :1;
    unsigned PTD26	     :1;
    unsigned PTD27        :1;
    unsigned PTD28        :1;
    unsigned PTD29   	 :1;
    unsigned PTD30   	 :1;
    unsigned PTD31	     :1;
  } Bits;
} PORTDSTR;
#define  OGPIOD  ((PORTDSTR*)PTD_BASE)

#define  GPIO_PTD0    OGPIOD->Bits.PTD0
#define  GPIO_PTD1    OGPIOD->Bits.PTD1
#define  GPIO_PTD2    OGPIOD->Bits.PTD2
#define  GPIO_PTD3    OGPIOD->Bits.PTD3
#define  GPIO_PTD4    OGPIOD->Bits.PTD4
#define  GPIO_PTD5    OGPIOD->Bits.PTD5
#define  GPIO_PTD6    OGPIOD->Bits.PTD6
#define  GPIO_PTD7    OGPIOD->Bits.PTD7

#define  GPIO_PTD8    OGPIOD->Bits.PTD8
#define  GPIO_PTD9    OGPIOD->Bits.PTD9
#define  GPIO_PTD10   OGPIOD->Bits.PTD10
#define  GPIO_PTD11   OGPIOD->Bits.PTD11
#define  GPIO_PTD12   OGPIOD->Bits.PTD12
#define  GPIO_PTD13   OGPIOD->Bits.PTD13
#define  GPIO_PTD14   OGPIOD->Bits.PTD14
#define  GPIO_PTD15   OGPIOD->Bits.PTD15

#define  GPIO_PTD16   OGPIOD->Bits.PTD16
#define  GPIO_PTD17   OGPIOD->Bits.PTD17
#define  GPIO_PTD18   OGPIOD->Bits.PTD18
#define  GPIO_PTD19   OGPIOD->Bits.PTD19
#define  GPIO_PTD20   OGPIOD->Bits.PTD20
#define  GPIO_PTD21   OGPIOD->Bits.PTD21
#define  GPIO_PTD22   OGPIOD->Bits.PTD22
#define  GPIO_PTD23   OGPIOD->Bits.PTD23

#define  GPIO_PTD24   OGPIOD->Bits.PTD24
#define  GPIO_PTD25   OGPIOD->Bits.PTD25
#define  GPIO_PTD26   OGPIOD->Bits.PTD26
#define  GPIO_PTD27   OGPIOD->Bits.PTD27
#define  GPIO_PTD28   OGPIOD->Bits.PTD28
#define  GPIO_PTD29   OGPIOD->Bits.PTD29
#define  GPIO_PTD30   OGPIOD->Bits.PTD30
#define  GPIO_PTD31   OGPIOD->Bits.PTD31


/******************************************************************************
*define Igpio pin name
*
*******************************************************************************/

#define  IGPIOA  ((PORTASTR*)(PTA_BASE + 0X10))

#define  IGPIO_PTA0    IGPIOA->Bits.PTA0
#define  IGPIO_PTA1    IGPIOA->Bits.PTA1
#define  IGPIO_PTA2    IGPIOA->Bits.PTA2
#define  IGPIO_PTA3    IGPIOA->Bits.PTA3
#define  IGPIO_PTA4    IGPIOA->Bits.PTA4
#define  IGPIO_PTA5    IGPIOA->Bits.PTA5
#define  IGPIO_PTA6    IGPIOA->Bits.PTA6
#define  IGPIO_PTA7    IGPIOA->Bits.PTA7

#define  IGPIO_PTA8    IGPIOA->Bits.PTA8
#define  IGPIO_PTA9    IGPIOA->Bits.PTA9
#define  IGPIO_PTA10   IGPIOA->Bits.PTA10
#define  IGPIO_PTA11   IGPIOA->Bits.PTA11
#define  IGPIO_PTA12   IGPIOA->Bits.PTA12
#define  IGPIO_PTA13   IGPIOA->Bits.PTA13
#define  IGPIO_PTA14   IGPIOA->Bits.PTA14
#define  IGPIO_PTA15   IGPIOA->Bits.PTA15

#define  IGPIO_PTA16   IGPIOA->Bits.PTA16
#define  IGPIO_PTA17   IGPIOA->Bits.PTA17
#define  IGPIO_PTA18   IGPIOA->Bits.PTA18
#define  IGPIO_PTA19   IGPIOA->Bits.PTA19
#define  IGPIO_PTA20   IGPIOA->Bits.PTA20
#define  IGPIO_PTA21   IGPIOA->Bits.PTA21
#define  IGPIO_PTA22   IGPIOA->Bits.PTA22
#define  IGPIO_PTA23   IGPIOA->Bits.PTA23

#define  IGPIO_PTA24   IGPIOA->Bits.PTA24
#define  IGPIO_PTA25   IGPIOA->Bits.PTA25
#define  IGPIO_PTA26   IGPIOA->Bits.PTA26
#define  IGPIO_PTA27   IGPIOA->Bits.PTA27
#define  IGPIO_PTA28   IGPIOA->Bits.PTA28
#define  IGPIO_PTA29   IGPIOA->Bits.PTA29
#define  IGPIO_PTA30   IGPIOA->Bits.PTA30
#define  IGPIO_PTA31   IGPIOA->Bits.PTA31

/*! @} End of Igpio_api_list */

/******************************************************************************
*define Igpio pin name
*
*******************************************************************************/
#define  IGPIOB  ((PORTBSTR*)(PTB_BASE+0X10))

#define  IGPIO_PTB0    IGPIOB->Bits.PTB0
#define  IGPIO_PTB1    IGPIOB->Bits.PTB1
#define  IGPIO_PTB2    IGPIOB->Bits.PTB2
#define  IGPIO_PTB3    IGPIOB->Bits.PTB3
#define  IGPIO_PTB4    IGPIOB->Bits.PTB4
#define  IGPIO_PTB5    IGPIOB->Bits.PTB5
#define  IGPIO_PTB6    IGPIOB->Bits.PTB6
#define  IGPIO_PTB7    IGPIOB->Bits.PTB7

#define  IGPIO_PTB8    IGPIOB->Bits.PTB8
#define  IGPIO_PTB9    IGPIOB->Bits.PTB9
#define  IGPIO_PTB10   IGPIOB->Bits.PTB10
#define  IGPIO_PTB11   IGPIOB->Bits.PTB11
#define  IGPIO_PTB12   IGPIOB->Bits.PTB12
#define  IGPIO_PTB13   IGPIOB->Bits.PTB13
#define  IGPIO_PTB14   IGPIOB->Bits.PTB14
#define  IGPIO_PTB15   IGPIOB->Bits.PTB15

#define  IGPIO_PTB16   IGPIOB->Bits.PTB16
#define  IGPIO_PTB17   IGPIOB->Bits.PTB17
#define  IGPIO_PTB18   IGPIOB->Bits.PTB18
#define  IGPIO_PTB19   IGPIOB->Bits.PTB19
#define  IGPIO_PTB20   IGPIOB->Bits.PTB20
#define  IGPIO_PTB21   IGPIOB->Bits.PTB21
#define  IGPIO_PTB22   IGPIOB->Bits.PTB22
#define  IGPIO_PTB23   IGPIOB->Bits.PTB23

#define  IGPIO_PTB24   IGPIOB->Bits.PTB24
#define  IGPIO_PTB25   IGPIOB->Bits.PTB25
#define  IGPIO_PTB26   IGPIOB->Bits.PTB26
#define  IGPIO_PTB27   IGPIOB->Bits.PTB27
#define  IGPIO_PTB28   IGPIOB->Bits.PTB28
#define  IGPIO_PTB29   IGPIOB->Bits.PTB29
#define  IGPIO_PTB30   IGPIOB->Bits.PTB30
#define  IGPIO_PTB31   IGPIOB->Bits.PTB31

//END B

/******************************************************************************
*define Igpioc pin name
*
*******************************************************************************/
#define  IGPIOC  ((PORTCSTR*)(PTC_BASE+0X10))

#define  IGPIO_PTC0    IGPIOC->Bits.PTC0
#define  IGPIO_PTC1    IGPIOC->Bits.PTC1
#define  IGPIO_PTC2    IGPIOC->Bits.PTC2
#define  IGPIO_PTC3    IGPIOC->Bits.PTC3
#define  IGPIO_PTC4    IGPIOC->Bits.PTC4
#define  IGPIO_PTC5    IGPIOC->Bits.PTC5
#define  IGPIO_PTC6    IGPIOC->Bits.PTC6
#define  IGPIO_PTC7    IGPIOC->Bits.PTC7

#define  IGPIO_PTC8    IGPIOC->Bits.PTC8
#define  IGPIO_PTC9    IGPIOC->Bits.PTC9
#define  IGPIO_PTC10   IGPIOC->Bits.PTC10
#define  IGPIO_PTC11   IGPIOC->Bits.PTC11
#define  IGPIO_PTC12   IGPIOC->Bits.PTC12
#define  IGPIO_PTC13   IGPIOC->Bits.PTC13
#define  IGPIO_PTC14   IGPIOC->Bits.PTC14
#define  IGPIO_PTC15   IGPIOC->Bits.PTC15

#define  IGPIO_PTC16   IGPIOC->Bits.PTC16
#define  IGPIO_PTC17   IGPIOC->Bits.PTC17
#define  IGPIO_PTC18   IGPIOC->Bits.PTC18
#define  IGPIO_PTC19   IGPIOC->Bits.PTC19
#define  IGPIO_PTC20   IGPIOC->Bits.PTC20
#define  IGPIO_PTC21   IGPIOC->Bits.PTC21
#define  IGPIO_PTC22   IGPIOC->Bits.PTC22
#define  IGPIO_PTC23   IGPIOC->Bits.PTC23

#define  IGPIO_PTC24   IGPIOC->Bits.PTC24
#define  IGPIO_PTC25   IGPIOC->Bits.PTC25
#define  IGPIO_PTC26   IGPIOC->Bits.PTC26
#define  IGPIO_PTC27   IGPIOC->Bits.PTC27
#define  IGPIO_PTC28   IGPIOC->Bits.PTC28
#define  IGPIO_PTC29   IGPIOC->Bits.PTC29
#define  IGPIO_PTC30   IGPIOC->Bits.PTC30
#define  IGPIO_PTC31   IGPIOC->Bits.PTC31

/******************************************************************************
*define IgpioD pin name
*
*******************************************************************************/
#define  IGPIOD  ((PORTDSTR*)(PTD_BASE + 0X10))

#define  IGPIO_PTD0    IGPIOD->Bits.PTD0
#define  IGPIO_PTD1    IGPIOD->Bits.PTD1
#define  IGPIO_PTD2    IGPIOD->Bits.PTD2
#define  IGPIO_PTD3    IGPIOD->Bits.PTD3
#define  IGPIO_PTD4    IGPIOD->Bits.PTD4
#define  IGPIO_PTD5    IGPIOD->Bits.PTD5
#define  IGPIO_PTD6    IGPIOD->Bits.PTD6
#define  IGPIO_PTD7    IGPIOD->Bits.PTD7

#define  IGPIO_PTD8    IGPIOD->Bits.PTD8
#define  IGPIO_PTD9    IGPIOD->Bits.PTD9
#define  IGPIO_PTD10   IGPIOD->Bits.PTD10
#define  IGPIO_PTD11   IGPIOD->Bits.PTD11
#define  IGPIO_PTD12   IGPIOD->Bits.PTD12
#define  IGPIO_PTD13   IGPIOD->Bits.PTD13
#define  IGPIO_PTD14   IGPIOD->Bits.PTD14
#define  IGPIO_PTD15   IGPIOD->Bits.PTD15

#define  IGPIO_PTD16   IGPIOD->Bits.PTD16
#define  IGPIO_PTD17   IGPIOD->Bits.PTD17
#define  IGPIO_PTD18   IGPIOD->Bits.PTD18
#define  IGPIO_PTD19   IGPIOD->Bits.PTD19
#define  IGPIO_PTD20   IGPIOD->Bits.PTD20
#define  IGPIO_PTD21   IGPIOD->Bits.PTD21
#define  IGPIO_PTD22   IGPIOD->Bits.PTD22
#define  IGPIO_PTD23   IGPIOD->Bits.PTD23

#define  IGPIO_PTD24   IGPIOD->Bits.PTD24
#define  IGPIO_PTD25   IGPIOD->Bits.PTD25
#define  IGPIO_PTD26   IGPIOD->Bits.PTD26
#define  IGPIO_PTD27   IGPIOD->Bits.PTD27
#define  IGPIO_PTD28   IGPIOD->Bits.PTD28
#define  IGPIO_PTD29   IGPIOD->Bits.PTD29
#define  IGPIO_PTD30   IGPIOD->Bits.PTD30
#define  IGPIO_PTD31   IGPIOD->Bits.PTD31
#endif /* #ifndef _GPIO_H_ */
