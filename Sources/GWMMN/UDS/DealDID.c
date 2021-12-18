/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ApplayLay.c
* ����������UDS��ϵ�Ӧ�ò������DID������ȡ��ʵ��
* 1  22 ��ȡDID ����
* 2  2E дDID
* 3  2F  IO���� 
* 4  23 д��ַ
* ���岻ͬ����Ҫ��Ҫ����ġ�
* �������� codewarrior IDE 11.1
* MCU˵������ģ��Ϊ�ϲ�Ӧ�ã���MCU �޹�
*/
/**************************���ļ�¼******************************************************
 * 
2019.1.17
ver0.1
1 ��ʼ�汾
2 ģ�����

 * 
2019.7.22
ver0.2
1 �����Զ���DID�Ĺ���

2019.10.25
ver0.3
1 Deal_GDID ��������ֱ�ӻ�31��

2020.5.21
ver0.4
���ߣ�xb
1 �汾�Ŷ�ȡDIDʱ  λBCD��
2 ����ȡSVN��
3 ��������DID(0xFE00) ��������

2020.9.27
ver0.5
���ߣ�xb
1 ���Ӷ�ECU�ͺŵĹ���

2021.8.23
ver0.6
���ߣ�xb
1 u32CheckSum = *(uint32*)(CODE_CHECK_ADDR-4); ��Ϊu32CheckSum = Flash_uint32_read(CODE_CHECK_ADDR-4);
*/

#include "ApplayLay.h"
#include "DIDList.h"

uint8  DidData[DID_MAXNUM] = 
{0};

#define IOCONTROL_RETURNTOECU  0x00
#define IOCONTROL_FRZEECURRENT 0x02
#define IOCONTROL_SHORTTERMADJUST 0x03




//�ⲿ���ú���������˵��
extern	A_PDU APData;
extern	uint8 AppState;           
extern	uint8 SessionMode;
extern 	uint8 SecurityState;  
extern const uint8_t PRODUCTTYPE[4];
extern void Respone_POS_IOControl(uint16 pid_num, uint8 *tbl, uint8 Length) ;
extern void Respone_NRC(uint8 SID,uint8 nrc_type,uint8 Target_Id);
extern void Respone_POS_WriteDid(uint16 pid_num);




uint8 BCD2HEX(uint8 bcd_data)    //BCDתΪHEX�ӳ���  
{ 
	uint8 temp; 
	temp=(bcd_data/16*10 + bcd_data%16); 
	return temp; 
} 


uint8 HEX2BCD(uint8 hex_data)    //HEXתΪBCD�ӳ���   
{ 
	uint8 temp; 
	temp=(hex_data/10*16 + hex_data%10); 
	return temp; 
} 
/******************************************************************************
* Function:   void Respone_Pos_DIDData(uint8 DIDNum)
* 
* Description:   �϶���ӦDID, ������֮ǰ�Ѿ�����DidData[]
*
* param1 ��   DIDNum  did �����ݸ���  �� ������ did��. 

* return :    none
* 
* Note  :   1 ���õ� DidData[0] =  APData.A_AI + 0x40
* 				2  DidData[1]  DidData[2] APData.data[0] APData.data[1]
* 				3 DIDData[3] ֮�������ݴ�����ʱ�Ѿ�������� 
* 				4  ��Ҫ���͵����ݾ�����������.
*
************************************************************************************/
void Respone_Pos_DIDData(uint8 DIDNum)
{
	DidData[0] = APData.A_PCI + 0x40;
	DidData[1] = APData.A_Data[0];
	DidData[2] = APData.A_Data[1];
	(void)F_N_USDATA_REQ(DidData,DIDNum + 3,0x11)  ;
   AppState=APP_STATE_RESPONE;
}

