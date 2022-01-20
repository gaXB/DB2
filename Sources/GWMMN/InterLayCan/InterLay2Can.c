/************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：InterLayCAN.c
* 功能描述：InterLay层 主要功能实现函数，此模块不需要更改
* 1  InterLay层的数据发送
* 2  InterlAY层的数据接收和超时
* 3   busoff
* 编译器： codewarrior IDE 11.1
* MCU说明：此模块为上层应用，和MCU 无关
*/
/**************************更改记录******************************************************
 * 此文件不能更改
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日
* 
* * 版本：1.1
* 说明：发送网络管理帧时，不能直接发送，直接发送由在电压不正常也会发送
* 作者：xb
* 完成日期：2019年3月11日

* * 版本：2.0
* 说明：增加发送通知函数，电压接口函数用新的LIB
* 作者：xb
* 完成日期：2019年7月22日
* 
* * * 版本：2.1
* 说明：InterLay_SendNM 在直接网络管理可以不用根据电压发送
* 作者：xb
* 完成日期：2019年7月22日
* 
* 版本：2.2
* 说明：1 Status 的接收状态只需关注MSG_LOST能使用，发送Status仍能用CAN_TX_EN 来实现发送一帧
*      2 诊断电压放到DTC 模块
* 作者：xb
* 完成日期：2019年10月25日
* 
* 版本：2.3
* 说明：1 加入此函数 void InCAN_TXImmediate(uint8 u8IfId) 立即发送
* 2  加入当前busoff状态，uint8 InGet_BusOffState(void) 用于NM的发送
* 作者：xb
* 完成日期：2019年12月24日
* 
* 版本：2.4
* 说明：1 加入CAN_RX_INVALID_ID宏定义 ，代表无效ID，超出接收范围
* 2  函数InCAN_L_Data_Indication(uint32 Identifier, uint8 DLC, uint8 *Data)
*    加入查询接收ID功能，属于接收范围，则放入缓存；否则 丢弃报文；
* 3  memcpy(stdlib.h标准库函数) 替换为 MemCopy(DealString.h我司定义函数)；  
* 作者：zhupan
* 完成日期：2021年08月31日
* 
*/
#include "InterLay2Can.h"
#include "InterLayMatrix.h"
#include "CanIf.h"
#include "DataType.h"

#define  CAN_RX_INVALID_ID  0xff  //无效ID，超出接收范围

static uint16 TimerTxInder[CAN2MSG_TXATTR], TimerRxInder[CAN2MSG_RXATTR];  //发送和接收超时的定时器
static uint8  u8SendBuffInder = 0xff;  //没有发送出去的报文
static uint8  u8InCanBusOffTimes;
static uint16 TimerBusOff;
static uint8  bBusOffInd;
static uint8  InBusOffState;

#define  _INCAN_DEBUG_TEST_ 1
#if _INCAN_DEBUG_TEST_
volatile uint8 DepthMax;
#endif
/******************************************************************************
CAN报文接收FIFO结构
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
  uint8         Iptr;    //结尾
  uint8         Optr;    //当前数据位置
  uint8         Depth;   //buff 数量
}INCANRxMsgFIFOStruct;
static INCANRxMsgFIFOStruct     sCANRxMsgFIFO;
/******************************************************************************
CAN通信控制结构
******************************************************************************/
typedef struct
{
  uint8   Mode;      //预留
  uint8   TxState;  //发送状态
  uint8   RxState;  //接收状态
  uint16  TimerMode;  //初始化发送时使用
}INCANCtrlStruct;
static INCANCtrlStruct          sInCANCtrl;

#define  ILCAN_DATA_LENGTH     8      //can 数据长度默认为8字节

//CAN DIAG volt MANGE 
#define  CDIAG_VOLT_INIT 	0
#define  CDIAG_VOLT_OK 		1
#define  CDIAG_VOLT_ERR 	2
#define  CDIAG_VOLT_TIMER  3

//外部函数声明
uint8 ComControlEnGet(void);
uint8 ComGetTxEn(void);

