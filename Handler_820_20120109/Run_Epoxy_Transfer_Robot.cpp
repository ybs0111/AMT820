// Run_Epoxy_Transfer_Robot.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_Epoxy_Transfer_Robot.h"
#include "Public_Function.h"
#include "AMTLotManager.h"
#include "Run_Device_Carrier_Robot.h"
#include "Run_EmptyTrayTransfer.h"
#include "SrcPart/APartHandler.h"
#include "IO_Manager.h"
#include "Cmmsdk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_Epoxy_Transfer_Robot
CRun_Epoxy_Transfer_Robot Run_Epoxy_Transfer_Robot;
IMPLEMENT_SERIAL(CRun_Epoxy_Transfer_Robot, CObject, 1);

CRun_Epoxy_Transfer_Robot::CRun_Epoxy_Transfer_Robot()
{

	//linear move 과련 셋팅
	//
	m_nLinearMove_Index	= 1;	//현재 map index
	m_lAxisCnt	= 2;				 //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개
	m_lpAxisNum[0] = M_EPOXY_TRANSFER_X; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다
	m_lpAxisNum[1] = M_EPOXY_TRANSFER_Y;
	m_lpAxisNum[2] = M_EPOXY_TRANSFER_Z;

	COMI.ml_axiscnt[m_nLinearMove_Index] = 2;
	COMI.mp_axisnum[m_nLinearMove_Index][0] = M_EPOXY_TRANSFER_X;
	COMI.mp_axisnum[m_nLinearMove_Index][1] = M_EPOXY_TRANSFER_Y;
	COMI.mp_axisnum[m_nLinearMove_Index][2] = M_EPOXY_TRANSFER_Z;



	m_nRobot_X = M_EPOXY_TRANSFER_X;
	m_nRobot_Y = M_EPOXY_TRANSFER_Y;
	m_nRobot_Z = M_EPOXY_TRANSFER_Z;
	m_nRobot_S = M_EPOXY_SCREW;

	mn_InitStep = 0;
	mn_RunStep = 0;
	mn_SafetyStep = 0;
}

CRun_Epoxy_Transfer_Robot::~CRun_Epoxy_Transfer_Robot()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_Epoxy_Transfer_Robot message handlers

void CRun_Epoxy_Transfer_Robot::Thread_Run()
{
	switch( st_work.mn_run_EpoxyStatus)
	{
	case dINIT:
		RunInit();
		break;

	case dRUN:
		RunMove();
		break;

	case dSTOP:

		break;

	case dWARNING:
		break;

	default:
		break;
	}

	st_work.mn_run_EpoxyStatus = st_work.mn_run_status;
	if( st_work.mn_run_status != dRUN )
	{
		if( st_work.nEpoxyBiliardThreadRunMode == dRUN ) st_work.mn_run_EpoxyStatus = dRUN;
	}
	else if( st_work.mn_run_status == dRUN)
	{
		if( st_work.nEpoxyBiliardThreadRunMode == dSTOP && st_work.nEpoxySuspendingMode == dSTOP )
			st_work.mn_run_EpoxyStatus = dSTOP;
	}
}


