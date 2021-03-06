// Run_UnPress_Robot.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_UnPress_Robot.h"
#include "Run_Device_Carrier_Robot.h"
#include "AMTLotManager.h"
#include "LogFromat.h"
#include "IO_Manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_UnPress_Robot
CRun_UnPress_Robot Run_UnPress_Robot;
IMPLEMENT_SERIAL(CRun_UnPress_Robot, CObject, 1);

CRun_UnPress_Robot::CRun_UnPress_Robot()
{
	m_nTrayDeviceGap = 0;
	m_nPressAxisX = M_CARRIER_X;
	m_nPressAxisY = M_PRESS_Y;
}

CRun_UnPress_Robot::~CRun_UnPress_Robot()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_UnPress_Robot message handlers
void CRun_UnPress_Robot::Thread_Run()
{

	switch( st_work.mn_run_status)
	{
		case dINIT:
			RunInit();
			break;

		case dRUN:
			RunMove();
			break;

		case dSTOP:

			CTL_Lib.mn_single_motmove_step[M_CARRIER_X] = 0;
			CTL_Lib.mn_single_motmove_step[M_PRESS_Y] = 0;
			break;

		case dWARNING:
			break;

		default:
			break;
	}
}

void CRun_UnPress_Robot::RunInit()
{
	int nRet_1 = 0, nRet_2 = 0, nRet_3 = 0;
	if( st_handler.mn_init_state[INIT_LD_ROBOT] == CTL_NO ) return;
 	if( st_handler.mn_init_state[INIT_ULD_ROBOT] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_UNPRESS_ROBOT] != CTL_NO ) return;
	
// 	st_handler.mn_init_state[INIT_UNPRESS_ROBOT] = CTL_YES;
	
	switch( mn_InitStep )
	{
		case 0:
			mn_InitStep = 10;
			break;
			
		case 10:
			Func.VppmOff();
			mn_InitStep = 100;
			break;

		case 100:
			nRet_1 = COMI.HomeCheck_Mot(m_nPressAxisY, st_motor[m_nPressAxisY].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 200;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 7001, dWARNING, COMI.mc_alarmcode);
			}
			break;
			
		case 200:
			Set_UnPress_PushClamp_OnOff(0, IO_OFF);//Z_UPDOWN
			Set_UnPress_PushClamp_OnOff(1, IO_OFF);//unpress
			mn_InitStep = 210;
			break;
			
		case 210:
			nRet_1 = Chk_UnPress_PushClamp_OnOff(0, IO_OFF );
			nRet_2 = Chk_UnPress_PushClamp_OnOff(1, IO_OFF );
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
			{
				mn_InitStep = 300;
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 7002, dWARNING, m_strAlarmCode);
			}
			break;

		case 300:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_REQ && 
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK)
			{
				mn_InitStep = 400;
			}
			break;

		case 400:
			nRet_1 = COMI.HomeCheck_Mot(m_nPressAxisX, st_motor[m_nPressAxisX].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 500;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 7003, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 500:
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_READY;
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] = WORK_PICK;
			mn_InitStep = 510;
			break;

		case 510:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_CHANGE && 
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK)
			{
				mn_InitStep = 600;
			}
			break;

		case 600:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nPressAxisX, st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_PUSH_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_InitStep = 700;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_InitStep = 600;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7004, dWARNING, alarm.mstr_code);
				mn_InitStep = 900;
			}
			break;

		case 700:
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_FREE;
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] = WORK_PICK;
			mn_InitStep = 710;
			break;

		case 710:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_NO && 
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == CTL_NO)
			{
				mn_InitStep = 800;
			}
			break;

		case 800:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nPressAxisX, st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_InitStep = 1000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_InitStep = 800;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7005, dWARNING, alarm.mstr_code);
				mn_InitStep = 900;
			}
			break;


		case 900:
			mn_InitStep = 0;
			st_handler.mn_initial_error = TRUE;
			break;
			
		case 1000:
			st_handler.mn_init_state[INIT_UNPRESS_ROBOT] = CTL_YES;
			mn_InitStep = 0;
			break;
		
		
	}
}

