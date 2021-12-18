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
//MSG����:  GW_VCU_0C9h
//MSGC9 ����
typedef union{ 
 	struct
	{
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :3;	 //Ԥ��_6
		unsigned	PrplsnSysAtv  :1;	 //Power Status: Ready or Not Ready����ǿ�ƣ�Propulsion System Active�� 
		unsigned	Reserved_7  :4;	 //Ԥ��_7
		unsigned	Reserved_8  :8;	 //Ԥ��_8
	}Bits;
	uint8 data[8];
}CANMsg0C9Union;
extern volatile CANMsg0C9Union sCANMsg0C9;
extern volatile uint8 CANMsg0C9Status;
#define TXIFID_0C9   IDIF_TX1
static const uint8  CANMsg0C9Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  BCM_1F1h
//MSG1F1 ����
typedef union{ 
 	struct
	{
		unsigned	SysPwrMd  :2;	 // System Power Mode 
		unsigned	Reserved_0  :2;	 //Ԥ��_0
		unsigned	SysOpnlMd  :3;	 // System Operational Mode��Ԥ����Ĭ�Ϸ�0 
		unsigned	SysVolMdV  :1;	 //  
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	SysVolV  :1;	 //  
		unsigned	Reserved_4  :7;	 //Ԥ��_4
		unsigned	SysVol  :8;	 // Battery Voltage 
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :1;	 //Ԥ��_6
		unsigned	SysVolMd  :2;	 // System Voltage Mode ��9-16V�� 
		unsigned	Reserved_7  :5;	 //Ԥ��_7
	}Bits;
	uint8 data[8];
}CANMsg1F1Union;
extern volatile CANMsg1F1Union sCANMsg1F1;
extern volatile uint8 CANMsg1F1Status;
#define TXIFID_1F1   IDIF_TX2
static const uint8  CANMsg1F1Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//����Ϊ�����ź�

//MSG����:  GW_IPK_1F2h
//MSG1F2 ����
typedef union{ 
 	struct
	{
		unsigned	Reserved_0  :4;	 //Ԥ��_0
		unsigned	SysBPM  :2;	 //System Backup Power Mode  
		unsigned	SysBPMEnbd  :1;	 //System Backup Power Mode Enabled  
		unsigned	Reserved_1  :1;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	Reserved_3  :8;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :8;	 //Ԥ��_6
		unsigned	Reserved_7  :8;	 //Ԥ��_7
		unsigned	Reserved_8  :8;	 //Ԥ��_8
	}Bits;
	uint8 data[8];
}CANMsg1F2Union;
extern volatile CANMsg1F2Union sCANMsg1F2;
extern volatile uint8 CANMsg1F2Status;
#define R_ID_1F2   0x1F2
static const uint8  CANMsg1F2Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//MSG����:  GW_VCU_1F5h
//MSG1F5 ����
typedef union{ 
 	struct
	{
		unsigned	Reserved_0  :8;	 //Ԥ��_0
		unsigned	Reserved_1  :8;	 //Ԥ��_1
		unsigned	Reserved_2  :8;	 //Ԥ��_2
		unsigned	TrShftLvrPos  :4;	 //Transmission Shift Lever Position��Forward Range AΪD��״̬��tbox������ 
		unsigned	TrShftLvrPosV  :1;	 //Transmission Shift Lever Position Validity 
		unsigned	Reserved_3  :3;	 //Ԥ��_3
		unsigned	Reserved_4  :8;	 //Ԥ��_4
		unsigned	Reserved_5  :8;	 //Ԥ��_5
		unsigned	Reserved_6  :8;	 //Ԥ��_6
		unsigned	Reserved_7  :8;	 //Ԥ��_7
	}Bits;
	uint8 data[8];
}CANMsg1F5Union;
extern volatile CANMsg1F5Union sCANMsg1F5;
extern volatile uint8 CANMsg1F5Status;
#define R_ID_1F5   0x1F5
static const uint8  CANMsg1F5Init[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};




//���巢�;����
static const CANMsgTXAttrStruct CANMsgTX_AttrTable[] =
{
	{ TXIFID_0C9,  sCANMsg0C9.data,  10,  &CANMsg0C9Status,  CANMsg0C9Init},
	{ TXIFID_1F1,  sCANMsg1F1.data,  50,  &CANMsg1F1Status,  CANMsg1F1Init},
};
#define   CANMSG_TXATTR    (sizeof(CANMsgTX_AttrTable)/sizeof(CANMsgTXAttrStruct))


//������վ����
static const CANMsgRXAttrStruct CANMsgRX_AttrTable[] =
{
	{ R_ID_1F2,  sCANMsg1F2.data,  500,  &CANMsg1F2Status,  CANMsg1F2Init,  CANMsgUNCTimeout, CANMsgUNCMask},
	{ R_ID_1F5,  sCANMsg1F5.data,  250,  &CANMsg1F5Status,  CANMsg1F5Init,  CANMsgUNCTimeout, CANMsgUNCMask},
};
#define   CANMSG_RXATTR    (sizeof(CANMsgRX_AttrTable)/sizeof(CANMsgRXAttrStruct))

#endif
