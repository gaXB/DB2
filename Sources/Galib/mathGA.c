/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：mathGA.c
* 功能描述：一些数学求段函数功能，包括：
* 1 线性函数求点
* 2 数据在数组中具体位置
* 3 求绝对值
* 4 具有回差的功能的求段功能
*
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从从原数学模块的 1.2 上移植
* 原作者：xb
* 完成日期：2019年4月25日
* 
*  版本：1.1
* 说明：
* 1 优化求段函数
* GAMath_CalDataSegU8
* GAMath_CalDataSegI16
* 
* 2 增加求分段函数
*  MathG_GetSegLinei16
* 作者：xb
* 完成日期：2020年1月6日
*/
#include"mathGA.h"


/*******************************************************************************
 * Function:   int16 GAMath_LineFuni16(int16 x1, int16 x2, int16 y1, int16 y2, int16 x)
 *
 * Description: 求线性函数，线性函数的点为（x1，y1），  （x2,y2）。其中   x2 > x1, x属于（x1，X2)。 y2 != y1   
 *              求x所对应y点。        
 *
 * Returns:     返回x所对应的y点。当x1=x2 ！=x 时， 返回y1。
 *
 * Notes:          ±20000以内的数没问题，当数过大时，       
 *                 Valuey = Valuey * (x- x1);  会溢出     
 *                          
*******************************************************************************/
int16 GAMath_LineFuni16(int16 x1, int16 x2, int16 y1, int16 y2, int16 x)
{
   int32   Valuey, ValueZ;
   int32   i32Result; 
   //判断条件不满足
   if (x == x1) 
   {
   	i32Result = y1;
   }
   else if (x == x2) 
   {
   	i32Result = y2;
   }
   else if (x2 == x1)
   {
   	i32Result = y1; 
   }
   else
   {
   	Valuey = y2 - y1;  
		Valuey = Valuey * (x - x1);       //Valuey > 0, 值范围0- 65535  乘之后也不会大于32位
		
		ValueZ = Valuey % (x2 - x1); //增加ValueZ 的余数 四舍五入
		ValueZ += ValueZ;
		ValueZ = ValueZ / (x2 - x1);
		
		i32Result = Valuey / (x2 - x1) + y1;     // 此值为 |y - y1|，  范围 0 - 65535
		i32Result += ValueZ;      
   }
   return  (int16)i32Result;
}
/*******************************************************************************
 * Function:  uint8 GAMath_CalDataSegU8(uint8 u8Cvalue, uint8* TBL_Data, uint8 u8MaxTBLNum)
 *
 *
 * Description: 求当前值u8Cvalue 在TBL_Data[DataNum]的哪一个段，已知Data数组从小到大排列 
 *
 * Returns:  <Data[0]  0,     
 *           >=Data[0]&& <Data[1] : 1   
 *           DATA[1]-DATA[2] 2
 *           . .... 
 *           DATA[n-1]-DATA[n]   n
 *           .... DATA[u8MaxTBLNum-2]-DATA[u8MaxTBLNum-1]   u8MaxTBLNum-1    
 *           >= DATA[u8MaxTBLNum-1] :  u8MaxTBLNum
 * Notes:       
 *
 *
*******************************************************************************/
uint8 GAMath_CalDataSegU8(uint8 u8Cvalue, uint8* TBL_Data, uint8 u8MaxTBLNum, uint8 u8Inverted)
{
   uint8 nSeg;
   
   if (u8Inverted)
   {//从大到小排列
      for (nSeg = 0; nSeg < u8MaxTBLNum && u8Cvalue <= TBL_Data[nSeg]; nSeg++)
      {
      }
   }
   else
   {//从小到大排列
      for (nSeg = 0; nSeg < u8MaxTBLNum && u8Cvalue >= TBL_Data[nSeg]; nSeg++)
      {
      }
   }
   
   return nSeg;
}

//从大到小排列的8位数组


