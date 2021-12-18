/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：keypad.c
* 功能描述：按键模块，具有
* 1 按键读取并滤波
* 2 组合按键功能
* 3 长按键
* 4 连续键功能
* 5 组合3击功能
*
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
*
* */
/**************************更改记录******************************************************
*
* 版本：1.0
* 说明：初始版本建立， 从原按键模块的1.3移植，去掉电压保护功能
* 原作者：xb
* 完成日期：2019年5月9日
*
*
* 版本：1.1
* 说明：组合双击和4击键程序优化
* 原作者：xb
* 完成日期：2020年5月15日
*
* 版本：1.2
* 说明：增加AC、循环4击键内外部软件版本检测
* 原作者：lyc
* 完成日期：2020年5月19日
* 
* 版本：1.3
* 说明：
* 1 增加内循环按键为长按键功能，用来显示客户版本号
* 2 更改AC 内外的名字，为版本号高低位键
* 作者：xb
* 完成日期：2020年6月3日
*/
#include"keypad.h"
#include"timerout.h"

//本模块静态变量
static uint16 ResKeydata[RESKEY_BUFFERNUM];     //响应的键值，KEY_CODE_NO 为没有按键，此程序没有缓冲功能
static uint16 LongErrKeyCode;
//static uint8   KeyErrCode;//错误代码，错误时的键值，错误flg

//功能配置项可减少代码量,预留
//#define   NEED_CONTINUEKEY_FUNICTION   1   是否需要连续键功能，需要的话将KEY_CODE_CONITUE1  KEY_CODE_CONITUE2 有此功能
//#define   NEED_LONGKEY_FUNICTION       1   是否需要连续键功能，需要的话将KEY_CODE_LONG 有此功能
//#define   NEED_COMBTHREE_FUNICTION     1   是否需要组合3击功能，需要的话将KEY_CODE_COMBTHREE 有此功能

//从硬件驱动层调用函数
extern uint16 KeyPadHW_ReadSignal(void);

//本模块的函数说明
static uint16  KeyBoard_Filter(uint16 CurrentKey);

/*******************************************************************************
 * Function:   uint16 Read_Keydate(void)
 *
 * Description: 读取按键函数
 *
 * Returns:     keycode
 *
 * Notes:   应用层可调用此函数来获得键值 ， KEY_CODE_NO为无键

 *
*******************************************************************************/
uint16 Read_Keydate(void)
{
   uint16 keycode;
   uint8 u8KBNum = 0;
   
   while(u8KBNum < RESKEY_BUFFERNUM && ResKeydata[u8KBNum] == KEY_CODE_NO)
   {
   	u8KBNum++;
   };
   if (u8KBNum < RESKEY_BUFFERNUM)
   {
   	keycode = ResKeydata[u8KBNum];
   	ResKeydata[u8KBNum] = KEY_CODE_NO;
   }
   else
   {
      keycode = KEY_CODE_NO;
   }
   
   return  keycode;
}

 /*******************************************************************************
 * Function:    uint8  Read_Key_Errcode(void)
 *
 * Description: 读取按键故障码
 *
 * Returns:     KeyErrCode
 *
 * Notes:   应用层可调用此函数读取故障码
		   KEY_ERROR_NONE            键盘正常
           KEY_ERROR_INVALID         无效键
           KEY_ERROR_LONGDOWN        某个键长按
           KEY_ERROR_READ	         物理层送来键有问题
 *
*******************************************************************************/
uint8  Read_Key_Errcode(void)
{
	uint8 keyErr;
	
	if(LongErrKeyCode != KEY_CODE_NO) 
	{
		keyErr = KEY_ERROR_LONGDOWN;
	}
	else
	{
		keyErr = KEY_ERROR_NONE;
	}
	
	return keyErr;
}

