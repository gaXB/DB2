/*
 * EXV_IF.c
 *
 *  Created on: 2021Äê12ÔÂ29ÈÕ
 *      Author: Administrator
 */
#include "LINEXV.h"
#include "lin_cfg.h"
#include "lin.h"

uint8 EXVIF_GetRecFlag(EXVDATA* pEXV, uint8 num)
{
	uint8 bGetComm = 0;

	if (num == EXV_NUM_EXV)
	{
		if (l_flg_tst_LI0_EXV_2_flag())
		{
			l_flg_clr_LI0_EXV_2_flag();


			pEXV->InitRet = l_u8_rd_LI0_CurrentInitState_EXV2();
			pEXV->u16RetPostion = l_u16_rd_LI0_CurrentPosition_EXV2();
			bGetComm = 1;
		}else{}
		l_u16_wr_LI0_PositionRequest_EXV2(pEXV->u16ComPostion);
		l_bool_wr_LI0_EnableRequest_EXV2(pEXV->bEnable);
		l_u8_wr_LI0_InitRequest_EXV2(pEXV->InitReq);

	}
	else if (num == EXV_NUM_BEXV)
	{
		if (l_flg_tst_LI0_EXV_VCU_flag())
		{
			l_flg_clr_LI0_EXV_VCU_flag();

			pEXV->InitRet = l_u8_rd_LI0_EXV_CurrentInitState();
			pEXV->u16RetPostion = l_u16_rd_LI0_EXV_CurrentPosition();
			bGetComm = 1;
		}else{}

		l_u16_wr_LI0_TMM_EXV_PositionRequest(pEXV->u16ComPostion);
		l_bool_wr_LI0_TMM_EXV_EnableRequest(pEXV->bEnable);
		l_u8_wr_LI0_TMM_EXV_initRequest(pEXV->InitReq);
	}
	else if (num == EXV_NUM_THREEWAY1)
	{
		if (l_flg_tst_LI0_BDC_1_Rsp_flag())
		{
			l_flg_clr_LI0_BDC_1_Rsp_flag();

			pEXV->u16RetPostion = l_u8_rd_LI0_BDC_1_RespPos();
			bGetComm = 1;
		}else{}

		pEXV->InitRet = 1;
		l_u8_wr_LI0_BDC_1_ReqMovePos(pEXV->u16ComPostion);
		l_bool_wr_LI0_BDC_1_MoveEnable(pEXV->bEnable);
	//	l_u8_wr_LI0_TMM_EXV_initRequest(pEXV->InitReq);

	}
	else if (num == EXV_NUM_THREEWAY2)
	{
		if (l_flg_tst_LI0_BDC_2_Rsp_flag())
		{
			l_flg_clr_LI0_BDC_2_Rsp_flag();

			pEXV->u16RetPostion = l_u8_rd_LI0_BDC_2_RespPos();
			bGetComm = 1;
		}else{}

		pEXV->InitRet = 1;
		l_u8_wr_LI0_BDC_2_ReqMovePos(pEXV->u16ComPostion);
		l_bool_wr_LI0_BDC_2_MoveEnable(pEXV->bEnable);
	//	l_u8_wr_LI0_TMM_EXV_initRequest(pEXV->InitReq);
	}
	else if (num == EXV_NUM_FOURWAY)
	{
		if (l_flg_tst_LI0_CCV4_Rsq_flag())
		{
			l_flg_clr_LI0_CCV4_Rsq_flag();


			pEXV->u16RetPostion = l_u8_rd_LI0_CCV4_RespPos();
			bGetComm = 1;
		}else{}

		pEXV->InitRet = 1;
		l_bool_wr_LI0_CCV4_ReqMovePos(pEXV->u16ComPostion);
		l_bool_wr_LI0_CCV4_MoveEnable(pEXV->bEnable);
	//	l_u8_wr_LI0_TMM_EXV_initRequest(pEXV->InitReq);

	}
	else if (num == EXV_NUM_AGS_A)
	{
		if (l_flg_tst_LI0_AGM_A_ECM_STATUS_flag())
		{
			l_flg_clr_LI0_AGM_A_ECM_STATUS_flag();


			pEXV->InitRet = l_bool_rd_LI0_AGM_A_CalibRequired();
			pEXV->u16RetPostion = l_u8_rd_LI0_AGM_A_ActualPosition();
			bGetComm = 1;
		}else{}


		l_u8_wr_LI0_ECM_TargetPosReqIgOn_A(0);
		l_u8_wr_LI0_ECM_TargetPosReqIgOff_A(15);
		if (pEXV->u16ComPostion > 0)
		{
			l_bool_wr_LI0_ECM_IgStatusReq_A(0);
		}
		else
		{
			l_bool_wr_LI0_ECM_IgStatusReq_A(1);
		}
		l_bool_wr_LI0_ECM_CalibReq_A(1);
	}
	else if (num == EXV_NUM_AGS_B)
	{
		if (l_flg_tst_LI0_AGM_B_ECM_STATUS_flag())
		{
			l_flg_clr_LI0_AGM_B_ECM_STATUS_flag();

			pEXV->InitRet = l_bool_rd_LI0_AGM_B_CalibRequired();
			pEXV->u16RetPostion = l_u8_rd_LI0_AGM_B_ActualPosition();
			bGetComm = 1;
		}else{}


		l_u8_wr_LI0_ECM_TargetPosReqIgOn_B(0);
		l_u8_wr_LI0_ECM_TargetPosReqIgOff_B(15);

		l_bool_wr_LI0_ECM_CalibReq_B(1);
		if (pEXV->u16ComPostion > 0)
		{
			l_bool_wr_LI0_ECM_IgStatusReq_B(0);
		}
		else
		{
			l_bool_wr_LI0_ECM_IgStatusReq_B(1);
		}
	}
	else{}

	return bGetComm;
}
