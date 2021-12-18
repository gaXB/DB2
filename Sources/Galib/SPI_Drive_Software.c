/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SPI_Drive_SoftWare.c
* 功能描述：SPI 软件模拟驱动，在整个系统设计时只能有1路SPI驱动
* 1 可通过初始化设置 极性，边沿触发，高低字节，来适应不同的器件
* 2 打入最多32个位，并返回IC的返回字。
* 3 CS脚的设置由上层应用负责,
* 
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原SPI模块的0.0.3移植
* 原作者：xb
* 完成日期：2019年5月21日
* 
* 
*/

#include"SPI_Drive.h"

static uint8 SpiCkPolarity=0;				//极性 正 1 负 0
static uint8 SpiCkEdge=0;              //边沿触发： false： 0  rise： 1 
static uint8 LsbFirst=0;               // 1：先传低位  0：先传高位  


//底层驱动函数声明
extern void SPIHW_ClkWrite(uint8 bSetState);
extern void SPIHW_DataWrite(uint8 bSetState);
extern uint8 SPIHW_DataRead(void);
// 本模块实现的函数类型声明
static uint32  Up_Set32(uint32 dat, uint8 u8BitNum);

 /*******************************************************************************
 * Function:     BOOL Init_Spi_Drive(uint8 edge，uint8 polarity,uint8 lsbf )
 *
 * Description: 初始化SPI，设置触发边沿，极性，先传高或低
 *  
 *  Parameter :    
  edge：                         
  SPIEDGE_FALL     下降沿
  SPIEDGE_RISE     上升沿
   
  polarity： 
  POLARITY_HIGH    0 Active-high SPI clock (idles low)
  POLARITY_LOW    1  Active-low SPI clock (idles high)
  
  lsbf：  
  TRUE     低位
  FALSE    高位
 *
 * Returns:     设置成功为1 ，否则为0
 *
 * Notes:        初始化spidrive		
 *
*******************************************************************************/
BOOL Init_Spi_Drive(uint8 edge,uint8 polarity,uint8 lsbf )
{
	BOOL bSpiInit = TRUE;
  //设置有效边沿
	switch(edge)
	{
		default: 
			bSpiInit = FALSE;
			break;
		case   SPI_EDGE_FALL:
			SpiCkEdge = 0;
			break;
		case   SPI_EDGE_RISE :
			SpiCkEdge = 1;
			break; 
    }
 
 /*设置极性 
   POLARITY_HIGH    0 Active-high SPI clock (idles low)
  POLARITY_LOW    1  Active-low SPI clock (idles high)*/  
	switch (polarity)
	{
		default: 
			bSpiInit = FALSE;
			break;
		case  SPI_POLARITY_HIGH:
			SpiCkPolarity = 0;
			break;
		case   SPI_POLARITY_LOW:
			SpiCkPolarity = 1;
			break; 
	}


//设置是否先传低位    lsbf：  TRUE     低位  FALSE    高位
	LsbFirst = lsbf;
//设置CLK
	SPIHW_ClkWrite(SpiCkPolarity);
	
	return  bSpiInit;
 }
 
 /*******************************************************************************
 * Function:   uint8  upset(uint8 dat)
 *
 * Description: 将1个8位的D7D6D5D4D3D2D1D0  转位  D0D1D2D3D4D5D6D7  	
 *
 * Returns:     返回此数的倒翻数
 *
 * Notes:       none
 *		
*******************************************************************************/
static uint32  Up_Set32(uint32 dat, uint8 u8BitNum)
{
	uint32 u32UpSetData = 0;
	
	while (u8BitNum--)
	{
		u32UpSetData <<= 1;
		if (dat & 0x01)
		{
			u32UpSetData |= 0x01; 
		}else{}
		
		dat >>= 1;
	}
	
	return u32UpSetData;
}

 /*******************************************************************************
 * Function:    uint32 Spi_Drive(uint32 u32Spiwd, uint8 u8SpiLength)
 *
 * Description: SPI数据串行输入,
 * 
 * parameter ： u32Spiwd ： 打入的字，最多32位  u8SpiLength： 长度
 *
 * Returns:     返回miso 的数据
 *
 * Notes:       none
 *		
*******************************************************************************/
uint32 Spi_Drive(uint32 u32Spiwd, uint8 u8SpiLength)
{
	uint8 spinum = u8SpiLength;
	uint32 u32SpiInData = 0;
	if (LsbFirst == 0) 
	{
		u32Spiwd = Up_Set32(u32Spiwd, u8SpiLength) ;
	}
	else{}
    
	do
	{
		u32SpiInData<<= 1;
		SPIHW_ClkWrite(!SpiCkEdge);
		
		if (u32Spiwd & 0x01)
		{
			SPIHW_DataWrite(1);
		}
		else
		{
			SPIHW_DataWrite(0);
		}	
		MCU_NOP;
		MCU_NOP;
		SPIHW_ClkWrite(SpiCkEdge);
		MCU_NOP;
		MCU_NOP;
		MCU_NOP;
		u32Spiwd >>=1;
		
		if (SPIHW_DataRead() != 0)
		{
			u32SpiInData |= 0x01; 
		}
		else
		{
			
		}
	}while (--spinum);
	
	SPIHW_ClkWrite (SpiCkPolarity);
	
	if (LsbFirst != 0)
	{
		u32SpiInData = Up_Set32(u32SpiInData, u8SpiLength);
	}
	else
	{}
	
	return u32SpiInData;
}





