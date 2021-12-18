/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ApplayLay.c
* ����������UDS��ϵ�Ӧ�ò������� ���ļ�����Ҫ���ģ��򲿷ֿͻ�Ҫ��ͬ���в���Ҫ�޸ģ�
* 1  ISO15765-2 �����ݷ��ͺͽ��չ���
* 2  ֪ͨӦ�ò����ݽ��շ�����״̬
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 

2019.1.17
ver0.1
1 ��ʼ�汾

2019.3.13
ver0.2
1 ��������


* �汾��1.1
* ˵��������Ԥ����__NRC22__
* ���ߣ�xb
* ������ڣ�2019��10��25��


* �汾��1.2
* ˵����
�� 1 BT ����Ԥ����ģʽ���ܼ���
�� 2 Ԥ��srand
�� 3 ��������diaging ����Diag_GetIsDiag
�� 4 ���ֹ����Ż�
�� 5 uint8 NmControlEnGet(void) ��Ͽ���NM
* 6 �滭ģʽ�õ�
* ���ߣ�xb
* ������ڣ�2019��12��24��
* 
* �汾��1.3
* ˵����
* 1 rand ��GA_rand �����滻
* 2 if (*(uint32*)(0x0001C000u-4) == 0xffffffff) ����Ϊ if (Flash_uint32_read(0x0001C000-4) == 0xffffffff)
* ���ߣ�xb
* ������ڣ�2021��8��23��
* 
*/


#include "ApplayLay.h"
#include "Deal_DTC.h"
#include "AppLayInterface.h"
#include "NetLaye.h"

/****************************************************************************************
1 �汾�ź�
2015 1-17
1 �޸� req 0x27 ����Ӧ����
2 F192/F194/F1A2/F18B/F18C ����did
3 F1A1 ���� 
4 3e 0x80 Ӧ����Ӧ


********************************************************************************************/
uint8  SessionMode;    //�Ựģʽ
uint8  SecurityState;   //��ȫ����ģʽ


static   uint8  SecurityErrTimes ;    //���ʴ������
static   uint16 TimerSecurity;        //��ʱ����

//uint8 A_Data[8];
A_PDU APData;
static   uint16 TimerP2Can,TimerP3Sever;
uint8 AppState;           //Ӧ�ò�״̬
static   uint8 P3SeverStop;
static   uint8 UDS_ResetState;  //��λ״̬�� 1 ��Ҫ��λ�� ��һ֡�������ɺ���и�λ���� F_N_USData_CON ������ʵ��
static   uint8 bBootPrepare;
//����״̬����
//CCSTATE
static union
{
   struct
   {
      unsigned st_ncm   :1;      //1 ��ֹ����
      unsigned st_nwmcm :1;
   }Bits;
   uint8  Byte;
}CCState;

//storing state  1: not stor
static   uint8 StorState;  
//uint16 pDATAT ;

static uint32  nSeed, nKey;

//pid tbl_char
//const
static uint8 PosRespone_EE;
static BOOL bDiaging;


//��ģ����ʵ�ֺ�������
void  F_N_USData_CON(N_USDATA_CON   NetCon);
void  Respone_POS_2(uint8 SID,uint8 FSID,uint8 Target_Id);
void Send_78NRC(uint8 SID);
//DID.Cʵ��
extern void Deal_ReadDID(void);
extern void Deal_WriteDID(void);
extern void Deal_IOControl(void);
extern void Deal_ReadAddr(void);


//snapshort record respone
/*******************************************************************************
 * Function:      void Respone_POS_SnapShort(uint8 dtc_num)
 *
 * Description:    snapshort record respone
 *
 * Returns:         none
 *
 * Notes:          ����ӦDTC Snap�� �˺�����Ҫ����Ӧ�����ٲ����ָ���
********************************************************************************/
void Respone_POS_SnapShort(uint8 dtc_num)
{
   uint8 DTC_Data[25];     //80

 //  uint8 i,Adata_num=3;   // PCI FSID MASK  thefirst data is 3
   DTC_Data[0] = SEVER_SID_READDTC+0x40 ;    //pci
   DTC_Data[1] = 0x04;
   DTC_Data[2] = APData.A_Data[1];
   DTC_Data[3] = APData.A_Data[2];
   DTC_Data[4] = APData.A_Data[3];
   DTC_Data[5] = DTC[dtc_num] & SUPPORT_DTC;
   
   
   DTC_Data[6] = 0X01;  //����saic �淶
   DTC_Data[7] = 0X04;  //֧��4��ȫ�ֿ���
   DTC_Data[8] = 0X01;
   DTC_Data[9] = 0X12;  //01 12 �ǵ�ѹ
   DTC_Data[10] = DTCSnap[dtc_num].u8Volt;
   

   DTC_Data[11] = 0Xe1; //��2����odo
   DTC_Data[12] = 0X01;  //01 12 �ǵ�ѹ
   DTC_Data[13] = (uint8)(DTCSnap[dtc_num].u32ODO>>16);
   DTC_Data[14] = (uint8)(DTCSnap[dtc_num].u32ODO>>8);
   DTC_Data[15] = (uint8)(DTCSnap[dtc_num].u32ODO);
   
 
   DTC_Data[16] = 0Xd0;  //��3����ign status
   DTC_Data[17] = 0X01;  //01 12 �ǵ�ѹ
   DTC_Data[18] = 0xff;
   

   DTC_Data[19] = 0X01;  //��4���� coolant temp
   DTC_Data[20] = 0X0c;  //01 12 �ǵ�ѹ
   DTC_Data[21] = DTCSnap[dtc_num].u8WaterTemp;
   
   (void)F_N_USDATA_REQ(DTC_Data,22,AID_PHYSIC) ;
   AppState=APP_STATE_RESPONE;
}

/**************************************************************
 ��Ӧһ������Ӧ  
 void  Respone_NRC(uint8 SID,uint8 nrc_type,uint8 Target_Id)
 Ӧ�ò��uds���񶼿���Ӧ��                           
*********************************************************************/
void  Respone_NRC(uint8 SID,uint8 nrc_type,uint8 Target_Id)
{
   uint8 A_Data[8];
   if(Target_Id==AID_FUNIC && (nrc_type==0x12 || nrc_type==0x11 || nrc_type==0x31 || nrc_type==0x7e || nrc_type==0x7f)   ) 
   {  //funct nrc respone
      //11 12 31 7e 7f ���ǹ���Ѱַ  ���ûط���Ӧ������Ҫ��
      //150212    
   }
   else
   {  //PHY nrc not resp
      A_Data[0]=0x7f ;    //pci 0
      A_Data[1]= SID;        //pci 1
      A_Data[2]= nrc_type;
   #ifdef _DEBUG_   
      if(Test_Ser)
      {
         SysErcode.Bits.b2=1;
      }
   #endif
      (void)F_N_USDATA_REQ(A_Data,3,Target_Id)  ;
      AppState=APP_STATE_RESPONE;
   }
}