void CRun_UnPress_Robot::RunMove()
{
	int nRet_1,nRet_2,nRet_3,nRet_4;
	int nRet[9] = {0,};
	long lMotionDone=0;
	CString strTemp;

	double mCurrPosY = COMI.Get_MotCurrentPos( M_PRESS_Y );
	double mCurrPosX = COMI.Get_MotCurrentPos( M_CARRIER_X );

	Func.ThreadFunctionStepTrace(19, mn_RunStep);
	switch(mn_RunStep)
	{
		case -1:
			mn_RunStep = 0;
			break;

		case 0:
			mn_RunStep = 100;
			break;

// 			st_io.o_Press_Up_Sol							=	1200;//device 눌러줌 									///S1114
// 			st_io.o_Press_Down_Sol							=	1201;								///S1115
// 			st_io.i_Press_Up_Check							=	410;								///PS0314
// 			st_io.i_Press_Down_Check						=	411;
// 			st_io.o_Press_PIN_Guide_Forward_Sol				=	1202;
// 			st_io.o_Press_PIN_Guide_Bakcward_Sol			=	1203;
// 			st_io.i_Press_PIN_Guide_Forward_Check			=	500;
// 			st_io.i_Press_PIN_Guide_Backward_Check			=	501;
// 			st_io.o_Press_Carrier_Holder_Up_Sol				=	1204;							///S1112
// 			st_io.o_Press_Carrier_Holder_Down_Sol			=	1205;
// 			st_io.i_Press_Carrier_Holder_Up_Check			=	502;
// 			st_io.i_Press_Carrier_Holder_Down_Check			=	503;

		case 100:
			if( Func.Check_RunAllSafety() == RET_GOOD )
			{
				if( Chk_PressClamp_Safety(0) == RET_GOOD )
				{
					mn_RunStep = 200;
				}
			}
			break;

		case 200:
			if( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[m_nPressAxisY] ) )
			{
				mn_RunStep = 210;
				break;
			}
			else
			{
				if( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_INIT_POS] + COMI.md_allow_value[m_nPressAxisX] ) )
				{
					mn_RunStep = 300;
					break;
				}				
			}
			mn_RunStep = 500;
			break;

		case 210:
			if( Func.Check_RunAllSafety() != RET_GOOD || Chk_PressClamp_Safety(0) != RET_GOOD )
			{
				mn_RunStep = 100;
				COMI.Set_MotStop(0 , m_nPressAxisY);
				COMI.Set_MotStop(0 , m_nPressAxisX);
				break;
			}
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nPressAxisY, st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 210;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7100, dWARNING, alarm.mstr_code);
				mn_RunStep = 210;
			}
			break;

		case 300:
			if( Func.Check_RunAllSafety() != RET_GOOD || Chk_PressClamp_Safety(0) != RET_GOOD )
			{
				mn_RunStep = 100;
				COMI.Set_MotStop(0 , m_nPressAxisY);
				COMI.Set_MotStop(0 , m_nPressAxisX);
				break;
			}

			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nPressAxisY, st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 210;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7101, dWARNING, alarm.mstr_code);
				mn_RunStep = 210;
			}
			break;


		case 500:
			if( g_lotMgr.GetLotCount() > 0 || st_work.n_OnlyCarrierMove == CTL_YES)
			{
				mn_RunStep = 1000;
			}
			break;


		//////////////////////////////////////////////////////////////////////////
		//1. 작업영역이라서 UNPRESS 한다.
		//2. 한칸 미는 동작을 수행한다.
		//////////////////////////////////////////////////////////////////////////
		case 1000:
			//DVC Clamp confirm
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_REQ && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == 3)
			{
				if( Func.Check_RunAllSafety() == RET_GOOD )
				{
					if( Chk_PressClamp_Safety(5) == RET_GOOD )
					{
						st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_READY;
						mn_RunStep = 1100;
					}
				}
			}//작업영역이라서 UNPRESS 한다.
			else if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_REQ && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK )
			{
				if( Func.Check_RunAllSafety() == RET_GOOD )
				{
					if( Chk_PressClamp_Safety(5) == RET_GOOD )
					{
						st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_READY;
						mn_RunStep = 1100;
					}
				}
			}//한칸 민다
			else if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_REQ && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PLACE )
			{
				if( Func.Check_RunAllSafety() == RET_GOOD )
				{
					if( Chk_PressClamp_Safety(5) == RET_GOOD )//pusher 가 업 상태로 뒤로 빠진다 holder는 on 유지
					{
						st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_READY;
						mn_RunStep = 3000;
					}
				}
			}
			break;

		case 1100:
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(0, IO_ON);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(1, IO_ON);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(2, IO_ON);
			mn_RunStep = 1110;
			break;
			
		case 1110:
			nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix( 1, IO_ON );
			nRet_2 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix( 2, IO_ON );
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				mn_RunStep = 1120;
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
			{
			    CTL_Lib.Alarm_Error_Occurrence(7102, dWARNING, Run_Device_Carrier_Robot.m_strAlarmCode);
				mn_RunStep = 1100;
			}
			break;
			
		case 1120:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_CHANGE && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK )
			{
				for ( int i = 0 ; i < 3; i++ )
				{
					if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].n_exist[i] == CTL_YES )
					{
						st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][1] = GetCurrentTime();
						st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2] = st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][1] - st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][0];
						if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2] <= 0 ) st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][0] = GetCurrentTime();
						if( st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].dwBdTime[i][2] > (15*60*1000) )//15분 완료시간
						{
							st_carrier_buff_info[TOPSHIFT_BUFF_LOADER_RECEIVE].nBin[i] = BIN_GOOD;
							//kwlee 2017.0614
						//	st_handler.cwnd_main->PostMessage(WM_WORK_END, TOPSHIFT_BUFF_LOADER_RECEIVE, 0);
						}
						else
						{
							return;
						}
					}
				}
				st_sync.nCarrierSateyflag[UNPRESS_Y] = RET_ERROR;
				m_nTrayDeviceGap = 0;
				mn_RunStep = 2000;
			}
			else if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_CHANGE && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == 3 )
			{
				st_sync.nCarrierSateyflag[UNPRESS_Y] = RET_ERROR;
				m_nTrayDeviceGap = 0;
				mn_RunStep = 2000;
			}

			break;

		case 2000:
			//nMode = 0 전부 업하여 모터가 움직일때
			if( m_nTrayDeviceGap == 0 && Chk_PressClamp_Safety(5) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				break;
			}
			else
			{//nMode = 3 Holder down상태로 움직일때
				if( m_nTrayDeviceGap == 0 && Chk_PressClamp_Safety(5) == RET_GOOD )
				{

				}
				else
				{
					if( Chk_PressClamp_Safety(3) == RET_ERROR )
					{
						COMI.Set_MotStop( 0 , m_nPressAxisX);
						COMI.Set_MotStop( 0 , m_nPressAxisY);
						break;
					}
				}
			}

			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
// 			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
// 				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
// 			{
// 				break;
// 			}


			if( m_nTrayDeviceGap > 0 && ( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + COMI.md_allow_value[m_nPressAxisX] ) ||
				mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - COMI.md_allow_value[m_nPressAxisX] ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(7103, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			if( m_nTrayDeviceGap == 0 && ( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[m_nPressAxisY] ) ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(7104, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			if( m_nTrayDeviceGap > 0 )
			{
				if( ( mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + COMI.md_allow_value[m_nPressAxisX] ) ) &&
					( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - COMI.md_allow_value[m_nPressAxisX] ) ) )
				{
					mn_RunStep = 2100;
					break;
				}
			}		

			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS];
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nPressAxisX, m_dTargetPosX, (COMI.mn_runspeed_rate*1.5));
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 2010;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7105, dWARNING, alarm.mstr_code);
				mn_RunStep = 2000;
			}
			break;

		case 2010:
			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS];
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nPressAxisX, m_dTargetPosX, (COMI.mn_runspeed_rate*1.5));
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 2010;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7106, dWARNING, alarm.mstr_code);
				mn_RunStep = 2010;
			}
			break;

		//UNPRESS 실린더 다운시켜서 움직인다.
		case 2100:
// 			if( Chk_PressClamp_Safety(3) == RET_ERROR )
// 			{
// 				COMI.Set_MotStop( 0 , m_nPressAxisX);
// 				COMI.Set_MotStop( 0 , m_nPressAxisY);
// 				break;
// 			}

			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