//从小到大排列的16位数组
uint8 GAMath_CalDataSegI16(int16 i16Cvalue, int16* TBL_Data, uint8 u8MaxTBLNum, uint8 u8Inverted)
{
   uint8 nSeg;

   if (u8Inverted)
   {//从大到小排列
   	for (nSeg = 0; nSeg < u8MaxTBLNum && i16Cvalue <= TBL_Data[nSeg]; nSeg++)
   	{
   	}
   }
   else
   {//从小到大排列
   	for (nSeg = 0; nSeg < u8MaxTBLNum && i16Cvalue >= TBL_Data[nSeg]; nSeg++)
   	{
   	}
   }
   
   return nSeg;
}

/*******************************************************************************
 * Function:    int16 GAMath_abs(int16 data)
 *
 * Description: 绝对值函数
 *
 * Returns:  返回此数的绝对值
 *
 * Notes:   data 不能为-32768
 *
 *
*******************************************************************************/
int16 GAMath_abs(int16 data)
{
	int16 i16PositData;
	if (data >= 0)
	{
		i16PositData = data;
	}
	else
	{
		i16PositData = -data;
	}
	
	return i16PositData;
}

/*******************************************************************************
* Function:    uint8 GAMath_CalTBLu8Level(uint8 u8CurrentLevel, uint8 u8Value, uint8* TBL_Value, uint8 u8MaxLevel)
*
* Description:     确定 u8Value 在数组TBL_Value在哪个位置
* TBL_Value[0]    TBL_Value[0]  和 TBL_Value[1] 之间分3分即1,2,3 ， 
*                       1 则返回0；  3 则返回1；  2 的话看u8CurrentLevel >=1 则返回1，else 返回0
*TBL_Value[1]
*TBL_Value[2]   
*TBL_Value[3]
*TBL_Value[...]
*TBL_Value[u8MaxLevel]
*
* param1 ：   u8CurrentLevel  当前档位
* param2 ：   u8Value 当前值
* param3 ： TBL_Value 数组指针
* param3 ： u8MaxLevel 最大档位，从0开始。 即 数组长度-1

* return :    实际档位
* 
* Note  :    可用于 电位器求档位等
*******************************************************************************/
uint8 GAMath_CalTBLu8Level(uint8 u8CurrentLevel, uint8 u8Value, uint8* TBL_Value, uint8 u8MaxLevel)
{
	uint8 u8FactLevel ,u8Value0, u8ValueA, u8ValueB, u8Value1, u8Diff;

	if (u8MaxLevel == 0) 
	{
		u8FactLevel = 0;
	}
	else
	{
		u8FactLevel = GAMath_CalDataSegU8(u8Value, TBL_Value, u8MaxLevel+1, 0);
		if (u8FactLevel == 0)
		{
			u8FactLevel = 0; 
		}
		else if (u8FactLevel > u8MaxLevel)
		{
			u8FactLevel = u8MaxLevel;  
		}
		else
		{
			u8FactLevel--;// 0 - (maxlevel-1)
			u8Value0 = TBL_Value[u8FactLevel];
			u8Value1 = TBL_Value[1+u8FactLevel];
			u8Diff = (u8Value1 - u8Value0);
			u8Diff = u8Diff / 3;
			u8ValueA = TBL_Value[u8FactLevel] + u8Diff;
			u8ValueB = TBL_Value[1+u8FactLevel] - u8Diff;
			
			// u8Value0 u8ValueA u8ValueB u8Value1
			if (u8Value <= u8ValueA)
			{
				//在下偏差，直接跳出
			}
			else if (u8Value >= u8ValueB)
			{//u8ValueB<u8Value<u8Value1	返回u8FactLevel+1
				u8FactLevel++;
			}
			else
			{//u8ValueA<u8Value<u8ValueB
				if (u8FactLevel >= u8CurrentLevel)
				{
					//返回u8FactLevel(原档位较小)
				}
				else
				{
					//返回u8FactLevel+1(原档位较大)
					u8FactLevel++;
				}
			}
		}
	}
	
	return (u8FactLevel);
}