/**************************************************************
sessionControl,��Ӧһ���϶���Ӧ
                                
*********************************************************************/
void  Respone_POS_SessionC(uint8 SID,uint8 subSID,uint8 Target_Id)
{
   uint8 A_Data[8];
   if(subSID&0x80)
   {//sprmib==1  ,sessioncontrol must need posrespone,so this is no used
      return;
   }
   else
   {
      //APData_Respone.A_AI=A_Id;
      A_Data[0]=SID+0x40 ;    //pci
      A_Data[1]= subSID&0x7f;
      A_Data[2]= P2CAN_Sever>>8;
      A_Data[3]= (P2CAN_Sever&0x00ff);
      A_Data[4]= P2CAN_Sever1>>8;
      A_Data[5]= (P2CAN_Sever1&0x00ff);
     
      (void)F_N_USDATA_REQ(A_Data,6,Target_Id)  ;
      AppState=APP_STATE_RESPONE ;
   }
}

/**************************************************************
sessionControl,��Ӧһ���϶���Ӧ
                                
*********************************************************************/
void  Respone_POS_SendSeed(uint8 sunSID)
{
   uint8 A_Data[8];

   //APData_Respone.A_AI=A_Id;
   A_Data[0]= 0x67;    //pci
   A_Data[1]= sunSID;
   
   Ga_srand(TimeOut_GetMsData());
   if ( (SessionMode == SESSION_MODE_EXTERN && SecurityState == SECURITY_STATE_LEVEL1) || 
         (SessionMode == SESSION_MODE_PROGRAM && SecurityState == SECURITY_STATE_LEVEL2)
   	)
   {//�Ѿ�����������00
      nSeed = 0;
   }
   else if (SessionMode == SESSION_MODE_EXTERN)
   {
      SecurityState = SECURITY_STATE_SSEED1;
    //  nSeed = SECURITY_SEED_LEVEL1;
      nSeed = Ga_rand();
      nSeed <<= 16;
      nSeed += Ga_rand();
      if (!nSeed) nSeed = 1;  //����Ϊ0
   }
   else if (SessionMode == SESSION_MODE_PROGRAM)
   {
      SecurityState = SECURITY_STATE_SSEED2;
      nSeed = Ga_rand();
      nSeed <<= 16;
      nSeed += Ga_rand();
      if (!nSeed) nSeed = 1;
   }
   else
   {  //err return
      return;
   }
   
   A_Data[2]= nSeed>>24;
   A_Data[3]= (uint8)((nSeed&0x00ff0000) >> 16);
   A_Data[4]= (nSeed&0x0000ff00) >> 8;
   A_Data[5]= (nSeed&0x000000ff);
     
   (void)F_N_USDATA_REQ(A_Data,6,AID_PHYSIC)  ;
   AppState=APP_STATE_RESPONE ;
}


/**************************************************************
��Ӧһ���϶���Ӧ ,2�����ݣ�1��SID��1��subId
                                
*********************************************************************/
void  Respone_POS_2(uint8 SID,uint8 FSID,uint8 Target_Id)
{
   uint8 A_Data[8];
   if(FSID&0x80)
   {//sprmib==1
      return;
   }
   else
   {
      //APData_Respone.A_AI=A_Id;
      A_Data[0]=SID+0x40 ;    //pci
      A_Data[1]= FSID&0x7f;


      (void)F_N_USDATA_REQ(A_Data,2,Target_Id) ;
      AppState=APP_STATE_RESPONE;
   } 
}


/**************************************************************
//��Ӧrc ����Ӧһ���϶���Ӧ                               
*********************************************************************/
void Respone_POS_RC(uint8 RCType, uint16 RC_ID, uint8 RCResult)
{
   uint8 Data[5];
   Data[0] = SEVER_SID_RC + 0x40;
   Data[1] = RCType;
   Data[2] = RC_ID>>8; 
   Data[3] = (uint8)(RC_ID&0x00ff);
   Data[4] = RCResult;
   if (RC_ID == ROUTINE_ID_ERASE) 
   {
      (void)F_N_USDATA_REQ(Data, 4, AID_PHYSIC)  ;
   }
   else
      (void)F_N_USDATA_REQ(Data, 5, AID_PHYSIC)  ;
   AppState=APP_STATE_RESPONE;
}


/**************************************************************
dtc״̬,��Ӧһ���϶���Ӧ
                                
*********************************************************************/
uint8 SearchNum_DTCMap(uint32 u32DTCData)
{
	uint8 i;
	for (i = 0; i < DTC_UDSNUM; i++)
	{
		if (u32DTCData == TBL_DTCMap[i])
		{
			return i;
		}
		else{}
	}
	
	return 0xff;
}
//��ȡDTC ����
void  Respone_POS_DtcNum(uint8 FSID,uint8 Target_Id,uint8 AvialMask)
{
   uint8 A_Data[8];
   uint8 DtcNum=0;
   uint8 i;
   
   if(FSID&0x80)
   {//sprmib==1
      return;
   } 
   
   
   for(i=0;i<DTC_UDSNUM;i++)
   {
      if(DTC[i]&AvialMask)
      {
         DtcNum++;   
      }
   }
   
   //APData_Respone.A_AI=A_Id;
   A_Data[0]=SEVER_SID_READDTC+0x40 ;    //pci
   A_Data[1]= FSID&0x7f;
   A_Data[2]= SUPPORT_DTC;
   A_Data[3]= 1;//DTC TYPE ,1
   A_Data[4]=0;   // max supoort dtc 255
   A_Data[5]=DtcNum;

   
   (void)F_N_USDATA_REQ(A_Data,6,Target_Id) ;
   AppState=APP_STATE_RESPONE;
}

