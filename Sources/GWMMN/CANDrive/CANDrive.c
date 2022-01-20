/************************文件说明************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：CANDrive.c
* 功能描述：实现CAN 的底层驱动，并通知canif层
* 1  can初始化， 2 发送数据  3 CAN接收  4 BUSOFF 初始化
* 编译器： codewarrior IDE 10.7
* MCU说明：keaz128 can底层驱动
*/
/**************************更改记录******************************************************
* 此文件适用于KEAZ128, 不能被改动
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年1月9日
* 
* 版本：2.0
* 说明：支持扩展帧,及掩码错误问题
* 原作者：xb
* 完成日期：2019年7月22日
* 
* 版本：3.0
* 说明：对是否接收到一个报文的逻辑进行修正以实现快慢恢复
* 原作者：xb
* 完成日期：2019年9月20日
* 
* 
* 版本：4.0
* 说明： 利用初始化来实现报文的停发，需要利用IndirectNM2.0 一上才可以用
* 原作者：xb
* 完成日期：2019年9月20日
* 
* 版本：4.1
* 说明： 增加 BOOL CANDrive_TxBufferEmptemp(void) 判定CAN 是否为空
* 原作者：xb
* 完成日期：2019年12月25日
* 
*/


#include "Config.h"

/* include the following header file to use FlexCAN PD layer driver API and data type */
#include "flexcan_driver.h"
/* include the following header file to use FlexCAN hardware access layer driver API and data type */
#include "flexcan_hw_access.h"
#include "CANDrive.h"

uint32_t CAN_RX_BufferIndex = 0;    /*buffer index for CAN RX*/
uint32_t CAN_RxDataBufferIndex = 0; /*the latest CAN message data buffer index */
bool CAN_RX_Completed_Flag = false; /*flag used for sync between CAN TXRX ISR and application*/
flexcan_msgbuff_t CAN_RX_Message_Buffer[CAN_RX_Buffer_MAX];/*circle buffer for CAN RX*/
flexcan_msgbuff_t CAN_RX_MessageTest;
uint32_t CAN_RxFIFO_RX_BufferIndex = 0;    /*buffer index for CAN RxFIFO RX*/
uint32_t CAN_RxFIFO_RxDataBufferIndex = 0; /*the latest RxFIFO CAN message data buffer index */
bool CAN_RxFIFO_RX_Completed_Flag = false; /*flag used for sync between CAN TXRX ISR and application*/
flexcan_msgbuff_t CAN_RxFIFO_RX_Message_Buffer[CAN_RxFIFO_RX_Buffer_MAX];/*circle buffer for CAN RxFIFO RX*/

bool BusOff_Flag = false; /* the bus off flag */

#define   CAN_STAND_MODE     FLEXCAN_MSG_ID_EXT
/* CAN hardware MB(Message Buffer) configuration */
flexcan_data_info_t CAN_TX_MB_Config = {
	.msg_id_type = CAN_STAND_MODE,
	.data_length = 8,
	.fd_enable = false,
	.fd_padding = 0xCC,
	.enable_brs = false,
	.is_remote = false
};

flexcan_data_info_t CAN_RX_MB_Config = {
	.msg_id_type = CAN_STAND_MODE,
	.data_length = 8,
	.fd_enable = false,
	.fd_padding = 0xCC,
	.enable_brs = false,
	.is_remote = false
};

flexcan_data_info_t CAN_RX1_MB_Config = {
	.msg_id_type = CAN_STAND_MODE,
	.data_length = 8,
	.fd_enable = false,
	.fd_padding = 0xCC,
	.enable_brs = false,
	.is_remote = false
};

flexcan_data_info_t CAN_RX_MB_Config1 = {
	.msg_id_type = CAN_STAND_MODE,
	.data_length = 8,
	.fd_enable = false,
	.fd_padding = 0xCC,
	.enable_brs = false,
	.is_remote = false
};


