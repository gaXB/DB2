/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ExParameter.h
* �������������ļ��ṩ�궨�����������ⲿ������ֻ��MoudleDrive��c ����
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��14��
* 
*/


#ifndef _EXT_PARAMETER_
#define _EXT_PARAMETER_
#include "DataType.h"
#include "CAUTO_Extern.h"
                      
/**************************��Ŀ�����ò���****************************************/
//TODO�� �ڴ˷�����Ҫ�궨�Ĳ�������auto
extern uint8  u8MIXMOTOR_HI_AD, u8MIXMOTOR_LO_AD;   

extern uint8  TBL_MODE_OSET[5]; 
extern uint8 TBL_NCFAD[2];

//��������������
extern TempType   tACON_EVP ,tACOFF_EVP;
extern int16   TBL_FANVOLT[];
#endif