//��ȡDTC ״̬
void  Respone_POS_DtcRead(uint8 FSID,uint8 Target_Id,uint8 AvialMask)
{
   uint8 DTC_Data[DTC_UDSNUM * 4 +3];     //14��֧��  dtc*4+3=59  
   uint8 i,Adata_num=3;   // PCI FSID MASK  thefirst data is 3
    
   if(FSID&0x80)
   {//sprmib==1
   #ifdef  _DEBUG_
      Test_Flg=3;
      return;
   #endif   
   } 
   
   //�˴��轫 dtc_Test �е�����EE�е������
   //read_EE
   for(i=0;i<DTC_UDSNUM;i++)
   {
		if(DTC[i]&AvialMask || AvialMask == 0xff) //��AvialMask==0xffʱ������Ϊ��Ҫ��ȡȫ��DTC��״̬
		{
			DTC_Data[Adata_num++] = (uint8)(TBL_DTCMap[i] >> 16); 
			DTC_Data[Adata_num++] = (uint8)(TBL_DTCMap[i] >> 8); 
			DTC_Data[Adata_num++]=  (uint8)(TBL_DTCMap[i]);  
			DTC_Data[Adata_num++]=DTC[i] & SUPPORT_DTC ;
		}  
   }

   //APData_Respone.A_AI=A_Id;
   DTC_Data[0]= SEVER_SID_READDTC+0x40 ;    //pci
   DTC_Data[1]= FSID&0x7f;
   DTC_Data[2]= SUPPORT_DTC;//AvialMask;//


   (void)F_N_USDATA_REQ(DTC_Data,Adata_num,Target_Id) ;
   AppState=APP_STATE_RESPONE;
}

/**************************************************************
//��Ӧrdif ����Ӧһ���϶���Ӧ                               
*********************************************************************/
void Respone_POS_WriteDid(uint16 pid_num)
{
   uint8 PID_Data[3];
   PID_Data[0]=SEVER_SID_WRITEDATABYINF+0x40;
   PID_Data[1]=pid_num>>8;
   PID_Data[2]=(uint8)(pid_num&0x00ff);
   
   (void)F_N_USDATA_REQ(PID_Data,3,AID_PHYSIC)  ;
   AppState=APP_STATE_RESPONE;
}




void Respone_POS_nSub(uint8 SID,uint8 Target_Id) 
{
   uint8 A_Data[8];

// û���ӹ���ʱ���϶���Ӧһ����Ҫ��Ӧ
   A_Data[0]=SID+0x40 ;    //pci
   
   

   (void)F_N_USDATA_REQ(A_Data,1,Target_Id)  ;
   AppState=APP_STATE_RESPONE;
}

void Respone_POS_IOControl(uint16 pid_num, uint8 *tbl, uint8 Length)
{
   uint8 IO_Data[17]; //���Ϊ14+3
  // pDATAT=(uint16)PID_Data;
  
   IO_Data[0]=SEVER_SID_IOCONTROL+0x40;
   IO_Data[1]=pid_num>>8;
   IO_Data[2]=(uint8)(pid_num&0x00ff);

   MemCopy(&IO_Data[3], tbl, Length);
    
    /* 
   if(AppState!=APP_STATE_IDLE) return;
   ��Ӧ�û��ڷ���ʱ���������Կ��Է���
   */
   (void)F_N_USDATA_REQ(IO_Data, Length+3, AID_PHYSIC)  ;
   AppState=APP_STATE_RESPONE;
}


