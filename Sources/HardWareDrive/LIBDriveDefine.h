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
#define ADID_VOLT       0
#define ADID_BATT       1     //�Ͳɼ����
#define ADID_FAN        2     //�ķ��
#define ADID_TIN        3      //����
#define ADID_TAMB       4     //����
#define ADID_TEVP       5     //����
#define ADID_FVENT      6
#define ADID_DVENT      7
#define ADID_SUN        8     //����
#define ADID_PRESS      9    //ѹ��������
#define ADID_MOTORMIX    10
#define ADID_MOTORMODE   11
#define ADID_MOTORNCF    12
//��ʼ��������
#define ADID_MAXNUM      13
#define ADID_VNUM        3
#define ADID_TNUM        5
#define ADID_SYSIGN      ADVOLT_MODE_12V



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
#define  KEY_CIRF       KEY_VER_H   //��λ���̶���ֵΪ0x0002u�����ɸ��ģ�������ֵ������ܿ�
#define  KEY_AC      	KEY_VER_L   //��λ���̶���ֵΪ0x0001u�����ɸ��ģ�������ֵ������ܿ�
#define  KEY_FANUP    	0x0004u
#define  KEY_FANDOWN    0x0020u
#define  KEY_FRE        0x2000u    //��ѭ��
#define  KEY_MODE     	0x0008u
#define  KEY_FAN        0x8000u
#define  KEY_DEF       	0x0010u
#define  KEY_TEMP       0x6000u
#define  KEY_AIRCLEAN   0x0040u
#define  KEY_AIRCLEAN_LEVE 0x0060u
#define  KEY_AIRCLEAN_AUTO_ON 0x0400u
#define  KEY_AIRCLEAN_AUTO    0x0600u
#define  KEY_RHEAT    	0x0100u
#define  KEY_CHEAT_R    0x0200u
#define  KEY_CHEAT_L    0x0080u

#define  KEY_VER_SLEF_LCD  		KEY_CODE_COMBFOUR_VERH      //��ס��λ 4����λ ��Ϊ����LCD��ʾ��λֵ
#define  KEY_VER_SLEF           	KEY_CODE_COMBFOUR_VERL      //��ס��λ 4����λ ��ʾ�ڲ��汾�ţ�
#define  KEY_CLIENT_VER        	KEY_VERH_LONG            //��ѭ������5s

#define  KEY_OFF        KEY_ONOFF
#define  COMD_FAN_ADD           2
#define  COMD_FAN_DEC           1 
#define  COMD_TSET_ADD         16
#define  COMD_TSET_DEC         15


/*******************************************************************************
 * �궨������������
********************************************************************************/
#define   CAILBRATION_RAMDATA_NUM    200
#define   CAILBRATION_PARAMETER_NUM    240 

#endif /* LIBDRIVEDEFINE_H_ */


