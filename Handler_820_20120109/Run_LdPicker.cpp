// Run_LdPicker.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_LdPicker.h"
#include "LogFromat.h"
#include "AMTLotManager.h"
#include "FastechPublic_IO.h"
#include "IO_Manager.h"
#include "CmmsdkDef.h"
#include "Run_EmptyTrayTransfer.h"
#include "Run_Device_Carrier_Robot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_LdPicker
CRun_LdPicker Run_LdPicker;
IMPLEMENT_SERIAL(CRun_LdPicker, CObject, 1);

CRun_LdPicker::CRun_LdPicker()
{
	m_nPickerPara = PICKER_PARA;//1
	m_nRobotSite  = THD_LOAD_WORK_RBT;

	m_nRobot_Y = M_LOADER_TRANSFER_Y;
	m_nRobot_Z = M_LOADER_TRANSFER_Z; 

	m_dwCycleTime[0][0] = 0;
	m_dwCycleTime[1][0] = 0;
	m_strLotNo[0] = m_strPartNo[0] = _T("");
	m_strLotNo[1] = m_strPartNo[1] = _T("");
	st_handler.mn_lduld_safety = MOVING_NOT_SAFETY;
}

CRun_LdPicker::~CRun_LdPicker()
{
}


/////////////////////////////////////////////////////////////////////////////
// CRun_LdPicker message handlers