/*******************************************************************************
 * Function:      void Deal_SecuritySever(void)    
 *
 * Description:    ��ȫ��Ϸ���
 *
 * Returns:         none
 *
 * Notes:   
********************************************************************************/
void Deal_SecuritySever(void)
{
   uint8 SubFunction;

   SubFunction = (APData.A_Data[0]&0x7f); 
   if(APData.A_AI!=AID_PHYSIC) return ;//ֻ֧������Ѱַ

   if (SessionMode == SESSION_MODE_DEFAULT)
   {  //sever not support in default session
      Respone_NRC(SEVER_SID_SECURITY,NRC_SNSIAS,AID_PHYSIC); 
      return;   
   }
   
   if (APData.A_Length == 1)
   {//27  1���ֽ�ʱ �� 20160804
   	Respone_NRC(SEVER_SID_SECURITY,NRC_IMLOIF,AID_PHYSIC);
   }
   
   switch (SubFunction) 
   {
      default:
         if (APData.A_Length != 2 && APData.A_Length != 6 )  //�˴� || ��Ϊ &&   sk81
         {
            Respone_NRC(SEVER_SID_SECURITY,NRC_IMLOIF,AID_PHYSIC);
         }
         else
         {         
            Respone_NRC(SEVER_SID_SECURITY,NRC_CNC1,AID_PHYSIC);  //Ԥ��ֵΪ����������
         }
         break;
      case SECURITY_REQSD_LEV3:
      case SECURITY_SENDK_LEV3:
         Respone_NRC(SEVER_SID_SECURITY,NRC_SFNS,AID_PHYSIC);
         break;
      case  SECURITY_REQSD_LEV1:
         if (SessionMode == SESSION_MODE_EXTERN)
         {  //��������1,ֻ֧����extern mode
            if(APData.A_Length == 2)
            {
               if (SecurityState == SECURITY_STATE_TIMELOCK)
               {  //ʱ������
                  Respone_NRC(SEVER_SID_SECURITY,NRC_RTDNE,AID_PHYSIC);     
               }
               else 
               {
                  //�϶���Ӧ
                  Respone_POS_SendSeed(SubFunction); 
               }
            }
            else
            {  //incorrect lenth   
               Respone_NRC(SEVER_SID_SECURITY,NRC_IMLOIF,AID_PHYSIC);      
            }
         }
         else
         {  //subfunction not support in active session
            Respone_NRC(SEVER_SID_SECURITY,NRC_SFNSIAS,AID_PHYSIC);      
         }
         break;
      case SECURITY_SENDK_LEV1:
         if (SessionMode == SESSION_MODE_EXTERN)
         {  //send key1,ֻ֧����extern mode
            if(APData.A_Length == 6)
            {
               if (SecurityState == SECURITY_STATE_TIMELOCK)
               {  //ʱ������
                  Respone_NRC(SEVER_SID_SECURITY,NRC_RTDNE,AID_PHYSIC);     
               }
               else 
               {        
                  if (SecurityState == SECURITY_STATE_SSEED1)
                  {//already send seed, else send nrc  requestSequenceError
                     nKey =  APData.A_Data[1];
                     nKey <<= 8;
                     nKey += APData.A_Data[2];
                     nKey <<= 8;
                     nKey += APData.A_Data[3];
                     nKey <<= 8;
                     nKey += APData.A_Data[4];
                     if ( CheckKeyBySeed(nKey, nSeed, 1))
                     { //�ж���Կ�Ƿ���ȷ, ��ȷ����������
                        SecurityState = SECURITY_STATE_LEVEL1;
                        SecurityErrTimes = 0;
                        Respone_POS_2(SEVER_SID_SECURITY,SECURITY_SENDK_LEV1,AID_PHYSIC);
                     }
                     else
                     {  //invaild key
                        SecurityErrTimes++;
                        if (SecurityErrTimes >=2)
                        {  //if err twice into delay process
                           SecurityErrTimes =2;
                           (void)TimeOutChkTenthSLong(&TimerSecurity, 0);
                           SecurityState =  SECURITY_STATE_TIMELOCK;
                           Respone_NRC(SEVER_SID_SECURITY,NRC_ENOA,AID_PHYSIC);  
                        }
                        else
                        {  //invaild key nrc 
                           SecurityState =  SECURITY_STATE_LOCK;
                           Respone_NRC(SEVER_SID_SECURITY,NRC_IK,AID_PHYSIC);  
                        }
                     }
                  }
                  else
                  {  //else send nrc  requestSequenceError
                     Respone_NRC(SEVER_SID_SECURITY,NRC_RSE,AID_PHYSIC);      
                  }
               }
            }
            else
            {  //incorrect lenth   
               Respone_NRC(SEVER_SID_SECURITY,NRC_IMLOIF,AID_PHYSIC);      
            }
         }        
         else
         {  //subfunction not support in active session
            Respone_NRC(SEVER_SID_SECURITY,NRC_SFNSIAS,AID_PHYSIC);      
         }
         break;
      case  SECURITY_REQSD_LEV2:
         if (SessionMode == SESSION_MODE_PROGRAM)
         {  //��������2,ֻ֧����program mode
            if(APData.A_Length == 2)
            {
               if (SecurityState == SECURITY_STATE_TIMELOCK)
               {  //ʱ������
                  Respone_NRC(SEVER_SID_SECURITY,NRC_RTDNE,AID_PHYSIC);     
               }
               else 
               {
                  //�϶���Ӧ
                  Respone_POS_SendSeed(SubFunction); 
               }
            }
            else
            {  //incorrect lenth   
               Respone_NRC(SEVER_SID_SECURITY,NRC_IMLOIF,AID_PHYSIC);      
            }
         }
         else
         {  //subfunction not support in active session
            Respone_NRC(SEVER_SID_SECURITY,NRC_SFNSIAS,AID_PHYSIC);      
         }
         break;
      case SECURITY_SENDK_LEV2:
         if (SessionMode == SESSION_MODE_PROGRAM)
         {  //send key2,ֻ֧����program mode
            if(APData.A_Length == 6)
            {
               if (SecurityState == SECURITY_STATE_TIMELOCK)
               {  //ʱ������
                  Respone_NRC(SEVER_SID_SECURITY,NRC_RTDNE,AID_PHYSIC);     
               }
               else 
               {
                  if (SecurityState == SECURITY_STATE_SSEED2)
                  {//already send seed, else send nrc  requestSequenceError
                     nKey =  APData.A_Data[1];
                     nKey <<= 8;
                     nKey += APData.A_Data[2];
                     nKey <<= 8;
                     nKey += APData.A_Data[3];
                     nKey <<= 8;
                     nKey += APData.A_Data[4];
                     if (CheckKeyBySeed(nKey, nSeed, 2))
                     { //�ж���Կ�Ƿ���ȷ, ��ȷ����������
                        SecurityState = SECURITY_STATE_LEVEL2;
                        SecurityErrTimes = 0;
                        Respone_POS_2(SEVER_SID_SECURITY,SECURITY_SENDK_LEV2,AID_PHYSIC);
                     }
                     else
                     {  //invaild key
                        SecurityErrTimes++;
                        if (SecurityErrTimes >=2)
                        {  //if err twice into delay process
                           SecurityErrTimes =2;
                           (void)TimeOutChkTenthSLong(&TimerSecurity, 0);
                           SecurityState =  SECURITY_STATE_TIMELOCK;
                           Respone_NRC(SEVER_SID_SECURITY,NRC_ENOA,AID_PHYSIC);  
                        }
                        else
                        {  //invaild key nrc 
                           SecurityState =  SECURITY_STATE_LOCK;
                           Respone_NRC(SEVER_SID_SECURITY,NRC_IK,AID_PHYSIC);  
                        }
                     }
                  }
                  else
                  {  //else send nrc  requestSequenceError
                     Respone_NRC(SEVER_SID_SECURITY,NRC_RSE,AID_PHYSIC);      
                  }
               }
            }
            else
            {  //incorrect lenth   
               Respone_NRC(SEVER_SID_SECURITY,NRC_IMLOIF,AID_PHYSIC);      
            }
         }        
         else
         {  //subfunction not support in active session
            Respone_NRC(SEVER_SID_SECURITY,NRC_SFNSIAS,AID_PHYSIC);      
         }
         break;
   }
}
/**************************************************************
�������Ӧ�ò���ʾ��
F_N_USData_IND       ָʾ
F_N_USData_FF_IND    ��ָ֡ʾ
F_N_USData_CON       confirm                                 
*********************************************************************/

