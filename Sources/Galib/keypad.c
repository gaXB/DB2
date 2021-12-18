/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�keypad.c
* ��������������ģ�飬����
* 1 ������ȡ���˲�
* 2 ��ϰ�������
* 3 ������
* 4 ����������
* 5 ���3������
*
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
*
* */
/**************************���ļ�¼******************************************************
*
* �汾��1.0
* ˵������ʼ�汾������ ��ԭ����ģ���1.3��ֲ��ȥ����ѹ��������
* ԭ���ߣ�xb
* ������ڣ�2019��5��9��
*
*
* �汾��1.1
* ˵�������˫����4���������Ż�
* ԭ���ߣ�xb
* ������ڣ�2020��5��15��
*
* �汾��1.2
* ˵��������AC��ѭ��4�������ⲿ����汾���
* ԭ���ߣ�lyc
* ������ڣ�2020��5��19��
* 
* �汾��1.3
* ˵����
* 1 ������ѭ������Ϊ���������ܣ�������ʾ�ͻ��汾��
* 2 ����AC ��������֣�Ϊ�汾�Ÿߵ�λ��
* ���ߣ�xb
* ������ڣ�2020��6��3��
*/
#include"keypad.h"
#include"timerout.h"

//��ģ�龲̬����
static uint16 ResKeydata[RESKEY_BUFFERNUM];     //��Ӧ�ļ�ֵ��KEY_CODE_NO Ϊû�а������˳���û�л��幦��
static uint16 LongErrKeyCode;
//static uint8   KeyErrCode;//������룬����ʱ�ļ�ֵ������flg

//����������ɼ��ٴ�����,Ԥ��
//#define   NEED_CONTINUEKEY_FUNICTION   1   �Ƿ���Ҫ���������ܣ���Ҫ�Ļ���KEY_CODE_CONITUE1  KEY_CODE_CONITUE2 �д˹���
//#define   NEED_LONGKEY_FUNICTION       1   �Ƿ���Ҫ���������ܣ���Ҫ�Ļ���KEY_CODE_LONG �д˹���
//#define   NEED_COMBTHREE_FUNICTION     1   �Ƿ���Ҫ���3�����ܣ���Ҫ�Ļ���KEY_CODE_COMBTHREE �д˹���

//��Ӳ����������ú���
extern uint16 KeyPadHW_ReadSignal(void);

//��ģ��ĺ���˵��
static uint16  KeyBoard_Filter(uint16 CurrentKey);

