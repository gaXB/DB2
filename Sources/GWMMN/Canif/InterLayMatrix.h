/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�InterLayMatrix.h
* ����������InterLay�����ݷ��ͣ�Ĭ��ֵ��timeoutֵ�����ͽ��ձ�Ķ��塣�밴�淶��д
* �������� codewarrior IDE 11.1 
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
* 
*���ļ���PC �Զ����ɣ�PC �汾 V1.0, DBC �ļ�����  CVTC_SV51-02&B1_CMX_20200409_BD_V4.81 - HCM.dbc
* �������ڣ�  01/28/21 09:23:21
*/
/**************************���ļ�¼******************************************************
 *
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�
* ������ڣ�
*/
#ifndef _INTERLAY_MATRIX_H_
#define _INTERLAY_MATRIX_H_
#include "timerout.h"
#include "CANIDConfig.h"


//CAN�������ݽṹ��
typedef struct
{
           uint8   IfID;            //if��ID
  volatile uint8   *pMsg;        //��������(���Ĵ洢��ַ)
           uint16  Cycle;        //���ķ�������(��)
  volatile uint8   *pStatus;     //����״̬
  const    uint8   *pInit;       //���ĳ�ʼ��ֵ
}CANMsgTXAttrStruct;

//CAN�������ݽṹ��
typedef struct
{
           uint32  ID;           //����Ϊ����ID
  volatile uint8   *pMsg;        //��������(���Ĵ洢��ַ)
           uint16  Cycle;        //���ĳ�ʱ����(��)
  volatile uint8   *pStatus;     //����״̬
  const    uint8   *pInit;       //���ĳ�ʼ��ֵ
  const    uint8   *pTimeOut;    //���ĵ���ֵ
  const    uint8   *pMask;       //��������λ(����ĳһλ�ڵ���ʱ�Ƿ����Ϊ����ֵ)
}CANMsgRXAttrStruct;

