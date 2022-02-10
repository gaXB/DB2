/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�mathGA.c
* ����������һЩ��ѧ��κ������ܣ�������
* 1 ���Ժ������
* 2 �����������о���λ��
* 3 �����ֵ
* 4 ���лز�Ĺ��ܵ���ι���
*
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ �Ӵ�ԭ��ѧģ��� 1.2 ����ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��4��25��
* 
*  �汾��1.1
* ˵����
* 1 �Ż���κ���
* GAMath_CalDataSegU8
* GAMath_CalDataSegI16
* 
* 2 ������ֶκ���
*  MathG_GetSegLinei16
* ���ߣ�xb
* ������ڣ�2020��1��6��
*/
#include"mathGA.h"


/*******************************************************************************
 * Function:   int16 GAMath_LineFuni16(int16 x1, int16 x2, int16 y1, int16 y2, int16 x)
 *
 * Description: �����Ժ��������Ժ����ĵ�Ϊ��x1��y1����  ��x2,y2��������   x2 > x1, x���ڣ�x1��X2)�� y2 != y1   
 *              ��x����Ӧy�㡣        
 *
 * Returns:     ����x����Ӧ��y�㡣��x1=x2 ��=x ʱ�� ����y1��
 *
 * Notes:          ��20000���ڵ���û���⣬��������ʱ��       
 *                 Valuey = Valuey * (x- x1);  �����     
 *                          
*******************************************************************************/
int16 GAMath_LineFuni16(int16 x1, int16 x2, int16 y1, int16 y2, int16 x)
{
   int32   Valuey, ValueZ;
   int32   i32Result; 
   //�ж�����������
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
		Valuey = Valuey * (x - x1);       //Valuey > 0, ֵ��Χ0- 65535  ��֮��Ҳ�������32λ
		
		ValueZ = Valuey % (x2 - x1); //����ValueZ ������ ��������
		ValueZ += ValueZ;
		ValueZ = ValueZ / (x2 - x1);
		
		i32Result = Valuey / (x2 - x1) + y1;     // ��ֵΪ |y - y1|��  ��Χ 0 - 65535
		i32Result += ValueZ;      
   }
   return  (int16)i32Result;
}
/*******************************************************************************
 * Function:  uint8 GAMath_CalDataSegU8(uint8 u8Cvalue, uint8* TBL_Data, uint8 u8MaxTBLNum)
 *
 *
 * Description: ��ǰֵu8Cvalue ��TBL_Data[DataNum]����һ���Σ���֪Data�����С�������� 
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
   {//�Ӵ�С����
      for (nSeg = 0; nSeg < u8MaxTBLNum && u8Cvalue <= TBL_Data[nSeg]; nSeg++)
      {
      }
   }
   else
   {//��С��������
      for (nSeg = 0; nSeg < u8MaxTBLNum && u8Cvalue >= TBL_Data[nSeg]; nSeg++)
      {
      }
   }
   
   return nSeg;
}

//�Ӵ�С���е�8λ����


//��С�������е�16λ����
uint8 GAMath_CalDataSegI16(int16 i16Cvalue, int16* TBL_Data, uint8 u8MaxTBLNum, uint8 u8Inverted)
{
   uint8 nSeg;

   if (u8Inverted)
   {//�Ӵ�С����
   	for (nSeg = 0; nSeg < u8MaxTBLNum && i16Cvalue <= TBL_Data[nSeg]; nSeg++)
   	{
   	}
   }
   else
   {//��С��������
   	for (nSeg = 0; nSeg < u8MaxTBLNum && i16Cvalue >= TBL_Data[nSeg]; nSeg++)
   	{
   	}
   }
   
   return nSeg;
}

/*******************************************************************************
 * Function:    int16 GAMath_abs(int16 data)
 *
 * Description: ����ֵ����
 *
 * Returns:  ���ش����ľ���ֵ
 *
 * Notes:   data ����Ϊ-32768
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
* Description:     ȷ�� u8Value ������TBL_Value���ĸ�λ��
* TBL_Value[0]    TBL_Value[0]  �� TBL_Value[1] ֮���3�ּ�1,2,3 �� 
*                       1 �򷵻�0��  3 �򷵻�1��  2 �Ļ���u8CurrentLevel >=1 �򷵻�1��else ����0
*TBL_Value[1]
*TBL_Value[2]   
*TBL_Value[3]
*TBL_Value[...]
*TBL_Value[u8MaxLevel]
*
* param1 ��   u8CurrentLevel  ��ǰ��λ
* param2 ��   u8Value ��ǰֵ
* param3 �� TBL_Value ����ָ��
* param3 �� u8MaxLevel ���λ����0��ʼ�� �� ���鳤��-1

* return :    ʵ�ʵ�λ
* 
* Note  :    ������ ��λ����λ��
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
				//����ƫ�ֱ������
			}
			else if (u8Value >= u8ValueB)
			{//u8ValueB<u8Value<u8Value1	����u8FactLevel+1
				u8FactLevel++;
			}
			else
			{//u8ValueA<u8Value<u8ValueB
				if (u8FactLevel >= u8CurrentLevel)
				{
					//����u8FactLevel(ԭ��λ��С)
				}
				else
				{
					//����u8FactLevel+1(ԭ��λ�ϴ�)
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
* Description:   ȷ�� i16Value ������TBL_Value���ĸ�λ��
* TBL_Value[0]    TBL_Value[0]  �� TBL_Value[1] ֮���3�ּ�1,2,3 �� 
*                       1 �򷵻�0��  3 �򷵻�1��  2 �Ļ���u8CurrentLevel >=1 �򷵻�1��else ����0
*TBL_Value[1]
*TBL_Value[2]   
*TBL_Value[3]
*TBL_Value[...]
*TBL_Value[u8MaxLevel]
*
* param1 ��   u8CurrentLevel  ��ǰ��λ
* param2 ��   i16Value ��ǰֵ
* param3 �� TBL_Value ����ָ��
* param3 �� u8MaxLevel ���λ����0��ʼ�� �� ���鳤��-1

* return :    ʵ�ʵ�λ
* 
* Note  :    ������ ��λ����λ��
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
			{//u16ValueB<u16Value<u16Value1	����u8FactLevel+1
				u8FactLevel++;
			}
			else
			{//u16ValueA<u16Value<u16ValueB
				if (u8FactLevel >= u8CurrentLevel)
				{
					//����u8FactLevel(ԭ��λ��С)
				}
				else
				{
					//����u8FactLevel+1(ԭ��λ�ϴ�)
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
* Description:   �õ��ֶ����ߵ��м�ֵ
* �߶εĵ�Ϊ DataX��Datay�� numΪ������DataX��С��������
*  Cvalue ��Ҫ���xֵ�� ���ض�Ӧ��yֵ
*  ��������Χʱ���� Datay[0] Datay[num-1]
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