/******************************************************************************
* Function:    void Respone_POSReadAddr(uint16 u16Size)
* 
* Description:   �϶���ӦDID, ������֮ǰ�Ѿ�����DidData[]
*
* param1 ��      u16Length  �ܹ������ݳ���

* return :    none
* 
* Note  :   1 ���õ� DidData[0] =  APData.A_AI + 0x40
* 				2 ADDR �� u16Size �����ݷ��� DidData[1]����
* 				3  ��Ҫ���͵����ݾ�����������.
*
************************************************************************************/
void Respone_POSReadAddr(uint16 u16Length)
{
	(void)F_N_USDATA_REQ(DidData, u16Length, 0x11)  ;
   AppState=APP_STATE_RESPONE;
}
/************************************************************

SEVER_SID_READDATABYINF

0x22 ���� �е�DID �����ڴ��ļ���ʵ��
DidNum ��  0xF183 -- 0xF1AB
*&************************************************************/
BOOL Deal_GDID(uint16 DIDNumber)
{
   const PID_DATA *pPIDData = PID ;
   uint8 i = MAX_PID_NUM;
   BOOL bSearch;
   
   do
   {
      if(DIDNumber == pPIDData->PID_ID)
      {
         break;     //����whlie ��pPIDDataָ���˵���DIDNumber ��DID
      }
      pPIDData++;
   }while (--i);
   
   if (i)
   {//�ҵ�DID	
		MemCopy(&DidData[3], (uint8*)pPIDData->pData, pPIDData->PIDLength);
		Respone_Pos_DIDData(pPIDData->PIDLength);
		bSearch = TRUE;
   }
   else
   {//��֧�ֵ�DID
   //   Respone_NRC(SEVER_SID_READDATABYINF,NRC_ROOR,APData.A_AI);
      bSearch = FALSE;
   }
   return bSearch;
}   


BOOL Deal_WDID(uint16 DIDNumber)
{
   const PID_DATA *pPIDData = PID ;
   uint8 i = WRITE_PID_NUM;
   BOOL bSearch;
   do
   {
      if(DIDNumber == pPIDData->PID_ID)
      {
         break;     //����whlie ��pPIDDataָ���˵���DIDNumber ��DID
      }
      pPIDData++;
   }while (--i);
   
   if (i)
   {//�ҵ�DID	
		if (APData.A_Length == (3 + pPIDData->PIDLength))
		{
			if (SecurityState == SECURITY_STATE_LEVEL1)
			{  //security state level2
				// write data
				Send_78NRC(SEVER_SID_WRITEDATABYINF); 
				MemCopy((uint8*)pPIDData->pData, &APData.A_Data[2], pPIDData->PIDLength);
				DID_UpdataList();
				Respone_POS_WriteDid(DIDNumber);      
			}
			else
			{
				Respone_NRC(SEVER_SID_WRITEDATABYINF,NRC_SAD,APData.A_AI);      
			}
		}
		else
		{
			Respone_NRC(SEVER_SID_WRITEDATABYINF,NRC_IMLOIF,APData.A_AI);   
		}
		
		bSearch = TRUE;
   }
   else
   {//��֧�ֵ�DID
   	bSearch = FALSE;
   }
   
   return bSearch;
}

