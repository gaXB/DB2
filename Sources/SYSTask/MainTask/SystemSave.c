/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�SystemSave.c
* ������������ģ�����ʵ��ϵͳӦ����ش洢���书��,���д洢�����ܷ���flash��,ֻ��noint ������ʵ��,
*  ��MCU ��ء��������¹���:
* 1  off���䣨����ram�У�
* 2  def ���䣨����ram�У������г�˪���������书��void Def_Operat(uint8 OPType)
* 3  lohiǰ�� ���䣨ram�� 
* 
* �������� codewarrior IDE 11.1
* MCU˵������MCU �޹�,��ģ��ֻ��Ϊģ��
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2019��3��14��
* 
* �汾��1.1
* ˵��������saveDID �ķ�ʽ��Ϊͨ��PID �Զ��洢
* ���ߣ�xb
* ������ڣ�2019��3��14��
* 
* * �汾��1.2
* ˵������flash��صļ������SystemSaveFlash ģ��,����ram�洢���ڴ�����
* ���ߣ�xb
* ������ڣ�2019��9��5��
* 
* �汾��1.3
* ˵��������Ĭ��ֵ����
* ���ߣ�xb
* ������ڣ�2020��11��21��
* 
* �汾��1.4
* ˵����1 ö�����͸�ֵ�Ż�
* 2 ����renesas
* ���ߣ�xb
* ������ڣ�2021��8��23��
*/
//TODO: Ӧ�ü���Ĺ��ܷ��ڴ˺�������ģ���ṩ��off def park�ȼ����ģ�壬�밴ʵ��������б�д
#include "SystemSave.h"


//��ر���
#if  NOINIT_TYPE ==  NOINIT_TYPE_KEA
__attribute__ ((section(".noinit"))) static SYSTEMCONTROL SystemOffState, SystemDefState, SystemLOHIState, SystemMaxACState;   //offʱ�ļ����defʱ�ļ���
__attribute__ ((section(".noinit"))) static SYSTEMCONTROL SystemParkState;
__attribute__ ((section(".noinit"))) static uint8 OffChkSum, DefChkSum, LOHIChkSum, MaxACChkSum;      //���洢����ram �н��д洢
__attribute__ ((section(".noinit"))) static uint8 ParkChksum;

__attribute__ ((section(".noinit"))) static struct
{
	uint8 MODE;
	uint8 FAN;
	uint8 AC;
	uint8 NCF;
}SaveDefFlg;
#elif NOINIT_TYPE == NOINIT_TYPE_RL78
#pragma section bss NOINIT
static SYSTEMCONTROL SystemOffState, SystemDefState, SystemLOHIState, SystemMaxACState;   //offʱ�ļ����defʱ�ļ���
static SYSTEMCONTROL SystemParkState;
static uint8 OffChkSum, DefChkSum, LOHIChkSum, MaxACChkSum;      //���洢����ram �н��д洢
static uint8 ParkChksum;
static struct
{
	uint8 MODE;
	uint8 FAN;
	uint8 AC;
	uint8 NCF;
}SaveDefFlg;
#pragma section 
#define __GOLBAL_VAR_
#endif


/*******************************************************************************
 * Function:      static  void SaveErr_GotoDefault(void)
 *
 * Description:    ������󷵻�Ĭ��ת̨
 *
 * Returns:         none
 *
 * Notes:        none
 *
********************************************************************************/
static  void SaveErr_GotoDefault(void)
{
#if _PROJECT_AUTO_
   SystemControl.AutoBits.AC = 1;
   SystemControl.AutoBits.NCF = 1;
   SystemControl.AutoBits.FAN = 1;
   SystemControl.AutoBits.MODE= 1;
#endif
}
/*******************************************************************************
 * Function:        BOOL Chkeck_SaveSystemControl(SYSTEMCONTROL *pSystem, uint8 ChkSum) 
 *
 * Description:     �ж�У�����Ƿ���ȷ
 *
 * Returns:         ��ȷ TRUE  ���� FALSE
 *
 * Notes:         pSystem Ϊ�����ϵͳ״̬�֣�ChkSum  pSystem��У����  
 *
********************************************************************************/
static BOOL Chkeck_SaveSystemControl(SYSTEMCONTROL *pSystem, uint8 ChkSum) 
{
   uint8  Num=0;
   uint8*  pSaveChar = (uint8*)pSystem;
   uint8 bCheck;
   for(Num=0; Num< sizeof(SYSTEMCONTROL);Num++)
   {
      ChkSum += *pSaveChar;
      pSaveChar++;  
   }
   if (ChkSum == 255)
   {//������ȷ
   	bCheck = TRUE;
   }
   else
   {
   	bCheck = FALSE;
   }
   
   return bCheck;
}