// 			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
// 				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
// 			{
// 				break;
// 			}


			if( m_nTrayDeviceGap == 0 && ( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[m_nPressAxisY] ) ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(7107, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			Set_Device_Carrier_Holder(IO_ON);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(0, IO_ON);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(2, IO_ON);

			Set_UnPress_PushClamp_OnOff(0, IO_OFF);
			Set_UnPress_PushClamp_OnOff(1, IO_ON);
			mn_RunStep = 2200;
			break;

		case 2200:
			nRet_1 = Chk_UnPress_PushClamp_OnOff(0, IO_OFF);
			nRet_2 = Chk_UnPress_PushClamp_OnOff(1, IO_ON);
			if( nRet_1 == RET_GOOD && nRet_1 == RET_GOOD )
			{
				m_dwWaitUntil[0] = GetCurrentTime();
				mn_RunStep = 2210;
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(7108, dWARNING, m_strAlarmCode);
				break;
			}
			break;

		case 2210:
			m_dwWaitUntil[1] = GetCurrentTime();
			m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
			if( m_dwWaitUntil[2] <= 0 ) m_dwWaitUntil[0] = GetCurrentTime();
			if( m_dwWaitUntil[2] < 500 ) break;
			nRet[0] = g_ioMgr.get_in_bit( st_io.i_Press_Up_Check, IO_ON );
			nRet[1] = g_ioMgr.get_in_bit( st_io.i_Press_Down_Check, IO_OFF );
			nRet[2] = g_ioMgr.get_out_bit( st_io.o_Press_Up_Sol, IO_ON );
			nRet[3] = g_ioMgr.get_out_bit( st_io.o_Press_Down_Sol, IO_OFF );
			if( nRet[0] == IO_OFF && nRet[1] == IO_ON && nRet[2] == IO_OFF && nRet[3] == IO_ON)
			{
				nRet_1 = Chk_Device_Carrier_Holder(IO_ON);
				nRet_2 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix(2, IO_ON);
				if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
				{
					mn_RunStep = 2300;
					//2017.0425
					if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_CHANGE && 
						st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == 3 )
					{
						mn_RunStep = 2400;
					}
				}
				else if(nRet_1 == RET_ERROR ||nRet_2 == RET_ERROR)
				{
					CTL_Lib.Alarm_Error_Occurrence(7109, dWARNING, m_strAlarmCode);
					mn_RunStep = 2200;
				}
			}
			else
			{
				if( nRet[0] != IO_OFF || nRet[2] != IO_OFF)
				{
					strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
					CTL_Lib.Alarm_Error_Occurrence(7110, dWARNING, strTemp);
				}
				else// if( nRet[1] != IO_ON || nRet[3] != IO_ON)
				{
					strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
					CTL_Lib.Alarm_Error_Occurrence(7111, dWARNING, strTemp);
				}
				mn_RunStep = 2100;
			}

			break;
			
		case 2300:
			//UNPRESS 실린더가 다운상태에서 움직여야 한다.
			if( Chk_PressClamp_Safety(3) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
// 			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
// 				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
// 			{
// 				break;
// 			}


			if( m_nTrayDeviceGap >= 0 && ( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + COMI.md_allow_value[m_nPressAxisX] ) ||
				mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - COMI.md_allow_value[m_nPressAxisX] ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(7112, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			m_dTargetPosY = st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_PRESS1_POS] + (m_nTrayDeviceGap*st_recipe.dLoaderTransferTrayDeviceGap);
			nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nPressAxisY, m_dTargetPosY, COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 2310;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 2300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7113, dWARNING, alarm.mstr_code);
				mn_RunStep = 2300;
			}
			break;

		case 2310:
			//UNPRESS 실린더가 다운상태에서 움직여야 한다.
			if( Chk_PressClamp_Safety(3) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
// 			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
// 				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
// 			{
// 				break;
// 			}

			if( m_nTrayDeviceGap >= 0 && ( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + COMI.md_allow_value[m_nPressAxisX] ) ||
				mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - COMI.md_allow_value[m_nPressAxisX] ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(7114, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			m_dTargetPosY = st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_PRESS1_POS] + (m_nTrayDeviceGap*st_recipe.dLoaderTransferTrayDeviceGap);
			nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_CHECK, m_nPressAxisY, m_dTargetPosY, COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 2320;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 2300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7115, dWARNING, alarm.mstr_code);
				mn_RunStep = 2300;
			}
			break;

			//Site 하나씩 UNPRESS 한다.
		case 2320:
			if( g_ioMgr.get_in_bit(st_io.i_Press_UpDown_CellIn_Check, IO_ON) != IO_ON)
			{
				m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(7116, dWARNING, m_strAlarmCode);
				break;
			}
// 			Set_Device_Carrier_Holder(OFF);
			Set_UnPress_PushClamp_OnOff(0, IO_OFF);
			Set_UnPress_PushClamp_OnOff(1, IO_OFF);
			mn_RunStep = 2330;
			break;

		case 2330:
// 			if( Chk_Device_Carrier_Holder(OFF) == RET_GOOD)
// 			{
// 				mn_RunStep = 2400;
// 			}
			nRet_1 = Chk_UnPress_PushClamp_OnOff( 0, IO_OFF );
			nRet_2 = Chk_UnPress_PushClamp_OnOff( 1, IO_OFF );
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				mn_RunStep = 2400;
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(7117, dWARNING, m_strAlarmCode);
				mn_RunStep = 2320;
			}	
			break;

			//nMode = 1 Holder 상태에서  Press2로 움직인다.
		case 2400:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_CHANGE && 
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == 3 )
			{
				//UNPRESS 실린더가 다운상태에서 움직여야 한다.
				if( Chk_PressClamp_Safety(3) == RET_ERROR )
				{
					COMI.Set_MotStop( 0 , m_nPressAxisX);
					COMI.Set_MotStop( 0 , m_nPressAxisY);
					break;
				}
			}
			else
			{
				if( Chk_PressClamp_Safety(1) == RET_ERROR )
				{
					COMI.Set_MotStop( 0 , m_nPressAxisY);
					COMI.Set_MotStop( 0 , m_nPressAxisX);
					break;
				}
			}


			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