/*******************************************************************************
 * Function:        void Deal_ReadDID(uint16 DIDNumber)
 *
 * Description:       ReadDataByIdentifier(0x22) sever
 *
 * Returns:         none
 *
 * Notes:           none		
 *
********************************************************************************/
void Deal_ReadDID(void)
{
   volatile uint16 DidNum;
   uint32 u32CheckSum; 

  // if(APData.A_Length>1&& APData.A_Length<12&&
  //    APData.A_Length%2==1)  
   if(APData.A_Length == 3)   
   {
      //����pid,pid����,�˴�ֻ֧��1��id
      DidNum = ((uint16)APData.A_Data[0]<<8)+APData.A_Data[1] ;
      if (Deal_GDID(DidNum))
      { //�ҵ���did
      }
      else if (DidNum >= PID_F110 && DidNum <= PID_F110+15) 
      {
      	if (DID_ReadF110(&DidData[3], (uint8)(DidNum- PID_F110)))
      	{
      		Respone_Pos_DIDData(PID_F110_LENGTH);
      	}
      	else
      	{
      		Respone_NRC(SEVER_SID_READDATABYINF,NRC_ROOR,APData.A_AI);
      	}
      }
      else if (DidNum >= 0xE000 && DidNum <= 0xE00E)
      {
      	DidData[3] = DID_GetDidData( DidNum - 0xE000);
      	Respone_Pos_DIDData(1);
      }
      else
      {  //io did
         switch(DidNum)
         {
            default://������Ӧ
               Respone_NRC(SEVER_SID_READDATABYINF,NRC_ROOR,APData.A_AI);
               break;
            case  IODID_BUSCONFIG+1:
            case  IODID_BUSCONFIG+2:
            case  IODID_BUSCONFIG+3:
            	DidData[3] =  DID_GetCANBUSConfig(DidNum - IODID_BUSCONFIG);
            	Respone_Pos_DIDData(1);
            	break;
            case  IODID_SYSTEMCONTROL:

            	DidData[3] =  DID_IOGetSystemControl();
            	Respone_Pos_DIDData(1);
               break;
            case  DID_SYSTEMSTATE:

            	GetSystemData(&DidData[3]);
            	Respone_Pos_DIDData(CAILBRATION_RAMDATA_NUM);
            	break;
            case  DID_SYSTMESET:
            	GetSetData(&DidData[3]);
               Respone_Pos_DIDData(CAILBRATION_PARAMETER_NUM);	
            	break;
            case  DID_SYSTEMPARMETER:
            	
            	GetParameterData(&DidData[3]);
            	Respone_Pos_DIDData(CAILBRATION_PARAMETER_NUM);
            	break;
            case  DID_SYSTEMPARMETER1:
            	GetParameterData_Sec(&DidData[3]);
            	Respone_Pos_DIDData(CAILBRATION_PARAMETER_NUM);
            	break;
            case  DID_CHECKSUM:
            	u32CheckSum = Flash_uint32_read(CODE_CHECK_ADDR-4);
            	DidData[3] = (uint8)(u32CheckSum >>24);
            	DidData[4] = (uint8)((u32CheckSum &0x00ffffff) >>16);
            	DidData[5] = (uint8)((u32CheckSum &0x0000ffff) >>8);
            	DidData[6] = (uint8)(u32CheckSum &0x000000ff);
            	Respone_Pos_DIDData(4);
               break;
            case  ACTION_DIAG_SESSION:
            	DidData[3] = SessionMode;
            	Respone_Pos_DIDData(1);
               break;
            case  DID_SWCLIENT_VER:
            	DidData[3] = HEX2BCD(SwV_CLIENT);
            	DidData[4] = HEX2BCD(SwsV_CLIENT);
            	Respone_Pos_DIDData(4);
            	break;
            case  DID_SW_SVN_VER:
            	DidData[3] = HEX2BCD(SwV_);
            	DidData[4] = HEX2BCD(SwsV_);
			    	DidData[5] = (uint8)(SVN_LOG/256);
            	DidData[6] = (uint8)(SVN_LOG&0xff);
            	Respone_Pos_DIDData(4);
            	break;
            case  DID_HW_VER:
            	DidData[3] = HEX2BCD(HwV_);
            	DidData[4] = HEX2BCD(HwsV_);
            	Respone_Pos_DIDData(2);
            	break;
            case  DID_PRODUCTTYPE:
            	DidData[3] = PRODUCTTYPE[0];
            	DidData[4] = PRODUCTTYPE[1];
            	DidData[5] = PRODUCTTYPE[2];
            	DidData[6] = PRODUCTTYPE[3];
            	Respone_Pos_DIDData(4);
            	break;
         }
      }   
   }
   else
   {//���ȴ���
   #ifdef _DEBUG_
      if(Test_Ser)
      {
         SysErcode.Bits.b1=1;
      }
   #endif
      Respone_NRC(SEVER_SID_READDATABYINF,NRC_IMLOIF,APData.A_AI);
   }
}
   
