/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SPI_Drive_SoftWare.c
* ����������SPI ���ģ��������������ϵͳ���ʱֻ����1·SPI����
* 1 ��ͨ����ʼ������ ���ԣ����ش������ߵ��ֽڣ�����Ӧ��ͬ������
* 2 �������32��λ��������IC�ķ����֡�
* 3 CS�ŵ��������ϲ�Ӧ�ø���,
* 
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭSPIģ���0.0.3��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��5��21��
* 
* 
*/

#include"SPI_Drive.h"

static uint8 SpiCkPolarity=0;				//���� �� 1 �� 0
static uint8 SpiCkEdge=0;              //���ش����� false�� 0  rise�� 1 
static uint8 LsbFirst=0;               // 1���ȴ���λ  0���ȴ���λ  


//�ײ�������������
extern void SPIHW_ClkWrite(uint8 bSetState);
extern void SPIHW_DataWrite(uint8 bSetState);
extern uint8 SPIHW_DataRead(void);
// ��ģ��ʵ�ֵĺ�����������
static uint32  Up_Set32(uint32 dat, uint8 u8BitNum);

 /*******************************************************************************
 * Function:     BOOL Init_Spi_Drive(uint8 edge��uint8 polarity,uint8 lsbf )
 *
 * Description: ��ʼ��SPI�����ô������أ����ԣ��ȴ��߻��
 *  
 *  Parameter :    
  edge��                         
  SPIEDGE_FALL     �½���
  SPIEDGE_RISE     ������
   
  polarity�� 
  POLARITY_HIGH    0 Active-high SPI clock (idles low)
  POLARITY_LOW    1  Active-low SPI clock (idles high)
  
  lsbf��  
  TRUE     ��λ
  FALSE    ��λ
 *
 * Returns:     ���óɹ�Ϊ1 ������Ϊ0
 *
 * Notes:        ��ʼ��spidrive		
 *
*******************************************************************************/
BOOL Init_Spi_Drive(uint8 edge,uint8 polarity,uint8 lsbf )
{
	BOOL bSpiInit = TRUE;
  //������Ч����
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
 
 /*���ü��� 
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


//�����Ƿ��ȴ���λ    lsbf��  TRUE     ��λ  FALSE    ��λ
	LsbFirst = lsbf;
//����CLK
	SPIHW_ClkWrite(SpiCkPolarity);
	
	return  bSpiInit;
 }
 
 /*******************************************************************************
 * Function:   uint8  upset(uint8 dat)
 *
 * Description: ��1��8λ��D7D6D5D4D3D2D1D0  תλ  D0D1D2D3D4D5D6D7  	
 *
 * Returns:     ���ش����ĵ�����
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
 * Description: SPI���ݴ�������,
 * 
 * parameter �� u32Spiwd �� ������֣����32λ  u8SpiLength�� ����
 *
 * Returns:     ����miso ������
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