void CRun_Epoxy_Transfer_Robot::RunInit()
{
	int nRet_1 = 0;
// 	if( st_handler.mn_init_state[INIT_LD_ROBOT] == CTL_NO ) return;
//  	if( st_handler.mn_init_state[INIT_EMPTYTRAY_TRANSFER] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_EPOXY_ROBOT] != CTL_NO ) return;
// 	st_handler.mn_init_state[INIT_EPOXY_ROBOT] = CTL_YES;

	switch( mn_InitStep )
	{
		case 0:
			Func.VppmOff();
			mn_InitStep = 100;
			break;

		case 100:
			Run_EmptyTrayTransfer.Set_Tray_Remover_Z_UpDown( IO_ON );
			mn_InitStep = 110;
			break;
			
		case 110:
			nRet_1 = Run_EmptyTrayTransfer.Chk_Tray_Remover_Z_UpDown( IO_ON );
			if( nRet_1 == RET_GOOD )
			{
				mn_InitStep = 200;
			}
			else if( nRet_1 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 2001, dWARNING, m_strAlarmCode);
				mn_InitStep = 900;
			}
			break;

		case 200:
			nRet_1 = COMI.HomeCheck_Mot(m_nRobot_Z, st_motor[m_nRobot_Z].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 300;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 9001, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 300:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Epoxy_Y_Axis_Safety_Check, IO_OFF);//감지 off
			if( nRet_1 == IO_OFF )//감지 off
			{
				mn_InitStep = 310;//일단 벗어나자
			}
			else
			{
				mn_InitStep = 400;
			}
			break;

		case 310:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Epoxy_Y_Axis_Safety_Check, IO_OFF);//감지 off
			if( nRet_1 == IO_OFF )//감지 off
			{
				mn_InitStep = 320;//일단 벗어나자
			}
			else
			{
				mn_InitStep = 300;
			}
			break;

		case 320:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Epoxy_Y_Axis_Safety_Check, IO_ON);//감지 off
			if( nRet_1 == IO_ON )
			{
				COMI.Set_MotStop( 0, m_nRobot_Y);
				mn_InitStep = 300;
				break;
			}
			
			nRet_1 = COMI.Start_JogMove(m_nRobot_Y, PLUS);
			if( nRet_1 == BD_GOOD) 
			{
				m_dwInitWaitTime[0] = GetCurrentTime();
				mn_InitStep = 330;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_InitStep = 320;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9105, dWARNING, alarm.mstr_code);
				mn_InitStep = 320;
			}
			break;

		case 330:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Epoxy_Y_Axis_Safety_Check, IO_ON);//감지 off
			if( nRet_1 == IO_ON )
			{
				COMI.Set_MotStop( 0, m_nRobot_Y);
				mn_InitStep = 300;
				break;
			}
			m_dwInitWaitTime[1] = GetCurrentTime();
			m_dwInitWaitTime[2] = m_dwInitWaitTime[1] - m_dwInitWaitTime[0];
			if( m_dwInitWaitTime[2]  <= 0 )  m_dwInitWaitTime[0] = GetCurrentTime();
			if( m_dwInitWaitTime[2] < 20000 ) break;

			if( st_handler.cwnd_list != NULL )
			{
				sprintf(st_msg.c_abnormal_msg, "%s", "EPOXY y axis move time out error");
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
				COMI.Set_MotStop( 0, m_nRobot_Y);
				mn_InitStep = 900;				
			}
			
			break;

		case 400:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_REQ && 
				st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] == WORK_PICK)
			{
				mn_InitStep = 410;
			}
			break;

		case 410:
			nRet_1 = COMI.HomeCheck_Mot(m_nRobot_X, st_motor[m_nRobot_X].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 440;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 9002, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 440:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY], COMI.mn_manualspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_InitStep = 500;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_InitStep = 440;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence(9003, dWARNING, alarm.mstr_code);
			}
			break;

		case 500:
			nRet_1 = COMI.HomeCheck_Mot(m_nRobot_Y, st_motor[m_nRobot_Y].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 600;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 9004, dWARNING, COMI.mc_alarmcode);
			}
			break;
			
		case 600:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY], COMI.mn_manualspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_InitStep = 1000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_InitStep = 500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence(9005, dWARNING, alarm.mstr_code);
			}
			break;

		case 900:
			mn_InitStep = 0;
			st_handler.mn_initial_error = TRUE;
			break;
			
		case 1000:
			Func.VppmOff();
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_READY;
			st_handler.mn_init_state[INIT_EPOXY_ROBOT] = CTL_YES;
			mn_InitStep = 0;
			break;

	}
}

