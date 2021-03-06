// Run_UldPicker.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_UldPicker.h"
#include "AMTLotManager.h"
#include "FastechPublic_IO.h"
#include "IO_Manager.h"
#include "CmmsdkDef.h"
#include "LogFromat.h"
#include "Run_Device_Carrier_Robot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_UldPicker
CRun_UldPicker Run_UldPicker;
IMPLEMENT_SERIAL(CRun_UldPicker, CObject, 1);

CRun_UldPicker::CRun_UldPicker()
{
	m_nPickerPara = PICKER_PARA;//1
	m_nRobotSite  = THD_UNLOAD_WORK_RBT;

	m_nLinearMove_Index	= 0 ;	//현재 map index
	m_lAxisCnt	= 2;				 //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개
	m_lpAxisNum[0] = M_UNLOADER_TRANSFER_X; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다
	m_lpAxisNum[1] = M_UNLOADER_TRANSFER_Y;
	m_lpAxisNum[2] = M_UNLOADER_TRANSFER_Z;
	
	m_nRobot_X = M_UNLOADER_TRANSFER_X;
	m_nRobot_Y = M_UNLOADER_TRANSFER_Y;
	m_nRobot_Z = M_UNLOADER_TRANSFER_Z; 

	COMI.ml_axiscnt[m_nLinearMove_Index] = 2;
	COMI.mp_axisnum[m_nLinearMove_Index][0] = M_UNLOADER_TRANSFER_X;
	COMI.mp_axisnum[m_nLinearMove_Index][1] = M_UNLOADER_TRANSFER_Y;
	COMI.mp_axisnum[m_nLinearMove_Index][2] = M_UNLOADER_TRANSFER_Z;


	m_dwCycleTime[0][0] = 0;
	m_dwCycleTime[1][0] = 0;
	m_strLotNo[0] = m_strPartNo[0] = _T("");
	m_strLotNo[1] = m_strPartNo[1] = _T("");

	mn_RunStep = 0;
	mn_RunSafetyStep = 0;
	mn_InitStep = 0;
	mn_Pick_Step = 0;
	mn_Place_Step = 0;
	mn_reinstate_step = 0;
	m_bConnectAssemblyTimeOut = FALSE;

}

CRun_UldPicker::~CRun_UldPicker()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_UldPicker message handlers
void CRun_UldPicker::Thread_Run()
{
	int nRet_1;
	switch( st_work.mn_run_status)
	{
		case dINIT:
			RunInit();
			break;

		case dRUN:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_case_assembler_status, IO_ON);
			if( nRet_1 != IO_ON ) break;

			RunMove();
			break;

		//kwlee 2017.0629			
// 		case dSTOP:
// // 			if(m_nRobot_Z_Stop_Flag == 0)
// // 			{
// 				nRet_1 = COMI.Check_Motion_State(m_nRobot_Z, cmMST_STOP);//20150427 nRet_1 = cmmSxIsDone(n_MotorNum, &dwMotionDone);
// 				if (nRet_1 != BD_GOOD) 
// 				{//모터 상태가 mMST_STOP 이다 
// 					if(CTL_Lib.mn_single_motmove_step[m_nRobot_Z] > 0)
// 					{
// 						COMI.Set_MotStop(1, m_nRobot_Z) ; //긴급정지 
// 						CTL_Lib.mn_single_motmove_step[m_nRobot_Z] = 0;
// 						m_nRobot_Z_Stop_Flag = 100; 
// 					}
// 					else
// 					{
// 						m_nRobot_Z_Stop_Flag = 1;
// 					}					
// 				}
// 				else
// 				{
// 					m_nRobot_Z_Stop_Flag = 1;
// 				}
// 
// 				//CTL_Lib.mn_single_motmove_step[m_nRobot_Z] = 0;
// // 			}
// 
// 			break;
// 
// 		case dWARNING:
// 			break;

		//kwlee 2017.0628
		case dREINSTATE:
			Run_ReinState();
			break;

		default:		
			if (st_work.mn_run_status == dSTOP)
			{
				nRet_1 = COMI.Check_Motion_State(m_nRobot_Z, cmMST_STOP);//20150427 nRet_1 = cmmSxIsDone(n_MotorNum, &dwMotionDone);
				if (nRet_1 != BD_GOOD) 
				{//모터 상태가 mMST_STOP 이다 
					if(CTL_Lib.mn_single_motmove_step[m_nRobot_Z] > 0)
					{
						COMI.Set_MotStop(1, m_nRobot_Z) ; //긴급정지 
						CTL_Lib.mn_single_motmove_step[m_nRobot_Z] = 0;
						m_nRobot_Z_Stop_Flag = 100; 
					}
					else
					{
						m_nRobot_Z_Stop_Flag = 1;
					}					
				}
				else
				{
					m_nRobot_Z_Stop_Flag = 1;
				}
			}
			//kwlee 2017.0626
			if (st_work.mn_run_status != dRUN || st_work.mn_run_status != dREINSTATE)
			{
				OnUldPicker_FinalPos();
			}
			
			if (st_handler.mn_menu_num == 501) return;
			
			COMI.Set_MotStop(0, M_UNLOADER_TRANSFER_X);
			COMI.Set_MotStop(0, M_UNLOADER_TRANSFER_Y);
			COMI.Set_MotStop(0, M_UNLOADER_TRANSFER_Z);
			

			CTL_Lib.mn_single_motmove_step[M_UNLOADER_TRANSFER_X] = 0;
			CTL_Lib.mn_single_motmove_step[M_UNLOADER_TRANSFER_Y] = 0;
			CTL_Lib.mn_single_motmove_step[M_UNLOADER_TRANSFER_Z] = 0;
			mn_InitStep = 0;
			mn_reinstate_step = 0;
			break;
	}
}

void CRun_UldPicker::RunInit()
{
	int i = 0,nRet_1=0, nSNum = 0;
	
// 	if( st_handler.mn_init_state[INIT_UNPRESS_ROBOT] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_LD_ROBOT] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_ULD_ROBOT] != CTL_NO ) return;
// 	st_handler.mn_init_state[INIT_ULD_ROBOT] = CTL_YES;
	switch(mn_InitStep)
	{
	case 0:
		mn_InitStep = 100;
		break;

	case 100:
		nRet_1 = COMI.HomeCheck_Mot( m_nRobot_Z, st_motor[m_nRobot_Z].mn_homecheck_method, MOTTIMEOUT);
		if( nRet_1 == BD_GOOD)
		{
			mn_InitStep = 200;
		}
		else if( nRet_1 == BD_ERROR)
		{
			mn_InitStep = 900;
			CTL_Lib.Alarm_Error_Occurrence( 4001, dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 200:
		nRet_1 = g_ioMgr.get_in_bit( st_io.i_Unloader_Transfer_Device_Check, IO_OFF);
		if( nRet_1 == IO_ON )
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Unloader_Transfer_Device_Check);
			CTL_Lib.Alarm_Error_Occurrence(4002, dWARNING, m_strAlarmCode);
		}
		else
		{
			mn_InitStep = 300;
		}
		break;

	case 300:
		Set_UnLoader_Transfer_Clamp_OnOff( IO_OFF );
		mn_InitStep = 400;
		break;

	case 400:
		nRet_1 = Chk_UnLoader_Transfer_Clamp_OnOff( IO_OFF );
		if( nRet_1 == RET_GOOD )
		{
			mn_InitStep = 500;
		}
		else if(nRet_1 == RET_ERROR)
		{
			mn_InitStep = 900;
			CTL_Lib.Alarm_Error_Occurrence(4003, dWARNING, m_strAlarmCode);
		}
		break;

	case 500:
		nRet_1 = COMI.HomeCheck_Mot( m_nRobot_X, st_motor[m_nRobot_X].mn_homecheck_method, MOTTIMEOUT);
		if( nRet_1 == BD_GOOD)
		{
			mn_InitStep = 600;
		}
		else if( nRet_1 == BD_ERROR)
		{
			mn_InitStep = 900;
			CTL_Lib.Alarm_Error_Occurrence( 4004, dWARNING, COMI.mc_alarmcode);
		}
		break;

	case 600:
		nRet_1 = COMI.HomeCheck_Mot( m_nRobot_X, st_motor[m_nRobot_X].mn_homecheck_method, MOTTIMEOUT);
		if( nRet_1 == BD_GOOD)
		{
			mn_InitStep = 700;
		}
		else if( nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence( 4005, dWARNING, COMI.mc_alarmcode);
			mn_InitStep = 900;
		}
		break;

	case 700:
		nRet_1 = COMI.HomeCheck_Mot( m_nRobot_Y, st_motor[m_nRobot_Y].mn_homecheck_method, MOTTIMEOUT);
		if( nRet_1 == BD_GOOD)
		{
			mn_InitStep = 800;
		}
		else if( nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence( 4006, dWARNING, COMI.mc_alarmcode);
			mn_InitStep = 900;
		}
		break;

	case 800:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_InitStep = 810;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_InitStep = 800;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4007, dWARNING, alarm.mstr_code);
			mn_InitStep = 900;
		}
		break;

	case 810:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_InitStep = 1000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_InitStep = 810;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4008, dWARNING, alarm.mstr_code);
			mn_InitStep = 900;
		}
		break;


	case 900:
		mn_InitStep = 0;
		st_handler.mn_initial_error = TRUE;
		break;

	case 1000:
		st_handler.mn_init_state[INIT_ULD_ROBOT] = CTL_YES;
		mn_InitStep = 0;
		break;

	}
}

void CRun_UldPicker::RunMove()
{
	int nRet_1 = 0, nLotNum = 0, nTotal = 0;
	long lMotionDone=0;

	double m_MotCurrPos = COMI.Get_MotCurrentPos( m_nRobot_X );

	Func.ThreadFunctionStepTrace(30, mn_RunStep);
	switch(mn_RunStep)
	{
		case -1:
			mn_RunStep = 0;
			break;

		case 0:
			mn_RunStep = 100;

		case 100:
			if( COMI.Get_MotCurrentPos( m_nRobot_Z ) > (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) )
			{
				mn_RunStep = 110;
			}
			else
			{
				mn_RunStep = 200;
			}
			break;

		case 110:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 110;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4100, dWARNING, alarm.mstr_code);
				mn_RunStep = 110;
			}
			break;
			
		case 200:
			if( Move_MakeSafetyPosBeforeWork( POS_MOV_READY ) == RET_GOOD )
			{
				mn_RunStep = 1000;
			}
			break;

		case 1000:
			if( g_lotMgr.GetLotCount() > 0 )
			{
				mn_RunStep = 1100;
			}
			break;

		case 1100:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][0] == CTL_REQ &&
				st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][1] == WORK_PICK )
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][0] = CTL_READY;
				mn_RunStep = 1200;
			}
			break;

		case 1200:
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y )  > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_LOADER_TRANSFER_Y] ) )
			{
				break;
			}