//内部函数声明
static uint8 InCAN2_Search_Rx_Msg(uint32 u32Id);
/*******************************************************************************
 * Function:       void InterLayInit(void)
 *
 * Description:    初始化CAN 交互层
 *
 * Returns:         none
 *
 * Notes:      1  将发送状态为发送使能， 
 * 				2    接收定时器开启，故障码清除
 *             3  设置初始值
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
函数名：InCAN_L_Data_Request
功  能：向链路层传递报文,并请求发送
参  数：Identifier：报文ID
        DLC       ：报文长度
        Data      ：报文数据
返回值：无
******************************************************************************/
//#pragma INLINE
BOOL InCAN2_L_Data_Request(uint32 Identifier, uint8 *Data)
{
   PduInfoType pduinfo;
   pduinfo.SduDataPtr = Data;
   pduinfo.SduLength = 8;
   
   if (Can2If_Transmit((uint16)Identifier, &pduinfo) == CANIF_RETURN_EOK)
   {//如果是CANIF_RETURN_INVALID 也直接退出
   	return FALSE;
   }
   else
   {
   	return TRUE;
   }
}

/******************************************************************************
函数名：InCAN_L_Data_Indication
功  能：由链路层传递而来,用于指示数据的到达并传递数据
        该函数必须被下层(链路层)函数引用
参  数：Identifier ：CAN报文ID
        DLC        ：报文数据长度
        Data       ：报文数据内容
返回值：无
******************************************************************************/
void InCAN2_L_Data_Indication(uint32 Identifier, uint8 DLC, uint8 *Data)
{
	uint8 u8RxIdNum = CAN_RX_INVALID_ID;
  //检查是否允许接收报文，预留
//  if (sInCANCtrl.RxState != CoCAN_ENABLE)
//    return;
  
  //检查FIFO是否已满
	if (sCANRxMsgFIFO.Depth >= CAN_RX_MSG_FIFO_MAX_DEPTH)
	{
		//asm("NOP");//调试用
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
		//拷贝报文  
		sCANRxMsgFIFO.Msg[sCANRxMsgFIFO.Iptr].Identifier = Identifier;
		MemCopy(sCANRxMsgFIFO.Msg[sCANRxMsgFIFO.Iptr].Data, Data, DLC);
		sCANRxMsgFIFO.Msg[sCANRxMsgFIFO.Iptr].DLC        = DLC;
		
		//FIFO处理
		sCANRxMsgFIFO.Depth++;
		sCANRxMsgFIFO.Iptr++;
		if (sCANRxMsgFIFO.Iptr >= CAN_RX_MSG_FIFO_MAX_DEPTH)
			sCANRxMsgFIFO.Iptr = 0;
	}
}
/*******************************************************************************
 * Function:        void InterLayCANSend(void)
 *
 * Description:     交互层数据发送，放在man-loop 中
 *
 * Returns:         none
 *
 * Notes:         实现数据发送
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
   {//发送上次没有发出去的报文
   	pTX_AttrTable = &CAN2MsgTX_AttrTable[u8SendBuffInder];
   	(void)APPlIL2_TXStart(pTX_AttrTable->IfID);   //不需要判定返回值
   	if (InCAN2_L_Data_Request(pTX_AttrTable->IfID, (uint8*)pTX_AttrTable->pMsg))
		{//发送正确
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
				{//允许发送
					if (InCAN2_L_Data_Request(pTX_AttrTable->IfID, (uint8*)pTX_AttrTable->pMsg))
					{//发送正确
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
函数名：InCAN_Search_Rx_Msg
功  能：从接收报文快速查找表中找到目标报文ID的信息存储位置
参  数：Identifier：目标报文ID
返回值：该报文的信息在接收报文快速查找表中的存储位置
        0xFF表示未能找到报文
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
 * Description:     交互层数据发送，放在man-loop 中
 *
 * Returns:         none
 *
 * Notes:         实现数据接收
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
 * Description:     交互层数据发送，放在man-loop 中
 *
 * Returns:         none
 *
 * Notes:         实现数据接收
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
  //从报文接收FIFO中收取报文
  ///Disable can ，暂时不改，没有影响
  
	while (sCANRxMsgFIFO.Depth)
	{
		DisableCan2RxInterrupt();
		//关闭中断取出数据再进行，处理
		u32ID = sCANRxMsgFIFO.Msg[sCANRxMsgFIFO.Optr].Identifier;
		MemCopy(u8RecData, sCANRxMsgFIFO.Msg[sCANRxMsgFIFO.Optr].Data, ILCAN_DATA_LENGTH);
		//FIFO处理
		sCANRxMsgFIFO.Depth--;
		sCANRxMsgFIFO.Optr++;
		if (sCANRxMsgFIFO.Optr >= CAN_RX_MSG_FIFO_MAX_DEPTH)
		sCANRxMsgFIFO.Optr = 0;
		EnableCan2RxInterrupt();
		
		Index	= InCAN2_Search_Rx_Msg(u32ID);
		if (Index != 0xFF)
		{
			pRXAttrTable =  (&CAN2MsgRX_AttrTable[Index]);
			//重置下一次超时周期
			(void)TimeOutChkMsLong(&TimerRxInder[Index], 0);
			//拷贝报文至解析Buffer
			MemCopy((uint8*)pRXAttrTable->pMsg, u8RecData, ILCAN_DATA_LENGTH);
			//清除报文丢失状态,置报文更新标志
			*pRXAttrTable->pStatus = CAN_UPDATE;
			//通知应用层接收到数据
			APPlIL2_RXComplete(u32ID);		
		}
		else
		{//如果列表里没有，将不会通知应用层
			
		}
	}
  
  //不在InCAN_PSEUDO_DISABLE接收状态下时进行周期接收报文监控,
	InterLay2Deal_RecDiag();
	pRXAttrTable = CAN2MsgRX_AttrTable;
	if (sInCANCtrl.RxState != InCAN_PSEUDO_DISABLE)
	{
		for (i = 0; i < CAN2MSG_RXATTR; i++)
		{
		//如果报文掉线了,拷贝掉线默认值
			if (TimeOutChkMsLong(&TimerRxInder[i], pRXAttrTable->Cycle)) 
			{
				for (j = 0; j < 8; j++)
				{
					pRXAttrTable->pMsg[j] &= pRXAttrTable->pMask[j];
					pRXAttrTable->pMsg[j] |= pRXAttrTable->pTimeOut[j];
				}

				//置报文丢失状态
				*pRXAttrTable->pStatus = CAN_MSG_LOST;
				 APPlIL2_RecTimeOut(pRXAttrTable->ID);
			}
			pRXAttrTable++;
		}
	}
	else
	{	//不进行报文掉线故障处理
		for (i = 0; i < CAN2MSG_RXATTR; i++)
		{
			(void)TimeOutChkMsLong(&TimerRxInder[i], 0);
			//清报文丢失状态
			*pRXAttrTable->pStatus &= (~CAN_MSG_LOST);	
			pRXAttrTable++;
		}
	}
}
/*******************************************************************************
 * Function:        void InterLay2Task(void)
 *
 * Description:     交互层任务，放在man-loop 中
 *
 * Returns:         none
 *
 * Notes:         实现数据接收，发送，超时 任务
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
 * Description:     当使用接间网络管理时，nm通过调用此函数来发送数据
 *                  u8Inder ，发送的arrt 的编号
 * Returns:         none
 *
 * Notes:          只能给IndirectNM 调用
 *
********************************************************************************/
void InterLay2_SendNM(uint8 u8Inder)	
{
	const CANMsgTXAttrStruct* pTX_AttrTable;
	int16 volt = ADPhy_GetFact(ADID_VOLT);
	
	if(volt < CANTX_VOLT_LOWERR)
	{//电压低不准发
		return;
	}
	else
	{}

		
	if (u8Inder < CAN2MSG_TXATTR)
	{
		pTX_AttrTable = &CAN2MsgTX_AttrTable[u8Inder];
		if (InCAN2_L_Data_Request(pTX_AttrTable->IfID, (uint8*)pTX_AttrTable->pMsg))
		{//发送正确
			//APPlIL_TXComplete(pTX_AttrTable->IfID);		不通知
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
 * Description:     通知当前为busoff
 *                  
 * Returns:         none
 *
 * Notes:            canif 层调用
 *
********************************************************************************/
void InCAN2_BusOff(void)
{
	bBusOffInd = 1; 
}
/*******************************************************************************
 * Function:        void InCAN_TxConfirmation(void)
 *						  
 * Description:     CAN 数据发送完成
 *                  
 * Returns:         none
 *
 * Notes:            canif 层调用
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
 * Description:     交互层 接收发送状态
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
 * Description:     清空发送缓存
 *                  
 * Returns:         none
 *
 * Notes:            canif 层调用
 *
********************************************************************************/
void InCAN2_ClearTXBuffer(void)
{
	u8SendBuffInder = 0xff;
}
/*******************************************************************************
 * Function:       uint8 InCAN2_GetBusOffDTC(void)
 *						  
 * Description:     返回busoff的故障码
 *                  
 * Returns:         none
 *
 * Notes:            应用 层调用
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
 * Description:     立即发送某个ID
 *                  
 * Returns:         none
 *
 * Notes:          canif 层调用
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