void CRun_Epoxy_Transfer_Robot::RunMove()
{
	int nRet_1,nRet_2;
	long lMotionDone=0;
	bool bSkip = false;
	
	Func.ThreadFunctionStepTrace(21, mn_RunStep);
	switch(mn_RunStep)
	{
	case -1:
		Func.VppmOff();
		mn_RunStep = 0;
		break;

	case 0:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			Func.VppmOff();
			mn_RunStep = 100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_RunStep = 0;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(9101, dWARNING, alarm.mstr_code);
			mn_RunStep = 0;
		}
		break;

	case 100:
		nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_LOADER_TRANSFER_Y_INIT_POS], COMI.md_allow_value[m_nRobot_X] );
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.md_allow_value[m_nRobot_Y] );
		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
		{
			mn_SafetyStep = 0;
			mn_RunStep = 200;
		}
		else
		{
			mn_RunStep = 1000;
		}
		break;

	case 200:
		nRet_1 = Robot_Move_Safety_Zone( 1, 0, 0 );
		if( nRet_1 == RET_GOOD )
		{
			mn_RunStep = 100;
		}
		break;

	case 1000:
		m_nFindLotNo_Flag = -1;
		if( g_lotMgr.GetLotCount() > 0 )
		{
			mn_RunStep = 1100;
		}
		break;

	case 1100:
		if( st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_REQ && st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] == WORK_PLACE )
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_READY;
			mn_RunStep = 1200;
		}
		break;

	case 1200:
		if( st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] == CTL_CHANGE)
		{
			mn_RunStep = 2000;
		}
		break;

	case 2000:
		if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_skip_flag[0] != CTL_YES || 
			st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_skip_flag[1] != CTL_YES ||
			st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_skip_flag[2] != CTL_YES )
		{
			if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[0] == CTL_YES && 
				st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[0] == BIN_CDIMM )
			{
				mn_BufferPos = 0;
				mn_RunStep = 2100;
			}
			else if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[1] == CTL_YES && 
				st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[1] == BIN_CDIMM )
			{
				mn_BufferPos = 1;
				mn_RunStep = 2100;
			}
			else if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[2] == CTL_YES && 
				st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[2] == BIN_CDIMM )
			{
				mn_BufferPos = 2;
				mn_RunStep = 2100;
			}
			mn_SafetyStep = 0;
		}
		if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[0] == CTL_YES ||
			st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[1] == CTL_YES ||
			st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].n_exist[2] == CTL_YES )
		{
			if( st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[0] == BIN_CDIMM ||
				st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[1] == BIN_CDIMM ||
				st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[2] == BIN_CDIMM )
			{

			}
			else
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_FREE;
				st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] = CTL_FREE;
				mn_RunStep = 0;
			}
		}
		else
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_FREE;
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] = CTL_FREE;
			mn_RunStep = 0;
		}	
		break;

	case 2100:
		nRet_1 = Robot_Move_Safety_Zone( 0, 0, 0 );
		if( nRet_1 == RET_GOOD )
		{
			mn_MoveStep = 0;
			mn_RunStep = 2200;
			ml_motflag[0] = CTL_NO;
			ml_motflag[1] = CTL_NO;
		}
		break;

	case 2200:
		nRet_1 = Move_Billiard_Epoxy( 0, mn_BufferPos);
		if( nRet_1 == RET_GOOD )
		{
			st_carrier_buff_info[TOPSHIFT_BUFF_EPOXY].nBin[mn_BufferPos] = BIN_EPOXY;
			mn_BufferPos++;
			if( mn_BufferPos >= st_recipe.nCarrierBuffer_Num )//단순 테스트 ->  3
			{
				mn_BufferPos = 0;
				mn_RunStep = 4000;
				mn_SafetyStep = 0;
			}
			else
			{
				mn_RunStep = 2000;
			}
			st_handler.m_nEpoxyCleanAlarm = CTL_NO;
			g_handler.AddEpoxyCnt();
		}
		break;

	case 4000:
		nRet_1 = Robot_Move_Safety_Zone( 1, 0, 0 );//nMode == 1  ==> 작업 완료후 안전위치로 이동
		if( nRet_1 == RET_GOOD )
		{
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][0] = CTL_FREE;
			st_sync.nCarrierRbt_Dvc_Req[THD_EPOXY_RBT][1] = CTL_FREE;
			mn_RunStep = 4100;
		}
		break;

	case 4100:
		mn_RunStep = 0;
		break;

	default:
		if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
		{
			sprintf(st_msg.c_abnormal_msg, "There is no Run_Step at Run_Epoxy_Transfer_Robot  = %d", mn_RunStep);
			st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
		}
		break;
	}
}