/*******************************************************************************
* Function:    uint8 GAMath_CalTBLi16Level(uint8 u8CurrentLevel, int16 i16Value, int16* TBL_Value, uint8 u8MaxLevel)
*
* Description:   确定 i16Value 在数组TBL_Value在哪个位置
* TBL_Value[0]    TBL_Value[0]  和 TBL_Value[1] 之间分3分即1,2,3 ， 
*                       1 则返回0；  3 则返回1；  2 的话看u8CurrentLevel >=1 则返回1，else 返回0
*TBL_Value[1]
*TBL_Value[2]   
*TBL_Value[3]
*TBL_Value[...]
*TBL_Value[u8MaxLevel]
*
* param1 ：   u8CurrentLevel  当前档位
* param2 ：   i16Value 当前值
* param3 ： TBL_Value 数组指针
* param3 ： u8MaxLevel 最大档位，从0开始。 即 数组长度-1

* return :    实际档位
* 
* Note  :    可用于 电位器求档位等
*******************************************************************************/
uint8 GAMath_CalTBLi16Level(uint8 u8CurrentLevel, int16 i16Value, int16* TBL_Value, uint8 u8MaxLevel)
{
	uint8 u8FactLevel;
	int16 i16Value0, i16ValueA, i16ValueB, i16Value1, i16Diff;

	if (u8MaxLevel == 0) 
	{
		u8FactLevel = 0;
	}
	else
	{
		u8FactLevel = GAMath_CalDataSegI16(i16Value, TBL_Value, u8MaxLevel, 0);
		
		if (u8FactLevel == 0)
		{
			u8FactLevel = 0; 
		}
		else if (u8FactLevel > u8MaxLevel)
		{
			u8FactLevel = u8MaxLevel;  
		}
		else
		{
			u8FactLevel--;// 0 - (maxlevel-1)
			i16Value0 = TBL_Value[u8FactLevel];
			i16Value1 = TBL_Value[1+u8FactLevel];
			i16Diff = (i16Value1 - i16Value0);
			i16Diff = i16Diff / 3;
			i16ValueA = TBL_Value[u8FactLevel] + i16Diff;
			i16ValueB = TBL_Value[1+u8FactLevel] - i16Diff;
			
			// u16Value0 u16ValueA u16ValueB 16Value1
			if (i16Value <= i16ValueA)
			{
			}
			else if (i16Value >= i16ValueB)
			{//u16ValueB<u16Value<u16Value1	返回u8FactLevel+1
				u8FactLevel++;
			}
			else
			{//u16ValueA<u16Value<u16ValueB
				if (u8FactLevel >= u8CurrentLevel)
				{
					//返回u8FactLevel(原档位较小)
				}
				else
				{
					//返回u8FactLevel+1(原档位较大)
					u8FactLevel++;
				}
			}
		}
	}
	
	return (u8FactLevel);
}

/*******************************************************************************
* Function:    int16 MathG_GetSegLinei16(int16 Cvalue,int16* DataX, int16 *Datay, uint8 Num)
*
* Description:   得到分段曲线的中间值
* 线段的点为 DataX，Datay， num为点数，DataX从小到大排列
*  Cvalue 需要查的x值， 返回对应的y值
*  当超过范围时按照 Datay[0] Datay[num-1]
* return :   
* 
* Note  :    
*******************************************************************************/
int16 MathG_GetSegLinei16(int16 Cvalue,int16* DataX, int16 *Datay, uint8 Num)
{
   uint8 nSeg;
   int16 i16ResultY;
   nSeg = GAMath_CalDataSegI16(Cvalue, DataX, Num, 0);
   if (nSeg ==0)
   {
   	i16ResultY = Datay[0];   
   }
   else if (nSeg == Num)
   {
   	i16ResultY = Datay[Num-1];   
   }
   else
   {
   	i16ResultY = GAMath_LineFuni16(DataX[nSeg-1], DataX[nSeg], Datay[nSeg-1], Datay[nSeg], Cvalue);
   }
   
   return i16ResultY;
}

int16 MathG_GetSegLinei16_rev(int16 Cvalue,int16* DataX, int16 *Datay, uint8 Num)
{
   uint8 nSeg;
   int16 i16ResultY;
   nSeg = GAMath_CalDataSegI16(Cvalue, DataX, Num, 1);
   if (nSeg ==0)
   {
   	i16ResultY = Datay[0];
   }
   else if (nSeg == Num)
   {
   	i16ResultY = Datay[Num-1];
   }
   else
   {
   	i16ResultY = GAMath_LineFuni16(DataX[nSeg-1], DataX[nSeg], Datay[nSeg-1], Datay[nSeg], Cvalue);
   }

   return i16ResultY;
}

