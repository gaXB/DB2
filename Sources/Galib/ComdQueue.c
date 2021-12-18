/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：ComdQueue.c
* 功能描述：队列 模块
* 
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原队列模块的1.0移植
* 原作者：xb
* 完成日期：2019年4月29日
* 
* 
*/
#include "ComdQueue.h"


static   uint8  QueueComd[MAX_COMD_NUM];
static   uint8  QueueFront;      //首元素下标
static   uint8  QueueRear;       //尾元素下标




/*******************************************************************************
 * Function:   void InitComdQueue(void)
 *
 * Description:  命令队列初始化
 *
 * Returns:    None
 *
 * Notes:    清空数据，有需要时调用
 *
*******************************************************************************/
void InitComdQueue(void)
{
   QueueFront = 0;
   QueueRear  = 0;
}


 /*******************************************************************************
 * Function:   BOOL InsertComd(uint8 Comd)
 *
 * Description:  命令进入队列
 *
 * Returns:    进入失败 FALSE,成功TRUE;
 *
 * Notes:   外界有命令时调用
 *
*******************************************************************************/
BOOL  InsertComd(uint8 Comd)
{
   uint8 bInitOK;
	if(Comd == COMD_NONE)
   {
		bInitOK = FALSE;
   }
	else
	{
	   QueueComd[QueueRear++] = Comd;

	   if (QueueRear >= MAX_COMD_NUM)
	   {
	      QueueRear = 0;
	   }
	   else{}
	   
	   if (QueueRear == QueueFront)
	   {//如果数据溢出则舍去第一个传入的数据
	   	QueueFront++;
	   	if (QueueFront >= MAX_COMD_NUM)
	   	{
	   		QueueFront = 0;
	   	}
	   	else{}
	   }
	   else{}
	   
	   bInitOK = TRUE;
	}


   return bInitOK;
}



 /*******************************************************************************
 * Function:   BOOL InsertComd(uint8 Comd)
 *
 * Description:  命令出队列
 *
 * Returns:    空了返回 TRUE;
 *
 * Notes:   应用层调用，一般放在按键处理中处理
 *
*******************************************************************************/
uint8  ReadComd(void)
{
   uint8   u8Comd;

   if (QueueFront != QueueRear)
   {//队列中有命令
   	u8Comd = QueueComd[QueueFront++];
      if (QueueFront >= MAX_COMD_NUM)
      {
         QueueFront = 0;
      }
      else{}
   }
   else
   {
   	u8Comd = COMD_NONE;
   } 
   
   return u8Comd;
}