int CRun_Epoxy_Transfer_Robot::Move_Billiard_Epoxy( int nMode, int nSite)//, int nStartSecond)//nMode: billard or Dot, nSite : site,  nStartSecond: start or second
{
	int nRet_1,nRet_2;
	long lMotionDone=0;
	bool bSkip = false;
	int FuncRet = RET_PROCEED;
// 	double d_Feed;

// 	double dCurrY = COMI.Get_MotCurrentPos( m_nRobot_Y );
// 	if( dCurrY < ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] - (5* COMI.md_allow_value[m_nRobot_Y]) ) )
// 	{
// 		mn_MoveStep
// 	}

	m_nLinearMove_Index	= 1;	//현재 map index
	m_lAxisCnt	= 2;				 //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개
	m_lpAxisNum[0] = M_EPOXY_TRANSFER_X; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다
	m_lpAxisNum[1] = M_EPOXY_TRANSFER_Y;
	m_lpAxisNum[2] = M_EPOXY_TRANSFER_Z;
	
	COMI.ml_axiscnt[m_nLinearMove_Index] = 2;
	COMI.mp_axisnum[m_nLinearMove_Index][0] = M_EPOXY_TRANSFER_X;
	COMI.mp_axisnum[m_nLinearMove_Index][1] = M_EPOXY_TRANSFER_Y;
	COMI.mp_axisnum[m_nLinearMove_Index][2] = M_EPOXY_TRANSFER_Z;


	Func.ThreadFunctionStepTrace(22, mn_MoveStep);
	switch(mn_MoveStep)
	{
		case 0:
			mn_FirstSecond = 0;
			mn_StartEnd = 0;
			COMI.Set_MotStop(0, m_nRobot_S);
			mn_MoveStep = 100;
			break;

		case 100:
			if( COMI.Get_MotCurrentPos(m_nRobot_Z)  > ( st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z] ) )
			{
				mn_MoveStep = 200;
			}
			else
			{
				mn_MoveStep = 900;
			}
			break;

		case 200:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{
				mn_MoveStep = 100;
			}	
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9102, dWARNING, alarm.mstr_code);
				mn_MoveStep = 200;
			}
			break;

		case 900:
			Get_Billard_Pos( nSite);
			
			m_dpTargetPosList[0] = md_TargetAxisXValue[mn_FirstSecond][0];
			m_dpTargetPosList[1] = md_TargetAxisYValue[mn_FirstSecond][0];
			
			dp_SpdRatio[0] = (double)COMI.mn_runspeed_rate;//40;////st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)COMI.mn_runspeed_rate;//300;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)COMI.mn_runspeed_rate;//300;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 
			
			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio );		
			if( nRet_1 == BD_GOOD)
			{
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				mn_MoveStep = 1000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 900;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9103, dWARNING, alarm.mstr_code);
				mn_MoveStep = 900;
			}
			break;

		case 1000:
			Get_Billard_Pos( nSite);

			m_dpTargetPosList[0] = md_TargetAxisXValue[mn_FirstSecond][0];
			m_dpTargetPosList[1] = md_TargetAxisYValue[mn_FirstSecond][0];

			dp_SpdRatio[0] = (double)30;////st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)100;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)100;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio );		
			if( nRet_1 == BD_GOOD)
			{
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				mn_MoveStep = 1100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9103, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 1000;
			}
			break;

		case 1100:
			if( nSite <0 || nSite > 2 ) nSite = 0;
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_TOP_DOWN_POS + nSite], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{
				COMI.Set_MotStop(0, m_nRobot_S);
				m_dwSatbleWaitTime[0] = GetCurrentTime();
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				mn_MoveStep = 1200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9104, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 1100;
			}
			break;

		case 1200:
			m_dwSatbleWaitTime[1] = GetCurrentTime();
			m_dwSatbleWaitTime[2] = m_dwSatbleWaitTime[1] - m_dwSatbleWaitTime[0];
			if( m_dwSatbleWaitTime[2] <= 0 ) m_dwSatbleWaitTime[0] = GetCurrentTime();
			if( m_dwSatbleWaitTime[2] > st_recipe.dSatbleTime )
			{
				Func.VppmSet();//미리하는데 체크해야한다.
				mn_MoveStep = 1210;
			}
			break;

		case 1210:
			nRet_1 = COMI.Start_JogMove(m_nRobot_S, PLUS);// cmmSxVMoveStart(m_nRobot_S, PLUS);
			
			if( nRet_1 == BD_GOOD) 
			{
				Func.VppmSet();//미리하는데 체크해야한다.
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				mn_MoveStep = 1300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(10604, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 1000;
			}
			break;

		case 1300:
			Get_Billard_Pos( nSite);

			m_dpTargetPosList[0] = md_TargetAxisXValue[mn_FirstSecond][1];
			m_dpTargetPosList[1] = md_TargetAxisYValue[mn_FirstSecond][1];

			if(st_basic.n_mode_7387 == CTL_YES)
			{
				//dp_PosList[2] = st_recipe.nEpoxyCircleCount;
			}
			else
			{
				//dp_PosList[2] = 10;
			}

			dp_SpdRatio[0] = (double)st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 //3
			dp_SpdRatio[1] = (double)st_recipe.nEpoxyXYRunSpeed[1];	// 가속 //5
			dp_SpdRatio[2] = (double)st_recipe.nEpoxyXYRunSpeed[2];	// 감속 //5
					
			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio );
			//nRet_2 = COMI.Start_JogMove(m_nRobot_S, PLUS);// cmmSxVMoveStart(m_nRobot_S, PLUS);

			if( nRet_1 == BD_GOOD) 
			{
				COMI.Set_MotStop(0, m_nRobot_S);
				mn_MoveStep = 1400;
				m_dwSatbleWaitTime[0] = GetCurrentTime();
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9105, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 1000;
			}
			break;

		case 1400:
			Func.VppmOff();			
			COMI.Set_MotStop(0, m_nRobot_S);
			m_dwSatbleWaitTime[1] = GetCurrentTime();
			m_dwSatbleWaitTime[2] = m_dwSatbleWaitTime[1] - m_dwSatbleWaitTime[0];
			if( m_dwSatbleWaitTime[2] <= 0 ) m_dwSatbleWaitTime[0] = GetCurrentTime();
			if( m_dwSatbleWaitTime[2] < 1000 ) break;
			//2017.0616
			st_handler.m_nEpoxyCleanAlarm = CTL_NO;
			mn_MoveStep = 1500;
			break;

		case 1500:
			nRet_1 = COMI.Start_JogMove(m_nRobot_S, MINUS);
			//nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_S, -0.5 , st_recipe.nEpoxyRunSpeed);
			if( nRet_1 == BD_GOOD)
			{
				COMI.Set_MotStop(0, m_nRobot_S);
				//st_work.nEpoxyBiliardThreadRunMode = dSTOP;
// 				cmmStSetPosition(m_nRobot_S, cmCNT_COMM, (double)0.0);
// 				cmmStSetPosition(m_nRobot_S, cmCNT_FEED, (double)0.0);
// 				m_dwSatbleWaitTime[0] = GetCurrentTime();
				cmmStGetPosition(m_nRobot_S, cmCNT_FEED, &m_dcurr_pos[0]);	//현재 Feedback 위치 
				mn_MoveStep = 1510;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1500;
			}