// 			if( st_handler.mn_lduld_safety != LD_MOVING_SAFETY )
// 			{
// 				st_sync.nLdUldSateyflag = ULD_MOVING_SAFETY;
// 				mn_RunStep = 1300;
// 			}
			//2017.0614
			//////////////////////////////////////////////////////////////
			if( st_handler.mn_lduld_safety == LD_MOVING_SAFETY)
			{
				break;
			}
			st_handler.mn_lduld_safety = ULD_MOVING_SAFETY;
			st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][5] = -1;			
			//////////////////////////////////////////////////////////////
			mn_RunStep = 1300;
			break;

		case 1300:
			//signal to move transfer 
			if( st_handler.n_CaseAssemblyUnloadingAbleOn != IO_ON ) break;

			nRet_1 = Process_DVC_Pickup( 0, THD_LDULD_CARRIER_BUFF, g_lotMgr.GetLotAt(0).GetLotID() );
			if(nRet_1 == RET_GOOD)
			{
				mn_RunStep = 2000;
			}
			else if(nRet_1 == RET_PICKER_NOT_FIND)
			{
				mn_RunStep = 1300;			
			}
			else if(nRet_1 == RET_TRAY_NOT_FIND)
			{
				mn_RunStep = 1300;  
			}
			break;

		case 2000:
			//signal to move transfer 
			if( st_handler.n_CaseAssemblyUnloadingAbleOn != IO_ON ) break;

			if( Move_MakeSafetyPosBeforeWork( POS_MOV_PLACE ) == RET_GOOD )
			{
				m_bConnectAssemblyTimeOut = FALSE;

// 				if( ( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[0] == CTL_YES && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[0] == BIN_GOOD ) ||
// 					( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[1] == CTL_YES && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[1] == BIN_GOOD ) ||
// 					( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[2] == CTL_YES && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[2] == BIN_GOOD ))
// 				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[0] == CTL_YES ||
// 					st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[1] == CTL_YES ||
// 					st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[2] == CTL_YES )
// 				{
// 					//2017.0614
// 					if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[0] == CTL_NO && st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][5] == 0 )
// 					{
// 						st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][2+0] = CTL_CHANGE;
// 					}
// 					else if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[1] == CTL_NO && st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][5] == 1 )
// 					{
// 						st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][2+1] = CTL_CHANGE;
// 					}
// 					else if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[2] == CTL_NO && st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][5] == 2)
// 					{
// 						st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][2+2] = CTL_CHANGE;
// 					}
// 					st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][5] = -1;
// 					////////////////////////////////////////////////////////////////////////////////////////
// 					
// 				}


				mn_RunStep = 2100;
			}
			break;

		case 2100:

			if( m_nRobot_X > (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[m_nRobot_X]) )
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_RunSafetyStep = 2000;
			}
// 			if( st_handler.mn_lduld_safety != LD_MOVING_SAFETY )
// 			{
// 				if( st_sync.nLdUldSateyflag != MOVING_NOT_SAFETY)
// 					st_sync.nLdUldSateyflag = MOVING_NOT_SAFETY;
// 			}
			//2017.0614
			//////////////////////////////////////////////////////////////
			if( st_handler.mn_lduld_safety == ULD_MOVING_SAFETY)
			{
				st_handler.mn_lduld_safety = MOVING_NOT_SAFETY;
			}
			//////////////////////////////////////////////////////////////

			//signal to move transfer 
			if( st_handler.n_CaseAssemblyUnloadingAbleOn != IO_ON ) break;

			//signal to place DVC
			if( st_handler.n_HSLowerPlaceAbleCheck == CTL_NO ) 
			{
				 if( mn_Place_Step >= 2000 && st_handler.m_nAssemblyAble == CTL_NO)
				 {
				 }
				 else
				 {
					 if( m_bConnectAssemblyTimeOut == FALSE)
					 {
						 m_bConnectAssemblyTimeOut = TRUE;
						 m_dwWaitTime[0] = GetCurrentTime();
					 }
					 else
					 {
						 m_dwWaitTime[1] = GetCurrentTime();
						 m_dwWaitTime[2] = m_dwWaitTime[1] - m_dwWaitTime[0];
						 if( m_dwWaitTime[2] <= 0 ) m_dwWaitTime[0] = GetCurrentTime();
						 if( m_dwWaitTime[2] > 60000)//40초
						 {
							 m_bConnectAssemblyTimeOut = FALSE;
							 //993000 0 A "Case Assembly장비에서 동작 가능상태(PS2102)가 아닙니다. 랏이 전달됐나요?"
							 CTL_Lib.Alarm_Error_Occurrence(4499, dWARNING, "993000" );
						 }
					 }
					 break;
				 }
			}

			if( st_work.n_DataYes[0] == "YES")
			{
				break;
			}

			//2017.0613
// 			st_handler.m_nAssemblyAble = CTL_NO;

			nRet_1 = Process_DVC_Place( 0, THD_ULD_ALIGN_BUFF, g_lotMgr.GetLotAt(0).GetLotID() );
			if(nRet_1 == RET_GOOD)
			{
				st_buffer_info[THD_ULD_ALIGN_BUFF].st_pcb_info[0].nYesNo = CTL_NO;
				mn_RunStep = 3000;
			}
			else if(nRet_1 == RET_PICKER_NOT_FIND)
			{
				mn_RunStep = 2100;			
			}
			else if(nRet_1 == RET_TRAY_NOT_FIND)
			{
				mn_RunStep = 2100;  
			}
			break;

		case 3000:
			if( Move_MakeSafetyPosBeforeWork( POS_MOV_READY ) == RET_GOOD )
			{
				st_work.n_DataYes[0] = "YES";
				st_handler.m_nAssemblyAble = CTL_YES;
				mn_RunStep = 3100;
			}
			break;

		case 3100:
// 			if( ( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[0] == CTL_YES && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[0] == BIN_GOOD ) ||
// 				( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[1] == CTL_YES && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[1] == BIN_GOOD ) ||
// 				( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[2] == CTL_YES && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[2] == BIN_GOOD ))
			if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[0] == CTL_YES ||
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[1] == CTL_YES ||
				st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[2] == CTL_YES )
			{
// 				//2017.0614
// 				if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[0] == CTL_NO && st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][5] == 0 )
// 				{
// 					st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][2+0] = CTL_CHANGE;
// 				}
// 				else if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[1] == CTL_NO && st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][5] == 1 )
// 				{
// 					st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][2+1] = CTL_CHANGE;
// 				}
// 				else if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[2] == CTL_NO && st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][5] == 2)
// 				{
// 					st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][2+2] = CTL_CHANGE;
// 				}
// 				st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][5] = -1;
// 				////////////////////////////////////////////////////////////////////////////////////////

				mn_RunStep = 1200;
			}
			else
			{
				//2017.0616
				memset(&st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE], 0x00, sizeof(st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE]));
				st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][0] = CTL_CHANGE;
				st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][1] = WORK_PICK;
				mn_RunStep = 3200;
			}
			break;
			
			//2017.0421
		case 3200:
			nLotNum = g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) + g_lotMgr.GetLotAt(0).GetFailCnt(PRIME);
			nTotal = g_lotMgr.GetLotAt(0).GetTotLotCount();
			if(g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES" && ( nLotNum >= nTotal ) )
			{
				if( st_sync.nLotEndFlag[0][THD_LOAD_WORK_RBT] == LOTEND )
				{
					st_sync.nLotEndFlag[0][THD_UNLOAD_WORK_RBT] = LOTEND;
					::PostMessage(st_handler.hWnd, WM_MAIN_EVENT, CTL_YES, 1);
				}
				mn_RunStep = 0;
			}
			else
			{
				mn_RunStep = 0;
			}
			break;

		default:
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_UldPicker  = %d", mn_RunStep);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			break;
		}
}

