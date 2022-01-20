/*
 * ReadTouchID.c
 *
 *  Created on: Aug 20, 2020
 *      Author: Administrator
 */
#include "ReadTouchID.h"
#include "IIC_Drive_Software.h"
#include "Galib.h"
#include "APPDrive.h"

uint8 u8TouchKey, u8TouchState;
uint8 u8CheckData = 0;
uint8 u8VoiceEn;
uint16 ErrNum;
uint8 CDWord;
uint16 keywd;
uint8 ResData;
uint8 checksum;



uint8 GetCurrentKey(void)
{
	return keywd;
}


uint8 GetCheckSum(uint8* pData, uint8 size)
{
	uint8 i;
	uint8 u8CheckSum = 0;
	for (i = 0; i< size; i++)
	{
		u8CheckSum += *pData++;
	}

	u8CheckSum = ~u8CheckSum;
	return u8CheckSum;
}
uint8 CheckCheckSum(uint8* pData, uint8 size)
{
	uint8 i;
	uint8 u8CheckSum = 0;
	for (i = 0; i< size; i++)
	{
		u8CheckSum += *pData++;
	}

	if (u8CheckSum == 255)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

volatile uint8 bWriteOk;
void ReadTouch_MainControl(void)
{
	static uint16 TimerRead;
	static uint8 u8VoiceTimes;
	uint16 i;
	static uint8 u8State;
	uint8 RecData[13];
	if (TimeOutChkMsLong(&TimerRead, 2))
	{
//		IIC_Start_Bit();
//		IIC_Send_Byte(0x10);
//		IIC_Send_Byte(0x01); 
//		IIC_Stop_Bit();
		if (u8State < 2)
		{
			IIC_Start_Bit();
			if (IIC_Send_Byte(0x11) == 1)
			{
				u8State++;
				GaLib_MCUNOP(20);
				RecData[0] = IIC_Receive_Byte(); //读按键中断标记
				IIC_Send_Ack(ACK_YES);
				//GaLib_MCUNOP(100);
				RecData[1] = IIC_Receive_Byte(); //读按键中断标记
   			IIC_Send_Ack(ACK_YES);
				RecData[2] = IIC_Receive_Byte();
				IIC_Send_Ack(ACK_YES);
				RecData[3] = IIC_Receive_Byte(); //读按键中断标记
				IIC_Send_Ack(ACK_YES);
				RecData[4] = IIC_Receive_Byte(); //读按键中断标记
				IIC_Send_Ack(ACK_NO);
			}
			else
			{
				ErrNum++;
			}

			IIC_Stop_Bit();
			if (CheckCheckSum(RecData, 5) && RecData[3] == 0xaa)
			{//ok
				if (RecData[0] != 0)
				{
					InsertComd(RecData[0]);
				}
				keywd = (RecData[1]<<8) + RecData[2];
				if (keywd != 0)
				{
					Respond_Keydata(keywd);
				}else{}
			}
			else
			{
				MCU_NOP;
				MCU_NOP;
				MCU_NOP;
				ErrNum++;
			}
		}
		else
		{
			u8State = 0;
			IIC_Start_Bit();
			if (IIC_Send_Byte(0x10) == 1)
			{
				//GaLib_MCUNOP(100);
				MemCopy(RecData, (uint8*)&sPanleData, sizeof(PANLEDATA));

				RecData[12] = GetCheckSum(RecData, 12);
				bWriteOk = IIC_Send_Byte(RecData[0]);
				if (bWriteOk == 1)
				{
					bWriteOk = IIC_Send_Byte(RecData[1]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[2]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[3]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[4]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[5]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[6]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[7]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[8]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[9]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[10]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[11]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[12]);
					GaLib_MCUNOP(10);
					IIC_Send_Byte(RecData[13]);
				}
				else
				{

				}
			}
			else
			{
				ErrNum++;
			}
			IIC_Stop_Bit();
		}
		
//		if (IIC_Send_Byte(0x11) == 1)
//		//IIC_Send_Byte(0x11);
//		{
//
//			for (i=0; i < 400; i++)
//			{
//				NOP;
//			}
//			u8TouchKey = IIC_Receive_Byte(); //读按键中断标记
//			if (u8TouchKey != u8CheckData)
//			{
//				InsertComd(u8TouchKey);
//			}
//			else
//			{
//				NOP;
//			}
//			u8CheckData++;
//		} 


		
		//IIC_Send_Ack(ACK_YES);
		//u8TouchState = IIC_Receive_Byte(); //读按键中断标记
//		IIC_Send_Ack(ACK_NO);
//		IIC_Stop_Bit();
	}
}

