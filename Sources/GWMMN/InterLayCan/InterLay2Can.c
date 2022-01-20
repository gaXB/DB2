/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�InterLayCAN.c
* ����������InterLay�� ��Ҫ����ʵ�ֺ�������ģ�鲻��Ҫ����
* 1  InterLay������ݷ���
* 2  InterlAY������ݽ��պͳ�ʱ
* 3   busoff
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * ���ļ����ܸ���
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��
* 
* * �汾��1.1
* ˵���������������֡ʱ������ֱ�ӷ��ͣ�ֱ�ӷ������ڵ�ѹ������Ҳ�ᷢ��
* ���ߣ�xb
* ������ڣ�2019��3��11��

* * �汾��2.0
* ˵�������ӷ���֪ͨ��������ѹ�ӿں������µ�LIB
* ���ߣ�xb
* ������ڣ�2019��7��22��
* 
* * * �汾��2.1
* ˵����InterLay_SendNM ��ֱ�����������Բ��ø��ݵ�ѹ����
* ���ߣ�xb
* ������ڣ�2019��7��22��
* 
* �汾��2.2
* ˵����1 Status �Ľ���״ֻ̬���עMSG_LOST��ʹ�ã�����Status������CAN_TX_EN ��ʵ�ַ���һ֡
*      2 ��ϵ�ѹ�ŵ�DTC ģ��
* ���ߣ�xb
* ������ڣ�2019��10��25��
* 
* �汾��2.3
* ˵����1 ����˺��� void InCAN_TXImmediate(uint8 u8IfId) ��������
* 2  ���뵱ǰbusoff״̬��uint8 InGet_BusOffState(void) ����NM�ķ���
* ���ߣ�xb
* ������ڣ�2019��12��24��
* 
* �汾��2.4
* ˵����1 ����CAN_RX_INVALID_ID�궨�� ��������ЧID���������շ�Χ
* 2  ����InCAN_L_Data_Indication(uint32 Identifier, uint8 DLC, uint8 *Data)
*    �����ѯ����ID���ܣ����ڽ��շ�Χ������뻺�棻���� �������ģ�
* 3  memcpy(stdlib.h��׼�⺯��) �滻Ϊ MemCopy(DealString.h��˾���庯��)��  
* ���ߣ�zhupan
* ������ڣ�2021��08��31��
* 
*/
#include "InterLay2Can.h"
#include "InterLayMatrix.h"
#include "CanIf.h"
#include "DataType.h"

#define  CAN_RX_INVALID_ID  0xff  //��ЧID���������շ�Χ

static uint16 TimerTxInder[CAN2MSG_TXATTR], TimerRxInder[CAN2MSG_RXATTR];  //���ͺͽ��ճ�ʱ�Ķ�ʱ��
static uint8  u8SendBuffInder = 0xff;  //û�з��ͳ�ȥ�ı���
static uint8  u8InCanBusOffTimes;
static uint16 TimerBusOff;
static uint8  bBusOffInd;
static uint8  InBusOffState;

#define  _INCAN_DEBUG_TEST_ 1
#if _INCAN_DEBUG_TEST_
volatile uint8 DepthMax;
#endif
/******************************************************************************
CAN���Ľ���FIFO�ṹ
******************************************************************************/
typedef struct
{
  uint32 Identifier;
  uint8  Data[8];
  uint8  DLC;
}CANMsgStruct;

#define  CAN_RX_MSG_FIFO_MAX_DEPTH  10
typedef struct
{
  CANMsgStruct    Msg[CAN_RX_MSG_FIFO_MAX_DEPTH];
  uint8         Iptr;    //��β
  uint8         Optr;    //��ǰ����λ��
  uint8         Depth;   //buff ����
}INCANRxMsgFIFOStruct;
static INCANRxMsgFIFOStruct     sCANRxMsgFIFO;
/******************************************************************************
CANͨ�ſ��ƽṹ
******************************************************************************/
typedef struct
{
  uint8   Mode;      //Ԥ��
  uint8   TxState;  //����״̬
  uint8   RxState;  //����״̬
  uint16  TimerMode;  //��ʼ������ʱʹ��
}INCANCtrlStruct;
static INCANCtrlStruct          sInCANCtrl;

