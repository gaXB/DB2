/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�NetLaye.c
* ��������������� �Ĺ���ʵ�ֺ����� ���ļ�����Ҫ����
* 1  ISO15765-2 �����ݷ��ͺͽ��չ���
* 2  ֪ͨӦ�ò����ݽ��շ�����״̬
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
ver 000 
2013.1.1
UDS ����� 15765-2 
��ʼ�汾������EJ04���Գɹ�

2015.01.30
ver01
1 �޸Ĳ��ֹ��ܾ���saic����
2 ȥ��������֡��id �ж���δ���ԣ�
3 �����ź�δ�õ�Ĭ��Ϊ0

2015.2.23
ver02
1 2015 2 13 ���µ����
2  ͨ��Ϊ��˫�� ģʽ�������ڷ���ʱ���ܽ��н��� 
   �ڽ��յ�ʱ���͵Ĵ��� δ��
3  ������֡����Ϊ����Ѱַ
4  ������������ܵ�������֡������
5  �����겻��Ҫ�ȴ�����֡

2015.8.20
ver03
1 ����ر�CAN�ĺ��� Dis/EnableCanRxInterrupt

2016.7.4
ver04
1 �������bug�޸�

2019.10.25
ver0.5
1 ����ɹ淶

2019.12.24
ver0.6
1 NM �ɿ�������Ƿ���

2020.7.6
ver0.7
1 �Ż�  L_SendDiagFram �������֡����������ȥ������

2021.8.23
ver0.8
1 NOP ��ΪMCU_NOP
*/

#include "NetLaye.h"

/**************************************************************
�����궨��                                      
*********************************************************************/

//pci type define
#define  PCI_TYPE_SF    0
#define  PCI_TYPE_FF    1
#define  PCI_TYPE_CF    2
#define  PCI_TYPE_FC    3

//flow control fram fs type
#define FC_FS_CONTISEND   0
#define FC_FS_WAIT        1
#define FC_FS_OVER        2

     
//����״̬
#define  RECIVE_STATE_IDLE 0
#define  RECIVE_STATE_     
#define  RECIVE_STATE_RECCONDION    2  //��������֡
#define  RECIVE_STATE_SENDFC   3

//����״̬
#define  SEND_STATE_IDLE 0
#define  SEND_STATE_SEND_FF   1     
#define  SEND_STATE_SENCONDION    2  //SEND����֡
#define  SEND_STATE_WAITFC    3
//#define  SEND_STATE_REC_FC    4




/**************************************************************
�Ĵ�������                                   
*********************************************************************/

static   N_PDU   nPdu_Buffer[MAX_NPDU_BUFFER];
static   uint8   nPduDealNumber=0;
static   N_USDATA_IND   nUsdata_Rec; //  �������ݻ���

static   N_USDATA_CON   nUsdata_Con;           //confirm
static   N_USDATA_IND   nUsdata_Send,nUsdata_Send_1;        //  �������ݻ���
static   uint8          SendBuffNumber=0;
//��ʱ����
static   uint16 TimerN_Ar,   //����㷢�͵�ʱ��
      TimerN_As,
      TimerN_Bs,     // ���ͷŽ���������֡��ʱ��
      TimerN_Cr;     // ���ܷŽ�������֡��ʱ��
 
static N_SendData SData_FL;     //funct and  phy  all use this
static N_RecData  RData_FL=
 {
 0,0,0,0,NETLAY_BS,NETLAY_STmin
 };         //funct and phy all use this

static uint8 Bug_Num;
N_PDU   nPduTest[8];
uint8 iTestNum;
  // static uint16 st_i,stmin[8];
  
/*******************************************************************************
����õ��ⲿ��������
********************************************************************************/
void  F_N_USData_IND(N_USDATA_IND*   NetInd) ;
void  F_N_USData_CON(N_USDATA_CON   NetCon) ;
void  F_N_USData_FF_IND(uint8 nId,uint8 Length);

