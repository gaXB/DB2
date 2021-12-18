/**************************文件说明*****************************************************
* Copyright (c) 2019 ,杭州广安汽车电器有限公司  All rights reserved.
*
* 文件名称：ADTreat.c
* 功能描述：AD 模块采集，并转化物理值并判断是否故障
* 1  AD 采集使用中断模式
* 2  物理意义转化包括3路电压，8路温度
* 3  等待模式采集一路点火电压的功能
* 
* 本模块使用需要定时器模块及数学函数处理模块
* 编译器： codewarrior IDE 10.7-KEAZ128
* MCU说明：此文件跟MCU无关
* 
* */
/**************************更改记录******************************************************
 * 
* 版本：1.0
* 说明：初始版本建立， 从原AD模块的1.2.6移植
* 原作者：xb
* 完成日期：2019年5月14日
* 
* 版本：1.1
* 修改 BOOL ADInit(uint8 u8MaxNum, uint8 u8VoltRegisterType) 来调用不同的电阻值类型
* 0 : 10/3.6 改为 1 : 100/33 
* 作者：xb
* 完成日期：2020年3月28日
*/


#include "ADTreat.h"
#include "mathGA.h"


//AD数据类型声明
typedef struct
{
	uint16 AdVData;          		  //滤波后的值
	uint16 AdCData;         		  //当前采集值
	uint16 AdVBuff[AD_FILTER_NUM];    //buffer	
}ADDATA;

//实际物理意义数据类型
typedef struct
{
	int16 Value;               //温度值,或者电压
	const uint16  *TBL_TempAD;  //温度表，电压时不用
	uint8 ErrCode;             //故障码
}ADPHYDATA;

//AD 及物理意义值，缓存变量数据定义
static ADDATA sADData[AD_MAXNUM];
static ADPHYDATA sADPhyData[ADVOLT_MAXNUM+ADTEMP_MAXNUM];

static uint8 u8ADTotalNum, u8ADTUseNum, u8ADVUseNum;  //实际数量
static uint8 u8SupVoltMode;          //0 12v, 1:24v

static uint8 u8RBuffTimes;           //读取buff 数量 ，8次滤波
static uint8 u8ConverState;          //AD转换状态
static uint8 u8AdPollPort;           //轮询采集值

static uint8 u8ADModuleErrCode, u8ADErrChanle;   //故障码及故障通道
static uint16 u16HWBackData;        // 硬件采集值
static uint8 u8RegisterType = 0;         // 1: 100k 30k分压     0:10k/3.6k
 /**************************************************
 通过硬件驱动需要调用的函数声明
 ****************************************************/
//外部调用的函数
extern void   ADCH_SEL(uint8 adid);
//本模块实现的函数，仅限本模块使用
static void Deal_ADToVolt(ADPHYDATA *pVoltSignal, uint16 u16ADVolt);
static ADTempType Get_Ad_T (uint16 adcal, const uint16 *tbl_tad);
static void Deal_ADToTemp(ADPHYDATA *pTempSignal, uint16 u16ADValue);
/*******************************************************************************
* Function:      BOOL ADInit(uint8 u8MaxNum)
*
* Description:   ad模块初始化， 使用的最大AD个数
*
*  parameter ：    u8MaxNum 最大支持数量为25个 
* return :       设置正确TRUE

* Note  :       上电初始化时调用

************************************************************************************/
BOOL ADInit(uint8 u8MaxNum, uint8 u8VoltRegisterType)
{
	BOOL bADInitResult;
	if (u8MaxNum > AD_MAXNUM)
	{
		bADInitResult = FALSE;
		u8ADTotalNum = 0;
	}
	else
	{
		bADInitResult = TRUE;
		u8ADTotalNum = u8MaxNum;
		u8ConverState = ADCONVER_STATE_INIT;  
		u8ADModuleErrCode = 0;
		u8AdPollPort = 0;
		u8RBuffTimes = 0;
	}
	
	u8RegisterType = u8VoltRegisterType;
	return bADInitResult;
}
/*******************************************************************************
* Function:      BOOL ADPhy_Init(uint8 u8VNum, uint8 u8IgnMode, uint8 u8TNum, const uint16** pTBL)
*
* Description:   ad模块物理意义转化功能初始化
*
* paramter ：   u8VNum：  电压的支持数量，最大3
*             u8IgnMode：  系统电压转化方式， 1：24v， 0：12v
*             u8TNum： 需要进行温度转换的数量，最大8
*             **pTBL： 温度表的数组
*             
* return :     设置正确TRUE; 
* 
* Note  :      上电初始化时调用
*
************************************************************************************/
BOOL ADPhy_Init(uint8 u8VNum, uint8 u8IgnMode, uint8 u8TNum, const uint16** pTBL)
{
	BOOL bADVInitResult = TRUE;
	uint8 u8i;
	
	if (u8VNum > ADVOLT_MAXNUM || u8TNum > ADTEMP_MAXNUM || u8IgnMode >= ADVOLT_MODE_INVAILED)
	{
		bADVInitResult = FALSE;
		u8ADTUseNum = 0;
		u8ADVUseNum = 0;
	}
	else
	{
		u8ADTUseNum = u8TNum;
		u8ADVUseNum = u8VNum;
		u8SupVoltMode = u8IgnMode;
		
		for (u8i = 0; u8i < u8ADTUseNum; u8i++)
		{
			if (pTBL[u8i] == PDATA_NULL)
			{
				bADVInitResult = FALSE;
			}
			else
			{
				sADPhyData[u8i +ADID_TEMP_BEGIN].TBL_TempAD = pTBL[u8i];
			}
		}
	}
	return bADVInitResult;
}