#define  ILCAN_DATA_LENGTH     8      //can ���ݳ���Ĭ��Ϊ8�ֽ�

//CAN DIAG volt MANGE 
#define  CDIAG_VOLT_INIT 	0
#define  CDIAG_VOLT_OK 		1
#define  CDIAG_VOLT_ERR 	2
#define  CDIAG_VOLT_TIMER  3

//�ⲿ��������
uint8 ComControlEnGet(void);
uint8 ComGetTxEn(void);

//�ڲ���������
static uint8 InCAN2_Search_Rx_Msg(uint32 u32Id);
/*******************************************************************************
 * Function:       void InterLayInit(void)
 *
 * Description:    ��ʼ��CAN ������
 *
 * Returns:         none
 *
 * Notes:      1  ������״̬Ϊ����ʹ�ܣ� 
 * 				2    ���ն�ʱ�����������������
 *             3  ���ó�ʼֵ
********************************************************************************/
void InterLay2Init(void)
{
	uint8 i;
   const CANMsgTXAttrStruct* pTAttrTable =  CAN2MsgTX_AttrTable;
   const CANMsgRXAttrStruct* pRAttrTable =  CAN2MsgRX_AttrTable;
	
	sInCANCtrl.RxState = InCAN_RX_ENABLE;
	sInCANCtrl.TxState = InCAN_TX_ENABLE;	
	
			
	for (i = 0; i< CAN2MSG_TXATTR; i++)
	{
		*pTAttrTable->pStatus = CAN_TX_EN;
		(void)TimeOutChkMsLong(&TimerTxInder[i], 0);
		MemCopy((uint8*)pTAttrTable->pMsg, pTAttrTable->pInit, 8);
		pTAttrTable++;
	}
	
	for (i = 0; i< CAN2MSG_RXATTR; i++)
	{
		*pRAttrTable->pStatus = 0;
		(void)TimeOutChkMsLong(&TimerRxInder[i], 0);
		MemCopy((uint8*)pRAttrTable->pMsg, pRAttrTable->pInit, 8);
		pRAttrTable++;
	}
	
	sCANRxMsgFIFO.Depth = 0;
	sCANRxMsgFIFO.Optr = 0;
	sCANRxMsgFIFO.Iptr = 0;
	
	u8InCanBusOffTimes = 0;
	bBusOffInd = 0;
}
/******************************************************************************
��������InCAN_L_Data_Request
��  �ܣ�����·�㴫�ݱ���,��������
��  ����Identifier������ID
        DLC       �����ĳ���
        Data      ����������
����ֵ����
******************************************************************************/
//#pragma INLINE
BOOL InCAN2_L_Data_Request(uint32 Identifier, uint8 *Data)
{
   PduInfoType pduinfo;
   pduinfo.SduDataPtr = Data;
   pduinfo.SduLength = 8;
   
   if (Can2If_Transmit((uint16)Identifier, &pduinfo) == CANIF_RETURN_EOK)
   {//�����CANIF_RETURN_INVALID Ҳֱ���˳�
   	return FALSE;
   }
   else
   {
   	return TRUE;
   }
}