uint8 CanTxComplete(void);
uint8 ComGetTxEn(void);
//��ģ��ʵ�ֵĺ���
void  NetLay_RecFC(N_PDU    *nPdu) ;
void  NetLay_Recive(N_PDU   *nPdu);

/*******************************************************************************
 * Function:        void L_SendDiagFram(NetData)
 *
 * Description:     ����һ���ź�
 *
 * Returns:         none
 *
 * Notes:           �����֡()�յ�ʱ,����		
 *
********************************************************************************/
static uint16 u16DigSErrTimes;
extern Std_ReturnType Can2If_Transmit(PduIdType CanTxPduId, PduInfoType const * PduInfoPtr);
void L_SendDiagFram(uint8* NetData)
{
	PduInfoType lPdu;
	lPdu.SduLength = 8;
	lPdu.SduDataPtr = NetData;
	uint8 nSendTimes = 0;
	
	do
	{
		if (Can2If_Transmit(CANDIAG_IFID, &lPdu) == CANIF_RETURN_OK)
		{
			break;
		}
		else
		{
			nSendTimes++;
		}
	}while (nSendTimes < 10);
	
	if (nSendTimes >= 10)
	{
		u16DigSErrTimes++; 
	}
	else{}
}

void Send_78NRC(uint8 SID)
{
   uint8 LL_Data[8] = 
   {
   0,0,0,0,0,0,0,0
   };
   uint16 u16wCount = 0;
   LL_Data[0] = 0x03;
   LL_Data[1]=0x7f ;    //pci 0
   LL_Data[2]= SID;        //pci 1
   LL_Data[3]= 0x78;
   
   L_SendDiagFram(LL_Data);
   //wait untile send 
   while (!CanTxComplete()) 
   {
   	MCU_NOP;
   	u16wCount++;
   	if (u16wCount > 1000)
   	{//��ֹ��ѭ��
   		break;
   	}
   	else{}
   }
}
/*******************************************************************************
 * Function:        void  NetLay_Recive(N_PDU *  nPdu)
 *
 * Description:     CAN��������� ���ܵ�һ���źţ�������
 *
 * Returns:         none
 *
 * Notes:           �����֡()�յ�ʱ,����		
 *
********************************************************************************/
uint8 test1;
void NetLay_Recive(N_PDU*   nPdu)
{
//   uint8 i;

   N_PCI CCF_Pci;
   uint8 LL_Data[8] = 
   {
   0,0,0,0,0,0,0,0
   };
   
   //fc fram   deal
  // NetLay_RecFC(nPdu);
   //rec deal
   if (SData_FL.S_State != SEND_STATE_IDLE) 
   { //ͨ��Ϊ��˫�� ģʽ�������ڷ���ʱ���ܽ��н��� 
     //�ڽ��յ�ʱ���͵Ĵ��� δ��
     return ;
   }
   if(nPdu->Pci.nPciType==PCI_TYPE_SF||
   nPdu->Pci.nPciType==PCI_TYPE_FF )
   {
     RData_FL.R_State=RECIVE_STATE_IDLE; 
   }
   switch(RData_FL.R_State)
   {
      case RECIVE_STATE_IDLE:
         if(nPdu->Pci.nPciType==PCI_TYPE_SF) 
         {  //SF
            if(nPdu->Pci.Length_SN&&
            nPdu->Pci.Length_SN<=7)
            {   
            #ifdef _DEBUG_   
               if(Test_Ser)
               {
                  SysErcode.Bits.b0=1;
               }
            #endif
               nUsdata_Rec.N_Result=N_Success;
               nUsdata_Rec.nId= nPdu->nId;
               nUsdata_Rec.Length=nPdu->Pci.Length_SN;
              //recive sf
               MemCopy(nUsdata_Rec.Data, nPdu->Data, (uint8)nUsdata_Rec.Length)  ; 
               RData_FL.R_Length=nUsdata_Rec.Length;
               F_N_USData_IND(&nUsdata_Rec) ;
            }
         }
         else  if(nPdu->Pci.nPciType==PCI_TYPE_FF)
         {  //FF
            nUsdata_Rec.nId= nPdu->nId;
            if(nPdu->Pci.Length_SN==0&&nPdu->Data[0]<NETLAY_MAXLENGTH)
            {
               if(nPdu->Data[0]==0) return;
               nUsdata_Rec.Length=nPdu->Data[0];  //ֻ֧�����255 ����
               CCF_Pci.Length_SN=FC_FS_CONTISEND ; // 
               RData_FL.R_State=RECIVE_STATE_RECCONDION;  //begin rec continue fram
               
               // ����㶨ʱ
               (void)TimeOutChkMsLong(&TimerN_Ar,0) ;
               (void)TimeOutChkMsLong(&TimerN_Cr,0) ;
               RData_FL.R_mId=nPdu->nId;
               //reciv ff  ,first fram  data[0]=length ,data[1]-ind.data[0]
               MemCopy(nUsdata_Rec.Data, &nPdu->Data[1], 6) ;  
               RData_FL.R_Length=6;
               RData_FL.R_SN=1;
               
               F_N_USData_FF_IND(nUsdata_Rec.nId,(uint8)nUsdata_Rec.Length);
            }
            else
            {  // max lenth<256 err  N_Buffer_Overflow
              // nUsdata_Rec.nId=nPdu.nId;
              // nUsdata_Rec.N_Result = N_Buffer_Overflow ;
               //confirm to applay
              // F_N_USData_CON(nUsdata_Con)  ;    //receiver don't need con to app
               
               CCF_Pci.Length_SN=FC_FS_OVER; //send over cf
               
            }

            if(nPdu->nId==AID_PHYSIC)
            {
               CCF_Pci.nPciType=PCI_TYPE_FC;
               //send flow control fram
               LL_Data[0]=*(uint8*)&CCF_Pci;
               LL_Data[1] = NETLAY_BS;         
               RData_FL.R_BS = NETLAY_BS;      // ����bs���趨�¸�������֡���͵�bs
               LL_Data[2]=(uint8)NETLAY_STmin; 
               //������ֻ֧֡������Ѱַ
               L_SendDiagFram(LL_Data);
            }
         }
         //other fram is err
         break;
      case  RECIVE_STATE_RECCONDION :
         if(TimeOutChkMsLong(&TimerN_Cr,TIMER_N_CR) )
         {//��ʱ
             nUsdata_Rec.nId= nPdu->nId;
             nUsdata_Rec.N_Result = N_Timeout_Cr ;
            //ind to applay
             F_N_USData_IND(&nUsdata_Rec)  ;
             RData_FL.R_State=RECIVE_STATE_IDLE;      //timerout cr stop rec
         }
         else
         {
            if( RData_FL.R_mId==nPdu->nId&&nPdu->Pci.nPciType==PCI_TYPE_CF)
            {
               //RECIVE CONTINUE
               (void)TimeOutChkMsLong(&TimerN_Cr,0)  ;
              
               if (RData_FL.R_SN==nPdu->Pci.Length_SN)
               { //drge SN
                  RData_FL.R_SN++;
                  if(RData_FL.R_SN>=16)  RData_FL.R_SN=0;
                  if((nUsdata_Rec.Length-RData_FL.R_Length)<=7)
                  {  //recieve ok
                     //Recive 
                     MemCopy(&nUsdata_Rec.Data[RData_FL.R_Length], nPdu->Data,  nUsdata_Rec.Length-RData_FL.R_Length) ;  
                     nUsdata_Rec.N_Result= N_Success;
                     RData_FL.R_Length+=nUsdata_Rec.Length;
                     
                     RData_FL.R_State=RECIVE_STATE_IDLE;
                     F_N_USData_IND(&nUsdata_Rec);
                     

                  }
                  else
                  {
                     //Recive cf 7
                     MemCopy(&nUsdata_Rec.Data[RData_FL.R_Length], nPdu->Data, 7) ; 
                     RData_FL.R_Length+=7;
                     
                     //add bs !=0  program
                     RData_FL.R_BS--;
                     test1 = RData_FL.R_BS;
                     if (test1 < 2)
                     {
                        MCU_NOP;
                     }
                     if (!RData_FL.R_BS && NETLAY_BS && nPdu->nId==AID_PHYSIC)
                     {// bs���������� ��Ҫ����fc�� ������Ѱַ
                        //send flow control fram
                       // CCF_Pci.nPciType=PCI_TYPE_FC;
                        LL_Data[0]= 0x30;     //����Ҫ�жϳ���ǰ���Ѿ��жϹ���
                        LL_Data[1] = NETLAY_BS;         
                        RData_FL.R_BS = NETLAY_BS; 
                        LL_Data[2]=(uint8)NETLAY_STmin; 
                        //������ֻ֧֡������Ѱַ
                        L_SendDiagFram(LL_Data);   
                     }
                     
                  }
               }
               else
               {
                   nUsdata_Rec.nId= nPdu->nId;
                   nUsdata_Rec.N_Result = N_Wrong_SN ;
                  //IND to applay Wrong SN
                   F_N_USData_IND(&nUsdata_Rec)  ;
                   RData_FL.R_State=RECIVE_STATE_IDLE;      // stop rec
               }   
            } 
         }
         break;
      default:
         break;  
   }
  //nPdu.nid
  //switch(nPdu.pci.PCI.nPciType) 
}