/*******************************************************************************
 * Function:        uint8 CalSys_ChkSum(SYSTEMCONTROL *pSystem) 
 *
 * Description:     ����У����
 *
 * Returns:         У����
 *
 * Notes:         
 *
********************************************************************************/
static uint8 CalSys_ChkSum(SYSTEMCONTROL *pSystem) 
{
   uint8  SaveNum=0;
   uint8*  pSaveChar = (uint8*)pSystem;
   
   uint8 ChkSum =0;
   for(SaveNum=0; SaveNum<sizeof(SYSTEMCONTROL);SaveNum++)
   {
      ChkSum += *pSaveChar;
      pSaveChar++;  
   }
   ChkSum = ~ChkSum;
   return ChkSum;
}

/*******************************************************************************
 * Function:        void Save_Off (void) 
 *
 * Description:     off ʱ����ϵͳ
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Save_Off (void)
{
	SystemOffState = SystemControl;

   OffChkSum = CalSys_ChkSum(&SystemOffState);
}

/*******************************************************************************
 * Function:       void Load_Off (void)
 *
 * Description:     off �˳�ʱ����
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Load_Off (void)
{
   if( Chkeck_SaveSystemControl(&SystemOffState, OffChkSum))
   {  //������ȷ��û�ж�ʧ
      SystemControl.acMode  =  SystemOffState.acMode;
      SystemControl.fanMode =  SystemOffState.fanMode;
      //SystemControl.mdMode =   SystemOffState.mdMode;  //����Ҫ����mode״̬��
      SystemControl.ncfMode =  SystemOffState.ncfMode;
      SystemControl.AutoBits = SystemOffState.AutoBits;
   }
   else
   {  //������󣬶�ʧ
   	//SaveErr_GotoDefault();
   }
   SystemControl.OffState = OFF_STATE_ON;
}

/*******************************************************************************
 * Function:        void Save_Off (void) 
 *
 * Description:     Park ʱ����ϵͳ
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Save_Park (void)
{
	SystemParkState = SystemControl;

	ParkChksum = CalSys_ChkSum(&SystemParkState);
}

/*******************************************************************************
 * Function:       void Load_Off (void)
 *
 * Description:     Park �˳�ʱ����
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Load_Park (void)
{
	 if( Chkeck_SaveSystemControl(&SystemParkState, ParkChksum))
	   {  //������ȷ��û�ж�ʧ
	      SystemControl.acMode  =  SystemParkState.acMode;
	      SystemControl.fanMode =  SystemParkState.fanMode;
	      SystemControl.mdMode =   SystemParkState.mdMode; 
	      SystemControl.ncfMode =  SystemParkState.ncfMode;
	      SystemControl.AutoBits = SystemParkState.AutoBits;
	   }
	   else
	   {  //������󣬶�ʧ
	   	SaveErr_GotoDefault();
	   }
	   SystemControl.ParkAC = 0;
	
}

/*******************************************************************************
 * Function:        void Save_Def (void)
 *
 * Description:     def ʱ����ϵͳ
 *
 * Returns:         none
 *
 * Notes:           none
 *
********************************************************************************/
void Save_Def (void)
{
   SystemDefState = SystemControl;

   DefChkSum = CalSys_ChkSum(&SystemDefState);
   
   SaveDefFlg.AC = 1;
   SaveDefFlg.FAN = 1;
   SaveDefFlg.MODE = 1;
   SaveDefFlg.NCF = 1;
}

