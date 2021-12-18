/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：ncfmotor.c
* 功能描述：内外电机控制模块，具有
* 1  最多3路的电机的控制功能
* 
* 本模块使用需要定时器模块
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原ncfmotor模块的2.1移植
* 原作者：xb
* 完成日期：2019年5月20日
* 
* 版本：1.1
* 说明：电机默认为不动作，设定为255时为不动作
* 作者：xb
* 完成日期：2020年9月29日
*/
#include "ncfmotor.h"


typedef struct
{
	NcfMotorSetType mSet;    //设定状态
   uint8 mState;            //电机过程控制状态
   uint16 mTimeCtl;         //电机控制时间间隔   
}NCFMOTORDATA;

static NCFMOTORDATA  NcfMotorData[MAX_NCFMOTOR_NUM]; //电机控制数据结构

static  uint8  motorTotal = 1;
static  uint16 RunTOut = 150;


/***********************************
 通过硬件驱动需要调用的函数声明
************************************/
//电机输出
extern void NcfMotorHW_Drive(uint8 mid,uint8 motor_stat);
 /*******************************************************************************
 * Function:    BOOL NcfMotor_Init(uint16 outtime, uint8 nmotor)
 *
 * Description: 电机模块初始化设置
 *
 * Returns:     电机个数，0为失败
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
	   {//初始化时，默认为stop
	      NcfMotorData[mId].mState = NCFMOTOR_STATE_HALT;
	      NcfMotorData[mId].mSet = NCFMOTOR_POSITION_STOP;
	   }
	}

	return bNCFInitRes;
}


/*******************************************************************************
 * Function:    void NcfMotor_Reset(void)
 *
 * Description: 电机初始化
 *
 * Returns:     none
 *
 * Notes:     可用来电机上电初始化 
			
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
 * Description: 设置电机设定值
 *
 * Returns:     none
 *
 * Notes:       当有需要时调用可随时调用此函数			
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
   {//设置失败
   	
   }
}

/*******************************************************************************
 * Function:    void NcfMotor_ChangePosition(uint8 motorId)
 *
 * Description: 设置电机设定值
 *
 * Returns:     none
 *
 * Notes:       当有需要时调用可随时调用此函数			
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
 * Description: 得到当前电机的位置
 *
 * Returns:     NcfMotorSetType
 *
 * Notes:       任何时候需要时候调用			
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
 * Description: 电机调整控制
 *
 * Returns:     none
 *
 * Notes:    该函数仅被main函数中的主任务循环调用 		
 *
*******************************************************************************/
void NcfMotor_Control(void)
{
   uint8 mId;
   NCFMOTORDATA* pMotorData = NcfMotorData;
    
   //此处为点火信号处理,由ad模块处理
   for (mId = 0; mId < motorTotal; mId++) 
   {   
   //枚举控制所有电机
   	switch (pMotorData->mState)
      {    //按电机当前状态散转
         default:                            //异常默认
            pMotorData->mState = NCFMOTOR_STATE_HALT;
            break;
         case NCFMOTOR_STATE_HALT:              //当前为停机状态
         	NcfMotorHW_Drive(mId, MOTOR_RUN_HALT);
            break;
         case NCFMOTOR_STATE_RUNBYSET:           //当前为正转状态
            //定时15s停止
            if(TimeOutChkTenthSLong(&pMotorData->mTimeCtl, RunTOut)
            		|| pMotorData->mSet == NCFMOTOR_POSITION_STOP)
            {      
               pMotorData->mState = NCFMOTOR_STATE_HALT ;
            }
            else
            {
               if(pMotorData->mSet == NCFMOTOR_POSITION_FORWARD)
               {
               	NcfMotorHW_Drive(mId, MOTOR_RUN_FORWARD);   //1 正转
               }
               else  
               {
               	NcfMotorHW_Drive(mId, MOTOR_RUN_BACKWARD);
               }
            }
            break;
         case  NCFMOTOR_STATE_RESET:                   //初始化 
            //时间初始化，次数
            (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0) ;                                                    
            //根据mset设定
     	      pMotorData->mState = NCFMOTOR_STATE_RUNBYSET; 
         	break;
      }
      pMotorData++;
   }
}





  
       