void CRun_LdPicker::Thread_Run()
{
	int nRet_1;
	switch( st_work.mn_run_status)
	{
	case dINIT:
		RunInit();
		break;

	case dRUN:
		RunMove();
		break;
	
	case dSTOP:
		if(m_nRobot_Z_Stop_Flag == 0)
		{
			nRet_1 = COMI.Check_Motion_State(M_LOADER_TRANSFER_Z, cmMST_STOP);//20150427 nRet_1 = cmmSxIsDone(n_MotorNum, &dwMotionDone);
			if (nRet_1 != BD_GOOD) 
			{//모터 상태가 mMST_STOP 이다 
				if(CTL_Lib.mn_single_motmove_step[M_LOADER_TRANSFER_Z] > 0)
				{
					COMI.Set_MotStop(1, M_LOADER_TRANSFER_Z) ; //긴급정지 
					CTL_Lib.mn_single_motmove_step[M_LOADER_TRANSFER_Z] = 0;
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

		m_dwWaitTime[0] = GetCurrentTime();
		break;

	case dWARNING:
		break;

		//kwlee 2017.0628 복귀 동작 막아 놓음.
	case dREINSTATE:
		Run_ReinState();
		break;

	default:
		return; //2017.0628 복귀 동작 막아 놓음.
		if (st_work.mn_run_status == dSTOP)
		{
			if(m_nRobot_Z_Stop_Flag == 0)
			{
				nRet_1 = COMI.Check_Motion_State(M_LOADER_TRANSFER_Z, cmMST_STOP);//20150427 nRet_1 = cmmSxIsDone(n_MotorNum, &dwMotionDone);
				if (nRet_1 != BD_GOOD) 
				{//모터 상태가 mMST_STOP 이다 
					if(CTL_Lib.mn_single_motmove_step[M_LOADER_TRANSFER_Z] > 0)
					{
						COMI.Set_MotStop(1, M_LOADER_TRANSFER_Z) ; //긴급정지 
						CTL_Lib.mn_single_motmove_step[M_LOADER_TRANSFER_Z] = 0;
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
			
			m_dwWaitTime[0] = GetCurrentTime();
		}

		//kwlee 2017.0623
		if (st_work.mn_run_status != dRUN || st_work.mn_run_status != dREINSTATE)
		{
			OnLdPicker_FinalPos();
		}

		if (st_handler.mn_menu_num == 501) return;

		COMI.Set_MotStop(0, M_LOADER_TRANSFER_Z);
		COMI.Set_MotStop(0, M_LOADER_TRANSFER_Y);

		CTL_Lib.mn_single_motmove_step[M_LOADER_TRANSFER_Z] = 0;
		CTL_Lib.mn_single_motmove_step[M_LOADER_TRANSFER_Y] = 0;
		mn_InitStep = 0;
		mn_reinstate_step = 0;
		break;
	}
}

void CRun_LdPicker::RunInit()
{
	int i = 0,nRet_1=0, nSNum = 0;
		
	if( st_handler.mn_init_state[INIT_LD_ROBOT] != CTL_NO ) return;
// 	st_handler.mn_init_state[INIT_LD_ROBOT] = CTL_YES;
	switch(mn_InitStep)
	{
		case 0:
			mn_InitStep = 10;
			break;

		case 10:
			Run_EmptyTrayTransfer.Set_Tray_Remover_Z_UpDown( IO_ON );
			mn_InitStep = 20;
			break;
			
		case 20:
			nRet_1 = Run_EmptyTrayTransfer.Chk_Tray_Remover_Z_UpDown( IO_ON );
			if( nRet_1 == RET_GOOD )
			{
				mn_InitStep = 100;
			}
			else if( nRet_1 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1001, dWARNING, Run_EmptyTrayTransfer.m_strAlarmCode);
				mn_InitStep = 900;
			}
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
				CTL_Lib.Alarm_Error_Occurrence( 1002, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 200:
			if( Check_DVC_Model( MODEL_INIT, CTL_YES, &nSNum) == CTL_GOOD )
			{//error
				mn_InitStep = 900;
				mn_InitStep = 300;
// 				sprintf(mc_alarmcode,"8%d%04d", CTL_YES, st_io.i_loading_rbt_glipper_sff_missalign_chk_1 + nSNum);
// 				CTL_Lib.Alarm_Error_Occurrence( 1003, dWARNING, mc_alarmcode);				
			}
			else
			{
				mn_InitStep = 300;
			}
			break;

		case 300:
			Set_Loader_Transfer_Clamp_OnOff( IO_OFF );
			mn_InitStep = 400;
			break;

		case 400:
			nRet_1 = Chk_Loader_Transfer_Clamp_OnOff( IO_OFF );
			if( nRet_1 == RET_GOOD )
			{
				mn_InitStep = 500;
			}
			else if(nRet_1 == RET_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence(1003, dWARNING, m_strAlarmCode);
			}
			break;

		case 500:
			nRet_1 = COMI.HomeCheck_Mot( m_nRobot_Y, st_motor[m_nRobot_Y].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 600;
			}
			else if( nRet_1 == BD_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence( 1004, dWARNING, COMI.mc_alarmcode);
				mn_InitStep = 900;
			}
			break;

		case 600:
			if( g_ioMgr.get_in_bit( st_io.i_Loading_Tr_Jig_Detect_Check, IO_OFF) == IO_ON )
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loader_Transfer_Clamp_Off_Check); 
				CTL_Lib.Alarm_Error_Occurrence(1005, dWARNING, m_strAlarmCode);
				mn_InitStep = 900;
			}
			else
			{
				mn_InitStep = 700;
			}
			break;

		case 700:
			Set_Loader_Buffer_Align_OnOff(IO_OFF);
			mn_InitStep = 800;
			break;

		case 800:
			nRet_1 = Chk_Loader_Buffer_Align_OnOff(IO_OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_InitStep = 1000;
			}
			else if ( nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence( 1006, dWARNING, COMI.mc_alarmcode);
				mn_InitStep = 900;
			}
			break;

		case 900:
			mn_InitStep = 0;
			st_handler.mn_initial_error = TRUE;
			break;

		case 1000:
			st_handler.mn_init_state[INIT_LD_ROBOT] = CTL_YES;
			mn_InitStep = 0;
			break;


	}
}

void CRun_LdPicker::RunMove()
{
	int i = 0,nRet_1=0,nRet_2=0,nRet_3=0,nRet_4=0,nCount=0;
	long lMotionDone=0;

	Func.ThreadFunctionStepTrace(6, mn_RunStep);
	switch(mn_RunStep)
	{
		case -1:
			mn_RunStep = 0;
			break;

		case 0:		
			if( g_lotMgr.GetLotCount() > 0 )
			{
				mn_RunStep = 20;
			}
			break;

// 			st_io.i_loading_rbt_glipper_sff_missalign_chk_1 =	2400;
// 			st_io.i_loading_rbt_glipper_sff_missalign_chk_2 =	2401;
// 			st_io.i_loading_rbt_glipper_sff_missalign_chk_3 =	2402;
// 			st_io.i_loading_rbt_glipper_sff_missalign_chk_4 =	2403;
// 			
// 			st_io.i_loading_rbt_glipper_tff_missalign_chk_1 =	2404;
// 			st_io.i_loading_rbt_glipper_tff_missalign_chk_2 =	2405;
// 			st_io.i_loading_rbt_glipper_tff_missalign_chk_3 =	2406;
// 			st_io.i_loading_rbt_glipper_tff_missalign_chk_4 =	2407;

		case 10:
			nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_1, IO_OFF);
			nRet_2 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_2, IO_OFF);
			nRet_3 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_3, IO_OFF);
			nRet_4 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_4, IO_OFF);
			if( nRet_1 == IO_OFF && nRet_2 == IO_OFF && nRet_3 == IO_OFF && nRet_4 == IO_OFF )
			{
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_1, IO_OFF);
				nRet_2 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_2, IO_OFF);
				nRet_3 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_3, IO_OFF);
				nRet_4 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_4, IO_OFF);
				if( nRet_1 == IO_OFF && nRet_2 == IO_OFF && nRet_3 == IO_OFF && nRet_4 == IO_OFF )
				{
					mn_RunStep = 20;
				}
				else
				{
					if( nRet_1 == IO_ON)
						m_strAlarmCode.Format("8%d%04d", st_io.i_loading_rbt_glipper_tff_missalign_chk_1);
					else if( nRet_2 == IO_ON )
						m_strAlarmCode.Format("8%d%04d", st_io.i_loading_rbt_glipper_tff_missalign_chk_2);
					else if( nRet_3 == IO_ON )
						m_strAlarmCode.Format("8%d%04d", st_io.i_loading_rbt_glipper_tff_missalign_chk_3);
					else// if( nRet_4 == IO_ON )
						m_strAlarmCode.Format("8%d%04d", st_io.i_loading_rbt_glipper_tff_missalign_chk_4);
					CTL_Lib.Alarm_Error_Occurrence( 1301, dWARNING, m_strAlarmCode);
				}
			}
			else
			{
				if( nRet_1 == IO_ON)
					m_strAlarmCode.Format("8%d%04d", st_io.i_loading_rbt_glipper_sff_missalign_chk_1);
				else if( nRet_2 == IO_ON )
					m_strAlarmCode.Format("8%d%04d", st_io.i_loading_rbt_glipper_sff_missalign_chk_2);
				else if( nRet_3 == IO_ON )
					m_strAlarmCode.Format("8%d%04d", st_io.i_loading_rbt_glipper_sff_missalign_chk_3);
				else// if( nRet_4 == IO_ON )
					m_strAlarmCode.Format("8%d%04d", st_io.i_loading_rbt_glipper_sff_missalign_chk_4);
				CTL_Lib.Alarm_Error_Occurrence( 1302, dWARNING, m_strAlarmCode);
			}
			break;

		case 20:
			Set_Loader_Transfer_Clamp_OnOff( IO_OFF);
			mn_RunStep = 30;
			break;
			
		case 30:
			nRet_1 = Chk_Loader_Transfer_Clamp_OnOff( IO_OFF );
			if(nRet_1 == RET_GOOD)
			{
				mn_RunStep = 100;
			}
			else if( nRet_1 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1303, dWARNING, m_strAlarmCode);
			}
			break;

		case 100:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_OFF);
			nRet_2 = g_ioMgr.get_out_bit(st_io.o_Loader_Transfer_Clamp_On_Sol,	 IO_OFF);
			nRet_3 = g_ioMgr.get_out_bit(st_io.o_Loader_Transfer_Clamp_Off_Sol, IO_ON);
			if(nRet_1 == IO_OFF || nRet_2 == IO_ON || nRet_3 == IO_OFF) 
			{
				//810113 0 A "LOADER_PICKER_CLAMP_ON_ERROR."
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loader_Transfer_Clamp_Off_Check); 
				CTL_Lib.Alarm_Error_Occurrence(1100, dWARNING, m_strAlarmCode);					
			}
			else
			{
				mn_RunStep = 200;				 
			}
			break;

		case 200:
			Set_Loader_Transfer_Clamp_OnOff( IO_OFF );
			mn_RunStep = 210;
			break;

		case 210:
			nRet_1 = Chk_Loader_Transfer_Clamp_OnOff( IO_OFF ); 			 
			if(nRet_1 == RET_GOOD)
			{
				mn_RunStep = 300;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1101, dWARNING, m_strAlarmCode);
				mn_RunStep = 100;
			}
			break;

		case 300:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunStep = 300;
			}
			break;

		case 400:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 500;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				mn_RunStep = 400;
			}
			break;

		case 500:
			if( g_lotMgr.GetLotCount() > 0 )
			{
				int num = g_lotMgr.GetLotAt(0).GetInputCnt(PRIME);
// 				g_lotMgr.GetLotAt(0).AddInputCnt(PRIME);
				int aa = g_lotMgr.GetLotAt(0).GetTotLotCount();
				if( st_sync.nLotEndFlag[0][THD_LOAD_WORK_RBT] != LOTEND && g_lotMgr.GetLotAt(0).GetTotLotCount() > 0 && 
					( g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() ) )
				{
					//load plate에 자재 요청
// 					st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
// 					st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK;
					if(st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] == CTL_READY )
					{
						st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ;
						st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PLACE;
						m_nFindLotNo_Flag = 0;						
						mn_RunStep = 1000;
					}
				}
				else if( g_lotMgr.GetLotCount() >= 2 )
				{
					if( st_sync.nLotEndFlag[1][THD_LOAD_WORK_RBT] != LOTEND && g_lotMgr.GetLotAt(1).GetTotLotCount() > 0 && 
						g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
					{
// 						st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
// 						st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][1] = WORK_PICK;

						if(st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] == CTL_READY )
						{
							st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_REQ;
							st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] = WORK_PLACE;
							m_nFindLotNo_Flag = 1;
							mn_RunStep = 1000;
						}
					}
					else
					{

// 						nRet_1 = COMI._cmmSxIsDone(m_nRobot_Y, &lMotionDone); //모션 동작 완료 == 1, 동작중은 0
// 						if(nRet_1 == cmERR_NONE &&  lMotionDone != cmTRUE)  //모터가 이동 중이면 안전 위치로 이동중으로 속도을 위해 최종 target pos 까지 바로 가게 위치 overide를 사용한다 
// 						{
// 							mn_override_flag = CTL_NO;	lp_OvrrideFlag = 0; 
// 							nRet_3 = cmmOverrideMoveTo(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], &lp_OvrrideFlag);
// 							st_motor[m_nRobot_Y].d_cmdpos_backup = st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_PICK_POS];//복구동작을 위해 이 부분에 내용 추가 
// 							if(nRet_3 == cmERR_NONE && lp_OvrrideFlag == 0) //정상적으로 override
// 							{
// 									mn_override_flag = CTL_YES
// 							}
// 							if(mn_override_flag == CTL_YES)
// 							{
// 								mn_RunStep= 500; 
// 								break;
// 							}
						if( COMI.Get_MotCurrentPos(m_nRobot_Y) >= ( st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[m_nRobot_Y] ) )
						{
							nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
							if (nRet_1 == BD_GOOD)
							{
								mn_RunStep = 500;
							}
							else if (nRet_1 == BD_RETRY)
							{
								mn_RunStep = 500;
							}
							else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
							{
								CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
								mn_RunStep = 500;
							}
						}					

						return;
					}
				}
				else
				{
					if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_REQ && 
						st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] == WORK_PLACE )
					{
						st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_READY;
					}
					else if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_READY && 
						st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] == WORK_PLACE )
					{
						st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_FREE; 
						st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = CTL_FREE;
					}

				}
			}
			else
			{
				if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_REQ && 
					st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] == WORK_PLACE )
				{
					st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_READY;
				}
				else if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_READY && 
					st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] == WORK_PLACE )
				{
					st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_FREE; 
					st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = CTL_FREE;
				}
			}
			break;


		case 1000:
			m_nFindLotNo_Flag = -1;
			m_strFindLotNo = "";
			if( g_lotMgr.GetLotCount() > 0 )
			{
				if( g_lotMgr.GetLotAt(0).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(0).GetInputCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
				{
					m_nFindLotNo_Flag = 0;
					m_strFindLotNo = g_lotMgr.GetLotAt(0).GetLotID();
				}
				else if( g_lotMgr.GetLotCount() >= 2 )
				{
					if( g_lotMgr.GetLotAt(1).GetTotLotCount() > 0 && g_lotMgr.GetLotAt(1).GetInputCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
					{
						m_nFindLotNo_Flag = 1;
						m_strFindLotNo = g_lotMgr.GetLotAt(1).GetLotID();
					}
					else
					{
						return;
					}
				}
				else
				{
					return;
				}

				//Carrier가 Unload 제품이 발생하여 unload하라고 Unload picker에게 요청했을때 load 피커에게 알려준다.
				if( st_sync.nUldWorkRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_REQ)//There is PCB to unload.
				{
					mn_RunStep = 1100;
				}
				else
				{
					//나올수 있는 조건이 아니다.
// 					if( m_nFindLotNo_Flag <= -1 )
// 					{
// 						if( g_lotMgr.GetLotCount() > 0 )
// 						{
// 							if( g_lotMgr.GetLotAt(0).GetStrLastUnloadModule() == "YES" )
// 							{
// 								g_lotMgr.DeleteLot( 0 );
// 								mn_RunStep = 0;
// 							}
// 						}
// 						return;
// 					}

					int nCount = 0;
					for(i = 0; i < st_recipe.nLdBuffer_Num; i++)
					{
						if(st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[i].nYesNo == CTL_YES)
						{
							nCount++;			
						} 
					}
					if(nCount > 0)
					{
						if( st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_READY )
						{
							//m_nLoadBuff_Job_Continue_Flag = CTL_NO; //clear

							mn_RunStep = 3000;
							break;
						}
					}

					if(st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] == CTL_READY)
					{
						if(m_strFindLotNo != st_tray_info[THD_LD_STACKER].strLotNo)
						{						
							//메세지 출력 
							break;
						}
						if( st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] != CTL_READY ) break;
						mn_RunStep = 2000;
					}
				}
			}
			else
			{
				mn_RunStep = 0;
			}
			break;
		
		case 1100:
			if( COMI.Get_MotCurrentPos(m_nRobot_Y) <= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS] )
			{
				mn_RunStep = 1400;
			}
			else
			{
				nRet_1 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], COMI.md_allow_value[m_nRobot_Y]);
				nRet_2 = COMI.Check_MotPosRange(m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.md_allow_value[m_nRobot_Z]);
				if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
				{
					mn_RunStep = 1200;
				}
				else
				{
					mn_RunStep = 1000;
				}
			}
			break;

		case 1200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 1300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 1200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1105, dWARNING, alarm.mstr_code);
				mn_RunStep = 1200;
			}
			break;

		case 1300:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 1400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 1300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1106, dWARNING, alarm.mstr_code);
				mn_RunStep = 1300;
			}
			break;

		case 1400:
			st_sync.nUldWorkRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_READY;
			mn_RunStep = 1500;
			break;

		case 1500:
			if(st_sync.nUldWorkRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_FREE)
			{
				mn_RunStep = 1000;
			}
			else if(st_sync.nUldWorkRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_REQ)
			{
				mn_RunStep = 1000;
			}
			break;

		case 2000://tray pick up
			nRet_1 = Process_DVC_Pickup( 0, THD_LD_TRAY_PLATE, m_strFindLotNo );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunStep = 2100;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				mn_RunStep = 2100;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				mn_RunStep = 2100;
			}
			break;

		case 2100:
			if( m_strFindLotNo == st_tray_info[THD_LD_STACKER].strLotNo )
			{
				st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] = CTL_REQ;
				mn_RunStep = 2200;
			}
			else
			{
				//900100 1 00 "IT_IS DIFFERENT_TO_LOT_NAME_BETWEEN_LOT_LOAD_STACKER."
				CTL_Lib.Alarm_Error_Occurrence(1107, dWARNING, "900100");
			}
			break;

		case 2200:
			if( st_picker[m_nRobotSite].st_pcb_info[0].nYesNo == CTL_NO )
			{
				mn_RunStep = 1000;
			}
			else
			{
				mn_RunStep = 3000;
			}
			break;

		case 3000:
			if( st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_READY && 
				st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] == WORK_PLACE )
			{
				mn_RunStep = 3100;
			}
			break;

		case 3100:
			nRet_1 = Process_DVC_Place( 0, THD_LD_ALIGN_BUFF, m_strFindLotNo );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunStep = 3200;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				mn_RunStep = 3100;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				mn_RunStep = 3100;
			}
			break;
		
		case 3200:
			nCount = 0;
			for(i = 0; i < PICKER_PARA; i++)
			{					 
				if(st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].nYesNo == CTL_YES)
				{
					m_strLotNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strLotNo;
					m_strPartNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strPartNo;
					nCount++;
				} 
			}
			if(nCount > 0) //자재가 남아있으면 
			{
				mn_RunStep = 3100;   
			}
			else
			{
				mn_RunStep = 3300;   
			}
			break;

		case 3300:
			nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, THD_LD_ALIGN_BUFF, CTL_NO, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, m_strFindLotNo );		 			
			if(nRet_1 == RET_SKIP) //더이상 작업할 공간이 없다 
			{	
				st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_CHANGE; //버퍼에 위치를 잡도록 함(실린더 동작)


				if( st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].nYesNo == CTL_YES)
				{
					m_strPartNo[0] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].strLotNo;
					m_strPartNo[0] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].strPartNo;
				}

				mn_RunStep = 3400;

				////////////////////////////////////////////////////////////////////////////////
				if(m_npFindWorkPosYXCPB[2] == 0)
				{//버퍼에는 더이상 놓은 공간이 없다
					m_dwCycleTime[0][1] = GetTickCount();
					m_dwCycleTime[0][2] = m_dwCycleTime[0][1] - m_dwCycleTime[0][0];

					if (m_dwCycleTime[0][2] <= (DWORD)0)
					{
						m_dwCycleTime[0][0] = GetTickCount();
					}
					else
					{
						if (m_dwCycleTime[0][2] > (DWORD)0)
						{
							if( m_strLotNo[0] != _T("") && m_strPartNo[0] != _T("") )
							{
								Func.OnCycleTime(0, 
									m_strLotNo[0],
									m_strPartNo[0],
									m_dwCycleTime[0][2], 
									0, 
									0);
							}

							m_strLotNo[0] = m_strPartNo[0] = _T("");
						}
						m_dwCycleTime[0][0] = GetTickCount();
					}
				}
				////////////////////////////////////////////////////////////////////////////////

			}
			else
			{
				mn_RunStep = 3400;
			}
			break;


