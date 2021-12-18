/**************************�ļ�˵��*****************************************************
* Copyright (c) 2019 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�FanControl.c
* �����������ķ���������������¹���
* 1 ͨ���ɼ��ķ���Ķ˵�ѹ�ͷ�����ѹ���õ���ǰ�ķ�������ѹ
* 2 ͨ��PID���ƣ�����PWM���̵������ﵽ��ѹ���ȶ�
* 3 �����õ�ѹ����
* 4 ���й����ж�
* 
* �������� codewarrior IDE 10.7-KEAZ128
* MCU˵�������ļ���MCU�޹�
* 
* */
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾������ ��ԭģ���1.2��ֲ
* ԭ���ߣ�xb
* ������ڣ�2019��6��9��
* 
* �汾��1.1
* ˵���� 1 ���ӵ�ѹ���������
*     
* ԭ���ߣ�xb
* ������ڣ�2019��9��23��
* 
* �汾��1.2
* ˵���� 1 �������жϲ��Ը���
*     
* �汾��1.3
* ˵���� 1 �޸Ĺķ�����ٵ��ڵ�Ĭ��ki ��4��Ϊ20������������0.5v��Ϊ0.4
*        2 ���ܵ������ٵ�Iֵ��BUG�޸�
* ԭ���ߣ�xb
* ������ڣ�2020��1��18��
*
*�汾��1.4  
* ˵���� 
* 1.Init��ʼ��ʱ�������ʱ����sFanData.ShortErrTime = 0;
* 2.FanVolt_Control����IGN��ѹ��5V�������ʱ������
* ԭ���ߣ�lyc
* ������ڣ�2020��9��29��
* 
* �汾��1.5  
* ˵���� 
* 1.Ŀ���ѹ�뵱ǰ��ѹ��ֵС�ڵ���0.3Vʱ���ٶȻ����,����PID��I������ڣ���һ�汾��0.5V��
* ԭ���ߣ�zhupan
* ������ڣ�2020��12��8��
*/
#include "FanControl.h"
#include "mathGA.h"
#include "DealString.h"

typedef struct
{
	ADVoltType  avSetVolt;             //���õ�ѹ
	ADVoltType  avLastVolt;            //������
	ADVoltType  avCurrentSetVolt;      //��ǰӦ�ôﵽ�ĵ�ѹ�������۵�ѹ
	uint16  u16SetRate;             //Ŀ��仯��ѹ ��λ��0.1v/s��, ����Χ 0.1v/s - 25.5v/s
   int16   i16ErrSum;              //��������
   int16   i16Err_1;               //ǰ�����,�����
	uint16  TimerFanAdj;            //�������ڴ���
	uint16  u16FanPWM;              //����PWM = D /10000	
	BOOL    bFanRealy;              //�̵���
	uint8   OpenErrTime;            //�����������      
	uint8   ShortErrTime;
	uint8   u8AdjState;             //����״̬	
	uint8   u8AdjStateTime;         //���ڼ�����
	uint8   u8ErrCode;              //������
	uint8   u8CurrentLevel;         //������ֵavCurrentSetVolt�����ó����ĵ�λ
}FANCONTROLDATA;   //����������


static FANCONTROLDATA  sFanData;
static FANCONTROLCONFIG sFanConfig;
static ADVoltType avMaxFanVolt;        //���λ�ĵ�ѹ