//nWorkSite: THD_LDULD_CARRIER_BUFF
int CRun_UldPicker::Process_DVC_Pickup( int nMode, int nWorkSite, CString strLotNo )
{
	int nFuncRet = RET_PROCEED;
	int i = 0, nRet_1, nRet_2, nRet_3, nRet_4, nRet_5, nFlag = 0;

	CString strMsg;
	CString strLogKey[10];
	CString strLogData[10];

	Func.ThreadFunctionStepTrace(31, mn_Pick_Step);

	switch( mn_Pick_Step )
	{
		case 0:
			m_bDvcWaitChk_Falg =  false;
			m_nPickRetry_Cnt = 0;
			mn_Pick_Step = 100;
			break;

		case 100:
			m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if( m_dCurrentPos[m_nRobot_Z] > ( st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) ) //안전 위치를 벗어나 있으면 저위치 시킨다
			{
				mn_Pick_Step = 900;
			}
			else
			{
				mn_Pick_Step = 1000;
			}
			break;

		case 900:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_bDvcWaitChk_Falg =  false;
				mn_Pick_Step = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 900;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4101, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 900;
			}
			break;

			//////////////////////////////////////////////////////
			//피커 상태를 확인 후 문제가 없으면 작업
			//////////////////////////////////////////////////////
		case 1000:
			m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if( m_dCurrentPos[m_nRobot_Z] > ( st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z] ) ) //안전 위치를 벗어나 있으면 저위치 시킨다
			{
				mn_Pick_Step = 900;
				break;
			}

			nRet_1 = Func.Check_PickerStatus(0, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
			if(nRet_1 == RET_GOOD) //전역 변수에 따른 피커 상태를 체크 
			{			//피커 정보와 현재 상태가 정상이다
			}
			else// if(nRet_1 == RET_ERROR)xx
			{
				if(m_bDvcWaitChk_Falg == false)
				{
					m_bDvcWaitChk_Falg =  true;
					m_dwWaitUntil[0] = GetCurrentTime();
					break;
				}
				else
				{
					m_dwWaitUntil[1] = GetCurrentTime();
					m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
					if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
					CTL_Lib.Alarm_Error_Occurrence(4102, dWARNING, Func.m_strAlarmCode);
					m_bDvcWaitChk_Falg = false;
					break;
				}
			}

			nFlag = 0;
			m_n_FirstTray_X_Num = -1; m_n_FirstTray_Y_Num = -1; m_n_FirstPicker_Num = -1; //flag clear
			for(i = 0; i < m_nPickerPara; i++)
			{
				if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_NO)
				{
					m_n_FirstPicker_Num = i;
					i = m_nPickerPara;
				}
			}

			if( m_n_FirstPicker_Num == -1 )
			{
				mn_Pick_Step = 0;
				nFuncRet = RET_PICKER_NOT_FIND;
				break;
			}

			nRet_1 = Func.Find_TrayWork_Pos(WORK_PICK, nWorkSite, CTL_YES, m_npFindWorkPosYXCPB, THD_UNLOAD_WORK_RBT, strLotNo);
			if( nRet_1 == RET_GOOD )
			{
				m_n_FirstTray_X_Num = m_npFindWorkPosYXCPB[0];
				m_n_FirstTray_Y_Num = m_npFindWorkPosYXCPB[1];
			}
			else if( nRet_1 == RET_SKIP )//no device
			{
				mn_Pick_Step = 0;
				nFuncRet = RET_TRAY_NOT_FIND;
				break;
			}

			nFlag = 0;
			if( m_n_FirstPicker_Num != -1 && m_n_FirstTray_Y_Num != -1 && m_n_FirstTray_X_Num != -1 )
			{
				for( i = 0; i < m_nPickerPara; i++ )
				{
					m_npPicker_YesNo[i] = CTL_NO;
					if( i < m_n_FirstPicker_Num ) continue;

					if( nWorkSite == THD_LDULD_CARRIER_BUFF )
					{
						if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_NO)
						{
							if(i >= 0 && m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num) < st_recipe.nCarrierBuffer_Num)
							{
								if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[ m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num) ] == CTL_YES &&
									st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[ m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num) ] != BIN_CDIMM ) //버퍼에 자재가 있으면 픽업가능하게 한다
								{
									m_npPicker_YesNo[i] = CTL_YES;
									nFlag++;
									st_picker[m_nRobotSite].st_pcb_info[i].nStatusChang = NO;
								}
							}
						}
					}
					else
					{
						if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
						{
							sprintf(st_msg.c_abnormal_msg, "There is no WorkSite in Process_DVC_Pickup. Run_UldPicker = %d", mn_Pick_Step);
							st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
							//st_work.mn_run_status = dSTOP;
						}
						break;
					}
				}
			}

			if( nFlag == 0 )
			{
				mn_Pick_Step = 0;
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "RET_PICKER_NOT_FIND in Process_DVC_Pickup. Run_UldPicker = %d", mn_Pick_Step);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					//st_work.mn_run_status = dSTOP;
				}
				nFuncRet = RET_PICKER_NOT_FIND;
				break;
			}

			m_npFindWorkPosYXCPB[0] = m_n_FirstTray_X_Num;
			m_npFindWorkPosYXCPB[1] = m_n_FirstTray_Y_Num;
			m_npFindWorkPosYXCPB[3] = m_n_FirstPicker_Num;

			m_nMove_Flag[0] = CTL_NO; m_nMove_Flag[1] = CTL_NO;

			if( nWorkSite == THD_LDULD_CARRIER_BUFF )
			{
				mn_Pick_Step = 1500;
			}
			break;
			

		case 1500:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				Set_UnLoader_Transfer_Clamp_OnOff( IO_OFF );
				mn_Pick_Step = 1600;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 1500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4103, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 1500;
			}
			break;

		case 1600:
			m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벅어나 있으면 저위치 시킨다
			{
				mn_Pick_Step = 900;
				break;
			}
			mn_Pick_Step = 2000;
			break;

		//피커의 위치를 PICK가능한 위치로 이동한다.
		//먼저들어가도 되는지 확인한다.
		case 2000:
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y )  > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_LOADER_TRANSFER_Y] ) )
			{
				break;
			}
			if( st_sync.nLdUldSateyflag == LD_MOVING_SAFETY )
			{
				break;
			}

			if( Move_MakeSafetyPosBeforeWork(POS_MOV_PICK) == RET_GOOD )
			{
				m_motPickFlag[0] = CTL_NO;
				m_motPickFlag[1] = CTL_NO;
				mn_Pick_Step = 2100;
			}
			break;

		case 2100:
			//st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][1] 와 위치가 맞는지 비교해 본다.
			nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_X, nWorkSite, WORK_PICK, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_X);
			nRet_2 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWorkSite, WORK_PICK, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD  )
			{//ok
				m_dpTargetPosList[0] = m_dTemp_CalTargetPos_X;
				m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
			}
			else
			{
				CTL_Lib.Alarm_Error_Occurrence(4104, dWARNING, Func.m_strAlarmCode);
				break;
			}
			
			nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nRobot_X, m_dpTargetPosList[0], COMI.mn_runspeed_rate );
			nRet_2 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate );

			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_motPickFlag[0] = CTL_YES;
			}
			if( nRet_2 == BD_GOOD )
			{
				m_motPickFlag[1] = CTL_YES;
			}
			if( m_motPickFlag[0] == CTL_YES && m_motPickFlag[1] == CTL_YES )
			{
				m_motPickFlag[0] = CTL_NO;
				m_motPickFlag[1] = CTL_NO;
				m_bDvcWaitChk_Falg =  false;
				mn_Pick_Step = 2200;
			}
			else if (nRet_1 == BD_RETRY || nRet_2 == BD_RETRY)
			{
				mn_Pick_Step = 2100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4105, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 2100;
			}
			break;

		case 2200:
			m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벅어나 있으면 저위치 시킨다
			{
				mn_Pick_Step = 900;
				break;
			}
			nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_CHECK, m_nRobot_X, m_dpTargetPosList[0], COMI.mn_runspeed_rate );
			nRet_2 = CTL_Lib.Single_Move( ONLY_MOVE_CHECK, m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate );
			
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_motPickFlag[0] = CTL_YES;
			}
			if( nRet_2 == BD_GOOD )
			{
				m_motPickFlag[1] = CTL_YES;
			}
			if( m_motPickFlag[0] == CTL_YES && m_motPickFlag[1] == CTL_YES )
			{
				mn_Pick_Step = 3000;
			}
			else if (nRet_1 == BD_RETRY || nRet_2 == BD_RETRY)
			{
				mn_Pick_Step = 2200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4105, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 2100;
			}
			break;

		case 2300:
			Set_UnLoader_Transfer_Clamp_OnOff( IO_OFF );
			mn_Pick_Step = 2400;
			break;

		case 2400:
			nRet_1 = Chk_UnLoader_Transfer_Clamp_OnOff( IO_OFF );
			if( nRet_1 == RET_GOOD )
			{
				mn_Pick_Step = 3000;
			}
			else if( nRet_1 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 4106, dWARNING, alarm.mstr_code);
			}
			break;

		case  3000:
			nRet_1 = g_ioMgr.get_out_bit(st_io.o_Transfer1_Clamp_Forward_Sol,	IO_OFF); 
			nRet_2 = g_ioMgr.get_out_bit(st_io.o_Transfer1_Clamp_Backward_Sol,	IO_ON);
			nRet_3 = g_ioMgr.get_in_bit( st_io.i_Transfer1_Clamp_Forward_Check, IO_OFF );
			nRet_4 = g_ioMgr.get_in_bit( st_io.i_Transfer1_Clamp_Backward_Check, IO_ON );
			nRet_5 = g_ioMgr.get_in_bit( st_io.i_Unloader_Transfer_Device_Check, IO_OFF);

			if(nRet_1 != IO_OFF || nRet_2 != IO_ON || nRet_3 != IO_OFF || nRet_4 != IO_ON || nRet_5 != IO_OFF )
			{  //그리퍼가 벌리고 있지 않으면 자재에 데미지를 주기때문에 미리 벌리고 내려야 한다 
				mn_Pick_Step = 2300;
				break;
			}

			nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, m_dpTargetPosList[0], COMI.md_allow_value[m_nRobot_X]);
			nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], COMI.md_allow_value[m_nRobot_Y]);
			if( nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				mn_Pick_Step = 2000;
				break;
			}

			if(nWorkSite == THD_LDULD_CARRIER_BUFF )
			{
				m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_PICKTOP_POS + m_n_FirstTray_Y_Num];
			}
			else
			{
				break;
			}

			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 
				Set_UnLoader_Transfer_Clamp_OnOff( IO_OFF );
				mn_Pick_Step = 3100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4107, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 3000;
			}
			break;

		case 3100:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 			 
				mn_Pick_Step = 3110;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4108, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 3000;
			}		
			break;

		case 3110:
			nRet_1 = Chk_UnLoader_Transfer_Clamp_OnOff(IO_OFF); 
			if(nRet_1 == RET_GOOD )
			{
				mn_Pick_Step = 3200;
			}
			else if(nRet_1 == RET_ERROR || nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(4109, dWARNING, m_strAlarmCode);
				mn_Pick_Step = 3000;
			}
			break;

		case 3200:
			Set_UnLoader_Transfer_Clamp_OnOff( IO_ON );
			mn_Pick_Step = 3210;
			break;

		case 3210:
			nRet_1 = Chk_UnLoader_Transfer_Clamp_OnOff( IO_ON);
			if(nRet_1 == RET_GOOD )
			{
				mn_Pick_Step = 3300;
			}
			else if( nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(4110, dWARNING, m_strAlarmCode);
				mn_Pick_Step = 3200;
			}
			break;

		case 3300:
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io. i_Transfer1_Clamp_Forward_Check, IO_ON, IO_STABLE_WAIT, 1000); //IO_STABLE_WAIT == 100ms
			if(nRet_1 == RET_GOOD )
			{
				if( st_handler.bLoaderOnFlag == CTL_NO )
				{
					mn_Pick_Step = 3400;
				}
				else
				{
					m_nPickRetry_Cnt++;
					if( m_nPickRetry_Cnt < 3)
					{
						mn_Pick_Step = 3210;
						break;
					}
					m_nPickRetry_Cnt = 0;
					//900200 0 0 "REMOVE DEVIC. INPUT DEVUCE IS MUCH MORE THAN PRODUCT QTY."
					CTL_Lib.Alarm_Error_Occurrence(4111, dWARNING, _T("900200") );
				}
			}
			else if(nRet_1 == RET_PROCEED)
			{
				break;
			}
			else if(nRet_1 == RET_ERROR)
			{
				if(st_basic.n_mode_device == WITHOUT_DVC )
				{
					mn_Pick_Step = 3400;
					break;
				}
				if( st_handler.bLoaderOnFlag == CTL_YES )
				{
					mn_Pick_Step = 3400;
					/*nFuncRet = RET_TRAY_NOT_FIND;*/
					break;
				}
				//800113 0 A "LOADER_TRANSFER_CLAMP_OFF_센서_OFF_CHECK_ERROR.
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loader_Transfer_Clamp_Off_Check); 
				CTL_Lib.Alarm_Error_Occurrence(4112, dWARNING, m_strAlarmCode);
			} 
			break;

		case 3400:
			if(nWorkSite == THD_LDULD_CARRIER_BUFF)
			{
				mn_Pick_Step = 4000;
			}
			else
			{
				if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
				{
					sprintf(st_msg.c_abnormal_msg, "There is no PickupPos(CARRIER_BUFF) at Process_DVC_Pickup Run_UldPicker  = %d", mn_Pick_Step);
					st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					//st_work.mn_run_status = dSTOP;
				}
				break;
			} 
			break;
		
		case  4000:
			m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 
				Set_UnLoader_Transfer_Clamp_OnOff(IO_ON); //동작한 피커를 올린다  
				mn_Pick_Step = 4100 ; //시간 단추을 위헤 체크는 다음으로 
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 4000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4113, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 4000;
			}		
			break;

		case 4100:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK,m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 
				m_bDvcWaitChk_Falg = false;
				mn_Pick_Step = 5000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 4000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4114, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 4000;
			}		
			break;

		case 5000:
			nRet_1 = Func.Check_PickerStatus(1, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);

			if (nRet_1 != RET_GOOD) //
			{
				if(m_bDvcWaitChk_Falg == false)
				{
					m_bDvcWaitChk_Falg =  true;
					m_dwWaitUntil[0] = GetCurrentTime();
					break;
				}
				else
				{
					m_dwWaitUntil[1] = GetCurrentTime();
					m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
					if(m_dwWaitUntil[2] <= 0) m_dwWaitUntil[0] = GetCurrentTime();
					if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;

					CTL_Lib.Alarm_Error_Occurrence(4115, dWARNING, Func.m_strAlarmCode);
					break;
				}
			}

			for (i = m_n_FirstPicker_Num; i < m_nPickerPara; i++)
			{
				if(nWorkSite == THD_LDULD_CARRIER_BUFF)
				{
					if(i >= 0 && m_npFindWorkPosYXCPB[1] + (i - m_n_FirstPicker_Num) < st_recipe.nCarrierBuffer_Num)
					{
						m_npPicker_YesNo[i] = CTL_NO; //작업한 피커 정보 클리어

						//아래 함수에서 사용위해 위치 정보를 셋팅한다
						m_npSet_WorkPosYXCPB[0]  = m_npFindWorkPosYXCPB[0]; //tray X Pos 위치
						m_npSet_WorkPosYXCPB[1]  = m_npFindWorkPosYXCPB[1] + (i - m_n_FirstPicker_Num); //tray Y Pos 위치
						m_npSet_WorkPosYXCPB[3]  = i; //picker 위치 정보

						Func.Data_Exchange_PickPlace(PICKER_PICK_MODE, 1, m_nRobotSite, nWorkSite, m_npSet_WorkPosYXCPB); //피커 및 트레이 정보 셋팅 함수	
						
						//2017.0614
						st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][5] = m_npSet_WorkPosYXCPB[1];
						////////////////////////////////////////////////////////////////////////////////////////////////////

						if (m_strLotNo[1] == _T(""))
						{
							m_strLotNo[1]	= st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strLotNo;
							m_strPartNo[1] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strPartNo;
						}
					}	
				}
				else
				{
					break;
				}
			}

			if (nRet_1 == RET_GOOD) // 1000 step에서 셋팅한 피커 정보에 맞게 모두 정상적 집었을 경우 이
			{
				//화면 정보 갱신
				m_nPickRetry_Cnt = 0;

				m_bDvcWaitChk_Falg = false;

				if (st_handler.cwnd_main != NULL)
				{
					st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);
				}

				mn_Pick_Step = 6000;
			}
			else if (nRet_1 == RET_ERROR) //셋팅한 피커중 한개라도 제대로 집지 못했으면 이곳으로 이동
			{
				m_nPickRetry_Cnt++;
				CTL_Lib.Alarm_Error_Occurrence(4116, dWARNING, Func.m_strAlarmCode);
				//kwlee 2017.0214
				//mn_Pick_Step = 0;
			}
			break;

		case 6000:
			nRet_1 = Func.Check_PickerStatus(0, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Data_Status, m_npPicker_OutputStatus);
			if (nRet_1 == RET_GOOD) //
			{
				nFuncRet = RET_GOOD;
				mn_Pick_Step = 0;
			}
			else //if(nRet_1 == RET_ERROR)
			{
				if(m_bDvcWaitChk_Falg == false)
				{
					m_bDvcWaitChk_Falg =  true;
					m_dwWaitUntil[0] = GetCurrentTime();
					break;
				}
				else
				{
					m_dwWaitUntil[1] = GetCurrentTime();
					m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
					if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
					CTL_Lib.Alarm_Error_Occurrence(4117, dWARNING, Func.m_strAlarmCode);
				}
			}
			break;

		default :
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Pick_Step at Process_DVC_Pickup Run_UldPicker  = %d", mn_Pick_Step);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			break;
	}

	return nFuncRet;
}