// 			if(st_sync.nLdWorkRbt_Dvc_Req[THD_LD_TRAY_PLATE][0] != CTL_READY)
// 			{
// 				//2016.1228
// 				if( st_handler_info.mn_NoInput_test == CTL_YES)//투입할 자재는 있지만 작업자가 아직 투입을 않하고 있다.일단, 투입한 걸로 동작하자.
// 				{
// 					st_sync.nWorkRbt_Dvc_Req[THD_LD_BUFF][0] = CTL_READY; //더이상 버퍼에 놓을 자재가 없으니 갖고가라 
// 					m_nLoadBuff_Job_Continue_Flag = CTL_NO; //clear
// 					m_nRunStep = 2700; 
// 				}
// 				break;
// 			}
// 
// 			nRet_1 = Process_DVC_Pick(0, THD_LD_TRAY_PLATE);
// 			if(nRet_1 == RET_GOOD)
// 			{				
// 				m_nRunStep = 2700;
// 			}
// 			else if(nRet_1 == RET_PICKER_NOT_FIND)
// 			{
// 				m_nRunStep = 2700;			
// 			}
// 			else if(nRet_1 == RET_TRAY_NOT_FIND) //트레이에는 더 이상 작업할 수 있는 내용이 없다
// 			{
// 				m_nRunStep = 2700;
// 			}
// 			break;

			//////////////////////////////////////////////////////////////
			//LD ALIGN BUFF에서 제대로 안착이 됐는지 확인한다.
			//////////////////////////////////////////////////////////////
		case 3400:
			if( st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] == CTL_REQ && st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][1] == WORK_PICK )
			{
				mn_RunStep = 4000;
			}
			break;

		case 4000:
			nRet_1 = Process_DVC_Pickup( 0, THD_LD_ALIGN_BUFF, m_strFindLotNo );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunStep = 4100;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				mn_RunStep = 4000;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				mn_RunStep = 4000;
			}
			break;

		case 4100:
			nCount = 0;
			for(i = 0; i < PICKER_PARA; i++)
			{					 
				if(st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].nYesNo == CTL_YES)
				{
					m_strLotNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strLotNo;
					m_strPartNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strPartNo;
					nCount++;
				} 
			}
			if(nCount > 0) //자재를 픽업하면
			{
				mn_RunStep = 4200;   
			}
			else
			{
				mn_RunStep = 4000;   
			}
			break;

		case 4200:
			if( g_lotMgr.GetLotAt(0).GetDvcType() == "SFF")
			{

				nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_OFF, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
				if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
				{
					break; 
				} 
				else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
				{
					m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loading_Tr_Jig_Detect_Check);
					CTL_Lib.Alarm_Error_Occurrence(1108, dWARNING, m_strAlarmCode);
					break;
				}
			}
			else
			{
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_buffer_tff_tilt_chk, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit( st_io.i_loading_buffer_sff_tilt_chk, IO_ON);
				if( nRet_1 == IO_OFF && nRet_2 == IO_OFF && st_basic.n_mode_device != WITHOUT_DVC)
				{
				}
				else if( (nRet_1 == IO_ON || nRet_2 == IO_ON) && st_basic.n_mode_device != WITHOUT_DVC)
				{
					if( nRet_1 == IO_ON ) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_loading_buffer_tff_tilt_chk); 
					else				  m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_loading_buffer_sff_tilt_chk); 
					CTL_Lib.Alarm_Error_Occurrence(1188, dWARNING, m_strAlarmCode);
				}

			}

			mn_RunStep = 4300;
			break;

		case 4300:
			//디바이스 픽업 센서체크함
			if( g_lotMgr.GetLotAt(0).GetDvcType() == "SFF")
			{

				nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_OFF, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
				if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
				{
					break; 
				} 
				else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
				{
					m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loading_Tr_Jig_Detect_Check);
					CTL_Lib.Alarm_Error_Occurrence(1109, dWARNING, m_strAlarmCode);
					break;
				}
			}
			else
			{
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_buffer_tff_tilt_chk, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit( st_io.i_loading_buffer_sff_tilt_chk, IO_ON);
				if( nRet_1 == IO_OFF && nRet_2 == IO_OFF && st_basic.n_mode_device != WITHOUT_DVC)
				{
				}
				else if( (nRet_1 == IO_ON || nRet_2 == IO_ON) && st_basic.n_mode_device != WITHOUT_DVC)
				{
					if( nRet_1 == IO_ON ) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_loading_buffer_tff_tilt_chk); 
					else				  m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_loading_buffer_sff_tilt_chk); 
					CTL_Lib.Alarm_Error_Occurrence(1189, dWARNING, m_strAlarmCode);
				}
				
			}
			mn_RunStep = 4400;
			break;

		case 4400:
			st_sync.nLdWorkRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_CHANGE;
			mn_RunStep = 5000;

			break;

		////////////////////////////////
		//Carrier Buffer Place
		////////////////////////////////
		case 5000:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_REQ && 
				st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] == WORK_PLACE )
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_READY;
				mn_RunStep = 5100;
			}
			if( st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] == CTL_READY && 
				st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] == WORK_PLACE )
			{
				mn_RunStep = 5100;
			}
			else
			{
				//2017.0614
				///////////////////////////////////////////////////////////////////////////
// 				if( st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][0] == CTL_READY &&
// 					st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][1] == WORK_PICK )
// 				{
// 					if( st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][ 2 + 0] == CTL_SORT)
// 					{
// 						st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][ 2 + 0] = CTL_NO;
// 						mn_RunStep = 5100;
// 					}
// 					else if( st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][ 2 + 1] == CTL_SORT)
// 					{
// 						st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][ 2 + 1] = CTL_NO;
// 						mn_RunStep = 5100;
// 					}
// 					else if( st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][ 2 + 2] == CTL_SORT)
// 					{
// 						st_sync.nCarrierRbt_Dvc_Req[THD_UNLOAD_WORK_RBT][ 2 + 2] = CTL_NO;
// 						mn_RunStep = 5100;
// 					}
// 				}
				///////////////////////////////////////////////////////////////////////////
			}
			break;

		case 5100:
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X )  > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + COMI.md_allow_value[M_UNLOADER_TRANSFER_X] ) )
			{
				break;
			}
			if( st_handler.mn_lduld_safety == ULD_MOVING_SAFETY)
			{
				break;
			}
			st_handler.mn_lduld_safety = LD_MOVING_SAFETY;
			mn_RunStep = 5200;
			break;


		case 5200:
			nRet_1 = Process_DVC_Place( 0, THD_LDULD_CARRIER_BUFF, m_strFindLotNo );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunStep = 5300;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				mn_RunStep = 5200;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				mn_RunStep = 5200;
			}
			break;

		case 5300:
			nCount = 0;
			for(i = 0; i < PICKER_PARA; i++)
			{					 
				if(st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].nYesNo == CTL_YES)
				{
					m_strLotNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strLotNo;
					m_strPartNo[0] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strPartNo;
					nCount++;
				} 
			}
			if(nCount > 0) //자재가 남아있으면 
			{
				mn_RunStep = 5200;   
			}
			else
			{
				mn_RunStep = 5400;   
			}
			break;

		case 5400:
			//nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, THD_LDULD_CARRIER_BUFF, CTL_YES, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, m_strFindLotNo );	
			//if(nRet_1 == RET_SKIP) //더이상 작업할 공간이 없다 
				
// 			if( st_buffer_info[THD_LDULD_CARRIER_BUFF].st_pcb_info[m_n_FirstTray_Y_Num].nYesNo == CTL_YES && (st_buffer_info[THD_LDULD_CARRIER_BUFF].st_pcb_info[m_n_FirstTray_Y_Num].strLotNo == m_strFindLotNo) )
// 			{
			if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[mn_override_flag] == CTL_YES )// && (st_buffer_info[THD_LDULD_CARRIER_BUFF].st_pcb_info[m_n_FirstTray_Y_Num].strLotNo == m_strFindLotNo) )
			{			
				st_sync.nCarrierRbt_Dvc_Req[THD_LD_ALIGN_BUFF][0] = CTL_READY;

				if( st_handler.cwnd_main != NULL )
				{
					st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_LOADER_RECEIVE, 0);
				}

				//Carrier
				//Carrier site 의 버퍼 위치 
				//nPos
				int nPos = 0;
				if( st_buffer_info[THD_LDULD_CARRIER_BUFF].st_pcb_info[nPos].nYesNo == CTL_YES)
				{
					m_strPartNo[0] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].strLotNo;
					m_strPartNo[0] = st_buffer_info[THD_LD_ALIGN_BUFF].st_pcb_info[0].strPartNo;
				}
				mn_RunStep = 5500;
				////////////////////////////////////////////////////////////////////////////////
				if(m_npFindWorkPosYXCPB[2] == 0)
				{//버퍼에는 더이상 놓은 공간이 없다
					m_dwCycleTime[0][1] = GetTickCount();
					m_dwCycleTime[0][2] = m_dwCycleTime[0][1] - m_dwCycleTime[0][0];

					if (m_dwCycleTime[0][2] <= (DWORD)0)
					{
						m_dwCycleTime[0][0] = GetTickCount();
					}
					else
					{
						if (m_dwCycleTime[0][2] > (DWORD)0)
						{
							if( m_strLotNo[0] != _T("") && m_strPartNo[0] != _T("") )
							{
								Func.OnCycleTime(0, 
									m_strLotNo[0],
									m_strPartNo[0],
									m_dwCycleTime[0][2], 
									0, 
									0);
							}

							m_strLotNo[0] = m_strPartNo[0] = _T("");
						}
						m_dwCycleTime[0][0] = GetTickCount();
					}
				}
				////////////////////////////////////////////////////////////////////////////////

			}
			else
			{
				mn_RunStep = 5500;
			}
			break;

		case 5500:
			//높이 체크
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
			if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
			{
				break; 
			} 
			else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loader_Transfer_Clamp_Off_Check);
				CTL_Lib.Alarm_Error_Occurrence(1110, dWARNING, m_strAlarmCode);
				break;
			}

			mn_RunStep = 5600;
			break;

		case 5510:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 5600;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 5510;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1111, dWARNING, alarm.mstr_code);
				mn_RunStep = 5510;
			}
			break;

		case 5600:
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.md_allow_value[m_nRobot_Z]);
			if(nRet_1 != BD_GOOD )
			{
				mn_RunStep = 5510;
			}
			else
			{
				mn_RunStep = 5700;
			}
			break;

		case 5700:
			//nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			nRet_1 = COMI.Start_SingleMove(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 5800;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 5700;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1112, dWARNING, alarm.mstr_code);
				mn_RunStep = 5700;
			}
			break;


		case 5800:
// 			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			nRet_1 = COMI.Check_SingleMove( m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] );
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				//2017.0614
				if( st_handler.mn_lduld_safety == LD_MOVING_SAFETY)
				{
					st_handler.mn_lduld_safety = MOVING_NOT_SAFETY;
				}
				mn_RunStep = 6000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 5700;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1112, dWARNING, alarm.mstr_code);
				mn_RunStep = 5700;
			}
			break;

		case 6000:
			if( ( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[0] == CTL_YES && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[0] == BIN_CDIMM ) &&
				( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[1] == CTL_YES && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[1] == BIN_CDIMM ) &&
				( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[2] == CTL_YES && st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[2] == BIN_CDIMM ) )
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][0] = CTL_FREE; 
				st_sync.nCarrierRbt_Dvc_Req[THD_LOAD_WORK_RBT][1] = CTL_FREE;
			}
			if( st_handler.cwnd_main != NULL )
			{
				st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_LOADER_RECEIVE, 0);
			}

			if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetStrLastModule() == "YES" )
			{
				st_sync.nLotEndFlag[m_nFindLotNo_Flag][THD_LOAD_WORK_RBT] = LOTEND;
			}

			mn_RunStep = 500;
			break;



		default:
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_LdPicker  = %d", mn_RunStep);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			break;

	}
}

