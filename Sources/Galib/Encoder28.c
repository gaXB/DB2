/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：Encoder28.c
* 功能描述：28编码器读取模块，具有如下功能
* 1 通过读取 28编码器 2个PIN的状态值，得到28的命令并将命令放入队列模块
* 2 用户通过队列模块读取命令
* 
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原模块的0.0.0移植
* 原作者：xb
* 完成日期：2019年5月24日
* 
*/
#include "Encoder28.h"


typedef struct
{
   uint8 CodeCur ;  //当前的状态
   uint8 CodeOld;  //上一次的状态
   uint8 CodeErr;
   uint8 KeyCode;
}CODEDATA28;    //定义数据结构

static CODEDATA28 CodeData28[MAX_CODE28NUM];
static uint8 Code28TotalNum = 0;

//需要从外部调用的函数声明
//返回编码器的值 2个脚 a, b 。 对应返回字 0位，1位的状态
extern uint8 Code28HW_ReadPin(uint8 num);
 
 /*******************************************************************************
 * Function:   BOOL Init_Code28(uint8 num ,uint8 *CodeKeyData)
 *
 * Description:  编码器28控制初始化,  
 *               CodeKeyData 数组(大小为3) 为编码器键值(左边的)编号，右边的键值默认为左边的--，
 *               如果错误则设置为0（无效）
 *
 * Returns:      TRUE 设置正确, FALSE 设置错误
 *
 * Notes:   编码器初始化
 *
*******************************************************************************/
 BOOL Init_Code28(uint8 num ,uint8 *CodeKeyData)
 {
	CODEDATA28  *pCodeData = CodeData28;
   BOOL bInitCode28; 
	if(num==0 || num>MAX_CODE28NUM) 
	{
		bInitCode28 = FALSE;
		Code28TotalNum = 0;
	}
	else
	{
		bInitCode28 = TRUE;
		Code28TotalNum = num;
    
		for(num=0; num < Code28TotalNum; num++)
		{
			if(CodeKeyData[num] != 0) 
			{
				pCodeData->KeyCode = CodeKeyData[num];
				pCodeData->CodeErr = CODE28_ERROR_NONE;
			}
			else
			{
				pCodeData->KeyCode = 0;
				pCodeData->CodeErr = CODE28_ERROR_KEYCODE;
			}
			pCodeData++;
		}
	}
	
	return bInitCode28;
 }
 
 
 /*******************************************************************************
 * Function:  void Read_Code28(void)
 *
 * Description: 编码器28控制
 *
 * Returns:     none
 *
 * Notes:    该函数仅被main函数循环调用,必须为4ms以上的频率，也可放在中断位置 		
 *
*******************************************************************************/
void Treat_Read_Code28(void)
{
	//读取风档开关电平
	uint8 num;
	uint8 KeyCode = 0; // 1 ++ ,2--
	uint8 ReadCode;
	CODEDATA28 *pCodeData = CodeData28;
	
	for(num = 0; num < Code28TotalNum; num++)
	{
      ReadCode = Code28HW_ReadPin(num);     //从硬件模块中读取当前的端口状态
      pCodeData->CodeCur = ReadCode&0x01;  //a 脚的电压需要知道上一次的值，b脚不需要
      ReadCode &= 0x02;                    //b 脚的状态
      
      if (pCodeData->CodeCur != pCodeData->CodeOld)
      {
         //不一样键值时需要处理
      	switch (pCodeData->CodeCur)
         {
            default:
               pCodeData->CodeErr |=CODE28_ERROR_HARDWARE;
               break;
            case  0:
               if (ReadCode)
   				{
               	KeyCode = 1;
   				}
   				else
   				{
   					KeyCode = 2;
   				}
   				break;
            case  1:
               if (ReadCode)
               {
                  KeyCode = 2;
               }
               else
               {
               	KeyCode = 1;
               } 
               break;
         }
         if (KeyCode == 1) 
         {  //清故障码
            pCodeData->CodeErr &= ~CODE28_ERROR_HARDWARE;  
            (void)InsertComd(pCodeData->KeyCode);
         }
         else if (KeyCode == 2)
         {
         	pCodeData->CodeErr &= ~CODE28_ERROR_HARDWARE;  
         	(void)InsertComd(pCodeData->KeyCode-1);
         }
         pCodeData->CodeOld = pCodeData->CodeCur;
      //不一样键值时需要处理
      }
      pCodeData++;
   }
}