/*******************************************************************************
 * Function:        uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length)
 *
 * Description:     CAN��������� ,��Ӧ�ò���÷�������
 *
 * parameter1 ��   ReqData ���ݣ� Length ����    mId_Target��ʵ�ʲ���Ҫ
 * Returns:         none
 *
 * Notes:           Ӧ�ò����		
 *
********************************************************************************/
uint8  F_N_USDATA_REQ(uint8* ReqData,uint16 Length,uint8 mId_Target)
{

  
   if(Length>NETLAY_MAXLENGTH)
   {
      return 0;
   }
   #ifdef _DEBUG_
   if(Test_Ser)
   {
      SysErcode.Bits.b3=1;
   }
   #endif
   if(SData_FL.S_State!=SEND_STATE_IDLE)
   {  //���뻺��ռ�
      nUsdata_Send_1.Length=Length;
      nUsdata_Send_1.N_Result=0;
      MemCopy(nUsdata_Send_1.Data, ReqData, (uint8)Length);
      nUsdata_Send_1.nId=mId_Target; 
      SendBuffNumber=1;
   }
   else
   {
      nUsdata_Send.Length=Length;
      nUsdata_Send.N_Result=0;
      MemCopy(nUsdata_Send.Data, ReqData, (uint8)Length);
         
      nUsdata_Send.nId=mId_Target;  
      SData_FL.S_mId=mId_Target;
      SData_FL.S_State=SEND_STATE_SEND_FF;
   }
   return 1;
}
/*******************************************************************************
 * Function:       void  NetLay_RecFC(N_PDU   *nPdu) 
 *
 * Description:     CAN��������� Receive Control Fram
 *
 * Returns:         none
 *
 * Notes:         ���ܵ� control fram ʱ����
 *
********************************************************************************/
void  NetLay_RecFC(N_PDU   *nPdu) 
{

   if(SData_FL.S_State==SEND_STATE_WAITFC
   &&nPdu->Pci.nPciType==PCI_TYPE_FC
   && nPdu->nId == AID_PHYSIC)
   {
       //���ܵ�������֡��������
     // if(SData_FL.S_mId==nPdu->nId)
    //  {����Ҫ�жϷ�������id���ϴ������ID�ǲ���ͬһ��ID 20150130 xb
         
         // Pci.Length_SN==FS data[0]=bs  data[1]=stmin  
         switch(nPdu->Pci.Length_SN)
         {
            case  FC_FS_CONTISEND:
            //���ܵ�������֡
               if(TimeOutChkMsLong(&TimerN_Bs,TIMER_N_BS))
               {  //����ʧ�ܣ�������֡�ȴ�ʱ�䳬ʱ
                  nUsdata_Con.nId= nUsdata_Send.nId;
                  nUsdata_Con.N_Result = N_Timeout_Bs ;
                  //con to  TIMER_N_BS
                  F_N_USData_CON(nUsdata_Con)  ;
                  SData_FL.S_State=SEND_STATE_IDLE;
                  if(nUsdata_Send.Length==51)
                  {
                     Bug_Num=128;
                      
                  }
               }
               else
               {
                 //���ܵ�������֡
                  SData_FL.S_BS=nPdu->Data[0];
                  SData_FL.S_STmin=nPdu->Data[1]+2;

                  SData_FL.S_State=SEND_STATE_SENCONDION ;
               }
               break;
            case  FC_FS_WAIT:
            // �ȴ���һ֡������֡
               (void)TimeOutChkMsLong(&TimerN_Bs,0);
               //�˴���Ҫע���Ƿ񳬹����N_WFT,
               if(NETLAY_N_WFTMAX==0)
               {
                  nUsdata_Con.nId= nUsdata_Send.nId;
                  nUsdata_Con.N_Result = N_WFT_OVRN ;
                  //con to applay N_WFT_OVRN
                  F_N_USData_CON(nUsdata_Con)  ;
                  //don't send  
                  SData_FL.S_State=SEND_STATE_IDLE ;
               }
               else
               {
               }
               
               break;
            case  FC_FS_OVER:
              //�������ݳ���̫��
               nUsdata_Con.nId= nUsdata_Send.nId;
               nUsdata_Con.N_Result = N_Buffer_Overflow ;
               //con to applay length over
               F_N_USData_CON(nUsdata_Con)  ;
               //don't send  
               SData_FL.S_State=SEND_STATE_IDLE ;
               break;
            default:           //err fc
               nUsdata_Con.nId= nUsdata_Send.nId;
               nUsdata_Con.N_Result = N_Invalid_FS ;
               //con to applay N_Invalid_FS
               F_N_USData_CON(nUsdata_Con)  ;
               //don't send  
               SData_FL.S_State=SEND_STATE_IDLE ;
               break;
               
         }
      }
      else
      {   ////ֻ���ڵȴ�������֡��״̬����Ч�������ͺ��Դ�֡   150212
      //   SData_FL.S_State=SEND_STATE_IDLE ;
      }
  // }
}