//Tray LdBuffer Carrier_Buffer
int CRun_LdPicker::Process_DVC_Place(int nMode, int nWork_Site/*, int nPosition*/, CString strLotNo)//WORK_TOP WORK_MID WORK_BTM
{
	int nFuncRet = RET_PROCEED;
	int nRet_1,nRet_2,nRet_3,nRet_4,i,nFlag;

	Func.ThreadFunctionStepTrace(7, mn_Place_Step);

	switch(mn_Place_Step)
	{
		case 0:
			m_bDvcWaitChk_Falg =  false;
			m_nPickRetry_Cnt = 0;
			mn_Place_Step = 100;
			break;

		case 100:
			m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[m_nRobot_Z] > st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벗어나 있으면 저위치 시킨다
			{
				mn_Place_Step = 200;
			}
			else
			{
				mn_Place_Step = 1000;
			}
			break;

		case 200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
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
				CTL_Lib.Alarm_Error_Occurrence(1200, dWARNING, alarm.mstr_code);
				mn_Place_Step = 200;
			}
			break;
			
		case 1000:
			nRet_1 = Func.Check_PickerStatus(0, THD_LOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus); //전역 변수와 센서 상태 체크 
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
					CTL_Lib.Alarm_Error_Occurrence(1201, dWARNING, Func.m_strAlarmCode);
					break;
				}
			}
			m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벅어나 있으면 저위치 시킨다
			{
				mn_Place_Step = 200;
				break;
			}
			m_n_FirstTray_Y_Num = -1; m_n_FirstTray_X_Num = -1; m_n_FirstPicker_Num = -1; //flag clear
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

			nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, nWork_Site, CTL_NO, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, m_strFindLotNo); //놓을때버퍼에서으 ㅣ작업순서는 뒤부터 놓는다 (4 -> 1 순서) 
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

					if( nWork_Site == THD_LD_ALIGN_BUFF /*|| nWork_Site == THD_LDULD_CARRIER_BUFF*/ )
					{
						if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_YES)// && st_picker[m_nRobotSite].st_pcb_info[i].nEnable == CTL_YES)
						{
							if(i >= 0 && m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num) < st_recipe.nLdBuffer_Num)
							{
								if(st_buffer_info[nWork_Site].st_pcb_info[ m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)].nYesNo == CTL_NO)
								{
									m_npPicker_YesNo[i] = CTL_YES;
									nFlag++;
								}
							}
						} 
					}
					else if(/*nWork_Site == THD_LD_ALIGN_BUFF ||*/ nWork_Site == THD_LDULD_CARRIER_BUFF)
					{
						if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_YES)
						{
							if(i >= 0 && ( ( m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)) < 3 ) )
							{
								if(st_buffer_info[nWork_Site].st_pcb_info[m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)].nYesNo == CTL_NO )
									//&& st_buffer_info[nWork_Site].st_pcb_info[m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)]->nEnable == CTL_YES) // 
								{//버퍼에 자재가 없고, 사용 가능한 위치일때만  자재를 놓을수 있게 한다.
									m_npPicker_YesNo[i] = CTL_YES;
									nFlag++;
								}
							}
						}
					}
				}

				if(nFlag == 0) //작업할 피커가 하나도 준비되 않았다.
				{ //이부분이 아오면 안됨 
					mn_Place_Step = 0;
					nFuncRet = RET_PICKER_NOT_FIND;//피커에 더이상 작업할 것이 없다
					break;
				}

				//실제 작업 위치를 셋팅
				m_npFindWorkPosYXCPB[0] = m_n_FirstTray_X_Num; m_npFindWorkPosYXCPB[1] = m_n_FirstTray_Y_Num; m_npFindWorkPosYXCPB[3] = m_n_FirstPicker_Num;

				m_nMove_Flag[0] = CTL_NO; m_nMove_Flag[1] = CTL_NO;
				mn_Place_Step = 2000;
			}
			break;

		case 2000:
			m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벗어나 있으면 정위치 시킨다
			{
				mn_Place_Step = 200;
				break;
			}

			if(nWork_Site == THD_LD_ALIGN_BUFF)
			{
				nRet_1 = Func.Check_Robot_Buffer_Clash( THD_LOAD_WORK_RBT, THD_LD_ALIGN_BUFF, m_n_FirstTray_Y_Num, m_n_FirstPicker_Num, m_nClash_ErrorInfoStatus); //피커와 버퍼등의 자재 충돌 확인 
				if(nRet_1 == RET_ERROR)
				{
					CTL_Lib.Alarm_Error_Occurrence(1202, dWARNING, Func.m_strAlarmCode);		
					break;
				}
			}
			else if(nWork_Site == THD_LDULD_CARRIER_BUFF)
			{
				nRet_1 = Func.Check_Robot_Buffer_Clash( THD_LOAD_WORK_RBT, THD_LDULD_CARRIER_BUFF, m_n_FirstTray_Y_Num, m_n_FirstPicker_Num, m_nClash_ErrorInfoStatus); //피커와 버퍼등의 자재 충돌 확인 
				if(nRet_1 == RET_ERROR)
				{
					CTL_Lib.Alarm_Error_Occurrence(1203, dWARNING, Func.m_strAlarmCode);		
					break;
				}
			}

	//		nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWork_Site, WORK_PLACE, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);             //Calculate_MovePos_Find(int nMotNum, int nSite, int nWorkPart, int *npFirstYXP_Pos, double *dpGetTargetPos)
			
			if(nWork_Site == THD_LDULD_CARRIER_BUFF)
				nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWork_Site, m_npFindWorkPosYXCPB[1], m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);             //Calculate_MovePos_Find(int nMotNum, int nSite, int nWorkPart, int *npFirstYXP_Pos, double *dpGetTargetPos)
			else
				nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWork_Site, WORK_PLACE, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);             //Calculate_MovePos_Find(int nMotNum, int nSite, int nWorkPart, int *npFirstYXP_Pos, double *dpGetTargetPos)
			if(nRet_1 == RET_GOOD)
			{//ok
				m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
			}
			else
			{
				CTL_Lib.Alarm_Error_Occurrence(1204, dWARNING, Func.m_strAlarmCode);
				break;
			}

			if(nWork_Site == THD_LDULD_CARRIER_BUFF)
			{
				if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X )  > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + COMI.md_allow_value[M_UNLOADER_TRANSFER_X] ) )
				{
					break;
				}
				if( st_handler.mn_lduld_safety == ULD_MOVING_SAFETY)
				{
					break;
				}
			}

// 			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate); 
			nRet_1 = COMI.Start_SingleMove( m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate); 
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_Place_Step = 2010;
			}
			else if (nRet_1 == BD_RETRY)
			{
				COMI.Set_MotStop(0, m_nRobot_Y);
				mn_Place_Step = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				//CTL_Lib.Alarm_Error_Occurrence(1205, dWARNING, alarm.mstr_code);
//				COMI.Set_MotStop(0, m_nRobot_Y);
				mn_Place_Step = 2000;
			}
			break; 

		case 2010:
			nRet_1 = COMI.Start_SingleMove( m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate); 
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_Place_Step = 2020;
			}
			else if (nRet_1 == BD_RETRY)
			{
				COMI.Set_MotStop(0, m_nRobot_Y);
				mn_Place_Step = 2010;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				//CTL_Lib.Alarm_Error_Occurrence(1205, dWARNING, alarm.mstr_code);
				//				COMI.Set_MotStop(0, m_nRobot_Y);
				mn_Place_Step = 2010;
			}
			break; 

		case 2020:
			nRet_1 = COMI.Check_SingleMove( m_nRobot_Y, m_dpTargetPosList[1] ); 
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_bDvcWaitChk_Falg =  false;
				mn_Place_Step = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				COMI.Set_MotStop(0, m_nRobot_Y);
				mn_Place_Step = 2010;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				//CTL_Lib.Alarm_Error_Occurrence(1205, dWARNING, alarm.mstr_code);
				//				COMI.Set_MotStop(0, m_nRobot_Y);
				mn_Place_Step = 2010;
			}
			break; 	

		case 2100://check picker and buffer etc
			m_bDvcWaitChk_Falg = false;
			mn_Place_Step = 3000;
			break;


		case 2500:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
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
				CTL_Lib.Alarm_Error_Occurrence(1206, dWARNING, alarm.mstr_code);
				mn_Place_Step = 2500;
			}
			break;

		////////////////////////////////////////////////////////
		// 작업 할 자재를 놓는다 
		////////////////////////////////////////////////////////
		case 3000:
			m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z])
			{
				mn_Place_Step = 2500;
				break;
			}

			nRet_1 = Func.Check_PickerStatus(0, THD_LOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
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
					CTL_Lib.Alarm_Error_Occurrence(1207, dWARNING, Func.m_strAlarmCode);
					break;
				}
			}

			nRet_1 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], COMI.md_allow_value[m_nRobot_Y]);
			if(nRet_1 != BD_GOOD)
			{
				mn_Place_Step = 2000;
				break;
			}

			if(nWork_Site == THD_LD_ALIGN_BUFF)
			{
				if( g_lotMgr.GetLotAt(0).GetDvcType() == "SFF")
				{

					nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loading_Tr_Jig_Detect_Check, IO_OFF, IO_STABLE_WAIT, IO_STABLE_LIMIT); 
					if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
					{
						break; 
					} 
					else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
					{
						m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loading_Tr_Jig_Detect_Check);
						CTL_Lib.Alarm_Error_Occurrence(1208, dWARNING, m_strAlarmCode);
						break;
					}
				}
				else
				{
					nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_buffer_tff_tilt_chk, IO_ON);
					nRet_2 = g_ioMgr.get_in_bit( st_io.i_loading_buffer_sff_tilt_chk, IO_ON);
					if( nRet_1 == IO_OFF && nRet_2 == IO_OFF && st_basic.n_mode_device != WITHOUT_DVC)
					{
					}
					else if( (nRet_1 == IO_ON || nRet_2 == IO_ON) && st_basic.n_mode_device != WITHOUT_DVC)
					{
						if( nRet_1 == IO_ON ) m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_loading_buffer_tff_tilt_chk); 
						else				  m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_loading_buffer_sff_tilt_chk); 
						CTL_Lib.Alarm_Error_Occurrence(1191, dWARNING, m_strAlarmCode);
					}
					
				}
				m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_ALIGN_PLACE_POS];
			}
			else if(nWork_Site == THD_LDULD_CARRIER_BUFF)// TOP MIDDEL BTM
			{
				nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loader_Transfer_Clamp_On_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT); //버퍼에 센서가 없어 피커 센서로 대체
				if(nRet_1 == RET_PROCEED && st_basic.n_mode_device != WITHOUT_DVC) //감지가 안되도 안전화 시간까지는 기다리자 
				{
					break; 
				} 
				else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC) //with device 가 아니면  
				{
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loader_Transfer_Clamp_Off_Check); 
					CTL_Lib.Alarm_Error_Occurrence(1209, dWARNING, m_strAlarmCode);
					break;
				} 
				m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_CARRIER_PLACE_POS];
			}
			else
			{//910000 1 A "THERE_IS_NO_SITE_TO_GO_AT_PLACE_DVC_IN_RUN_LDPICKER."
				CTL_Lib.Alarm_Error_Occurrence(1210, dWARNING, "910000");
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
				CTL_Lib.Alarm_Error_Occurrence(1211, dWARNING, alarm.mstr_code);
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
				CTL_Lib.Alarm_Error_Occurrence(1212, dWARNING, alarm.mstr_code);
				mn_Place_Step = 3000;
			}		
			break;

		case 3200:
			if(nWork_Site == THD_LD_ALIGN_BUFF)
			{
				mn_Place_Step = 3300;
			}
			else
			{
				mn_Place_Step = 3400;
			} 
			break;

		////////////////////////////////////////////////////////////
		//Load buffer의 clamp를 이곳에서 제어 
		//제어 단순화을 위해 로딩 버퍼의 그리퍼를 이곳에서 제어 한다 
		////////////////////////////////////////////////////////////
		case 3300:
			FAS_IO.Set_IO_OnOff(st_io.o_Loader_Transfer_Clamp_On_Sol, IO_ON);//오므린다			 
			mn_Place_Step = 3310;
			break;

		case 3310:
			nRet_1 = FAS_IO.Wait_IO_OnOff(st_io.o_Loader_Transfer_Clamp_On_Sol, IO_ON, st_wait.nOffWaitTime[WAIT_PICKER_CLAMP_ON_OFF]);
			if(nRet_1 == RET_GOOD) //작업 가능한 자재가 있다
			{
				mn_Place_Step = 3320;
			}
			break;

		case 3320:
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_OFF, IO_STABLE_WAIT, 1000);
			if(nRet_1 == RET_GOOD) //감지가 안되도 안전화 시간까지는 기다리자 
			{
				mn_Place_Step = 3400;
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
				CTL_Lib.Alarm_Error_Occurrence(1213, dWARNING, m_strAlarmCode);
			} 
			break; 

		case 3400:
			Set_Loader_Transfer_Clamp_OnOff( IO_OFF );
			mn_Place_Step = 3410;
			break;

		case 3410:
			nRet_1 = Chk_Loader_Transfer_Clamp_OnOff(IO_OFF); 			 
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
				CTL_Lib.Alarm_Error_Occurrence(1214, dWARNING, m_strAlarmCode);
				mn_Place_Step = 3400;
			}
			break;

		case 3420:
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_ON, st_wait.nOnWaitTime[WAIT_PICKER_CLAMP_ON_OFF], st_wait.nLimitWaitTime[WAIT_PICKER_CLAMP_ON_OFF]); 
			if(nRet_1 == RET_GOOD) //감지가 안되도 안전화 시간까지는 기다리자 
			{
				if(nWork_Site == THD_LD_ALIGN_BUFF)
					mn_Place_Step = 3500;
				else
					mn_Place_Step = 4000;
			} 
			else if(nRet_1 == RET_ERROR)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_OFF, st_io.i_Loader_Transfer_Clamp_Off_Check); 
				CTL_Lib.Alarm_Error_Occurrence(1215, dWARNING, m_strAlarmCode);
			} 
			break; 

			//align 에서 디바이스 체크