/* CAN message data buffer to transmit */
uint8_t can_msg_data[8] = {0};

  /*
   * configure the RxFIFO filter acceptance ID element table to receive the follow specified 8 ID messages
   * standard data frame with ID = 0x111,0x222,0x333,0x444,0x555,0x666 and 0x777 and extend ID = 0x888
   *
   * user should assign this table to can_pal component's extension idFilterTable member if initialize configuration structure
   * before call can_pal init() API
   *
   * for example
   *
   * ///@brief User configuration structure
   *  can_user_config_t can_pal1_Config0;
   *
   * in application code:
   * {
   *     ....
   * 		//assign the RxFIFIO filter acceptance ID table to rx_fif0 extension of can_pal config structure
   *      can_pal1_rx_fifo_ext0.idFilterTable = (flexcan_id_table_t *)RxFIFO_FilterAcceptanceID_Table;
   *
   * 		// initialize the CAN module
   * 		CAN_Init(&can_pal1_instance, &can_pal1_Config0);
   * 		....
   * }
   */
  const flexcan_id_table_t RxFIFO_FilterAcceptanceID_Table[16] =
  {
  		{    /*RxFIFO ID table element 0*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = RECEIVE_ID1_1
  		},
  		{	 /*RxFIFO ID table element 1*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = RECEIVE_ID1_2
  		},
  		{    /*RxFIFO ID table element 2*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = RECEIVE_ID2_1
  		},
  		{	 /*RxFIFO ID table element 3*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = RECEIVE_ID2_2
  		},
  		{	 /*RxFIFO ID table element 4*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = RECEIVE_ID3_1
  		},
  		{	 /*RxFIFO ID table element 5*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = RECEIVE_ID3_2
  		},
  		{	 /*RxFIFO ID table element 6*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = RECEIVE_ID4_1
  		},
  		{	 /*RxFIFO ID table element 7*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = RECEIVE_ID4_2
  		},
  		{    /*RxFIFO ID table element 8*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = 0x111
  		},
  		{	 /*RxFIFO ID table element 9*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = 0x222
  		},
  		{    /*RxFIFO ID table element 10*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = 0x333
  		},
  		{	 /*RxFIFO ID table element 11*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = 0x444
  		},
  		{	 /*RxFIFO ID table element 12*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = 0x555
  		},
  		{	 /*RxFIFO ID table element 13*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = 0x666
  		},
  		{	 /*RxFIFO ID table element 14*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = 0x777
  		},
  		{	 /*RxFIFO ID table element 15*/
  			.isRemoteFrame = false,
  			.isExtendedFrame = true,
  			.id = 0x788
  		}
  };

  /* Table of base addresses for CAN instances. */
  static CAN_Type * const g_flexcanBasePtr[] = CAN_BASE_PTRS;

  void FLEXCAN_BOFFREC_Config(uint32_t instance, bool enable);



  /*******************************************************************
   * The CAN TX and RX completed interrupt ISR callback
   *  1) use the new buffer to continue CAN message receive
   *  2) set the RX_Completed_Flag for application handle
   *
   *******************************************************************/
  volatile  int nLastBufferNum, nLastBufferNum_1;
  void CAN_TX_RX_Callback(uint8_t instance, flexcan_event_type_t eventType, uint32_t buffIdx, flexcan_state_t *flexcanState)
  {

  	/*check and handle the TX complete event*/
  	if(FLEXCAN_EVENT_TX_COMPLETE==eventType)
  	{
  		/*add your TX complete event handle codes here if needed*/

  	}

  	/*check and handle the RX complete event*/
  	if(FLEXCAN_EVENT_RX_COMPLETE==eventType)
  	{

		CAN_RX_BufferIndex++; /*increase the RX buffer index*/

		if(CAN_RX_BufferIndex >= CAN_RX_Buffer_MAX)
			CAN_RX_BufferIndex = 0; /* reset the index */

		/*use next circle buffer for the new CAN message receive*/
		FLEXCAN_DRV_Receive(INST_CANCOM1,buffIdx,&CAN_RX_Message_Buffer[CAN_RX_BufferIndex%CAN_RX_Buffer_MAX]);
		CAN_RX_MessageTest = CAN_RX_Message_Buffer[nLastBufferNum];
		nLastBufferNum_1 = nLastBufferNum;
		nLastBufferNum = CAN_RX_BufferIndex%CAN_RX_Buffer_MAX;

		CAN_RX_Completed_Flag = true;/*set the RX completed flag*/

		//开始接收
		CAN_RX_Completed_Flag = false;/* clean the flag */

		/* the new receive data stored in previous RX buffer, get its index */
		CAN_RxDataBufferIndex = (0==CAN_RX_BufferIndex) ? (CAN_RX_Buffer_MAX - 1) : (CAN_RX_BufferIndex - 1);

		CanIf_RxIndication(CAN_RX_Message_Buffer[CAN_RxDataBufferIndex].msgId, CAN_RX_Message_Buffer[CAN_RxDataBufferIndex].data);
  	}

  	/*
  	 * check whether the RxFIFO receive completed
  	 *
  	 * use FLEXCAN_EVENT_DMA_COMPLETE for DMA event and
  	 * use FLEXCAN_EVENT_RXFIFO_COMPLETE for CPU interrupt event
  	 *
  	 */
  	if((FLEXCAN_EVENT_DMA_COMPLETE==eventType)||(FLEXCAN_EVENT_RXFIFO_COMPLETE==eventType))
  	{
  		CAN_RxFIFO_RX_BufferIndex++; /*increase the RxFIFO RX buffer index*/

  		if(CAN_RxFIFO_RX_BufferIndex >= CAN_RxFIFO_RX_Buffer_MAX)
  			CAN_RxFIFO_RX_BufferIndex = 0; /* reset the index */

		/*use next RxFIFO circle buffer for the new CAN message receive*/
  		FLEXCAN_DRV_RxFifo(INST_CANCOM1,&CAN_RxFIFO_RX_Message_Buffer[CAN_RxFIFO_RX_BufferIndex%CAN_RxFIFO_RX_Buffer_MAX]);

		CAN_RxFIFO_RX_Completed_Flag = true;/*set the RxFIFO RX completed flag*/

		//开始接收
		CAN_RxFIFO_RX_Completed_Flag = false;/* clean the flag */

		/* the new receive data stored in previous RX buffer, get its index */
		CAN_RxFIFO_RxDataBufferIndex = (0==CAN_RxFIFO_RX_BufferIndex) ? (CAN_RxFIFO_RX_Buffer_MAX - 1 ) : (CAN_RxFIFO_RX_BufferIndex - 1);
		CanIf_RxIndication(CAN_RxFIFO_RX_Message_Buffer[CAN_RxFIFO_RxDataBufferIndex].msgId, CAN_RxFIFO_RX_Message_Buffer[CAN_RxFIFO_RxDataBufferIndex].data);
  	}
  }
  /*************************************************************************************
   * the CAN error ISR callback
   * user can add handle code for :
   *
   * FlexCAN Error, Error Fast, Tx Warning, Rx Warning and Bus Off as well as Bus Off Done event
   *
   * Note: Here only add the bus-off recovery code for demonstration
   *************************************************************************************/
  void CAN_Error_Callback(uint8_t instance, flexcan_event_type_t eventType,
                                           flexcan_state_t *flexcanState)
  {
  	/* temp variable to store ESR1 register value */
  	uint32_t temp_ESR1_RegVal = 0;

  	DEV_ASSERT(instance < CAN_INSTANCE_COUNT);

  	CAN_Type * base = g_flexcanBasePtr[instance];

  	if(FLEXCAN_EVENT_ERROR==eventType)
  	{
  		/*get the error flags for further process */
  		temp_ESR1_RegVal = FLEXCAN_DRV_GetErrorStatus(instance);


  		if(CAN_ESR1_BOFFINT_MASK&temp_ESR1_RegVal)
  		{
  			/* record the bus off event */
  			BusOff_Flag = true;

  #ifdef CAN_BUSOFF_RECOVERY_MANUAL
  			/* abort current MB transmit */
  			FLEXCAN_DRV_AbortTransfer(INST_CANCOM1,TX_MB);
  #endif  /* end of CAN_BUSOFF_RECOVERY_MANUAL */

  			/* use can add their network management handle codes here  */
  			PINS_DRV_ClearPins(RGB_LED_GPIO,(1<<RGB_GREEN_LED_PIN)); /*turn ON green RGB LED*/

  			/*get the error flags again in case bus-off recovery have done */
  			temp_ESR1_RegVal = FLEXCAN_DRV_GetErrorStatus(instance);
  		}

  		if(CAN_ESR1_BOFFDONEINT_MASK&temp_ESR1_RegVal)
  		{

  #ifdef CAN_BUSOFF_RECOVERY_MANUAL
			/* the bus off recovery condition(the Tx Error Counter (TXERRCNT) has finished counting 128 occurrences of 11 consecutive recessive bits on the CAN bus and is ready to leave Bus Off) has been detected*/
			/*
			 * clean BOFFREC bit to trigger bus off recovery
			 */
			FLEXCAN_BOFFREC_Config(instance, false);
  #endif  /* end of CAN_BUSOFF_RECOVERY_MANUAL */

			/* wait for the FlexCAN has actually exited bus-off  */
  			while((base->ESR1&CAN_ESR1_FLTCONF_MASK)>=CAN_ESR1_FLTCONF(0x2));

  			PINS_DRV_SetPins(RGB_LED_GPIO,(1<<RGB_GREEN_LED_PIN)); /*turn OFF green RGB LED*/

  			BusOff_Flag = false; /* clean the flag */

  #ifdef CAN_BUSOFF_RECOVERY_MANUAL
  			/* disable automatic bus-off recovery again */
  			FLEXCAN_BOFFREC_Config(instance, true);
  #endif  /* end of CAN_BUSOFF_RECOVERY_MANUAL */

  		}

  	}
  }
  /*************************************************************
   * API to configure the FlexCAN bus off recovery method
   *
   * enable = true:  disable automatic recovering from Bus Off state
   * enable = false: enable automatic recovering from Bus Off state
   *                 it is also a MUST for user recovery
   *
   *************************************************************/
  void FLEXCAN_BOFFREC_Config(uint32_t instance, bool enable)
  {
  	DEV_ASSERT(instance < CAN_INSTANCE_COUNT);

  	CAN_Type * base = g_flexcanBasePtr[instance];

  	FLEXCAN_EnterFreezeMode(base);

  	if(true==enable)
  	{
  		/* set BOFFREC bit to disable automatic bus-off recovery */
  		base->CTRL1 |= CAN_CTRL1_BOFFREC(enable);

  		/* enable buss off done interrupt */
  		base->CTRL2 |= CAN_CTRL2_BOFFDONEMSK(enable);
  	}
  	else
  	{
  		/* clean BOFFREC bit to enable automatic bus-off recovery */
  		base->CTRL1 &= (uint32_t)(~CAN_CTRL1_BOFFREC_MASK);

  		/* clean the bus-off done interrupt flag */
  		base->ESR1 = CAN_ESR1_BOFFDONEINT_MASK;
  	}

  	FLEXCAN_ExitFreezeMode(base);
  }

  /*FUNCTION**********************************************************************
 *
 * Function Name : CAN_MB_Config_Update
 * Description   : This function update code and status configuration of
 *                 the specified FlexCAN module TX MB
 *  config  : the FlexCAN MB configuration to be updated
 *  cs      : code and status input
 *
 * Implements    : CAN_MB_Config_Update_Activity
 *END**************************************************************************/
