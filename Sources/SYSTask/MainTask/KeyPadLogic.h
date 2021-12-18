/**************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�KeyPadLogic.h
* ���������� KeyPadLogicͷ�ļ�
* 
* �������� codewarrior IDE 11.1
* MCU˵���� ��MCU�޹أ��������ø���
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
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