//�ײ�������������
extern ADVoltType FanHW_BackVolt(void);
extern ADVoltType FanHW_SupperVolt(void);
extern void FanHW_OutPWM(uint16 pwm);
extern void FanHW_OutFanRealy(uint8 bSetState);
/******************************************************************************
* Function:   BOOL FanControl_Init(FANCONTROLCONFIG* pFConfig)
* 
* Description:   �ķ�����Ƴ�ʼ��
*
* param1 ��       pFConfig

* return :    BOOL 
* 
* Note  :     �ķ����ʼ������
*
************************************************************************************/
BOOL FanControl_Init(FANCONTROLCONFIG* pFConfig)
{
	BOOL bFanInitOK = TRUE;
	uint8 i;
	ADVoltType avFanLastVolt = 1;

	MemCopy(&sFanConfig, pFConfig, sizeof(FANCONTROLCONFIG));
	avMaxFanVolt = pFConfig->TBL_FanVolt[pFConfig->u8FanMaxLevel];

	sFanData.avCurrentSetVolt = 0;
	sFanData.u16FanPWM = 0;
	sFanData.avLastVolt = 0;
	sFanData.u16SetRate = pFConfig->u16InitRate;
	sFanData.avSetVolt = 0;
	sFanData.u8CurrentLevel = 0;
	sFanData.u8ErrCode = FAN_ERRCODE_NONE;
	sFanData.u8AdjState = FAN_STATE_CLOSE;
	sFanData.OpenErrTime = 0;
    sFanData.ShortErrTime = 0;
	
	//rate ����Ϊ0
	if (pFConfig->u16InitRate == 0 && pFConfig->u8SlowAdjFactor == 0)
	{
		sFanConfig.u16InitRate = FAN_DEFAULT_RATE;
		sFanConfig.u8SlowAdjFactor = FAN_SLOWADJ_FACTOR;
		bFanInitOK = FALSE;
	}
	else
	{

	}

	//����tbl �����д�
	for (i = 1; i <= pFConfig->u8FanMaxLevel; i++)
	{
		if (pFConfig->TBL_FanVolt[i] <= avFanLastVolt)
		{
			bFanInitOK = FALSE;
		}
		else
		{
			avFanLastVolt = pFConfig->TBL_FanVolt[i];
		}
	}
	
	// v offset ������200~256 ֮��
	if (sFanConfig.u8VoltOffSet >= 200)
	{
		
	}
	else
	{
		sFanConfig.u8VoltOffSet = VOUT_OFFSET_DEFAULT;
		bFanInitOK = FALSE;
	}
	return bFanInitOK;
}
/******************************************************************************
* Function:   void FanControl_SetVolt(ADVoltType u16Volt)
* 				
* Description:   ���ùķ��Ŀ���ѹ
*
* param1 ��   ADVoltType  Ŀ���ѹ

* return :    NONE
* 
* Note  : 
*
************************************************************************************/
void FanControl_SetVolt(ADVoltType avSetValue)
{	
	
	if ( GAMath_abs(sFanData.avSetVolt - avSetValue) > ACD_V(0.1) )
	{
		if (sFanData.avSetVolt <= 0)
		{//���ӣ���ԭ��Ϊ0��ʱ������������Ϊ��ʼ�ٶ�
			sFanData.u16SetRate = sFanConfig.u16InitRate;
		}
		else
		{
		}		
		
		//sFanData.u8AdjState = FAN_STATE_PID;
		sFanData.i16ErrSum = 0;
		sFanData.i16Err_1 = 0;
		sFanData.avSetVolt = avSetValue;
	}
	else
	{
		//sFanData.avSetVolt = avSetValue;
	}
}
/******************************************************************************
* Function:   void FanControl_SetRate(uint16 u16Rate)
* 				
* Description:   ���ùķ���仯�ٶ�
*
* param ��           u16Rate �仯���� ��λΪ 0.1v/s, ����Ϊ0

* return :     none
* 
* Note  :    
*
************************************************************************************/
void FanControl_SetRate(uint16 u16Rate)
{
	if (sFanData.u16SetRate != u16Rate)
	{
		sFanData.u16SetRate = u16Rate;	
		sFanData.i16ErrSum = 0;
		sFanData.i16Err_1 = 0;
	}
	else
	{
		//������ͬ�򲻴���
	}
}