///��ʼ��Ϊ0�����ı��ֵ�����û�ж�������������ΪĬ��ֵ
static const uint8  CANMsgZEROInit[8]    = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8  CANMsgUNCTimeout[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8  CANMsgUNCMask[8]    = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static const uint8  CANMsgZEROMask[8]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//
//
//MSG����:  AC
//MSG12F85351 ����
typedef union{ 
 	struct
	{
		unsigned	INCAR_TEMP  :8;	 //
		unsigned	Reserved_0  :7;	 //Ԥ��_0
		unsigned	C_RRDEF  :1;	 //
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :6;	 //Ԥ��_4
		unsigned	C_FRDEF  :1;	 //
		unsigned	Reserved_5  :1;	 //Ԥ��_5
		unsigned	C_OUTTEMP10BIT_1  :8;	 //<<2
		unsigned	Reserved_6  :6;	 //Ԥ��_6
		unsigned	C_OUTTEMP10BIT_2  :2;	 //<<0
	}Bits;
	uint8 data[8];
}CANMsg12F85351Union;
extern volatile CANMsg12F85351Union sCANMsg12F85351;
extern volatile uint8 CANMsg12F85351Status;
#define TXIFID_12F85351   IDIF_TX1
static const uint8  CANMsg12F85351Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  ACLOAD
//MSG12F98851 ����
typedef union{ 
 	struct
	{
		unsigned	Reserved_0  :4;	 //Ԥ��_0
		unsigned	C_T_FR_BLW  :4;	 //
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :8;	 //Ԥ��_6
		unsigned	Reserved_7  :8;	 //Ԥ��_7
	}Bits;
	uint8 data[8];
}CANMsg12F98851Union;
extern volatile CANMsg12F98851Union sCANMsg12F98851;
extern volatile uint8 CANMsg12F98851Status;
#define TXIFID_12F98851   IDIF_TX2
static const uint8  CANMsg12F98851Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//����Ϊ�����ź�

//MSG����:  AUTOLT_ALT
//MSGEF87372 ����
typedef union{ 
 	struct
	{
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	C_ALTSUN_1  :4;	 //<<6
		unsigned	Reserved_4  :4;	 //Ԥ��_4
		unsigned	Reserved_5  :2;	 //Ԥ��_5
		unsigned	C_ALTSUN_2  :6;	 //<<0
		unsigned	Reserved_6  :8;	 //Ԥ��_6
		unsigned	Reserved_7  :8;	 //Ԥ��_7
	}Bits;
	uint8 data[8];
}CANMsg0EF87372Union;
extern volatile CANMsg0EF87372Union sCANMsg0EF87372;
extern volatile uint8 CANMsg0EF87372Status;
//#define R_ID_0EF87372   0x0EF87372
static const uint8  CANMsg0EF87372Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  MICU_BCM
//MSG12F81018 ����
typedef union{
 	struct
	{
		unsigned	C_UNDER10V  :1;	 //
		unsigned	Reserved_0  :2;	 //Ԥ��_0
		unsigned	C_IG1  :1;	 //
		unsigned	Reserved_1  :4;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :7;	 //Ԥ��_3
		unsigned	C_ACCESSORY  :1;	 //
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :8;	 //Ԥ��_6
		unsigned	Reserved_7  :8;	 //Ԥ��_7
		unsigned	Reserved_8  :8;	 //Ԥ��_8
	}Bits;
	uint8 data[8];
}CANMsg12F81018Union;
extern volatile CANMsg12F81018Union sCANMsg12F81018;
extern volatile uint8 CANMsg12F81018Status;
//#define R_ID_12F81018   0x12F81018
static const uint8  CANMsg12F81018Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  VSPNE_MET
//MSG12F85050 ����
typedef union{ 
 	struct
	{
		unsigned	C_VSP  :8;	 //
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :8;	 //Ԥ��_6
	}Bits;
	uint8 data[8];
}CANMsg12F85050Union;
extern volatile CANMsg12F85050Union sCANMsg12F85050;
extern volatile uint8 CANMsg12F85050Status;
//#define R_ID_12F85050   0x12F85050
static const uint8  CANMsg12F85050Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};




//���巢�;����
static const CANMsgTXAttrStruct CANMsgTX_AttrTable[] =
{
	{ TXIFID_12F85351,  sCANMsg12F85351.data,  300,  &CANMsg12F85351Status,  CANMsg12F85351Init},
	{ TXIFID_12F98851,  sCANMsg12F98851.data,  100,  &CANMsg12F98851Status,  CANMsg12F98851Init},
};
#define   CANMSG_TXATTR    (sizeof(CANMsgTX_AttrTable)/sizeof(CANMsgTXAttrStruct))


//������վ����
static const CANMsgRXAttrStruct CANMsgRX_AttrTable[] =
{
	{ R_ID_0EF87372,  sCANMsg0EF87372.data,  3000,  &CANMsg0EF87372Status,  CANMsg0EF87372Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_12F81018,  sCANMsg12F81018.data,  3000,  &CANMsg12F81018Status,  CANMsg12F81018Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_12F85050,  sCANMsg12F85050.data,  1000,  &CANMsg12F85050Status,  CANMsg12F85050Init,  CANMsgUNCTimeout, CANMsgUNCMask},
};
#define   CANMSG_RXATTR    (sizeof(CANMsgRX_AttrTable)/sizeof(CANMsgRXAttrStruct))





