/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�Init_Task.c
* ����������SystemState == INIT_TASKʱ ��ʼ���������д˳���
* ��ģ��ʵ��
* 1 �ϵ� 1-2s���˳���ʼ��������������������ֵ
* �������� codewarrior IDE 10.7
* MCU˵���� ��MCU�޹أ��������ø���
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��3��14��
*
*/
#include  "APPDrive.h"

static  uint8   InitState=0;     //�Ƿ��ǵ�һ�ν��뱾���� 0:y 1:n
static  uint16  TimerInitTask;
static  uint8   InitFlg=1;  // ����ʼ������ȫ�����ʼ���ı�ǣ� 1Ϊ ȫ����

#define  INIT_STATE_ALL   0      //ȫ����ʼ��Ϊ10s
#define  INIT_STATE_IGN   1      //IGN ��ʼ��Ϊ2s
#define  INIT_STATE_OVER   2    //�˳�
/*******************************************************************************
 * Function:  void Enter_InitTask(uint8 InitType)
 *
 * Description:  �����ʼ������ʱ���ã������������
 *
 * Returns:     none
 *
 * Notes:     ��Initflg=1 �������ȫ��ʼ����״̬������Ϊign��ʼ���Ĺ��̡�
 *            InitType  1 ȫ����ʼ��  0 ������Ĭ�ϵ�InitFlg������
*******************************************************************************/
void Enter_InitTask(uint8 InitType)
{
   if(InitType)
   { 
	   InitFlg=1;
   }else{}
   
   if(InitFlg)
   {
      InitState =INIT_STATE_ALL;   
   }
   else
   {
      InitState =INIT_STATE_IGN;
   }

   (void)TimeOutChkTenthSLong(&TimerInitTask,0); 
}

/*******************************************************************************
 * Function:   static  void Deal_Keypad_Code(void)
 *
 * Description: ��ǰ����İ�������
 *
 * Returns:     none
 *
 * Notes:       ������ʱֻ��Ҫ�尴��		
 *
*******************************************************************************/
static  void Deal_Keypad_Code(void)
{

  (void)Read_Keydate();        //��ʼ��ʱ�κΰ�����Ч
  (void)   ReadComd();   
   return;
}

/*******************************************************************************
 * Function:    uint8 Run_Init_Task(void)
 *
 * Description: ִ�г�ʼ�����������
 *
 * Returns:     ��Ҫ�˳�ʱ����1 �����򷵻�0
 *
 * Notes:       ��ʼ������1 �尴�� 2 �ر����2s �������ͨ����ģʽ
 *
*******************************************************************************/
uint8 Run_Init_Task(void)
{
	uint8 u8RunEndFlg;
	sPanleData.LCDONOFF = 0;
	sPanleData.AC = 0;
	sPanleData.CIRF = 0;
	sPanleData.FANLevel = 0;
	sPanleData.PTCn = 0;
	sPanleData.RHEAT = 0;
	sPanleData.VentMode = 0;
	sPanleData.VerClient = 0;
	sPanleData.VerSelf = 0;
	sPanleData.i16Temp = 0;
   switch(InitState)
   {
      default:
      case  INIT_STATE_OVER:
         InitFlg=0;
         u8RunEndFlg = 1;
		 break;
      case  INIT_STATE_ALL:
         //���ֻ��Ҫ2s���˳���ֻ��Ҫ�����µ�break ����
       /*  if(TimeOutChkTenthSLong(&TimerInitTask,100))
         {
            InitState =INIT_STATE_OVER;

         }
         SetLcdDisplayMode(LCD_MODE_ALL);
         TrOut_BLed(1);
         break;*/
   case   INIT_STATE_IGN:
         if(TimeOutChkTenthSLong(&TimerInitTask,10))
         {
            InitState =INIT_STATE_OVER;  
         }
         SetLcdDisplayMode(LCD_MODE_CLEAR);
         u8RunEndFlg = 0;
         break;
   }
 
   Deal_Keypad_Code();
   return u8RunEndFlg;
}












