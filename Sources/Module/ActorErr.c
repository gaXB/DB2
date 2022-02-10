/*
 * ActorErr.c
 *
 *  Created on: 2022年2月9日
 *      Author: Administrator
 */

#include "SystemData.h"
#include "APPDrive.h"
#include "WaterPump.h"


void Deal_ActorErr(void)
{
//3way1 处理
	if (EXVGetLostErr(EXV_NUM_THREEWAY1))
	{
		sActorData.ThreeWay1Err = 1;
	}
	else
	{
		if (l_u8_rd_LI0_BDC_1_voltageErr() || l_u8_rd_LI0_BDC_1_Fault_Signal())
		{
			sActorData.ThreeWay1Err = 2;
		}
		else
		{
			sActorData.ThreeWay1Err = 0;
		}
	}

//3way2 处理
	if (EXVGetLostErr(EXV_NUM_THREEWAY2))
	{
		sActorData.ThreeWay2Err = 1;
	}
	else
	{
		if (l_u8_rd_LI0_BDC_2_voltageErr() || l_u8_rd_LI0_BDC_2_Fault_Signal())
		{
			sActorData.ThreeWay2Err = 2;
		}
		else
		{
			sActorData.ThreeWay2Err = 0;
		}
	}
//EXV
	if (EXVGetLostErr(EXV_NUM_EXV) || l_u8_rd_LI0_EXV2_FaultState() || l_bool_rd_LI0_EXV2_ResponseError() ||
			l_u8_rd_LI0_EXV2_VoltageState())
	{
		sActorData.EXVError = 1;
	}
	else
	{
		sActorData.EXVError= 0;
	}

//BEXV
	if (EXVGetLostErr(EXV_NUM_BEXV) || l_u8_rd_LI0_EXV_FaultState() || l_bool_rd_LI0_EXV_ResponseError() ||
			l_u8_rd_LI0_EXV_VoltageState())
	{
		sActorData.BEXVError = 1;
	}
	else
	{
		sActorData.BEXVError= 0;
	}

	if (l_bool_rd_LI0_CCV4_Rsp_Comm_Err() || l_u8_rd_LI0_CCV4_voltageErr() || l_u8_rd_LI0_CCV4_Fault_Signal())
	{
		sActorData.FourWayErr = 1;
	}
	else
	{
		sActorData.FourWayErr = 0;
	}

	sActorData.EWP1Error = WPumpGetErr(0);
	sActorData.EWP2Error = WPumpGetErr(1);
	sActorData.EWP3Error = WPumpGetErr(2);

	sActorData.SOV1Error = SRealyGetErr(SREALY_NUM_SOV1);
	sActorData.SOV2Error = SRealyGetErr(SREALY_NUM_SOV2);
}