/*******************************************************************************
 * Function:      void  F_N_USData_IND(N_USDATA_IND*  NetInd)
 *
 * Description:    ������յ� 1������
 *
 * Returns:         none
 *
 * Notes:          �յ����񲢽��д���
********************************************************************************/
void  F_N_USData_IND(N_USDATA_IND*  NetInd)
{
   uint8 DTCAvailMask,dtc_i;
   uint8 u8lData;
   uint32 u32lData;
   uint16 u16WDID;
 //  uint8 SubFunction;
 //  uint16 Addr;
   if(NetInd->N_Result==N_Success)
   {
      //�ɹ����ܵ�1��֡ӦֹͣP3 ֱ����������ɲ��ܿ�ʼ��
      (void)TimeOutChkMsLong(&TimerP3Sever,0) ;
      bDiaging = 1;
      //P3SeverStop=1;    //������������¿�ʼ���Բ���ֹͣ
      APData.A_AI=NetInd->nId;
      APData.A_PCI=NetInd->Data[0];
      APData.A_Length=NetInd->Length;
      if(APData.A_Length<1) return;
      MemCopy(APData.A_Data, &NetInd->Data[1], (uint8)(APData.A_Length-1));
      //begin p2can_server
      (void)TimeOutChkMsLong(&TimerP2Can,0);
      //AppState=APP_STATE_RESPONE;    
   
      if(AppState==APP_STATE_RESPONE||AppState==APP_STATE_DEALREQUEST||AppState==APP_STATE_IDLE) 
      {
         //�ж�ϵͳ״̬
         //������ʲô״̬����Ҫ�������
      }
      else
      {
       #ifdef _DEBUG_
         Test_Flg=2;
       #endif
      }
      //SubFunction = (APData.A_Data[0]&0x7f);
      switch(APData.A_PCI) 
      {
         case  SEVER_SID_SESSIONCONTROL:
            //session control begin
            if(APData.A_Length==2)
            {                
               switch(APData.A_Data[0]&0x7f)
               {
                  default:
                     Respone_NRC(SEVER_SID_SESSIONCONTROL,NRC_CNC1,NetInd->nId);

                     break;
                  case  SESSIONCONTROL_TYPE_DS:
                  // SessionMode
                     Respone_POS_SessionC(SEVER_SID_SESSIONCONTROL,APData.A_Data[0],NetInd->nId);
                     SessionMode = SESSION_MODE_DEFAULT;
                     CCState.Byte = 0;
                     StorState = 0;
                     if (SecurityState != SECURITY_STATE_TIMELOCK)
                     {
                     	SecurityState = SECURITY_STATE_LOCK;
                     }
                     break;
                  case  SESSIONCONTROL_TYPE_PRGS:
                  	if (Flash_uint32_read(CODE_END_ADDR-4) == 0xffffffff)
                  	{//û��BT
                  		Respone_NRC(SEVER_SID_SESSIONCONTROL, NRC_SFNS, NetInd->nId);
                  	}
                  	else if (bBootPrepare == 0 && FUN_NEED_PRECONDITIONS)
                  	{
                  		Respone_NRC(SEVER_SID_SESSIONCONTROL, NRC_CNC, NetInd->nId);
                  	}
                  	else
                  	{     	
                  		Send_78NRC(SEVER_SID_SESSIONCONTROL); 
                  		ApplUDS_GotoProgramMode();
                  	}
                     break;
                  case   SESSIONCONTROL_TYPE_EXTDSE:
                     if(SessionMode == SESSION_MODE_PROGRAM) 
                     {// program mode can't  to extern
                        Respone_NRC(SEVER_SID_SESSIONCONTROL,NRC_SFNSIAS,NetInd->nId); //sk81
                     }
                     else
                     {
                        Respone_POS_SessionC(SEVER_SID_SESSIONCONTROL,APData.A_Data[0],NetInd->nId);
  
                        if (SecurityState != SECURITY_STATE_TIMELOCK)
                        {
                        	SecurityState = SECURITY_STATE_LOCK;
                        }
    
                        SessionMode = SESSION_MODE_EXTERN;
                        //p3 begin timer
                        (void)TimeOutChkMsLong(&TimerP3Sever,0);
                        P3SeverStop=0;
                     }
                     break;
                  case   SESSIONCONTROL_TYPE_EOLDS:
                     Respone_NRC(SEVER_SID_SESSIONCONTROL,NRC_SFNS,NetInd->nId);

                     break;
                  case   0:
                     Respone_NRC(SEVER_SID_SESSIONCONTROL,NRC_SFNS,NetInd->nId);

                     break;
                        
               }
            }
            else
            {
               //����Ѱַ��Ҫ��Ӧ����Ӧ��
               //Respone 0x
               Respone_NRC(SEVER_SID_SESSIONCONTROL,NRC_IMLOIF,NetInd->nId);
            }
            break;
       /***************session control end********************************/
         case  SEVER_SID_RESET:
         // ��λ����
            if(APData.A_Length==2)
            {
               switch(APData.A_Data[0]&0x7f)
               {
                  default:
                     Respone_NRC(SEVER_SID_RESET,NRC_CNC1,NetInd->nId); 
                     break;
                  case RESET_TYPE_HARDRESET:
							#if JAC_UDS
								Respone_POS_2(SEVER_SID_RESET,APData.A_Data[0],APData.A_AI);
								UDS_ResetState = 1;						
							#else
                  	if (APData.A_AI == AID_FUNIC )  
                     {  //����Ѱַ ����Ҫ��ȫ�ȼ�
                        Respone_POS_2(SEVER_SID_RESET,APData.A_Data[0],APData.A_AI);
                        UDS_ResetState = 1;
                     }
                     else
                     {
                        if(SessionMode == SESSION_MODE_DEFAULT)
                        {
                        	Respone_NRC(SEVER_SID_RESET,NRC_SNSIAS,NetInd->nId);
                        }else
                        {
                        	if (SecurityState == SECURITY_STATE_LEVEL1 || SecurityState == SECURITY_STATE_LEVEL2)
                           {
                              Respone_POS_2(SEVER_SID_RESET,APData.A_Data[0],APData.A_AI);
                              UDS_ResetState = 1;
                           }
                           else
                           {  //��ȫ�ȼ�������
                              Respone_NRC(SEVER_SID_RESET,NRC_SAD,NetInd->nId);
                           }
                        }
                     }
							#endif
                  	
                  	if ((APData.A_Data[0] & 0x80) && UDS_ResetState == 1)
                  	{//����
                  		N_USDATA_CON   NetCon;
                  		NetCon.N_Result = N_Success;
                  		F_N_USData_CON(NetCon);
                  	}
                  	else
                  	{			
                  	}
                     break;
                  case RESET_TYPE_KOFFONR:
                  case RESET_TYPE_SOFTRESET:
                     Respone_NRC(SEVER_SID_RESET,NRC_SFNS,NetInd->nId);
                     break;
               }
            }
            else
            {
               Respone_NRC(SEVER_SID_RESET,NRC_IMLOIF,NetInd->nId);   
            }
            break;
        /***************reset sever end********************************/  
         case  SEVER_SID_SECURITY:
            Deal_SecuritySever();
            break;
        /***************security sever end********************************/
         case  SEVER_SID_TP:
         //tp begin   test present
            if(APData.A_Length==2)
            {
               if( (APData.A_Data[0] & 0x7f) != 0)
               {  //�˹���û���ӹ��� ������
                  Respone_NRC(SEVER_SID_TP,NRC_SFNS,NetInd->nId);   
               }
               else
               { //�˹���û���ӹ��� �����Ƿ���ʱ��Ҫ2���ֽڵĿ϶���Ӧ 
                  Respone_POS_2(SEVER_SID_TP, APData.A_Data[0], APData.A_AI);
                  (void)TimeOutChkMsLong(&TimerP3Sever,0);  //20191204 ��Ӧ��5000����0
               }
               
            }
            else
            {   //�˹���û���ӹ��� ������
               Respone_NRC(SEVER_SID_TP,NRC_IMLOIF,NetInd->nId);
            }
            break;
         //tp end
         case  SEVER_SID_CLEARDIAGINF:
         //clear diagnostic info
            if(APData.A_Length==4)
            {     
            	u32lData = ( ((uint32)APData.A_Data[0]<<16) +  ((uint32)APData.A_Data[1]<<8) + APData.A_Data[2] );
            	u8lData = SearchNum_DTCMap(u32lData);
            	if ( (u8lData != 0xff)||
            			(u32lData == 0X00ffffff)) 
               {               
                  Respone_POS_nSub(SEVER_SID_CLEARDIAGINF,APData.A_AI);
                  if (u8lData < DTC_UDSNUM) 
                  {
                     ClearDTC(u8lData);
                  }
                  else
                  {
                     ClearDTC(0xff);
                  }
               //   Save_DTC();  should add
               }
               else
               {
                  Respone_NRC(SEVER_SID_CLEARDIAGINF,NRC_ROOR,APData.A_AI); 
               }
            }
            else
            {//���ȴ����id��û���ӹ��ܵķ�������Ѱַ�͹���Ѱַ����Ҫ���ͷ񶨣���
               Respone_NRC(SEVER_SID_CLEARDIAGINF,NRC_IMLOIF,APData.A_AI);
            }
            break;
         case  SEVER_SID_COMMUNICATIONCONTROL:
         //conmunication control sever
            if(SessionMode != SESSION_MODE_EXTERN)
            {  //��Ӧ����Ӧ��0x7f�������Ӧ�������ַ��
               Respone_NRC(SEVER_SID_COMMUNICATIONCONTROL,NRC_SNSIAS,APData.A_AI); 
               return;
            }
            if(APData.A_Length==3)
            {
               switch(APData.A_Data[0]&0x7f)
               {
                  case CC_TYPE_ERXTX:
                      
                     if(APData.A_Data[1]<4 && APData.A_Data[1])
                     {
                        switch(APData.A_Data[1])
                        {
                           case  1: 
                              CCState.Bits.st_ncm=0;
                              break;
                           case  2:
                              CCState.Bits.st_nwmcm=0;
                              break;
                           case  3:
                              CCState.Bits.st_nwmcm=0;
                              CCState.Bits.st_ncm=0;
                              break;
                        }
                        Respone_POS_2(SEVER_SID_COMMUNICATIONCONTROL,APData.A_Data[0],APData.A_AI);
                     }
                     else
                     {
                        Respone_NRC(SEVER_SID_COMMUNICATIONCONTROL,NRC_ROOR,APData.A_AI); 
                     }
                     break;
                  case  CC_TYPE_DRXTX:       
                     if(APData.A_Data[1]<4 && APData.A_Data[1])
                     {
                        switch(APData.A_Data[1]) 
                        {
                           case  1: 
                              CCState.Bits.st_ncm=1;
                              break;
                           case  2:
                              CCState.Bits.st_nwmcm=1;
                              break;
                           case  3:
                              CCState.Bits.st_nwmcm=1;
                              CCState.Bits.st_ncm=1;
                              break;
                        }
                        Respone_POS_2(SEVER_SID_COMMUNICATIONCONTROL,APData.A_Data[0],APData.A_AI); 
                     }
                     else
                     {
                        Respone_NRC(SEVER_SID_COMMUNICATIONCONTROL,NRC_ROOR,APData.A_AI); 
                     }
                     break; 
                  case  CC_TYPE_ERXDTX:
                  case  CC_TYPE_DRXETX:
                     Respone_NRC(SEVER_SID_COMMUNICATIONCONTROL,NRC_SFNS,APData.A_AI);
                    
                     break;
                  default:
                     Respone_NRC(SEVER_SID_COMMUNICATIONCONTROL,NRC_SFNS,APData.A_AI);
                     break;
               }
            }
            else
            {
               Respone_NRC(SEVER_SID_COMMUNICATIONCONTROL,NRC_IMLOIF,APData.A_AI);
            }
            break;
        //conmunication control sever end
         case SEVER_SID_READDATABYINF:
         // read data by info
            Deal_ReadDID();
            break;             
         // write data by info
         case SEVER_SID_WRITEDATABYINF:
            Deal_WriteDID();
            break;
         // write data by info   end
         case SEVER_SID_IOCONTROL:
            Deal_IOControl();
            break;         
         case SEVER_SID_WRITEBYADDR:
            Respone_NRC(APData.A_PCI,NRC_SNS,APData.A_AI);
            break;
         case SEVER_SID_READBYADDR:
            Deal_ReadAddr();
            break; 
         //IO CONTROL  end
         case  SEVER_SID_DTCCONTROL:
         //dtc control begine
            if(SessionMode != SESSION_MODE_EXTERN)
            {  //��Ӧ����Ӧ��0x22  ,����������
               Respone_NRC(SEVER_SID_DTCCONTROL,NRC_SNSIAS,APData.A_AI); 
               return;
            }
            if(APData.A_Length==2)//if(APData.A_Length>=5)
            {
               switch(APData.A_Data[0]&0x7f)
               {
                  case DTCCONTROL_TYPE_STORON:
                     Respone_POS_2(SEVER_SID_DTCCONTROL,APData.A_Data[0],APData.A_AI); 
                     StorState=0;
                     
                     break;
                  case  DTCCONTROL_TYPE_STOROFF:
                     Respone_POS_2(SEVER_SID_DTCCONTROL,APData.A_Data[0],APData.A_AI); 
                     StorState=1;
                     break; 
                  default:
                     Respone_NRC(SEVER_SID_DTCCONTROL,NRC_SFNS,APData.A_AI);
                     break;
               }
            }
            else
            {
               Respone_NRC(SEVER_SID_DTCCONTROL,NRC_IMLOIF,APData.A_AI);
            }
            break;
            //dtc control end
          case  SEVER_SID_READDTC :
            //read   dtc  begin
            DTCAvailMask=SUPPORT_DTC&(APData.A_Data[1]);//
            switch(APData.A_Data[0]&0x7f)
            {
               case READDTC_SID_NUM:
                  if (APData.A_Length == 3)  
                  {
                     Respone_POS_DtcNum(APData.A_Data[0],APData.A_AI,DTCAvailMask); 
                  }
                  else
                  {
                     Respone_NRC(SEVER_SID_READDTC,NRC_IMLOIF,APData.A_AI);
                  }
                  break;
               case  READDTC_SID_READ:
                  if (APData.A_Length == 3)
                  {
                     Respone_POS_DtcRead(APData.A_Data[0],APData.A_AI,DTCAvailMask); 
                  }
                  else
                  {
                     Respone_NRC(SEVER_SID_READDTC,NRC_IMLOIF,APData.A_AI);
                  }
                  break; 
               case  READDTC_SID_ALLREAD:
                  if (APData.A_Length == 2)
                  {
                  	Respone_POS_DtcRead(APData.A_Data[0],APData.A_AI,0xff);  
                  }
                  else
                  {
                     Respone_NRC(SEVER_SID_READDTC,NRC_IMLOIF,APData.A_AI);
                  }
                  break; 
               case READDTC_SID_SNAPSHORT:
                  if (APData.A_Length == 6)
                  {
                  	dtc_i = SearchNum_DTCMap((uint32)APData.A_Data[1]*65536 + 
                  			(uint32)APData.A_Data[2]*256 + APData.A_Data[3]);
                  	
                     if (dtc_i < DTC_UDSNUM && APData.A_Data[4] == 01)
                     {
                     	Respone_POS_SnapShort(dtc_i); 
                     }
                     else
                     {  //���෶Χ����
                        Respone_NRC(SEVER_SID_READDTC,NRC_ROOR,APData.A_AI);
                     }
                  }
                  else
                  {
                     Respone_NRC(SEVER_SID_READDTC,NRC_IMLOIF,APData.A_AI);
                  }
                  break;
               default:
               	if (APData.A_Length < 2)
               	{
               		Respone_NRC(SEVER_SID_READDTC,NRC_IMLOIF,APData.A_AI);
               	}
               	else 
               	{
               		Respone_NRC(SEVER_SID_READDTC,NRC_SFNS,APData.A_AI);
               	}
               	break;
            }

            break;
           //read   dtc  end
         case  SEVER_SID_RC :
            if(SessionMode != SESSION_MODE_EXTERN)  //SK81
            {  //��Ӧ����Ӧ��0x22  ,����������
               Respone_NRC(SEVER_SID_RC, NRC_SNSIAS, APData.A_AI); 
               return;
            }
            u16WDID = ((uint16)APData.A_Data[1]<<8)+APData.A_Data[2];
            if (u16WDID == ROUTINE_ID_PRECONDITIONS)
            {//�������Ƿ����ˢд
            	bBootPrepare = 1;
            	Respone_POS_RC(APData.A_Data[0], u16WDID, 0);
            	return;
            }
            if (SecurityState == SECURITY_STATE_LEVEL1)    //sk81
            {
               switch (u16WDID)
               {// APData.A_Data[0]   control type 
                  default:
                  	if (APData.A_Length <= 3)
                  	{
                  		Respone_NRC(SEVER_SID_RC, NRC_IMLOIF, APData.A_AI); 
                  	}
                  	else
                  	{
                  		Respone_NRC(SEVER_SID_RC, NRC_ROOR, APData.A_AI); 
                  	}
                     break;
                 /* case ROUTINE_ID_ERASE: 
                  //����
                     switch(APData.A_Data[0])
                     {
                        case RC_SID_START:
                           if (APData.A_Length == 13)
                           {
                              Addr =  ((uint16)APData.A_Data[6]<<8) + APData.A_Data[7] ;
 
                              if (APData.A_Data[4] == 0 && APData.A_Data[5] == 0 &&
                                  (Addr >= CODE_START_ADDR && Addr < CODE_END_ADDR)  )
                              { 
                                 //EraseFlash((uint8*)Addr);
                                 (void)Load_ProgramData();
                                 ProgramData[PD_NUM_INTEGRITY] = 0xff;
                                 ProgramData[PD_NUM_DEPENDCE] = 0xff;
                                 ProgramData[PD_NUM_SESSIONMODE] = 2;
                                 
                                 Save_ProgramData();
                                 Run_BootLoader();
                                // Respone_POS_RC(RC_SID_START, ROUTINE_ID_ERASE, 0);
                              }
                              else
                              {
                                 Respone_NRC(SEVER_SID_RC, NRC_ROOR, AID_PHYSIC);   
                              }
                           }
                           else
                           {
                              Respone_NRC(SEVER_SID_RC, NRC_IMLOIF, AID_PHYSIC);   
                           }

                           break;
                        case RC_SID_STOP:
                        case RC_SID_RESULT:    
                           Respone_NRC(SEVER_SID_RC, NRC_RSE, AID_PHYSIC);   
                           
                           break;
                        default:
                           Respone_NRC(SEVER_SID_RC, NRC_ROOR, AID_PHYSIC); 
                     }
                     break;*/    //sk81 ��ģʽ���ܲ�д ����FF01 ���ܣ�
                  case ROUTINE_ID_INTEGRITY:
                  case ROUTINE_ID_PRECONDITIONS:
                  case ROUTINE_ID_DEPENDENCE:
                     //������
                     if (APData.A_Length == 4)
                     {
                        switch(APData.A_Data[0])
                        {
                           case RC_SID_START:
                               //Deal_ProgramIntegrity();

                           case RC_SID_STOP:
                           case RC_SID_RESULT:
                               Respone_POS_RC(APData.A_Data[0], u16WDID, 0);
                               break;
                           default:
                               Respone_NRC(SEVER_SID_RC, NRC_ROOR, APData.A_AI);  
                               break; 
                        }
                     }
                     else
                     {
                        Respone_NRC(SEVER_SID_RC, NRC_IMLOIF, APData.A_AI);   
                     }
                     break; 
               }
            }
            else
            {  //��ȫģʽ����ȷ
               Respone_NRC(SEVER_SID_RC, NRC_SAD, APData.A_AI);
            }
            break;
         case SEVER_SID_REQDOWNLOAD:
            Respone_NRC(SEVER_SID_REQDOWNLOAD, NRC_CNC, APData.A_AI);
            break;
            
         case SEVER_SID_TRANSDATA:
            Respone_NRC(SEVER_SID_TRANSDATA, NRC_CNC, APData.A_AI);
            break;
         case SEVER_SID_EXITTRANS:  
            Respone_NRC(SEVER_SID_TRANSDATA, NRC_CNC, APData.A_AI);
            break;  
         default:
            //��֧�ִ�sid
            Respone_NRC(APData.A_PCI,NRC_SNS,APData.A_AI);
            break;    
      }
     

   }
   else
   {//result not success
      switch(NetInd->N_Result)
      {
         default:
            break;
         case  N_Timeout_A:

         case  N_Timeout_Cr:
         //rec err
         case  N_Wrong_SN:
          //rec err ,start s3 
            (void)TimeOutChkMsLong(&TimerP3Sever,0) ;
            P3SeverStop=0;
            AppState= APP_STATE_IDLE;
            break;  
      }
   }

}

