/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�OsekNm_App.c
* �����������������ص�����
* �����ڽӼ��osek���������ʹ�ô��ļ�
* �������� codewarrior IDE 10.7
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��
* �汾��2.0
* ˵����֧��autosar
* ԭ���ߣ�xb
* ������ڣ�2019��1��9��
*/

#include "CanIf.h"
#include "CANIDConfig.h"
#include "NM_APP.h"
#if _NEED_OSKNM_
	#include "OsekNm.h"
#endif
#if _INDIRECT_NM_
	#include "IndirectNM.h"
#endif

#if _NEED_AUTOSARNM_
	#include "IndirectNMAutosar.h"
#endif


uint8 Network_WakeFlag;
static uint8 u8BusOffTimes;
static uint8 u8EndTimse;
static uint8 nLimpHomeTimes = 0;
static uint8 bLimpHomeDTC = 0;

extern void PowerMN_CanSleepEn(uint8 bEn);
/*******************************************************************************
*   Call back functions for OsekNm Module
*******************************************************************************/
/*Called only when NM started*/
void ApplNmCanStart(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    /*Add App code here*/
    Com_RxStart();
    Com_TxStart();
    Com_TxEnDealy();
    
    PowerMN_CanSleepEn(0);
}
/*************************************************************/
/*Perform when enter limphome mode*/
void ApplNmCanLimpHomeStart(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    /*Add App code here*/
}
/*************************************************************/
/*Perform when Terror TimeOut.*/
void ApplNmLimpHomeState(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    nLimpHomeTimes++;
    if (nLimpHomeTimes >= 2)
    {
    	bLimpHomeDTC = 1;
    }
    /*Add App code here*/
}
/*************************************************************/
/*Perform when Exit LimpHome State*/
void ApplNmCanLimpHomeEnd(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    nLimpHomeTimes = 0;
    bLimpHomeDTC = 0;
    
    /*Add App code here*/
}
/*************************************************************/
/*indicates bus off error occured*/
void ApplNmBusOff(const nmIndexType nmChannelHandle)
{
    /*Stop Com Tx (and Rx)*/
    (void)nmChannelHandle;
    /*Add App code here*/
   // Com_RxStop();
    Com_TxStop();
    if (u8BusOffTimes <= 255)
    {
   	 u8BusOffTimes++;
    }
    else{}
}
/*************************************************************/
/*indicate end of bus off error */
void ApplNmBusOffEnd(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    /*Add App code here*/
    Com_RxStart();
    Com_TxStart();
    u8EndTimse = u8BusOffTimes;
    u8BusOffTimes = 0;
    
}
/*************************************************************/
/*This callback function is performed when busOff recovery successed.*/
void ApplNmBusOffRecoverySuccess(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    //u8BusOffTimes = 0;
    /*Add App code here*/
}
/*************************************************************/
void ApplNmCanBusOffRecoveryFail(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    /*Add App code here*/
	/*Bus Off recovery failed */
   /*Add App code here*/
}

/*************************************************************/
/*This callback sets the communication hardware (CAN controller, bus transceiver)
to a state from which they can be woken up by the CAN bus.*/
void ApplNmCanSleep(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    PowerMN_CanSleepEn(1);
    /*Add App code here*/
  //  CanIf_SetControllerMode(NmToLogCanChn(nmChannelHandle),CANIF_CS_SLEEP);
}

/*************************************************************/
/*indicates can bus wake up*/
void ApplNmCanWakeUp(const nmIndexType nmChannelHandle)
{
    /*Be sure Can Trv or Can controller work normal*/
    /*Set CANcontroller started mode*/
    Network_WakeFlag = 1;
 //   Com_RxStart();
   Com_TxStart();
  //  CanIf_SetControllerMode(NmToLogCanChn(nmChannelHandle),CANIF_CS_STARTED);
}
/*************************************************************/
void ApplNmWaitBusSleepCancel(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    /*Add App code here*/
    Com_RxStart();
    Com_TxStart();
}
/*************************************************************/
void ApplNmWaitBusSleep(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    /*Add App code here*/
  //  Com_RxStop();
    Com_TxStop();
    CanIf_ClearTransmitBuffer(0);
    CANDriver_ClearTransmitBuffer();  //Ϊ�˷�ֹ�ر�ʱ��Ӳ�����滹���������Ӵ˾�
}
/*************************************************************/
void ApplNmShutDown(const nmIndexType nmChannelHandle)
{
    (void)nmChannelHandle;
    /*Add App code here*/
}
/*************************************************************/
void ApplNmRingIndication(const nmIndexType nmChannelHandle,const nmNetIdType SourceID)
{
    (void)nmChannelHandle;
    /*Add App code here*/
}
/*************************************************************/
void ApplNmLimpHIndication(const nmIndexType nmChannelHandle,const nmNetIdType SourceID)
{
    (void)nmChannelHandle;
    /*Add App code here*/
}

#if(NM_ACK_CHECK == STD_ON)
	/*************************************************************/
	void ApplNmAckRecoveryStart(const nmIndexType nmChannelHandle)
	{
	    CanNm_Silent(nmChannelHandle);	
	}
	/*************************************************************/
	void ApplNmAckRecoveryEnd(const nmIndexType nmChannelHandle)
	{
	    
	    PduInfoType a;
	    a.SduLength = 8;
	    CanIf_Transmit(0,&a);
	    CanNm_Talk(nmChannelHandle);
	    
	}
#endif

	
uint8 NMAPP_GetLimpHomeDTC(void)
{
	return bLimpHomeDTC;
}


uint8 NMAPP_GetBusOffTimes(void)
{
	return u8BusOffTimes;
}
