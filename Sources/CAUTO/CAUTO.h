/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CAUTO.h
* ����������AUTO ģ���ͷ�ļ�
* 
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2021��3��8��
* 
*/
#ifndef CAUTO_H_
#define CAUTO_H_
#include "APPDrive.h"


#define   _AFANVOLT_SAME_    1     //�ķ����ѹ�͵�λ�Ƿ���Զ���һ���� һ����Ϊ1
#define   _CFAN_PROTECT_     1     //3�������ģ�1��������   0������
#define   _HFAN_PROTECT_     1     //1���������   0������
#define   TADT_DIFF_LOHI      SCD_T(15)   //��籣���õ��²�ϵ�������ø�
#define   CFAN_SEG4_PERVOLT   SCD_V(0.2)  //��籣��ʱ���������ٶ�
#define   HMIX_VENTPID_EN      1   //1 ��������¶� 0 �ò��õ���ͬTD��Ӧ�Ļ��ֵ
#define    AUTO_BASE_TSET      SCD_T(24)  //tdֵ����Ļ�׼ֵ



typedef int16 TdValueType;
typedef int16 AutoFanVoltType;
typedef struct
{
	int16 TDResult;
	int16 i16TDRamb;
	int16 i16TDRsun;
	int16 i16TDTin;
	int16 i16TDTset;
}TDDATA;

typedef struct
{
	uint8 AC;
	uint8 NCF;
	uint8 MODE;
	AutoFanVoltType aFanVolt;   //��ѹ��100
	int16 i16Hmix;       //���ǧ�ֱ�
	uint8  CFanState;
	uint8  HFanState;
}AUTORESULT;
 

typedef enum
{
	AUTOTD_TYPE_AC = 0,
	AUTOTD_TYPE_NCF,
	AUTOTD_TYPE_MODE,
	AUTOTD_TYPE_MIX,
	AUTOTD_TYPE_FAN,
}AUTOTYPE;



#define   _TDVALUE(data)   ((int16)((data)*10))


#if _AFANVOLT_SAME_
extern int16  TBL_FANVOLT[];
#define TBL_AFANVOLT  TBL_FANVOLT
#else
#endif



int16 AUTOTD_GetResult(AUTOTYPE u8TDType);
void AUTOTD_Init(void);
void AUTOTD_Control(void);
TDDATA AUTOTD_GetTD(void);
void  Cal_TD(void);
AUTORESULT AUTOTD_GetARes(void);
#endif /* CAUTO_S3_H_ */