/*******************************************************************************
* Function:       uint16  ADValue_Get(uint8 adid, ADValueGetType u8ADGetType)
*                
*           
* Description:    adid： 需要采集ad的ID，返回此信号的ad，
*                 u8ADGetType ： AD值 类型， 见enum ADValueGetType
*                 
* return :         0---255

* Note  :         当adid错误时 ，返回0

************************************************************************************/
uint16  ADValue_Get(uint8 adid, ADValueGetType u8ADGetType)
{
   uint16 u16AD;
   
   if(adid >= u8ADTotalNum || u8ADGetType >= ADVALUE_GETTYPE_INVAILED)
   {
   	u16AD = 0;
   }
   else
   {
      if (u8ADGetType == ADVALUE_GETTYPE_8AD || u8ADGetType == ADVALUE_GETTYPE_10AD)
      {
      	u16AD = sADData[adid].AdVData;
      }
      else
      {
      	u16AD = sADData[adid].AdCData;
      }
      
      if (u8ADGetType == ADVALUE_GETTYPE_8AD || u8ADGetType == ADVALUE_GETTYPE_8ADFAST)
      {//是8位 移位处理
   		if ((u16AD & 0x3) == 0x03)
   		{
   			u16AD++;
   		}else{}
   		
   		if (u16AD >= 1023)
   		{
   			u16AD = 1023;
   		}else{}
   		u16AD >>=2;
      }
      else{}
   }
   
   return  (u16AD);
}
/*******************************************************************************
* Function:        int16 ADPhy_GetFact(uint8 adid)
*						 
* Description:     返回adid通道的实际物理值，
* 
* return :         温度 -40--80  实际温度：data/10  
                                      电压  0- 8000  实际电压： data/100    精度： 0.05v
                   
* Note  :      

************************************************************************************/
int16 ADPhy_GetFact(uint8 adid)
{
	int16 i16Value;
	if(adid >= (ADTEMP_MAXNUM + ADVOLT_MAXNUM))
	{
		i16Value = 0;
	}
	else
	{
		i16Value = sADPhyData[adid].Value;
	}
	
	return i16Value;
}

/*******************************************************************************
* Function:        uint8 ADPhy_GetErr(uint8 adid)
*						 
* Description:     返回adid通道的故障码
* 
* return :         ADPHY故障类型  ADPHYERR_XXX
                   
* Note  :      

************************************************************************************/
uint8 ADPhy_GetErr(uint8 adid)
{
	uint8 u8ErrCode;
	if(adid >= (ADTEMP_MAXNUM + ADVOLT_MAXNUM))
	{
		u8ErrCode = 0xff;
	}
	else
	{
		u8ErrCode = sADPhyData[adid].ErrCode;
	}
	
	return u8ErrCode;
}
/*******************************************************************************
* Function:     uint8 ADModule_ErrCode (void)
*
* Description:    返回此模块的故障码
* 
* return :         
* Note  :      
************************************************************************************/
uint8 ADModule_ErrCode (void)
{
	return u8ADModuleErrCode;
}