int CRun_UldPicker::Process_DVC_Place( int nMode, int nWorkSite, CString strLotNo)//THD_ULD_ALIGN_BUFF
{
	int nFuncRet = RET_PROCEED;
	int nRet_1, nRet_2, i, nFlag;

	Func.ThreadFunctionStepTrace(32, mn_Place_Step);

	//2017.0614
	if( mn_Place_Step >= 2000 && mn_Place_Step < 6000 )
	{
		st_handler.m_nAssemblyAble = CTL_NO;
		g_ioMgr.set_out_bit(st_io.o_Interface_Output_1, IO_OFF);
	}

	switch(mn_Place_Step)
	{
	case 0:
		m_bDvcWaitChk_Falg =  false;
		m_nPickRetry_Cnt = 0;
		mn_Place_Step = 100;
		break;

	case 100:
		m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
		if(m_dCurrentPos[m_nRobot_Z] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벗어나 있으면 저위치 시킨다
		{
			mn_Place_Step = 200;
		}
		else
		{
			mn_Place_Step = 1000;
		}
		break;

	case 200:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_bDvcWaitChk_Falg =  false;
			mn_Place_Step = 1000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Place_Step = 200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4201, dWARNING, alarm.mstr_code);
			mn_Place_Step = 200;
		}
		break;

	case 1000:
		nRet_1 = Func.Check_PickerStatus(0, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus); //전역 변수와 센서 상태 체크 
		if(nRet_1 == RET_GOOD)
		{
			m_bDvcWaitChk_Falg = false; //피커 정보와 현재 상태가 정상이다
		}
		else// if(nRet_1 == RET_ERROR)
		{
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
				CTL_Lib.Alarm_Error_Occurrence(4203, dWARNING, Func.m_strAlarmCode);
				break;
			}
		}
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
		if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벅어나 있으면 저위치 시킨다
		{
			mn_Place_Step = 200;
			break;
		}
		m_n_FirstTray_X_Num = -1; m_n_FirstTray_Y_Num = -1; m_n_FirstPicker_Num = -1; //flag clear
		for(i = 0; i < m_nPickerPara; i++)
		{
			if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_YES)//피커에 자재가 들고 있다는 정보가 없다
			{
				m_n_FirstPicker_Num = i;
				i = m_nPickerPara;
			}
		}
		if(m_n_FirstPicker_Num == -1)
		{
			//피커에 더이상 작업할 것이 없다
			mn_Place_Step = 0;
			nFuncRet = RET_PICKER_NOT_FIND;
			break;
		}

		nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, nWorkSite, CTL_NO, m_npFindWorkPosYXCPB, THD_UNLOAD_WORK_RBT, strLotNo); //놓을때버퍼에서으 ㅣ작업순서는 뒤부터 놓는다 (4 -> 1 순서) 
		if(nRet_1 == RET_GOOD) //작업 가능한 자재가 있다
		{
			m_n_FirstTray_X_Num = m_npFindWorkPosYXCPB[0]; m_n_FirstTray_Y_Num = m_npFindWorkPosYXCPB[1]; 
			//st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][2] = mn_BufferPos 의 위치와 m_n_FirstTray_Y_Num가 맞는지 확인필요
		}
		else if(nRet_1 == RET_SKIP) //더이상 자재가 없다
		{	//계속 진행할 수 없다
			mn_Place_Step = 0;
			nFuncRet = RET_TRAY_NOT_FIND;
			break;
		}
		nFlag = 0;
		if(m_n_FirstPicker_Num != -1 && m_n_FirstTray_Y_Num != -1 && m_n_FirstTray_X_Num != -1) //작업할 공간이 다 있을때 다음 작업을 진행한다
		{
			for(i = 0;  i < m_nPickerPara; i++)  //m_n_FirstPicker_Num; y < m_mPickerPara; y++)
			{
				m_npPicker_YesNo[i] = CTL_NO; //작업할 피커정보 clear
				if(i < m_n_FirstPicker_Num) continue;

				if( nWorkSite == THD_ULD_ALIGN_BUFF )
				{
					if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_YES)// && st_picker[m_nRobotSite].st_pcb_info[i].nEnable == CTL_YES)
					{
						if(i >= 0 && m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num) < st_recipe.nTrayY)
						{
							if(st_buffer_info[nWorkSite].st_pcb_info[ m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)].nYesNo == CTL_NO)
							{
								m_npPicker_YesNo[i] = CTL_YES;
								nFlag++;
							}
						}
					} 
				}
				else
				{
					if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
					{
						sprintf(st_msg.c_abnormal_msg, "There is no Place_Step at Process_DVC_Place Place_Step = %d", mn_Place_Step);
						st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
					}
					Func.OnSet_IO_Port_Stop();
					break;
				}
			}
			if(nFlag == 0) //작업할 피커가 하나도 준비되 않았다.
			{ //이부분이 아오면 안됨 
				mn_Place_Step = 0;
				nFuncRet = RET_PICKER_NOT_FIND;//피커에 더이상 작업할 것이 없다
				break;
			}

			//실제 작업 위치를 셋팅
			m_npFindWorkPosYXCPB[0] = m_n_FirstTray_Y_Num; m_npFindWorkPosYXCPB[1] = m_n_FirstTray_X_Num; m_npFindWorkPosYXCPB[3] = m_n_FirstPicker_Num;

			m_nMove_Flag[0] = CTL_NO; m_nMove_Flag[1] = CTL_NO;
			mn_Place_Step = 2000;
		}
		break;

	case 2000:
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
		if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벗어나 있으면 정위치 시킨다
		{
			mn_Place_Step = 200;
			break;
		}

		if(nWorkSite == THD_ULD_ALIGN_BUFF)
		{//Let's check clash condition
		}
		else 
		{
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Place_Step at Process_DVC_Place Place_Step = %d", mn_Place_Step);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			Func.OnSet_IO_Port_Stop();
			break;
		}
		nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_X, nWorkSite, 0, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_X); 
		nRet_2 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWorkSite, 0, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y); 		
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{//ok
			m_dpTargetPosList[0] = m_dTemp_CalTargetPos_X;
			m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(4203, dWARNING, Func.m_strAlarmCode);
			break;
		}
		m_motPlaceFlag[0] = CTL_NO;
		m_motPlaceFlag[1] = CTL_NO;
		mn_Place_Step = 2100;
		break;

	case 2100:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_X, m_dpTargetPosList[0], COMI.mn_runspeed_rate); 
		nRet_2 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate); 
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_motPlaceFlag[0] = CTL_YES;
		}
		if( nRet_2 == BD_GOOD )
		{
			m_motPlaceFlag[1] = CTL_YES;
		}
		if( m_motPlaceFlag[0] == CTL_YES && m_motPlaceFlag[1] == CTL_YES )
		{
			m_motPlaceFlag[0] = CTL_NO;
			m_motPlaceFlag[1] = CTL_NO;
			m_bDvcWaitChk_Falg =  false;
			mn_Place_Step = 2200;
		}
		else if (nRet_1 == BD_RETRY || nRet_2 == BD_RETRY )
		{
			mn_Place_Step = 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4204, dWARNING, alarm.mstr_code);
			mn_Place_Step = 2100;
		}
		break; 	

	case 2200:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_X, m_dpTargetPosList[0], COMI.mn_runspeed_rate); 
		nRet_2 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate); 
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_motPlaceFlag[0] = CTL_YES;
		}
		if( nRet_2 == BD_GOOD )
		{
			m_motPlaceFlag[1] = CTL_YES;
		}
		if( m_motPlaceFlag[0] == CTL_YES && m_motPlaceFlag[1] == CTL_YES )
		{
			m_motPlaceFlag[0] = CTL_NO;
			m_motPlaceFlag[1] = CTL_NO;
			m_bDvcWaitChk_Falg =  false;
			mn_Place_Step = 3000;
		}
		else if (nRet_1 == BD_RETRY|| nRet_2 == BD_RETRY)
		{
			mn_Place_Step = 2100;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4205, dWARNING, alarm.mstr_code);
			mn_Place_Step = 2100;
		}
		break; 	

	case 2500:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_bDvcWaitChk_Falg =  false;
			mn_Place_Step = 3000;

		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Place_Step = 2500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4206, dWARNING, alarm.mstr_code);
			mn_Place_Step = 2500;
		}
		break;

		////////////////////////////////////////////////////////
		// 작업 할 자재를 놓는다 
		////////////////////////////////////////////////////////
	case 3000:
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
		if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z])
		{
			mn_Place_Step = 2500;
			break;
		}

		nRet_1 = Func.Check_PickerStatus(0, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
		if(nRet_1 == RET_GOOD)
		{
			m_bDvcWaitChk_Falg = false; //피커 정보와 현재 상태가 정상이다
		}
		else// if(nRet_1 == RET_ERROR)
		{
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
				CTL_Lib.Alarm_Error_Occurrence(4207, dWARNING, Func.m_strAlarmCode);
				break;
			}
		}

		nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, m_dpTargetPosList[0], COMI.md_allow_value[m_nRobot_X]);
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], COMI.md_allow_value[m_nRobot_Y]);
		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
		{
			mn_Place_Step = 2000;
			break;
		}

		if( nWorkSite == THD_ULD_ALIGN_BUFF )
		{//UNLOAD JIG의 디바이스 감지 센서가 맞는가?
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Unloader_Transfer_Device_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
			if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
			{
				break; 
			} 
			else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Unloader_Transfer_Device_Check);
				CTL_Lib.Alarm_Error_Occurrence(4208, dWARNING, m_strAlarmCode);
				break;
			} 
			m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_ZIGPLACE_POS];
		}
		else
		{//920000 1 A "THERE_IS_NO_SITE_TO_GO_AT_PLACE_DVC_IN_RUN_ULDPICKER."
			CTL_Lib.Alarm_Error_Occurrence(4209, dWARNING, "920000");
			break;
		}

		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{ 			
			mn_Place_Step = 3100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Place_Step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4210, dWARNING, alarm.mstr_code);
			mn_Place_Step = 3000;
		}		
		break;

	case 3100:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{ 			 
			mn_Place_Step = 3200;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Place_Step = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4211, dWARNING, alarm.mstr_code);
			mn_Place_Step = 3000;
		}		
		break;

	case 3200:
		if(nWorkSite == THD_ULD_ALIGN_BUFF)
		{
			mn_Place_Step = 3300;
		}
		else
		{
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Place_Step at Process_DVC_Place Place_Step = %d", mn_Place_Step);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			Func.OnSet_IO_Port_Stop();
		} 
		break;

		////////////////////////////////////////////////////////////
		//Unload buffer의 clamp를 이곳에서 제어 
		//제어 단순화을 위해 언로딩 버퍼의 클램프를 이곳에서 제어 한다 
		////////////////////////////////////////////////////////////
	case 3300:
		FAS_IO.Set_IO_OnOff(st_io.o_Transfer1_Clamp_Forward_Sol, IO_OFF);
		FAS_IO.Set_IO_OnOff(st_io.o_Transfer1_Clamp_Backward_Sol , IO_ON);
		mn_Place_Step = 3310;
		break;

	case 3310:
		nRet_1 = FAS_IO.Wait_IO_OnOff(st_io.o_Transfer1_Clamp_Backward_Sol, IO_ON, st_wait.nOffWaitTime[WAIT_PICKER_CLAMP_ON_OFF]);
		if(nRet_1 == RET_GOOD) //작업 가능한 자재가 있다
		{
			mn_Place_Step = 3320;
		}
		break;

	case 3320:
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.o_Transfer1_Clamp_Forward_Sol, IO_OFF, IO_STABLE_WAIT, 1000);
		if(nRet_1 == RET_GOOD) //감지가 안되도 안전화 시간까지는 기다리자 
		{
			mn_Place_Step = 4000;
		} 
		else if(nRet_1 == RET_PROCEED)
		{
			break;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_basic.n_mode_device == WITHOUT_DVC)
			{
				mn_Place_Step = 3400;
				break;
			}
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loader_Transfer_Clamp_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(4212, dWARNING, m_strAlarmCode);
		} 
		break; 

	case 3400:
		Set_UnLoader_Transfer_Clamp_OnOff( IO_OFF );
		mn_Place_Step = 3410;
		break;

	case 3410:
		nRet_1 = Chk_UnLoader_Transfer_Clamp_OnOff(IO_OFF); 			 
		if(nRet_1 == RET_GOOD)
		{
			m_bDvcWaitChk_Falg = false;
			mn_Place_Step = 3420;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_basic.n_mode_device == WITHOUT_DVC)
			{
				m_bDvcWaitChk_Falg = false;
				mn_Place_Step = 3420;
				break;
			}
			CTL_Lib.Alarm_Error_Occurrence(4213, dWARNING, m_strAlarmCode);
			mn_Place_Step = 3400;
		}
		break;

	case 3420:
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Transfer1_Clamp_Backward_Check, IO_ON, st_wait.nOnWaitTime[WAIT_PICKER_CLAMP_ON_OFF], st_wait.nLimitWaitTime[WAIT_PICKER_CLAMP_ON_OFF]); 
		if(nRet_1 == RET_GOOD) //감지가 안되도 안전화 시간까지는 기다리자 
		{
			mn_Place_Step = 4000;
		} 
		else if(nRet_1 == RET_ERROR)
		{
			m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loader_Transfer_Clamp_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(4214, dWARNING, m_strAlarmCode);
		} 
		break; 

		////////////////////////////////////////////////////////////
		//로보트를 안전 위치로 올린다 
		//Carrier에서는 제자리에 있는 체크한다.
		////////////////////////////////////////////////////////////
	case 4000:
		if( nWorkSite == THD_ULD_ALIGN_BUFF )
		{
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_Place_Step = 4010;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Place_Step = 4000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4215, dWARNING, alarm.mstr_code);
				mn_Place_Step = 4000;	
			}
		}
		else
		{//920000 1 A "THERE_IS_NO_SITE_TO_GO_AT_PLACE_DVC_IN_RUN_ULDPICKER."
			CTL_Lib.Alarm_Error_Occurrence(4216, dWARNING, "920000");
		}
		break;

		//carrirer buffer에서 디바이스 위치가 맞는치 체크힌다.
	case 4010:
		nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Unloader_Transfer_Device_Check, IO_OFF, IO_STABLE_WAIT, 1000);
		if(nRet_1 == RET_GOOD) //감지가 안되도 안전화 시간까지는 기다리자 
		{
			mn_Place_Step = 4020;
		} 
		else if(nRet_1 == RET_PROCEED)
		{
			break;
		}
		else if(nRet_1 == RET_ERROR)
		{
			if(st_basic.n_mode_device == WITHOUT_DVC)
			{
				mn_Place_Step = 4020;
				break;
			}
			m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loader_Transfer_Clamp_Off_Check); 
			CTL_Lib.Alarm_Error_Occurrence(4217, dWARNING, m_strAlarmCode);
		} 
		break; 

	case 4020:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_Place_Step = 4100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_Place_Step = 4000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4218, dWARNING, alarm.mstr_code);
			mn_Place_Step = 4000;	
		}
		break;

	case 4100:
		if( nWorkSite == THD_ULD_ALIGN_BUFF )
		{
			mn_Place_Step = 4200;
		}
		else
		{//920000 1 A "THERE_IS_NO_SITE_TO_GO_AT_PLACE_DVC_IN_RUN_ULDPICKER."
			CTL_Lib.Alarm_Error_Occurrence(4219, dWARNING, "920000");
		}
		break;

	case 4200:
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
		if(m_dCurrentPos[2] < st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벅어나 있으면 저위치 시킨다
		{
			mn_Place_Step = 5000;
		}
		else
		{
			mn_Place_Step = 4020;
		}
		break;

		////////////////////////////////////////////////////////
		// 작업을 완료한 상태로 피커 상태를 확인한다
		// 피커 데이터를 버퍼/트레이에 data exchange 한다
	case 5000:
		nRet_1 = Func.Check_PickerStatus(1, THD_UNLOAD_WORK_RBT, CTL_NO, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
		if (nRet_1 != RET_GOOD) //
		{//안정 시간을 준다
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] <= 0) m_dwWaitUntil[0] = GetCurrentTime();
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
				CTL_Lib.Alarm_Error_Occurrence(4220, dWARNING, Func.m_strAlarmCode);
				break;
			}
		}

		for (i = m_n_FirstPicker_Num; i < m_nPickerPara; i++)
		{
			if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_YES && m_npPicker_YesNo[i] == CTL_YES
				&& (m_npPicker_Vacuum_Status[i] == IO_OFF && m_npPicker_OutputStatus[i] == IO_OFF))
			{
				if (m_strLotNo[1] == _T(""))
				{
					m_strLotNo[1]	= st_picker[m_nRobotSite].st_pcb_info[i].strLotNo;
					m_strPartNo[1] = st_picker[m_nRobotSite].st_pcb_info[i].strPartNo;
				}				 

				if(nWorkSite = THD_ULD_ALIGN_BUFF)
				{
					if(i >= 0 && m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num) < st_recipe.nLdBuffer_Num)
					{
						m_npPicker_YesNo[i] = CTL_NO;	//작업한 피커 정보 클리어

						//아래 함수에서 사용위해 위치 정보를 셋팅한다
						m_npSet_WorkPosYXCPB[0] = m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num); //Y Pos 위치
						m_npSet_WorkPosYXCPB[3]  = i; //picker 위치 정보

						Func.Data_Exchange_PickPlace(PICKER_PLACE_MODE, 1, m_nRobotSite, nWorkSite, m_npSet_WorkPosYXCPB); //피커 및 트레이 정보 셋팅 함수
						g_lotMgr.GetLotAt(0).AddPassCnt(PRIME);
					}
					//2017.0109
					clsLog.LogTransfer(_T("LOADING_ROBOT"),
						_T("MOVE_CDIMM"),
						0,
						st_picker[m_nRobotSite].st_pcb_info[i].strMaterial,
						_T("CDIMM"),
						st_picker[m_nRobotSite].st_pcb_info[i].strMovePos[0],
						st_picker[m_nRobotSite].st_pcb_info[i].strMovePos[1],
						1, 
						st_picker[m_nRobotSite].st_pcb_info[i].strLogKey,
						st_picker[m_nRobotSite].st_pcb_info[i].strLogData);
				}
				else
				{//920000 1 A "THERE_IS_NO_SITE_TO_GO_AT_PLACE_DVC_IN_RUN_ULDPICKER."
					CTL_Lib.Alarm_Error_Occurrence(4221, dWARNING, "920000");	
					return nFuncRet;
				}
			}
		}

		if (nRet_1 == RET_GOOD) // 1000 step에서 셋팅한 피커 정보에 맞게 모두 정상적 집었을 경우 이
		{
			//화면 정보 갱신
			m_bDvcWaitChk_Falg = false;

			if (st_handler.cwnd_main != NULL)
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_COUNTINFO, 0);
			}

			mn_Place_Step = 6000;
		}
		else if (nRet_1 == RET_ERROR) //셋팅한 피커중 한개라도 제대로 집지 못했으면 이곳으로 이동
		{
			CTL_Lib.Alarm_Error_Occurrence(4222, dWARNING, Func.m_strAlarmCode);
		}
		break;

	case 6000:
		nRet_1 = Func.Check_PickerStatus(0, THD_UNLOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Data_Status, m_npPicker_OutputStatus);

		if(nRet_1 == RET_GOOD)
		{
			nFuncRet = RET_GOOD;
			mn_Place_Step = 0;
		}
		if (nRet_1 == RET_ERROR) //
		{
			if(m_bDvcWaitChk_Falg == false)
			{
				m_bDvcWaitChk_Falg =  true;
				m_dwWaitUntil[0] = GetCurrentTime();
				break;
			}
			else
			{
				m_dwWaitUntil[1] = GetCurrentTime();
				m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) m_dwWaitUntil[0] = GetCurrentTime();
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
			}
			CTL_Lib.Alarm_Error_Occurrence(4223, dWARNING, Func.m_strAlarmCode);
		}
		break;

	default:
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "There is no lace_Ste at Process_DVC_Place in RunLdUldRobot  = %d", mn_Place_Step);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		break;
	}
	return nFuncRet;
}

