/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�AppLayInterface.h
* ����������UDS��ϵ�Ӧ�ò�������Ľӿ��ļ���ͷ�ļ��밴�ձ�׼���ġ�
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 

2019.10.25
ver0.1
1 ��ʼ�汾
2 ģ�����
*/

#ifndef APPLAYDRIVE_H_
#define APPLAYDRIVE_H_

#define    JAC_UDS         0

BOOL CheckKeyBySeed(uint32 nKey, uint32 nSeed, uint8 u8Level);


//������Ŷ���
#define   DTC_UDSNUM  18   //UDS �� ���������

static const uint32 TBL_DTCMap[DTC_UDSNUM] = {
		0x96001C, 0x96011C, 0x96021C, 0x96031C, 0x96041C, 0x96061C, 0x96081C, 0xF00317, 
		0xF00316, 0xC14087, 0xC15587, 0xC31587, 0xC16087, 0xC16187, 0xC16287, 0xC16387, 
		0xC16487, 0xC07388
};
#endif /* APPLAYDRIVE_H_ */