//CAN ���ݶ���
//MSG����:  TMU_410
//MSG410 ����
typedef union{
 	struct
	{
		unsigned	COMPSPDREQ_1  :6;	 //<<8
		unsigned	Reserved_0  :2;	 //Ԥ��_0
		unsigned	COMPSPDREQ_2  :8;	 //<<0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :4;	 //Ԥ��_3
		unsigned	KEYSTART  :4;	 //
		unsigned	VCU_AC_EAC_POWER_LIMIT_1  :8;	 //<<8
		unsigned	VCU_AC_EAC_POWER_LIMIT_2  :8;	 //<<0
		unsigned	Reserved_4  :8;	 //Ԥ��_4
	}Bits;
	uint8 data[8];
}CANMsg410Union;
extern volatile CANMsg410Union sCANMsg410;
extern volatile uint8 CANMsg410Status;
#define TXIFID_410   IDIF_TX1
static const uint8  CANMsg410Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  TMU_44E
//MSG44E ����
typedef union{
 	struct
	{
		unsigned	WPTC_POWER_LIMIT_1  :8;	 //<<8
		unsigned	WPTC_POWER_LIMIT_2  :8;	 //<<0
		unsigned	WPTC_REQ_1  :8;	 //<<8
		unsigned	WPTC_REQ_2  :8;	 //<<0
		unsigned	TWO_WPTC_OUTLET_1  :8;	 //<<8
		unsigned	TWO_WPTC_OUTLET_2  :8;	 //<<0
		unsigned	Reserved_0  :7;	 //Ԥ��_0
		unsigned	WPTC_OPE_PERMIT  :1;	 //
		unsigned	CHECKSUM  :4;	 //
		unsigned	LIVECOUNTER  :2;	 //
		unsigned	Reserved_1  :2;	 //Ԥ��_1
	}Bits;
	uint8 data[8];
}CANMsg44EUnion;
extern volatile CANMsg44EUnion sCANMsg44E;
extern volatile uint8 CANMsg44EStatus;
#define TXIFID_44E   IDIF_TX2
static const uint8  CANMsg44EInit[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  TMU_453
//MSG453 ����
typedef union{
 	struct
	{
		unsigned	AGS_A_ERR_OVERTEMP  :1;	 //
		unsigned	AGS_A_ERR_ELECTRICAL  :1;	 //
		unsigned	AGS_A_ERR_MECHBLOCK  :1;	 //
		unsigned	AGS_A_ERR_MECHBREAK  :1;	 //
		unsigned	AGS_POSITION_FB  :4;	 //
		unsigned	AGS_B_ERR_LINRESP  :1;	 //
		unsigned	AGS_B_ERR_VOLTAGE  :1;	 //
		unsigned	AGS_B_ERR_OVERTEMP  :1;	 //
		unsigned	AGS_B_ERR_ELECTRICAL  :1;	 //
		unsigned	AGS_B_ERR_MECHBLOCK  :1;	 //
		unsigned	AGS_B_ERR_MECHBREAK  :1;	 //
		unsigned	AGS_A_ERR_LINRESP  :1;	 //
		unsigned	AGS_A_ERR_VOLTAGE  :1;	 //
		unsigned	FBEV_FAULTSTATE_1  :2;	 //<<1
		unsigned	EXV_RESPONSEERROR  :1;	 //
		unsigned	EXV_VOLTAGESTATE  :2;	 //
		unsigned	EXV_FAULTSTATE  :3;	 //
		unsigned	TMU_4WAY_VOLTAGEERR_1  :1;	 //<<1
		unsigned	TMU_4WAY_FAULT_SIGNAL  :3;	 //
		unsigned	FBEV_RESPONSEERROR  :1;	 //
		unsigned	FBEV_VOLTAGESTATE  :2;	 //
		unsigned	FBEV_FAULTSTATE_2  :1;	 //<<0
		unsigned	TMU_3WAY1_RSP_COMM_ERR  :1;	 //
		unsigned	TMU_3WAY1_VOLTAGEERR  :2;	 //
		unsigned	TMU_3WAY1_FAULT_SIGNAL  :3;	 //
		unsigned	TMU_4WAY_RSP_COMM_ERR  :1;	 //
		unsigned	TMU_4WAY_VOLTAGEERR_2  :1;	 //<<0
		unsigned	Reserved_0  :2;	 //Ԥ��_0
		unsigned	TMU_3WAY2_RSP_COMM_ERR  :1;	 //
		unsigned	TMU_3WAY2_VOLTAGEERR  :2;	 //
		unsigned	TMU_3WAY2_FAULT_SIGNAL  :3;	 //
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
	}Bits;
	uint8 data[8];
}CANMsg453Union;
extern volatile CANMsg453Union sCANMsg453;
extern volatile uint8 CANMsg453Status;
#define TXIFID_453   IDIF_TX3
static const uint8  CANMsg453Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  TMU_454
//MSG454 ����
typedef union{
 	struct
	{
		unsigned	HVAC_RFD_FB  :8;	 //
		unsigned	HVAC_AM_FB  :8;	 //
		unsigned	EXV_PULSE_NUM_FB_1  :1;	 //<<9
		unsigned	HVAC_MODE_FB  :3;	 //
		unsigned	HVAC_BLW_LV_FB  :4;	 //
		unsigned	EXV_PULSE_NUM_FB_2  :8;	 //<<1
		unsigned	FBEV_PULSE_NUM_FB_1  :7;	 //<<3
		unsigned	EXV_PULSE_NUM_FB_3  :1;	 //<<0
		unsigned	TMU_3WAY2_POSITION_FB_1  :5;	 //<<3
		unsigned	FBEV_PULSE_NUM_FB_2  :3;	 //<<0
		unsigned	TMU_3WAY1_POSITION_FB_1  :5;	 //<<3
		unsigned	TMU_3WAY2_POSITION_FB_2  :3;	 //<<0
		unsigned	Reserved_0  :3;	 //Ԥ��_0
		unsigned	TMU_4WAY_POSITION_FB  :2;	 //
		unsigned	TMU_3WAY1_POSITION_FB_2  :3;	 //<<0
	}Bits;
	uint8 data[8];
}CANMsg454Union;
extern volatile CANMsg454Union sCANMsg454;
extern volatile uint8 CANMsg454Status;
#define TXIFID_454   IDIF_TX4
static const uint8  CANMsg454Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  TMU_455
//MSG455 ����
typedef union{
 	struct
	{
		unsigned	P_OHX_OUTLET_1  :8;	 //<<4
		unsigned	T_OHX_OUTLET_1  :4;	 //<<4
		unsigned	P_OHX_OUTLET_2  :4;	 //<<0
		unsigned	EXV_PULSE_NUMTGT_1  :4;	 //<<6
		unsigned	T_OHX_OUTLET_2  :4;	 //<<0
		unsigned	FBEV_PULSE_NUMTGT_1  :2;	 //<<8
		unsigned	EXV_PULSE_NUMTGT_2  :6;	 //<<0
		unsigned	FBEV_PULSE_NUMTGT_2  :8;	 //<<0
		unsigned	TMU_3WAY1_POSITION_TGT  :8;	 //
		unsigned	TMU_3WAY2_POSITION_TGT  :8;	 //
		unsigned	Reserved_0  :3;	 //Ԥ��_0
		unsigned	AGS_POSITION_TGT  :4;	 //
		unsigned	TMU_4WAY_POSITION_TGT  :1;	 //
	}Bits;
	uint8 data[8];
}CANMsg455Union;
extern volatile CANMsg455Union sCANMsg455;
extern volatile uint8 CANMsg455Status;
#define TXIFID_455   IDIF_TX5
static const uint8  CANMsg455Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  TMU_456
//MSG456 ����
typedef union{
 	struct
	{
		unsigned	DUTY_MOTOR_PUMPTGT  :8;	 //
		unsigned	DUTY_BATT_PUMPTGT  :8;	 //
		unsigned	DUTY_WCPTC_PUMPTGT  :8;	 //
		unsigned	P_EAC_DIS_1  :8;	 //<<4
		unsigned	T_EAC_DIS_1  :4;	 //<<4
		unsigned	P_EAC_DIS_2  :4;	 //<<0
		unsigned	P_EAC_SUC_1  :4;	 //<<8
		unsigned	T_EAC_DIS_2  :4;	 //<<0
		unsigned	P_EAC_SUC_2  :8;	 //<<0
		unsigned	T_EAC_SUC  :8;	 //
	}Bits;
	uint8 data[8];
}CANMsg456Union;
extern volatile CANMsg456Union sCANMsg456;
extern volatile uint8 CANMsg456Status;
#define TXIFID_456   IDIF_TX6
static const uint8  CANMsg456Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  TMU_457
//MSG457 ����
typedef union{
 	struct
	{
		unsigned	HVAC_TEVA  :8;	 //
		unsigned	HVAC_TEO  :8;	 //
		unsigned	TWO_WPTC_OUTLET  :8;	 //
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	AC_ON_OFF  :1;	 //
		unsigned	Reserved_2  :7;	 //Ԥ��_2
		unsigned	Reserved_3  :1;	 //Ԥ��_3
		unsigned	SOV_12_NC_TGT  :1;	 //
		unsigned	SOV_6_NO_TGT  :1;	 //
		unsigned	WPTC_ON_OFF  :1;	 //
		unsigned	RAD_FAN_LV_TGT  :2;	 //
		unsigned	RR_DEF  :1;	 //
		unsigned	FR_DEF  :1;	 //
		unsigned	DR_MODE  :8;	 //
	}Bits;
	uint8 data[8];
}CANMsg457Union;
extern volatile CANMsg457Union sCANMsg457;
extern volatile uint8 CANMsg457Status;
#define TXIFID_457   IDIF_TX7
static const uint8  CANMsg457Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  TMU_551
//MSG551 ����
typedef union{
 	struct
	{
		unsigned	WCPTC_PUMPSTATUS_1  :2;	 //<<1
		unsigned	BATT_PUMPSTATUS  :3;	 //
		unsigned	MOTOR_PUMPSTATUS  :3;	 //
		unsigned	SOV_6_NO_STATUS  :2;	 //
		unsigned	TMU_4WAY_STATUS  :1;	 //
		unsigned	TMU_3WAY1_STATUS  :1;	 //
		unsigned	TMU_3WAY2_STATUS  :1;	 //
		unsigned	FBEV_STATUS  :1;	 //
		unsigned	EXV_STATUS  :1;	 //
		unsigned	WCPTC_PUMPSTATUS_2  :1;	 //<<0
		unsigned	TMU_PUMPENCONFIRMATION  :1;	 //
		unsigned	TMS_ACTUATOR_STATUS  :1;	 //
		unsigned	RAD_FAN_STATUS  :2;	 //
		unsigned	AGS_B_STATUS  :1;	 //
		unsigned	AGS_A_STATUS  :1;	 //
		unsigned	SOV_12_NC_STATUS  :2;	 //
		unsigned	Reserved_0  :6;	 //Ԥ��_0
		unsigned	TMU_HEATCONFIRMATION  :1;	 //
		unsigned	TMU_COOLCONFIRMATION  :1;	 //
		unsigned	TMU_THEMALSYSREQUEST  :1;	 //
		unsigned	Reserved_1  :7;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
	}Bits;
	uint8 data[8];
}CANMsg551Union;
extern volatile CANMsg551Union sCANMsg551;
extern volatile uint8 CANMsg551Status;
#define TXIFID_551   IDIF_TX8
static const uint8  CANMsg551Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//����Ϊ�����ź�

//MSG����:  VCU_115
//MSG115 ����
typedef union{
 	struct
	{
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	SLEEPINDICATIONSIGNAL  :1;	 //
		unsigned	Reserved_6  :7;	 //Ԥ��_6
		unsigned	VCU_CHECKSUM_VCU_3  :4;	 //
		unsigned	VCU_LIVECOUNTER_VCU_3  :2;	 //
		unsigned	Reserved_7  :2;	 //Ԥ��_7
	}Bits;
	uint8 data[8];
}CANMsg115Union;
extern volatile CANMsg115Union sCANMsg115;
extern volatile uint8 CANMsg115Status;
//#define R_ID_115   0x115
static const uint8  CANMsg115Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  MCU_131
//MSG131 ����
typedef union{
 	struct
	{
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	CURRENTMOTORTEMP  :8;	 //
		unsigned	CURRENTIGBTTEMP  :8;	 //
		unsigned	MCU_CHECKSUM_0x131  :4;	 //
		unsigned	MCU_LIVECOUNTER0x131  :2;	 //
		unsigned	Reserved_5  :2;	 //Ԥ��_5
	}Bits;
	uint8 data[8];
}CANMsg131Union;
extern volatile CANMsg131Union sCANMsg131;
extern volatile uint8 CANMsg131Status;
//#define R_ID_131   0x131
static const uint8  CANMsg131Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  VCU_22F
//MSG22F ����
typedef union{
 	struct
	{
		unsigned	VCU_TO_TMU_TMS_POWER_LIMIT_1  :8;	 //<<8
		unsigned	VCU_TO_TMU_TMS_POWER_LIMIT_2  :8;	 //<<0
		unsigned	Reserved_0  :7;	 //Ԥ��_0
		unsigned	VCU_HV_STATE  :1;	 //
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	CHECKSUM  :4;	 //
		unsigned	LIVECOUNTER  :2;	 //
		unsigned	Reserved_5  :2;	 //Ԥ��_5
	}Bits;
	uint8 data[8];
}CANMsg22FUnion;
extern volatile CANMsg22FUnion sCANMsg22F;
extern volatile uint8 CANMsg22FStatus;
//#define R_ID_22F   0x22F
static const uint8  CANMsg22FInit[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  BMS_424
//MSG424 ����
typedef union{
 	struct
	{
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :4;	 //Ԥ��_1
		unsigned	PUMPEN  :1;	 //
		unsigned	BMSHEATREQUEST  :2;	 //
		unsigned	BMSCOOLREQUEST  :1;	 //
		unsigned	AVERAGEBATTEMP  :8;	 //
		unsigned	INLETTARGETTEMP  :8;	 //
		unsigned	INLETCURRENTTEMP  :8;	 //
		unsigned	OUTLETCURRENTTEMP  :8;	 //
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	CHECKSUMBMS4  :4;	 //
		unsigned	LIVECOUNTERBMS4  :2;	 //
		unsigned	Reserved_3  :2;	 //Ԥ��_3
	}Bits;
	uint8 data[8];
}CANMsg424Union;
extern volatile CANMsg424Union sCANMsg424;
extern volatile uint8 CANMsg424Status;
//#define R_ID_424   0x424
static const uint8  CANMsg424Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  WPTC_432
//MSG432 ����
typedef union{
 	struct
	{
		unsigned	Reserved_0  :6;	 //Ԥ��_0
		unsigned	AIRPTC_ENMU_HVHTR_STTR  :2;	 //
		unsigned	AIRPTC_W_HVHTR_POWER  :8;	 //
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	CHECKSUM  :4;	 //
		unsigned	LIVECOUNTER  :2;	 //
		unsigned	Reserved_2  :2;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :8;	 //Ԥ��_6
	}Bits;
	uint8 data[8];
}CANMsg432Union;
extern volatile CANMsg432Union sCANMsg432;
extern volatile uint8 CANMsg432Status;
//#define R_ID_432   0x432
static const uint8  CANMsg432Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  TMU_451
//MSG451 ����
typedef union{
 	struct
	{
		unsigned	TMU_ECOMPR_RPM_1  :2;	 //<<3
		unsigned	TMU_SOV2_OPEN_CLOSE  :1;	 //
		unsigned	TMU_SOV1_OPEN_CLOSE  :1;	 //
		unsigned	TMU_AGS_POSITION  :4;	 //
		unsigned	TMU_WPTC_HTR_OUTPUT_1  :5;	 //<<3
		unsigned	TMU_ECOMPR_RPM_2  :3;	 //<<0
		unsigned	TMU_HVAC_AM_1  :5;	 //<<3
		unsigned	TMU_WPTC_HTR_OUTPUT_2  :3;	 //<<0
		unsigned	TMU_HVAC_RF_1  :5;	 //<<3
		unsigned	TMU_HVAC_AM_2  :3;	 //<<0
		unsigned	TMU_HVAC_MODE_1  :1;	 //<<2
		unsigned	TMU_HVAC_BLW_V  :4;	 //
		unsigned	TMU_HVAC_RF_2  :3;	 //<<0
		unsigned	Reserved_0  :6;	 //Ԥ��_0
		unsigned	TMU_HVAC_MODE_2  :2;	 //<<0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
	}Bits;
	uint8 data[8];
}CANMsg451Union;
extern volatile CANMsg451Union sCANMsg451;
extern volatile uint8 CANMsg451Status;
//#define R_ID_451   0x451
static const uint8  CANMsg451Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  TMU_452
//MSG452 ����
typedef union{
 	struct
	{
		unsigned	TMU_DUTY_MOTOR_PUMP  :8;	 //
		unsigned	TMU_DUTY_BATT_PUMP  :8;	 //
		unsigned	TMU_DUTY_WCPTC_PUMP  :8;	 //
		unsigned	TMU_3WAY1_POSITION  :8;	 //
		unsigned	TMU_3WAY2_POSITION  :8;	 //
		unsigned	TMU_FBEV_PULSE_NUM_1  :5;	 //<<5
		unsigned	TMU_RAD_FAN_LV  :2;	 //
		unsigned	TMU_4WAY_POSITION  :1;	 //
		unsigned	TMU_EXV_PULSE_NUM_1  :3;	 //<<7
		unsigned	TMU_FBEV_PULSE_NUM_2  :5;	 //<<0
		unsigned	Reserved_0  :1;	 //Ԥ��_0
		unsigned	TMU_EXV_PULSE_NUM_2  :7;	 //<<0
	}Bits;
	uint8 data[8];
}CANMsg452Union;
extern volatile CANMsg452Union sCANMsg452;
extern volatile uint8 CANMsg452Status;
//#define R_ID_452   0x452
static const uint8  CANMsg452Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  OBC_460
//MSG460 ����
typedef union{
 	struct
	{
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	OBCTEMP  :8;	 //
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	OBC_CHECKSUM_0x460  :4;	 //
		unsigned	OBC_LIVECOUNTER_0x460  :2;	 //
		unsigned	Reserved_6  :2;	 //Ԥ��_6
	}Bits;
	uint8 data[8];
}CANMsg460Union;
extern volatile CANMsg460Union sCANMsg460;
extern volatile uint8 CANMsg460Status;
//#define R_ID_460   0x460
static const uint8  CANMsg460Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  DCDC_470
//MSG470 ����
typedef union{
 	struct
	{
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	DCDCBODYTEMP  :8;	 //
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	DCDC_CHECKSUM_0x470  :4;	 //
		unsigned	DCDC_LIVECOUNTER_0x470  :2;	 //
		unsigned	Reserved_6  :2;	 //Ԥ��_6
	}Bits;
	uint8 data[8];
}CANMsg470Union;
extern volatile CANMsg470Union sCANMsg470;
extern volatile uint8 CANMsg470Status;
//#define R_ID_470   0x470
static const uint8  CANMsg470Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  DCDC_472
//MSG472 ����
typedef union{
 	struct
	{
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	DCDCINLETTEMP  :8;	 //
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :8;	 //Ԥ��_6
	}Bits;
	uint8 data[8];
}CANMsg472Union;
extern volatile CANMsg472Union sCANMsg472;
extern volatile uint8 CANMsg472Status;
//#define R_ID_472   0x472
static const uint8  CANMsg472Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  EAC_490
//MSG490 ����
typedef union{
 	struct
	{
		unsigned	Reserved_0  :3;	 //Ԥ��_0
		unsigned	COMPSTAT  :4;	 //
		unsigned	Reserved_1  :1;	 //Ԥ��_1
		unsigned	COMPELECPWR  :8;	 //
		unsigned	COMPSPD_1  :6;	 //<<8
		unsigned	Reserved_2  :2;	 //Ԥ��_2
		unsigned	COMPSPD_2  :8;	 //<<0
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :8;	 //Ԥ��_6
	}Bits;
	uint8 data[8];
}CANMsg490Union;
extern volatile CANMsg490Union sCANMsg490;
extern volatile uint8 CANMsg490Status;
//#define R_ID_490   0x490
static const uint8  CANMsg490Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  WPTC_0x26D
//MSG26D ����
typedef union{
 	struct
	{
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :8;	 //
		unsigned	Reserved_2  :8;	 //Ԥ��_1
		unsigned	HVHTR_OUTLET_TEMP  :8;	 // ˮ��

		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :8;	 //Ԥ��_6
	}Bits;
	uint8 data[8];
}CANMsg26DUnion;
extern volatile CANMsg26DUnion sCANMsg26D;
extern volatile uint8 CANMsg26DStatus;
//#define R_ID_26D   0x26D
static const uint8  CANMsg26DInit[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//���巢�;����
static const CANMsgTXAttrStruct CAN2MsgTX_AttrTable[] =
{
	{ TXIFID_410,  sCANMsg410.data,  100,  &CANMsg410Status,  CANMsg410Init},
	{ TXIFID_44E,  sCANMsg44E.data,  40,  &CANMsg44EStatus,  CANMsg44EInit},
	{ TXIFID_453,  sCANMsg453.data,  100,  &CANMsg453Status,  CANMsg453Init},
	{ TXIFID_454,  sCANMsg454.data,  100,  &CANMsg454Status,  CANMsg454Init},
	{ TXIFID_455,  sCANMsg455.data,  100,  &CANMsg455Status,  CANMsg455Init},
	{ TXIFID_456,  sCANMsg456.data,  100,  &CANMsg456Status,  CANMsg456Init},
	{ TXIFID_457,  sCANMsg457.data,  100,  &CANMsg457Status,  CANMsg457Init},
	{ TXIFID_551,  sCANMsg551.data,  100,  &CANMsg551Status,  CANMsg551Init},
};
#define   CAN2MSG_TXATTR    (sizeof(CAN2MsgTX_AttrTable)/sizeof(CANMsgTXAttrStruct))


//������վ����
static const CANMsgRXAttrStruct CAN2MsgRX_AttrTable[] =
{
	{ R_ID_115,  sCANMsg115.data,  100,  &CANMsg115Status,  CANMsg115Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_131,  sCANMsg131.data,  100,  &CANMsg131Status,  CANMsg131Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_22F,  sCANMsg22F.data,  400,  &CANMsg22FStatus,  CANMsg22FInit,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_424,  sCANMsg424.data,  1000,  &CANMsg424Status,  CANMsg424Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_432,  sCANMsg432.data,  3000,  &CANMsg432Status,  CANMsg432Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_451,  sCANMsg451.data,  1000,  &CANMsg451Status,  CANMsg451Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_452,  sCANMsg452.data,  1000,  &CANMsg452Status,  CANMsg452Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_460,  sCANMsg460.data,  1000,  &CANMsg460Status,  CANMsg460Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_470,  sCANMsg470.data,  1000,  &CANMsg470Status,  CANMsg470Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_472,  sCANMsg472.data,  1000,  &CANMsg472Status,  CANMsg472Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_490,  sCANMsg490.data,  1000,  &CANMsg490Status,  CANMsg490Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_26D,  sCANMsg26D.data,  400,  &CANMsg26DStatus,  CANMsg26DInit,  CANMsgUNCTimeout, CANMsgUNCMask},
};
#define   CAN2MSG_RXATTR    (sizeof(CAN2MsgRX_AttrTable)/sizeof(CANMsgRXAttrStruct))


#endif
