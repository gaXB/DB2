/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�ncfmotor.c
* ��������������������ģ�飬����
* 1  ���3·�ĵ���Ŀ��ƹ���
* 
* ��ģ��ʹ����Ҫ��ʱ��ģ��
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭncfmotorģ���2.1��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��5��20��
* 
* �汾��1.1
* ˵�������Ĭ��Ϊ���������趨Ϊ255ʱΪ������
* ���ߣ�xb
* ������ڣ�2020��9��29��
*/
#include "ncfmotor.h"


typedef struct
{
	NcfMotorSetType mSet;    //�趨״̬
   uint8 mState;            //������̿���״̬
   uint16 mTimeCtl;         //�������ʱ����   
}NCFMOTORDATA;

static NCFMOTORDATA  NcfMotorData[MAX_NCFMOTOR_NUM]; //����������ݽṹ

static  uint8  motorTotal = 1;
static  uint16 RunTOut = 150;


/***********************************
 ͨ��Ӳ��������Ҫ���õĺ�������
************************************/
//������
extern void NcfMotorHW_Drive(uint8 mid,uint8 motor_stat);
 /*******************************************************************************
 * Function:    BOOL NcfMotor_Init(uint16 outtime, uint8 nmotor)
 *
 * Description: ���ģ���ʼ������
 *
 * Returns:     ���������0Ϊʧ��
 *
 * Notes:      
			
 *
*******************************************************************************/
BOOL NcfMotor_Init(uint16 outtime, uint8 nmotor)
{
	uint8 bNCFInitRes;
   uint8 mId;
   
	if(nmotor > MAX_NCFMOTOR_NUM || nmotor == 0 || outtime == 0)
	{
		bNCFInitRes = FALSE;
	} 
	else
	{
		RunTOut = outtime;
		motorTotal = nmotor;
		bNCFInitRes = motorTotal;

	   for(mId = 0; mId < motorTotal; mId++)
	   {//��ʼ��ʱ��Ĭ��Ϊstop
	      NcfMotorData[mId].mState = NCFMOTOR_STATE_HALT;
	      NcfMotorData[mId].mSet = NCFMOTOR_POSITION_STOP;
	   }
	}

	return bNCFInitRes;
}


/*******************************************************************************
 * Function:    void NcfMotor_Reset(void)
 *
 * Description: �����ʼ��
 *
 * Returns:     none
 *
 * Notes:     ����������ϵ��ʼ�� 
			
 *
*******************************************************************************/
void NcfMotor_Reset(void)
{
   uint8 mId;
   for(mId = 0;mId < motorTotal; mId++)
   {
      NcfMotorData[mId].mState = NCFMOTOR_STATE_RESET;
   }
}     
     
/*******************************************************************************
 * Function:    void NcfMotor_SetPosition(uint8 motorId, NcfMotorSetType mState)
 *
 * Description: ���õ���趨ֵ
 *
 * Returns:     none
 *
 * Notes:       ������Ҫʱ���ÿ���ʱ���ô˺���			
 *
*******************************************************************************/
void NcfMotor_SetPosition(uint8 motorId, NcfMotorSetType mPostion)
{
   if (motorId < motorTotal) 
   {
   	if (mPostion != NcfMotorData[motorId].mSet)
      {
         NcfMotorData[motorId].mState = NCFMOTOR_STATE_RESET ;   
      }
      else{}
      
      NcfMotorData[motorId].mSet = mPostion; 
   }
   else 
   {//����ʧ��
   	
   }
}

/*******************************************************************************
 * Function:    void NcfMotor_ChangePosition(uint8 motorId)
 *
 * Description: ���õ���趨ֵ
 *
 * Returns:     none
 *
 * Notes:       ������Ҫʱ���ÿ���ʱ���ô˺���			
 *
*******************************************************************************/
void NcfMotor_ChangePosition(uint8 motorId)
{
   if (motorId < motorTotal) 
   {
      if (NcfMotorData[motorId].mSet == NCFMOTOR_POSITION_BACKWARD)
      {
      	NcfMotorData[motorId].mSet = NCFMOTOR_POSITION_FORWARD;
      }
      else
      {
      	NcfMotorData[motorId].mSet = NCFMOTOR_POSITION_BACKWARD;
      }
      NcfMotorData[motorId].mState = NCFMOTOR_STATE_RESET;
   }
   else 
   {
   }
}

 /*******************************************************************************
 * Function:   NcfMotorSetType NcfMotor_GetPosition(uint8 motorId) 
 *
 * Description: �õ���ǰ�����λ��
 *
 * Returns:     NcfMotorSetType
 *
 * Notes:       �κ�ʱ����Ҫʱ�����			
 *
*******************************************************************************/
NcfMotorSetType NcfMotor_GetPosition(uint8 motorId)
{
	NcfMotorSetType u8State;
	if (motorId < motorTotal) 
	{
		u8State = NcfMotorData[motorId].mSet;
   }
   else 
   {
   	u8State = NCFMOTOR_POSITION_INVALID;
   }
	
	return u8State;
}
 /*******************************************************************************
 * Function:  void Motor_Control(void)
 *
 * Description: �����������
 *
 * Returns:     none
 *
 * Notes:    �ú�������main�����е�������ѭ������ 		
 *
*******************************************************************************/
void NcfMotor_Control(void)
{
   uint8 mId;
   NCFMOTORDATA* pMotorData = NcfMotorData;
    
   //�˴�Ϊ����źŴ���,��adģ�鴦��
   for (mId = 0; mId < motorTotal; mId++) 
   {   
   //ö�ٿ������е��
   	switch (pMotorData->mState)
      {    //�������ǰ״̬ɢת
         default:                            //�쳣Ĭ��
            pMotorData->mState = NCFMOTOR_STATE_HALT;
            break;
         case NCFMOTOR_STATE_HALT:              //��ǰΪͣ��״̬
         	NcfMotorHW_Drive(mId, MOTOR_RUN_HALT);
            break;
         case NCFMOTOR_STATE_RUNBYSET:           //��ǰΪ��ת״̬
            //��ʱ15sֹͣ
            if(TimeOutChkTenthSLong(&pMotorData->mTimeCtl, RunTOut)
            		|| pMotorData->mSet == NCFMOTOR_POSITION_STOP)
            {      
               pMotorData->mState = NCFMOTOR_STATE_HALT ;
            }
            else
            {
               if(pMotorData->mSet == NCFMOTOR_POSITION_FORWARD)
               {
               	NcfMotorHW_Drive(mId, MOTOR_RUN_FORWARD);   //1 ��ת
               }
               else  
               {
               	NcfMotorHW_Drive(mId, MOTOR_RUN_BACKWARD);
               }
            }
            break;
         case  NCFMOTOR_STATE_RESET:                   //��ʼ�� 
            //ʱ���ʼ��������
            (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0) ;                                                    
            //����mset�趨
     	      pMotorData->mState = NCFMOTOR_STATE_RUNBYSET; 
         	break;
      }
      pMotorData++;
   }
}





  
       