// 			int i_loading_buffer_sff_tilt_chk;
// 			int i_loading_buffer_tff_tilt_chk;
		case 3500:
			if( g_lotMgr.GetLotByLotID( m_strFindLotNo).GetDvcType() = "SFF" )
			{
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_buffer_sff_tilt_chk, IO_ON);
				if( nRet_1 == IO_OFF )
				{
					mn_Place_Step = 4000;
				}
				else
				{
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_loading_buffer_sff_tilt_chk); 
					CTL_Lib.Alarm_Error_Occurrence(1255, dWARNING, m_strAlarmCode);
				}
			}
			else
			{
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_buffer_tff_tilt_chk, IO_ON);
				if( nRet_1 == IO_OFF )
				{
					mn_Place_Step = 4000;
				}
				else
				{					
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_loading_buffer_tff_tilt_chk); 
					CTL_Lib.Alarm_Error_Occurrence(1256, dWARNING, m_strAlarmCode);
				}
			}
			break;
		

		////////////////////////////////////////////////////////////
		//로보트를 안전 위치로 올린다 
		//Carrier에서는 제자리에 있는 체크한다.
		////////////////////////////////////////////////////////////
		case 4000:
			if( nWork_Site == THD_LD_ALIGN_BUFF )
			{
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
					CTL_Lib.Alarm_Error_Occurrence(1216, dWARNING, alarm.mstr_code);
					mn_Place_Step = 4000;	
				}
			}
			else if( nWork_Site == THD_LDULD_CARRIER_BUFF )
			{
				nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_LASER_CHKPOS], COMI.mn_runspeed_rate);
				if (nRet_1 == BD_GOOD) //좌측으로 이동
				{
					m_dwWaitUntil[0] = GetCurrentTime();
					mn_Place_Step = 4010;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_Place_Step = 4000;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
					CTL_Lib.Alarm_Error_Occurrence(1217, dWARNING, alarm.mstr_code);
					mn_Place_Step = 4000;	
				}
			}
			else
			{//910000 1 A "THERE_IS_NO_SITE_TO_GO_AT_PLACE_DVC_IN_RUN_LDPICKER."
				CTL_Lib.Alarm_Error_Occurrence(1218, dWARNING, "910000");
			}
			break;

			//carrirer buffer에서 디바이스 위치가 맞는치 체크힌다.
		case 4010:
			if( g_lotMgr.GetLotByLotID(m_strFindLotNo).GetDvcType() == "SFF" )
			{
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_1, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_2, IO_ON);
				nRet_3 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_3, IO_ON);
				nRet_4 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_4, IO_ON);
				if( nRet_1 == IO_OFF && nRet_2 == IO_OFF && nRet_3 == IO_OFF && nRet_4 == IO_OFF)
				{
					mn_Place_Step = 4020;
				}
				else
				{
					m_dwWaitUntil[1] = GetCurrentTime();
					m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
					if( m_dwWaitUntil[2] <= 0 ) m_dwWaitUntil[0] = GetCurrentTime();
					if( m_dwWaitUntil[2] < (2*IO_STABLE_WAIT)) break;
					if	   ( nRet_1 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_sff_missalign_chk_1);
					else if( nRet_2 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_sff_missalign_chk_2);
					else if( nRet_3 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_sff_missalign_chk_3);
					else /*if( nRet_4 != IO_ON )*/ m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_sff_missalign_chk_4);
					//CTL_Lib.Alarm_Error_Occurrence(1281, dWARNING, m_strAlarmCode);
					mn_Place_Step = 4015;
				}
			}
			else if( g_lotMgr.GetLotByLotID(m_strFindLotNo).GetDvcType() == "TFF" )
			{
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_1, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_1, IO_ON);
				nRet_3 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_1, IO_ON);
				nRet_4 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_1, IO_ON);
				if( nRet_1 == IO_OFF && nRet_2 == IO_OFF && nRet_3 == IO_OFF && nRet_4 == IO_OFF)
				{
					mn_Place_Step = 4020;
				}
				else
				{
					m_dwWaitUntil[1] = GetCurrentTime();
					m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
					if( m_dwWaitUntil[2] <= 0 ) m_dwWaitUntil[0] = GetCurrentTime();
					if( m_dwWaitUntil[2] < (2*IO_STABLE_WAIT)) break;
					if	   ( nRet_1 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_tff_missalign_chk_1);
					else if( nRet_2 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_tff_missalign_chk_1);
					else if( nRet_3 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_tff_missalign_chk_1);
					else/* if( nRet_4 != IO_ON )*/ m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_sff_missalign_chk_4);
					//CTL_Lib.Alarm_Error_Occurrence(1282, dWARNING, m_strAlarmCode);
					mn_Place_Step = 4015;
				}
			}
			break; 

		case 4015:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 			 
				mn_Place_Step = 4000;
				CTL_Lib.Alarm_Error_Occurrence(1282, dWARNING, m_strAlarmCode);
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Place_Step = 4015;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1299, dWARNING, alarm.mstr_code);
				mn_Place_Step = 4015;
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
				mn_Place_Step = 4020;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1220, dWARNING, alarm.mstr_code);
				mn_Place_Step = 4020;	
			}
			break;

		case 4100:
			if(nWork_Site == THD_LD_ALIGN_BUFF)
			{
// 				mn_BcrFirst = CTL_YES;//2016.1111
// 				mn_BcrFirstChk = CTL_NO;
// 				mn_BcrFirstFinal = CTL_NO;
				mn_Place_Step = 4200;
			}
			else
			{
				mn_Place_Step = 4200; 
			}
			break;
			
		case 4200:
			m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[2] < st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벅어나 있으면 저위치 시킨다
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
			nRet_1 = Func.Check_PickerStatus(1, THD_LOAD_WORK_RBT, CTL_NO, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
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
					CTL_Lib.Alarm_Error_Occurrence(1229, dWARNING, Func.m_strAlarmCode);
					break;
				}
			}

			for (i = m_n_FirstPicker_Num; i < m_nPickerPara; i++)
			{
				if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_YES && m_npPicker_YesNo[i] == CTL_YES
					&& m_npPicker_Vacuum_Status[i] == IO_OFF/*IO_ON*/ && m_npPicker_OutputStatus[i] == IO_OFF/*IO_ON*/)
				{
					if (m_strLotNo[1] == _T(""))
					{
// 						m_strLotNo[1]	= st_picker[m_nRobotSite].st_pcb_info[i].strLotNo;
// 						m_strPartNo[1] = st_picker[m_nRobotSite].st_pcb_info[i].strPartNo;
					}				 
					
					if(nWork_Site == THD_LD_ALIGN_BUFF)
					{
						if(i >= 0 && ( ( m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num) ) < st_recipe.nLdBuffer_Num ) )
						{
							m_npPicker_YesNo[i] = CTL_NO;	//작업한 피커 정보 클리어

							//아래 함수에서 사용위해 위치 정보를 셋팅한다
							m_npSet_WorkPosYXCPB[1] = m_npFindWorkPosYXCPB[1] + (i - m_n_FirstPicker_Num); //Y Pos 위치
							m_npSet_WorkPosYXCPB[3]  = i; //picker 위치 정보

							Func.Data_Exchange_PickPlace(PICKER_PLACE_MODE, 1, m_nRobotSite, nWork_Site, m_npSet_WorkPosYXCPB); //피커 및 트레이 정보 셋팅 함수
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
					else if( nWork_Site == THD_LDULD_CARRIER_BUFF )
					{
						if(i >= 0 && ( ( m_npFindWorkPosYXCPB[1] + (i - m_n_FirstPicker_Num) ) < st_recipe.nCarrierBuffer_Num ) )
						{
							m_npPicker_YesNo[i] = CTL_NO;	//작업한 피커 정보 클리어

							//아래 함수에서 사용위해 위치 정보를 셋팅한다
							m_npSet_WorkPosYXCPB[1] = m_npFindWorkPosYXCPB[1] + (i - m_n_FirstPicker_Num); //Y Pos 위치
							m_npSet_WorkPosYXCPB[3]  = i; //picker 위치 정보

							g_lotMgr.GetLotAt(m_nFindLotNo_Flag).AddInputCnt( PRIME );
							st_picker[m_nRobotSite].st_pcb_info[i].nBinNum = g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetInputCnt( PRIME );
							Func.Data_Exchange_PickPlace(PICKER_PLACE_MODE, 1, m_nRobotSite, nWork_Site, m_npSet_WorkPosYXCPB); //피커 및 트레이 정보 셋팅 함수

							if( g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetInputCnt(PRIME) >= g_lotMgr.GetLotAt(m_nFindLotNo_Flag).GetTotLotCount() )
							{
								g_lotMgr.GetLotAt(m_nFindLotNo_Flag).SetLastModule("YES");
							}
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
				CTL_Lib.Alarm_Error_Occurrence(1221, dWARNING, Func.m_strAlarmCode);
			}
			break;

		case 6000:
			nRet_1 = Func.Check_PickerStatus(0, THD_LOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Data_Status, m_npPicker_OutputStatus);

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
					if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;
				}
				CTL_Lib.Alarm_Error_Occurrence(1222, dWARNING, Func.m_strAlarmCode);
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

int CRun_LdPicker::Process_DVC_Pickup( int nMode, int nWorkSite, CString strLotNo)
{
	int nFuncRet = RET_PROCEED;
	int i, nRet_1, nRet_2, nRet_3, nRet_4, nFlag = 0;

	CString strMsg;
	CString strLogKey[10];
	CString strLogData[10];

	Func.ThreadFunctionStepTrace(8, mn_Pick_Step);

	switch( mn_Pick_Step )
	{
		case 0:
			m_bDvcWaitChk_Falg =  false;
			m_nPickRetry_Cnt = 0;
			mn_Pick_Step = 100;
			break;

		case 100:
			m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[m_nRobot_Z] > st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벗어나 있으면 저위치 시킨다
			{
				mn_Pick_Step = 900;
			}
			else
			{
				mn_Pick_Step = 1000;
			}
			break;

		case 900:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
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
				CTL_Lib.Alarm_Error_Occurrence(1300, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 900;
			}
			break;

		//////////////////////////////////////////////////////
		//피커 상태를 확인 후 문제가 없으면 작업
		//////////////////////////////////////////////////////
		case 1000:
			m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[m_nRobot_Z] > st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벗어나 있으면 저위치 시킨다
			{
				mn_Pick_Step = 900;
				break;
			}
			if(nWorkSite == THD_LD_TRAY_PLATE) // loadig tray 자재 집기 
			{
				nRet_1 = COMI.Get_MotIOSensor(M_TRAY1_Z, MOT_SENS_SD); 	
				if(nRet_1 != BD_GOOD) //로더 플레이트에 트레이가 감지 된 상태 
				{
					strLogKey[0]	= _T("TYPE");
					strLogData[0]	= _T("LD TRAY_PLATE");
					strLogKey[1]	= _T("STATUS");
					strLogData[1]	= _T("OFF");
					strLogKey[2].Format(_T("SD"));
					strLogData[2].Format(_T("0"));
					clsLog.OnAddKeyAlarm(_T("IO"), _T("ERROR"), 3, strLogKey, strLogData);

					m_strAlarmCode.Format(_T("900001")); //900000 1 0 "LOAD_STACKER_PLATE_SD_TRAY_ON_CHECK_ERROR" //kwlee 2016.0902 "900000" ->"900001"
					CTL_Lib.Alarm_Error_Occurrence(1301, dWARNING, m_strAlarmCode);			
					break;
				}  			 
			}
			else if(nWorkSite == THD_LD_ALIGN_BUFF)
			{
			}		
		
			nRet_1 = Func.Check_PickerStatus(0, THD_LOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
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
					CTL_Lib.Alarm_Error_Occurrence(1302, dWARNING, Func.m_strAlarmCode);
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


			if( m_n_FirstPicker_Num == -1)
			{
				mn_Pick_Step = 0;
				nFuncRet = RET_PICKER_NOT_FIND;
				break;
			}

			nRet_1 = Func.Find_TrayWork_Pos(WORK_PICK, nWorkSite, CTL_YES, m_npFindWorkPosYXCPB, THD_LOAD_WORK_RBT, strLotNo);
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

					if( nWorkSite == THD_LD_TRAY_PLATE )
					{
						if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_NO)
						{
							if(i >= 0 && m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num) < st_recipe.nTrayY)
							{
								if(st_tray_info[nWorkSite].st_pcb_info[ m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)][m_n_FirstTray_X_Num].nYesNo == CTL_YES) //트레이에 자재가  있으면 집기 가능하게 셋
								{
									m_npPicker_YesNo[i] = CTL_YES;
									if( st_handler.bLoaderOnFlag == CTL_YES )
									{
										m_npPicker_YesNo[i] = CTL_NO;
									}
									nFlag++;
									st_picker[m_nRobotSite].st_pcb_info[i].nStatusChang = NO;
								}
							}
						}
					}
					else if( nWorkSite == THD_LD_ALIGN_BUFF )
					{
						if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_NO)
						{
							if(i >= 0 && (( m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)) < st_recipe.nLdBuffer_Num))//nLdBuffer_Num 1
							{
								if(st_buffer_info[nWorkSite].st_pcb_info[m_n_FirstTray_Y_Num - (i - m_n_FirstPicker_Num)].nYesNo == CTL_YES) //트레이에 자재가  있으면 집기 가능하게 셋
								{
									m_npPicker_YesNo[i] = CTL_YES;
									nFlag++;
									st_picker[m_nRobotSite].st_pcb_info[i].nStatusChang = NO;
								}
							}
						}
					}
				}
			}

			if( nFlag == 0 )
			{
				mn_Pick_Step = 0;
				nFuncRet = RET_PICKER_NOT_FIND;
				break;
			}

			m_npFindWorkPosYXCPB[0] = m_n_FirstTray_X_Num;
			m_npFindWorkPosYXCPB[1] = m_n_FirstTray_Y_Num;
			m_npFindWorkPosYXCPB[3] = m_n_FirstPicker_Num;

			m_nMove_Flag[0] = CTL_NO; m_nMove_Flag[1] = CTL_NO;

			if( nWorkSite == THD_LD_TRAY_PLATE )
			{
				mn_Pick_Step = 2000;
			}
			else
			{
				mn_Pick_Step = 2000;
			}	
			break;

		case 1500:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_Pick_Step = 2000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 1500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1303, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 1500;
			}
			break;

		case 2000:
			m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벅어나 있으면 저위치 시킨다
			{
				mn_Pick_Step = 1500;
				break;
			}

			nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWorkSite, WORK_PICK, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);             

			if( nRet_1 == RET_GOOD )
			{//ok
				m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
			}
			else
			{
				CTL_Lib.Alarm_Error_Occurrence(1304, dWARNING, Func.m_strAlarmCode);
				break;
			}

			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, m_dpTargetPosList[1], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_bDvcWaitChk_Falg =  false;
				mn_Pick_Step = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1305, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 2000;
			}
			break;

		case 2100:
			m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z]) //안전 위치를 벅어나 있으면 저위치 시킨다
			{
				mn_Pick_Step = 900;
				break;
			}

			mn_Pick_Step = 3000;
			break;

		case 2200:
			Set_Loader_Transfer_Clamp_OnOff( IO_OFF );
			mn_Pick_Step = 2300;
			break;

		case 2300:
			nRet_1 = Chk_Loader_Transfer_Clamp_OnOff( IO_OFF );
			if( nRet_1 == RET_GOOD )
			{
				mn_Pick_Step = 3000;
			}
			else if( nRet_1 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 1306, dWARNING, alarm.mstr_code);
			}
			break;

		case  3000:
			nRet_1 = g_ioMgr.get_out_bit(st_io.o_Loader_Transfer_Clamp_On_Sol,	IO_OFF); 
			nRet_2 = g_ioMgr.get_out_bit(st_io.o_Loader_Transfer_Clamp_Off_Sol,	IO_ON);
			nRet_3 = g_ioMgr.get_in_bit( st_io.i_Loader_Transfer_Clamp_Off_Check, IO_ON );
			if(nRet_1 == IO_ON || nRet_2 == IO_OFF || nRet_3 == IO_OFF) 
			{  //그리퍼가 벌리고 있지 않으면 자재에 데미지를 주기때문에 미리 벌리고 내려야 한다 
				mn_Pick_Step = 2200;
				break;
			}

			nRet_1 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], COMI.md_allow_value[m_nRobot_Y]);
			if(nRet_1 != BD_GOOD)
			{
				mn_Pick_Step = 2000;
				break;
			}

			if(nWorkSite == THD_LD_TRAY_PLATE)
			{
				m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_TRAY_POS];
			}
			else if(nWorkSite == THD_LD_ALIGN_BUFF)
			{
				m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_ALIGN_PICKPOS];
			}
			else
			{
				break;
			}

			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 
				Set_Loader_Transfer_Clamp_OnOff( IO_OFF );
				mn_Pick_Step = 3100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1307, dWARNING, alarm.mstr_code);
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
				CTL_Lib.Alarm_Error_Occurrence(1308, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 3000;
			}		
			break;

		case 3110:
			nRet_1 = Chk_Loader_Transfer_Clamp_OnOff(IO_OFF); 
			if(nRet_1 == RET_GOOD )
			{
				mn_Pick_Step = 3200;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1309, dWARNING, m_strAlarmCode);
				mn_Pick_Step = 3000;
			}
			break;

		case 3200:

			Set_Loader_Transfer_Clamp_OnOff( IO_ON );
			mn_Pick_Step = 3210;
			break;

		case 3210:
			nRet_1 = Chk_Loader_Transfer_Clamp_OnOff( IO_ON);
			if(nRet_1 == RET_GOOD )
			{
				mn_Pick_Step = 3300;
			}
			else if( nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1310, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 3200;
			}
			break;

		case 3300:
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io. i_Loader_Transfer_Clamp_Off_Check, IO_OFF, IO_STABLE_WAIT, 1000); //IO_STABLE_WAIT == 100ms
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
					CTL_Lib.Alarm_Error_Occurrence(1311, dWARNING, _T("900200") );
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
				CTL_Lib.Alarm_Error_Occurrence(1312, dWARNING, m_strAlarmCode);
			} 
			break;

		case 3400:
			if(nWorkSite == THD_LD_TRAY_PLATE) //load tray에서 자재를 집을떄 
			{
				mn_Pick_Step = 4000;
			}
			else// if(nWork_Site == THD_LD_ALIGN_BUFF)//align 버퍼에서 자재를 집을때
			{
				mn_Pick_Step = 3500;// 
			} 
			break;

		case 3500:
			Set_Loader_Buffer_Align_OnOff( IO_OFF );
			mn_Pick_Step = 3510;
			break;

		case 3510:
			nRet_1 = FAS_IO.Wait_IO_OnOff(st_io.o_Loader_Align_Backward_Sol, IO_ON, st_wait.nOnWaitTime[WAIT_BUFFER_ALIGN_CLAMP]);
			if(nRet_1 == RET_GOOD) //작업 가능한 자재가 있다
			{
				mn_Pick_Step = 3520;
			}
			break;

		case 3520:
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loader_Align_Backward_Check , IO_ON, IO_STABLE_WAIT, 2000); 
			if(nRet_1 == RET_GOOD) //감지가 안되도 안전화 시간까지는 기다리자 
			{
				mn_Pick_Step = 3530;
			} 
			else if(nRet_1 == RET_PROCEED)
			{
				break;
			}
			else if(nRet_1 == RET_ERROR)
			{
				if(st_basic.n_mode_device == WITHOUT_DVC)
				{
					mn_Pick_Step = 3530;
					break;
				}

				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loader_Transfer_Clamp_Off_Check); 
				CTL_Lib.Alarm_Error_Occurrence(1313, dWARNING, m_strAlarmCode);
			} 
			break; 

		case 3530:
			nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Loader_Transfer_Clamp_On_Check, IO_ON, IO_STABLE_WAIT, 5000);
			if(nRet_1 == RET_GOOD)
			{
				mn_Pick_Step = 4000;
			}
			else if(nRet_1 == RET_PROCEED)
			{
				break;
			}
			else if(nRet_1 == RET_ERROR)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_Loader_Transfer_Clamp_Off_Check); 
				CTL_Lib.Alarm_Error_Occurrence(1314, dWARNING, m_strAlarmCode);
			} 
			break;

		case  4000:
			m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 
				Set_Loader_Transfer_Clamp_OnOff(IO_ON); //동작한 피커를 올린다  
				mn_Pick_Step = 4100 ; //시간 단추을 위헤 체크는 다음으로 
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 4000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1315, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 4000;
			}		
			break;

		case 4100:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK,m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 
				m_bDvcWaitChk_Falg = false;
				mn_Pick_Step = 4200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 4000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1316, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 4000;
			}		
			break;