void Def_Operat(uint8 OPType)
{
	switch(OPType)
	{
		default:
			break;
		case DEF_OPTYPE_AC:
			SaveDefFlg.AC = 0;
			break;
		case DEF_OPTYPE_NCF:
			SaveDefFlg.NCF = 0;
			break;
		case DEF_OPTYPE_MODE:
			SaveDefFlg.MODE = 0;
			break;
		case DEF_OPTYPE_FAN:
			SaveDefFlg.FAN = 0;
			break;
	}
} 
/*******************************************************************************
 * Function:       void Load_Def (void)
 *
 * Description:     def �˳�ʱ����
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Load_Def (void)
{
   if( Chkeck_SaveSystemControl(&SystemDefState, DefChkSum))
   {  //������ȷ��û�ж�ʧ
      if (SaveDefFlg.AC)
      {
      	SystemControl.acMode  =  SystemDefState.acMode;
      	SystemControl.AutoBits.AC = SystemDefState.AutoBits.AC;
      }
      
      if (SaveDefFlg.MODE)
      {
      	SystemControl.mdMode  =  SystemDefState.mdMode;
      	SystemControl.AutoBits.MODE = SystemDefState.AutoBits.MODE;
      }
      
      if (SaveDefFlg.FAN)
      {
      	SystemControl.fanMode  =  SystemDefState.fanMode;
      	SystemControl.AutoBits.FAN = SystemDefState.AutoBits.FAN;
      }
      
      if (SaveDefFlg.NCF)
      {
      	SystemControl.ncfMode  =  SystemDefState.ncfMode;
      	SystemControl.AutoBits.NCF = SystemDefState.AutoBits.NCF;
      } 
   }
   else
   {  //������󣬶�ʧ
   	SaveErr_GotoDefault();
   }
   if (SystemControl.mdMode == MD_MODE_OST)
   {//����
   	SystemControl.mdMode = MD_MODE_OSF;
   	SystemControl.AutoBits.MODE = 0;  
   }
   SystemControl.OffState = OFF_STATE_ON;
}

/*******************************************************************************
 * Function:       void LoadMode_BeforDef(void)
 *
 * Description:     ��������˪ǰ��ģʽ��
 *
 * Returns:         none
 *
 * Notes:           �˳�off��ʱ��Ҫʹ�ô˺���			
 *
********************************************************************************/
void LoadMode_BeforDef(void)
{
	if( Chkeck_SaveSystemControl(&SystemDefState, DefChkSum))
	{
		SystemControl.mdMode = SystemDefState.mdMode;
	}
	else
	{
		SystemControl.mdMode = MD_MODE_OSF;
	}
}

/*******************************************************************************
 * Function:        void Save_LOHI (void) 
 *
 * Description:     LOHI ʱ����ϵͳ
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Save_LOHI (void) 
{
   SystemLOHIState = SystemControl; 

   LOHIChkSum = CalSys_ChkSum(&SystemLOHIState);  
}

/*******************************************************************************
 * Function:       void Load_LOHI (void)
 *
 * Description:     LOHI �˳�ʱ����
 *
 * Returns:         none
 *
 * Notes:           none			
 *
********************************************************************************/
void Load_LOHI (void)
{  
   if( Chkeck_SaveSystemControl(&SystemLOHIState, LOHIChkSum) )
   {  //������ȷ��û�ж�ʧ
      SystemControl.acMode  =  SystemLOHIState.acMode;  
      SystemControl.fanMode =  SystemLOHIState.fanMode;  
      SystemControl.mdMode =   SystemLOHIState.mdMode;
      SystemControl.ncfMode =  SystemLOHIState.ncfMode;
      SystemControl.AutoBits = SystemLOHIState.AutoBits;
   }        
   else
   {  //������󣬶�ʧ
   	SaveErr_GotoDefault();
   }
   SystemControl.OffState = OFF_STATE_ON;   
  
}

void Save_MaxAC(void)
{
	SystemMaxACState = SystemControl;
	MaxACChkSum = CalSys_ChkSum(&SystemMaxACState);
}

void Load_MaxAC(void)
{
   if( Chkeck_SaveSystemControl(&SystemMaxACState, MaxACChkSum))
   {  //������ȷ��û�ж�ʧ
	  SystemControl.tset  = SystemMaxACState.tset;
	  SystemControl.acMode  =  SystemMaxACState.acMode;
	  SystemControl.fanMode =  SystemMaxACState.fanMode;
	  SystemControl.mdMode  =   SystemMaxACState.mdMode; 
	  SystemControl.ncfMode =  SystemMaxACState.ncfMode;
	  SystemControl.AutoBits = SystemMaxACState.AutoBits;
   }
   else
   {  //������󣬶�ʧ
   	SaveErr_GotoDefault();
   }
   SystemControl.OffState = OFF_STATE_ON;
   SystemControl.MaxACMode = 0;
}