/*******************************************************************************
* Function:       uint8 ADModule_Test(ADTESTDATA* pTestTable)
*
* Description:    模块测试
* parameter :     将AD值及物理意义 返回到 pTestTable
* return :        反回故障码  
* Note  :      
************************************************************************************/
uint8 ADModule_Test(ADTESTDATA* pTestTable)
{
	uint8 u8ADIDNum;
	ADDATA *pADData = sADData;
	ADPHYDATA *pPhyData = sADPhyData;
	ADTESTDATA* pADTest = pTestTable;
	
	for (u8ADIDNum = 0; u8ADIDNum < u8ADTotalNum; u8ADIDNum++)
	{
		pADTest->u16ADValue = pADData->AdVData;
		pADTest->i16PhyValue = pPhyData->Value;
		
		pPhyData++;
		pADData++;
		pADTest++;
	}

	return u8ADModuleErrCode;		
}

/*******************************************************************************
* Function:        void ADHWEnd_Interrupt(uint16 u16ReadResult)
*
* Description:    ad 中断时，通过此函数将ad值传给本模块
*
* parameter :  u16ReadResult 读到的AD值
* 
* return :       none
* Note   :       AD中断函数调用
************************************************************************************/
void ADHWEnd_Interrupt(uint16 u16ReadResult)
{
	u16HWBackData = u16ReadResult;
	if (u8ConverState == ADCONVER_STATE_INTREAD)
	{
		if (u8AdPollPort < u8ADTotalNum)
		{
			sADData[u8AdPollPort++].AdCData = u16HWBackData;
			
			if (u8AdPollPort >= u8ADTotalNum)
			{//本轮转换完成
				u8ConverState = ADCONVER_STATE_INTOK;
			}
			else
			{
				ADCH_SEL(u8AdPollPort);
			}
		}
		else
		{
			u8ADModuleErrCode |= ADMODULE_ERRCODE_INT;
		}
	}
	else if (u8ConverState == ADCONVER_STATE_WAITREAD)
	{
		u8ConverState = ADCONVER_STATE_WAITOK;
	}
	else
	{
		u8ADModuleErrCode |= ADMODULE_ERRCODE_INT;
	}
}

/*******************************************************************************
 * Function:   static void ADConverPhysic(void)
 *
 * Description: 将AD 值的转化为物理实际意义
 *
 * Returns:     none
 *
 * Notes:   
 *
*******************************************************************************/
static void ADConverPhysic(void)
{
	uint8 u8ADIDNum;
	ADDATA *pADData;
	ADPHYDATA *pPhyData;
	
	pADData = sADData;
	pPhyData = sADPhyData;
	for (u8ADIDNum = 0; u8ADIDNum < u8ADVUseNum; u8ADIDNum++)
	{
		Deal_ADToVolt(pPhyData, pADData->AdVData);
		pADData++;
		pPhyData++;
	}
	
	pADData = &sADData[ADID_TEMP_BEGIN];
	pPhyData = &sADPhyData[ADID_TEMP_BEGIN];
	for (u8ADIDNum = ADID_TEMP_BEGIN; u8ADIDNum < (ADID_TEMP_BEGIN + u8ADTUseNum); u8ADIDNum++)
	{
		Deal_ADToTemp(pPhyData, pADData->AdVData);
		pADData++;
		pPhyData++;
	}
}


