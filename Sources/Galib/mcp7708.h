/************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�mcp7708.h
* ����������7708 ģ���ͷ�ļ���
* Ϊ��ģ��������������Ҫ���� SPI ģ��
* 
* �������� codewarrior IDE 10.7-KEA
* MCU˵������MCU�޹�
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��5��23��
* 
*/

#ifndef MCP_7708__H__
#define MCP_7708__H__

#include "SPI_Drive.h"


#define   MCP7708_MAXNUM     2

//�����̵��� ���ʱ �̵���ͨ�� nch
typedef enum
{
	X7708CH_L1 = 1,
	X7708CH_H1 = 2,
	X7708CH_L2 = 3,
	X7708CH_H2 = 4,
	X7708CH_L3 = 5,
	X7708CH_H3 = 6,
	X7708CH_L4 = 7,
	X7708CH_H4 = 8,
	X7708CH_L5 = 9,
	X7708CH_H5 = 10,
	X7708CH_L6 = 11,
	X7708CH_H6 = 12,
	//2��ic
	X7708CH_L11 = 13,
	X7708CH_H11 = 14,
	X7708CH_L12 = 15,
	X7708CH_H12 = 16,
	X7708CH_L13 = 17,
	X7708CH_H13 = 18,
	X7708CH_L14 = 19,
	X7708CH_H14 = 20,
	X7708CH_L15 = 21,
	X7708CH_H15 = 22,
	X7708CH_L16 = 23,
	X7708CH_H16 = 24,
}MCPSinglePinType;

//���ͨ������
typedef enum
{
	M7708_COMBINPIN1 = 1,
	M7708_COMBINPIN2 = 2,
	M7708_COMBINPIN3 = 3,
	M7708_COMBINPIN4 = 4,
	M7708_COMBINPIN5 = 5,
	M7708_COMBINPIN6 = 6,
	//2��IC 
	M7708_COMBINPIN11 = 7,
	M7708_COMBINPIN12 = 8,
	M7708_COMBINPIN13 = 9,
	M7708_COMBINPIN14 = 10,
	M7708_COMBINPIN15 = 11,
	M7708_COMBINPIN16 = 12,
}MCPComPinType;

//״̬���ú궨��
#define MCP_SINGLESET_OFF      0        //����ͨ�������ر�
#define MCP_SINGLESET_ON       1
#define MCP_COMBINSET_LOW      0        //2��ͨ���ϲ���״̬�� 
#define MCP_COMBINSET_HIGH     1
#define MCP_COMBINSET_FLOAT    2

//�ⲿ�ɵ��ú���
BOOL Mcp_MotorInit(uint8 u8SetMotorNum, MCPComPinType* pSetChannel);
BOOL Mcp_Drive_Motor(uint8 mcpMotorId, uint8 outstat);
uint8 Mcp_SignalOut(MCPSinglePinType ncp,uint8 SetState);
uint8 MCP_CombinOut(MCPComPinType u8Channel, uint8 u8Out);
void Mcp_Test(void);
void Mcp_Close(void);
#endif




