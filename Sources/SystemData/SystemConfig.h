/************************�ļ�˵��*****************************************************
* Copyright (c) 2019,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SystemConfig.h
* ����������ϵͳ�����֣�
* 1 �Ƿ�Ϊ˫����
* 2 ϵͳ�¶Ȼ��㶨��
* 3 �¶�Ĭ��ֵ
* 4 �����¶ȷ�Χ��������Χ
* ���ļ���Ϊģ�壬����ϵͳӦ��Ҫ����ģ���MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��15��
*/

#ifndef SYSTEMCONFIG_H_
#define SYSTEMCONFIG_H_

//ϵͳ�趨
#define   DOUBLE_CLM       0  //�Ƿ�Ϊ˫��������

/*******************************************************************************
 *ϵͳ�¶Ȼ��㶨��
 *
********************************************************************************/
#define  SCD_T(temp)          ((int16)((temp) *10))             //�¶�ת���ķ�ʽ
#define  DCS_T(TempData)      (( (TempData) + (TempData)%10)/10)   // 10λ���¶�ת��Ϊʵ���¶�
#define  SCD_DIFFT(DiffTemp)   ((int16)((DiffTemp) *10))   

/******************************************************************************
 *ϵͳ��ѹ���㶨��
********************************************************************************/
#define  SCD_V(volt)          ((int16)((volt) *100))  

/*******************************************************************************
 * ϵͳ����ʱ��Ĭ���¶�
 *
********************************************************************************/
#define  _TSET_DEFAULT_     SCD_T(24)
#define  _TAMB_DEFAULT_     SCD_T(20)
#define  _TIN_DEFAULT_      SCD_T(25)
#define  _TEVP_DEFAULT_     SCD_T(-2)
#define  _VENT_DEFAULT_     SCD_T(25)
#define  _FOOT_DEFAULT_     SCD_T(25)
#define  _TWATER_DEFAULT_   SCD_T(90)        // ˮ��Ĭ��80
/*******************************************************************************
 * HP SENSOR ��Ĭ��ֵ
 *
********************************************************************************/
#define  _TPRESS_DEFAULT_     SCD_T(25)
#define  _PRESS_DEFAULT_      2000
#define  _TPTC_DEFAULT_       SCD_T(100)
#define  _TOIPTC_DEFAULT_       SCD_T(100)
/*******************************************************************************
 * �����¶ȼ���������
 *
********************************************************************************/
#define  TSET_LO        SCD_T(17)       //���Ϊ17
#define  TSET_HI        SCD_T(32)       //���Ϊ32
#define  TSET_STEP      SCD_T(1)        //��ťÿ���¶ȸı�
//�������¶�ת����CAN�ź�
#define  _TT05(Data)    ((Data)/10 + 14)   //�����¶�ת��
#define  TSET_ZERO      0               //�¶�ֵ0
#define  MAX_FELVL      8               //���絲
#define  MIN_FELVL      1               //��С�絲 ��off

/*******************************************************************************
 * ������
 *
********************************************************************************/
#define BUSCONFIG_RCP_MASK     0X01    //SUV Ĭ��Ϊ1
#define BUSCONFIG_APM_MASK     0x02    
#define BUSCONFIG_ATC_MASK     0x04
#define BUSCONFIG_FHM_MASK     0x08

#define BUSCONFIG_USE_RCP      (!(u8ConfigData[0]&BUSCONFIG_RCP_MASK))    //Ϊ1 Ϊ����
#define BUSCONFIG_USE_APM      (!(u8ConfigData[0]&BUSCONFIG_APM_MASK))
#define BUSCONFIG_USE_ATC      (!(u8ConfigData[0]&BUSCONFIG_ATC_MASK))
#define BUSCONFIG_USE_FHM      (!(u8ConfigData[0]&BUSCONFIG_FHM_MASK))

#endif /* SYSTEMCONFIG_H_ */