/*******************************************************************************
 * Function:   static void ADC_INT_Begin(void)
 *
 * Description: 中断采集，开始
 *
 * Returns:     none
 *
 * Notes:   
 *
*******************************************************************************/
static void ADC_INT_Begin(void)
{
	u8AdPollPort = 0;
	ADCH_SEL(0);
	u8ConverState = ADCONVER_STATE_INTREAD;
}
/*******************************************************************************
 * Function:   void Treat_Read_Ad(void)
 *
 * Description: 采集各个AD，并对ad进行求和平均处理，并求出其物理意义
 *
 * Returns:     none
 *
 * Notes:   此处为AD采集控制的核心函数，每1ms采集1个数据
 *          该函数仅被main函数中的主任务循环调用
 *
 *
*******************************************************************************/
void Treat_Read_Ad(void)
{ 
   static uint16 TimerADC;
   static uint8 u8UNCompleteTimes;
   ADDATA *pADData;
   uint8 u8i, u8j;
   
	if (TimeOutChkMsLong(&TimerADC, ADTREAT_READTIME))
	{
		switch (u8ConverState)
		{
			default:
			case ADCONVER_STATE_INIT:
				u8RBuffTimes = 0;
				ADC_INT_Begin();
				break;
			case ADCONVER_STATE_INTREAD:
				//未转化完成,重新开始转换
				u8UNCompleteTimes++;
				if (u8UNCompleteTimes >= ADTREAT_UNCOMPLETE_TIMES)
				{//2次都失败，则记录故障
					u8UNCompleteTimes = 0;
					u8RBuffTimes = 0;
					u8ADModuleErrCode |= ADMODULE_ERRCODE_UNCOMPETE;
					u8ADErrChanle = u8AdPollPort;
					ADC_INT_Begin();
				}
				break;
			case ADCONVER_STATE_INTOK:
				//转换完成
				//将菜到的AD值放入ad缓存
				u8UNCompleteTimes = 0;
				pADData = sADData;
				for (u8i = 0; u8i < u8ADTotalNum; u8i++)
				{
					pADData->AdVBuff[u8RBuffTimes] = pADData->AdCData;
					pADData++;
				}
				
				u8RBuffTimes++;
				
				if (u8RBuffTimes >= AD_FILTER_NUM)
				{//8次滤波完成，进行求平均，和物理意义转换
					u8RBuffTimes = 0;
					pADData = sADData;
					for (u8i = 0; u8i < u8ADTotalNum; u8i++)
					{
						pADData->AdVData = 0;
						for (u8j = 0; u8j < AD_FILTER_NUM; u8j++)
						{
							pADData->AdVData += pADData->AdVBuff[u8j];
						}
						pADData->AdVData /= AD_FILTER_NUM;
						pADData++;
					}
					//数据转换
					ADConverPhysic();
				}
				else{}
				ADC_INT_Begin();//开始下缓存个转换
				break;

		}
	}
	else
	{}
}
/*******************************************************************************
* Function:        static uint8 Get_Ad_T ( uint8 adcal, const uint8*tbl_tad)
*
* Description:    通过ad值得到温度.
                   adcal :当前的ad，   *tbl_tad： 对应的温度表
* return :       -40度到80度  i -400---800
* Note  :        温度采集通过查表采集，精度为0.1度
                 5度一格
************************************************************************************/
static ADTempType Get_Ad_T (uint16 adcal, const uint16 *tbl_tad)
{
   int16 i;
   ADTempType tBackTemp;
   

	if (adcal >= tbl_tad[0]) 
	{
		tBackTemp = -400;
	}
	else if (adcal <= tbl_tad[24]) 
	{ 
		tBackTemp = 800;
	}
	else 
	{
		for (i = 0; adcal < tbl_tad[i]; i++)
		{// 因有前面的判断，所以 i 在 1 - 24 之间
		}
		if (i > 0)
		{ 
			i--; //这个i不会变成0，  之后 tbl[i] > adcal ;  tbl[i+1] <= adcal
		}
		else{}
		//  t   adcal   ( i*50 - 400 )      tbl_tad[i]      
		//              ((i+1)*50 -400)     tbl_tad[i+1]   
		//                                    adcal
		if (tbl_tad[i] == tbl_tad[i+1]) 
		{
			tBackTemp = (i*50) - 400;
		}
		else
		{                  
			tBackTemp = GAMath_LineFuni16(tbl_tad[i], tbl_tad[i+1],  (i*50 - 400), (i*50 - 350), adcal);
			//tTempCal = (adcal - tbl_tad[i]) * 50 / (tbl_tad[i+1] - tbl_tad[i]) + i * 50 -400;
		}
	}
	return tBackTemp; 
}
//*/
/*******************************************************************************
 * Function:   static void Deal_TempSignal(ADDATA *pTempSignal)
 *
 * Description: 处理  pTempSignal 的电压信号值  并判断其是否故障
 *
 * Returns:     none
 *
 * Notes:
 *
*******************************************************************************/
static void Deal_ADToTemp(ADPHYDATA *pTempSignal, uint16 u16ADValue)
{
	pTempSignal->Value = Get_Ad_T(u16ADValue, pTempSignal->TBL_TempAD);    

   //得到故障码
   if (u16ADValue >= AD_TEMPERR_OPEN)
   {
   	pTempSignal->ErrCode = ADPHYERR_TEMP_OPEN;
   }
   else if(u16ADValue <= AD_TEMPERR_SHORT)
   {
   	pTempSignal->ErrCode = ADPHYERR_TEMP_SHORT;
   }
   else
   {
   	pTempSignal->ErrCode = ADPHYERR_TEMP_OK;
   }
}
/*******************************************************************************
 * Function:   static void Deal_VoltSignal(ADDATA *pVoltSignal)
 *
 * Description: 处理  pVoltSignal 的电压信号值，并判断其是否故障
 *
 * Returns:     none
 *
 * Notes:
 *
*******************************************************************************/
static void Deal_ADToVolt(ADPHYDATA *pVoltSignal, uint16 u16ADVolt)
{
	if (u8SupVoltMode == ADVOLT_MODE_12V)
	{//12v ,用100k，30k分压
		if (u8RegisterType)
		{
			pVoltSignal->Value = (int16)((252 * (int32)u16ADVolt) >> 7);
		}
		else
		{
			pVoltSignal->Value = (59 * u16ADVolt) >> 5;
		}
		//故障判断
		if (pVoltSignal->Value > AD_VOLT12_HIGHERR)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_HIGH;
		}
		else if (pVoltSignal->Value < AD_VOLT12_LOWERR)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_LOW;
		}
		else if (pVoltSignal->Value > AD_VOLT12_LOWOK && pVoltSignal->Value < AD_VOLT12_HIGHOK)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_OK;
		}
		else
		{//不变
			
		}
	}
	else if (u8SupVoltMode == ADVOLT_MODE_24V)
	{//24 用200k，30k分压
		pVoltSignal->Value = (int16)((479 * (int32)u16ADVolt) >> 7);
		//故障判断
		if (pVoltSignal->Value > AD_VOLT24_HIGHERR)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_HIGH;
		}
		else if (pVoltSignal->Value < AD_VOLT24_LOWERR)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_LOW;
		}
		else if (pVoltSignal->Value > AD_VOLT24_LOWOK && pVoltSignal->Value < AD_VOLT24_HIGHOK)
		{
			pVoltSignal->ErrCode = ADPHYERR_VOLT_OK;
		}
		else
		{//不变
			
		}
	}
	else
	{
		pVoltSignal->Value = 0;
		pVoltSignal->ErrCode = ADPHYERR_VOLT_INVAILED;
	}
}

