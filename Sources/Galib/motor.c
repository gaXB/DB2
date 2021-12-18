/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�motor.c
* �����������������ģ�飬����
* 1  ���4·�ĵ���Ŀ��ƹ���
* 2  ���п����õĶ�ת����
* 3  ���ж�·��������
* 4  �����Լ���Թ���
* 
* ��ģ��ʹ����Ҫ��ʱ��ģ�鼰��ѧ��������ģ��
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭmotorģ���3.2��ֲ�����Ӷ�ʱ����
* ԭ���ߣ�xb
* ������ڣ�2019��5��20��
* 
* �汾��1.1
* ˵��������ͣ�����������趨λ��=255ʱ ���ͣ��
* ԭ���ߣ�xb
* ������ڣ�2020��9��28��
* 
* �汾��1.2
* ˵����
* 1 ���������ʱδ����DriveState��bug�޸�,����AD MOTOR_STOPSHAKE_AD ��2->3
* 2 �������ֵ-Ĭ��ֵ��3��Ϊ5
* ԭ���ߣ�xb
* ������ڣ�2020��12��2��
* 
* �汾��1.3
* ˵����
* 1 ����ֵMOTOR_DRIVE_ACCURACY �ɺ궨���Ϊint16,Ĭ��5
* 2 ��ǰ��Ĭ��Ϊ1��������
* 3 ����MotorDriveInit ���趨���Ⱥ���ǰ��
* ���ߣ�xb
* ������ڣ�2020��12��5��
* 
*/

#include "motor.h" 
#include "mathGA.h"
 
typedef struct
{
   uint8 mSet;         //λ���趨ֵ
   uint8 mGet;         //��ǰλ��
   uint8 mGetOld;      //��һ�ε�ֵ       
   uint8 mState;       //������̿���״̬
   uint8 Work;
   uint8 mErrorCode;     //���ϴ���
   uint8 mTestError;     //���ϴ���
   uint16 mTimeCtl;      //�������ʱ���� ,���ת��ʱ��
   uint16 mTimeCtl_1;     //�������ʱ��1 �������Ĵ���ʱ�����
   uint8 nMotorJ;        //�����ץ����
   uint8 nShortRecove;   //��·�ָ��Ĵ��� �����������ȷλ��ʱ����0
   uint8 nShakeTimes;    //�������� ��motor_reset �ſ�����0
   uint8 mShortErr;     //���ϴ���
   uint8 mShortTimes;   //���ϴ���
}MOTORDATA; 

MOTORDATA motorData[MAX_MOTOR_NUM]; //����������ݽṹ
static  uint8  MaxJamed = 4, motorTotal = 3;
static  uint16 RunTOut = 150;

int16 MOTOR_DRIVE_ACCURACY = DEFAULT_MOTOR_DRIVE_ACCURACY;   // �������
int8  _admotor = DEFAULT_MOTOR_BEFORDATA;          // �����ǰ��


/*******************************************************************************
 ͨ���ⲿ��Ҫ���õĺ�������
**********************************************************************************/
extern void MotorHW_Drive(uint8 mid,uint8 motor_stat); //������
extern uint8 MotorHW_GetAD(uint8 MotorId);
extern int16 MotorHW_SupplyVolt(void);
/*******************************************************************************
 * Function:    uint8 Motor_Init(uint8 tout,uint8 njamde,uint8 );
 *
 * Description: ���ģ���ʼ������
 *
 * Returns:     ���������0Ϊʧ��
 *
 * Notes:      
			
 *
*******************************************************************************/
uint8 Motor_Init(uint16 outtime,uint8 njame,uint8 nmotor)
{
   uint8 i;
	if (nmotor > MAX_MOTOR_NUM || !nmotor || !njame || !outtime)
   {
      return 0;
   }
   RunTOut = outtime;
   MaxJamed = njame;
   motorTotal = nmotor;
   for (i = 0; i < motorTotal; i++)
   {//20200928 Ĭ�ϸ���ͣ��
   	motorData[i].mSet = MOTOR_STOP_AD;
   	motorData[i].Work = MOTOR_WORK_STOP;
   }
   return motorTotal;
}