void  F_N_USData_FF_IND(uint8 nId,uint8 Length)
{
   if(AppState==APP_STATE_IDLE)
   {  //stop p3 sever
      P3SeverStop=1; 
   }
}


/*******************************************************************************
 * Function:        F_N_USData_CON(N_USDATA_CON   NetCon)
 *
 * Description:     ����㷢��һ֡��ȥ�����ص�״̬
 *
 * Returns:        none   
 *
 *
 * Notes:          NetCon ��״̬ Ϊ ���¡���λ����������ʵ��		
 *
********************************************************************************/

void  F_N_USData_CON(N_USDATA_CON   NetCon)
{
   switch(NetCon.N_Result)
   {
      default:
         break;
      case  N_Buffer_Overflow:
      case  N_WFT_OVRN:
      case  N_Invalid_FS:
      case  N_Timeout_Bs:
      //���ͳ��ֹ������¿�ʼ��ʱ
            (void)TimeOutChkMsLong(&TimerP3Sever,0) ;
            P3SeverStop=0;
            AppState= APP_STATE_IDLE;
            break;
      case  N_Success:
            //����������¶�ʱ
            (void)TimeOutChkMsLong(&TimerP3Sever,0) ;
            P3SeverStop=0;
            AppState= APP_STATE_IDLE;
            //��λ��ش���
            if (UDS_ResetState)
            {  //hardware reset use watchdog out reset
               UDS_ResetState = 0;
              
               ApplUDS_Reset();
               while (1)
               {
                  MCU_NOP;
               }
            }
            break;
      
   }
}