/*******************************************************************************
 * Function:   ADVoltType ReadIgn_Only(uint8 VoltId)
 *
 * Description:  只读取点火电压，
 *
 * Returns:     none
 *
 * Notes:  此模块只能在休眠时使用

 *
*******************************************************************************/
ADVoltType ReadIgn_Only(uint8 VoltId)
{
	static uint16 u16WaiteTimes = 0;
	uint8 u8ADIgnTimes, u8ConverOKTimes;
	ADVoltType avBackVolt = 0;
	
	while (u8ConverState != ADCONVER_STATE_INTOK && u16WaiteTimes < RIGN_INTCONVER_TIMES)
	{
		u16WaiteTimes++;
	};//中断采集还没完成，等待完后无论是否完成，都转为等待采集模式
	
	u8ADIgnTimes = 0;
	u8ConverOKTimes = 0;
	
	do 
	{
		u16WaiteTimes = 0;
		u8ConverState = ADCONVER_STATE_WAITREAD;
		ADCH_SEL(VoltId);
		while (u16WaiteTimes < RIGN_WAITAD_TIMES)
		{
			u16WaiteTimes++;
		};
		
		if (u8ConverState != ADCONVER_STATE_WAITOK)
		{
			
		}
		else
		{
			sADData[VoltId].AdVData = u16HWBackData;
			u8ConverOKTimes++;
		}
		u8ADIgnTimes++;
	}while(u8ADIgnTimes < READ_ADIGN_TIMES);

	if (u8ConverOKTimes > 0)
	{//有一次成功即可
		Deal_ADToVolt(&sADPhyData[VoltId], sADData[VoltId].AdVData);
		avBackVolt = sADPhyData[VoltId].Value;
	}
	else
	{
		avBackVolt = 0;
	}
	
	u8ConverState = ADCONVER_STATE_INTOK;
	return avBackVolt;
}