// 			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
// 			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
// 				CTL_Lib.Alarm_Error_Occurrence(9106, dWARNING, alarm.mstr_code);
// 				mn_MoveStep = 1500;
// 			}
			break;

		case 1510:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_S, m_dcurr_pos[0] - 2 , st_recipe.nEpoxyRunSpeed);
			if( nRet_1 == BD_GOOD)
			{
				COMI.Set_MotStop(0, m_nRobot_S);
				cmmStSetPosition(m_nRobot_S, cmCNT_COMM, (double)0.0);
				cmmStSetPosition(m_nRobot_S, cmCNT_FEED, (double)0.0);
				m_dwSatbleWaitTime[0] = GetCurrentTime();
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				mn_MoveStep = 1600;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1510;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9196, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 1510;
			}
			break;

		case 1600:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{
				mn_MoveStep = 1700;
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1600;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9107, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 1600;
			}
			break;

		case 1700:
			mn_FirstSecond++;
			if( mn_FirstSecond >= 2)
			{
				mn_FirstSecond = 0;
				mn_dotPos = 0;
				mn_MoveStep = 2000;
				st_recipe.nEpoxyDotScrewCount = 20;
			}
			else
			{
				mn_MoveStep = 1000;
			}
			break;

			//design DOTDOTDOTDOT

		case 1900:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{	
				mn_MoveStep = 2000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 1900;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9108, dWARNING, alarm.mstr_code);
				mn_MoveStep = 1900;
			}
			break;

		case 2000:
			Get_Billard_Pos( nSite);

			m_dpTargetPosList[0] = md_TargetDotXValue[mn_dotPos];
			m_dpTargetPosList[1] = md_TargetDotYValue[mn_dotPos];

			dp_SpdRatio[0] = (double)30;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)100;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)100;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio );		
			if( nRet_1 == BD_GOOD)
			{
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				mn_MoveStep = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9109, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 2000;
			}
			break;

		case 2100:
			if( nSite < 0 || nSite > 2 ) nSite = 0;
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_TOP_DOWN_POS + nSite], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{	
				m_dwSatbleWaitTime[0] = GetCurrentTime();
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				mn_MoveStep = 2200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 2100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9110, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 2100;
			}
			break;

		case 2200:
			m_dwSatbleWaitTime[1] = GetCurrentTime();
			m_dwSatbleWaitTime[2] = m_dwSatbleWaitTime[1] - m_dwSatbleWaitTime[0];
			if( m_dwSatbleWaitTime[2] <= 0 ) m_dwSatbleWaitTime[0] = GetCurrentTime();
			if( m_dwSatbleWaitTime[2] > st_recipe.dSatbleTime )
			{
				Func.VppmSet();
// 				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				ml_motflag[0] = CTL_NO;
				ml_motflag[1] = CTL_NO;
				mn_MoveStep = 2300;
			}
			break;

		case 2300:
			//x Axis - md_TargetDotXValue[mn_dotPos];
			//Y- Axis md_TargetDotYValue[mn_dotPos];
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, md_TargetDotXValue[mn_dotPos], 5*COMI.md_allow_value[m_nRobot_X]);
			nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, md_TargetDotYValue[mn_dotPos], 5*COMI.md_allow_value[m_nRobot_Y]);
			if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				COMI.Set_MotStop( 0, m_nRobot_X);
				COMI.Set_MotStop( 0, m_nRobot_Y);
				mn_MoveStep = 1900;
				break;
			}

			Get_Billard_Pos( nSite);
			nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nRobot_X, md_TargetDotXValue[mn_dotPos], st_recipe.nEpoxyRunSpeed);
			nRet_2 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nRobot_S, st_recipe.nEpoxyDotScrewCount, st_recipe.nEpoxyRunSpeed);
			if( nRet_1 == BD_GOOD ) ml_motflag[0] = CTL_YES;
			if( nRet_2 == BD_GOOD ) ml_motflag[1] = CTL_YES;
			if( ml_motflag[0] == CTL_YES && ml_motflag[1] == CTL_YES )
			{
				ml_motflag[0] = CTL_NO;
				ml_motflag[1] = CTL_NO;		
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				mn_MoveStep = 2310;
			}
			else if( nRet_1 == BD_RETRY || nRet_2 == BD_RETRY )
			{
				mn_MoveStep = 2300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(9111, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 2300;
			}
			break;

		case 2310:
			Get_Billard_Pos( nSite);
			if( mn_dotPos < 2) nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_CHECK, m_nRobot_X, md_TargetDotXValue[mn_dotPos], st_recipe.nEpoxyRunSpeed);
			else               nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_CHECK, m_nRobot_Y, md_TargetDotYValue[mn_dotPos]/* - st_recipe.dEpoxyYOffSet */, st_recipe.nEpoxyRunSpeed);
			nRet_2 = CTL_Lib.Single_Move( ONLY_MOVE_CHECK, m_nRobot_S, st_recipe.nEpoxyDotScrewCount, st_recipe.nEpoxyRunSpeed);
			if( nRet_1 == BD_GOOD ) ml_motflag[0] = CTL_YES;
			if( nRet_2 == BD_GOOD ) ml_motflag[1] = CTL_YES;
			if( ml_motflag[0] == CTL_YES && ml_motflag[1] == CTL_YES )
			{
				Func.VppmOff();
				ml_motflag[0] = CTL_NO;
				ml_motflag[1] = CTL_NO;	
				COMI.Set_MotStop(0, m_nRobot_S);	
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				mn_MoveStep = 2400;
			}
			else if( nRet_1 == BD_RETRY || nRet_2 == BD_RETRY )
			{
				mn_MoveStep = 2310;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY || nRet_2 == BD_ERROR || nRet_2 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(9112, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 2310;
			}
			break;

		case 2400:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_S, st_recipe.nEpoxyDotScrewCount-2 , st_recipe.nEpoxyRunSpeed);
			if( nRet_1 == BD_GOOD)
			{
				COMI.Set_MotStop(0, m_nRobot_S);
// 				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				cmmStSetPosition(m_nRobot_S, cmCNT_COMM, (double)0.0);
				cmmStSetPosition(m_nRobot_S, cmCNT_FEED, (double)0.0);
				m_dwSatbleWaitTime[0] = GetCurrentTime();
				st_work.nEpoxyBiliardThreadRunMode = dRUN;
				mn_MoveStep = 2500;
				//2017.0616
				st_handler.m_nEpoxyCleanAlarm = CTL_NO;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 2400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9113, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 2400;
			}
			break;

		case 2500://Zaxis do a littlebit up
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 2600;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 2500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9114, dWARNING, alarm.mstr_code);
				st_work.nEpoxyBiliardThreadRunMode = dSTOP;
				mn_MoveStep = 2500;
			}
			break;

		case 2600:
			mn_dotPos++;
			if( mn_dotPos < 4 )
			{
				mn_MoveStep = 2000;
			}
			else
			{
				mn_dotPos = 0;
				mn_MoveStep = 3000;
			}
			break;

		case 3000:
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if( nRet_1 == BD_GOOD)
			{
				FuncRet = RET_GOOD;
				mn_MoveStep = 0;
			}	
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveStep = 200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9115, dWARNING, alarm.mstr_code);
				mn_MoveStep = 200;
			}
			break;
		}

		//m_nBillardWorking = CTL_YES;//작업시작	

		return FuncRet;
}