//nMode = 0 safety 영역인지 아닌지 확인한다 그리고 아니면 이동한다.
//nMode = 1 reay위치로 이동
//nMode = 2 pick 로보트가 일단 safety위치로 이동한다. LOAD로봇이 동작할 수 있도록  //,로드 로봇 및 unpress로보 위치를 파악한다.
//nMode = 3 place 로보트가 Pickeup을 위해 carrier로봇으로 들어가기 전에 대기한다.
//nWorkSite - 대기 위치, Pick위치, Place위치
int CRun_UldPicker::Move_MakeSafetyPosBeforeWork( int nMode )
{
	int nFuncRet = RET_PROCEED, nRet_1 = 0, nRet_2 = 2;
	double d_MotorPos[3], d_MotorLoaderY;

	d_MotorPos[0] = COMI.Get_MotCurrentPos( m_nRobot_X );
	d_MotorPos[1] = COMI.Get_MotCurrentPos( m_nRobot_Y );
	d_MotorPos[2] = COMI.Get_MotCurrentPos( m_nRobot_Z );
	d_MotorLoaderY = COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y );
	//unpress는 실린더 확인을 해야하는데

	Func.ThreadFunctionStepTrace(33, mn_RunSafetyStep);
	switch( mn_RunSafetyStep )
	{
	case 0:
		mn_RunSafetyStep = 100;
		break;

	case 100:
		if( COMI.Get_MotCurrentPos( m_nRobot_Z ) > (st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) )
		{
			mn_RunSafetyStep = 110;
		}
		else
		{
			mn_RunSafetyStep = 1000;
		}
		break;

	case 110:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_RunSafetyStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4301, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 110;
		}
		break;

	case 1000:
		if( nMode < POS_CHK_READY || nMode > POS_MOV_PLACE)
		{
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Mode at Move_MakeSafetyPosBeforeWork  RunSafetyStep= %d", mn_RunSafetyStep);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			//st_work.mn_run_status = dSTOP;
			break;
		}
		if( nMode == POS_CHK_READY )
		{
			mn_RunSafetyStep = 2000;
		}
		else if( nMode == POS_MOV_READY)
		{
			mn_RunSafetyStep = 3000;
		}
		else if( nMode == POS_MOV_PICK)
		{
			mn_RunSafetyStep = 4000;
		}
		else//POS_MOV_PLACE
		{
			mn_RunSafetyStep = 5000;
		}
		break;

	case 2000:
		if( m_nRobot_X > (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + COMI.md_allow_value[m_nRobot_X]) ) nFuncRet = RET_ERROR;
		else                                                                                                                                                                         	nFuncRet = RET_GOOD;
		mn_RunSafetyStep = 0;
		break;

	//1. x위치가 READY 위치보다 크면 X,Y 각각 Ready위치로 이동한다.
	//1. x위치가 READY 위치보 사이에 있으면 안된다. 혹시 y위치가 에메한 위치에 있으면 Y부터 safety로 움직여야한다.
	//x위치가 ready보다 크면 안된다,
	case 3000:
		if( COMI.Get_MotCurrentPos( m_nRobot_X ) < (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + COMI.md_allow_value[m_nRobot_X]) )
		{
			mn_RunSafetyStep = 3100;
		}//체크해보자
		else if ( ( COMI.Get_MotCurrentPos( m_nRobot_X ) >= (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + COMI.md_allow_value[m_nRobot_X]) ) &&
			( COMI.Get_MotCurrentPos( m_nRobot_X ) < st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS] - ( 5*COMI.md_allow_value[m_nRobot_X]) ) )
		{
			mn_RunSafetyStep = 3200;
		}
		else if(  COMI.Get_MotCurrentPos( m_nRobot_X ) >= (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS] - ( 5*COMI.md_allow_value[m_nRobot_X]) ) )
		{
			mn_RunSafetyStep = 3200;
		}
		else
		{
			nFuncRet = RET_GOOD;
			mn_RunSafetyStep = 6000;
		}
		break;

	case 3100:
		if( Move_ReadySafeyXY() == RET_GOOD )
		{
			nFuncRet = RET_GOOD;
			mn_RunSafetyStep = 0;
		}
		break;

	case 3200:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_RunSafetyStep = 3300;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 3200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4302, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 3200;
		}
		break;

	case 3300:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_RunSafetyStep = 3000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 3300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4303, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 3300;
		}
		break;

	//POS_MOV_PICK
	case 4000:
		if( COMI.Get_MotCurrentPos( m_nRobot_X ) < (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] - COMI.md_allow_value[m_nRobot_X]) )
		{
			mn_RunSafetyStep = 4100;
		}//체크해보자
		else if ( ( COMI.Get_MotCurrentPos( m_nRobot_X ) >= (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] - COMI.md_allow_value[m_nRobot_X] ) ) &&
			( COMI.Get_MotCurrentPos( m_nRobot_X ) < st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS] - ( 5*COMI.md_allow_value[m_nRobot_X] ) ) )
		{
			mn_RunSafetyStep = 4200;
		}
