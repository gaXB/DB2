/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ADTreat.h
* ����������AD ģ���ͷ�ļ���
* Ϊ��ģ��������������Ҫ���� ��Ҫ��ʱ��ģ�鼰��ѧ��������ģ��
* 
* �������� codewarrior IDE 11.1-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��5��14��
* 
* �汾��1.1
* ˵����
* �޸� BOOL ADInit(uint8 u8MaxNum, uint8 u8VoltRegisterType) �����ò�ͬ�ĵ���ֵ����
* 0 : 10/3.6 ��Ϊ 1 : 100/33 
* 100/33 Ϊ1
* ԭ���ߣ�xb
* ������ڣ�2020��3��28��
* 
*/


#ifndef READ_AD__
#define READ_AD__

#include "timerout.h"


//��Ŀ֧��AD����
#define   AD_MAXNUM         25      //AD ģ�����֧��·��
#define   AD_FILTER_NUM     8       //AD �ʲ�buf����
#define   ADTEMP_MAXNUM     8       //���֧�ֵ��¶�ת������
#define   ADVOLT_MAXNUM     3       //���֧�ֵĵ�ѹת������
#define   ADID_TEMP_BEGIN   3       //�¶ȵ���ʼIDֵ
#define   ADTREAT_READTIME   2      //��ȡADģ���ʱ��
#define   ADTREAT_UNCOMPLETE_TIMES   5   //���� ADTREAT_READTIME���޷���ȡ��ָ��AD ʱ���ȴ� �˴�������ΪUNCOMPLETE ����

//READ IGN ONLY ʱ�ĵȴ�ʱ�䣬��2��������Ҫ����MCU�ʵ�����
#define  RIGN_INTCONVER_TIMES    1000     //AD�ж������Ҫ�ȴ���ʱ��
#define  RIGN_WAITAD_TIMES        200     //����ADת����ɵĵȴ�ʱ��

 //ֻ��ȡADʱ����ȡAD�Ĵ���
#define   READ_ADIGN_TIMES        2 

//�¶��źŶ�·�Ͷ�·�ж�˵��
#define    AD_TEMPERR_OPEN     1002  // >= �¶ȶ�·AD 
#define    AD_TEMPERR_SHORT    21    //<= ��·AD 
//��ѹ�����жϷ�Χ
#define    AD_VOLT12_HIGHERR    1610
#define    AD_VOLT12_HIGHOK     1590
#define    AD_VOLT12_LOWERR     890
#define    AD_VOLT12_LOWOK      910

#define    AD_VOLT24_HIGHERR    3210
#define    AD_VOLT24_HIGHOK     3190
#define    AD_VOLT24_LOWERR     1790
#define    AD_VOLT24_LOWOK      1810
/*********************�����ı����ֽ�************************/
//AD ת����״̬
#define  ADCONVER_STATE_INTREAD        0           //�ж�ģʽ     
#define  ADCONVER_STATE_INTOK          1           
#define  ADCONVER_STATE_INIT           2
#define  ADCONVER_STATE_WAITREAD       0x10        //�ȴ�ģʽ
#define  ADCONVER_STATE_WAITOK         0x11

//��ģ�������
#define  ADMODULE_ERRCODE_OK         0x00 
#define  ADMODULE_ERRCODE_INT        0x01      //�󴥷��ж�
#define  ADMODULE_ERRCODE_UNCOMPETE  0x02      //1��������û��ת�����

//��Դģʽ
#define   ADVOLT_MODE_12V       0
#define   ADVOLT_MODE_24V       1
#define   ADVOLT_MODE_INVAILED  2

//ADPHY��������
#define ADPHYERR_TEMP_OK       0
#define ADPHYERR_TEMP_SHORT    1        //�¶ȴ�������·
#define ADPHYERR_TEMP_OPEN     2        //�¶ȴ�������·
#define ADPHYERR_TEMP_INVAILED 0xff     //��Чֵ  

#define ADPHYERR_VOLT_OK       0
#define ADPHYERR_VOLT_HIGH     1         //��ѹ����
#define ADPHYERR_VOLT_LOW      2         //��ѹ����
#define ADPHYERR_VOLT_INVAILED 0xff      //��Чֵ  


typedef int16 ADTempType;        //AD �¶���ʽ�� temp = data/10
typedef int16 ADVoltType;       //AD �ĵ�ѹת��ģʽ�� V =data/100

#define ACD_V(volt)        ((int16)(volt *100))    //ʵ�ʵ�ѹvת��Ϊ ������
#define ACD_T(temp)        ((int16)(temp *10))     //ʵ���¶ȡ�ת��Ϊ ������


typedef struct
{
	uint16 u16ADValue;            
	int16 i16PhyValue;            
}ADTESTDATA;                     //���Է������ݽṹ�嶨��

//AD_ValueGet �õ����������� 
 typedef enum
 {
	 ADVALUE_GETTYPE_8AD = 0,        //8λad
	 ADVALUE_GETTYPE_8ADFAST,        //����8λad
	 ADVALUE_GETTYPE_10AD,           //10λad
	 ADVALUE_GETTYPE_10ADFAST,       //����10λad
	 ADVALUE_GETTYPE_INVAILED,      //���ڵ��ڴ�ֵΪ����
 }ADValueGetType;  
 

 
//�ײ�������
void ADHWEnd_Interrupt(uint16 u16ReadResult);
//�ϲ�ɵ��ú�������
BOOL ADInit(uint8 u8MaxNum, uint8 u8VoltRegisterType);
BOOL ADPhy_Init(uint8 u8VNum, uint8 u8IgnMode, uint8 u8TNum, const uint16** pTBL);
uint16  ADValue_Get(uint8 adid, ADValueGetType u8ADGetType);
int16 ADPhy_GetFact(uint8 adid);
uint8 ADPhy_GetErr(uint8 adid);
uint8 ADModule_ErrCode (void);
void Treat_Read_Ad(void);
ADVoltType ReadIgn_Only(uint8 VoltId);
uint8 ADModule_Test(ADTESTDATA* pTestTable);
#endif