void CAN_MB_Config_Update(flexcan_data_info_t *config, uint32_t cs)
{
	/* get the CAN-FD enable configuration */
	config->fd_enable = (cs & CAN_MB_EDL_MASK)? true: false;

	/* get the bitrate switch enable configuration */
	config->enable_brs = (cs & CAN_MB_BRS_MASK)? true: false;

	/* get the ID type configuration */
	config->msg_id_type = (cs & CAN_CS_IDE_MASK)? FLEXCAN_MSG_ID_EXT: FLEXCAN_MSG_ID_STD;
	config->msg_id_type = FLEXCAN_MSG_ID_EXT;
	/* get the remote/data frame configuration */
	config->is_remote = (cs & CAN_CS_RTR_MASK)?true: false;
}


#define  MAX_LEN    8

/*******************************************************************************
 * Function:       uint8 CanTxComplete(void)
 *
 * Description:     MSCAN Send Message Complete?
 *
 * Returns:         yes 1, no 0
 *
 * Notes:           uds used		
 *
********************************************************************************/
uint8 CanTxComplete(void)
{
	return 1;
}

/*******************************************************************************
 * Function:        MscanTxdMsg
 *
 * Description:     CAN数据发送
 *
 * Returns:         none
 *
 * Notes:           本模块静态函数		
 *
********************************************************************************/
//static BOOL MscanTxdMsg(MSCAN_FramePtr pCANTxInfo)
//{
//	return TRUE;
//}


