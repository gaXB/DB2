/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ApplayLay.h
* ����������������ͷ�ļ���ʵ�ִ�ģ����Ҫ���� netlay, systemsave,dealdtc
* ���ļ�����Ҫ����
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��

* �汾��1.1
* ˵��������Ԥ����RC
* ԭ���ߣ�xb
* ������ڣ�2019��10��25��

* �汾��1.2
* ˵����
* 1 CAILBRATION_RAMDATA_NUM/CAILBRATION_PARAMETER_NUM �궨������������LIBDriveDefine.h
* 2 DID_MAXNUM ֱ�Ӷ���Ϊ250
* ���ߣ�xb
* ������ڣ�2020��9��27��
*/


#ifndef  __APPLAY_LAY_H__
#define  __APPLAY_LAY_H__
#include  "NetLaye.h"
/*******************************************************************************
 * Ӧ�ò㶨ʱ�궨��
 *
********************************************************************************/

#define  P2CAN_Sever    50
#define  P2CAN_Sever1   500      //��ϵͳ�в���  ����10
#define  S3_Sever       5000

//�����壬���ӹ��ܶ���
#define  SEVER_SID_SESSIONCONTROL      0X10
//�ӹ���
#define  SESSIONCONTROL_TYPE_RESVER    0x00
#define  SESSIONCONTROL_TYPE_DS        0x01
#define  SESSIONCONTROL_TYPE_PRGS      0x02
#define  SESSIONCONTROL_TYPE_EXTDSE    0x03
#define  SESSIONCONTROL_TYPE_EOLDS     0x04
// reset sever
#define  SEVER_SID_RESET      0x11
#define  RESET_TYPE_HARDRESET 0x01
#define  RESET_TYPE_KOFFONR   0x02
#define  RESET_TYPE_SOFTRESET  0x03
//security sever
#define  SEVER_SID_SECURITY   0x27
#define  SECURITY_REQSD_LEV1  0x01
#define  SECURITY_REQSD_LEV2  0x05
#define  SECURITY_SENDK_LEV1  0x02
#define  SECURITY_SENDK_LEV2  0x06
#define  SECURITY_REQSD_LEV3  0x09
#define  SECURITY_SENDK_LEV3  0x0a


#define  SEVER_SID_TP         0X3E     //TEST PRESENT   
#define  SEVER_SID_COMMUNICATIONCONTROL   0x28      //COMM CONTROL
//�ӹ���
#define  CC_TYPE_ERXTX    0x00
#define  CC_TYPE_ERXDTX   0x01
#define  CC_TYPE_DRXETX   0x02
#define  CC_TYPE_DRXTX    0x03

//DTCCONTROL
#define  SEVER_SID_DTCCONTROL       0x85

//�ӹ���
#define  DTCCONTROL_TYPE_STORON     0x01
#define  DTCCONTROL_TYPE_STOROFF    0x02

#define  SEVER_SID_READDATABYINF    0x22
#define  SEVER_SID_WRITEDATABYINF   0x2E
#define  SEVER_SID_IOCONTROL        0x2F

#define  SEVER_SID_READBYADDR       0x23
#define  SEVER_SID_WRITEBYADDR       0x3D

#define  SEVER_SID_CLEARDIAGINF     0x14

#define  SEVER_SID_READDTC    0x19
//�ӹ���
#define  READDTC_SID_NUM      0X01
#define  READDTC_SID_READ     0X02
#define  READDTC_SID_SNAPSHORT  0x04
#define  READDTC_SID_ALLREAD  0x0A


#define  SEVER_SID_RC      0x31   //routine control

#define  RC_SID_START      0x01
#define  RC_SID_STOP       0x02
#define  RC_SID_RESULT     0x03


#define  FUN_NEED_PRECONDITIONS     1
#define  ROUTINE_ID_PRECONDITIONS  0xFF02
#define  ROUTINE_ID_ERASE          0xFF00
#define  ROUTINE_ID_INTEGRITY      0xDFFF
#define  ROUTINE_ID_DEPENDENCE     0xFF01


