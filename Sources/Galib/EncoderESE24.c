/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：EncoderESE24.c
* 功能描述：ESE24编码器读取模块，具有如下功能
* 1 通过读取 ESE24编码器 2个PIN的状态值，得到命令并将命令放入队列模块
* 2 用户通过队列模块读取命令
* 
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立
* 原作者：xb
* 完成日期：2019年7月3日
* 
*/

#include "EncoderESE24.h"
#include "ComdQueue.h"

typedef struct
{
   uint8 CodeCur ;  //当前的状态
   uint8 CodeOld;  //上一次的状态
   uint8 CodeErr;
   uint8 KeyCode;
}CODEDATAKEY;    //定义数据结构

static CODEDATAKEY  CodeDataKey[MAX_CODEESE24NUM];
static uint8 CodeKeyTotalNum = 0;

//需要从外部调用的函数声明
//返回编码器的值 2个脚 a, b 。 对应返回字 0位，1位的状态
extern uint8 CodeESE24HW_ReadPin(uint8 num);
 
 /*******************************************************************************
 * Function:     BOOL Init_CodeESE24(uint8 num ,uint8 *CodeKeyData)
 *
 * Description:  编码器ese24控制初始化,  
 *               CodeKeyData 数组(大小为3) 为编码器键值(左边的)编号，右边的键值默认为左边的--，
 *               如果错误则设置为0（无效）
 *
 * Returns:      TRUE 设置正确, FALSE 设置错误
 *
 * Notes:   编码器初始化
 *
*******************************************************************************/
 BOOL Init_CodeESE24(uint8 num ,uint8 *CodeKeyData)
 {
	CODEDATAKEY  *pCodeData = CodeDataKey;
	BOOL bInitCKResult;
	if(num == 0 || num > MAX_CODEESE24NUM) 
	{
		bInitCKResult = FALSE;
	}
	else
	{
		bInitCKResult = TRUE;
		CodeKeyTotalNum = num;
    
		for(num = 0; num < CodeKeyTotalNum; num++)
		{
			if(CodeKeyData[num] !=0) 
			{
				pCodeData->KeyCode = CodeKeyData[num];
				pCodeData->CodeErr = CODEESE24_ERROR_NONE;
			}
			else
			{
				pCodeData->KeyCode = 0;
				pCodeData->CodeErr = CODEESE24_ERROR_KEYCODE;
			}
			pCodeData++;
		}
	}
	
	return bInitCKResult;
 }
 
 
 /*******************************************************************************
 * Function:  void Treat_Read_CodeESE24(void)
 *
 * Description: 编码器ESE24 控制
 *
 * Returns:     none
 *
 * Notes:    该函数仅被main函数循环调用
 *
*******************************************************************************/
void Treat_Read_CodeESE24(void)
{
	//读取风档开关电平
	uint8 num ;

	CODEDATAKEY *pCodeData=CodeDataKey;
	
	for(num=0; num < CodeKeyTotalNum; num++)
	{
		pCodeData->CodeCur = CodeESE24HW_ReadPin(num);  //从硬件模块中读取当前的端口状态
		
		if (pCodeData->CodeCur == 1 && pCodeData->CodeCur != pCodeData->CodeOld)
		{
			(void)InsertComd(pCodeData->KeyCode);
		}
		else  if (pCodeData->CodeCur==2 && pCodeData->CodeCur!=pCodeData->CodeOld)
		{
			(void)InsertComd(pCodeData->KeyCode-1);
		}
		else{}
		
		pCodeData->CodeOld = pCodeData->CodeCur;

		pCodeData++;
   }
    
}