// 		else if(  m_nRobot_X > (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS] - ( 5*COMI.md_allow_value[m_nRobot_X]) ) )
// 		{
// 			mn_RunSafetyStep = 4300;
// 		}
		else
		{
			nFuncRet = RET_GOOD;
			mn_RunSafetyStep = 6000;
		}
		break;

	case 4100:
		if( Move_ReadySafeyXY() == RET_GOOD )
		{
			mn_RunSafetyStep = 4000;
		}
		break;

	case 4200:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_RunSafetyStep = 4210;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 4200;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4303, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 4200;
		}
		break;

	case 4210:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_RunSafetyStep = 4000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 4210;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4304, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 4210;
		}
		break;

	//POS_MOV_PLACE 동작
	case 5000:
		if( COMI.Get_MotCurrentPos( m_nRobot_X ) < (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] - COMI.md_allow_value[m_nRobot_X]) )
		{//위험지역을 벗어났으므로 XY, 동시동작으로 JIG로 움지인다.
			m_motPickFlag[0] = CTL_NO;
			m_motPickFlag[1] = CTL_NO;
			mn_RunSafetyStep = 5500;
		}
		else if( COMI.Get_MotCurrentPos( m_nRobot_X ) >= (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[m_nRobot_X]) &&
				  COMI.Get_MotCurrentPos( m_nRobot_X ) < (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_OUTREADY_POS] - COMI.md_allow_value[m_nRobot_X]))
		{//위험지역이므로 Y를 먼저 움직인다.
			mn_RunSafetyStep = 5100;//위험지역
		}
		else //if(m_nRobot_X >= (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_OUTREADY_POS] - st_motor[m_nRobot_X].mn_allow))
		{
			m_motPickFlag[0] = CTL_NO;
			m_motPickFlag[1] = CTL_NO;
			mn_RunSafetyStep = 5600;
		}
		break;

	case 5100:
		//ASEMBLER쪽에 위치해 있는가?
		if( COMI.Get_MotCurrentPos( m_nRobot_X ) <  st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS]  + COMI.md_allow_value[m_nRobot_Y] )
		{//X를 먼저 P_UNLOADER_TRANSFER_X_INREADY_POS로 이동
			mn_RunSafetyStep = 5110;
		}
		else
		{//X를 먼저 P_UNLOADER_TRANSFER_X_OUTREADY_POS로 이동
			mn_RunSafetyStep = 5150;
		}
		break;

	case 5110:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_RunSafetyStep = 5000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 5110;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4305, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 5110;
		}
		break;
		
	case 5150:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_OUTREADY_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_RunSafetyStep = 5000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 5150;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4306, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 5150;
		}
		break;

		//asembler위치에서 Inready위치로 이동한다
	case 5500:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS], COMI.mn_runspeed_rate);
		nRet_2 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_motPickFlag[0] = CTL_YES;
		}
		if(nRet_2 == BD_GOOD)
		{
			m_motPickFlag[1] = CTL_YES;
		}
		if( m_motPickFlag[0] == CTL_YES && m_motPickFlag[1] == CTL_YES )
		{
			m_motPickFlag[0] = CTL_NO; m_motPickFlag[1] = CTL_NO;
			mn_RunSafetyStep = 5600;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 5500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4307, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 5500;
		}
		break;

		//asembler위치에서 outready위치로 이동한다
	case 5600:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS], COMI.mn_runspeed_rate);
		nRet_2 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);

		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_motPickFlag[0] = CTL_YES;
		}
		if(nRet_2 == BD_GOOD)
		{
			m_motPickFlag[1] = CTL_YES;
		}
		if( m_motPickFlag[0] == CTL_YES && m_motPickFlag[1] == CTL_YES )
		{
			m_motPickFlag[0] = CTL_NO; m_motPickFlag[1] = CTL_NO;
			mn_RunSafetyStep = 5700;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunSafetyStep = 5600;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(4308, dWARNING, alarm.mstr_code);
			mn_RunSafetyStep = 5600;
		}
		break;

	case 5700:
		nRet_1 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.md_allow_value[m_nRobot_Y]);
		if(nRet_1 != BD_GOOD)
		{
			mn_RunSafetyStep = 5000;
		}
		else
		{
			if( m_nRobot_X < (st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] - COMI.md_allow_value[m_nRobot_X]) )
			{
				mn_RunSafetyStep = 5800;
			}
			else
			{
				nFuncRet = RET_GOOD;
				mn_RunSafetyStep = 6000;
			}
		}
		break;

	case 5800:
		nRet_1 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.md_allow_value[m_nRobot_Y]);
		if(nRet_1 != BD_GOOD)
		{
			COMI.Set_MotStop( 0 , m_nRobot_X );
			COMI.Set_MotStop( 0 , m_nRobot_Y );
			mn_RunSafetyStep = 5000;
		}
		else
		{
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				nFuncRet = RET_GOOD;
				mn_RunSafetyStep = 6000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunSafetyStep = 5800;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(4310, dWARNING, alarm.mstr_code);
				mn_RunSafetyStep = 5800;
			}
			break;
		}

	case 6000:
		mn_RunSafetyStep = 0;
		break;


	default:
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "There is no RunSafetyStep at Move_MakeSafetyPosBeforeWork in Run_UldPicker  = %d", mn_RunSafetyStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		break;
	}

	return nFuncRet;
}