// 			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
// 				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
// 			{
// 				break;
// 			}
			if( ( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + COMI.md_allow_value[m_nPressAxisX] ) ) ||
				( mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - COMI.md_allow_value[m_nPressAxisX] ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(7118, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			m_dTargetPosY = st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_PRESS2_POS] + (m_nTrayDeviceGap*st_recipe.dLoaderTransferTrayDeviceGap);
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nPressAxisY, m_dTargetPosY, COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 2410;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 2400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7119, dWARNING, alarm.mstr_code);
				mn_RunStep = 2400;
			}
			break;	

			//Press 이동 후 Holder 실린더를 내린다.
		case 2410:
			Set_Device_Carrier_Holder(ON);
			Set_UnPress_PushClamp_OnOff(0, IO_OFF);
			Set_UnPress_PushClamp_OnOff(1, IO_ON);
			mn_RunStep = 2420;
			break;

		case 2420:
			nRet_1 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff(0, IO_OFF);
			nRet_2 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff(1, IO_ON);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
			{
				nRet[0] = g_ioMgr.get_in_bit( st_io.i_Press_Up_Check, IO_ON );
				nRet[1] = g_ioMgr.get_in_bit( st_io.i_Press_Down_Check, IO_OFF );
				nRet[2] = g_ioMgr.get_out_bit( st_io.o_Press_Up_Sol, IO_ON );
				nRet[3] = g_ioMgr.get_out_bit( st_io.o_Press_Down_Sol, IO_OFF );
				if( nRet[0] == IO_OFF && nRet[1] == IO_ON && nRet[2] == IO_OFF && nRet[3] == IO_ON)
				{
					nRet_1 = Chk_Device_Carrier_Holder(ON);
					if(nRet_1 == RET_GOOD)
					{
						mn_RunStep = 2500;
					}
					else if(nRet_1 == RET_ERROR)
					{
						CTL_Lib.Alarm_Error_Occurrence(7120, dWARNING, m_strAlarmCode);
						mn_RunStep = 2420;
					}
				}
				else
				{
					if( nRet[0] != IO_OFF || nRet[2] != IO_OFF)
					{
						strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
						CTL_Lib.Alarm_Error_Occurrence(7121, dWARNING, strTemp);
					}
					else if( nRet[1] != IO_ON || nRet[3] != IO_ON)
					{
						strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
						CTL_Lib.Alarm_Error_Occurrence(7122, dWARNING, strTemp);
					}
				}
			
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(7123, dWARNING, m_strAlarmCode);
				mn_RunStep = 2410;
			}
			break;

		case 2500:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_CHANGE && 
				st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == 3 )
			{
				//UNPRESS 실린더가 다운상태에서 움직여야 한다.
				//strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
				if( g_ioMgr.get_in_bit(st_io.i_Press_UpDown_CellIn_Check, IO_ON) == IO_OFF )
				{
					CTL_Lib.Alarm_Error_Occurrence(7214, dWARNING, "810412");
					break;
				}				
			}

			m_nTrayDeviceGap++;
			if( m_nTrayDeviceGap >= 3 )
			{// 전부 UNPRESS를 했으므로 원래 상태로돌린다
				m_nTrayDeviceGap = 0;
				mn_RunStep = 2550;
				Set_UnPress_PushClamp_OnOff(0, IO_OFF);
				Set_UnPress_PushClamp_OnOff(1, IO_ON);
			}
			else
			{
				mn_RunStep = 2000;
			}
			break;

		case 2510:
			Set_UnPress_PushClamp_OnOff(0, IO_OFF);
			Set_UnPress_PushClamp_OnOff(1, IO_ON);
			mn_RunStep = 2550;
			break;
			
		case 2550:
			nRet_1 = Chk_UnPress_PushClamp_OnOff(0, IO_OFF);
			nRet_2 = Chk_UnPress_PushClamp_OnOff(1, IO_ON);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
			{
				mn_RunStep = 2600;
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 7124, dWARNING, m_strAlarmCode);
				mn_RunStep = 2510;
			}
			break;

		case 2600:
			if( Chk_PressClamp_Safety(3) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			nRet[0] = g_ioMgr.get_in_bit( st_io.i_Press_Up_Check, IO_OFF );
			nRet[1] = g_ioMgr.get_in_bit( st_io.i_Press_Down_Check, IO_ON );
			nRet[2] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_X2_Up_Check, IO_ON );
			nRet[3] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_X2_Down_Check, IO_OFF );
			if( nRet[0] != IO_OFF || nRet[1] != IO_ON || nRet[2] != IO_ON || nRet[3] != IO_OFF)
			{
				mn_RunStep = 2510;
				break;
			}

			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
// 			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
// 				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
// 			{
// 				break;
// 			}

			if( ( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + COMI.md_allow_value[m_nPressAxisX] ) ) ||
				( mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - COMI.md_allow_value[m_nPressAxisX] ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(7125, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			m_dTargetPosY = st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nPressAxisY, m_dTargetPosY, COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 2610;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 2600;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7126, dWARNING, alarm.mstr_code);
				mn_RunStep = 2600;
			}
			break;
		
		case 2610:
// 			Set_Device_Carrier_Holder(OFF);
			Set_UnPress_PushClamp_OnOff(0, OFF);
			Set_UnPress_PushClamp_OnOff(1, OFF);
			mn_RunStep = 2620;
			break;

		case 2620:
// 			nRet_1 = Chk_Device_Carrier_Holder(OFF);
			nRet_1 = Chk_UnPress_PushClamp_OnOff(0, IO_OFF);
			nRet_2 = Chk_UnPress_PushClamp_OnOff(1, IO_OFF);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				mn_RunStep = 2700;
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence(7127, dWARNING, m_strAlarmCode);
				mn_RunStep = 2610;
			}
			break;

		case 2700:
			if( mCurrPosY > ( st_motor[m_nPressAxisX].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[m_nPressAxisX] )  )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(7128, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nPressAxisX, m_dTargetPosX, (COMI.mn_runspeed_rate*1.5) );
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 2710;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 2700;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7129, dWARNING, alarm.mstr_code);
				mn_RunStep = 2700;
			}
			break;

		case 2710:
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_FREE;
			mn_RunStep = 1000;
			st_sync.nCarrierSateyflag[UNPRESS_Y] = RET_GOOD;
			break;

		case 3000:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_CHANGE && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PLACE )
			{
				st_sync.nCarrierSateyflag[UNPRESS_Y] = RET_ERROR;
				mn_RunStep = 3100;
			}
			break;

		case 3100:
			//nMode = 0 전부 업하여 모터가 움직일때
			if( Chk_PressClamp_Safety(5) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
// 			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
// 				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
// 			{
// 				break;
// 			}

			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[m_nPressAxisY] ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(7130, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}	

			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_PUSHER_DOWN];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nPressAxisX, m_dTargetPosX, (COMI.mn_runspeed_rate*1.5));
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 3110;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 3100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7131, dWARNING, alarm.mstr_code);
				mn_RunStep = 3100;
			}
			break;

		case 3110:
			if( Chk_PressClamp_Safety(5) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
// 			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
// 				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
// 			{
// 				break;
// 			}
			if( m_nTrayDeviceGap == 0 && ( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[m_nPressAxisY] ) ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(7132, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(0, IO_OFF);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(1, IO_OFF);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(2, IO_OFF);

			Set_Device_Carrier_Holder( IO_OFF);//UNPRESS
			Set_UnPress_PushClamp_OnOff(0, IO_ON);
			Set_UnPress_PushClamp_OnOff(1, IO_OFF);
			mn_RunStep = 3120;
			break;

		case 3115:
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
// 			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
// 				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
// 			{
// 				break;
// 			}
			if( m_nTrayDeviceGap == 0 && ( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[m_nPressAxisY] ) ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(7132, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(0, IO_OFF);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(1, IO_OFF);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(2, IO_OFF);
			
			Set_Device_Carrier_Holder( IO_OFF);//UNPRESS
			Set_UnPress_PushClamp_OnOff(0, IO_ON);
			Set_UnPress_PushClamp_OnOff(1, IO_OFF);
			mn_RunStep = 3120;
			break;

		case 3120:
			nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix( 1, IO_OFF );
			nRet_2 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix( 2, IO_OFF );
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				nRet_1 = Run_UnPress_Robot.Chk_Device_Carrier_Holder(OFF);
				nRet_2 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff( 0, ON );
				nRet_3 = Run_UnPress_Robot.Chk_UnPress_PushClamp_OnOff( 1, OFF );

				if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD && nRet_3 == RET_GOOD )
				{
					mn_RunStep = 3200;
				}
				else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR || nRet_3 == RET_ERROR )
				{
					CTL_Lib.Alarm_Error_Occurrence( 7133, dWARNING, Run_Device_Carrier_Robot.m_strAlarmCode);
					mn_RunStep = 3115;
				}
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 7134, dWARNING, Run_Device_Carrier_Robot.m_strAlarmCode);
				mn_RunStep = 3115;
			}
			break;

		case 3200:
			//PRESS 실린더가 다운상태에서 움직여야 한다.
			if( Chk_PressClamp_Safety(2) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
			{
				break;
			}
			//2017.0425
			if( COMI.Get_MotCurrentPos( M_HEATSINK_INSPECT_Y) > ( st_motor[M_HEATSINK_INSPECT_Y].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS] + COMI.md_allow_value[M_HEATSINK_INSPECT_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y) > ( st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] + COMI.md_allow_value[M_HEATSINK_INSPECT_Y] ) )
			{
				break;
			}
			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[m_nPressAxisY] ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(7135, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_PUSH_POS];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nPressAxisX, m_dTargetPosX, COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_dwWaitUntil[0] = GetCurrentTime();
				mn_RunStep = 3300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 3200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7136, dWARNING, alarm.mstr_code);
				mn_RunStep = 3200;
			}
			break;

		case 3300:
			m_dwWaitUntil[1] = GetCurrentTime();
			m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
			if( m_dwWaitUntil[2] <= 0 ) m_dwWaitUntil[0] = GetCurrentTime();
			if( m_dwWaitUntil[2] > 2000 )//2017.0610
			{
				mn_RunStep = 3310; //Carrier X를 조금 두;로 뺀다. 실린더를 업하기 위해
				mn_RunStep = 3301;
			}
			break;

			//2017.0610
		case 3301:
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(2, IO_ON);
			mn_RunStep = 3302;
			break;

		case 3302:
			nRet_1 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix(2, IO_ON);
			
			if( nRet_1 == RET_GOOD )
			{
				mn_RunStep = 3310;
			}
			else if( nRet_1 == RET_ERROR )
			{
				 CTL_Lib.Alarm_Error_Occurrence(7940, dWARNING, Run_Device_Carrier_Robot.m_strAlarmCode);
				mn_RunStep = 3301;
			}
			break;

		case 3310://실린더 고정
			Set_Device_Carrier_Holder(IO_ON);
			mn_RunStep = 3320;
			break;

		case 3320:
			nRet_1 = Chk_Device_Carrier_Holder(IO_ON);
			if( nRet_1 == RET_GOOD)
			{
				m_dwWaitUntil[0] = GetCurrentTime();
				mn_RunStep = 3400;
			}
			else if( nRet_1 == RET_ERROR )
			{
				CTL_Lib.Alarm_Error_Occurrence( 7137, dWARNING, m_strAlarmCode);
				mn_RunStep = 3310;
			}
			break;

		case 3400:
			m_dwWaitUntil[1] = GetCurrentTime();
			m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
			if( m_dwWaitUntil[2] <= 0 ) m_dwWaitUntil[0] = GetCurrentTime();
			if( m_dwWaitUntil[2] < 1000 ) break;//2017.0610
			//PRESS 실린더가 다운상태에서 Holder가 On되어 고정시킨다.

			if( Chk_PressClamp_Safety(4) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
			{
				break;
			}
			//2017.0425
			if( COMI.Get_MotCurrentPos( M_HEATSINK_INSPECT_Y) > ( st_motor[M_HEATSINK_INSPECT_Y].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS] + COMI.md_allow_value[M_HEATSINK_INSPECT_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y) > ( st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] + COMI.md_allow_value[M_HEATSINK_INSPECT_Y] ) )
			{
				break;
			}

			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[m_nPressAxisY] ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(7138, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_BACK_PUSH_POS];
			nRet_1 = CTL_Lib.Single_Move( ONLY_MOVE_START, m_nPressAxisX, m_dTargetPosX, COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_dwWaitUntil[0] = GetCurrentTime();
				mn_RunStep = 3410;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 3400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7139, dWARNING, alarm.mstr_code);
				mn_RunStep = 3400;
			}
			break;

		case 3410:
			Set_Device_Carrier_Holder(IO_ON);
			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(2, IO_ON);
			Set_UnPress_PushClamp_OnOff( 0, IO_OFF);
			Set_UnPress_PushClamp_OnOff( 1, IO_OFF);
			mn_RunStep = 3420;
			break;

		case 3420:
			nRet_1 = Chk_Device_Carrier_Holder(IO_ON);
			nRet_2 = Chk_UnPress_PushClamp_OnOff( 0, IO_OFF );
			nRet_3 = Chk_UnPress_PushClamp_OnOff( 1, IO_OFF );
			nRet_4 = Run_Device_Carrier_Robot.Chk_Device_Carrier_HolderPin_Fix(2, IO_ON);

			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD && nRet_3 == RET_GOOD && nRet_4 == RET_GOOD )
			{
				mn_RunStep = 3500;
			}
			else if( nRet_1 == RET_ERROR || nRet_2 == RET_ERROR || nRet_3 == RET_ERROR || nRet_4 == RET_ERROR)
			{
				if( nRet_4 == RET_ERROR) CTL_Lib.Alarm_Error_Occurrence(7140, dWARNING, Run_Device_Carrier_Robot.m_strAlarmCode);
				else					 CTL_Lib.Alarm_Error_Occurrence(7141, dWARNING, m_strAlarmCode);
				mn_RunStep = 3410;//2017.0610
			}
			break;

		case 3500:
			//PRESS 실린더가 다운상태에서 움직여야 한다.
			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + COMI.md_allow_value[m_nPressAxisY] ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(7142, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			//2017.0425
			if( COMI.Get_MotCurrentPos( M_UNLOADER_TRANSFER_X) > ( st_motor[M_UNLOADER_TRANSFER_X].md_pos[P_UNLOADER_TRANSFER_X_INREADY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_LOADER_TRANSFER_Y) > ( st_motor[M_LOADER_TRANSFER_Y].md_pos[P_LOADER_TRANSFER_Y_READY_POS] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_X) < ( st_motor[M_EPOXY_TRANSFER_X].md_pos[P_EPOXY_TRANSFER_X_SAFETY] - COMI.md_allow_value[M_EPOXY_TRANSFER_X] ) ||
				COMI.Get_MotCurrentPos( M_EPOXY_TRANSFER_Y) > ( st_motor[M_EPOXY_TRANSFER_Y].md_pos[P_EPOXY_TRANSFER_Y_SAFETY] + COMI.md_allow_value[M_EPOXY_TRANSFER_Y] ))
			{
				break;
			}
			
			if( COMI.Get_MotCurrentPos( M_HEATSINK_INSPECT_Y) > ( st_motor[M_HEATSINK_INSPECT_Y].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS] + COMI.md_allow_value[M_HEATSINK_INSPECT_Y] ) )
			{
				break;
			}
			if( COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y) > ( st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] + COMI.md_allow_value[M_HEATSINK_INSPECT_Y] ) )
			{
				break;
			}

			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nPressAxisX, m_dTargetPosX, COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 3600;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 3500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(7143, dWARNING, alarm.mstr_code);
				mn_RunStep = 3500;
			}
			break;

		case 3600:
			st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_FREE;
			st_sync.nCarrierSateyflag[UNPRESS_Y] = RET_GOOD;
			mn_RunStep = 1000;
			break;	


		default:
			if (st_handler.cwnd_list != NULL)	// 리스트 바 화면 존재
			{
				sprintf(st_msg.c_abnormal_msg, "There is no Run_Up at Run_Device_Carrier_Robot  = %d", mn_RunStep);
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청
			}
			break;
	}
}