/*******************************************************************************
 * Function:       void  AppLay_DiagControl(void)
 *
 * Description:     CAN��������� ���ܵ�һ���źţ�������
 *
 * Returns:         none
 *
 * Notes:           �����֡()�յ�ʱ,����		
 *
********************************************************************************/
void  AppLay_DiagControl(void)
{

   switch(AppState)
   {
      case  APP_STATE_IDLE:
         if(TimeOutChkMsLong(&TimerP3Sever,S3_Sever) && !P3SeverStop)
         {
            if (SessionMode != SESSION_MODE_DEFAULT)
            {
					SessionMode=SESSION_MODE_DEFAULT;      //out udefault session
					CCState.Byte=0;
					StorState=0; 
					bBootPrepare = 0;
					SecurityState = SECURITY_STATE_LOCK;
            }else{}
            bDiaging = 0;
         }
         break;
      case  APP_STATE_RESPONE:
         break;
      case  APP_STATE_DEALREQUEST:
         if(TimeOutChkMsLong(&TimerP2Can,P2CAN_Sever))
         {//p2 can sever timeout
            
         }
         break;
      default:
         break  ;
   }
   //��ȫ���� ��ʱ���ƴ���
   if (SecurityState == SECURITY_STATE_TIMELOCK)
   {
      if (TimeOutChkTenthSLong(&TimerSecurity, 100))
      {
         SecurityErrTimes--;
         SecurityState = SECURITY_STATE_LOCK;      
      }
   }
   
   if (PosRespone_EE)
   {
      PosRespone_EE = 0;
      Respone_POS_SessionC(SEVER_SID_SESSIONCONTROL,SESSIONCONTROL_TYPE_DS,AID_PHYSIC);  
   }
   
}