// 			st_io.i_loading_rbt_glipper_sff_missalign_chk_1 =	2400;
// 			st_io.i_loading_rbt_glipper_sff_missalign_chk_2 =	2401;
// 			st_io.i_loading_rbt_glipper_sff_missalign_chk_3 =	2402;
// 			st_io.i_loading_rbt_glipper_sff_missalign_chk_4 =	2403;
// 			
// 			st_io.i_loading_rbt_glipper_tff_missalign_chk_1 =	2404;
// 			st_io.i_loading_rbt_glipper_tff_missalign_chk_2 =	2405;
// 			st_io.i_loading_rbt_glipper_tff_missalign_chk_3 =	2406;
// 			st_io.i_loading_rbt_glipper_tff_missalign_chk_4 =	2407;
		case 4200:
			if( g_lotMgr.GetLotByLotID(m_strFindLotNo).GetDvcType() == "SFF" )
			{
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_1, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_2, IO_ON);
				nRet_3 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_3, IO_ON);
				nRet_4 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_4, IO_ON);
				if( nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON || nRet_4 == IO_ON)
				{
					mn_Pick_Step = 5000;
				}
				else
				{
					if	   ( nRet_1 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_sff_missalign_chk_1);
					else if( nRet_2 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_sff_missalign_chk_2);
					else if( nRet_3 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_sff_missalign_chk_3);
					else /*if( nRet_4 != IO_ON )*/ m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_sff_missalign_chk_4);
					CTL_Lib.Alarm_Error_Occurrence(1370, dWARNING, m_strAlarmCode);
				}
			}
			else if( g_lotMgr.GetLotByLotID(m_strFindLotNo).GetDvcType() == "TFF" )
			{
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_1, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_2, IO_ON);
				nRet_3 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_3, IO_ON);
				nRet_4 = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_4, IO_ON);
				if( nRet_1 == IO_ON || nRet_2 == IO_ON || nRet_3 == IO_ON || nRet_4 == IO_ON)
				{
					mn_Pick_Step = 5000;
				}
				else
				{
					if	   ( nRet_1 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_tff_missalign_chk_1);
					else if( nRet_2 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_tff_missalign_chk_2);
					else if( nRet_3 != IO_ON ) m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_tff_missalign_chk_3);
					else/* if( nRet_4 != IO_ON )*/ m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_loading_rbt_glipper_sff_missalign_chk_4);
					CTL_Lib.Alarm_Error_Occurrence(1371, dWARNING, m_strAlarmCode);
				}
			}
			break;

		case 5000:
