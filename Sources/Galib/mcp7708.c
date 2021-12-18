/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�mcp7708.c
* ����������7708 ����ģ�飬�������¹���
* 1  ���֧��2·IC, ���������
* 2  ��ʼ���õ��ͨ���󣬿�ֱ���������
* 3  �ɵ�·��������̵���
* 4  ��HLͬʱ�������
* 
* ��ģ��ʹ����ҪSPIģ��
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭģ���0.0.6��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��5��23��
* 
* 
*/
#include "mcp7708.h"


typedef struct
{
	 uint16 u16WriteData;           //spiд����
	 uint16 u16WriteDataLast;       //��һ��д����
	 uint16 u16ReadData;            //spi������
}MCP7708DATA;

static MCP7708DATA sMcp7708Data[MCP7708_MAXNUM];
static uint8 u8McpMotorNum;                                     //�ܹ��趨�ĵ������
static MCPComPinType TBL_McpMotorCOnfig[MCP7708_MAXNUM *6];     //���ͨ��������

//��ģ��ʵ�ֺ�������
static void Mcp_SPIOut(uint8 u8McpId);
static uint8 MCP_CombinSet(MCPComPinType u8Channel, uint8 u8Out);
//�ײ���������
extern void McpHW_CSSet(uint8 u8ICId, PinOutState BitSet);
/*******************************************************************************
 * Function:   BOOL Mcp_MotorInit(MCPMOTORCHANNEL* pSetChannel)
 *
 * Description:  mcp7708 ���ͨ������
 *  
 *  Parameter :  MCPMOTORCHANLE
 *
 * Returns:     ���óɹ�ΪTRUE ������ΪFALSE
 *
 * Notes:       ��Ҫ��������ú���ʱ���ô˺�����ʵ��	
 *
*******************************************************************************/
BOOL Mcp_MotorInit(uint8 u8SetMotorNum, MCPComPinType* pSetChannel)
{
	uint8 i;
	uint8 bInitMcpMotor = TRUE;
	if (u8McpMotorNum <= MCP7708_MAXNUM * 3)
	{
		for (i = 0; i <= u8SetMotorNum*2; i++)	
		{
			TBL_McpMotorCOnfig[i] = pSetChannel[i];
			if (TBL_McpMotorCOnfig[i] >= M7708_COMBINPIN1 && TBL_McpMotorCOnfig[i] <= M7708_COMBINPIN16)
			{
				
			}
			else
			{
				bInitMcpMotor = FALSE;
			}
		}
		
	}
	else
	{
		bInitMcpMotor = FALSE;
	}
	
	if (bInitMcpMotor)
	{
		u8McpMotorNum = u8SetMotorNum;
	}
	else
	{
		u8McpMotorNum = 0;
	}
	return bInitMcpMotor;
}


