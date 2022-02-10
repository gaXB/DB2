/************************�ļ�˵��*****************************************************
* Copyright (c) 2020 ,���ݹ㰲�����������޹�˾  All rights reserved.
*
* �ļ����ƣ�CommSet.c
* ���������� CAN LIN �ź����ã��� CommSetTask()��ʵ��
* 1 100ms����һ�Σ��������⹦����Ҫ��InterLay_App.c��ʵ��
* �������� codewarrior IDE 11.1
* MCU˵������MCU �޹�
*  
*/
/**************************���ļ�¼******************************************************
 * 
* �汾��1.0
* ˵������ʼ�汾����
* ԭ���ߣ�xb
* ������ڣ�2020��1��13��
* 
*/
#include "CommSet.h"
#include "APPDrive.h"
#include "OutANDDisplay.h"
#if _PROJECT_CAN_
#include "InterLayMatrix.h"
#endif



/*******************************************************************************
 * Function:        void CommSetTask(void)
 *
 * Description:     �ź����ã��ײ�����loop���� 
 *
 * Returns:        none
 *
 * Notes:           		
 *
********************************************************************************/
void CommSetTask(void)
{
	static uint16 TimerSetMessage;
	extern int16 i16HartErrNum;
	if (TimeOutChkMsLong(&TimerSetMessage, 10))
	{//TODO: �ź�ֵ�����ڴ˴���д
		//CAN2 ����
		sCANMsg551.Bits.Reserved_4 = (uint8)i16HartErrNum;
		sCANMsg551.Bits.Reserved_3 = (uint8)MCUPM_PowerOnResetTimes();


//		sCANMsg551.Bits.TMU_THEMALSYSREQUEST = sEVOutData.ThemalSysRequest;
//		sCANMsg551.Bits.TMU_HEATCONFIRMATION = sEVOutData.HeatConfirmation;
//		sCANMsg551.Bits.TMU_COOLCONFIRMATION = sEVOutData.CoolConfirmation;
		sCANMsg551.Bits.TMU_PUMPENCONFIRMATION = sCANMsg424.Bits.PUMPEN;
		sCANMsg551.Bits.TMU_HEATCONFIRMATION = sVehicleControl.bBattHotReq;
		sCANMsg551.Bits.TMU_COOLCONFIRMATION = sVehicleControl.bBattCoolReq;
		if (sVehicleControl.bBattHotReq || sVehicleControl.bBattCoolReq)
		{
			sCANMsg551.Bits.TMU_THEMALSYSREQUEST = 1;
		}
		else
		{
			sCANMsg551.Bits.TMU_THEMALSYSREQUEST = 0;
		}

		sCANMsg551.Bits.TMS_ACTUATOR_STATUS = 0;// �ȹ���ϵͳ״̬  0-��������	1-������ϵͳ���ϡ�
		//��Ŀǰ��Ķ���Ϊ��ˮ��1~3��SOV1~2��RAD����״̬��EXV��FBEV����ͨ��1~2����ͨ����ѹ����״̬��WPTC״̬��
		if (sEVOutData.IncarFAN_H)
		{
			sCANMsg551.Bits.RAD_FAN_STATUS = 2;
		}
		else if (sEVOutData.IncarFAN_L)
		{
			sCANMsg551.Bits.RAD_FAN_STATUS = 1;
		}
		else
		{
			sCANMsg551.Bits.RAD_FAN_STATUS = 0;
		}

		sCANMsg551.Bits.AGS_A_STATUS = sActorData.AGS_AError;
		sCANMsg551.Bits.AGS_A_STATUS = sActorData.AGS_BError;
		sCANMsg551.Bits.SOV_12_NC_STATUS = sActorData.SOV1Error;
		sCANMsg551.Bits.SOV_6_NO_STATUS = sActorData.SOV2Error;
		sCANMsg551.Bits.TMU_4WAY_STATUS = sActorData.FourWayErr;
		sCANMsg551.Bits.TMU_4WAY_STATUS = sActorData.ThreeWay1Err;
		sCANMsg551.Bits.FBEV_STATUS = sActorData.BEXVError;
		sCANMsg551.Bits.EXV_STATUS = sActorData.EXVError;
		sCANMsg551.Bits.WCPTC_PUMPSTATUS_1 =sActorData.EWP3Error>>1; //
		sCANMsg551.Bits.WCPTC_PUMPSTATUS_2 =sActorData.EWP3Error & 0x01; //
		sCANMsg551.Bits.BATT_PUMPSTATUS = sActorData.EWP2Error;
		sCANMsg551.Bits.MOTOR_PUMPSTATUS = sActorData.EWP1Error;

		if (sActorData.ThreeWay2Err == 1)
		{
			sCANMsg453.Bits.TMU_3WAY2_RSP_COMM_ERR = 1;
		}
		else
		{
			sCANMsg453.Bits.TMU_3WAY2_RSP_COMM_ERR = 0;
		}
		sCANMsg453.Bits.TMU_3WAY2_VOLTAGEERR = l_u8_rd_LI0_BDC_2_voltageErr();
		sCANMsg453.Bits.TMU_3WAY2_FAULT_SIGNAL = l_u8_rd_LI0_BDC_2_Fault_Signal();
	//	sCANMsg453.Bits.TMU_3WAY1_FAULT_SIGNAL
		if (sActorData.ThreeWay1Err == 1)
		{
			sCANMsg453.Bits.TMU_3WAY1_RSP_COMM_ERR = 1;
		}
		else
		{
			sCANMsg453.Bits.TMU_3WAY1_RSP_COMM_ERR = 0;
		}
		sCANMsg453.Bits.TMU_3WAY1_VOLTAGEERR = l_u8_rd_LI0_BDC_1_voltageErr();
		sCANMsg453.Bits.TMU_3WAY1_FAULT_SIGNAL = l_u8_rd_LI0_BDC_1_Fault_Signal();
		if (EXVGetLostErr(EXV_NUM_FOURWAY))
		{
			sCANMsg453.Bits.TMU_4WAY_RSP_COMM_ERR = 1;
		}
		else
		{
			sCANMsg453.Bits.TMU_4WAY_RSP_COMM_ERR = 0;
		}
		sCANMsg453.Bits.TMU_4WAY_RSP_COMM_ERR = l_bool_rd_LI0_CCV4_Rsp_Comm_Err();
		sCANMsg453.Bits.TMU_4WAY_VOLTAGEERR_1 = l_u8_rd_LI0_CCV4_voltageErr() >> 1;
   	sCANMsg453.Bits.TMU_4WAY_VOLTAGEERR_2 = l_u8_rd_LI0_CCV4_voltageErr() & 0x01;
   	sCANMsg453.Bits.TMU_4WAY_FAULT_SIGNAL = l_u8_rd_LI0_CCV4_Fault_Signal();

   	sCANMsg453.Bits.FBEV_FAULTSTATE_1 = l_u8_rd_LI0_EXV_FaultState() >> 1;
   	sCANMsg453.Bits.FBEV_FAULTSTATE_2 = l_u8_rd_LI0_EXV_FaultState()& 0x01;
   	sCANMsg453.Bits.FBEV_RESPONSEERROR = l_bool_rd_LI0_EXV_ResponseError();
   	sCANMsg453.Bits.FBEV_VOLTAGESTATE = l_u8_rd_LI0_EXV_VoltageState();
   	sCANMsg453.Bits.AGS_A_ERR_ELECTRICAL = l_bool_rd_LI0_AGM_A_Err_Electrical();
   	sCANMsg453.Bits.AGS_A_ERR_LINRESP = l_bool_rd_LI0_AGM_A_Err_LINResp();
   	sCANMsg453.Bits.AGS_A_ERR_MECHBLOCK = l_bool_rd_LI0_AGM_A_Err_MechBlock();
   	sCANMsg453.Bits.AGS_A_ERR_MECHBREAK = l_bool_rd_LI0_AGM_A_Err_MechBreak();
   	sCANMsg453.Bits.AGS_A_ERR_OVERTEMP = l_bool_rd_LI0_AGM_A_Err_OverTemp();
   	sCANMsg453.Bits.AGS_A_ERR_VOLTAGE = l_bool_rd_LI0_AGM_A_Err_Voltage();
   	sCANMsg453.Bits.AGS_B_ERR_ELECTRICAL = l_bool_rd_LI0_AGM_B_Err_Electrical();
   	sCANMsg453.Bits.AGS_B_ERR_LINRESP = l_bool_rd_LI0_AGM_B_Err_LINResp();
   	sCANMsg453.Bits.AGS_B_ERR_MECHBLOCK = l_bool_rd_LI0_AGM_B_Err_MechBlock();
   	sCANMsg453.Bits.AGS_B_ERR_MECHBREAK = l_bool_rd_LI0_AGM_B_Err_MechBreak();
   	sCANMsg453.Bits.AGS_B_ERR_OVERTEMP = l_bool_rd_LI0_AGM_B_Err_OverTemp();
   	sCANMsg453.Bits.AGS_B_ERR_VOLTAGE = l_bool_rd_LI0_AGM_B_Err_Voltage();

   	sCANMsg453.Bits.EXV_FAULTSTATE = l_u8_rd_LI0_EXV2_FaultState();
   	sCANMsg453.Bits.EXV_RESPONSEERROR = l_bool_rd_LI0_EXV2_ResponseError();
   	sCANMsg453.Bits.EXV_VOLTAGESTATE = l_u8_rd_LI0_EXV2_VoltageState();
//454
   	sCANMsg454.Bits.TMU_3WAY1_POSITION_FB_1 = l_u8_rd_LI0_BDC_1_RespPos()>>3;
   	sCANMsg454.Bits.TMU_3WAY1_POSITION_FB_2 = l_u8_rd_LI0_BDC_1_RespPos()& 0X7;
   	sCANMsg454.Bits.TMU_3WAY2_POSITION_FB_1 = l_u8_rd_LI0_BDC_2_RespPos()>>3;
   	sCANMsg454.Bits.TMU_3WAY2_POSITION_FB_2 = l_u8_rd_LI0_BDC_2_RespPos()& 0X7;
   	sCANMsg454.Bits.TMU_4WAY_POSITION_FB = l_u8_rd_LI0_CCV4_RespPos();
   	sCANMsg454.Bits.EXV_PULSE_NUM_FB_1 = l_u16_rd_LI0_CurrentPosition_EXV2()>>9;
   	sCANMsg454.Bits.EXV_PULSE_NUM_FB_2 = (l_u16_rd_LI0_CurrentPosition_EXV2()>>1) & 0xFF;
   	sCANMsg454.Bits.EXV_PULSE_NUM_FB_3 = l_u16_rd_LI0_CurrentPosition_EXV2()&0x001;

   	sCANMsg454.Bits.FBEV_PULSE_NUM_FB_1 = l_u16_rd_LI0_EXV_CurrentPosition()>>3;
   	sCANMsg454.Bits.FBEV_PULSE_NUM_FB_2 = (l_u16_rd_LI0_CurrentPosition_EXV2()) & 0x7;
   	sCANMsg454.Bits.HVAC_AM_FB =(uint8)(Motor_GetPosition(MOTOR_ID_MIX) *20/ 51);
   	sCANMsg454.Bits.HVAC_BLW_LV_FB =SystemControl.fanMode;
   	sCANMsg454.Bits.HVAC_MODE_FB = SystemControl.mdMode;
   	sCANMsg454.Bits.HVAC_RFD_FB = (uint8)(Motor_GetPosition(MOTOR_ID_NCF) *20/ 51);
   	//455
   	sCANMsg455.Bits.AGS_POSITION_TGT = sEVOutData.AGS_Postion*15/100;
   	sCANMsg455.Bits.EXV_PULSE_NUMTGT_1 = sEVOutData.EXV_Postion >>6;
   	sCANMsg455.Bits.EXV_PULSE_NUMTGT_2 = sEVOutData.EXV_Postion &0x3f;
   	sCANMsg455.Bits.FBEV_PULSE_NUMTGT_1 = sEVOutData.BEXV_Postion >>8;
   	sCANMsg455.Bits.FBEV_PULSE_NUMTGT_2 = sEVOutData.BEXV_Postion;
   	sCANMsg455.Bits.T_OHX_OUTLET_1 =(uint8) (sEVSensorData.TPL2_Temp/10 +40) >> 4;     //����Ǹ�ѹ���
     	sCANMsg455.Bits.T_OHX_OUTLET_2 =(uint8) (sEVSensorData.TPL2_Temp/10 +40);
   	sCANMsg455.Bits.P_OHX_OUTLET_1 =(sEVSensorData.TPL2_Press) >> 4;     //����Ǹ�ѹ���
     	sCANMsg455.Bits.P_OHX_OUTLET_2 = (sEVSensorData.TPL2_Press);
     	sCANMsg455.Bits.TMU_3WAY1_POSITION_TGT = sEVOutData.ThreeWay1_Postion/5;
   	sCANMsg455.Bits.TMU_3WAY2_POSITION_TGT = sEVOutData.ThreeWay2_Postion/5;
   	sCANMsg455.Bits.TMU_4WAY_POSITION_TGT = sEVOutData.FourWay_Postion;
      //456
   	sCANMsg456.Bits.DUTY_BATT_PUMPTGT = sEVOutData.EWPT2;
   	sCANMsg456.Bits.DUTY_MOTOR_PUMPTGT = sEVOutData.EWPT1;
   	sCANMsg456.Bits.DUTY_WCPTC_PUMPTGT = sEVOutData.EWPT3;
   	sCANMsg456.Bits.T_EAC_DIS_1  = (sEVSensorData.TPH_Temp/10+40)>>4;    //ѹ����������PT�������¶�ֵ
   	sCANMsg456.Bits.T_EAC_DIS_2  = sEVSensorData.TPH_Temp/10+40;
   	sCANMsg456.Bits.P_EAC_DIS_1  = sEVSensorData.TPH_Press >>4;
   	sCANMsg456.Bits.P_EAC_DIS_2  = sEVSensorData.TPH_Press;
   	sCANMsg456.Bits.T_EAC_SUC  = (sEVSensorData.TPL1_Temp/10+40);    //ѹ����������PT�������¶�ֵ
   	sCANMsg456.Bits.P_EAC_SUC_1  = sEVSensorData.TPL1_Press >>8;
   	sCANMsg456.Bits.P_EAC_SUC_2  = sEVSensorData.TPL1_Press;
//457
   	sCANMsg457.Bits.DR_MODE = SystemControl.u8TestLevel;
   	sCANMsg457.Bits.SOV_12_NC_TGT = sEVOutData.SOV1;
   	sCANMsg457.Bits.SOV_6_NO_TGT = sEVOutData.SOV2;
   	if (sEVOutData.EACSpeed > 0)
   	{
   		sCANMsg457.Bits.AC_ON_OFF = 1;
   	}
   	else
   	{
   		sCANMsg457.Bits.AC_ON_OFF = 0;
   	}

   	if (SystemControl.mdMode == MD_MODE_OST)
   	{
   		sCANMsg457.Bits.FR_DEF = 1;
   	}
   	else
   	{
   		sCANMsg457.Bits.FR_DEF = 0;
   	}
   	sCANMsg457.Bits.HVAC_TEO = sEVSensorData.TevpTarge/10 + 30;
   	sCANMsg457.Bits.HVAC_TEVA	= sSenSorData.TevpCal/10 +30;

		if (sEVOutData.IncarFAN_H)
		{
			sCANMsg457.Bits.RAD_FAN_LV_TGT = 2;
		}
		else if (sEVOutData.IncarFAN_L)
		{
			sCANMsg457.Bits.RAD_FAN_LV_TGT = 1;
		}
		else
		{
			sCANMsg457.Bits.RAD_FAN_LV_TGT = 0;
		}

		sCANMsg457.Bits.RR_DEF = SystemControl.RdefMode;
		sCANMsg457.Bits.TWO_WPTC_OUTLET = sEVSensorData.TwaterTarge/10 + 40;
		if (sEVOutData.PTCPower > 0)
		{
			sCANMsg457.Bits.WPTC_ON_OFF = 1;
		}
		else
		{
			sCANMsg457.Bits.WPTC_ON_OFF = 0;
		}

		sCANMsg410.Bits.COMPSPDREQ_1 = sEVOutData.EACSpeed >> 8;
		sCANMsg410.Bits.COMPSPDREQ_2 = sEVOutData.EACSpeed ;
		sCANMsg410.Bits.VCU_AC_EAC_POWER_LIMIT_1 = sVehicleControl.i16PowerLimte >> 8;
		sCANMsg410.Bits.VCU_AC_EAC_POWER_LIMIT_2 = sVehicleControl.i16PowerLimte;
		if ( sEVOutData.EACSpeed > 0)
		{
			sCANMsg410.Bits.KEYSTART = 2;
		}
		else
		{
			sCANMsg410.Bits.KEYSTART = 3;
		}
		sCANMsg44E.Bits.CHECKSUM = 0;
		sCANMsg44E.Bits.LIVECOUNTER = 0;
//		sCANMsg44E.Bits.TWO_WPTC_OUTLET_1 = 0;
//		sCANMsg44E.Bits.TWO_WPTC_OUTLET_2 = 0;
//
//		sCANMsg44E.Bits.WPTC_POWER_LIMIT_1 = sEVOutData.PTCPower >> 8;
//		sCANMsg44E.Bits.WPTC_POWER_LIMIT_2 = sEVOutData.PTCPower;
		if (sEVOutData.PTCPower > 0)
		{
			sCANMsg44E.Bits.WPTC_OPE_PERMIT = 1;
		}
		else
		{
			sCANMsg44E.Bits.WPTC_OPE_PERMIT = 0;
		}
		//sCANMsg44E.Bits.WPTC_REQ_1 = 0;
		//sCANMsg44E.Bits.WPTC_REQ_2 = 0;
		sCANMsg44E.Bits.TWO_WPTC_OUTLET_1 = (sEVSensorData.TwaterTarge/10 +40)>>8;
		sCANMsg44E.Bits.TWO_WPTC_OUTLET_2 = (sEVSensorData.TwaterTarge/10 +40);
		sCANMsg44E.Bits.WPTC_POWER_LIMIT_1 = sVehicleControl.i16PowerLimte >> 8;
		sCANMsg44E.Bits.WPTC_POWER_LIMIT_2 = sVehicleControl.i16PowerLimte ;
		sCANMsg44E.Bits.WPTC_REQ_1 = sEVOutData.PTCPower >> 8;
		sCANMsg44E.Bits.WPTC_REQ_2 = sEVOutData.PTCPower;
//�����뿴131 �� ͨ���б�
		//bcan
		//sCANMsg12F85351.Bits.C_FRDEF = SystemControl.RdefMode;
		extern int16 AD3KValue;
		sCANMsg12F85351.Bits.C_OUTTEMP10BIT_1 = AD3KValue >> 2;
		sCANMsg12F85351.Bits.C_OUTTEMP10BIT_2 = AD3KValue&0x03;
		sCANMsg12F85351.Bits.C_RRDEF = SystemControl.RdefMode;

		if (SystemControl.mdMode == MD_MODE_OST)
		{
			sCANMsg12F85351.Bits.C_FRDEF = 1;
		}
		else
		{
			sCANMsg12F85351.Bits.C_FRDEF = 0;
		}


		sCANMsg12F85351.Bits.INCAR_TEMP = sSenSorData.TinCal/10 +30;
		sCANMsg12F98851.Bits.C_T_FR_BLW = FanFBVoltGet()/100;
	}
	else{}
	
	
	//ʵ�ֻػ�ģʽ���л��������ͼ�ģ��
}