/*******************************************************************************
 * Function:    void Treat_Read_Keypad(void)
 *
 * Description: 控制核心函数
 *
 * Returns:     none
 *
 * Notes:     放在Main-LOOP
 *
*******************************************************************************/
void Treat_Read_Keypad(void)
{
	uint16 KeyCodeHW, KeyFilter;
	static uint16 LastKey;  //上次响应的键值，上次的滤波键值
	static uint16 KeyDownTimer;   //同一按键按键计时
	static uint16 CombFourTimer;  //4击计时
	static uint8 CombFourNum;   //4击次数
	static uint8 CombFourAcNcfNum;   //ac 循环4击次数
	
	static uint8 ConKeyState;  //连续按键的状态
	

	KeyCodeHW = KeyPadHW_ReadSignal();
	KeyFilter = KeyBoard_Filter(KeyCodeHW);

	if(KeyFilter == KEY_CODE_TREMBLE)
	{	//抖动返回

	}
	else if ((KeyFilter&KEY_CODE_SPEC) == KEY_CODE_SPEC)
	{

	}
	else if (KeyFilter == KEY_CODE_NO)
	{//按照流程，响应按键值
		LastKey = KEY_CODE_NO;
	}
	else if(KeyFilter == LastKey)
	{ //是上一次响应过的键，就是处理是不是长按键，或者连续键
//#if NEED_CONTINUEKEY_FUNICTION
		if(KeyFilter == KEY_CODE_CONTINUE1 || KeyFilter == KEY_CODE_CONTINUE2)
		{//连续键功能处理
			switch (ConKeyState)
			{ //特殊按键状态，处理过程为  CONITUEKEY_SEG1_TIME 0.1s后，再每隔CONITUEKEY_SEG2_TIME 0.1s 响应一次
				default:
					break;
				case  CONKEY_STATE_BEGIN:
					//开始，
					if (TimeOutChkTenthSLong(&KeyDownTimer, CONTINUEKEY_SEG1_TIME))
					{  //第一阶段
						ConKeyState = CONKEY_STATE_CONTINUE;
						Respond_Keydata(KeyFilter);
					}
					break;
				case CONKEY_STATE_CONTINUE:
					if (TimeOutChkTenthSLong(&KeyDownTimer, CONTINUEKEY_SEG2_TIME))
					{   //第二阶段，每隔一段时间响应一次
						Respond_Keydata(KeyFilter);
					}
					break;
			}
		}
		else
//#endif
//#if NEED_LONGKEY_FUNICTION
		if (KeyFilter == KEY_CODE_LONG || KeyFilter == KEY_VER_H)
		{
			if(TimeOutChkTenthSLong(&KeyDownTimer, LONGKEY_TIME))
			{//长按 LONGKEY_TIME 响应 ，响应值为 此键 后8位 全为1
				if (KeyFilter > KEY_CODE_SPEC)
				{
					Respond_Keydata(KeyFilter+KEY_CODE_SPEC);
				}
				else
				{
					Respond_Keydata(KeyFilter+KEY_CODE_SPEC_H);
				}
			}
		}
		else
//#endif
		{}//其他按键不做处理
		//LastKey = KeyFilter;
	}
	else //不是上次的键
	{
		(void)TimeOutChkTenthSLong(&KeyDownTimer,0) ;   //开始计时
		ConKeyState = CONKEY_STATE_BEGIN;
		if(~LastKey & KeyFilter)
      { //这次的按键比上次的键值有多的键，则响应其组合键或者单键
//#if NEED_COMBTHREE_FUNICTION
			if(KeyFilter == KEY_CODE_COMBFOUR)
			{//4击键功能开始
				if (CombFourNum == 0)
				{//第一次,开始计时
					CombFourNum = 1;
					(void)TimeOutChkTenthSLong(&CombFourTimer, 0);
				}
				else
				{
					CombFourNum++;
					if (CombFourNum >= 4)
					{
						CombFourNum = 0;
						if (TimeOutChkTenthSLong(&CombFourTimer, COMBFOUR_TIME))
						{
						  //在    COMBFOUR_TIME外，则重新计时
						}
						else
						{ //响应键值
							Respond_Keydata(KeyFilter);
						}
					}
					else
					{}
				}
			}
			else if(KeyFilter == KEY_CODE_VER_COMBIN)
			{
				//4击键功能开始
				if (CombFourAcNcfNum == 0)
				{//第一次,开始计时
					CombFourAcNcfNum = 1;
					(void)TimeOutChkTenthSLong(&CombFourTimer, 0);
				}
				else
				{
					CombFourAcNcfNum++;
					if (CombFourAcNcfNum >= 4)
					{
						CombFourAcNcfNum = 0;
						if (TimeOutChkTenthSLong(&CombFourTimer, COMBFOUR_TIME))
						{
						  //在    COMBFOUR_TIME外，则重新计时
						}
						else
						{ //响应键值
							if(LastKey==KEY_VER_H)
							{
								Respond_Keydata(KEY_CODE_COMBFOUR_VERL);
							}
							else if(LastKey==KEY_VER_L)
							{
								Respond_Keydata(KEY_CODE_COMBFOUR_VERH);
							}
							else{}
						}
					}
					else
					{}
				}					
			}
			else
			{
				Respond_Keydata(KeyFilter);
			}
      }
		else
		{//没有其他键，不响应

		}
		LastKey = KeyFilter;

		if ((KeyFilter& ~KEY_CODE_COMBFOUR))
		{ //有其他键值，计数清0
			CombFourNum=0;
		}
		else{}
		if ((KeyFilter& ~KEY_CODE_VER_COMBIN))
		{ //有其他键值，计数清0
			CombFourAcNcfNum=0;
		}
		else{}
	}

}


 /*******************************************************************************
 * Function:   static void Respond_Keydata(uint16 keycode)
 *
 * Description: 响应 keycode  ,并记录在LastActiveKey
 *
 * Returns:
 *
 * Notes:     本模块中调用，下同

 *
*******************************************************************************/
void Respond_Keydata(uint16 keycode)
{
   uint8 u8KBNum = 0;
   
   while(u8KBNum < RESKEY_BUFFERNUM && ResKeydata[u8KBNum] != KEY_CODE_NO)
   {
   	u8KBNum++;
   };
   if (u8KBNum < RESKEY_BUFFERNUM)
   {
   	ResKeydata[u8KBNum] = keycode;
   }
   else
   {//太多则将最后一个缓存占用
   	ResKeydata[RESKEY_BUFFERNUM-1] = keycode;
   }
}