/*******************************************************************************
 * Function:        BOOL CANDrive_SendFram(uint32 id, uint8* u8SendData)
 *
 * Description:     CAN数据发送
 *
 * Returns:         none
 *
 * Notes:           id: id    	*u8SendData:	发送的8字节数据指针
 *
********************************************************************************/
BOOL CANDrive_SendFram(uint32 id, uint8* u8SendData)
{
	//flexcan_data_info_t sTXConfig;

	CAN_MB_Config_Update(&CAN_TX_MB_Config, CAN_RxFIFO_RX_Message_Buffer[CAN_RxFIFO_RxDataBufferIndex].cs);

	/*get the data length */
	CAN_TX_MB_Config.data_length = 8;

	/* send it back via CAN */
	FLEXCAN_DRV_SendBlocking(INST_CANCOM1,TX_MB, &CAN_TX_MB_Config,\
			id,\
			u8SendData, \
			2);

	//extern BOOL CAN2Drive_SendFram(uint32 id, uint8* u8SendData);
	//CAN2Drive_SendFram(id, u8SendData);
	return TRUE;
}

//BOOL CANDrive_SendFram1(uint32 id, uint8* u8SendData)
//{
//	//flexcan_data_info_t sTXConfig;
//
//	CAN_MB_Config_Update(&CAN_TX_MB_Config, CAN_RxFIFO_RX_Message_Buffer[CAN_RxFIFO_RxDataBufferIndex].cs);
//
//	/*get the data length */
//	CAN_TX_MB_Config.data_length = 8;
//
//	/* send it back via CAN */
/*	FLEXCAN_DRV_SendBlocking(INST_CANCOM1,TX_MB, &CAN_TX_MB_Config,\
			id,\
			u8SendData, \
			2);

	return TRUE;
}
*/
/*******************************************************************************
 * Function:      CANDriver_ClearTransmitBuffer
 *
 * Description:     CAN 清除硬件缓存
 *
 * Returns:         none
 *
 * Notes:        本模块静态函数
 *
********************************************************************************/
void CANDriver_ClearTransmitBuffer(void)
{

}

