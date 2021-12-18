/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：motor.c
* 功能描述：电机控制模块，具有
* 1  最多4路的电机的控制功能
* 2  具有可配置的堵转功能
* 3  具有短路保护功能
* 4  具有自检测试功能
* 
* 本模块使用需要定时器模块及数学函数处理模块
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原motor模块的3.2移植，增加定时处理
* 原作者：xb
* 完成日期：2019年5月20日
* 
* 版本：1.1
* 说明：增加停机工作，当设定位置=255时 电机停机
* 原作者：xb
* 完成日期：2020年9月28日
* 
* 版本：1.2
* 说明：
* 1 当电机抖动时未设置DriveState的bug修复,抖动AD MOTOR_STOPSHAKE_AD 由2->3
* 2 电机精度值-默认值由3改为5
* 原作者：xb
* 完成日期：2020年12月2日
* 
* 版本：1.3
* 说明：
* 1 精度值MOTOR_DRIVE_ACCURACY 由宏定义变为int16,默认5
* 2 提前量默认为1，可配置
* 3 增加MotorDriveInit 来设定精度和提前量
* 作者：xb
* 完成日期：2020年12月5日
* 
*/

#include "motor.h" 
#include "mathGA.h"
 
typedef struct
{
   uint8 mSet;         //位置设定值
   uint8 mGet;         //当前位置
   uint8 mGetOld;      //上一次的值       
   uint8 mState;       //电机过程控制状态
   uint8 Work;
   uint8 mErrorCode;     //故障代码
   uint8 mTestError;     //故障代码
   uint16 mTimeCtl;      //电机控制时间间隔 ,电机转动时间
   uint16 mTimeCtl_1;     //电机控制时间1 ，其他的处理时间控制
   uint8 nMotorJ;        //电机堵抓次数
   uint8 nShortRecove;   //短路恢复的次数 ，电机到达正确位置时可清0
   uint8 nShakeTimes;    //抖动次数 ，motor_reset 才可以清0
   uint8 mShortErr;     //故障代码
   uint8 mShortTimes;   //故障次数
}MOTORDATA; 

MOTORDATA motorData[MAX_MOTOR_NUM]; //电机控制数据结构
static  uint8  MaxJamed = 4, motorTotal = 3;
static  uint16 RunTOut = 150;

int16 MOTOR_DRIVE_ACCURACY = DEFAULT_MOTOR_DRIVE_ACCURACY;   // 电机精度
int8  _admotor = DEFAULT_MOTOR_BEFORDATA;          // 电机提前量