//nMode = 0 전부 업하여 모터가 움직일때
//nMode = 1 Press Holder 상태에서 움직일때
//nMode = 2 Press down상태로 밀때
//nMode = 3 Holder down상태로 움직일때
//nMode = 4 Press down상태로 민 상태에서 홀더 고정
//nMode = 5 Holder 상태에서 Pusher 가 뒤로움직일때
int CRun_UnPress_Robot::Chk_PressClamp_Safety( int nMode)
{
	int nRet[13] = {0,};
	CString strTemp;
	int nFuncRet = RET_PROCEED;


	nRet[0] = g_ioMgr.get_in_bit( st_io.i_Press_Up_Check, IO_ON );
	nRet[1] = g_ioMgr.get_in_bit( st_io.i_Press_Down_Check, IO_ON );
	nRet[2] = g_ioMgr.get_out_bit( st_io.o_Press_Up_Sol, IO_ON );
	nRet[3] = g_ioMgr.get_out_bit( st_io.o_Press_Down_Sol, IO_ON );
	nRet[4] = g_ioMgr.get_in_bit( st_io.i_Press_Carrier_Holder_Up_Check, IO_ON );
	nRet[5] = g_ioMgr.get_in_bit( st_io.i_Press_Carrier_Holder_Down_Check, IO_ON );
	nRet[6] = g_ioMgr.get_out_bit( st_io.o_Press_Carrier_Holder_Up_Sol, IO_ON );
	nRet[7] = g_ioMgr.get_out_bit( st_io.o_Press_Carrier_Holder_Down_Sol, IO_ON );
	nRet[8] = g_ioMgr.get_in_bit(st_io.i_Press_UpDown_CellIn_Check, IO_OFF);
	nRet[9] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_X2_Up_Check, IO_ON );
	nRet[10] = g_ioMgr.get_in_bit( st_io.i_Slide_Guide_X2_Down_Check, IO_ON );
	nRet[11] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_X2_Forward_Sol, IO_ON );//up
	nRet[12] = g_ioMgr.get_out_bit( st_io.o_Slide_Guide_X2_Backward_Sol, IO_ON );//down

	if( nMode == 0 )
	{
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_OFF && 
			nRet[4] == IO_ON && nRet[5] == IO_OFF && nRet[6] == IO_ON && nRet[7] == IO_OFF && nRet[8] == IO_OFF )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			if( nRet[0] != IO_ON || nRet[2] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7201, dWARNING, strTemp);
			}
			else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7202, dWARNING, strTemp);
			}
			else if( nRet[4] != IO_ON || nRet[6] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7203, dWARNING, strTemp);					
			}
			else if( nRet[8] != IO_OFF )
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_UpDown_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(7204, dWARNING, strTemp);					
			}
			else//nRet[6] != IO_ON  nRet[7] != IO_OFF
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7205, dWARNING, strTemp);
			}
			nFuncRet = RET_ERROR;
		}
	}//nMode = 1 Press Holder 상태에서 움직일때
	else if( nMode == 1)
	{
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_OFF && 
			nRet[4] == IO_OFF && nRet[5] == IO_ON && nRet[6] == IO_OFF && nRet[7] == IO_ON && /*nRet[8] == IO_ON &&*/
			nRet[9] == IO_ON && nRet[10] == IO_OFF && nRet[11] == IO_ON && nRet[12] == IO_OFF)
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			if( nRet[0] != IO_ON || nRet[2] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7206, dWARNING, strTemp);
			}
			else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7207, dWARNING, strTemp);
			}
			else if( nRet[4] != IO_OFF || nRet[6] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7208, dWARNING, strTemp);					
			}
			else if(nRet[5] != IO_ON || nRet[7] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7210, dWARNING, strTemp);
			}
// 			else if( nRet[8] != IO_ON )
// 			{
// 				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
// 				CTL_Lib.Alarm_Error_Occurrence(7209, dWARNING, strTemp);					
// 			}
			else if( nRet[9] != IO_ON || nRet[11] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Slide_Guide_X2_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7213, dWARNING, strTemp);					
			}
			else// if( nRet[10] != IO_OFF || nRet[12] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Slide_Guide_X2_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7263, dWARNING, strTemp);					
			}

			nFuncRet = RET_ERROR;
		}
	}
	else if( nMode == 2 )// Press down상태로 밀때
	{
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_OFF && 
			nRet[4] == IO_ON && nRet[5] == IO_OFF && nRet[6] == IO_ON && nRet[7] == IO_OFF && nRet[8] == IO_OFF &&
			nRet[9] == IO_OFF && nRet[10] == IO_ON && nRet[11] == IO_OFF && nRet[12] == IO_ON)
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			if( nRet[0] != IO_ON || nRet[2] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7211, dWARNING, strTemp);
			}
			else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7212, dWARNING, strTemp);
			}
			else if( nRet[4] != IO_ON || nRet[6] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7213, dWARNING, strTemp);					
			}
			else if(nRet[5] != IO_ON  || nRet[7] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7215, dWARNING, strTemp);
			}
			else if( nRet[8] != IO_OFF )
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(7214, dWARNING, strTemp);					
			}
			else if( nRet[9] != IO_OFF || nRet[11] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Slide_Guide_X2_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7213, dWARNING, strTemp);					
			}
			else// if( nRet[10] != IO_ON || nRet[12] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Slide_Guide_X2_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7213, dWARNING, strTemp);					
			}
			nFuncRet = RET_ERROR;
		}
	}
	else if( nMode == 3)
	{
		if( nRet[0] == IO_OFF && nRet[1] == IO_ON && nRet[2] == IO_OFF && nRet[3] == IO_ON && 
			nRet[4] == IO_OFF && nRet[5] == IO_ON && nRet[6] == IO_OFF && nRet[7] == IO_ON /*&& nRet[8] == IO_OFF*/ )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			if( nRet[0] != IO_OFF || nRet[2] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7216, dWARNING, strTemp);
			}
			else if( nRet[1] != IO_ON || nRet[3] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7217, dWARNING, strTemp);
			}
			else if( nRet[4] != IO_OFF || nRet[6] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7218, dWARNING, strTemp);					
			}
			else// if(nRet[5] != IO_ON || nRet[7] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Carrier_Holder_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7220, dWARNING, strTemp);
			}