void CRun_Epoxy_Transfer_Robot::Get_Billard_Pos( int nPos )
{
	md_DvcGap = st_recipe.dLoaderTransferTrayDeviceGap * nPos;
	md_TargetAxisXValue[0][0] = st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_FIRST_START_POS];
	md_TargetAxisYValue[0][0] = st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_FIRST_START_POS]  - md_DvcGap;
	md_TargetAxisXValue[0][1] = st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_FIRST_END_POS];
	md_TargetAxisYValue[0][1] = st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_FIRST_END_POS] - md_DvcGap;
	md_TargetAxisXValue[1][0] = st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SECOND_START_POS];
	md_TargetAxisYValue[1][0] = st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SECOND_START_POS] - md_DvcGap;
	md_TargetAxisXValue[1][1] = st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SECOND_END_POS];
	md_TargetAxisYValue[1][1] = st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SECOND_END_POS] - md_DvcGap;

	md_TargetDotXValue[0] = st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_DOT_POS1];
	md_TargetDotYValue[0] = st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_DOT_POS1] - md_DvcGap;
	md_TargetDotXValue[1] = st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_DOT_POS2];
	md_TargetDotYValue[1] = st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_DOT_POS2] - md_DvcGap;
	md_TargetDotXValue[2] = st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_DOT_POS3];
	md_TargetDotYValue[2] = st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_DOT_POS3] - md_DvcGap;
	md_TargetDotXValue[3] = st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_DOT_POS4];
	md_TargetDotYValue[3] = st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_DOT_POS4] - md_DvcGap;
}