/*******************************************************************************
 * Function:   uint8 Mcp_SignalOut(MCPSinglePinType ncp,uint8 SetState)
 *
 * Description: ����ͨ������ʱʹ�ã���Ҫ�����������̵���
 *  
 *  Parameter :           ncp�� MCPSinglePinType  X7708_L1/X7708_H1/X7708_L2������������
 *                SetState��  MCP_SINGLESET_ON---����    ��  MCP_SINGLESET_OFF---�ر�     
 *
 * Returns:     �������õ�IC�ţ����0xff��Ϊʧ��
 *
 * Notes:       ������Ҫʱ���ÿ���ʱ���ô˺�����			
 *
*******************************************************************************/
uint8 Mcp_SignalOut(MCPSinglePinType ncp,uint8 SetState)
{
   uint8 u8ICNum; 
   uint8 u8ICChannel;
	uint16 u16SetMask;
	
	if(SetState > MCP_SINGLESET_ON)
	{
		u8ICNum = 0xff;
	}
	else
	{
	//
		if (ncp > X7708CH_H6)
		{
			u8ICNum = 1;
			u8ICChannel = ncp - X7708CH_H6;
		}
		else
		{
			u8ICNum = 0;
			u8ICChannel = ncp;
		}
		
		u16SetMask = 0x1;
		u16SetMask <<= u8ICChannel;
		if (SetState == MCP_SINGLESET_ON)
		{
			sMcp7708Data[u8ICNum].u16WriteData |= u16SetMask;
		}
		else
		{
			u16SetMask = ~u16SetMask;
			sMcp7708Data[u8ICNum].u16WriteData &= u16SetMask;
		}
		Mcp_SPIOut(u8ICNum);
	}
	return u8ICNum;
}
/*******************************************************************************
 * Function:     uint8 MCP_CombinSet(MCPComPinType u8Channel, uint8 u8Out)
 *
 * Description:  �ϲ���ͨ������
 *  
 *  Parameter :  u8Channel MCPComPinType ,u8Out: MCP_COMBINSET_xxx
 *
 * Returns:      �������õ�IC�ţ����0xff��Ϊʧ��
 *
 * Notes:       ��ģ�龲̬����
 *
*******************************************************************************/
static uint8 MCP_CombinSet(MCPComPinType u8Channel, uint8 u8Out)
{
	uint16 u16SetMask, u16SetData;
	uint8 u8ICChannel;
	uint8 u8ICNum;
	
	if (u8Channel > M7708_COMBINPIN16)
	{
		u8ICNum = 0xff;
	}
	else
	{
		if (u8Channel > M7708_COMBINPIN6)
		{
			u8ICChannel = u8Channel - M7708_COMBINPIN6;
			u8ICNum = 1;
		}
		else
		{
			u8ICChannel = u8Channel;
			u8ICNum = 0;
		}
		
		if (u8Out == MCP_COMBINSET_HIGH)
		{
			u16SetData = 0x0004;
		}
		else if (u8Out == MCP_COMBINSET_LOW)
		{
			u16SetData = 0x0002;
		}
		else
		{
			u16SetData = 0x0000;
		}
		
		if (u8ICChannel != 0)
		{
			u8ICChannel--;
		}else{}
		
		u8ICChannel <<= 1; 
		u16SetMask = 0x06;
		u16SetData <<= u8ICChannel;
		u16SetMask <<= u8ICChannel;
		u16SetMask = ~u16SetMask;
		
		sMcp7708Data[u8ICNum].u16WriteData &= u16SetMask;
		sMcp7708Data[u8ICNum].u16WriteData |= u16SetData;
	}
	
	return u8ICNum;
}