/*******************************************************************************
 通过外部需要调用的函数声明
**********************************************************************************/
extern void MotorHW_Drive(uint8 mid,uint8 motor_stat); //电机输出
extern uint8 MotorHW_GetAD(uint8 MotorId);
extern int16 MotorHW_SupplyVolt(void);
/*******************************************************************************
 * Function:    uint8 Motor_Init(uint8 tout,uint8 njamde,uint8 );
 *
 * Description: 电机模块初始化设置
 *
 * Returns:     电机个数，0为失败
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
   {//20200928 默认更改停机
   	motorData[i].mSet = MOTOR_STOP_AD;
   	motorData[i].Work = MOTOR_WORK_STOP;
   }
   return motorTotal;
}


/*******************************************************************************
 * Function:    BOOL MotorDriveInit(uint8 u8AccData, uint8 u8BeforData)
 *
 * Description: 电机模块驱动初始化。精度和提前量
 *
 * Returns:    u8AccData (2-9)，  u8BeforData小于5, u8AccData > u8BeforData才能返回TRUE,
 *
 * Notes:      不调用，默认值是5,1
			
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
 * Description: 电机自检设置
 *
 * Returns:     0-1
 *
 * Notes:     id号自检 
			
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
 * Description: 电机初始化
 *
 * Returns:     none
 *
 * Notes:     可用来电机上电初始化 
			
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
 * Description: 设置电机设定值
 *
 * Returns:     设置成功为1 ，否则为0
 *
 * Notes:       当有需要时调用可随时调用此函数			
 *
*******************************************************************************/
BOOL Motor_SetPosition(uint8 motorId, uint8 mPosition)
{

   MOTORDATA  *pMotorData = &motorData[motorId]; 
   
   //不能设定故障电压范围内
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
      //和上次设定值不相等，进行如下处理  
         if ( pMotorData->Work == MOTOR_WORK_NORMAL)
         {   //在正常工作时位置重新设定时，电机到初始化状态
            pMotorData->mState = MOTOR_STATE_RESET ;            
         }
         else  if (pMotorData->Work == MOTOR_WORK_SHORT) 
         {    //在短路停机时。设定位置变化大于DEAL_SHORTRECOVE_AD 时 ，进入短路恢复进程
            #if   NEEDFUNCTION_SHORT_AUTORECOVEY
               if ( (pMotorData->mSet > mPosition && (pMotorData->mSet - mPosition) > DEAL_SHORTRECOVE_AD)||
                    (pMotorData->mSet < mPosition && (mPosition - pMotorData->mSet) > DEAL_SHORTRECOVE_AD) )
               {
                  //当电机设定位置差大于 DEAL_SHORTRECOVE_AD 才可以动作,否则返回0 说明设置失败
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
         {//其他模式只设定位置
         	
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
 * Description: 得到当前电机的位置
 *
 * Returns:     0-255
 *
 * Notes:       任何时候需要时候调用			
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
//调试电机模块时使用
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
 * Description: 返回电机故障代码
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
 * Description: 电机调整控制
 *
 * Returns:     none
 *
 * Notes:    该函数仅被main函数中的主任务循环调用 		
 *
*******************************************************************************/
void Motor_Control(void)
{
    static  uint16  TimerMotorRun;
	 uint8   mId;               // 电机号
    int16   mdiffGet ;         // 这次的反馈电压和上次的差
    int16   difmset;           // 设定值于当前的反馈值得差
    uint16  mTimeout;          // 电机运行时间
    uint8   DriveState = MOTOR_RUN_HALT;      // 电机运行状态
    MOTORDATA  *pMotorData;    // 电机控制指针


   if (TimeOutChkMsLong(&TimerMotorRun, MOTOR_RUN_TIME) == FALSE)
   {
   	return;
   }
   pMotorData = motorData;
   for (mId = 0; mId < motorTotal; mId++) 
   {   //枚举控制所有电机
   
   	//20160412 增加电机 设定 位置范围为 MOTOR_OPEN_AD MOTOR_SHORT_AD
   	if (pMotorData->mSet < MOTOR_SHORT_AD)
   	{
   		pMotorData->mSet = MOTOR_SHORT_AD + 2;
   	}
   	
   	if (pMotorData->mSet > MOTOR_OPEN_AD)
   	{
   		pMotorData->mSet = MOTOR_OPEN_AD - 2;
   	}
   	
      //得到电机ad，并判断是否故障
      pMotorData->mGet = MotorHW_GetAD(mId);
#if NEEDFUNCTION_SHORT_AUTORECOVEY 
      //故障码策略更新
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
      
      difmset = pMotorData->mGet - pMotorData->mSet;        //得到当前与设定的差，有负号
   
      mdiffGet = pMotorData->mGet - pMotorData->mGetOld;    //与上一次的差值
      
      switch (pMotorData->Work)
      {
         default:
            pMotorData->Work = MOTOR_WORK_NORMAL;
         /**************正常工作*********************************************************/
         /*********************************************************************************************
         正常工作实现 1 普通的电机动作 2 堵转工作  3 堵转时 2s动作20s停的功能
         4 抖动故障功能。  5 短路断路时进入 短路工作模式
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
               pMotorData->mState = MOTOR_STATE_SHORT_ENTER;          //初始化时故障进入故障调整
               (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0);
               //区分故障类型
               if (pMotorData->mShortErr == 1)
                  pMotorData->mErrorCode = MOTOR_ERROR_SHORT; 
               else pMotorData->mErrorCode = MOTOR_ERROR_OPEN;
               
               DriveState = MOTOR_RUN_HALT;//短路断路故障立即停止电机
               break;
            }
			#endif
            //else
            //{
            //   pMotorData->mErrorCode ^= ~MOTOR_ERROR_SHORT ;  //清除此故障码
            //}
            switch (pMotorData->mState) 
            {    //按电机当前状态转动
               default:                            //异常默认
                  pMotorData->mState = MOTOR_STATE_WAITHALT;
               case  MOTOR_STATE_WAITHALT:
               //当电机正反转切换，或由转动到停止，需要在进入此状态，停止0.5s。
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl_1, MOTOR_RUNHATL_LEASTTIME) )
                  {
                     pMotorData->mState = MOTOR_STATE_HALT;    
                  }   
                  DriveState = MOTOR_RUN_HALT;                       
                  break;
               case MOTOR_STATE_HALT:              //当前为停机状态  
               	if (NEEDFUNCTION_DEALSHAKE == 0 ||
                   (mdiffGet <= MOTOR_STOPSHAKE_AD && mdiffGet >= -MOTOR_STOPSHAKE_AD))
                  { //电机停止时动作，此时电机故障
                  //   if(pMotorData->nErrRecove<MAX_ERECOVE_TIMES)//可以故障调整电机的次数
                 //    {//此处为了防止电机故障了后恢复，又故障过于频繁，实际上没有此问题
                        //只有在电机没有动作的时候才能进入此项
							if (difmset < MOTOR_DRIVE_ACCURACY && difmset > -MOTOR_DRIVE_ACCURACY)
							{//达到精度
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
               case MOTOR_STATE_FORWARD:           //当前为正转 
                  //到达位置
                  DriveState = MOTOR_RUN_FORWARD;
                 
                  #if (NEEDFUNCTION_DEALSHAKE >=2)
                     if (mdiffGet < -MOTOR_RUNSHAKE_AD || mdiffGet > MOTOR_JUMP_AD)
                     {//正转时有如果ad变小说明有问题，则无法动作 。或者电机电压变化过大
                       // pMotorData->ShakeETimes++;      
                        pMotorData->mState = MOTOR_STATE_HALT; 
                        DriveState = MOTOR_RUN_HALT; 
                     }
                  #endif
                  
                  if (difmset >= -_admotor)
                  {//达到位置先停止
                     pMotorData->mState = MOTOR_STATE_WAITHALT;
                     (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl_1, 0); //为MOTOR_STATE_WAITHALT时间控制，mTimeCtl用来控制电机超时，所以需要用mTimeCtl_1
                     DriveState = MOTOR_RUN_HALT;  
                  }
                  else
                  {
                     if (pMotorData->nMotorJ == 0) 
                     {  //设定转动时间              
                        mTimeout = RunTOut;            //第一次为40s
                     }
                     else
                     {
                        mTimeout = 20;        //以后为2s
                     }
                     //定时到还未到达故障
                     if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, mTimeout))
                     {      
                        pMotorData->mState = MOTOR_STATE_JAMMED_ERROR ;  
                        pMotorData->mErrorCode = MOTOR_ERROR_JAMMED;   //定时到还未到进入堵转状态
                        pMotorData->nMotorJ++;
                        DriveState = MOTOR_RUN_HALT;
                        break;
                     }
                  }
                  break;
               case MOTOR_STATE_BACKWARD:          //当前为反转状态
                  //到达位置
                  DriveState = MOTOR_RUN_BACKWARD;
                  
                  #if (NEEDFUNCTION_DEALSHAKE >= 2)
                     if ( mdiffGet > MOTOR_RUNSHAKE_AD || mdiffGet<-MOTOR_JUMP_AD)
                     {//正转时有如果ad变大说明有问题，1则无法动作.或者电机电压变化过大 
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
                     {  //设定转动时间              
                        mTimeout= RunTOut;            //第一次为40s
                     }
                     else
                     {
                        mTimeout=20;        //以后为2s
                     }
                     //定时到还未到达故障
                     if(TimeOutChkTenthSLong(&pMotorData->mTimeCtl, mTimeout))
                     {      
                        pMotorData->mState = MOTOR_STATE_JAMMED_ERROR ;  
                        pMotorData->mErrorCode = MOTOR_ERROR_JAMMED;   //定时到还未到进入堵转状态
                        pMotorData->nMotorJ++;
                        DriveState = MOTOR_RUN_HALT;
                        break;
                     }
                  }
                  break;
                  
               case MOTOR_STATE_JAMMED_ERROR:             //当前为堵转故障状态
                  if (difmset < MOTOR_DRIVE_ACCURACY && difmset > -MOTOR_DRIVE_ACCURACY)
                  {    //在误差范围内，解除故障
                     pMotorData->mState = MOTOR_STATE_HALT ;
                     pMotorData->mErrorCode = MOTOR_ERROR_NONE;
                     break;
                  }
                  if (pMotorData->nMotorJ >= MaxJamed)
                  {   //堵转次数达到最大
                     pMotorData->mErrorCode = MOTOR_ERROR_JAMMED;   
                  }
                  else 
                  {  //没有到最大次数，停止20s秒再转动
                      if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl,100)) 
                      {//jac 10s
                         if (difmset<0)   //重新定位
                            pMotorData->mState = MOTOR_STATE_FORWARD; 
                         else   pMotorData->mState = MOTOR_STATE_BACKWARD; 
                      }
                  }
                  DriveState = MOTOR_RUN_HALT;
                  break;
               case  MOTOR_STATE_SHAKE:
						#if (NEEDFUNCTION_DEALSHAKE >= 1)
                     if (mdiffGet <= MOTOR_STOPSHAKE_AD && mdiffGet >= -MOTOR_STOPSHAKE_AD) 
                     {    //没有故障，等待 1s，返回故障继续转动
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
               case  MOTOR_STATE_RESET:                   //初始化

                  //时间初始化，次数
                  (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0);
                  pMotorData->nMotorJ = 0;                                                        
                  pMotorData->nShakeTimes =0;
                  //根据转动设定
                  //2014 10 05 修改,只要进入此状态判断是否一样，不一样就开始动作 
           		   if ( !difmset ) 
           		   {   //初始化时如果设定与当前位置差距小则停止
             		   pMotorData->mState = MOTOR_STATE_HALT ;
                     pMotorData->mErrorCode = MOTOR_ERROR_NONE;
             	   }
             		else
             		{	
             	      if(difmset<0)       //确定正反转
             	         pMotorData->mState=MOTOR_STATE_FORWARD; 
             	      else   pMotorData->mState=MOTOR_STATE_BACKWARD;
             		}
             		DriveState = MOTOR_RUN_HALT;
             		break;   

            }
            break;
         /**************正常工作end*********************************************************/
		#if NEEDFUNCTION_SHORT_AUTORECOVEY      
         case  MOTOR_WORK_SHORT:  
         /**************短路工作 *********************************************************/
         /************************************************************************************
         短路恢复过程为 1 等待2s  2 正转2s  ，反转,  如果正常电压 3 等待2s到没有故障则恢复到正常工作
         电压不正常则 4 进入永久停机状态  直到设定位置变化。
         如果NEEDFUNCTION_SHORT_AUTORECOVEY  ==0 则 故障停机，恢复继续工作(改为不会进入)
         ************************************************************************************/   
            switch(pMotorData->mState)
            {
               default:
               case  MOTOR_STATE_SHORT_ENTER:

                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, MOTOR_SHORTERROR_WAITTIME))
                  {//过2s后开始动作，20171024 更改减少等待时间0.5s 因为故障判断已经加了延时 
                     if (pMotorData->mShortErr == 0)
                     { //故障消除,进入短路等待状态
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
                  {//这2s内是正转。
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
                  {//这2s内是正转。
                     if (pMotorData->mShortErr)
                     {//仍旧故障
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
                  {//此时故障直接进入 lose
                     pMotorData->mState = MOTOR_STATE_SHORT_LOSE;
                  }
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, MOTOR_SHORTERROR_WAITTIME))
                  {//这2s内不故障。取消故障，返回到正常工作模式，20171024 减少时间
                     pMotorData->mState =MOTOR_STATE_HALT;  
                     pMotorData->Work = MOTOR_WORK_NORMAL;  
                     pMotorData->nMotorJ = 0;
                  }
                  DriveState = MOTOR_RUN_HALT;
                  break;
               case MOTOR_STATE_SHORT_LOSE:
                  //进入故障，只有重定位才能使其动作？
						if(!pMotorData->mShortErr)
						{ //没有故障
							pMotorData->mState = MOTOR_STATE_SHAKE;  //等待2s 再恢复
							pMotorData->Work = MOTOR_WORK_NORMAL; 
							(void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0) ;  
						}
                  DriveState = MOTOR_RUN_HALT;
                  break;
            }
            break;
		#endif
         /**************短路工作end*********************************************************/
		#if NEEDFUNCTION_SELF_TSET     
			case  MOTOR_WORK_SELFTEST:
         /**************测试工作开始*********************************************************/
         /************************************************************************************
         测试内容为1  正转10s 到达 TEST_FORWARD_AD   2 反转10s 到达TEST_BACKWARD_AD
          3 以上达不到，中间出现抖动为故障 4 123 内由故障则不去找中间位置在10s内寻找中间位置
          5 通过故障码，第8位 体现   
         ************************************************************************************/   
            switch (pMotorData->mState) 
            {    //按电机当前状态散转
               default:                            //异常默认
               case  MOTOR_STATE_TEST_ENTER:
                  (void)TimeOutChkTenthSLong(&pMotorData->mTimeCtl, 0);   
                  pMotorData->mState = MOTOR_STATE_TEST_FORWARD; 
                  DriveState = MOTOR_RUN_HALT;
                  pMotorData->mTestError = 0;
                  break;
               case  MOTOR_STATE_TEST_FORWARD :
                  if (mdiffGet<-MOTOR_RUNSHAKE_AD || mdiffGet>MOTOR_JUMP_AD)
                  {//正转时有如果ad变小说明有问题，则无法动作 。或者电机电压变化过大
                    // pMotorData->ShakeETimes++;      
                     pMotorData->mTestError |= TEST_ERROR_SHAKE;
                  }
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, TEST_FORWARD_TIME))
                  {//运行10s，如果还是小于 TEST_FORWARD_AD 则故障
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
                  {//判断是否抖动故障
                     pMotorData ->mTestError |= TEST_ERROR_SHAKE;
                  }
                  if (TimeOutChkTenthSLong(&pMotorData->mTimeCtl, TEST_BACKWARD_TIME))
                  {   //10s 达到  TEST_BACKWARD_AD
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
               //找到中间位置
                  if (pMotorData->mGet > (TEST_MIDDLE_AD-MOTOR_DRIVE_ACCURACY)&&
                      pMotorData->mGet < (TEST_MIDDLE_AD+MOTOR_DRIVE_ACCURACY))
                  {
                  	//找到位置直接跳处
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
                  { //找不到故障
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
            /**************测试工作结束*********************************************************/
         case MOTOR_WORK_STOP:
				if (pMotorData->mSet != MOTOR_STOP_AD)
				{            	
					pMotorData->mState = MOTOR_STATE_SHAKE;  //等待2s 再恢复
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
 * Description: 直接电机驱动，
 *
 * Returns:     电机个数不在范围内返回0。
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