/*******************************************************************************
 * Function:     void  NetLay_SendFram(void)
 *
 * Description:     CAN���������1  ���ݷ��ʹ���   2 ��������֡ʱ TimerN_Cr��ʱ����
 *
 * Returns:         none
 *
 * Notes:       LOOP����
 *
********************************************************************************/
void  NetLay_SendFram(void)
{
   uint8   LL_Data[8] =
   {
   0, 0, 0, 0, 0, 0, 0, 0
   };
   N_PDU   nPdu;
   switch(SData_FL.S_State) 
   {
      default:
      case SEND_STATE_IDLE:
         if(SendBuffNumber)
         {
            SendBuffNumber=0;
            nUsdata_Send=nUsdata_Send_1;
            SData_FL.S_mId=nUsdata_Send.nId;
            if (ComGetTxEn())
         	{
         		SData_FL.S_State=SEND_STATE_SEND_FF; 
         	}
         	else
         	{
         		
         	}   
         }
         break; 
      case  SEND_STATE_SEND_FF:
        // send first fram
         if(nUsdata_Send.Length<=7)
         {  //send signal fram
          //  nPdu.Pci.PCI.             
            nPdu.Pci.Length_SN=(uint8)nUsdata_Send.Length;
            nPdu.Pci.nPciType=PCI_TYPE_SF;
            LL_Data[0]=*(uint8*)&nPdu.Pci;
            MemCopy(&LL_Data[1], nUsdata_Send.Data, nPdu.Pci.Length_SN) ;
            SData_FL.S_State=SEND_STATE_IDLE;
            //send ok confrim to app ok
            nUsdata_Con.nId= nUsdata_Send.nId;
            nUsdata_Con.N_Result = N_Success ;
            L_SendDiagFram(LL_Data); 
            F_N_USData_CON(nUsdata_Con) ;   //��success Ӧ���ڷ��������ָʾ��Ӧ�ò�
         }
         else
         {  //send first fram
            nPdu.Pci.Length_SN=0;
            nPdu.Pci.nPciType=PCI_TYPE_FF;
            LL_Data[0]=*(uint8*)&nPdu.Pci;
            LL_Data[1]= nUsdata_Send.Length&0x00ff;
            MemCopy(&LL_Data[2], nUsdata_Send.Data, 6) ;
            SData_FL.S_State=SEND_STATE_WAITFC;
            (void)TimeOutChkMsLong(&TimerN_Bs,0);
            SData_FL.S_Length=6;
            SData_FL.S_SN=1;
            L_SendDiagFram(LL_Data); 
         }
         
         break;
      case  SEND_STATE_WAITFC:
      //�ȴ�������֡
         if(TimeOutChkMsLong(&TimerN_Bs,TIMER_N_BS))
         {  //����ʧ�ܣ�������֡�ȴ�ʱ�䳬ʱ
            nUsdata_Con.nId= nUsdata_Send.nId;
            nUsdata_Con.N_Result = N_Timeout_Bs ;
            //con to applay N_Timeout_Bs 
            F_N_USData_CON(nUsdata_Con)  ;
            SData_FL.S_State=SEND_STATE_IDLE;
            if(nUsdata_Send.Length==51)Bug_Num++;
         }
         break;
      case  SEND_STATE_SENCONDION:
         //send continue fram
         //if(SData_FL.)
         if(TimeOutChkMsLong(&SData_FL.S_TimerSend, SData_FL.S_STmin))
         {
            nPdu.Pci.nPciType=PCI_TYPE_CF; //CONTINUE FRAM
            nPdu.Pci.Length_SN=SData_FL.S_SN++;
            LL_Data[0]=*(uint8*)&nPdu.Pci;
            if((nUsdata_Send.Length-SData_FL.S_Length)>7)
            {
               MemCopy(&LL_Data[1], &nUsdata_Send.Data[SData_FL.S_Length], 7) ;
               L_SendDiagFram(LL_Data);
               
               SData_FL.S_BS--;  //150212
               if (SData_FL.S_BS == 0)
               {//�������֡���ʹ����Ѿ�����BS����Ҫ�ȴ�������֡
                  SData_FL.S_State=SEND_STATE_WAITFC;
                  (void)TimeOutChkMsLong(&TimerN_Bs,0);  
               }  
            }
            else
            {//send last fram
               MemCopy(&LL_Data[1], &nUsdata_Send.Data[SData_FL.S_Length], nUsdata_Send.Length-SData_FL.S_Length) ;
               SData_FL.S_State=SEND_STATE_IDLE;   
              //send ok confrim to app ok
               nUsdata_Con.nId= nUsdata_Send.nId;
               nUsdata_Con.N_Result = N_Success ;
               L_SendDiagFram(LL_Data); 
               F_N_USData_CON(nUsdata_Con) ;
            }
            
            SData_FL.S_Length+=7;

         }
         //must care the bs
         break;     
   }
   
   //rec continue fram  TimerN_Cr timeout
   if(RData_FL.R_State==RECIVE_STATE_RECCONDION)
   {
      if(TimeOutChkMsLong(&TimerN_Cr,TIMER_N_CR) )
      {//��ʱ
          nUsdata_Rec.nId= nPdu.nId;
          nUsdata_Rec.N_Result = N_Timeout_Cr ;
         //ind to applay
          F_N_USData_IND(&nUsdata_Rec)  ;
          RData_FL.R_State=RECIVE_STATE_IDLE;      //timerout cr stop rec
      }  
   }
   
}