/******************************************************************************
��������InCAN_L_Data_Indication
��  �ܣ�����·�㴫�ݶ���,����ָʾ���ݵĵ��ﲢ��������
        �ú������뱻�²�(��·��)��������
��  ����Identifier ��CAN����ID
        DLC        ���������ݳ���
        Data       ��������������
����ֵ����
******************************************************************************/
void InCAN2_L_Data_Indication(uint32 Identifier, uint8 DLC, uint8 *Data)
{
	uint8 u8RxIdNum = CAN_RX_INVALID_ID;
  //����Ƿ�������ձ��ģ�Ԥ��
//  if (sInCANCtrl.RxState != CoCAN_ENABLE)
//    return;
  
  //���FIFO�Ƿ�����
	if (sCANRxMsgFIFO.Depth >= CAN_RX_MSG_FIFO_MAX_DEPTH)
	{
		//asm("NOP");//������
		return;
	}   
#if	_INCAN_DEBUG_TEST_
	if (sCANRxMsgFIFO.Depth > DepthMax)
	{
		DepthMax = sCANRxMsgFIFO.Depth;
	}
#endif
 
	u8RxIdNum = InCAN2_Search_Rx_Msg(Identifier);
	if(u8RxIdNum < CAN_RX_INVALID_ID)
	{
		//��������  
		sCANRxMsgFIFO.Msg[sCANRxMsgFIFO.Iptr].Identifier = Identifier;
		MemCopy(sCANRxMsgFIFO.Msg[sCANRxMsgFIFO.Iptr].Data, Data, DLC);
		sCANRxMsgFIFO.Msg[sCANRxMsgFIFO.Iptr].DLC        = DLC;
		
		//FIFO����
		sCANRxMsgFIFO.Depth++;
		sCANRxMsgFIFO.Iptr++;
		if (sCANRxMsgFIFO.Iptr >= CAN_RX_MSG_FIFO_MAX_DEPTH)
			sCANRxMsgFIFO.Iptr = 0;
	}
}
/*******************************************************************************
 * Function:        void InterLayCANSend(void)
 *
 * Description:     ���������ݷ��ͣ�����man-loop ��
 *
 * Returns:         none
 *
 * Notes:         ʵ�����ݷ���
 *
********************************************************************************/
static void InterLay2CANSend(void)
{
	uint8 i;
   const CANMsgTXAttrStruct* pTX_AttrTable;
  
   sInCANCtrl.TxState = InCAN_TX_ENABLE;
#if _TX_USENM_
   if (ComGetTxEn())
   {
   	
   }
   else
   {
   	sInCANCtrl.TxState = InCAN_TX_DISABLE;
   }
#endif
   
#if _TX_USEUDS_
   if (ComControlEnGet())
   {
   	
   }
   else
   {
   	sInCANCtrl.TxState = InCAN_TX_DISABLE;
   }
#endif
   
#if _TX_USEVOLT_
   static uint8 MangVolt = 0;
   static uint16 TimerCANVoltErr, TimerCANVoltOK;
   int16 volt = ADPhy_GetFact(ADID_VOLT);
   if(volt>CANTX_VOLT_HIGHERR ||volt<CANTX_VOLT_LOWERR)
   {
      if (TimeOutChkMsLong(&TimerCANVoltErr, 10))
      {
      	MangVolt=1;
      }else{}
      
      (void)TimeOutChkMsLong(&TimerCANVoltOK, 0);
   }
   if(volt < CANTX_VOLT_HIGHOK && volt > CANTX_VOLT_LOWOK)
   {
   	if (TimeOutChkMsLong(&TimerCANVoltOK, 150))
   	{
   		MangVolt=0;
   	}
   	else{}
   	
   	(void)TimeOutChkMsLong(&TimerCANVoltErr, 0);
   }

   if (MangVolt == 1)
   {
   	sInCANCtrl.TxState = InCAN_TX_DISABLE;
   }
   else
   {
   	
   }
#endif
   if (sInCANCtrl.TxState != InCAN_TX_ENABLE)
   {
   //	return ;
   }

   if (u8SendBuffInder < CAN2MSG_TXATTR)
   {//�����ϴ�û�з���ȥ�ı���
   	pTX_AttrTable = &CAN2MsgTX_AttrTable[u8SendBuffInder];
   	(void)APPlIL2_TXStart(pTX_AttrTable->IfID);   //����Ҫ�ж�����ֵ
   	if (InCAN2_L_Data_Request(pTX_AttrTable->IfID, (uint8*)pTX_AttrTable->pMsg))
		{//������ȷ
			APPlIL2_TXComplete(pTX_AttrTable->IfID);		
			*pTX_AttrTable->pStatus |= CAN_UPDATE;
			(void)TimeOutChkMsLong(&TimerTxInder[u8SendBuffInder], 0);
			u8SendBuffInder = 0xff;
		}
		else
		{
			return;
		}
   }
   else
   {	}
   
   pTX_AttrTable = CAN2MsgTX_AttrTable;
	for (i = 0; i < CAN2MSG_TXATTR; i++)
	{
		if (TimeOutChkMsLong(&TimerTxInder[i], pTX_AttrTable->Cycle))
		{
			if (*pTX_AttrTable->pStatus & CAN_TX_EN)
			{
				if (APPlIL2_TXStart(pTX_AttrTable->IfID))
				{//������
					if (InCAN2_L_Data_Request(pTX_AttrTable->IfID, (uint8*)pTX_AttrTable->pMsg))
					{//������ȷ
						APPlIL2_TXComplete(pTX_AttrTable->IfID);	
						*pTX_AttrTable->pStatus |= CAN_UPDATE;
					}
					else
					{
						u8SendBuffInder = i;
						return;
					}
				}
				else{}
			}
		}
		pTX_AttrTable++;
	}
}


