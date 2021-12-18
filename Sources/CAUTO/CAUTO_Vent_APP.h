/************************�ļ�˵��*****************************************************
* Copyright (c) 2021 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CAUTO_Vent_APP.h
* �������������ļ�Ϊ������㷨�Ķ�Ӧ�궨��CAUTO �Ĳ�������������
* 
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
* ע����ļ�ֻ�ܱ�CAUTO_Vent.c ����
*/
/**************************���ļ�¼******************************************************
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2021��3��8��
* 
*/

#include "CAUTO.h"


//parameter
//���⽥�����
uint8   TDSunAddTimer = 40, TDSunDecTimer = 40;		//  tdsunÿ0.1*data s ����+-1

//���·ֶ� ����Ӧ�Ĳ���ֵ
#define  TD_TAMBSEG_MAXNUM   10
TempType TBL_SEGTAMB[TD_TAMBSEG_MAXNUM] = 
{
		SCD_T(-30),SCD_T(-20), SCD_T(-10), SCD_T(-5), SCD_T(0), 
		SCD_T(5), SCD_T(10), SCD_T(20), SCD_T(30),SCD_T(40),	
};

//Kamb
TdValueType TBL_OFFSETAMB[TD_TAMBSEG_MAXNUM]=
{
	_TDVALUE(-3), _TDVALUE(-2), _TDVALUE(-2), _TDVALUE(-1), _TDVALUE(0), 
	_TDVALUE(0), _TDVALUE(0), _TDVALUE(0), _TDVALUE(1), _TDVALUE(2), 
};

//ktset ��ktin�� kamb ksun
uint16  u16KTset = 25 ,u16KTin = 13, u16KTamb = 6 ,u16KSun = 20;    //������Ҫ/10 

//������sSenSorData.u8ADSunR ֵ�� sTDData.i16TDRsun�� TD��ʽ�л�Ҫ i16TDRsun*u16KSun
#define  TD_SUNSEG_MAXNUM   6
int8 TBL_OFFSETSUN[TD_SUNSEG_MAXNUM]=
{
	50, 40, 30, 15, 5, 0
};

uint8 TBL_ADSUN[TD_SUNSEG_MAXNUM] = 
{
	100, 140, 165, 180, 210, 230
};

//ac ���¿�������
TempType TBL_ACTAMB[2]=
{
SCD_T(-1), SCD_T(1)
}  ;

//���±��� ��ģʽ�Ĳ���
//���´��ų�˪ ���´��ŵ��¶�ֵ
TempType  tLOTamb_ProMode[2] ={SCD_T(-30), SCD_T(-27)};
//ģʽ���ȼ�  ��籣��-���ȷ籣��-LOHI->���±���-���Զ�����

//ģʽTD����
TdValueType TBL_MODETD[4]=
{
	_TDVALUE(-2), _TDVALUE(-1), _TDVALUE(2), _TDVALUE(3)	
};

//ѭ��Td����
TdValueType TBL_NCFTD[2]=
{
	_TDVALUE(5), _TDVALUE(9)	
} ;

//�ķ��tdֵ
TdValueType TBL_FANTD[16]=
{
		_TDVALUE(-30), _TDVALUE(-25), _TDVALUE(-20), _TDVALUE(-12), _TDVALUE(-10), _TDVALUE(-8), _TDVALUE(-5), _TDVALUE(-2), 
		_TDVALUE(2), _TDVALUE(4), _TDVALUE(6), _TDVALUE(8), _TDVALUE(9), _TDVALUE(14), _TDVALUE(16), _TDVALUE(18), 
} ;


//TD��Ӧ��� û�г�����¶ȴ�����ʱʹ��
#define  TD_MIXSEG_MAXNUM   11  
TdValueType  TBL_TEMPMOTOR_TD[TD_MIXSEG_MAXNUM] =     //����λ��
{
		_TDVALUE(-30), _TDVALUE(-25), _TDVALUE(-15), _TDVALUE(-10), _TDVALUE(-5), _TDVALUE(0),
		_TDVALUE(5), _TDVALUE(10), _TDVALUE(15), _TDVALUE(20), _TDVALUE(25)
} ;

int16  TBL_TEMPMOTOR_POSTION[TD_MIXSEG_MAXNUM] = //ʵ��λ��
{
	1000, 900, 800, 700, 600, 500, 400, 300, 200, 100, 0	//
} ;//λ��%

//�ķ����ѹ�������Զ�������ʵ�ʷ�����һ��ʱ����Ч
#if _AFANVOLT_SAME_
#else
AutoFanVoltType  TBL_AFANVOLT[9] = {0 , 390, 490, 590, 680, 790, 890, 1090, 1300};
#endif

//���������Ż�
uint8   u8BV_FRESH = 104;
AutoFanVoltType fVoltBLR_VSMAX = 200;


//����ڲ������
uint8 u8T1Per = 50;   // %
uint8 u8G_TD = 17;   // /10 sSenSorData.Tddt = SCD_T(25) - sTDData.TDResult * u8G_TD /10;


//��籣����������׼�汾����
TempType  Tamb_STHOT = SCD_T(20); 
TempType  Tadt_Lo1 = SCD_T(40), Tadt_Lo2 = SCD_T(25);
AutoFanVoltType fVoltBLR_1A = SCD_V(4);
AutoFanVoltType fVoltBLR_2A = SCD_V(5);
uint8 u8HOTSEQ_1 = 30;
//��������籣������
AutoFanVoltType TBL_CFAN[5] = {380, 640, 860, 1130, 1350};
//�ȷ籣������ֵ
TdValueType TD_STHOT = _TDVALUE(-4);
TempType Tamb_STCOOL = SCD_T(30);
TdValueType TD_STCOOL = _TDVALUE(5); 
uint8 u8COOLSEQ_1 = 20, u8COOLSEQ_2 = 20;
