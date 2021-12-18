/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Deal_DTC.h
* ����������DTC �����ͷ�ļ���ʵ�ִ�ģ����Ҫ���� LIBDriveDefine.h
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
* ����˵���� �밴�չ淶���и���
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��10��26��
* 
* * �汾��2.0
* ˵������Begin_DTCCycle/End_DTCCycle �ɱ�ģ��ʵ�֣��ⲿ����Ҫ����
* ԭ���ߣ�xb
* ������ڣ�2019��10��29��
*/



#ifndef DEAL_DTC_H__
#define DEAL_DTC_H__

#include "LIBDriveDefine.h"

//��Ŀ��Ҫʵ�ֵ�DTC ���壬��Ҫ������Ŀ����
#define  DTCNUM_TIN          0               
#define  DTCNUM_TAMB         1             
#define  DTCNUM_TEVP         2        
#define  DTCNUM_PRESS        3

#define  DTCNUM_MDMOTOR_BACKERR         4   
#define  DTCNUM_MIXMOTOR_BACKERR        5        
#define  DTCNUM_NCFMOTOR_BACKERR        6      
#define  DTCNUM_VOLTAGE_HIGH            8         
#define  DTCNUM_VOLTAGE_LOW             7        
#define  DTCNUM_EMSCAN_NORESPONE        9   
#define  DTCNUM_IPKCAN_NORESPONE        10    
#define  DTCNUM_ESCCAN_NORESPONE        11   
#define  DTCNUM_BCMCAN_NORESPONE        12 
#define  DTCNUM_ICGMCAN_NORESPONE       13   
#define  DTCNUM_FICMCAN_NORESPONE       14 
#define  DTCNUM_TCUCAN_NORESPONE        15 
#define  DTCNUM_LINACSW_NORESPONE       16
#define  DTCNUM_BUSOFF                  17 
#define  DTC_MAXNUM                     18

//dtc �жϹ��ϵĵ�ѹ
#define   DTCVOLT_INIT_DELAY            1500        //��ѹ������ʱ ms
#define   DTCVOLT_RECOVER_DELAY         500        //��ѹ�ӹ��ϵ��ָ���ʱ�� ms
#define   DTC_LVOLT_DISABLE             ACD_V(9)
#define   DTC_LVOLT_ENABLE              ACD_V(9.5)
#define   DTC_HVOLT_DISABLE             ACD_V(16)
#define   DTC_HVOLT_ENABLE              ACD_V(15.5)

//DTCNUM_VOLTAGE_HIGH /LOW �ж�
#define   DTC_EVOLT_ADID                ADID_VOLT       //�жϵ�ѹ�������ADID��Դ 
#define   DTC_EVOLT_LOW_L               ACD_V(5)        //�ڴ˷�Χ�ڵ�ѹ����
#define   DTC_EVOLT_LOW_H               ACD_V(9)
#define   DTC_EVOLT_LOW_OK              ACD_V(9.5)      //L�ָ���ѹ
#define   DTC_EVOLT_HIGH                ACD_V(16)     //�ڴ˷�Χ�ڸ�ѹ����
#define   DTC_EVOLT_HIGH_OK             ACD_V(15.5)       //H�ָ���ѹ
#define   DTC_EVOLT_TIME                1300            //���ϳ���ʱ��Ϊ��ֵ
//dtc snap �ṹ�嶨�壬����Ŀ��������Ҫʱ��Ҫ����
typedef struct
{
	uint8  DTCAgeTimes;  //32bit 
	uint8  DTCPendTimes;
	uint8  u8Volt;   // *0.1
	uint8  IgnState;
	uint8  u8WaterTemp;  // 40
	uint32 u32ODO;    //odometer
}DTCSNAPDATA;
//DTC ��ȡ�������壬 һ�㲻��Ҫ����
#define  DTCDEAL_TIME   8                // dtc�ɼ�ʱ��  *ms
#define  DTC_FAILED_TIMES      30        // ��������
#define  DTC_PASSED_TIMES      -30		  // ��ȷ����
#define  DTC_FAILED_STEP       2         // ÿ�δ���+��ֵ 
#define  DTC_PASSED_STEP       1         // ÿ����ȷ-��ֵ 
#define  DTC_AGE_TIMES         10        //�ϻ�����
#define  _SAVE_DTC_            1         //����dtc �ı���Ҫ�洢ʱ��Ϊ1��Ϊ0����Ҫ�������
#define  SAVE_CYC_TIMES        250       //��SAVE_DTC == 1 ʱ��Ч����ʾ ������ʱ�䲻����SAVE_CYC_TIMES *DTCDEAL_TIME�� �����赽65535
#define  DTC_CONFIRM_TIMES     2         //��ʷ������Ҫѭ���Ĵ���

//���²��ܸ�
//DTC ÿһλ�Ķ���
#define  DTCB_TESTFAILED                   0x01
#define  DTCB_FAILED_THISOC                0x02
#define  DTCB_PENDING                      0x04
#define  DTCB_CONFIRM                      0x08
#define  DTCB_TESTNOTCOMPLETED_SINCECLEAR  0x10
#define  DTCB_TESTFAILED_SINCECLEAR        0x20
#define  DTCB_TESTNOTCOMPLETED             0x40

//GetDTCState ����ֵ����
typedef enum
{
        DTC_RETSTATE_OK = 0,             //�޹���
        DTC_RETSTATE_ERR = 1,            //�й���
        DTC_RETSTATE_WAITE = 2,          //���ϵȴ��������ж�
        DTC_RETSTATE_IMMEDI_OK = 0x80,   //�����޹���
        DTC_RETSTATE_IMMEDI_ERR  = 0x81,  //�����й���
        DTC_RETSTATE_COMFIRM_ERR = 0xC1  //����ȷ��Ϊ��ʷ����
}eDTCRETSTATE;



//�ⲿ�ɵ��ñ���
extern DTCSNAPDATA DTCSnap[DTC_MAXNUM];
extern uint8 DTC[DTC_MAXNUM];
#define DTCTFAILED(DtcNum)    (DTC[DtcNum] & DTCB_TESTFAILED)

//��ģ��ʵ�ֺ���
uint8 Deal_DTC(void);
BOOL Init_DealDTC(uint8 MaxNum);
void ClearDTC(uint8 nId);
//�ⲿӦ�ú���ʵ�ֵĺ���
extern eDTCRETSTATE GetDTCState(uint8 DtcNum);    //�鿴DTC��״̬����Ӧ�ó����ṩ
extern void ReCord_DTCSnap(uint8 num);
#endif
