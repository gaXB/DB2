/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SPI_Drvie.h
* ��������: spi ����ģ���ͷ�ļ�����Ҫ���� Galib_MCUComm.h �ļ�
* 
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��5��21��
* 
*/
#ifndef SPI_DRIVE__H_
#define SPI_DRIVE__H_

#include "DataType.h"
#include "Galib_MCUComm.h"

#define	SPI_EDGE_RISE  0       //�����ش�����
#define	SPI_EDGE_FALL  1       //�½��ش�����

//SPI����
#define	SPI_POLARITY_HIGH  0       //Active-high SPI clock (idles low)
#define	SPI_POLARITY_LOW   1       //Active-low SPI clock (idles high)


uint32 Spi_Drive(uint32 u32Spiwd, uint8 u8SpiLength);
BOOL	Init_Spi_Drive(uint8 edge,uint8 polarity,uint8 lsbf );
#endif