/*******************************************************************************
 * Function:        void  NetLay_Recive(N_PDU   nPdu)
 *
 * Description:     CAN LL_LAY ���ܵ�һ��CAN ֡ʱ���д�������nPdu_Buffer[] 
 *
 * Returns:         none
 *
 * Notes:           �߼���·����ã�		
 *
********************************************************************************/
void  NetLay_ReciveDiag(uint8 nId,uint8* L_data)
{
 //  int i=0;
   N_PDU   nPdu;
   nPdu.nId=nId;
   // nPdu.Pci=*(N_PCI*)L_data;
	nPdu.Pci.nPciType = (L_data[0]&0xf0)>>4;
	nPdu.Pci.Length_SN = L_data[0]&0x0f;
    
    
   MemCopy(nPdu.Data, &L_data[1], 7) ;
   nPduTest[iTestNum++] = nPdu;
   if (iTestNum >= 8)
   {
   	iTestNum = 0;
   }
   #ifdef _DEBUG_ 
   if( Test_Ser)  Test_Ser=0;

   if(nId==0&&L_data[0]==4&&
   L_data[1]==0X22&&L_data[2]==0&L_data[4]==0)
   {
      #ifdef _DEBUG_
         if(Bug_Num%4==1)
         {
            Test_Ser=1;
         }
         else
         {
            Test_Ser=0;  
            SysErcode.Byte=0;
         }
      #endif
   } 
   #endif
   if(nPduDealNumber<MAX_NPDU_BUFFER)
   {
      nPdu_Buffer[nPduDealNumber]=nPdu; 
      nPduDealNumber++;
   }
   else
   {
      MCU_NOP;  //���
   #ifdef _DEBUG_
      Test_Flg=1;
   #endif
   }
}