/*******************************************************************************
 * Function:    BOOL MotorDriveInit(uint8 u8AccData, uint8 u8BeforData)
 *
 * Description: ���ģ��������ʼ�������Ⱥ���ǰ��
 *
 * Returns:    u8AccData (2-9)��  u8BeforDataС��5, u8AccData > u8BeforData���ܷ���TRUE,
 *
 * Notes:      �����ã�Ĭ��ֵ��5,1
			
 *
*******************************************************************************/
BOOL MotorDriveInit(uint8 u8AccData, uint8 u8BeforData)
{
	BOOL bDriveInit;
	if (u8AccData >= 10 || u8AccData <= 1 || u8BeforData >=5 || u8AccData <= u8BeforData)
	{
		bDriveInit = FALSE;
	}
	else
	{
		MOTOR_DRIVE_ACCURACY = u8AccData;
		_admotor = u8BeforData;
		bDriveInit = TRUE;
	}

	return bDriveInit;
}
/*******************************************************************************
 * Function:    uint8 Motor_SelfTset(uint8 id);
 *
 * Description: ����Լ�����
 *
 * Returns:     0-1
 *
 * Notes:     id���Լ� 
			
 *
*******************************************************************************/
uint8 Motor_SelfTset(uint8 id)
{
    if (id >= motorTotal) return 0;
    motorData[id].Work = MOTOR_WORK_SELFTEST;
    motorData[id].mState = MOTOR_STATE_TEST_ENTER;
    return 1;
}
      
/*******************************************************************************
 * Function:    uint8 Motor_Rset(uint8 id);
 *
 * Description: �����ʼ��
 *
 * Returns:     none
 *
 * Notes:     ����������ϵ��ʼ�� 
			
 *
*******************************************************************************/
void Motor_Reset(void)
{
   uint8 mId;
   MOTORDATA  *pMotorData = motorData;

   for (mId=0; mId<motorTotal; mId++)
   {
      pMotorData->mState =MOTOR_STATE_RESET;
      pMotorData->Work =   MOTOR_WORK_NORMAL;
      pMotorData++;
   }
}     
     
/*******************************************************************************
 * Function:    BOOL Motor_SetPosition( uint8 motorId,uint8 mPosition)
 *
 * Description: ���õ���趨ֵ
 *
 * Returns:     ���óɹ�Ϊ1 ������Ϊ0
 *
 * Notes:       ������Ҫʱ���ÿ���ʱ���ô˺���			
 *
*******************************************************************************/
BOOL Motor_SetPosition(uint8 motorId, uint8 mPosition)
{

   MOTORDATA  *pMotorData = &motorData[motorId]; 
   
   //�����趨���ϵ�ѹ��Χ��
   if (mPosition == MOTOR_STOP_AD)
   {
   	pMotorData->mSet = MOTOR_STOP_AD;
   	if (pMotorData->Work != MOTOR_WORK_SELFTEST)
   	{
   		pMotorData->mState = MOTOR_WORK_STOP;
   	}else{}
   	return 1;
   }
   if (mPosition <= MOTOR_SHORT_AD)
   {
      mPosition =  MOTOR_SHORT_AD+1;
   }
   if (mPosition >= MOTOR_OPEN_AD)
   {
      mPosition =  MOTOR_OPEN_AD -1; 
   }
   
   if (motorId < motorTotal ) 
   {
   	if (GAMath_abs( (int16)pMotorData->mSet - mPosition) > MOTOR_SET_AD)
   	//if (pMotorData->mSet != mPosition) 
      {        
      //���ϴ��趨ֵ����ȣ��������´���  
         if ( pMotorData->Work == MOTOR_WORK_NORMAL)
         {   //����������ʱλ�������趨ʱ���������ʼ��״̬
            pMotorData->mState = MOTOR_STATE_RESET ;            
         }
         else  if (pMotorData->Work == MOTOR_WORK_SHORT) 
         {    //�ڶ�·ͣ��ʱ���趨λ�ñ仯����DEAL_SHORTRECOVE_AD ʱ �������·�ָ�����
            #if   NEEDFUNCTION_SHORT_AUTORECOVEY
               if ( (pMotorData->mSet > mPosition && (pMotorData->mSet - mPosition) > DEAL_SHORTRECOVE_AD)||
                    (pMotorData->mSet < mPosition && (mPosition - pMotorData->mSet) > DEAL_SHORTRECOVE_AD) )
               {
                  //������趨λ�ò���� DEAL_SHORTRECOVE_AD �ſ��Զ���,���򷵻�0 ˵������ʧ��
                  pMotorData->mState = MOTOR_STATE_SHORT_ENTER;
                  pMotorData->nShortRecove = 0;
               }
               else
               {
                  return (0);
               }
            #endif
         }
         else 
         {//����ģʽֻ�趨λ��
         	
         }
         pMotorData->mSet = mPosition;
      }
      return(1);   
   }
   else
   {
      return(0);
   }
}