// 			else if( nRet[8] != IO_OFF )
// 			{
// 				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
// 				CTL_Lib.Alarm_Error_Occurrence(7219, dWARNING, strTemp);					
// 			}
			nFuncRet = RET_ERROR;
		}
	}
	else if( nMode == 4 )// Press down 민상태에서 홀더 고정
	{
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_OFF && 
			nRet[4] == IO_OFF && nRet[5] == IO_ON && nRet[6] == IO_OFF && nRet[7] == IO_ON && nRet[8] == IO_OFF &&
			nRet[9] == IO_OFF && nRet[10] == IO_ON && nRet[11] == IO_OFF && nRet[12] == IO_ON)
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			if( nRet[0] != IO_ON || nRet[2] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7211, dWARNING, strTemp);
			}
			else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7212, dWARNING, strTemp);
			}
			else if( nRet[4] != IO_OFF || nRet[6] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7213, dWARNING, strTemp);					
			}
			else if(nRet[5] != IO_ON  || nRet[7] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7215, dWARNING, strTemp);
			}
			else if( nRet[8] != IO_OFF )
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(7214, dWARNING, strTemp);					
			}
			else if( nRet[9] != IO_OFF || nRet[12] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Slide_Guide_X2_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7213, dWARNING, strTemp);					
			}
			else// if( nRet[10] != IO_OFF || nRet[11] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Slide_Guide_X2_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7213, dWARNING, strTemp);					
			}
			nFuncRet = RET_ERROR;
		}
	}
	else if( nMode == 5)
	{//Pusher가 되로 빠질떄는 
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_OFF && 
			/*nRet[4] == IO_OFF && nRet[5] == IO_ON && nRet[6] == IO_OFF && nRet[7] == IO_ON &&*/ nRet[8] == IO_OFF &&
			nRet[9] == IO_ON && nRet[10] == IO_OFF && nRet[11] == IO_ON && nRet[12] == IO_OFF)
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			if( nRet[0] != IO_ON || nRet[2] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7206, dWARNING, strTemp);
			}
			else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7207, dWARNING, strTemp);
			}
// 			else if( nRet[4] != IO_OFF || nRet[6] != IO_OFF)
// 			{
// 				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Up_Check);
// 				CTL_Lib.Alarm_Error_Occurrence(7208, dWARNING, strTemp);					
// 			}
// 			else if(nRet[5] != IO_ON || nRet[7] != IO_ON)
// 			{
// 				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check);
// 				CTL_Lib.Alarm_Error_Occurrence(7210, dWARNING, strTemp);
// 			}
			else if( nRet[8] != IO_OFF )
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(7209, dWARNING, strTemp);					
			}
			else if( nRet[9] != IO_ON || nRet[11] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Slide_Guide_X2_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(7213, dWARNING, strTemp);					
			}
			else// if( nRet[10] != IO_OFF || nRet[12] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Slide_Guide_X2_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(7213, dWARNING, strTemp);					
			}
			
			nFuncRet = RET_ERROR;
		}
	}
	else
	{
		//"901000 1 A "THERE_IS_NO_MODE_MOVING_PRESSRBT_IN_SAFETY_CLAMP."
		CTL_Lib.Alarm_Error_Occurrence(7221, dWARNING, "901000");
		nFuncRet = RET_ERROR;
	}

	return nFuncRet;
}


int CRun_UnPress_Robot::Chk_PressMotor_Safety()
{
	int nFuncRet = RET_GOOD;
	double dInterval = 10.0;

	int nRet_1 = g_ioMgr.get_in_bit(st_io.i_Press_UpDown_CellIn_Check, IO_OFF);
	if( nRet_1 == IO_ON && ( COMI.Get_MotCurrentPos(M_PRESS_Y) >= st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS] - ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS] + ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) ) &&
		( COMI.Get_MotCurrentPos(M_PRESS_Y) >= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS]+st_recipe.dLoaderTransferTrayDeviceGap) - ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS]+st_recipe.dLoaderTransferTrayDeviceGap) + ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) ) &&
		( COMI.Get_MotCurrentPos(M_PRESS_Y) >= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS]+2*st_recipe.dLoaderTransferTrayDeviceGap) - ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS]+2*st_recipe.dLoaderTransferTrayDeviceGap) + ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) ) )
	{
		nFuncRet = RET_ERROR;
	}

	if( nRet_1 == IO_ON && ( COMI.Get_MotCurrentPos(M_PRESS_Y) >= st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS] - ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS] + ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) ) &&
		( COMI.Get_MotCurrentPos(M_PRESS_Y) >= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS]+st_recipe.dLoaderTransferTrayDeviceGap) - ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS]+st_recipe.dLoaderTransferTrayDeviceGap) + ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) ) &&
		( COMI.Get_MotCurrentPos(M_PRESS_Y) >= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS]+2*st_recipe.dLoaderTransferTrayDeviceGap) - ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS]+2*st_recipe.dLoaderTransferTrayDeviceGap) + ( dInterval * COMI.md_allow_value[M_PRESS_Y] ) ) )
	{
		nFuncRet = RET_ERROR;
	}
	return nFuncRet;
}

int CRun_UnPress_Robot::Process_Unpress_Pickup( int nMode, int nWorkSite, CString strLotNo)
{
	return RET_PROCEED;
}

// nMode == 0 Press holder
// nMode == 1 Vision Holder
void CRun_UnPress_Robot::Set_Device_Carrier_Holder( int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);


	m_bCarrierHolder = false;
	m_dwCarrierHolder[0] = GetCurrentTime();


	//ON->DOWN OFF->UP
	g_ioMgr.set_out_bit(st_io.o_Press_Carrier_Holder_Down_Sol, OnOff);
	g_ioMgr.set_out_bit(st_io.o_Press_Carrier_Holder_Up_Sol, !OnOff);



	if (OnOff == IO_ON)//DOWN FWD
	{
		clsLog.LogFunction(_T("DVC_UNPRESS_ROBOT"),_T("DOWN"),0,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DVC_UNPRESS_ROBOT"),_T("UP"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);			
	}
}