// 			if( st_handler.bLoaderOnFlag == CTL_YES )
// 			{
// 				nRet_1 = Func.Check_PickerStatus(0, THD_LOAD_WORK_RBT, CTL_NO, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
// 			}
// 			else
// 			{
				nRet_1 = Func.Check_PickerStatus(1, THD_LOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
// 			}

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
					CTL_Lib.Alarm_Error_Occurrence(1372, dWARNING, Func.m_strAlarmCode);
					break;
				}
			}

			for (i = m_n_FirstPicker_Num; i < m_nPickerPara; i++)
			{		

				if(nWorkSite == THD_LD_TRAY_PLATE)
				{

					if( st_handler.bLoaderOnFlag == CTL_YES )
					{
						if(nWorkSite == THD_LD_TRAY_PLATE)
						{
							if(i >= 0 && m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num) < st_recipe.nTrayY)
							{
								m_npPicker_YesNo[i] = CTL_NO; //작업한 피커 정보 클리어

								//아래 함수에서 사용위해 위치 정보를 셋팅한다
								m_npSet_WorkPosYXCPB[0]  = m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num); //tray Y Pos 위치
								m_npSet_WorkPosYXCPB[1]  = m_npFindWorkPosYXCPB[1]; //tray X Pos 위치
								m_npSet_WorkPosYXCPB[3]  = i; //picker 위치 정보

								Func.Data_Exchange_PickPlace(PICKER_PICK_MODE, 1, m_nRobotSite, nWorkSite, m_npSet_WorkPosYXCPB); //피커 및 트레이 정보 셋팅 함수						
							}
						}
					}
					else if(st_picker[m_nRobotSite].st_pcb_info[i].nYesNo == CTL_NO && m_npPicker_YesNo[i] == CTL_YES
						&& (m_npPicker_Vacuum_Status[i] == IO_ON && m_npPicker_OutputStatus[i] == IO_ON/*IO_OFF*/))
					{
						if(nWorkSite == THD_LD_TRAY_PLATE)
						{
							if(i >= 0 && m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num) < st_recipe.nTrayY)
							{
								m_npPicker_YesNo[i] = CTL_NO; //작업한 피커 정보 클리어

								m_npSet_WorkPosYXCPB[0]  = m_npFindWorkPosYXCPB[0];
								m_npSet_WorkPosYXCPB[1]  = m_npFindWorkPosYXCPB[1] + (i - m_n_FirstPicker_Num); //tray Y Pos 위치
								m_npSet_WorkPosYXCPB[3]  = i; //picker 위치 정보

								Func.Data_Exchange_PickPlace(PICKER_PICK_MODE, 1, m_nRobotSite, nWorkSite, m_npSet_WorkPosYXCPB);

								if ( strLotNo == st_picker[m_nRobotSite].st_pcb_info[i].strLotNo)//진행중인 Lot과 같으면
								{
									// 현재 또는 next lot 수량 증가
								}
								else
								{ 
									//error 현재 진행한 lot 정보와 같지 않으면 메세지 출력 
								}
							}
						}
					}
				}
				else if( nWorkSite == THD_LD_ALIGN_BUFF)
				{
					if(i >= 0 && ( ( m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num)) < st_recipe.nLdBuffer_Num ) )
					{
						m_npPicker_YesNo[i] = CTL_NO; //작업한 피커 정보 클리어

						//아래 함수에서 사용위해 위치 정보를 셋팅한다
						m_npSet_WorkPosYXCPB[0]  = m_npFindWorkPosYXCPB[0]; //tray Y Pos 위치
						m_npSet_WorkPosYXCPB[1]  = m_npFindWorkPosYXCPB[1] + (i - m_n_FirstPicker_Num); //tray X Pos 위치
						m_npSet_WorkPosYXCPB[3]  = i; //picker 위치 정보

						Func.Data_Exchange_PickPlace(PICKER_PICK_MODE, 1, m_nRobotSite, nWorkSite, m_npSet_WorkPosYXCPB); //피커 및 트레이 정보 셋팅 함수		

						if (m_strLotNo[1] == _T(""))
						{
							m_strLotNo[1]	= st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strLotNo;
							m_strPartNo[1] = st_picker[THD_LOAD_WORK_RBT].st_pcb_info[i].strPartNo;
						}
					}
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
				CTL_Lib.Alarm_Error_Occurrence(1317, dWARNING, Func.m_strAlarmCode);
				//kwlee 2017.0214
				//mn_Pick_Step = 0;
			}
			break;

		case 6000:
			nRet_1 = Func.Check_PickerStatus(0, THD_LOAD_WORK_RBT, CTL_YES, m_npPicker_YesNo, m_npPicker_Data_Status, m_npPicker_OutputStatus);
			if (nRet_1 == RET_GOOD) //
			{
// 				nFuncRet = RET_GOOD;
				mn_Pick_Step = 6100;
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
					CTL_Lib.Alarm_Error_Occurrence(1318, dWARNING, Func.m_strAlarmCode);
				}
			}
			break;

		case 6100:
			if( nWorkSite == THD_LD_TRAY_PLATE)
			{
				mn_Pick_Step = 6200;
			}
			else
			{
				nFuncRet = RET_GOOD;
				mn_Pick_Step = 0;
			}
			break;

		case 6200:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_Pick_Step = 6300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 6200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1319, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 6200;
			}
			break;

		case 6300:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				nFuncRet = RET_GOOD;
				mn_Pick_Step = 0;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_Pick_Step = 6300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1320, dWARNING, alarm.mstr_code);
				mn_Pick_Step = 6200;
			}
			break;
			

		default :
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Pick_Step at Process_DVC_Pick  = %d", mn_Pick_Step);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			break;
	}

	return nFuncRet;
}

void CRun_LdPicker::Set_Loader_Buffer_Align_OnOff(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bClampOnOffFlag	= false;
	m_dwClampOnOff[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Loader_Align_Forward_Sol, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Loader_Align_Backward_Sol, !OnOff);

	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("FORWARD"),0,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("BACKWARD"),0,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

int CRun_LdPicker::Chk_Loader_Buffer_Align_OnOff( int OnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_BUFFER_ALIGN_CLAMP;

	if (OnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag == false && g_ioMgr.get_in_bit(st_io.i_Loader_Align_Forward_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Loader_Align_Backward_Check, IO_ON) == IO_ON )
		{
			m_bClampOnOffFlag		= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Loader_Align_Forward_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Loader_Align_Backward_Check, IO_ON) == IO_ON )
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
				clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("BACKWARD"),1,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loader_Align_Forward_Check); 
				clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("BACKWARD"),1,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag == false && g_ioMgr.get_in_bit(st_io.i_Loader_Align_Forward_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Loader_Align_Backward_Check, IO_OFF)	== IO_OFF )
		{
			m_bClampOnOffFlag			= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Loader_Align_Forward_Check, IO_ON)	== IO_ON &&
				 g_ioMgr.get_in_bit(st_io.i_Loader_Align_Backward_Check, IO_OFF) == IO_OFF )
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
				clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("FORWARD"),1,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loader_Align_Forward_Check); 
				clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("FORWARD"),1,_T("ALIGN"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}


void CRun_LdPicker::Set_Loader_Transfer_Clamp_OnOff(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bClampOnOffFlag	= false;
	m_dwClampOnOff[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Loader_Transfer_Clamp_On_Sol, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Loader_Transfer_Clamp_Off_Sol, !OnOff);

	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("FORWARD"),0,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("BACKWARD"),0,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}

}

int CRun_LdPicker::Chk_Loader_Transfer_Clamp_OnOff( int OnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_PICKER_CLAMP_ON_OFF;

	if (OnOff == IO_OFF)
	{
		if (m_bClampOnOffFlag == false &&	g_ioMgr.get_in_bit(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_ON)	== IO_ON )
		{
			m_bClampOnOffFlag		= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_ON)	== IO_ON )
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
				clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("BACKWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loader_Transfer_Clamp_Off_Check); 
				clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("BACKWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bClampOnOffFlag == false && g_ioMgr.get_in_bit(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_OFF)	== IO_OFF )
		{
			m_bClampOnOffFlag			= true;
			m_dwClampOnOff[0]	= GetCurrentTime();
		}
		else if (m_bClampOnOffFlag == true && g_ioMgr.get_in_bit(st_io.i_Loader_Transfer_Clamp_Off_Check, IO_OFF)	== IO_OFF )
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
				clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("FORWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
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
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Loader_Transfer_Clamp_Off_Check); 
				clsLog.LogFunction(_T("LD_PICKER_ROBOT"),_T("FORWARD"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}

//nModel : 0 initial
//nMode1 : 1 SFF
//nModel : 2 TFF
int CRun_LdPicker::Check_DVC_Model(int nModel, int nYESNO, int* nSNum)
{
	int nFuncRet = RET_ERROR;

	int i = 0, nRet[8] = {0,};

	nRet[0] = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_1, IO_ON);
	nRet[1] = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_2, IO_ON);
	nRet[2] = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_3, IO_ON);
	nRet[3] = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_sff_missalign_chk_4, IO_ON);

	nRet[4] = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_1, IO_ON);
	nRet[5] = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_2, IO_ON);
	nRet[6] = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_3, IO_ON);
	nRet[7] = g_ioMgr.get_in_bit( st_io.i_loading_rbt_glipper_tff_missalign_chk_4, IO_ON);

	if( st_handler.mn_virtual_mode == CTL_YES || st_basic.n_mode_device == WITHOUT_DVC)
	{
		if( nModel == MODEL_INIT)		nFuncRet = RET_ERROR;
		else if( nModel == MODEL_SFF )  nFuncRet = RET_GOOD;
		else if( nModel == MODEL_TFF )  nFuncRet = RET_GOOD;
	}
	else if( nModel == MODEL_INIT)
	{
		for( i = 0; i < 8; i++ )
		{
			if( nRet[i] == nYESNO ) 
			{
				nSNum[0] = i;
				nFuncRet = RET_GOOD;
				break;
			}
		}
	}
	else if( nModel == MODEL_SFF)
	{
		for( i = 0; i < 4; i++ )
		{
			if( nRet[i] == nYESNO ) 
			{
				nSNum[0] = i;
				nFuncRet = RET_GOOD;
				break;
			}
		}
	}
	else if( nModel == MODEL_TFF)
	{
		for( i = 4; i < 84; i++ )
		{
			if( nRet[i] == nYESNO ) 
			{
				nSNum[0] = i;
				nFuncRet = RET_GOOD;
				break;
			}
		}
	}

	return nFuncRet;
}

int CRun_LdPicker::GetMotorPosY(double dPos)
{
	int nPos = -1;

	if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_LOADER_TRANSFER_Y_INIT_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_LOADER_TRANSFER_Y_READY_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_PICK_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_PICK_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_LOADER_TRANSFER_Y_PICK_POS;
	}	
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[p_LOADER_TRANSFER_Y_ALIGN_PLACE_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[p_LOADER_TRANSFER_Y_ALIGN_PLACE_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= p_LOADER_TRANSFER_Y_ALIGN_PLACE_POS;
	}	
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_ALIGN_PICK_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_ALIGN_PICK_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_LOADER_TRANSFER_Y_ALIGN_PICK_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_PLACE_TOP_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_PLACE_TOP_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_LOADER_TRANSFER_Y_PLACE_TOP_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_PLACE_MID_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_PLACE_MID_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_LOADER_TRANSFER_Y_PLACE_MID_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_PLACE_BOT_POS] - COMI.md_allow_value[m_nRobot_Y] &&
		dPos <= st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Y_PLACE_BOT_POS] + COMI.md_allow_value[m_nRobot_Y])
	{
		nPos= P_LOADER_TRANSFER_Y_PLACE_BOT_POS;
	}
	
	return nPos;
}