/*******************************************************************************
 * Function:       void CANDriver_BusOffInit(void)
 *
 * Description:     CAN 初始化请求
 *
 * Returns:         none
 *
 * Notes:          暂无用
 *
********************************************************************************/
void CANDriver_BusOffInit(void)
{
	Init_Mscan();
}

/*******************************************************************************
 * Function:       void CANDrive_Close(void)
 *
 * Description:     CAN 关闭请求
 *
 * Returns:         none
 *
 * Notes:          暂无用
 *
********************************************************************************/
void CANDrive_Close(void)
{
	CANDriver_ClearTransmitBuffer();
	//(void)CAN_DeInit(MSCAN); //此句运行后CAN 关中断等操作将不能用
//	SIM->PINSEL1 |= SIM_PINSEL1_MSCANPS_MASK;此模式可以少一个管教，但是另一个CANTX。CANRX 也无效不建议
}
/*******************************************************************************
 * Function:        Init_Mscan
 *
 * Description:     CAN模块初始化，CAN模块总线错误处理，CAN数据发送，CAN数据接收
 *
 * Returns:         none
 *
 * Notes:           此示例为实现	
 *                 1 20mhz总线时钟时实现500k的波特率
 *                 2 滤波器只接受 (uint16)RECEIVE_ID1` 4
 *
********************************************************************************/
/*FUNCTION**********************************************************************
 *
 * Function Name : CAN_User_Init
 * Description   : This function complete the CAN initialization
 *
 * Notes         : Please add the CAN initialization codes per requirements
 *
 * Implements    : CAN_User_Init_Activity
 *END**************************************************************************/
