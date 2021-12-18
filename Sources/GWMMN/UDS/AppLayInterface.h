/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：AppLayInterface.h
* 功能描述：UDS诊断的应用层服务程序的接口文件的头文件请按照标准更改。
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 

2019.10.25
ver0.1
1 初始版本
2 模板程序
*/

#ifndef APPLAYDRIVE_H_
#define APPLAYDRIVE_H_

#define    JAC_UDS         0

BOOL CheckKeyBySeed(uint32 nKey, uint32 nSeed, uint8 u8Level);


//故障码号定义
#define   DTC_UDSNUM  18   //UDS 的 故障码个数

static const uint32 TBL_DTCMap[DTC_UDSNUM] = {
		0x96001C, 0x96011C, 0x96021C, 0x96031C, 0x96041C, 0x96061C, 0x96081C, 0xF00317, 
		0xF00316, 0xC14087, 0xC15587, 0xC31587, 0xC16087, 0xC16187, 0xC16287, 0xC16387, 
		0xC16487, 0xC07388
};
#endif /* APPLAYDRIVE_H_ */