int CRun_LdPicker::GetMotorPosZ(double dPos)
{
	int nPos = -1;
	
	if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_LOADER_TRANSFER_Z_INIT_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_UP_POS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_UP_POS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_LOADER_TRANSFER_Z_UP_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_ALIGN_PLACE_POS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_ALIGN_PLACE_POS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_LOADER_TRANSFER_Z_ALIGN_PLACE_POS;
	}	
	else if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_ALIGN_PICKPOS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_ALIGN_PICKPOS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_LOADER_TRANSFER_Z_ALIGN_PICKPOS;
	}	
	else if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_TRAY_POS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_TRAY_POS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_LOADER_TRANSFER_Z_TRAY_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_CARRIER_PLACE_POS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_CARRIER_PLACE_POS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_LOADER_TRANSFER_Z_CARRIER_PLACE_POS;
	}
	else if ( dPos >= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_LASER_CHKPOS] - COMI.md_allow_value[m_nRobot_Z] &&
		dPos <= st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_LASER_CHKPOS] + COMI.md_allow_value[m_nRobot_Z])
	{
		nPos= P_LOADER_TRANSFER_Z_LASER_CHKPOS;
	}
	
	
	return nPos;
}

void CRun_LdPicker::OnLdPicker_FinalPos()
{
	if (st_work.nReinstatement_mode[LD_PICKER] == 0)
	{

		st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y] = COMI.md_cmdpos_backup[M_LOADER_TRANSFER_Y];
		st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Z] = COMI.md_cmdpos_backup[M_LOADER_TRANSFER_Z];

		//kwlee 2017.0626
		st_work.nReinst_MotorPos[0][M_LOADER_TRANSFER_Y] = GetMotorPosY(COMI.md_cmdpos_backup[M_LOADER_TRANSFER_Y]);
		st_work.nReinst_MotorPos[0][M_LOADER_TRANSFER_Z] = GetMotorPosZ(COMI.md_cmdpos_backup[M_LOADER_TRANSFER_Z]);

		st_work.nPickerClampState[0] = g_ioMgr.get_out_bit(st_io.o_Loader_Transfer_Clamp_On_Sol, IO_OFF);
		st_work.nPickerClampState[1] = g_ioMgr.get_out_bit(st_io.o_Loader_Transfer_Clamp_Off_Sol, IO_ON);
		
		st_work.nRestState_LdUldSafety = st_handler.mn_lduld_safety;
		st_work.nReinstatement_mode[LD_PICKER] = 1;
	}
}

void CRun_LdPicker::Robot_BackupPos_Check()
{
	st_work.dReinstatement_pos[1][M_LOADER_TRANSFER_Y] = st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y];
	st_work.dReinstatement_pos[1][M_LOADER_TRANSFER_Z] = st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Z];
}

void CRun_LdPicker::Run_ReinState()
{
	return; //2017.0628복귀 동작 막아 놓음.

	int i;
	int nRet_1,nRet_2;

	//int nChangePos[2];
	double dCurrPos[4];
	double dTargetPos;

	if (st_work.nLd_Picker_ReinstateMent_Ok == YES) return;
	
	switch(mn_reinstate_step)
	{
	case 0:
		
// 		st_handler.m_nAssemblyAble = CTL_NO;
// 		g_ioMgr.set_out_bit(st_io.o_Interface_Output_1, IO_OFF);
		mn_reinstate_step = 10;
		break;

	case 10:
		m_dcurr_pos[0] = COMI.Get_MotCurrentPos(M_LOADER_TRANSFER_Y);
	//	m_dcurr_pos[1] = COMI.Get_MotCurrentPos(M_LOADER_TRANSFER_Z);
	//	m_dcurr_pos[2] = COMI.Get_MotCurrentPos(M_UNLOADER_TRANSFER_X);

		if( st_handler.mn_lduld_safety == ULD_MOVING_SAFETY)
		{
			//if( dCurrentPos <== sfty &&  st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y] <= sfty)
			if( m_dcurr_pos[0] <= st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS] &&  
				st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y] <= st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS])
			{
				mn_reinstate_step = 1000;
			}
			else
			{
				//if( dCurrentPos > sfty + allow)
				if (m_dcurr_pos[0] > st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS]  + COMI.md_allow_value[M_LOADER_TRANSFER_Y])
				{
					//알람 
					alarm.mstr_code.Format("%02d0008",M_LOADER_TRANSFER_Y);
					mn_reinstate_step = 20000;	
				}		
				else
				{
					//알람 
					alarm.mstr_code.Format("%02d0008",M_LOADER_TRANSFER_Y);
					//if(st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y] > safety + allow )
					//알람 버그 reset  step저장
					mn_reinstate_step = 20000;
				}
			}
			break;
		}
		else
		{
			//if( dCurrentPos > sfty || st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y] > sfty)
			if( m_dcurr_pos[0] > st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS] || 
				st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y] > st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_INIT_POS])
			{
				if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X )  > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_READY_POS] + COMI.md_allow_value[M_UNLOADER_TRANSFER_X] ) )
				{
					//나올수 없는 상황 setep reset alarm
					alarm.mstr_code.Format("%02d0008",M_UNLOADER_TRANSFER_X);
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
		m_dcurr_pos[0] = COMI.Get_MotCurrentPos(M_LOADER_TRANSFER_Y);
		m_dcurr_pos[1] = COMI.Get_MotCurrentPos(M_LOADER_TRANSFER_Z);

		for (i = 0; i< 3; i++)
		{
			m_nChange[i] = 0;
		}

		if (m_dcurr_pos[0] > st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y] +  COMI.md_allow_value[m_nRobot_Y] ||
			m_dcurr_pos[0] < st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y] - COMI.md_allow_value[m_nRobot_Y])
		{
			m_nChange[0]++;
			sprintf(st_msg.c_abnormal_msg, "Ld Picker Robot Y POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Y], (long)m_dcurr_pos[0], (long)COMI.md_allow_value[m_nRobot_Y]);	
		}
		
		if (m_dcurr_pos[1] > st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Z] +  COMI.md_allow_value[m_nRobot_Z] ||
			m_dcurr_pos[1] < st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Z] - COMI.md_allow_value[m_nRobot_Z])
		{
			m_nChange[1]++;
			sprintf(st_msg.c_abnormal_msg, "Ld Picker Robot Z POS [OLD] : %ld -> [NOW] : %ld [GAP] : %ld", (long)st_work.dReinstatement_pos[0][M_LOADER_TRANSFER_Z], (long)m_dcurr_pos[1], (long)COMI.md_allow_value[m_nRobot_Z]);
		}
		
		//피커 확인
		nRet_1 = g_ioMgr.get_in_bit( st_io.i_Loader_Transfer_Clamp_On_Check, IO_ON);
		nRet_2 = g_ioMgr.get_in_bit( st_io.i_Loader_Transfer_Clamp_Off_Check, IO_OFF);

		if( nRet_1 == st_work.nPickerClampState[0] && nRet_2 == st_work.nPickerClampState[1])
		{
			m_nChange[2] = 1; 
		}
	
		if (m_nChange[0] > 0 || m_nChange[1] > 0)
		{
			//m_nChange[2] == 1 : Picker 동작 변화 없음.
// 			if( m_nChange[2] == 1)
// 			{
// 				mn_reinstate_step = 2000; // 위치 몇 동작 변화.
// 			}
//  		else
//  		{
//  			
//  		}
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
			if( m_nChange[2] == 1)
				mn_reinstate_step = 5000;
			else
				mn_reinstate_step = 4800;
		}
		break;


	case 2000:
		Robot_BackupPos_Check();
		COMI.Set_MotPower(M_LOADER_TRANSFER_Y, TRUE);
		COMI.Set_MotPower(M_LOADER_TRANSFER_Z, TRUE);
		mn_reinstate_step = 2100;
		break;

	case 2100:
		//z가 틀리면 Up후에 y 동작
		if( st_work.dReinstatement_pos[1][M_LOADER_TRANSFER_Y] == st_motor[m_nRobot_Y].md_pos[st_work.nReinst_MotorPos[0][M_LOADER_TRANSFER_Y]])
		{ 
// 			if( m_nChange[2] == 1)
// 			{
// 				mn_reinstate_step = 5000;
// 			}
// 			else
// 			{
// 				mn_reinstate_step = 4800;
// 			}
			//kwlee 2017.0626
			mn_reinstate_step = 2300;
		}
		else
		{
			//z축 home 후
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD)
			{
				mn_reinstate_step = 2300;
			}
			else if(nRet_1 == BD_ERROR)
			{
				mn_reinstate_step = 20000;
				//alarm.mstr_code.Format("%02d0008",m_nRobot_Z);
			}
			//mn_reinstate_step = 2300;
		}
		break;

	case 2300:
		if( st_work.nReinst_MotorPos[0][M_LOADER_TRANSFER_Y] == -1 )
		{
			dTargetPos = st_work.dReinstatement_pos[1][M_LOADER_TRANSFER_Y]; //이동 
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, dTargetPos, COMI.mn_runspeed_rate);

			if (nRet_1 == BD_GOOD)
			{
// 				if( m_nChange[2] == 1)
// 				{
// 					mn_reinstate_step = 5000;
// 				}
// 				else
// 				{
// 					mn_reinstate_step = 4800;
// 				}
				//kwlee 2017.0626
				mn_reinstate_step = 2400;
			}
			else if (nRet_1 == BD_ERROR)
			{
				mn_reinstate_step = 20000;
				//alarm.mstr_code.Format("%02d0008",m_nRobot_Z);
			}
		}
		else
		{
			//st_work.nReinst_MotorPos[0][M_LOADER_TRANSFER_Y] = 작업 위치.
			dTargetPos = st_motor[m_nRobot_Y].md_pos[st_work.nReinst_MotorPos[0][M_LOADER_TRANSFER_Y]]; //이동
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, dTargetPos, COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD)
			{
// 				if( m_nChange[2] == 1)
// 				{
// 					mn_reinstate_step = 5000;
// 				}
// 				else
// 				{
// 					mn_reinstate_step = 4800;
// 				}
				//kwlee 2017.0626
				mn_reinstate_step = 2400;
			}
			else if (nRet_1 == BD_ERROR)
			{
				mn_reinstate_step = 20000;
			}
		}
		break;

	  // Z축 이동
	case 2400:
		if( st_work.nReinst_MotorPos[0][M_LOADER_TRANSFER_Z] == -1 )
		{
			dTargetPos = st_work.dReinstatement_pos[1][M_LOADER_TRANSFER_Z]; //이동 
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, dTargetPos, COMI.mn_runspeed_rate);
			
			if (nRet_1 == BD_GOOD)
			{
				if( m_nChange[2] == 1)
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
				mn_reinstate_step = 20000;
			}
		}
		else
		{
			//st_work.nReinst_MotorPos[0][M_LOADER_TRANSFER_Y] = 작업 위치.
			dTargetPos = st_motor[m_nRobot_Z].md_pos[st_work.nReinst_MotorPos[0][M_LOADER_TRANSFER_Z]]; //이동
				
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, dTargetPos, COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD)
			{
				if( m_nChange[2] == 1)
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
				mn_reinstate_step = 20000;
			}
		}
		break;
		

   case 4800:
// 	   g_ioMgr.get_out_bit(st_io.o_Loader_Transfer_Clamp_On_Sol, st_work.nPickerClampState[0]);
// 	   g_ioMgr.get_out_bit(st_io.o_Loader_Transfer_Clamp_Off_Sol, st_work.nPickerClampState[1]);
	   //kwlee 2017.0626
	   	Set_Loader_Transfer_Clamp_OnOff(st_work.nPickerClampState[0]); 
	   mn_reinstate_step = 4900;
	   break;

   case 4900:
	   //확인 
	   nRet_1 = Chk_Loader_Transfer_Clamp_OnOff(st_work.nPickerClampState[0]);

	   if (nRet_1 == RET_GOOD)
	   {
		   mn_reinstate_step = 5000;
	   }
		else if (nRet_1 == RET_ERROR)
	   {
			mn_reinstate_step = 20000;
	   }
	   break;

	case 5000:
		st_work.nLd_Picker_ReinstateMent_Ready = YES;
		st_work.nLd_Picker_ReinstateMent_Ok = YES;//20121126
		mn_reinstate_step = 0;
		break;

	case 20000:
		st_handler.n_sync_reinstate = NO;
		mn_reinstate_step = 0;
		break;
	}


	
}