void CAN_User_Init(void)
{
	uint32_t i = 0;

    /* initialize eDMA for CAN RxFIFO transfer */
	EDMA_DRV_Init(&dmaController1_State, &dmaController1_InitConfig0, edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);

    /* initialize the FlexCAN PD driver */
    FLEXCAN_DRV_Init(INST_CANCOM1, &canCom1_State, &canCom1_InitConfig0);

    /* install the error handler call back */
    FLEXCAN_DRV_InstallEventCallback(INST_CANCOM1,CAN_TX_RX_Callback, NULL);

 #ifdef CAN_BUSOFF_RECOVERY_MANUAL
    /* disable bus off automatic recovery */
    FLEXCAN_BOFFREC_Config(INST_CANCOM1, true);
 #endif

    /* install the FlexCAN error ISR callback, which will also enable error, TX warning, RX warning and bus-off interrupt */
    FLEXCAN_DRV_InstallErrorCallback(INST_CANCOM1, CAN_Error_Callback,NULL);

    /* configure the TX MB */
    FLEXCAN_DRV_ConfigTxMb(INST_CANCOM1,TX_MB, &CAN_TX_MB_Config,TX_MSG_ID);

    /* configure the FlexCAN RxFIFO filter table */
    FLEXCAN_DRV_ConfigRxFifo(INST_CANCOM1,FLEXCAN_RX_FIFO_ID_FORMAT_B,RxFIFO_FilterAcceptanceID_Table);

    /*configure the FlexCAN RxFIFO filter table ID mask */
    for(i=0; i<RxFIFO_USED_MB; i++)
    {
    	FLEXCAN_DRV_SetRxIndividualMask(INST_CANCOM1,CAN_STAND_MODE,i,0x7FC);//RxFIFO_ACK_ID_MASK
    }

    /* configure the RX MB with receive acceptance ID */
    FLEXCAN_DRV_ConfigRxMb(INST_CANCOM1,RX_MB, &CAN_RX1_MB_Config,RX_ACK_ID);

    /* configure the RX MB ID filer type as individual */
    FLEXCAN_DRV_SetRxMaskType(INST_CANCOM1,FLEXCAN_RX_MASK_INDIVIDUAL);

    /* configure the RX MB individual ID filter mask */
    FLEXCAN_DRV_SetRxIndividualMask(INST_CANCOM1,CAN_STAND_MODE,RX_MB,RX_ACK_ID_MASK);
}
void Init_Mscan(void)
{
	extern void Init_Mscan2(void);
    /* Initialize pins */
    CAN_User_Init();/* initialize the CAN module as needed */

    /* start the RxFIFO CAN message receive */
    FLEXCAN_DRV_RxFifo(INST_CANCOM1,&CAN_RxFIFO_RX_Message_Buffer[CAN_RxFIFO_RX_BufferIndex]);

   /* start the individual MB CAN message receive */
    FLEXCAN_DRV_Receive(INST_CANCOM1, RX_MB, &CAN_RX_Message_Buffer[CAN_RX_BufferIndex]);
    Init_Mscan2();
}


/*****************************************************************************//*!
   *
   * @brief MSCAN rx interrupt
   *        
   * @param  
   *
   * @return none
   *
   * @ Pass/ Fail criteria:  none
*****************************************************************************/
void MSCAN_Rx_IRQHandler(void)
{

}

/*****************************************************************************//*!
   *
   * @brief MSCAN tx interrupt
   *        
   * @param  
   *
   * @return none
   *
   * @ Pass/ Fail criteria:  none WAKEUP ,AND ERR  的中断也此处完成
   *  实际测试错误中断无法响应
*****************************************************************************/
void MSCAN_Tx_IRQHandler(void)
{

}

/*******************************************************************************
 * Function:        BOOL CANDrive_TxBufferEmptemp(void)
 *					
 * Description:     CAN bufer是否为空
 *
 * Returns:         none
 *
 * Notes:           none		
 *
********************************************************************************/
BOOL CANDrive_TxBufferEmptemp(void)
{
	return FALSE;
}