/*******************************************************************************
 * Function:     uint8 MCP_CombinOut(MCPComPinType u8Channel, uint8 u8Out)
 *
 * Description:  �ϲ���ͨ�����
 *  
 *  Parameter :  u8Channel MCPComPinType ,u8Out: MCP_COMBINSET_xxx
 *
 * Returns:      �������õ�IC�ţ����0xff��Ϊʧ��
 *
 * Notes:       none
 *
*******************************************************************************/
uint8 MCP_CombinOut(MCPComPinType u8Channel, uint8 u8Out)
{
	uint8 u8ICNum;
	
	u8ICNum = MCP_CombinSet(u8Channel, u8Out);
	
	if (u8ICNum < 2)
	{
		Mcp_SPIOut(u8ICNum);
	}
	else{}
	
	return u8ICNum;
}
/*******************************************************************************
 * Function:    BOOL Mcp_Drive_Motor(uint8 nmotor,uint8 outstat)
 *
 * Description: ����ͨ�������������ʱʹ��,��Ҫ ��ȷ ��ʼ�� Mcp_MotorInitʱ������
 *  
 *  Parameter :     mcpMotorId��  7708 �ĵ���ţ���motorcontorl�ĵ������Ҫ����
 *               outstat��  ���״̬:MOTOR_STATE_HALT/MOTOR_STATE_FORWARD/MOTOR_STATE_BACKWARD 
 *
 * Returns:     ���óɹ�Ϊ1 ������Ϊ0
 *
 * Notes:       ������Ҫʱ���ÿ���ʱ���ô˺���			
 *
*******************************************************************************/
BOOL Mcp_Drive_Motor(uint8 mcpMotorId, uint8 outstat)
{
	BOOL bDriveSet;
	MCPComPinType u8PositChanle, u8NegatChanle;  //����ͨ��
	BOOL bOut0, bOut1;
	
	if(mcpMotorId >= u8McpMotorNum) 
	{
		bDriveSet = FALSE;
	}
	else
	{
		bDriveSet = TRUE;
	}
   
	//ѡ����ͨ��
	u8PositChanle = TBL_McpMotorCOnfig[mcpMotorId*2];
	u8NegatChanle = TBL_McpMotorCOnfig[mcpMotorId*2+1];
	
   //�������״̬
	switch(outstat)
	{
		default: 
		case	MOTOR_RUN_HALT:	
			(void)MCP_CombinSet(u8PositChanle, MCP_COMBINSET_LOW);
			(void)MCP_CombinSet(u8NegatChanle, MCP_COMBINSET_LOW);
			break;
		case	MOTOR_RUN_FORWARD:				
			(void)MCP_CombinSet(u8PositChanle, MCP_COMBINSET_HIGH);
			(void)MCP_CombinSet(u8NegatChanle, MCP_COMBINSET_LOW);
			break;
		case	MOTOR_RUN_BACKWARD:
			(void)MCP_CombinSet(u8PositChanle, MCP_COMBINSET_LOW);
			(void)MCP_CombinSet(u8NegatChanle, MCP_COMBINSET_HIGH);
			break;
	}
	  
	bOut0 = 0;
	bOut1 = 0;
	if (u8PositChanle <= M7708_COMBINPIN6)
	{
		bOut0 = 1;
	}
	else
	{
		bOut1 = 1;
	}
	
	if (u8NegatChanle <= M7708_COMBINPIN6)
	{
		bOut0 = 1;
	}
	else
	{
		bOut1 = 1;
	}
	
	if (bOut0)
	{
		Mcp_SPIOut(0);
	}else{}
	
	if (bOut1)
	{
		Mcp_SPIOut(1);
	}else{}
	
	return bDriveSet;
}

/*******************************************************************************
 * Function:    void Mcp_SPIOut(uint8 u8McpId)
 *
 * Description: SPI�ڴ���ֵ	����MCP, u8McpId:IC ��
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
void Mcp_SPIOut(uint8 u8McpId)
{
	if (u8McpId < MCP7708_MAXNUM)
	{
		MCP7708DATA* pMcp7708Data = &sMcp7708Data[u8McpId]; 	
		
		pMcp7708Data->u16WriteData |= 0xA001U;	//mcp_rest=1;mcp_ovlo=1;mcp_over=1;
	
		if (pMcp7708Data->u16WriteDataLast != pMcp7708Data->u16WriteData)
		{//��һ������һ��SPI,Ԥ������
			pMcp7708Data->u16WriteDataLast = pMcp7708Data->u16WriteData;		
			
			(void) Init_Spi_Drive(SPI_EDGE_FALL,SPI_POLARITY_HIGH,TRUE);
			
			McpHW_CSSet(u8McpId, 0);
			MCU_NOP;
			MCU_NOP;
			pMcp7708Data->u16ReadData = Spi_Drive(pMcp7708Data->u16WriteData, 16);
		
			McpHW_CSSet(u8McpId, 1);
		}
		else
		{
			
		}
	}
	else{}
}



/*******************************************************************************
 * Function:    void Mcp_Test(void)
 *
 * Description: 7708 ����	
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
void Mcp_Test(void)
{
 	//mcp6208.Byte=0b11001101;
	//mcp6208h.Byte=0b10101100;
	sMcp7708Data[0].u16WriteData = 0xACCDU;
	Mcp_SPIOut(0);
	
	sMcp7708Data[1].u16WriteData = 0xACCDU;
	Mcp_SPIOut(1);
}
/*******************************************************************************
 * Function:   void CLose_Mcp(void)
 *
 * Description: 7708 �ر����	
 *
 * Returns:     none
 *
 * Notes:       none
 *		
*******************************************************************************/
void Mcp_Close(void)
{
	sMcp7708Data[0].u16WriteData = 0xAAABU;
	Mcp_SPIOut(0);
	
	sMcp7708Data[1].u16WriteData = 0xAAABU;
	Mcp_SPIOut(1);
}
	