int CRun_UldPicker::Move_ReadySafeyXY()
{
	int nRet_1 = 0, nRet_2 = 0;
	int nFuncRet = RET_PROCEED;

	switch( mn_MoveStep)
	{
		case 0:
			m_nPickRetry_Cnt = 0;
			m_motFlag[0] = CTL_NO;
			m_motFlag[1] = CTL_NO;
			mn_MoveStep = 1000;
			break;

		case 100:
			m_motFlag[0] = CTL_NO;
			m_motFlag[1] = CTL_NO;
			mn_MoveStep = 1000;
			break;

		case 1000:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS], COMI.mn_runspeed_rate);
			nRet_2 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_motFlag[0] = CTL_YES;
			}
			if (nRet_2 == BD_GOOD) //좌측으로 이동
			{
				m_motFlag[1] = CTL_YES;
			}
			if(m_motFlag[0] == CTL_YES  && m_motFlag[1] == CTL_YES)
			{
				m_motFlag[0] = CTL_NO;
				m_motFlag[1] = CTL_NO;
				mn_MoveStep = 1100;
			}
			else if (nRet_1 == BD_RETRY || nRet_2 == BD_RETRY)
			{
				mn_MoveStep = 1000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				m_nPickRetry_Cnt++;
				if( m_nPickRetry_Cnt > st_basic.n_count_retry )
				{
					m_nPickRetry_Cnt = 0;
					CTL_Lib.Alarm_Error_Occurrence(4401, dWARNING, alarm.mstr_code);
				}
				mn_MoveStep = 1000;
			}
			break;

		case 1100:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS], COMI.mn_runspeed_rate);
			nRet_2 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_motFlag[0] = CTL_YES;
			}
			if (nRet_2 == BD_GOOD) //좌측으로 이동
			{
				m_motFlag[1] = CTL_YES;
			}
			if(m_motFlag[0] == CTL_YES  && m_motFlag[1] == CTL_YES)
			{
				mn_MoveStep = 1200;
			}
			else if (nRet_1 == BD_RETRY || nRet_2 == BD_RETRY)
			{
				mn_MoveStep = 1100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				m_nPickRetry_Cnt++;
				if( m_nPickRetry_Cnt > st_basic.n_count_retry )
				{
					m_nPickRetry_Cnt = 0;
					CTL_Lib.Alarm_Error_Occurrence(4402, dWARNING, alarm.mstr_code);
				}
				mn_MoveStep = 100;
			}
			break;

		case 1200:
			m_motFlag[0] = CTL_NO;
			m_motFlag[1] = CTL_NO;
			nFuncRet = RET_GOOD;
			mn_MoveStep = 0;
			break;
	}
	return nFuncRet;
}

void CRun_UldPicker::Set_UnLoader_Transfer_Clamp_OnOff(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bClampOnOffFlag	= false;
	m_dwClampOnOff[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Transfer1_Clamp_Forward_Sol, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Transfer1_Clamp_Backward_Sol, !OnOff);

	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("FORWARD"),0,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("BACKWARD"),0,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

int CRun_UldPicker::Chk_UnLoader_Transfer_Clamp_OnOff( int nOnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_PICKER_CLAMP_ON_OFF;

	if (nOnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag == false &&	g_ioMgr.get_in_bit(st_io.i_Transfer1_Clamp_Backward_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Transfer1_Clamp_Forward_Check, IO_OFF)	== IO_OFF )
		{
			m_bClampOnOffFlag		= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Transfer1_Clamp_Backward_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Transfer1_Clamp_Forward_Check, IO_OFF)	== IO_OFF )
		{
			m_dwClampOnOff[1] = GetCurrentTime();
			m_dwClampOnOff[2] = m_dwClampOnOff[1] - m_dwClampOnOff[0];

			if (m_dwClampOnOff[2] <= 0)
			{
				m_dwClampOnOff[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("BACKWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwClampOnOff[1] = GetCurrentTime();
			m_dwClampOnOff[2] = m_dwClampOnOff[1] - m_dwClampOnOff[0];

			if (m_dwClampOnOff[2] <= 0)
			{
				m_dwClampOnOff[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Transfer1_Clamp_Backward_Check); 
				clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("BACKWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag == false && g_ioMgr.get_in_bit(st_io.i_Transfer1_Clamp_Backward_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Transfer1_Clamp_Forward_Check, IO_ON) == IO_ON )
		{
			m_bClampOnOffFlag			= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Transfer1_Clamp_Backward_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Transfer1_Clamp_Forward_Check, IO_ON)	== IO_ON )
		{
			m_dwClampOnOff[1]	= GetCurrentTime();
			m_dwClampOnOff[2]	= m_dwClampOnOff[1] - m_dwClampOnOff[0];

			if (m_dwClampOnOff[2] <= 0)
			{
				m_dwClampOnOff[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwClampOnOff[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("FORWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwClampOnOff[1]	= GetCurrentTime();
			m_dwClampOnOff[2]	= m_dwClampOnOff[1] - m_dwClampOnOff[0];

			if (m_dwClampOnOff[2] <= 0)
			{
				m_dwClampOnOff[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwClampOnOff[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Transfer1_Clamp_Forward_Check); 
				clsLog.LogFunction(_T("UNLOADER_PICKER_ROBOT"),_T("FORWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}
//kwlee 2017.0626
int CRun_UldPicker::GetMotorPosX(double dPos)
{
	int nPos = -1;
	
	if ( dPos >= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INIT_POS] - COMI.md_allow_value[m_nRobot_X] &&
		dPos <= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INIT_POS] + COMI.md_allow_value[m_nRobot_X])
	{
		nPos= P_UNLOADER_TRANSFER_X_INIT_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] - COMI.md_allow_value[m_nRobot_X] &&
		dPos <= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[m_nRobot_X])
	{
		nPos= P_UNLOADER_TRANSFER_X_INREADY_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] - COMI.md_allow_value[m_nRobot_X] &&
		dPos <= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + COMI.md_allow_value[m_nRobot_X])
	{
		nPos = P_UNLOADER_TRANSFER_X_READY_POS;
	}	
	else if ( dPos >= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_OUTREADY_POS] - COMI.md_allow_value[m_nRobot_X] &&
		dPos <= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_OUTREADY_POS] + COMI.md_allow_value[m_nRobot_X])
	{
		nPos= P_UNLOADER_TRANSFER_X_OUTREADY_POS;
	}	
	else if ( dPos >= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS] - COMI.md_allow_value[m_nRobot_X] &&
		dPos <= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_TOP_POS] + COMI.md_allow_value[m_nRobot_X])
	{
		nPos= P_UNLOADER_TRANSFER_X_PICK_TOP_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_MID_POS] - COMI.md_allow_value[m_nRobot_X] &&
		dPos <= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_MID_POS] + COMI.md_allow_value[m_nRobot_X])
	{
		nPos= P_UNLOADER_TRANSFER_X_PICK_MID_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_BOT_POS] - COMI.md_allow_value[m_nRobot_X] &&
		dPos <= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_PICK_BOT_POS] + COMI.md_allow_value[m_nRobot_X])
	{
		nPos= P_UNLOADER_TRANSFER_X_PICK_BOT_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_ZIGPLACE_POS] - COMI.md_allow_value[m_nRobot_X] &&
		dPos <= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_ZIGPLACE_POS] + COMI.md_allow_value[m_nRobot_X])
	{
		nPos= P_UNLOADER_TRANSFER_X_ZIGPLACE_POS;
	}
	
	return nPos;
}

int CRun_UldPicker::GetMotorPosY(double dPos)
{
	int nPos = -1;
	
	if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_INIT_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_UNLOADER_TRANSFER_Y_INIT_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_INREADY_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_INREADY_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_UNLOADER_TRANSFER_Y_INREADY_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_UNLOADER_TRANSFER_Y_READY_POS;
	}	
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_OUTREADY_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_OUTREADY_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_UNLOADER_TRANSFER_Y_OUTREADY_POS;
	}	
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_PICK_TOP_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_PICK_TOP_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_UNLOADER_TRANSFER_Y_PICK_TOP_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_PICK_MID_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_PICK_MID_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_UNLOADER_TRANSFER_Y_PICK_MID_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_PICK_BOT_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_PICK_BOT_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_UNLOADER_TRANSFER_Y_PICK_BOT_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_ZIGPLACE_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_UNLOADER_TRANSFER_Y_ZIGPLACE_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_UNLOADER_TRANSFER_Y_ZIGPLACE_POS;
	}
	
	return nPos;
}


int CRun_UldPicker::GetMotorPosZ(double dPos)
{
	int nPos = -1;
	
	if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_UNLOADER_TRANSFER_Z_INIT_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_PICKTOP_POS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_PICKTOP_POS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_UNLOADER_TRANSFER_Z_PICKTOP_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_PICKMID_POS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_PICKMID_POS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_UNLOADER_TRANSFER_Z_PICKMID_POS;
	}	
	else if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_PICKBOT_POS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_PICKBOT_POS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_UNLOADER_TRANSFER_Z_PICKBOT_POS;
	}	
	else if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_ZIGPLACE_POS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_ZIGPLACE_POS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_UNLOADER_TRANSFER_Z_ZIGPLACE_POS;
	}
	return nPos;
}

void CRun_UldPicker::Robot_BackupPos_Check()
{
	st_work.dReinstatement_pos[1][M_UNLOADER_TRANSFER_X] = st_work.dReinstatement_pos[0][M_UNLOADER_TRANSFER_X];
	st_work.dReinstatement_pos[1][M_UNLOADER_TRANSFER_Y] = st_work.dReinstatement_pos[0][M_UNLOADER_TRANSFER_Y];
	st_work.dReinstatement_pos[1][M_UNLOADER_TRANSFER_Z] = st_work.dReinstatement_pos[0][M_UNLOADER_TRANSFER_Z];
}

void CRun_UldPicker::OnUldPicker_FinalPos()
{
	if (st_work.nReinstatement_mode[ULD_PICKER] == 0)
	{	
		st_work.dReinstatement_pos[0][M_UNLOADER_TRANSFER_X] = COMI.md_cmdpos_backup[M_UNLOADER_TRANSFER_X];
		st_work.dReinstatement_pos[0][M_UNLOADER_TRANSFER_Y] = COMI.md_cmdpos_backup[M_UNLOADER_TRANSFER_Y];
		st_work.dReinstatement_pos[0][M_UNLOADER_TRANSFER_Z] = COMI.md_cmdpos_backup[M_UNLOADER_TRANSFER_Z];
		
		//kwlee 2017.0626
		st_work.nReinst_MotorPos[0][M_UNLOADER_TRANSFER_X] = GetMotorPosX(COMI.md_cmdpos_backup[M_UNLOADER_TRANSFER_X]);
		st_work.nReinst_MotorPos[0][M_UNLOADER_TRANSFER_Y] = GetMotorPosY(COMI.md_cmdpos_backup[M_UNLOADER_TRANSFER_Y]);
		st_work.nReinst_MotorPos[0][M_UNLOADER_TRANSFER_Z] = GetMotorPosZ(COMI.md_cmdpos_backup[M_UNLOADER_TRANSFER_Z]);
		
		st_work.nPickerClampState[0] = g_ioMgr.get_out_bit(st_io.o_Transfer1_Clamp_Forward_Sol, IO_OFF);
		st_work.nPickerClampState[1] = g_ioMgr.get_out_bit(st_io.o_Transfer1_Clamp_Backward_Sol, IO_ON);
		
		st_work.nRestState_LdUldSafety = st_handler.mn_lduld_safety;
		st_work.nReinstatement_mode[ULD_PICKER] = 1;
	}
}
//