/*******************************************************************************
 * Function:        void  Deal_UDSNetLay(void)
 *
 * Description:     ������������ݣ� 1   ��������  2 ������� 
 *
 *                 
 * Returns:         none
 *
 * Notes:           ����mainloop�С�		
 *
********************************************************************************/
void  Deal_UDSNetLay(void)
{
   N_PDU   nPdu;
   uint8   deal_en=0,deal_num, i;
   
   DisableCanRxInterrupt();
 //  DisableInterrupts;
   if(nPduDealNumber)
   {  //���л��崦��
      nPduDealNumber--;
      nPdu=nPdu_Buffer[0]; 
      deal_en=1;
      deal_num=nPduDealNumber;
      
      
      //2016/7/4 ����bug�� 
      for (i = 0; i < deal_num; i++)
      {
      	nPdu_Buffer[i]=nPdu_Buffer[i+1];
      }
      /*while(deal_num) 
      {
         nPdu_Buffer[deal_num-1]=nPdu_Buffer[deal_num];
         deal_num--;
      }*/

   }

//   EnableInterrupts ;
   EnableCanRxInterrupt();
   
   if(deal_en)
   {
	   Clear_WatchDog(); /* feeds the dog */
      //������ܵ�����
      if(nPdu.Pci.nPciType==PCI_TYPE_FC)
      {//���ܵ�������֡Ϊ���� Send����������REC
         NetLay_RecFC( &nPdu) ;
      } 
      else
      {
         NetLay_Recive( &nPdu)  ;
      }
   }
   
   //������
   NetLay_SendFram() ;
   
}