#define  SEVER_SID_REQDOWNLOAD      0x34
#define  SEVER_SID_TRANSDATA      0x36
#define  SEVER_SID_EXITTRANS      0x37
//���������


//DTC ��ض���
#define  DTC_HIGHTBYTE     0x96      //ACĬ��Ϊ96
#define  DTC_MIDLLE_TYPE   0X00      //00--0a ΪDTC
#define  DTC_LOWBTYE       0x00      //û��  0x00
#define  SUPPORT_DTC       0x4B        //





//����Ӧ��
#define  NRC_SNS        0x11
#define  NRC_SFNS       0x12
#define  NRC_IMLOIF     0x13   //��ʽ����
#define  NRC_CNC        0X22   //�ӹ���Ϊ����ֵ����������������
#define  NRC_ROOR       0x31
#define  NRC_SFNSIAS    0x7e   //subFunctionNotSupportedInActiveSession
#define  NRC_SNSIAS     0x7f   //serviceNotSupportedInActiveSession
#define  NRC_78         0x78


#define  NRC_RSE        0x24   //requestSequenceError
#define  NRC_SAD        0x33   //securityAccessDenied
#define  NRC_IK         0x35   //invalid key
#define  NRC_ENOA       0x36   // exceededNumberOfAttempts
#define  NRC_RTDNE      0x37   // requiredTimeDelayNotExpired
#define  NRC_WBSC       0x73   //wrongBlockSequenceCounter
#define  NRC_GPF        0x72   //generalProgrammingFailure

#ifdef  __NRC22__    //���ڷ�Χ�ڵķ�����22��12 �ĺ궨�� 
#define NRC_CNC1   0x22   
#else
#define NRC_CNC1   0x12
#endif


//����״̬����
#define  APP_STATE_IDLE      0
#define  APP_STATE_RESPONE   1
#define  APP_STATE_DEALREQUEST    2




#define  SESSION_MODE_DEFAULT     0       //default session
#define  SESSION_MODE_PROGRAM     2       //program session
#define  SESSION_MODE_EXTERN      3

#define  SECURITY_STATE_LOCK      0      //LOCK STATE INIT IS this
#define  SECURITY_STATE_SSEED1    1      //����1����״̬
#define  SECURITY_STATE_LEVEL1    2      //�ȼ�1����
#define  SECURITY_STATE_SSEED2    3      //����2����״̬ 
#define  SECURITY_STATE_LEVEL2    4      //�ȼ�2����
#define  SECURITY_STATE_TIMELOCK  5      //����10s ����ܽ���  


#define   APPDATA_MAXLENGTH        255
typedef struct
{
   uint8  A_AI;                        //address idefine
   uint8  A_PCI;                       //pci is sid
   uint8  A_Data[APPDATA_MAXLENGTH];    //?
   uint16  A_Length;                   //lenth a_data's length +1
}A_PDU;


#define   IOCONTROL_STATE_LENGTH  2     //2F sever �����涨״̬�̶ֹ�Ϊ2

/*******************************************************************************
 * DID����������
 *
********************************************************************************/
#define	DID_MAXNUM  250

/**********************************************************************

   ����״̬��ض���  ,  NM_IND(XXX) 
   ************************************************************************/

/***********************************************************************
Ӧ�ò�ɵ��ú���
**********************************************************************/
void Diagnostic_IgnReset(void) ;
void Enter_DefaultSession(void);
void AppLay_DiagControl(void);
BOOL Diag_GetIsDiag(void);
uint8 Diag_GetSession(void);
uint8 NmControlEnGet(void);
/***********************************************************************
��Ҫ�����ⲿ��������
**********************************************************************/
extern void ClearDTC(uint8 nId);
extern void ApplUDS_GotoProgramMode(void);
extern void ApplUDS_Reset(void);	
#endif
