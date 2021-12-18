/**************************文件说明*****************************************************
* Copyright (c) 2020 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：KeyPadLogic.h
* 功能描述： KeyPadLogic头文件
* 
* 编译器： codewarrior IDE 11.1
* MCU说明： 与MCU无关，基本不用更改
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2020年1月14日
*
*/
#ifndef KEYPADLOGIC_H_
#define KEYPADLOGIC_H_

#define   FANVOLT_RATE_MAN   255
#define   FANVOLT_RATE_INIT  20   



#define  IMKEY_FAN       0
#define  IMKEY_TEMP      1

void KeyLogic_Respone(uint8 u8KeyIMDType, int16 i16Data);
void Deal_Keypad_Normal(void);
void PanleState_Logic(void);
#endif /* KEYPADLOGIC_H_ */