/*******************************************************************************
 * Function:   uint16 Read_Keydate(void)
 *
 * Description: ��ȡ��������
 *
 * Returns:     keycode
 *
 * Notes:   Ӧ�ò�ɵ��ô˺�������ü�ֵ �� KEY_CODE_NOΪ�޼�

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
 * Description: ��ȡ����������
 *
 * Returns:     KeyErrCode
 *
 * Notes:   Ӧ�ò�ɵ��ô˺�����ȡ������
		   KEY_ERROR_NONE            ��������
           KEY_ERROR_INVALID         ��Ч��
           KEY_ERROR_LONGDOWN        ĳ��������
           KEY_ERROR_READ	         �����������������
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
 * Description: ���ƺ��ĺ���
 *
 * Returns:     none
 *
 * Notes:     ����Main-LOOP
 *
*******************************************************************************/
void Treat_Read_Keypad(void)
{
	uint16 KeyCodeHW, KeyFilter;
	static uint16 LastKey;  //�ϴ���Ӧ�ļ�ֵ���ϴε��˲���ֵ
	static uint16 KeyDownTimer;   //ͬһ����������ʱ
	static uint16 CombFourTimer;  //4����ʱ
	static uint8 CombFourNum;   //4������
	static uint8 CombFourAcNcfNum;   //ac ѭ��4������
	
	static uint8 ConKeyState;  //����������״̬
	

	KeyCodeHW = KeyPadHW_ReadSignal();
	KeyFilter = KeyBoard_Filter(KeyCodeHW);

	if(KeyFilter == KEY_CODE_TREMBLE)
	{	//��������

	}
	else if ((KeyFilter&KEY_CODE_SPEC) == KEY_CODE_SPEC)
	{

	}
	else if (KeyFilter == KEY_CODE_NO)
	{//�������̣���Ӧ����ֵ
		LastKey = KEY_CODE_NO;
	}
	else if(KeyFilter == LastKey)
	{ //����һ����Ӧ���ļ������Ǵ����ǲ��ǳ�����������������
//#if NEED_CONTINUEKEY_FUNICTION
		if(KeyFilter == KEY_CODE_CONTINUE1 || KeyFilter == KEY_CODE_CONTINUE2)
		{//���������ܴ���
			switch (ConKeyState)
			{ //���ⰴ��״̬���������Ϊ  CONITUEKEY_SEG1_TIME 0.1s����ÿ��CONITUEKEY_SEG2_TIME 0.1s ��Ӧһ��
				default:
					break;
				case  CONKEY_STATE_BEGIN:
					//��ʼ��
					if (TimeOutChkTenthSLong(&KeyDownTimer, CONTINUEKEY_SEG1_TIME))
					{  //��һ�׶�
						ConKeyState = CONKEY_STATE_CONTINUE;
						Respond_Keydata(KeyFilter);
					}
					break;
				case CONKEY_STATE_CONTINUE:
					if (TimeOutChkTenthSLong(&KeyDownTimer, CONTINUEKEY_SEG2_TIME))
					{   //�ڶ��׶Σ�ÿ��һ��ʱ����Ӧһ��
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
			{//���� LONGKEY_TIME ��Ӧ ����ӦֵΪ �˼� ��8λ ȫΪ1
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
		{}//����������������
		//LastKey = KeyFilter;
	}
	else //�����ϴεļ�
	{
		(void)TimeOutChkTenthSLong(&KeyDownTimer,0) ;   //��ʼ��ʱ
		ConKeyState = CONKEY_STATE_BEGIN;
		if(~LastKey & KeyFilter)
      { //��εİ������ϴεļ�ֵ�ж�ļ�������Ӧ����ϼ����ߵ���
//#if NEED_COMBTHREE_FUNICTION
			if(KeyFilter == KEY_CODE_COMBFOUR)
			{//4�������ܿ�ʼ
				if (CombFourNum == 0)
				{//��һ��,��ʼ��ʱ
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
						  //��    COMBFOUR_TIME�⣬�����¼�ʱ
						}
						else
						{ //��Ӧ��ֵ
							Respond_Keydata(KeyFilter);
						}
					}
					else
					{}
				}
			}
			else if(KeyFilter == KEY_CODE_VER_COMBIN)
			{
				//4�������ܿ�ʼ
				if (CombFourAcNcfNum == 0)
				{//��һ��,��ʼ��ʱ
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
						  //��    COMBFOUR_TIME�⣬�����¼�ʱ
						}
						else
						{ //��Ӧ��ֵ
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
		{//û��������������Ӧ

		}
		LastKey = KeyFilter;

		if ((KeyFilter& ~KEY_CODE_COMBFOUR))
		{ //��������ֵ��������0
			CombFourNum=0;
		}
		else{}
		if ((KeyFilter& ~KEY_CODE_VER_COMBIN))
		{ //��������ֵ��������0
			CombFourAcNcfNum=0;
		}
		else{}
	}

}


 /*******************************************************************************
 * Function:   static void Respond_Keydata(uint16 keycode)
 *
 * Description: ��Ӧ keycode  ,����¼��LastActiveKey
 *
 * Returns:
 *
 * Notes:     ��ģ���е��ã���ͬ

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
   {//̫�������һ������ռ��
   	ResKeydata[RESKEY_BUFFERNUM-1] = keycode;
   }
}

/*******************************************************************************
 * Function:    static uint16  KeyBoard_Filter(uint16 CurrentKey)
 *
 * Description:  �˲���ֵ��1 ������������KEY_FILTER_TIME ms
 *                         2 ����������ʱ�����˰�����ȥ KEY_LONG_FILTER_TIME
 *
 * Returns:   ���˺�ļ�ֵ
 *
 * Notes:     ��ģ�龲̬��������ģ���е���

 *
*******************************************************************************/
static uint16  KeyBoard_Filter(uint16 CurrentKey)
{

   static uint16  Keycode_1;   //��¼��һ�εļ�ֵ
   static uint16  KeyfilterTimer;   //��ʱ����
   static uint16  KeyLongTimer;     //��ʱ����
   static uint16  KeyFilter;
   uint16 u16RetKey;
   if (CurrentKey == Keycode_1)
   {
      if (TimeOutChkMsLong(&KeyfilterTimer, KEY_FILTER_TIME))
      {
         KeyFilter = CurrentKey;
      }
      else
      {//�����ϴε�keyfilter
      }

      if(CurrentKey == KEY_CODE_NO)
      {  //�޼�ʱ�ָ�������
         if(TimeOutChkTenthSLong(&KeyLongTimer, NOKEY_LONG_FILTER_TIME))
         {
            LongErrKeyCode = KEY_CODE_NO;
         }
         else
         {}
      }
      else if (TimeOutChkTenthSLong(&KeyLongTimer, KEY_LONG_FILTER_TIME))
      {  //�м�ʱ��KEY_LONG_FILTER_TIME*0.1 S ����Ϊ�ǿ���
         LongErrKeyCode = CurrentKey;
      }
      else
      {}
   }
   else//���� ���¼�ʱ
   {
   	(void)TimeOutChkMsLong(&KeyfilterTimer, 0);
      (void)TimeOutChkTenthSLong(&KeyLongTimer, 0);
      KeyFilter = KEY_CODE_TREMBLE;
   }
   Keycode_1 = CurrentKey;          //��¼�ϴεļ�ֵ
   //���볤�����˼�����

   if(KeyFilter == KEY_CODE_TREMBLE)
   {//�Ƕ����������� �����˼�
   	u16RetKey = KEY_CODE_TREMBLE;
   }
   else
   {
   	u16RetKey = KeyFilter&(~LongErrKeyCode);         //��ȥ����
   }

   return u16RetKey;
}