/******************************************************************************
��������InCAN_Search_Rx_Msg
��  �ܣ��ӽ��ձ��Ŀ��ٲ��ұ����ҵ�Ŀ�걨��ID����Ϣ�洢λ��
��  ����Identifier��Ŀ�걨��ID
����ֵ���ñ��ĵ���Ϣ�ڽ��ձ��Ŀ��ٲ��ұ��еĴ洢λ��
        0xFF��ʾδ���ҵ�����
******************************************************************************/
static uint8 InCAN2_Search_Rx_Msg(uint32 u32Id)
{
	uint8 i;
   const CANMsgRXAttrStruct* pAttrTable = CAN2MsgRX_AttrTable;
   
   for (i = 0; i< CAN2MSG_RXATTR; i++)
   {
   	if (pAttrTable->ID == u32Id)
   	{
   		return i;
   	}
   	else
   	{
   		pAttrTable++;
   	}
   }
   return 0xff;
}
/*******************************************************************************
 * Function:        void InterLayCANReceive(void)
 *
 * Description:     ���������ݷ��ͣ�����man-loop ��
 *
 * Returns:         none
 *
 * Notes:         ʵ�����ݽ���
 *
********************************************************************************/
static void InterLay2Deal_RecDiag(void)
{
	if (u8InCanBusOffTimes == 0)
	{//busoff
		sInCANCtrl.RxState = InCAN_RX_ENABLE;
	}
	else
	{
		sInCANCtrl.RxState = InCAN_PSEUDO_DISABLE;
	}
}