int CRun_UnPress_Robot::Chk_Device_Carrier_Holder( int nOnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_CARRIER_HOLDER_UPDN;

	int nRet_1 = 0, nRet_2 = 0;

	//ON->down OFF->up
	nRet_1 = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Up_Check, !nOnOff);
	nRet_2 = g_ioMgr.get_in_bit(st_io.i_Press_Carrier_Holder_Down_Check, nOnOff);



	if (nOnOff == IO_OFF)//UP
	{
		if (m_bCarrierHolder == false && nRet_1 == IO_ON && nRet_2 == IO_OFF )
		{
			m_bCarrierHolder = true;
			m_dwCarrierHolder[0]	= GetCurrentTime();
		}
		else if (m_bCarrierHolder == true && nRet_1 == IO_ON && nRet_2 == IO_OFF )
		{
			m_dwCarrierHolder[1] = GetCurrentTime();
			m_dwCarrierHolder[2] = m_dwCarrierHolder[1] - m_dwCarrierHolder[0];

			if (m_dwCarrierHolder[2] <= 0)
			{
				m_dwCarrierHolder[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierHolder[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCarrierHolder[1] = GetCurrentTime();
			m_dwCarrierHolder[2] = m_dwCarrierHolder[1] - m_dwCarrierHolder[0];

			if (m_dwCarrierHolder[2] <= 0)
			{
				m_dwCarrierHolder[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierHolder[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Press_Carrier_Holder_Up_Check); 
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("DOWN"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bCarrierHolder == false && nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			m_bCarrierHolder = true;
			m_dwCarrierHolder[0]	= GetCurrentTime();
		}
		else if (m_bCarrierHolder == true && nRet_1 == IO_OFF && nRet_2 == IO_ON )
		{
			m_dwCarrierHolder[1]	= GetCurrentTime();
			m_dwCarrierHolder[2]	= m_dwCarrierHolder[1] - m_dwCarrierHolder[0];

			if (m_dwCarrierHolder[2] <= 0)
			{
				m_dwCarrierHolder[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwCarrierHolder[2]> (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCarrierHolder[1]	= GetCurrentTime();
			m_dwCarrierHolder[2]	= m_dwCarrierHolder[1] - m_dwCarrierHolder[0];

			if (m_dwCarrierHolder[2] <= 0)
			{
				m_dwCarrierHolder[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCarrierHolder[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Press_Carrier_Holder_Down_Check); 
				clsLog.LogFunction(_T("DVC_CARRIER_ROBOT"),_T("UP"),1,_T("CLAMP_HOLDER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}

//nMode == 0 z축 pusher
//nMode == 1 press unpress
void CRun_UnPress_Robot::Set_UnPress_PushClamp_OnOff( int nMode, int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bPressUpDnFlag[nMode]	= false;
	m_dwPressUpDn[nMode][0]	= GetCurrentTime();

	if( nMode == 0 )//미는 실린더
	{
		g_ioMgr.set_out_bit( st_io.o_Slide_Guide_X2_Backward_Sol, OnOff);
		g_ioMgr.set_out_bit( st_io.o_Slide_Guide_X2_Forward_Sol, !OnOff);
	}
	else//unpress 실린더
	{
		g_ioMgr.set_out_bit( st_io.o_Press_Down_Sol, OnOff);
		g_ioMgr.set_out_bit( st_io.o_Press_Up_Sol, !OnOff);
	}


	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("DVC_CARRIER_PRESS_ROBOT"),_T("DOWN"),0,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DVC_CARRIER_PRESS_ROBOT"),_T("UP"),0,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

int CRun_UnPress_Robot::Chk_UnPress_PushClamp_OnOff( int nMode, int OnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_CARRIER_PRESS_UPDN;

	int nRet1, nRet2;

	if( nMode == 0 )
	{
		nRet1 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, OnOff);
		nRet2 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, !OnOff);
	}
	else
	{
		nRet1 = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, OnOff);
		nRet2 = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, !OnOff);
	}

	if (OnOff == IO_OFF)
	{
		if (m_bPressUpDnFlag[nMode] == false && nRet1 == IO_OFF && nRet2 == IO_ON )
		{
			m_bPressUpDnFlag[nMode]		= true;
			m_dwPressUpDn[nMode][0]	= GetCurrentTime();
		}
		else if (m_bPressUpDnFlag[nMode] == true && nRet1 == IO_OFF && nRet2 == IO_ON  )
		{
			m_dwPressUpDn[nMode][1] = GetCurrentTime();
			m_dwPressUpDn[nMode][2] = m_dwPressUpDn[nMode][1] - m_dwPressUpDn[nMode][0];

			if (m_dwPressUpDn[nMode][2] <= 0)
			{
				m_dwPressUpDn[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwPressUpDn[nMode][2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_CARRIER_PRESS_ROBOT"),_T("UP"),1,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwPressUpDn[nMode][1] = GetCurrentTime();
			m_dwPressUpDn[nMode][2] = m_dwPressUpDn[nMode][1] - m_dwPressUpDn[nMode][0];

			if (m_dwPressUpDn[nMode][2] <= 0)
			{
				m_dwPressUpDn[nMode][0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwPressUpDn[nMode][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Press_Up_Check);
				clsLog.LogFunction(_T("DVC_CARRIER_PRESS_ROBOT"),_T("UP"),1,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bPressUpDnFlag[nMode] == false && nRet1 ==  IO_ON && nRet2 == IO_OFF  )
		{
			m_bPressUpDnFlag[nMode]			= true;
			m_dwPressUpDn[nMode][0]	= GetCurrentTime();
		}
		else if (m_bPressUpDnFlag[nMode] == true && nRet1 ==  IO_ON && nRet2 == IO_OFF )
		{
			m_dwPressUpDn[nMode][1]	= GetCurrentTime();
			m_dwPressUpDn[nMode][2]	= m_dwPressUpDn[nMode][1] - m_dwPressUpDn[nMode][0];

			if (m_dwPressUpDn[nMode][2] <= 0)
			{
				m_dwPressUpDn[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwPressUpDn[nMode][2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_CARRIER_PRESS_ROBOT"),_T("DOWN"),1,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwPressUpDn[nMode][1]	= GetCurrentTime();
			m_dwPressUpDn[nMode][2]	= m_dwPressUpDn[nMode][1] - m_dwPressUpDn[nMode][0];

			if (m_dwPressUpDn[nMode][2] <= 0)
			{
				m_dwPressUpDn[nMode][0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwPressUpDn[nMode][2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_Press_Down_Check); 
				clsLog.LogFunction(_T("DVC_CARRIER_PRESS_ROBOT"),_T("DOWN"),1,_T("PRESS"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}