/*******************************************************************************
 * Function:        void Deal_WriteDID(void)
 *
 * Description:      WriteDataByIdentifier(0x2e) sever
 *
 * Returns:         none
 *
 * Notes:           none		
 *
********************************************************************************/
void Deal_WriteDID(void)
{
   uint16 u16WDID;
	if (APData.A_AI != AID_PHYSIC) return ;//ֻ֧������Ѱַ
   if(SessionMode != SESSION_MODE_DEFAULT)   
   {  //һ������3
   	u16WDID = ((uint16)APData.A_Data[0]<<8)+APData.A_Data[1];
   	if (Deal_WDID(u16WDID))
   	{
   		
   	}
   	else
   	{
			switch (u16WDID) 
			{
				default:
					if (APData.A_Length <= 3)
					{
						Respone_NRC(SEVER_SID_WRITEDATABYINF, NRC_IMLOIF, APData.A_AI);
					}
					else
						Respone_NRC(SEVER_SID_WRITEDATABYINF, NRC_ROOR, APData.A_AI);
					break; 
				case  IODID_BUSCONFIG+1:
				case  IODID_BUSCONFIG+2:
				case  IODID_BUSCONFIG+3:
					if (APData.A_Length == (3 + 1))
					{
						if (SecurityState == SECURITY_STATE_LEVEL1)
						{  //security state level2
							// write data
							Send_78NRC(SEVER_SID_WRITEDATABYINF); 
							DID_SetCANBUSConfig(( ((uint16)APData.A_Data[0]<<8)+APData.A_Data[1] - IODID_BUSCONFIG) ,APData.A_Data[2]);
							Respone_POS_WriteDid( ((uint16)APData.A_Data[0]<<8)+APData.A_Data[1]) ;  
					  
						}
						else
						{
							Respone_NRC(SEVER_SID_WRITEDATABYINF,NRC_SAD,APData.A_AI);      
						}
					}
					else
					{
						Respone_NRC(SEVER_SID_WRITEDATABYINF,NRC_IMLOIF,APData.A_AI);   
					}
					break;
				case PID_F110:
					if (APData.A_Length == (3 + PID_F110_LENGTH))
					{
						if (SecurityState == SECURITY_STATE_LEVEL2)
						{  //security state level2
							// write data
							Send_78NRC(SEVER_SID_WRITEDATABYINF); 
							DID_WriteF110(&APData.A_Data[2]);
							Respone_POS_WriteDid(PID_F110);	
						}
						else
						{
							Respone_NRC(SEVER_SID_WRITEDATABYINF,NRC_SAD,APData.A_AI);      
						}
					}
					else
					{
						Respone_NRC(SEVER_SID_WRITEDATABYINF,NRC_IMLOIF,APData.A_AI);   
					}
					break;
				case DID_SYSTMESET:
					if (APData.A_Length == (CAILBRATION_PARAMETER_NUM + 3))
					{
						SetSetData(&APData.A_Data[2]);
						Respone_POS_WriteDid(DID_SYSTMESET);
					}
					else
					{
						Respone_NRC(SEVER_SID_WRITEDATABYINF,NRC_IMLOIF,APData.A_AI);   
					}
					break;
				case  DID_SYSTEMPARMETER:
					if (APData.A_Length == (CAILBRATION_PARAMETER_NUM + 3))
					{
						Send_78NRC(SEVER_SID_WRITEDATABYINF); 
						SetParameterData(&APData.A_Data[2]);
						Respone_POS_WriteDid(DID_SYSTEMPARMETER);
					}
					else
					{
						Respone_NRC(SEVER_SID_WRITEDATABYINF,NRC_IMLOIF,APData.A_AI);   
					}
					break;
				case  DID_SYSTEMPARMETER1:
					if (APData.A_Length == (CAILBRATION_PARAMETER_NUM + 3))
					{
						Send_78NRC(SEVER_SID_WRITEDATABYINF); 
						SetParameterData_Sec(&APData.A_Data[2]);
						Respone_POS_WriteDid(DID_SYSTEMPARMETER1);
					}
					else
					{
						Respone_NRC(SEVER_SID_WRITEDATABYINF,NRC_IMLOIF,APData.A_AI);   
					}
					break;
			}
   	}
   }
   else
   {
      Respone_NRC(SEVER_SID_WRITEDATABYINF,NRC_SNSIAS,APData.A_AI);
   }
}