void CRun_UldPicker::Run_ReinState()
{
	int i, nRet_1, nRet_2;
	double dTargetPos;

	if (st_work.nUld_Picker_ReinstateMent_Ok == YES) return;
	
	switch(mn_reinstate_step)
	{
	case 0:
		mn_reinstate_step = 10;
		break;
		
	case 10:
		if( st_handler.mn_lduld_safety == LD_MOVING_SAFETY)
		{
			m_dcurr_pos[0] = COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_X);
		//	m_dcurr_pos[1] = COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_Y);
		//	m_dcurr_pos[2] = COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_Z);
			
			if( m_dcurr_pos[0] <= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INIT_POS] &&  
				st_work.dReinstatement_pos[0][m_nRobot_X] <= st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INIT_POS])
			{
				mn_reinstate_step = 1000;
			}
			else
			{
				if (m_dcurr_pos[0] > st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INIT_POS]  + COMI.md_allow_value[m_nRobot_X])
				{
					//알람 
					alarm.mstr_code.Format("%02d0008",m_nRobot_X);
					CTL_Lib.Alarm_Error_Occurrence(1330, dWARNING, alarm.mstr_code);
					mn_reinstate_step = 20000;	
				}		
				else
				{
					//알람 
					alarm.mstr_code.Format("%02d0008",m_nRobot_X);
					CTL_Lib.Alarm_Error_Occurrence(1331, dWARNING, alarm.mstr_code);
					//if(st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y] > safety + allow )
					//알람 버그 reset  step저장
					mn_reinstate_step = 20000;
				}	
			}
		}
		else
		{
			if( m_dcurr_pos[0] > st_motor[m_nRobot_X].md_pos[P_UNLOADER_TRANSFER_X_INIT_POS] || 
				st_work.dReinstatement_pos[0][m_nRobot_X] > st_motor[m_nRobot_X].md_pos[P_LOADER_TRANSFER_Y_INIT_POS])
			{
				if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y )  > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_LOADER_TRANSFER_Y]))
				{
					//나올수 없는 상황 setep reset alarm
					alarm.mstr_code.Format("%02d0008",M_LOADER_TRANSFER_Y);
					CTL_Lib.Alarm_Error_Occurrence(1332, dWARNING, alarm.mstr_code);
					mn_reinstate_step = 20000;
				}
				else
				{
					mn_reinstate_step = 1000;
				}
			}
			else
			{
				mn_reinstate_step = 1000;
			}
		}
		break;
		
	case 1000:
		m_dcurr_pos[0] = COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_X);
		m_dcurr_pos[1] = COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_Y);
		m_dcurr_pos[2] = COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_Z);
		
		for (i = 0; i< 4; i++)
		{
			m_nChange[i] = 0;
		}
		if (m_dcurr_pos[0] > st_work.dReinstatement_pos[0][m_nRobot_X] +  COMI.md_allow_value[m_nRobot_X] ||
			m_dcurr_pos[0] < st_work.dReinstatement_pos[0][m_nRobot_X] - COMI.md_allow_value[m_nRobot_X])
		{
			m_nChange[0]++;
			sprintf(st_msg.c_abnormal_msg, "Uld Picker Robot X POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.dReinstatement_pos[0][m_nRobot_X], (long)m_dcurr_pos[0], (long)COMI.md_allow_value[m_nRobot_X]);	
		}

		if (m_dcurr_pos[1] > st_work.dReinstatement_pos[0][m_nRobot_Y] +  COMI.md_allow_value[m_nRobot_Y] ||
			m_dcurr_pos[1] < st_work.dReinstatement_pos[0][m_nRobot_Y] - COMI.md_allow_value[m_nRobot_Y])
		{
			m_nChange[1]++;
			sprintf(st_msg.c_abnormal_msg, "Uld Picker Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.dReinstatement_pos[0][m_nRobot_Y], (long)m_dcurr_pos[1], (long)COMI.md_allow_value[m_nRobot_Y]);	
		}
		
		if (m_dcurr_pos[2] > st_work.dReinstatement_pos[0][m_nRobot_Z] +  COMI.md_allow_value[m_nRobot_Z] ||
			m_dcurr_pos[2] < st_work.dReinstatement_pos[0][m_nRobot_Z] - COMI.md_allow_value[m_nRobot_Z])
		{
			m_nChange[2]++;
			sprintf(st_msg.c_abnormal_msg, "Uld Picker Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.dReinstatement_pos[0][m_nRobot_Z], (long)m_dcurr_pos[2], (long)COMI.md_allow_value[m_nRobot_Z]);
		}
		
		//피커 확인
		nRet_1 = g_ioMgr.get_in_bit( st_io.i_Transfer1_Clamp_Forward_Check, IO_ON);
		nRet_2 = g_ioMgr.get_in_bit( st_io.i_Transfer1_Clamp_Backward_Check, IO_OFF);
		
		if( nRet_1 == st_work.nPickerClampState[0] && nRet_2 == st_work.nPickerClampState[1])
		{
			m_nChange[3] = 1; 
		}
		
		if (m_nChange[0] > 0 || m_nChange[1] > 0 || m_nChange[2] > 0)
		{
			mn_reinstate_step = 2000; // 위치 몇 동작 변화.
		}
		else
		{
			mn_reinstate_step = 1100; // 위치 몇 동작 변화 없음.
		}
		break;
		

	case 1100:
		m_dwReinstate_time[0] = GetCurrentTime();
		mn_reinstate_step = 1200;
		break;
		
	case 1200:
		m_dwReinstate_time[1] = GetCurrentTime();
		m_dwReinstate_time[2] = m_dwReinstate_time[1] - m_dwReinstate_time[0];
		
		if (m_dwReinstate_time[2] < 0)
		{
			m_dwReinstate_time[0] = GetCurrentTime();
			break;
		}
		
		if (m_dwReinstate_time[2] >= 100)
		{
			//m_nChange[2] == 1 : Picker 동작 변화 없음.
			if( m_nChange[3] == 1)
				mn_reinstate_step = 5000;
			else
				mn_reinstate_step = 4800;
		}
		break;

	case 2000:
		Robot_BackupPos_Check();
		COMI.Set_MotPower(M_UNLOADER_TRANSFER_X, TRUE);
		COMI.Set_MotPower(M_UNLOADER_TRANSFER_Y, TRUE);
		COMI.Set_MotPower(M_UNLOADER_TRANSFER_Z, TRUE);
		mn_reinstate_step = 2100;
		break;
		
		case 2100:
		//z가 틀리면 Up후에 y 동작
		if( st_work.dReinstatement_pos[1][m_nRobot_X] != st_motor[m_nRobot_X].md_pos[st_work.nReinst_MotorPos[0][M_UNLOADER_TRANSFER_X]] || 
			st_work.dReinstatement_pos[1][m_nRobot_Y] != st_motor[m_nRobot_Y].md_pos[st_work.nReinst_MotorPos[0][M_UNLOADER_TRANSFER_Y]])
		{ 
			mn_reinstate_step = 2110;
		}
		else
		{
			mn_reinstate_step = 2300;
		}
		break;

		case 2110:
			//z축 home 후
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD)
			{
				mn_reinstate_step = 2300;
			}
			else if(nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1333, dWARNING, alarm.mstr_code);
				mn_reinstate_step = 20000;
			}
			break;

	case 2300:
		if( st_work.nReinst_MotorPos[0][m_nRobot_X] == -1 )
		{
			mn_reinstate_step = 2310;
		}
		else
		{
			mn_reinstate_step = 2320;
		}
		break;

	case 2310:
		dTargetPos = st_work.dReinstatement_pos[1][m_nRobot_X]; //이동 
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, dTargetPos, COMI.mn_runspeed_rate);
		
		if (nRet_1 == BD_GOOD)
		{
			//kwlee 2017.0626
			mn_reinstate_step = 2400;
		}
		else if (nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1334, dWARNING, alarm.mstr_code);
			mn_reinstate_step = 20000;
		}
		break;

	case 2320:
		dTargetPos = st_motor[m_nRobot_X].md_pos[st_work.nReinst_MotorPos[0][M_UNLOADER_TRANSFER_X]]; //이동
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, dTargetPos, COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD)
		{
			//kwlee 2017.0626
			mn_reinstate_step = 2400;
		}
		else if (nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1335, dWARNING, alarm.mstr_code);
			mn_reinstate_step = 20000;
		}
		break;

	case 2400:
		if( st_work.nReinst_MotorPos[0][m_nRobot_Y] == -1 )
		{
			mn_reinstate_step = 2410;
		}
		else
		{
			mn_reinstate_step = 2420;
		}
		break;

	case 2410:
		dTargetPos = st_work.dReinstatement_pos[1][m_nRobot_Y];//이동 
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, dTargetPos, COMI.mn_runspeed_rate);
		
		if (nRet_1 == BD_GOOD)
		{
			//kwlee 2017.0626
			mn_reinstate_step = 2500;
		}
		else if (nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1336, dWARNING, alarm.mstr_code);
			mn_reinstate_step = 20000;
		}
		break;
	
	case 2420:
		dTargetPos = st_motor[m_nRobot_Y].md_pos[st_work.nReinst_MotorPos[0][M_UNLOADER_TRANSFER_Y]]; //이동
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, dTargetPos, COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD)
		{
			//kwlee 2017.0626
			mn_reinstate_step = 2500;
		}
		else if (nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1337, dWARNING, alarm.mstr_code);
			mn_reinstate_step = 20000;
		}
		break;
	  // Z축 이동
	case 2500:
		if( st_work.nReinst_MotorPos[0][M_UNLOADER_TRANSFER_Z] == -1 )
		{
			mn_reinstate_step = 2510;
		}
		else
		{
			mn_reinstate_step = 2520;
		}
		break;

	case 2510:
		dTargetPos = st_work.dReinstatement_pos[1][M_UNLOADER_TRANSFER_Z]; //이동 
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, dTargetPos, COMI.mn_runspeed_rate);
		
		if (nRet_1 == BD_GOOD)
		{
			if( m_nChange[3] == 1)
			{
				mn_reinstate_step = 5000;
			}
			else
			{
				mn_reinstate_step = 4800;
			}
		}
		else if (nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1338, dWARNING, alarm.mstr_code);
			mn_reinstate_step = 20000;
		}
		break;

	case 2520:
		dTargetPos = st_motor[m_nRobot_Z].md_pos[st_work.nReinst_MotorPos[0][M_UNLOADER_TRANSFER_Z]]; //이동
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, dTargetPos, COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD)
		{
			if( m_nChange[3] == 1)
			{
				mn_reinstate_step = 5000;
			}
			else
			{
				mn_reinstate_step = 4800;
			}
		}
		else if (nRet_1 == BD_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1339, dWARNING, alarm.mstr_code);
			mn_reinstate_step = 20000;
		}
		break;

	case 4800:
		Set_UnLoader_Transfer_Clamp_OnOff(st_work.nPickerClampState[0]); 
		mn_reinstate_step = 4900;
		break;
		
	case 4900:
		//확인 
		nRet_1 = Chk_UnLoader_Transfer_Clamp_OnOff(st_work.nPickerClampState[0]);
			
		if (nRet_1 == RET_GOOD)
		{
			mn_reinstate_step = 5000;
		}
		else if (nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1340, dWARNING, m_strAlarmCode);
			mn_reinstate_step = 20000;
		}
		break;
			
	case 5000:
		st_work.nUld_Picker_ReinstateMent_Ready = YES;
		st_work.nUld_Picker_ReinstateMent_Ok = YES;
		mn_reinstate_step = 0;
		break;
		
	case 20000:
		st_handler.n_sync_reinstate = NO;
		mn_reinstate_step = 0;
		break;
	}
}