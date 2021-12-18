/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：SPI_Drvie.h
* 功能描述: spi 驱动模块的头文件。需要加入 Galib_MCUComm.h 文件
* 
* 编译器： codewarrior IDE 10.7-KEA
* MCU说明：于MCU无关
*/
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年5月21日
* 
*/
#ifndef SPI_DRIVE__H_
#define SPI_DRIVE__H_

#include "DataType.h"
#include "Galib_MCUComm.h"

#define	SPI_EDGE_RISE  0       //上升沿传数据
#define	SPI_EDGE_FALL  1       //下降沿传数据

//SPI极性
#define	SPI_POLARITY_HIGH  0       //Active-high SPI clock (idles low)
#define	SPI_POLARITY_LOW   1       //Active-low SPI clock (idles high)


uint32 Spi_Drive(uint32 u32Spiwd, uint8 u8SpiLength);
BOOL	Init_Spi_Drive(uint8 edge,uint8 polarity,uint8 lsbf );
#endif