/*******************************************************************************
 * Function:   uint8 Motor_GetPosition(uint8 motorId) 
 *
 * Description: �õ���ǰ�����λ��
 *
 * Returns:     0-255
 *
 * Notes:       �κ�ʱ����Ҫʱ�����			
 *
*******************************************************************************/
uint8 Motor_GetPosition(uint8 motorId)
{
   if (motorId <  motorTotal) 
   {
      return(motorData[motorId].mGet);
   }
   else 
   {
      return(0);
   }
}
//���Ե��ģ��ʱʹ��
uint8 Motor_GetSetPosition(uint8 motorId)
{
   if (motorId <  motorTotal) 
   {
      return(motorData[motorId].mSet);
   }
   else 
   {
      return(0);
   }
}


 /*******************************************************************************
 * Function:  uint8 Motor_GetErrorCode(uint8 motorId ) 
 *
 * Description: ���ص�����ϴ���
 *
 * Returns:     MOTOR_ERROR
 *
 * Notes:       		
 *
*******************************************************************************/
uint8 Motor_GetErrorCode(uint8 motorId)
{
   if (motorId < motorTotal) 
   {
      return(motorData[motorId].mErrorCode);
   }
   else 
   {
      return(MOTOR_ERROR_INVALID);
   }
}

uint8 Motor_GetSeltTestCode(uint8 motorId)
{
   if (motorId < motorTotal)
   {
   	return(motorData[motorId].mTestError);
   }
   else
   {
      return(MOTOR_ERROR_INVALID);
   }
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
void Motor_Control(void)
{
    static  uint16  TimerMotorRun;
	 uint8   mId;               // �����
    int16   mdiffGet ;         // ��εķ�����ѹ���ϴεĲ�
    int16   difmset;           // �趨ֵ�ڵ�ǰ�ķ���ֵ�ò�
    uint16  mTimeout;          // �������ʱ��
    uint8   DriveState = MOTOR_RUN_HALT;      // �������״̬
    MOTORDATA  *pMotorData;    // �������ָ��


   if (TimeOutChkMsLong(&TimerMotorRun, MOTOR_RUN_TIME) == FALSE)
   {
   	return;
   }
   pMotorData = motorData;
   for (mId = 0; mId < motorTotal; mId++) 
   {   //ö�ٿ������е��
   
   	//20160412 ���ӵ�� �趨 λ�÷�ΧΪ MOTOR_OPEN_AD MOTOR_SHORT_AD
   	if (pMotorData->mSet < MOTOR_SHORT_AD)
   	{
   		pMotorData->mSet = MOTOR_SHORT_AD + 2;
   	}
   	
   	if (pMotorData->mSet > MOTOR_OPEN_AD)
   	{
   		pMotorData->mSet = MOTOR_OPEN_AD - 2;
   	}
   	
      //�õ����ad�����ж��Ƿ����
      pMotorData->mGet = MotorHW_GetAD(mId);
#if NEEDFUNCTION_SHORT_AUTORECOVEY 
      //��������Ը���
      if (pMotorData->mGet > MOTOR_OPEN_AD || pMotorData->mGet < MOTOR_SHORT_AD)
      {
      	if (pMotorData->mShortTimes < MOTORERR_SHORT_TIMES)
      	{
      		pMotorData->mShortTimes++;
      	}
      	else
      	{
            if (pMotorData->mGet > MOTOR_OPEN_AD)
            {  
            	pMotorData->mShortErr = 2;
            }
            else
            {
            	pMotorData->mShortErr = 1;
            }
      	}
      }
      else
      {
      	if (pMotorData->mShortTimes)
      	{
      		pMotorData->mShortTimes--;
      	}
      	else
      	{
      		pMotorData->mShortErr = 0;
      	}
      }
#endif
      
      difmset = pMotorData->mGet - pMotorData->mSet;        //�õ���ǰ���趨�Ĳ�и���
   
      mdiffGet = pMotorData->mGet - pMotorData->mGetOld;    //����һ�εĲ�ֵ
      
      switch (pMotorData->Work)
      {
         default:
            pMotorData->Work = MOTOR_WORK_NORMAL;
         /**************��������*********************************************************/
         /*********************************************************************************************
         ��������ʵ�� 1 ��ͨ�ĵ������ 2 ��ת����  3 ��תʱ 2s����20sͣ�Ĺ���
         4 �������Ϲ��ܡ�  5 ��·��·ʱ���� ��·����ģʽ
         ************************************************************************************/  
         case  MOTOR_WORK_NORMAL:
         	if (pMotorData->mSet == MOTOR_STOP_AD)
         	{
         		pMotorData->mState = MOTOR_WORK_STOP;
         	}
         	else{}
			#if NEEDFUNCTION_SHORT_AUTORECOVEY
         	if (pMotorData->mShortErr)
            {
               pMotorData->Work = MOTOR_WORK_SHORT;
               pMotorData->mState = MOTOR_STATE_SHORT_ENTER;          //��ʼ��ʱ���Ͻ�����ϵ���
               (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0);
               //���ֹ�������
               if (pMotorData->mShortErr == 1)
                  pMotorData->mErrorCode = MOTOR_ERROR_SHORT; 
               else pMotorData->mErrorCode = MOTOR_ERROR_OPEN;
               
               DriveState = MOTOR_RUN_HALT;//��·��·��������ֹͣ���
               break;
            }
			#endif
            //else
            //{
            //   pMotorData->mErrorCode ^= ~MOTOR_ERROR_SHORT ;  //����˹�����
            //}
            switch (pMotorData->mState) 
            {    //�������ǰ״̬ת��
               default:                            //�쳣Ĭ��
                  pMotorData->mState = MOTOR_STATE_WAITHALT;
               case  MOTOR_STATE_WAITHALT:
               //���������ת�л�������ת����ֹͣ����Ҫ�ڽ����״̬��ֹͣ0.5s��
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl_1, MOTOR_RUNHATL_LEASTTIME) )
                  {
                     pMotorData->mState = MOTOR_STATE_HALT;    
                  }   
                  DriveState = MOTOR_RUN_HALT;                       
                  break;
               case MOTOR_STATE_HALT:              //��ǰΪͣ��״̬  
               	if (NEEDFUNCTION_DEALSHAKE == 0 ||
                   (mdiffGet <= MOTOR_STOPSHAKE_AD && mdiffGet >= -MOTOR_STOPSHAKE_AD))
                  { //���ֹͣʱ��������ʱ�������
                  //   if(pMotorData->nErrRecove<MAX_ERECOVE_TIMES)//���Թ��ϵ�������Ĵ���
                 //    {//�˴�Ϊ�˷�ֹ��������˺�ָ����ֹ��Ϲ���Ƶ����ʵ����û�д�����
                        //ֻ���ڵ��û�ж�����ʱ����ܽ������
							if (difmset < MOTOR_DRIVE_ACCURACY && difmset > -MOTOR_DRIVE_ACCURACY)
							{//�ﵽ����
								pMotorData->mErrorCode = MOTOR_ERROR_NONE;  
								pMotorData->nShortRecove = 0;
							}
							else if (difmset>0)
							{
								pMotorData->mState = MOTOR_STATE_BACKWARD;  
							}
							else
							{
								pMotorData->mState = MOTOR_STATE_FORWARD;     
							}
                  }
                  else
                  {
                     pMotorData->mErrorCode = MOTOR_ERROR_SHAKE;
                     pMotorData->mState = MOTOR_STATE_SHAKE;  
                  }
                  DriveState = MOTOR_RUN_HALT;
                  break;
               case MOTOR_STATE_FORWARD:           //��ǰΪ��ת 
                  //����λ��
                  DriveState = MOTOR_RUN_FORWARD;
                 
                  #if (NEEDFUNCTION_DEALSHAKE >=2)
                     if (mdiffGet < -MOTOR_RUNSHAKE_AD || mdiffGet > MOTOR_JUMP_AD)
                     {//��תʱ�����ad��С˵�������⣬���޷����� �����ߵ����ѹ�仯����
                       // pMotorData->ShakeETimes++;      
                        pMotorData->mState = MOTOR_STATE_HALT; 
                        DriveState = MOTOR_RUN_HALT; 
                     }
                  #endif
                  
                  if (difmset >= -_admotor)
                  {//�ﵽλ����ֹͣ
                     pMotorData->mState = MOTOR_STATE_WAITHALT;
                     (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl_1, 0); //ΪMOTOR_STATE_WAITHALTʱ����ƣ�mTimeCtl�������Ƶ����ʱ��������Ҫ��mTimeCtl_1
                     DriveState = MOTOR_RUN_HALT;  
                  }
                  else
                  {
                     if (pMotorData->nMotorJ == 0) 
                     {  //�趨ת��ʱ��              
                        mTimeout = RunTOut;            //��һ��Ϊ40s
                     }
                     else
                     {
                        mTimeout = 20;        //�Ժ�Ϊ2s
                     }
                     //��ʱ����δ�������
                     if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, mTimeout))
                     {      
                        pMotorData->mState = MOTOR_STATE_JAMMED_ERROR ;  
                        pMotorData->mErrorCode = MOTOR_ERROR_JAMMED;   //��ʱ����δ�������ת״̬
                        pMotorData->nMotorJ++;
                        DriveState = MOTOR_RUN_HALT;
                        break;
                     }
                  }
                  break;
               case MOTOR_STATE_BACKWARD:          //��ǰΪ��ת״̬
                  //����λ��
                  DriveState = MOTOR_RUN_BACKWARD;
                  
                  #if (NEEDFUNCTION_DEALSHAKE >= 2)
                     if ( mdiffGet > MOTOR_RUNSHAKE_AD || mdiffGet<-MOTOR_JUMP_AD)
                     {//��תʱ�����ad���˵�������⣬1���޷�����.���ߵ����ѹ�仯���� 
                      //  pMotorData->ShakeETimes++;      
                        pMotorData->mState = MOTOR_STATE_HALT;  
                        DriveState = MOTOR_RUN_HALT;
                     }
                  #endif
                  
                  if (difmset < _admotor)
                  {
                     pMotorData->mState = MOTOR_STATE_WAITHALT; 
                     (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl_1, 0);
                     DriveState = MOTOR_RUN_HALT;
                  }
                  else
                  {
                     if(pMotorData->nMotorJ == 0) 
                     {  //�趨ת��ʱ��              
                        mTimeout= RunTOut;            //��һ��Ϊ40s
                     }
                     else
                     {
                        mTimeout=20;        //�Ժ�Ϊ2s
                     }
                     //��ʱ����δ�������
                     if(TimeOutChkTenthSLong(&pMotorData->mTimeCtl, mTimeout))
                     {      
                        pMotorData->mState = MOTOR_STATE_JAMMED_ERROR ;  
                        pMotorData->mErrorCode = MOTOR_ERROR_JAMMED;   //��ʱ����δ�������ת״̬
                        pMotorData->nMotorJ++;
                        DriveState = MOTOR_RUN_HALT;
                        break;
                     }
                  }
                  break;
                  
               case MOTOR_STATE_JAMMED_ERROR:             //��ǰΪ��ת����״̬
                  if (difmset < MOTOR_DRIVE_ACCURACY && difmset > -MOTOR_DRIVE_ACCURACY)
                  {    //����Χ�ڣ��������
                     pMotorData->mState = MOTOR_STATE_HALT ;
                     pMotorData->mErrorCode = MOTOR_ERROR_NONE;
                     break;
                  }
                  if (pMotorData->nMotorJ >= MaxJamed)
                  {   //��ת�����ﵽ���
                     pMotorData->mErrorCode = MOTOR_ERROR_JAMMED;   
                  }
                  else 
                  {  //û�е���������ֹͣ20s����ת��
                      if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl,100)) 
                      {//jac 10s
                         if (difmset<0)   //���¶�λ
                            pMotorData->mState = MOTOR_STATE_FORWARD; 
                         else   pMotorData->mState = MOTOR_STATE_BACKWARD; 
                      }
                  }
                  DriveState = MOTOR_RUN_HALT;
                  break;
               case  MOTOR_STATE_SHAKE:
						#if (NEEDFUNCTION_DEALSHAKE >= 1)
                     if (mdiffGet <= MOTOR_STOPSHAKE_AD && mdiffGet >= -MOTOR_STOPSHAKE_AD) 
                     {    //û�й��ϣ��ȴ� 1s�����ع��ϼ���ת��
                        if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl_1, MOTOR_SHAKEERROR_WAITTIME))
                        {
                           pMotorData->mState = MOTOR_STATE_HALT; 
                           pMotorData->nShakeTimes++;  
                        }
                     }
                     else
                     {
                        (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl_1, 0);
                     }
                  #else
                      pMotorData->mState = MOTOR_STATE_HALT; 
                  #endif
                  DriveState = MOTOR_RUN_HALT;   
                  break;
               case  MOTOR_STATE_RESET:                   //��ʼ��

                  //ʱ���ʼ��������
                  (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0);
                  pMotorData->nMotorJ = 0;                                                        
                  pMotorData->nShakeTimes =0;
                  //����ת���趨
                  //2014 10 05 �޸�,ֻҪ�����״̬�ж��Ƿ�һ������һ���Ϳ�ʼ���� 
           		   if ( !difmset ) 
           		   {   //��ʼ��ʱ����趨�뵱ǰλ�ò��С��ֹͣ
             		   pMotorData->mState = MOTOR_STATE_HALT ;
                     pMotorData->mErrorCode = MOTOR_ERROR_NONE;
             	   }
             		else
             		{	
             	      if(difmset<0)       //ȷ������ת
             	         pMotorData->mState=MOTOR_STATE_FORWARD; 
             	      else   pMotorData->mState=MOTOR_STATE_BACKWARD;
             		}
             		DriveState = MOTOR_RUN_HALT;
             		break;   

            }
            break;
         /**************��������end*********************************************************/
		#if NEEDFUNCTION_SHORT_AUTORECOVEY      
         case  MOTOR_WORK_SHORT:  
         /**************��·���� *********************************************************/
         /************************************************************************************
         ��·�ָ�����Ϊ 1 �ȴ�2s  2 ��ת2s  ����ת,  ���������ѹ 3 �ȴ�2s��û�й�����ָ�����������
         ��ѹ�������� 4 ��������ͣ��״̬  ֱ���趨λ�ñ仯��
         ���NEEDFUNCTION_SHORT_AUTORECOVEY  ==0 �� ����ͣ�����ָ���������(��Ϊ�������)
         ************************************************************************************/   
            switch(pMotorData->mState)
            {
               default:
               case  MOTOR_STATE_SHORT_ENTER:

                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, MOTOR_SHORTERROR_WAITTIME))
                  {//��2s��ʼ������20171024 ���ļ��ٵȴ�ʱ��0.5s ��Ϊ�����ж��Ѿ�������ʱ 
                     if (pMotorData->mShortErr == 0)
                     { //��������,�����·�ȴ�״̬
                        pMotorData->mState = MOTOR_STATE_SHORT_GETPOSTION;
                        (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0);
                     }
                     else
                     {
                        if (pMotorData->nShortRecove< SHORT_AUTORECOVEY_MAXTIMES)
                        {  
                           pMotorData->mState =MOTOR_STATE_SHORT_FORWARD;
                        }
                        else
                        {
                           pMotorData->mState =MOTOR_STATE_SHORT_LOSE;      
                        } 
                     }
                     pMotorData->nShortRecove++;   
                  }
                  DriveState = MOTOR_RUN_HALT;
                  break;
               case  MOTOR_STATE_SHORT_FORWARD:
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, MOTOR_SHORT_FORWARD_TIME))
                  {//��2s������ת��
                     pMotorData->mState = MOTOR_STATE_SHORT_BACKWARD;
                  }
                  
                  if ( (pMotorData->mShortErr == 0) && (pMotorData->mGet > 51 && pMotorData->mGet < 200))
                  {
                  	pMotorData->mState = MOTOR_STATE_SHORT_GETPOSTION;   
                  	(void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0);
                  }
                  if (pMotorData->mErrorCode == MOTOR_ERROR_SHORT) 
                     DriveState = MOTOR_RUN_BACKWARD;
                  else  DriveState = MOTOR_RUN_FORWARD;
                  break;
               case  MOTOR_STATE_SHORT_BACKWARD:
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, MOTOR_SHORT_BACKWARD_TIME))
                  {//��2s������ת��
                     if (pMotorData->mShortErr)
                     {//�Ծɹ���
                        pMotorData->mState = MOTOR_STATE_SHORT_LOSE;
                     }
                     else
                     {
                        pMotorData->mState = MOTOR_STATE_SHORT_GETPOSTION;   
                        (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0);
                     }
                  }
                  if (pMotorData->mErrorCode == MOTOR_ERROR_SHORT) 
                     DriveState = MOTOR_RUN_FORWARD;
                  else  DriveState = MOTOR_RUN_BACKWARD;
                  break;
               case  MOTOR_STATE_SHORT_GETPOSTION:
                  if (pMotorData->mShortErr)
                  {//��ʱ����ֱ�ӽ��� lose
                     pMotorData->mState = MOTOR_STATE_SHORT_LOSE;
                  }
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, MOTOR_SHORTERROR_WAITTIME))
                  {//��2s�ڲ����ϡ�ȡ�����ϣ����ص���������ģʽ��20171024 ����ʱ��
                     pMotorData->mState =MOTOR_STATE_HALT;  
                     pMotorData->Work = MOTOR_WORK_NORMAL;  
                     pMotorData->nMotorJ = 0;
                  }
                  DriveState = MOTOR_RUN_HALT;
                  break;
               case MOTOR_STATE_SHORT_LOSE:
                  //������ϣ�ֻ���ض�λ����ʹ�䶯����
						if(!pMotorData->mShortErr)
						{ //û�й���
							pMotorData->mState = MOTOR_STATE_SHAKE;  //�ȴ�2s �ٻָ�
							pMotorData->Work = MOTOR_WORK_NORMAL; 
							(void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0) ;  
						}
                  DriveState = MOTOR_RUN_HALT;
                  break;
            }
            break;
		#endif
         /**************��·����end*********************************************************/
		#if NEEDFUNCTION_SELF_TSET     
			case  MOTOR_WORK_SELFTEST:
         /**************���Թ�����ʼ*********************************************************/
         /************************************************************************************
         ��������Ϊ1  ��ת10s ���� TEST_FORWARD_AD   2 ��ת10s ����TEST_BACKWARD_AD
          3 ���ϴﲻ�����м���ֶ���Ϊ���� 4 123 ���ɹ�����ȥ���м�λ����10s��Ѱ���м�λ��
          5 ͨ�������룬��8λ ����   
         ************************************************************************************/   
            switch (pMotorData->mState) 
            {    //�������ǰ״̬ɢת
               default:                            //�쳣Ĭ��
               case  MOTOR_STATE_TEST_ENTER:
                  (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0);   
                  pMotorData->mState = MOTOR_STATE_TEST_FORWARD; 
                  DriveState = MOTOR_RUN_HALT;
                  pMotorData->mTestError = 0;
                  break;
               case  MOTOR_STATE_TEST_FORWARD :
                  if (mdiffGet<-MOTOR_RUNSHAKE_AD || mdiffGet>MOTOR_JUMP_AD)
                  {//��תʱ�����ad��С˵�������⣬���޷����� �����ߵ����ѹ�仯����
                    // pMotorData->ShakeETimes++;      
                     pMotorData->mTestError |= TEST_ERROR_SHAKE;
                  }
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, TEST_FORWARD_TIME))
                  {//����10s���������С�� TEST_FORWARD_AD �����
                  	if(pMotorData->mGet < 10)
							{
							  pMotorData ->mTestError |= TEST_ERROR_RANGEOUT;
							}
                  	else if(pMotorData->mGet < TEST_FORWARD_AD)
                     {
                        pMotorData ->mTestError |= TEST_ERROR_RANGESMALL;
                     }else{}
                     pMotorData->mState= MOTOR_STATE_TEST_BACKWARD;   
                  }
                  DriveState = MOTOR_RUN_FORWARD; 
                  break; 
               case  MOTOR_STATE_TEST_BACKWARD :
                  if (mdiffGet > MOTOR_RUNSHAKE_AD || mdiffGet < -MOTOR_JUMP_AD)
                  {//�ж��Ƿ񶶶�����
                     pMotorData ->mTestError |= TEST_ERROR_SHAKE;
                  }
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, TEST_BACKWARD_TIME))
                  {   //10s �ﵽ  TEST_BACKWARD_AD
                     if (pMotorData->mGet > 250)
                     {
                        pMotorData->mTestError |= TEST_ERROR_RANGEOUT;
                     }
                     else if (pMotorData->mGet > 10)
                     {
                        pMotorData->mTestError |= TEST_ERROR_RANGESMALL;
                     }
                     else
                     {
                     }
                     pMotorData->mState = MOTOR_STATE_TEST_GOTOMIDDLE;
                  }
                  DriveState = MOTOR_RUN_BACKWARD; 
                  break; 
               case  MOTOR_STATE_TEST_GOTOMIDDLE:
               //�ҵ��м�λ��
                  if (pMotorData->mGet > (TEST_MIDDLE_AD-MOTOR_DRIVE_ACCURACY)&&
                      pMotorData->mGet < (TEST_MIDDLE_AD+MOTOR_DRIVE_ACCURACY))
                  {
                  	//�ҵ�λ��ֱ������
                  	pMotorData->mTestError |= TEST_ERROR_DOING;
                     DriveState = MOTOR_RUN_HALT;
                     pMotorData->mState= MOTOR_STATE_TEST_END;
                  }
                  else if (pMotorData->mGet < TEST_MIDDLE_AD)
                  {
                     DriveState = MOTOR_RUN_FORWARD;       
                  }
                  else  if (pMotorData->mGet > TEST_MIDDLE_AD)
                  {
                     DriveState = MOTOR_RUN_BACKWARD;  
                  }
                  
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, TEST_BACKWARD_TIME) )
                  { //�Ҳ�������
                    	pMotorData->mTestError |= TEST_ERROR_DOING;
                    	pMotorData->mTestError |= TEST_ERROR_NOTGETPOSTION;
                     pMotorData->mState = MOTOR_STATE_TEST_END;  
                     DriveState = MOTOR_RUN_HALT;        
                  }
                  break;
               case MOTOR_STATE_TEST_END:
                  pMotorData->mState = MOTOR_STATE_HALT;
                  pMotorData->Work = MOTOR_WORK_NORMAL;
                  break;
            }
            break;
		#endif
            /**************���Թ�������*********************************************************/
         case MOTOR_WORK_STOP:
				if (pMotorData->mSet != MOTOR_STOP_AD)
				{            	
					pMotorData->mState = MOTOR_STATE_SHAKE;  //�ȴ�2s �ٻָ�
					pMotorData->Work = MOTOR_WORK_NORMAL; 
					(void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0); 
				}
				else{}
				DriveState = MOTOR_RUN_HALT;
				break;
      }
         
      pMotorData->mGetOld = pMotorData->mGet ;
      MotorHW_Drive(mId, DriveState);
      pMotorData++;
   }
}

/*******************************************************************************
 * Function:    uint8 Motor_RunControl(uint8 motorId,uint8 motorState)
 *
 * Description: ֱ�ӵ��������
 *
 * Returns:     ����������ڷ�Χ�ڷ���0��
 *
 * Notes:      
			
 *
*******************************************************************************/
BOOL Motor_RunControl(uint8 motorId,uint8 motorState)
{
	if (motorId >= motorTotal) return FALSE;
	MotorHW_Drive(motorId, motorState);
	return TRUE;
}