/******************************************************************************
* Function:   uint8 Fan_CurrentLevelGet(void)
* 
* Description:   �õ���ǰ�ķ絵��λ
*
* return :    ��λ
* 
* Note  :  
*
************************************************************************************/
uint8 FanControl_GetCurrentLevel(void)
{
	return sFanData.u8CurrentLevel;
}
/******************************************************************************
* Function:   ADVoltType FanControl_GetCurrentVolt(void)
* 
* Description:   �õ���ǰ�Ĺķ���ĵ�ѹ
*
* return :    ADVoltType 
* 
* Note  :  
*
************************************************************************************/
ADVoltType FanControl_GetCurrentVolt(void)
{
	return (sFanData.avLastVolt);
}

/******************************************************************************
* Function:   uint8 Fan_ErrCode(void)
* 
* Description:   �õ��������ƹ�����
*
* return :   ������
* 
* Note  :    ���嶨��� ������궨��
*
************************************************************************************/
uint8 FanControl_ErrCodeGet(void)
{
	return sFanData.u8ErrCode;
}

/******************************************************************************
* Function:    void FanVolt_Control(void)
* 
* Description:   �ķ������ ���غ���
*
* return :     none
* Note  :   
*
************************************************************************************/
void FanVolt_Control(void)
{
	static uint16	TimerFanControl;
	int16  i16Delta;                          //�������
	ADVoltType avCurrentVolt;                 //���������ѹ
	uint16 u16DiffVolt;                       //��Ŀ���ѹ�Ĳ��
	int32  i32Pid;
	ADVoltType avSupperVolt, avBackVolt;      //�ķ���˵�ѹ��������ѹ
	ADVoltType avSystemVolt;                 //ϵͳ�ĵ�ѹ12��24
	int16 i16VoltOffset;
	//PID ����Ӧ�������ѹ��Χ��
	ADVoltType avCloseVolt; 
	ADVoltType avStopAdjVolt;	
	uint8  u8FanAdjState;                    //0 �����ڣ� 1 ���ϣ� 2����
	
	if (TimeOutChkMsLong(&TimerFanControl, FAN_CONTROL_TIME))
	{  //10ms ����һ��
		//����ķ���˵�ѹ
		avSupperVolt = FanHW_SupperVolt();
		if (sFanData.bFanRealy)
		{
			avBackVolt = FanHW_BackVolt();
		}
		else
		{
			avBackVolt = avSupperVolt;
		}
		
		
   	if (sFanConfig.bSystemVolt24)
   	{
   		avCloseVolt = IGN_CLOSE_24VOLT;
   		avStopAdjVolt = IGN_STOPADJ_24VOLT;
   		avSystemVolt = SYSTEM_VOLT_24;
   	}
   	else
   	{
   		avCloseVolt = IGN_CLOSE_12VOLT;
   		avStopAdjVolt = IGN_STOPADJ_12VOLT;
   		avSystemVolt = SYSTEM_VOLT_12;
   	}
   	
   	//����Դ��ѹ����5v������ϼ�ʱ���
      if (avSupperVolt <= avCloseVolt)
   	{
      	sFanData.ShortErrTime = 0;
      	sFanData.OpenErrTime  = 0;
      	sFanData.u8ErrCode = FAN_ERRCODE_NONE;
   	}
		
		if (avSupperVolt >= avBackVolt)
		{
			//�˴�Ϊ����ѹ���Ĵ����õ���ǰ�Ĺķ��ʵ�ʵ�ѹavCurrentVolt
			avCurrentVolt = avSupperVolt - avBackVolt;
			if (avCurrentVolt > avSystemVolt)
			{
				i16VoltOffset = sFanConfig.u8VoltOffSet;
			}
			else 
			{
				i16VoltOffset = GAMath_LineFuni16(0, avSystemVolt, 256, sFanConfig.u8VoltOffSet, avCurrentVolt);
			}
			
			if (i16VoltOffset > 256 || i16VoltOffset < sFanConfig.u8VoltOffSet) i16VoltOffset = sFanConfig.u8VoltOffSet;
			else {}
			
			avCurrentVolt = (uint16)((uint32)avCurrentVolt * i16VoltOffset >>8); 
		}
		else
		{//����Դ��ѹС�ڷ�����ѹ�� FAN_ERRCODE_BACK
			avCurrentVolt = 0;
		}

		if (avSupperVolt <= avCloseVolt)
		{//����Դ��ѹ���ڹرյ�ѹ��5v���������Ϊ0
			sFanData.u16FanPWM = 0;
			FanHW_OutPWM(0);	
			FanHW_OutFanRealy(0);
		}
		else if (avSupperVolt <= avStopAdjVolt)
		{//�����ѹ�仯ʱ��ͣ���ڣ�PID ���¼���
			sFanData.u8AdjState = FAN_STATE_PID;
			sFanData.i16ErrSum = 0;
			FanHW_OutPWM(sFanData.u16FanPWM);	
			FanHW_OutFanRealy(sFanData.bFanRealy);
		}
		else
		{
			//����Ϊ���㵱ǰ��Ӧ�õķ��������ٶ�
			if (sFanData.avSetVolt > sFanData.avCurrentSetVolt)
			{
				u8FanAdjState = 1;
			}
			else if (sFanData.avSetVolt < sFanData.avCurrentSetVolt)
			{
				u8FanAdjState = 2;
			}
			else
			{
				u8FanAdjState = 0;
			}
			//�����������õ�ѹ������ǰӦ�õ��ĵ�ѹֵ
			int16 i16FanAdjSetV;   //���㵱ǰĿ��ĵ�ѹ�Ե�������
			i16FanAdjSetV = sFanData.u16SetRate * FAN_CONTROL_TIME / 100;
			if (i16FanAdjSetV == 0)
			{
				i16FanAdjSetV = 1;
			}else{}
			
			//���յ����ٶȸ���Ŀ���ѹ���Ҽ������ǰ��λ
			if (sFanData.avSetVolt <= 0)
			{
				sFanData.u8AdjState = FAN_STATE_CLOSE;
				sFanData.avCurrentSetVolt = 0;
				sFanData.u16SetRate = sFanConfig.u16InitRate; //
			}
//			else if (sFanData.u8AdjState == FAN_STATE_START)
//			{
//				if (sFanData.avSetVolt > sFanConfig.TBL_FanVolt[1])
//				{
//					sFanData.avCurrentSetVolt = sFanConfig.TBL_FanVolt[1];
//				}
//				else 
//				{
//					sFanData.avCurrentSetVolt = sFanData.avSetVolt;
//				}
//			}
			else if (u8FanAdjState == 1)
			{
				sFanData.avCurrentSetVolt += i16FanAdjSetV; 
				if (sFanData.avCurrentSetVolt >= sFanData.avSetVolt)
				{
					sFanData.avCurrentSetVolt = sFanData.avSetVolt;
				}else{}
			}
			else if (u8FanAdjState == 2)
			{
				sFanData.avCurrentSetVolt -= i16FanAdjSetV;    
				if (sFanData.avCurrentSetVolt <= sFanData.avSetVolt)
				{
					sFanData.avCurrentSetVolt = sFanData.avSetVolt;
				}else{}
			}
			else{}

			//����ѹת��Ϊ��λ
			sFanData.u8CurrentLevel = GAMath_CalTBLi16Level(sFanData.u8CurrentLevel, sFanData.avCurrentSetVolt,
						(int16*)sFanConfig.TBL_FanVolt, sFanConfig.u8FanMaxLevel);
			
			if (sFanData.u8CurrentLevel >= sFanConfig.u8FanMaxLevel )
			{
				sFanData.u8CurrentLevel = sFanConfig.u8FanMaxLevel;
			}
			else
			{}
			
			if (sFanData.avSetVolt > 0)
			{//�����ò�Ϊ0ʱ����λ�϶�>=1 
				if (sFanData.u8CurrentLevel == 0)
				{
					sFanData.u8CurrentLevel = 1;
				}
				else
				{}
			}
			else
			{
				sFanData.u8CurrentLevel = 0;
			}
			
			
			//��ǰ�����
			if (sFanData.avSetVolt >= avCurrentVolt)
			{
				u16DiffVolt = sFanData.avSetVolt - avCurrentVolt;	
			}
			else
			{
				u16DiffVolt = avCurrentVolt - sFanData.avSetVolt;		
			}	
			//�����ж�
			if (u16DiffVolt >= ACD_V(0.5))
			{
				if (sFanData.avSetVolt < (avMaxFanVolt - ACD_V(1)) && sFanData.avSetVolt != 0 
						&& avBackVolt <= FANSHORT_VOLT)
				{//���������ʱ��0����������ѹС��1V
					sFanData.ShortErrTime++;
					if (sFanData.ShortErrTime >= SHORT_ERR_TIME)
					{//���ϳ���1s, ��ȷ�Ϲ���
						sFanData.u8ErrCode = FAN_ERRCODE_SHORT;
					}
					else{}
				}
				else
				{
					sFanData.ShortErrTime = 0;
				}
			
				if (sFanData.u16FanPWM >= sFanConfig.u16MinPwm && avCurrentVolt <= FANOUT_OPENVOLT)
				{ //�����ռ�ձ�>= FANOUT_MINPWM  ����������ѹ�ӽ���Դ��ѹ��FAN_ERRCODE_OPEN
					//�� avCurrentVolt <= FANOUT_OPENVOLT�� �������ʱ ������ѹ����ӽ���Դ��ѹ�����߹ķ���˵�ѹ����̫С
					sFanData.OpenErrTime++;
					if (sFanData.OpenErrTime >= OPEN_ERR_TIME)
					{//�ι��ϳ���1s ��ȷ�Ϲ���
						sFanData.u8ErrCode = FAN_ERRCODE_OPEN;
					}
					else{}
				}
				else
				{
					sFanData.OpenErrTime = 0;
				}
			}
			else
			{
				sFanData.ShortErrTime = 0;
				sFanData.OpenErrTime = 0;
				if (u16DiffVolt <= FAN_CONTROL_ACCURACY_VOLT)
				{
					sFanData.u8ErrCode = FAN_ERRCODE_NONE;
				}
				else{}
			}
	//�����ж�end

			
			//pid ����
			i16Delta = sFanData.avCurrentSetVolt - avCurrentVolt;  //����� ��Ŀ���ٶȵĲ�
			
			sFanData.i16ErrSum += i16Delta;   //������
			if (sFanData.i16ErrSum  >  PID_I_MAXVALUE) 
			{
				sFanData.i16ErrSum =  PID_I_MAXVALUE;
			}
			else if (sFanData.i16ErrSum  < -PID_I_MAXVALUE) 
			{
				sFanData.i16ErrSum = -PID_I_MAXVALUE;
			}
			else{}
	
			//����״̬����״̬����������ٶ�
			switch (sFanData.u8AdjState)
			{
				default:
				case FAN_STATE_CLOSE:
					if (sFanData.avSetVolt <= 0)
					{
						sFanData.bFanRealy = 0;
					}
					else
					{
						sFanData.bFanRealy = 1;
						sFanData.u8AdjState = FAN_STATE_START;
						sFanData.avCurrentSetVolt = sFanConfig.TBL_FanVolt[1];
					}
					sFanData.u16FanPWM = 0;
					i32Pid = 0;
					sFanData.u8AdjStateTime =0;
					sFanData.i16ErrSum = 0;
					break;
				case FAN_STATE_START:
					sFanData.u8AdjStateTime++;
					if (sFanData.u8AdjStateTime > FAN_START_TIME)
					{
						sFanData.u8AdjStateTime = 0;
						sFanData.u8AdjState = FAN_STATE_PID;
					}
					else{}
					//��״̬��ֱ������PID����break
				case FAN_STATE_PID:	
					if ( (u8FanAdjState == 1 && i16Delta < 0) || (u8FanAdjState == 2 && i16Delta > 0) )
					{//����ֻ��Ϊ+������ֻ��Ϊ-�����ʱ��������
						i32Pid = 0;
						sFanData.i16ErrSum = 0;
					}
					else
					{
						//����PID����
						if (u16DiffVolt <= FAN_CONTROL_SLOWADJ_VOLT)
						{
							sFanData.u8AdjStateTime = 0;
							sFanData.u8AdjState = FAN_STATE_SLOWADJ;
						}
						else{}
						
						i32Pid = i16Delta * (int32)sFanConfig.i16FanPFactor;
						i32Pid += sFanData.i16ErrSum * (int32)sFanConfig.i16FanIFactor;
						i32Pid += (i16Delta - sFanData.i16Err_1) * (int32)sFanConfig.i16FanDFactor;//+΢����
					}
					sFanData.bFanRealy = 1;
					break;
				case FAN_STATE_SLOWADJ:
					//���ٵ���,ÿ�ε���1
					if (u16DiffVolt > FAN_CONTROL_SLOWADJ_VOLT)
					{
						sFanData.u8AdjStateTime++;
						if (sFanData.u8AdjStateTime > FANADJ_STOPTIMES)
						{//���200ms�ڶ������⣬���˳�
							sFanData.u8AdjStateTime = 0;
							sFanData.u8AdjState = FAN_STATE_PID;
						}else{}
					}
					else if (u16DiffVolt > FAN_CONTROL_ACCURACY_VOLT)
					{
						if (sFanData.u8AdjStateTime > 0)
						{
							sFanData.u8AdjStateTime--;
						}else{}
					}
					else
					{
						sFanData.u8AdjStateTime = 0;
					}
					
					if (sFanData.i16ErrSum * sFanConfig.u8SlowAdjFactor >= PID_PARAMETER_SHIFTVALUE)
					{
						i32Pid = PID_PARAMETER_SHIFTVALUE;
						sFanData.i16ErrSum = 0;
					}
					else if (sFanData.i16ErrSum * sFanConfig.u8SlowAdjFactor <= -PID_PARAMETER_SHIFTVALUE)
					{
						i32Pid = -PID_PARAMETER_SHIFTVALUE;
						sFanData.i16ErrSum = 0;
					}
					else
					{
						i32Pid = 0;
					}
					sFanData.bFanRealy = 1;
					break;
				case FAN_STATE_STOP:
					//ֹͣ
					i32Pid = 0;
					break;	
			}
			
			sFanData.i16Err_1 = i16Delta; //������һ�ε����,��ѹ

			if (i32Pid < 0 && i32Pid > -PID_PARAMETER_SHIFTVALUE)
			{
				i32Pid = 0;
			}
			else
			{
				i32Pid >>= PID_PARAMETER_SHIFTNUM;
			}
	
			if (sFanData.u8AdjState != FAN_STATE_CLOSE)
			{
				i32Pid += sFanData.u16FanPWM;	
				//pwm �ķ�Χ	
				if (i32Pid < sFanConfig.u16MinPwm)
				{
					sFanData.u16FanPWM = sFanConfig.u16MinPwm;
				}
				else if (i32Pid >= FANOUT_MAXPWM)
				{
					sFanData.u16FanPWM = FANOUT_MAXPWM;
				}
				else
				{
					sFanData.u16FanPWM = i32Pid;
				}
			}
			else
			{
				sFanData.u16FanPWM = 0;
			}
			
			sFanData.avLastVolt = avCurrentVolt;
			// piD ���ƽ���
			
			if (sFanData.avSetVolt == avMaxFanVolt && sFanData.avLastVolt >= (avMaxFanVolt - ACD_V(0.5))
					&& sFanData.u16FanPWM > MAXOUT_MINPWM )
			{//���ķ��Ϊ���ʱ���ҹķ��������ѹ�Ѿ���ﵽ���õ�ѹʱ��pwmΪ���ֵ
				sFanData.u16FanPWM = FANOUT_MAXPWM;
			}
			else{}
			
			FanHW_OutPWM(sFanData.u16FanPWM);//���
			FanHW_OutFanRealy(sFanData.bFanRealy);
		}
	}
	else
	{
		//10msδ��
	}
}