/*******************************************************************************
 * Function:        void Deal_ReadDID(void)
 *
 * Description:       ReadDataByIdentifier(0x2f) sever
 *
 * Returns:         none
 *
 * Notes:           none		
 *
********************************************************************************/
void Deal_IOControl(void)
{  
   uint16 u16ID = ((uint16)APData.A_Data[0]<<8)+APData.A_Data[1];
   if(SessionMode != SESSION_MODE_DEFAULT)
   {
      if (SecurityState == SECURITY_STATE_LEVEL1)
      {  //security state level1
//�� λӳ�䣬����Ϊ 4 ��5�� λӳ�䳤�� 6��8
      	if ( (u16ID >= 0xe004 && u16ID <= 0xe006) 
      			|| u16ID == 0xe00d || u16ID == 0xe00e )
      	{
            if (APData.A_Data[2] == IOCONTROL_RETURNTOECU)
            {
               if (APData.A_Length == 4)
               {   // returntoecu and freeze  not use state
               	// 2f e0 01 00 
               	DidData[3] = IOCONTROL_RETURNTOECU;
               	DidData[4] = DID_SetDidData(u16ID-0xe000,0,IOCONTROL_RETURNTOECU);
               	Respone_Pos_DIDData(2);
               }
               else
               {
                  Respone_NRC(SEVER_SID_IOCONTROL,NRC_IMLOIF,APData.A_AI);
               }
            }
            else if (APData.A_Data[2] == IOCONTROL_SHORTTERMADJUST) 
            {
               if (APData.A_Length == 5)
               {// 2f e0 01 03 01(state)
               	DidData[3] = IOCONTROL_SHORTTERMADJUST;
               	DidData[4] = DID_SetDidData(u16ID-0xe000, APData.A_Data[3], IOCONTROL_SHORTTERMADJUST);
               	Respone_Pos_DIDData(2);
               }
               else
               {
                  Respone_NRC(SEVER_SID_IOCONTROL,NRC_IMLOIF,APData.A_AI);
               }  
            }
            else
            {
            	Respone_NRC(SEVER_SID_IOCONTROL,NRC_SFNS,APData.A_AI);
            }
      	}
      	else
      	{
            switch (u16ID)
            {
               default:
               	if (APData.A_Length <= 3)
               	{//20190912 ���� С��3�ֽ�Ӧ����13nrc
               		Respone_NRC(SEVER_SID_IOCONTROL, NRC_IMLOIF, APData.A_AI);
               	}
               	else
               	{
               		Respone_NRC(SEVER_SID_IOCONTROL, NRC_ROOR, APData.A_AI);
               	}
                  break; 
               case IODID_SYSTEMCONTROL:
                  if (APData.A_Data[2] == IOCONTROL_RETURNTOECU)
                  {
                     if (APData.A_Length == 6)
                     {   // returntoecu and freeze  not use state
                        //IO_Data[0] = IOCONTROL_RETURNTOECU;
                     	DidData[3] = IOCONTROL_RETURNTOECU;
                     	DidData[4] = DID_IOSetSystemControl(&APData.A_Data[3],0);
                     	DidData[5] = 0;
                     	Respone_Pos_DIDData(3);
                     	//  IO_Data[1] = EnOffAC_IOControl(IOCONTROL_RETURNTOECU, 0, APData.A_Data[3]);
                       // Respone_POS_IOControl(IODID_SYSTEMCONTROL,IO_Data,3);
                     }
                     else
                     {
                        Respone_NRC(SEVER_SID_IOCONTROL,NRC_IMLOIF,APData.A_AI);
                     }
                  }
                  else  if (APData.A_Data[2] == IOCONTROL_SHORTTERMADJUST) 
                  {//	2F D0 00 03 00 FF(yanma)
                     if (APData.A_Length == 8)
                     {
                     	DidData[3] = IOCONTROL_SHORTTERMADJUST;
                     	DidData[4] = DID_IOSetSystemControl(&APData.A_Data[3], IOCONTROL_SHORTTERMADJUST);
                     	DidData[5] = 0;
                     	Respone_Pos_DIDData(3);
                     }
                     else
                     {
                        Respone_NRC(SEVER_SID_IOCONTROL,NRC_IMLOIF,APData.A_AI);
                     }   
                  }
                  else
                  {  //�������Ʒ�ΧΪ������Χ
                     Respone_NRC(SEVER_SID_IOCONTROL,NRC_SFNS,APData.A_AI);
                  }
                  break;
            }
      	}    
      }
      else
      {
         Respone_NRC(SEVER_SID_IOCONTROL,NRC_SAD,APData.A_AI);      
      }

   }   
   else
   {
      Respone_NRC(SEVER_SID_IOCONTROL,NRC_SNSIAS,APData.A_AI);
   }
}


 /*******************************************************************************
 * Function:        void Deal_ReadDID(void)
 *
 * Description:       ReadDataByIdentifier(0x2f) sever
 *
 * Returns:         none
 *
 * Notes:           none		
 *
********************************************************************************/
void Deal_ReadAddr(void)
{
   uint32 nAddr, nSize;
  
   if (APData.A_AI != AID_PHYSIC) return ;//ֻ֧������Ѱַ
   if (SessionMode != SESSION_MODE_PROGRAM) // || SecurityState == SECURITY_STATE_LEVEL1)
   {//ֻ������ʹ��
      if (APData.A_Length == 8 && APData.A_Data[0] == 0x24)
      {//23 24 xxxx��addr�� xx(size)
         nAddr = ((uint32)APData.A_Data[1]<<24) + ((uint32)APData.A_Data[2]<<16) + ((uint32)APData.A_Data[3]<<8) + APData.A_Data[4];
         nSize = ((uint16)APData.A_Data[5]<<8) + APData.A_Data[6];
         DidData[0] = SEVER_SID_READBYADDR + 0x40;
         MemAddrCopy(&DidData[1], nAddr, nSize);
         Respone_POSReadAddr(nSize+1);     
      }
      else
      {
         Respone_NRC(SEVER_SID_READBYADDR,NRC_IMLOIF,APData.A_AI);
      }
   }
   else
   {
      if (SessionMode == SESSION_MODE_EXTERN)
      {  //��ȫģʽ������
         Respone_NRC(SEVER_SID_READBYADDR,NRC_SAD,APData.A_AI);
      }
      else
      {
         Respone_NRC(SEVER_SID_READBYADDR,NRC_SNSIAS,APData.A_AI);
      }
   }
}