/*******************************************************************************
 * Function:        void InterLayCANReceive(void)
 *
 * Description:     ���������ݷ��ͣ�����man-loop ��
 *
 * Returns:         none
 *
 * Notes:         ʵ�����ݽ���
 *
********************************************************************************/
static void InterLay2CANReceive(void)
{
  uint8  i;
  uint8  j;
  uint8	Index;
  const CANMsgRXAttrStruct* pRXAttrTable;
  uint32 u32ID;
  uint8  u8RecData[8];
  //�ӱ��Ľ���FIFO����ȡ����
  ///Disable can ����ʱ���ģ�û��Ӱ��
  
	while (sCANRxMsgFIFO.Depth)
	{
		DisableCan2RxInterrupt();
		//�ر��ж�ȡ�������ٽ��У�����
		u32ID = sCANRxMsgFIFO.Msg[sCANRxMsgFIFO.Optr].Identifier;
		MemCopy(u8RecData, sCANRxMsgFIFO.Msg[sCANRxMsgFIFO.Optr].Data, ILCAN_DATA_LENGTH);
		//FIFO����
		sCANRxMsgFIFO.Depth--;
		sCANRxMsgFIFO.Optr++;
		if (sCANRxMsgFIFO.Optr >= CAN_RX_MSG_FIFO_MAX_DEPTH)
		sCANRxMsgFIFO.Optr = 0;
		EnableCan2RxInterrupt();
		
		Index	= InCAN2_Search_Rx_Msg(u32ID);
		if (Index != 0xFF)
		{
			pRXAttrTable =  (&CAN2MsgRX_AttrTable[Index]);
			//������һ�γ�ʱ����
			(void)TimeOutChkMsLong(&TimerRxInder[Index], 0);
			//��������������Buffer
			MemCopy((uint8*)pRXAttrTable->pMsg, u8RecData, ILCAN_DATA_LENGTH);
			//������Ķ�ʧ״̬,�ñ��ĸ��±�־
			*pRXAttrTable->pStatus = CAN_UPDATE;
			//֪ͨӦ�ò���յ�����
			APPlIL2_RXComplete(u32ID);		
		}
		else
		{//����б���û�У�������֪ͨӦ�ò�
			
		}
	}
  
  //����InCAN_PSEUDO_DISABLE����״̬��ʱ�������ڽ��ձ��ļ��,
	InterLay2Deal_RecDiag();
	pRXAttrTable = CAN2MsgRX_AttrTable;
	if (sInCANCtrl.RxState != InCAN_PSEUDO_DISABLE)
	{
		for (i = 0; i < CAN2MSG_RXATTR; i++)
		{
		//������ĵ�����,��������Ĭ��ֵ
			if (TimeOutChkMsLong(&TimerRxInder[i], pRXAttrTable->Cycle)) 
			{
				for (j = 0; j < 8; j++)
				{
					pRXAttrTable->pMsg[j] &= pRXAttrTable->pMask[j];
					pRXAttrTable->pMsg[j] |= pRXAttrTable->pTimeOut[j];
				}

				//�ñ��Ķ�ʧ״̬
				*pRXAttrTable->pStatus = CAN_MSG_LOST;
				 APPlIL2_RecTimeOut(pRXAttrTable->ID);
			}
			pRXAttrTable++;
		}
	}
	else
	{	//�����б��ĵ��߹��ϴ���
		for (i = 0; i < CAN2MSG_RXATTR; i++)
		{
			(void)TimeOutChkMsLong(&TimerRxInder[i], 0);
			//�屨�Ķ�ʧ״̬
			*pRXAttrTable->pStatus &= (~CAN_MSG_LOST);	
			pRXAttrTable++;
		}
	}
}
/*******************************************************************************
 * Function:        void InterLay2Task(void)
 *
 * Description:     ���������񣬷���man-loop ��
 *
 * Returns:         none
 *
 * Notes:         ʵ�����ݽ��գ����ͣ���ʱ ����
 *
********************************************************************************/
void InterLay2Task(void)
{
	uint8 lbBusOffInd = 0;
	DisableCan2RxInterrupt();
	lbBusOffInd = bBusOffInd;
	bBusOffInd = 0;
	EnableCan2RxInterrupt();
	
	if (lbBusOffInd)
	{
		if (u8InCanBusOffTimes < 255)
		{
			u8InCanBusOffTimes++;
		}
		else
		{
			
		}
		(void)TimeOutChkMsLong(&TimerBusOff, 0);
		InBusOffState = 1;
	}
	else
	{
		if (TimeOutChkMsLong(&TimerBusOff, BUSOFF_CLEAR_TIME))
		{
			u8InCanBusOffTimes = 0;
			InBusOffState =0;
		}
		else{}
	}
	InterLay2CANReceive();
	InterLay2CANSend();
}
/*******************************************************************************
 * Function:        void InterLay_SendNM(uint8 u8Inder)	
 *						  
 * Description:     ��ʹ�ýӼ��������ʱ��nmͨ�����ô˺�������������
 *                  u8Inder �����͵�arrt �ı��
 * Returns:         none
 *
 * Notes:          ֻ�ܸ�IndirectNM ����
 *
********************************************************************************/
void InterLay2_SendNM(uint8 u8Inder)	
{
	const CANMsgTXAttrStruct* pTX_AttrTable;
	int16 volt = ADPhy_GetFact(ADID_VOLT);
	
	if(volt < CANTX_VOLT_LOWERR)
	{//��ѹ�Ͳ�׼��
		return;
	}
	else
	{}

		
	if (u8Inder < CAN2MSG_TXATTR)
	{
		pTX_AttrTable = &CAN2MsgTX_AttrTable[u8Inder];
		if (InCAN2_L_Data_Request(pTX_AttrTable->IfID, (uint8*)pTX_AttrTable->pMsg))
		{//������ȷ
			//APPlIL_TXComplete(pTX_AttrTable->IfID);		��֪ͨ
			(void)TimeOutChkMsLong(&TimerTxInder[u8SendBuffInder], 0);
		}
		else
		{
			
		}
	}
	else
	{	
		
	}
	return;
}