int CRun_Epoxy_Transfer_Robot::Robot_Move_Safety_Zone( int nMode, int n_site, int n_flag)
{
	int Ret=0, nRet_1=0, FuncRet = RET_PROCEED, i=0;
	double dCurrentPosX =0, dCurrentPosY=0;

	//nMode == 0 => Carrier 쪽으로 이동
	//nMode == 1  ==> 작업 완료후 안전위치로 이동
	//nMode == 2 => 작업자 쪽으로이동 ->Clean  DISCHARGE_POS

	//n_site 아직
	//n_flag == 아직
	dCurrentPosX = COMI.Get_MotCurrentPos( m_nRobot_X );
	dCurrentPosY = COMI.Get_MotCurrentPos( m_nRobot_Y );
	switch(mn_SafetyStep)
	{		 
		case 0:
			mn_SafetyStep = 100;
			break;

		case 100:
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.md_allow_value[m_nRobot_Z]);
			if( nRet_1 == RET_GOOD )
			{
				mn_SafetyStep = 1000;
			}
			else
			{
				mn_SafetyStep = 200;
			}
			break;

		case 200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_SafetyStep = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(9201, dWARNING, alarm.mstr_code);
				mn_SafetyStep = 200;
			}
			break;

		case 1000:			
			if( dCurrentPosY >= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - COMI.md_allow_value[m_nRobot_Y] ) )
			{
				if( dCurrentPosX >=  ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[m_nRobot_X] ) )
				{//Carrier영역  이영역에서는 안전영역이므로 linear로 이동이 가능하다
					if(nMode == 0)  //로보트가 Carrier로 들어가는 동작 
					{
						mn_SafetyStep = 1100;	//이미Carrier 안으로 들어가 있는 상태로 어느 위치에 있는지 확인한다 
					}
					else if( nMode == 1 )
					{
						mn_SafetyStep = 1200;
					}
					else //if(nMode == 2) //로보트가 Carrier 이외의 작업을 동작 할 예정
					{
						mn_SafetyStep = 2000;	//Y축을 Carrier를 빠져 나가 안전 위치까지 이동 시킨다 
					}
				}
				else//약간 왼쪽 영역이므로 일단 약간 오른쪽으로 이동 후 동작한다.
				{
					if(nMode == 0 )  //로보트가 Carrier로 들어가는 동작 
					{
						mn_SafetyStep = 1100;	//이미Carrier 안으로 들어가 있는 상태로 어느 위치에 있는지 확인한다 
					}
					else if(  nMode == 1)//안전 위칠 빠진다.
					{
						mn_SafetyStep = 5000;
					}
					else //if(nMode == 2) //로보트가 Carrier 이외의 작업을 동작 할 예정
					{
						mn_SafetyStep = 4000;	//Y축을 Carrier를 빠져 나가 안전 위치까지 이동 시킨다 
					}
				}
			}
			else //if( dCurrentPosY < ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] - COMI.md_allow_value[m_nRobot_Y] ) )
			{
				if( dCurrentPosX >=  ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[m_nRobot_X] ) )
				{//Carrier영역  이영역에서는 안전영역이므로 linear로 이동이 가능하다
					//10mm정도 떨어져 있어도 좋다고 본다 체크해야한다.
// 					if( dCurrentPosY > ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - ( 30 * COMI.md_allow_value[m_nRobot_Y] ) ) )
// 					{
						if(nMode == 0 || nMode == 1)  //로보트가 Carrier로 들어가는 동작 
						{
							mn_SafetyStep = 1220;	//이미Carrier 안으로 들어가 있는 상태로 어느 위치에 있는지 확인한다 
						}
						else //if(nMode == 2) //로보트가 Carrier 이외의 작업을 동작 할 예정
						{
							mn_SafetyStep =6000;	//Y축을 Carrier를 빠져 나가 안전 위치까지 이동 시킨다 
						}
// 					}
// 					else
// 					{//알람 트레이 트랜스퍼와 충돌 우려가 있다
// 						COMI.Set_MotStop( 0 , m_nRobot_X );
// 						COMI.Set_MotStop( 0 , m_nRobot_Y );
// 						COMI.Set_MotPower( m_nRobot_X, IO_OFF);
// 						COMI.Set_MotPower( m_nRobot_Y, IO_OFF);
// 						//960000 1 A "EPOXY_MOTOR_IS_NOT_SAFETY_MOVE_MANUALLY."
// 						CTL_Lib.Alarm_Error_Occurrence(9202, dWARNING, "960000");
// 						mn_SafetyStep = 0;
// 					}					
				}
				else//약간 왼쪽 영역이므로 일단 약간 오른쪽으로 이동 후 동작한다.
				{
					if(nMode == 0 || nMode == 1)  //로보트가 Carrier로 들어가는 동작 
					{
						mn_SafetyStep = 7000;	//이미Carrier 안으로 들어가 있는 상태로 어느 위치에 있는지 확인한다 
					}
					else //if(nMode == 1) //로보트가 Carrier 이외의 작업을 동작 할 예정
					{
						mn_SafetyStep = 8000;	//Y축을 Carrier를 빠져 나가 안전 위치까지 이동 시킨다 
					}
				}
			}
			break;			

		case 1100:
			mn_SafetyStep = 0;
			FuncRet = RET_GOOD;			
			break;

		case 1200:
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS];

			dp_SpdRatio[0] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 0;
				FuncRet = RET_GOOD;	
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 1200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 1200;
				CTL_Lib.Alarm_Error_Occurrence(9203, dWARNING, alarm.mstr_code);
			}
			break;

		case 1220:

			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 5000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 2000;
				CTL_Lib.Alarm_Error_Occurrence(9205, dWARNING, alarm.mstr_code);
			}
			break;

		case 2000://clean 영역으로 이동 DISCHARGE_POS
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY];

			dp_SpdRatio[0] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 2000;
				CTL_Lib.Alarm_Error_Occurrence(9204, dWARNING, alarm.mstr_code);
			}
			break;

		case 2100:
			if( dCurrentPosY >= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - (5*COMI.md_allow_value[m_nRobot_Y]) ) &&
				dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + (5*COMI.md_allow_value[m_nRobot_Y]) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 2200;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 2000;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					mn_SafetyStep = 2000;
					CTL_Lib.Alarm_Error_Occurrence(9205, dWARNING, alarm.mstr_code);
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 2200:
			if( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[m_nRobot_Y])  &&
				dCurrentPosX <= ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS] + COMI.md_allow_value[m_nRobot_Y]) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{					
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 2200;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(9206, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 2200;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 3000://Y가 safety위이고 x가 safety보다 작은데 위치할 경우 우선 safety로 이동하고 Init위치로 이동한다
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY];

			dp_SpdRatio[0] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 3100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 3000;
				CTL_Lib.Alarm_Error_Occurrence(9207, dWARNING, alarm.mstr_code);
			}
			break;

		case 3100:
			if( dCurrentPosY >= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] - (5*COMI.md_allow_value[m_nRobot_Y]) ) &&
				dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + (5*COMI.md_allow_value[m_nRobot_Y]) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 3100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					mn_SafetyStep = 3100;
					CTL_Lib.Alarm_Error_Occurrence(9208, dWARNING, alarm.mstr_code);
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 4000://Y가 safety위이고 x가 safety보다 작은데 위치할 경우 우선 safety로 이동하고 Init위치로 이동한다
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY];

			dp_SpdRatio[0] = (double)COMI.mn_runspeed_rate;//30;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)COMI.mn_runspeed_rate;//200;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)COMI.mn_runspeed_rate;//200;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 4100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 4000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 4000;
				CTL_Lib.Alarm_Error_Occurrence(9209, dWARNING, alarm.mstr_code);
			}
			break;

		case 4100:
			if( ( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[m_nRobot_Y]) ) &&
				( dCurrentPosX <= ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS] + COMI.md_allow_value[m_nRobot_Y]) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{					
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 4100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(9210, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 4100;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 5000:
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS];

			dp_SpdRatio[0] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 0;
				FuncRet = RET_GOOD;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 5000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 5000;
				CTL_Lib.Alarm_Error_Occurrence(9211, dWARNING, alarm.mstr_code);
			}
			break;

		case 6000:
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS];

			dp_SpdRatio[0] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[0];	//work 속도 
			dp_SpdRatio[1] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[1];	// 가속 
			dp_SpdRatio[2] = (double)COMI.mn_runspeed_rate;//st_recipe.nEpoxyXYRunSpeed[2];	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, dp_SpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				mn_SafetyStep = 6100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 6000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 6000;
				CTL_Lib.Alarm_Error_Occurrence(9212, dWARNING, alarm.mstr_code);
			}
			break;

		case 6100:
			if( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[m_nRobot_Y])  &&
				dCurrentPosX <= ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS] + COMI.md_allow_value[m_nRobot_Y]) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{					
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 6100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(9213, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 6100;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 7000:
			if( ( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_Y] ) ) &&
				( dCurrentPosX < ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 7100;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 7000;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(9214, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 7000;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 7100:
			if( ( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_Y] ) ) &&
				( dCurrentPosX < ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS] + COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 7200;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 7100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(9215, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 7100;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 7200:
			if( ( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_INIT_POS] + COMI.md_allow_value[m_nRobot_Y] ) ) &&
				( dCurrentPosX < ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS] + COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_INIT_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 7200;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(9216, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 7200;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;


		case 8000:
			if( ( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[m_nRobot_Y] ) ) &&
				( dCurrentPosX < ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 8100;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 8000;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(9217, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 8000;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;

		case 8100:
			if(	( dCurrentPosY <= ( st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[m_nRobot_Y] ) ) &&
				( dCurrentPosX < ( st_motor[m_nRobot_X].md_pos[P_EPOXY_TRANSFER_X_DISCHARGE_POS] + COMI.md_allow_value[m_nRobot_X] ) ) )
			{
				nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_EPOXY_TRANSFER_Y_DISCHARGE_POS], COMI.mn_runspeed_rate);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_SafetyStep = 0;
					FuncRet = RET_GOOD;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_SafetyStep = 8100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(9218, dWARNING, alarm.mstr_code);
					mn_SafetyStep = 8100;
				}
			}
			else
			{
				COMI.Set_MotStop( 0 , m_nRobot_X );
				COMI.Set_MotStop( 0 , m_nRobot_Y );
				mn_SafetyStep = 1000;
			}
			break;
	}


	return FuncRet;
}