/*******************************************************************************
 * Function:      uint8 ComControlEnGet(void)      uint8 DTCStorGet(void)
 *
 * Description:    ��Ϸ������Ӧ�ò���շ�����ϵͳ����
 *
 * Returns:         none
 *
 * Notes:           CAN ����ʹ��   ��dtc ��λʹ��	
 *
********************************************************************************/
uint8 ComControlEnGet(void)
{
   if(CCState.Bits.st_ncm)
   {   //
      return FALSE;
   }
   else
      return   TRUE;   //
}

uint8 NmControlEnGet(void)
{
   if(CCState.Bits.st_nwmcm)
   {   //
      return FALSE;
   }
   else
      return   TRUE;   //
}
/*******************************************************************************
 * Function:    uint8 DTCStorGet(void)
 *
 * Description:    ��Ϸ������Ӧ�ò���շ�����ϵͳ����
 *
 * Returns:         none
 *
 * Notes:          dtc ��λʹ��	
 *
********************************************************************************/
uint8 DTCStorGet(void)
{
   return !StorState; 
}


/*******************************************************************************
 * Function:      void Diagnostic_IgnReset(void)    
 *
 * Description:    ��Ϸ�����������ϵ�ʱ����ش���ϵͳIGN reset����
 *
 * Returns:         none
 *
 * Notes:      SessionMode = default ;CCState.Byte=0;  StorState=0; 	
 *             if SecurityTimes >=1 -> securitystate = timelock 
********************************************************************************/
void Diagnostic_IgnReset(void)  
{
   SessionMode = SESSION_MODE_DEFAULT;
   CCState.Byte=0;  
   StorState=0; 
   bBootPrepare = 0;
   
   if(SecurityErrTimes)
   {
      SecurityErrTimes = 1;
      SecurityState = SECURITY_STATE_TIMELOCK;   
   }
   else
   {
      SecurityState = SECURITY_STATE_LOCK;    
   }
}

BOOL Diag_GetIsDiag(void)
{
	return bDiaging;
}

//��һ�ν���PROGRAM SESSION ��λ�������Ҫ��Ӧ10 ����
void Enter_DefaultSession(void)
{
 //  SecurityState = SECURITY_STATE_LOCK;  
 //  SessionMode = SESSION_MODE_PROGRAM;
 //  SecurityErrTimes = 0;
   PosRespone_EE = 1;
}
//get current scurity times
uint8 UDS_GetSecuriTimes(void)
{
	return SecurityErrTimes;
}

void UDS_SetSecuriTimes(uint8 u8Times)
{
	SecurityErrTimes = u8Times;
}

uint8 Diag_GetSession(void)
{
	uint8 u8ReMode;
	if (SessionMode == SESSION_MODE_DEFAULT)
	{
		u8ReMode = 4;
	}
	else 
	{
		u8ReMode = 5;
	}
	
	return u8ReMode;
}