/*******************************************************************************
 * Function:        void InCAN_BusOff(void)
 *						  
 * Description:     ֪ͨ��ǰΪbusoff
 *                  
 * Returns:         none
 *
 * Notes:            canif �����
 *
********************************************************************************/
void InCAN2_BusOff(void)
{
	bBusOffInd = 1; 
}
/*******************************************************************************
 * Function:        void InCAN_TxConfirmation(void)
 *						  
 * Description:     CAN ���ݷ������
 *                  
 * Returns:         none
 *
 * Notes:            canif �����
 *
********************************************************************************/
void InCAN2_TxConfirmation(void)
{
	InBusOffState =0;
}

/*******************************************************************************
 * Function:        uint8 InterLay_GetTxMode(void)
 *						  uint8 InterLay_GetRxMode(void)
 *						  
 * Description:     ������ ���շ���״̬
 *
 * Returns:         none
 *
 * Notes:         mode
 *
********************************************************************************/
uint8 InterLay2_GetRxMode(void)
{
	return sInCANCtrl.RxState;
}

uint8 InterLay2_GetTxMode(void)
{
	return sInCANCtrl.TxState;	
}


/*******************************************************************************
 * Function:        void InCAN2_ClearTXBuffer(void)
 *						  
 * Description:     ��շ��ͻ���
 *                  
 * Returns:         none
 *
 * Notes:            canif �����
 *
********************************************************************************/
void InCAN2_ClearTXBuffer(void)
{
	u8SendBuffInder = 0xff;
}
/*******************************************************************************
 * Function:       uint8 InCAN2_GetBusOffDTC(void)
 *						  
 * Description:     ����busoff�Ĺ�����
 *                  
 * Returns:         none
 *
 * Notes:            Ӧ�� �����
 *
********************************************************************************/
uint8 InCAN2_GetBusOffDTC(void)
{
	if (u8InCanBusOffTimes >= BUSOFF_DTC_TIMES)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/*******************************************************************************
 * Function:       void InCAN2_TXImmediate(uint8 u8IfId)
 *						  
 * Description:     ��������ĳ��ID
 *                  
 * Returns:         none
 *
 * Notes:          canif �����
 *
********************************************************************************/
void InCAN2_TXImmediate(uint8 u8IfId)
{
	uint8 i;
   const CANMsgTXAttrStruct* pTX_AttrTable = CAN2MsgTX_AttrTable;
	for (i = 0; i < CAN2MSG_TXATTR; i++)
	{
		if (pTX_AttrTable->IfID == u8IfId)
		{
			u8SendBuffInder = i;
			return;
		}
		pTX_AttrTable++;
	}
}


uint8 InGet2_BusOffState(void)
{
	return InBusOffState;
}