/*******************************************************************************
 * Function:    static uint16  KeyBoard_Filter(uint16 CurrentKey)
 *
 * Description:  滤波键值，1 按键抖动处理，KEY_FILTER_TIME ms
 *                         2 按键被卡死时，将此按键滤去 KEY_LONG_FILTER_TIME
 *
 * Returns:   过滤后的键值
 *
 * Notes:     本模块静态函数，本模块中调用

 *
*******************************************************************************/
static uint16  KeyBoard_Filter(uint16 CurrentKey)
{

   static uint16  Keycode_1;   //记录上一次的键值
   static uint16  KeyfilterTimer;   //计时参数
   static uint16  KeyLongTimer;     //计时参数
   static uint16  KeyFilter;
   uint16 u16RetKey;
   if (CurrentKey == Keycode_1)
   {
      if (TimeOutChkMsLong(&KeyfilterTimer, KEY_FILTER_TIME))
      {
         KeyFilter = CurrentKey;
      }
      else
      {//返回上次的keyfilter
      }

      if(CurrentKey == KEY_CODE_NO)
      {  //无键时恢复，处理
         if(TimeOutChkTenthSLong(&KeyLongTimer, NOKEY_LONG_FILTER_TIME))
         {
            LongErrKeyCode = KEY_CODE_NO;
         }
         else
         {}
      }
      else if (TimeOutChkTenthSLong(&KeyLongTimer, KEY_LONG_FILTER_TIME))
      {  //有键时，KEY_LONG_FILTER_TIME*0.1 S 后认为是卡键
         LongErrKeyCode = CurrentKey;
      }
      else
      {}
   }
   else//抖动 重新计时
   {
   	(void)TimeOutChkMsLong(&KeyfilterTimer, 0);
      (void)TimeOutChkTenthSLong(&KeyLongTimer, 0);
      KeyFilter = KEY_CODE_TREMBLE;
   }
   Keycode_1 = CurrentKey;          //记录上次的键值
   //进入长按键滤键处理

   if(KeyFilter == KEY_CODE_TREMBLE)
   {//是抖动键不进行 长键滤键
   	u16RetKey = KEY_CODE_TREMBLE;
   }
   else
   {
   	u16RetKey = KeyFilter&(~LongErrKeyCode);         //滤去长键
   }

   return u16RetKey;
}

