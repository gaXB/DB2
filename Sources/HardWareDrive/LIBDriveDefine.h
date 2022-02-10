/**************************�ļ�˵��*****************************************************
* Copyright (c) 2021 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�LIBDriveDefine.h
* �����������ײ�����ͷ�ļ����ϣ��Ͷ�LIB�������Ž��ж������Ӧ��ʹ��
* 1  galib
* 2  stepmotor
* 3  MCU_PORT
* 4  DTC
* 5  SYSTEMSAVE
* 6  CAN
* 7  ��ʾ�������
* �������� codewarrior IDE 11.1
* MCU˵�������ļ���MCU �޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��14��
* 
* �汾��2.0
* ˵������������汾�ļ�ֵ
* ԭ���ߣ�xb
* ������ڣ�2020��6��3��
*/
#ifndef LIBDRIVEDEFINE_H_
#define LIBDRIVEDEFINE_H_

#include "Galib.h"
//#include "Stepmotor.h"
//#include "Drv8860_Stepmotor.h"
#include "HardWareDrive.h"
#include "MCU_PORT.h"
#include "MoudleDrive.h"
#include "SystemSaveFlash.h"
#include "GlobalDefine.h"
#include "Deal_DTC.h"
#include "OutANDDisplay.h"
#include "Signal_Refresh.h"
#include "LINEXV.h"
#include "WaterPump.h"
#include "ReadTouchID.h"

#include "SpecialRealy.h"
#include "ActorErr.h"
#if  _PROJECT_CAN_
#include "ApplayLay.h"
#include "IndirectNM.h"
#include "InterLayCan.h"
#include "UDS_APP.h"
#include "printf.h"
#endif

/*******************************************************************************
 *TODO: AD �Ŷ���
 *
********************************************************************************/
#define ADID_VOLT       0     // 01
#define ADID_BATT       1     //�Ͳɼ����  1c
#define ADID_FAN        2     //�ķ��        00
#define ADID_TIN        3      //����         00
#define ADID_TAMB       4     //����           18
#define ADID_TEVP       5     //����           1b
#define ADID_TPTL1      6     //          10
#define ADID_TPTL2      7      //         13
#define ADID_TPTH       8      //         16
#define ADID_SUN        9     //����           1a
#define ADID_PPTL1      10    //ѹ��������   11
#define ADID_PPTL2     11    //           17
#define ADID_PPTH       12    //          12
#define ADID_MOTORMODE   13  //          1F
#define ADID_MOTORNCF    14   //          1E
#define ADID_MOTORMIX    15  //           19
//��ʼ��������
#define ADID_MAXNUM      16
#define ADID_VNUM        3
#define ADID_TNUM        6
#define ADID_SYSIGN      ADVOLT_MODE_12V

#define ADID_FVENT      255
#define ADID_DVENT      255
/*******************************************************************************
 * TODO: ���ģ�����ŷ���
 *
********************************************************************************/
#define  MOTOR_ID_MIX      0
#define  MOTOR_ID_MODE     1
#define  MOTOR_ID_NCF      2

#define  NCFMOTOR_ID_NCF   0
//�������������Ŷ���
/*#define  STEPMOTOR_ID_MODE_OSF     0
#define  STEPMOTOR_ID_MODE_DEF     1
#define	STEPMOTOR_ID_MIX          2
#define	STEPMOTOR_ID_MIX_ASSIANT  3
#define	STEPMOTOR_ID_NCF          4

#define  MODE_OSF_MAXSTEP         1500
#define  MODE_DEF_MAXSTEP         2000
#define	MIX_MAXSTEP              2000
#define	MIX_ASSIANT_MAXSTEP      3000
#define	NCF_MAXSTEP              3000

#define   BACK_STEP               48*/
/*******************************************************************************
 * TODO: 7708ģ�����ŷ���
 *
********************************************************************************/
#define  MOTOR7708_ID_MIX     0
#define  MOTOR7708_ID_MODE    1
#define  MOTOR7708_ID_NCF     2

/*******************************************************************************
 * TODO: ����������
 *
********************************************************************************/
#define  CODE_NUM_TSET       0
#define  CODE_NUM_FAN        1
#define  CODE_NUM_MAX     2


/*******************************************************************************
 *TODO:  �����궨��
 *
********************************************************************************/
#define  KEY_DIAG       0x1800u
#define  KEY_AUTO       0x1000u    
#define  KEY_ONOFF      0x0800u
#define  KEY_NCF       KEY_VER_H   //��λ���̶���ֵΪ0x0002u�����ɸ��ģ�������ֵ������ܿ�  �ĳ�on
#define  KEY_AC      	KEY_VER_L   //��λ���̶���ֵΪ0x0001u�����ɸ��ģ�������ֵ������ܿ�  �ĳ���ѭ��
#define  KEY_OSF    	0x0004u       //�ĳ�AC
#define  KEY_OSFD    0x0020u        //�ĳ�ǰ��˪
#define  KEY_PTC        0x2000u    //�ĳ�ptc
#define  KEY_OSDT     	0x0008u      //�ĳ�OFF
#define  KEY_OST        0x8000u     //�ĳ�ģʽ
#define  KEY_MODE       	0x0010u
#define  KEY_TEMP       0x6000u
#define  KEY_AIRCLEAN   0x0040u
#define  KEY_AIRCLEAN_LEVE 0x0060u
#define  KEY_AIRCLEAN_AUTO_ON 0x0400u
#define  KEY_AIRCLEAN_AUTO    0x0600u
#define  KEY_RHEAT    	0x0100u    //�ĳɺ�
#define  KEY_CHEAT_R    0x0200u
#define  KEY_CHEAT_L    0x0080u
#define  KEY_OFF        0x0080u
#define  KEY_ON         KEY_ONOFF
#define  KEY_DEF        KEY_OST

#define  KEY_VER_SLEF_LCD  		KEY_CODE_COMBFOUR_VERH      //��ס��λ 4����λ ��Ϊ����LCD��ʾ��λֵ
#define  KEY_VER_SLEF           	KEY_CODE_COMBFOUR_VERL      //��ס��λ 4����λ ��ʾ�ڲ��汾�ţ�
#define  KEY_CLIENT_VER        	KEY_VERH_LONG            //��ѭ������5s


#define  COMD_FAN_ADD           2
#define  COMD_FAN_DEC           1 
#define  COMD_TSET_ADD         16
#define  COMD_TSET_DEC         15

#define   KEY_DEBUG      (KEY_PTC+KEY_NCF+0x8000)
/*******************************************************************************
 * �궨������������
********************************************************************************/
#define   CAILBRATION_RAMDATA_NUM    200
#define   CAILBRATION_PARAMETER_NUM    240 

#endif /* LIBDRIVEDEFINE_H_ */


