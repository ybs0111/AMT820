// Run_HeatSinkVision_Transfer_Robot.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "Run_HeatSinkVision_Transfer_Robot.h"
#include "AMTLotManager.h"
#include "IO_Manager.h"
#include "FastechPublic_IO.h"
#include "LogFromat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRun_HeatSinkVision_Transfer_Robot
CRun_HeatSinkVision_Transfer_Robot Run_HeatSinkVision_Transfer_Robot;
IMPLEMENT_SERIAL(CRun_HeatSinkVision_Transfer_Robot, CObject, 1);

CRun_HeatSinkVision_Transfer_Robot::CRun_HeatSinkVision_Transfer_Robot()
{

	m_nLinearMove_Index	=2 ;	//현재 map index
	m_lAxisCnt	= 2;				 //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개
	m_lpAxisNum[0] = M_HEATSINK_TRANSFER_X; //m_lpAxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다
	m_lpAxisNum[1] = M_HEATSINK_TRANSFER_Y;
	m_lpAxisNum[2] = M_HEATSINK_TRANSFER_Z;

	m_nRobot_X = M_HEATSINK_TRANSFER_X;
	m_nRobot_Y = M_HEATSINK_TRANSFER_Y;
	m_nRobot_Z = M_HEATSINK_TRANSFER_Z;
	m_nRobot_P = M_HEATSINK_PICKER_PITCH;

	COMI.ml_axiscnt[m_nLinearMove_Index] = 2;
	COMI.mp_axisnum[m_nLinearMove_Index][0] = M_HEATSINK_TRANSFER_X;
	COMI.mp_axisnum[m_nLinearMove_Index][1] = M_HEATSINK_TRANSFER_Y;
	COMI.mp_axisnum[m_nLinearMove_Index][2] = M_HEATSINK_TRANSFER_Z;

	m_nRobot_DisY = M_DISPENSER_Y;
	m_nRobot_VisY = M_HEATSINK_INSPECT_Y;
	m_nRobot_VisZ = M_HEATSINK_INSPECT_Z;
	m_nPickerPara = PICKER_PARA;
	mn_InitStep = 0;
	mn_RunHsStep = -1;
	mn_RunVisStep = -1;
	mn_MoveMeasureStep = 0;

	int i =0, m = 0, x = 0, y = 0;
	for( i = 0; i <m_nPickerPara; i++ )
	{
		st_picker[THD_HEATSINK_RBT].st_pcb_info[i].nYesNo = CTL_NO;
		st_picker[THD_HEATSINK_RBT].st_pcb_info[i].nBin = CTL_NO;
		st_picker[THD_HEATSINK_PRBT].st_pcb_info[i].nYesNo = CTL_NO;
		st_picker[THD_HEATSINK_PRBT].st_pcb_info[i].nBin = CTL_NO;		
		st_picker[THD_LD_HEATSINK_BUFF].st_pcb_info[i].nYesNo = CTL_NO;
		st_picker[THD_LD_HEATSINK_BUFF].st_pcb_info[i].nBin = CTL_NO;
		st_picker[THD_PICK_HEATSINK_DVC].st_pcb_info[i].nYesNo = CTL_NO;
		st_picker[THD_PICK_HEATSINK_DVC].st_pcb_info[i].nBin = CTL_NO;
		st_picker[THD_PLACE_HEATSINK_DVC].st_pcb_info[i].nYesNo = CTL_NO;
		st_picker[THD_PLACE_HEATSINK_DVC].st_pcb_info[i].nBin = CTL_NO;
		st_picker[THD_PLACE_CARRIER_DVC].st_pcb_info[i].nYesNo = CTL_NO;
		st_picker[THD_PLACE_CARRIER_DVC].st_pcb_info[i].nBin = CTL_NO;
	}

	for(m = 0; m < 2/*st_recipe.nTrayNum*/; m++)
	{
		for(y = 0; y < MAX_TRAY_ROW; y++) //Y 방향 정보
		{
			for(x = 0; x < MAX_TRAY_COL; x++) //X 방향 정보 
			{ 
				st_tray_info[THD_LD_HEATSINK_BUFF].st_dvc_info[m][y][x].nYesNo = CTL_YES;
			}
		}
	}
	m_nCarriorPos = 0;
	m_nVisCarriorPos = 0;
}

CRun_HeatSinkVision_Transfer_Robot::~CRun_HeatSinkVision_Transfer_Robot()
{
}

/////////////////////////////////////////////////////////////////////////////
// CRun_HeatSinkVision_Transfer_Robot message handlers
void CRun_HeatSinkVision_Transfer_Robot::Thread_Run()
{
	switch( st_work.mn_run_status)
	{
		case dINIT:
			RunInit();
			break;
			
		case dRUN:
			RunMoveHeatSink();
			RunMoveVision();
			RunMoveDispensor();
			break;
			
		case dSTOP:
			break;
			
		case dWARNING:
			break;
			
		default:
			break;
	}
}

void CRun_HeatSinkVision_Transfer_Robot::RunInit()
{
	int nRet_1 = 0, nRet_2 = 0;
	if( st_handler.mn_init_state[INIT_LD_ROBOT] == CTL_NO ) return;
 	if( st_handler.mn_init_state[INIT_ULD_ROBOT] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_EPOXY_ROBOT] == CTL_NO ) return;
	if( st_handler.mn_init_state[INIT_HEAT_SINK_ROBOT] != CTL_NO ) return;
	st_handler.mn_init_state[INIT_HEAT_SINK_ROBOT] = CTL_YES;
	st_handler.mn_init_state[INIT_DISPENSOR_ROBOT] = CTL_YES;
	st_handler.mn_init_state[INIT_VISION_ROBOT] = CTL_YES;
	
	switch( mn_InitStep )
	{
		case 0:
			Set_Dispenser_Air_Blow(IO_OFF);
			mn_InitStep = 100;
			break;

		case 100:
			nRet_1 = COMI.HomeCheck_Mot(m_nRobot_Z, st_motor[m_nRobot_Z].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				Set_HeatSink_Vacuum_OnOff(IO_ON);
				mn_InitStep = 200;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2004, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 200:
			Set_HeatSink_Transfer_Pad_UpDown(IO_OFF);
			mn_InitStep = 210;
			break;

		case 210:
			nRet_1 = Chk_HeatSink_Transfer_Pad_UpDown(IO_OFF);
			if( nRet_1 == RET_GOOD)
			{
				mn_InitStep = 220;
			}
			else if(nRet_1 == RET_ERROR )
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2004, dWARNING, m_strAlarmCode);
			}
			break;

		case 220:
			nRet_1 = Chk_HeatSink_Vacuum_OnOff(IO_OFF);
			if( nRet_1 == RET_GOOD)
			{
				Set_HeatSink_Vacuum_OnOff(IO_OFF);
				mn_InitStep = 300;
			}
			else if( nRet_1 == RET_ERROR )
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2004, dWARNING, m_strAlarmCode);
			}
			break;

		case 300:
			nRet_1 = COMI.HomeCheck_Mot(m_nRobot_P, st_motor[m_nRobot_P].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 310;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2004, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 310:
			nRet_1 = COMI.HomeCheck_Mot(m_nRobot_X, st_motor[m_nRobot_X].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 320;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2004, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 320:
			nRet_1 = COMI.HomeCheck_Mot(m_nRobot_Y, st_motor[m_nRobot_Y].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 330;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2004, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 330:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_InitStep = 400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_InitStep = 330;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_InitStep = 900;
			}
			break;

		case 400:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_InitStep = 500;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_InitStep = 400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_InitStep = 900;
			}
			break;

		case 500:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_REQ && 
				st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][1] == WORK_PICK)
			{
				mn_InitStep = 510;
			}
			break;

		case 510:
			nRet_1 = COMI.HomeCheck_Mot(m_nRobot_VisZ, st_motor[m_nRobot_VisZ].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 520;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2004, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 520:
			Set_Device_CameraY_Jig_Press_ForwardBackward(IO_OFF);
			mn_InitStep = 530;
			break;

		case 530:
			nRet_1 = Chk_Device_CameraY_Jig_Press_ForwardBackward(IO_OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_InitStep = 600;
			}
			else if( nRet_1 == RET_ERROR )
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2005, dWARNING, IO_OFF);
			}
			break;

			
		case 600:
			nRet_1 = COMI.HomeCheck_Mot(m_nRobot_VisY, st_motor[m_nRobot_VisY].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 610;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2004, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 610:
			Set_Vision_Y_Clamp_ForeardBackward(IO_OFF);
			mn_InitStep = 620;
			break;

		case 620:
			nRet_1 = Chk_Vision_Y_Clamp_ForeardBackward(IO_OFF);
			if( nRet_1 == RET_GOOD)
			{
				mn_InitStep = 700;
			}
			else if( nRet_1 == RET_ERROR )
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2005, dWARNING, m_strAlarmCode);
			}
			break;

		case 700:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_0_Check, IO_ON);
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_180_Check, IO_OFF);
			if( nRet_1 == IO_ON && nRet_2 == IO_OFF)
			{
				mn_InitStep = 710;
			}
			else
			{
				mn_InitStep = 710;
			}
			break;

		case 710:
			Set_HeatSink_Reverse_Clamp_ForwardBackward(IO_OFF);
			mn_InitStep = 720;
			break;

		case 720:
			nRet_1 = Chk_HeatSink_Reverse_Clamp_ForwardBackward(IO_OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_InitStep = 730;
			}
			else if( nRet_1 == RET_ERROR )
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2005, dWARNING, m_strAlarmCode);
			}
			break;

		case 730:
			Set_HeatSink_Reverse_UpDown(IO_ON);
			mn_InitStep = 740;
			break;

		case 740:
			nRet_1 = Chk_HeatSink_Reverse_UpDown(IO_ON);
			if( nRet_1 == RET_GOOD )
			{
				mn_InitStep = 750;
			}
			else if( nRet_1 == RET_ERROR )
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2005, dWARNING, m_strAlarmCode);
			}
			break;

		case 750:
			Set_HeatSink_Reverse_Turn(IO_OFF);
			mn_InitStep = 760;
			break;

		case 760:
			nRet_1 = Chk_HeatSink_Reverse_Turn(IO_OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_InitStep = 770;
			}
			else if( nRet_1 == RET_ERROR )
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2005, dWARNING, IO_OFF);
			}
			break;

		case 770:
			Set_HeatSink_Reverse_UpDown(IO_OFF);
			mn_InitStep = 780;
			break;
			
		case 780:
			nRet_1 = Chk_HeatSink_Reverse_UpDown(IO_OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_InitStep = 800;
			}
			else if( nRet_1 == RET_ERROR )
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2005, dWARNING, IO_OFF);
			}
			break;

		case 800:
			nRet_1 = COMI.HomeCheck_Mot(m_nRobot_DisY, st_motor[m_nRobot_DisY].mn_homecheck_method, MOTTIMEOUT);
			if( nRet_1 == BD_GOOD)
			{
				mn_InitStep = 810;
			}
			else if( nRet_1 == BD_ERROR)
			{
				mn_InitStep = 900;
				CTL_Lib.Alarm_Error_Occurrence( 2004, dWARNING, COMI.mc_alarmcode);
			}
			break;

		case 810:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Device_Check, IO_ON);//B접점
			if( nRet_1 == IO_OFF)
			{
				m_strAlarmCode.Format("8%d%04d", IO_ON, st_io.i_Dispenser_Device_Check);
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, m_strAlarmCode);
				mn_InitStep = 900;
			}
			else
			{
				mn_InitStep = 1000;
			}
			break;
			

		case 900:
			mn_InitStep = 0;
			st_handler.mn_initial_error = TRUE;
			break;
			
		case 1000:
			st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_READY;
			st_handler.mn_init_state[INIT_DISPENSOR_ROBOT] = CTL_YES;
			st_handler.mn_init_state[INIT_VISION_ROBOT] = CTL_YES;
			st_handler.mn_init_state[INIT_HEAT_SINK_ROBOT] = CTL_YES;
			mn_InitStep = 0;
			break;

	}
}

void CRun_HeatSinkVision_Transfer_Robot::RunMoveVision()
{
	int nRet_1;
	long lMotionDone=0;
	bool bSkip = false;

	Func.ThreadFunctionStepTrace(35, mn_RunVisStep);
	switch(mn_RunVisStep)
	{
		case -1:
			m_nTotalVError = 0;
			mn_RunVisStep = 0;
			break;

		case 0:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisZ, st_motor[m_nRobot_VisZ].md_pos[P_HEATSINK_INSPECT_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunVisStep = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunVisStep = 0;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunVisStep = 0;
			}
			break;

		case 100:
			Set_Device_Carrier_Camera_Y_Press_UpDown(IO_OFF);
			mn_RunVisStep = 110;
			break;

		case 110:
			nRet_1 = Chk_Device_Carrier_Camera_Y_Press_UpDown( IO_OFF);
			if( nRet_1 == IO_ON )
			{
				mn_RunVisStep = 200;
			}
			break;

		case 200:
			Set_Device_CameraY_Jig_Press_ForwardBackward(IO_OFF);
			mn_RunVisStep = 210;
			break;

		case 210:
			nRet_1 = Chk_Device_CameraY_Jig_Press_ForwardBackward( IO_OFF);
			if( nRet_1 == IO_ON )
			{
				mn_RunVisStep = 300;
			}
			break;

		case 300:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisY, st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunVisStep = 1000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunVisStep = 300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunVisStep = 300;
			}
			break;

		case 1000:
			if( g_lotMgr.GetLotCount() > 0 )
			{
				mn_RunVisStep = 1100;
			}
			break;

		case 1100:
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_REQ && st_sync.nHeatSinkRbt_Dvc_Req[THD_VISION_RBT][1] == WORK_PLACE )
			{
				st_sync.nHeatSinkRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_READY;
				mn_RunVisStep = 1200;
			}
			break;

		case 1200:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] == CTL_CHANGE && st_sync.nHeatSinkRbt_Dvc_Req[THD_VISION_RBT][1] == WORK_PLACE )
			{
				m_nVisCarriorPos = 0;
				m_nVisTeachPos = 0;
				mn_RunVisStep = 2000;
			}
			break;

		case 1800:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisZ, st_motor[m_nRobot_VisZ].md_pos[P_HEATSINK_INSPECT_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunVisStep = 2000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunVisStep = 1800;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunVisStep = 1800;
			}
			break;

		case 1900:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisY, st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunVisStep = 2000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunVisStep = 1900;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunVisStep = 1900;
			}
			break;


		case 2000:
			if( COMI.Get_MotCurrentPos(m_nRobot_VisZ) > (st_motor[m_nRobot_VisZ].md_pos[P_HEATSINK_INSPECT_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_VisZ]) )
			{
				mn_RunVisStep = 1800;
				break;
			}
			if( COMI.Get_MotCurrentPos(m_nRobot_VisY) > (st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_VisY]) )
			{
				mn_RunVisStep = 1900;
				break;
			}

			if(st_basic.n_3874InspectMode == 0)
			{
				Set_Device_Carrier_Camera_LED_LAMP_OnOff(IO_ON);
				Set_Device_Carrier_Camera_Y_Press_UpDown(IO_ON);
			}
			mn_RunVisStep = 2100;
			break;

		case 2100:			
			if(st_basic.n_3874InspectMode == 0)
			{
				nRet_1 = Chk_Device_Carrier_Camera_Y_Press_UpDown(IO_ON);
				if( nRet_1 == RET_GOOD )
				{

				}
				else if( nRet_1 == RET_ERROR )
				{
					CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, m_strAlarmCode);
				}
				else
				{
					break;
				}				
			}
			mn_RunVisStep = 2110;
			break;

		case 2110:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisY, st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Y_VISION_TOP_POS + m_nVisCarriorPos], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunVisStep = 2200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunVisStep = 2110;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunVisStep = 2110;
			}
			break;

		case 2200:
			st_vision.bHeatsinkMeasureCmp = FALSE;
			st_vision.bEpoxyMeasureCmp = FALSE;
			st_vision.nEpoxyMeasureResultFlag = -1;
			st_vision.nHeatSinkMeasureResultFlag = -1;
			Func.SendEpoxyMeasureStart();
			mn_RunVisStep = 2300;
			break;

		case 2300:
			if( st_vision.bEpoxyMeasureCmp == TRUE )
			{
				mn_RunVisStep = 2310;
			}
			break;

		case 2310:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisY, st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunVisStep = 2400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunVisStep = 2310;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunVisStep = 2310;
			}
			break;

		case 2400:
			Set_Device_Carrier_Camera_LED_LAMP_OnOff(IO_OFF);
			mn_RunVisStep = 2500;
			break;

		case 2500:
			st_vision.bHeatsinkMeasureCmp = FALSE;
			st_vision.bEpoxyMeasureCmp = FALSE;
			st_vision.nEpoxyMeasureResultFlag = -1;
			st_vision.nHeatSinkMeasureResultFlag = -1;
			Func.SendHeatsinkMeasureStart(m_nVisTeachPos);
			mn_RunVisStep = 2600;
			break;

		case 2600:
			if( st_vision.bHeatsinkMeasureCmp == TRUE )
			{
				mn_RunVisStep = 2700;
			}
			break;

		case 2700:





			Func.SendHeatsinkMeasureStart(m_nVisTeachPos);
			mn_RunVisStep = 2800;
			break;

		case 2800:
			break;

		case 2900:
			break;

		case 5000:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_VisY, st_motor[m_nRobot_VisY].md_pos[P_HEATSINK_INSPECT_Y_PRESS_START_POS] + (m_nVisCarriorPos*1), COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunVisStep = 5100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunVisStep = 5000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunVisStep = 5000;
			}
			break;

		case 5100:
			Set_Device_Carrier_Camera_Y_Press_UpDown(IO_OFF);
			break;


		case 6000:
			m_nVisCarriorPos++;
			if( m_nVisCarriorPos >= 3)
			{

			}
			else
			{
				mn_RunVisStep = 2000;
			}


	}
}

int CRun_HeatSinkVision_Transfer_Robot::Process_Measure_Vision()
{
	int nRet_1,nRet_2, nFuncRet = RET_PROCEED;
	long lMotionDone=0;
	bool bSkip = false;
	char Jamcode[10];
	CString strAlarm;
	
	Func.ThreadFunctionStepTrace(29, mn_MoveMeasureStep);
	switch(mn_MoveMeasureStep)
	{
		case  0:
			m_nContinusVError = 0;
			mn_MoveMeasureStep = 1000;
			break;

		case 1000:
			//nMode == 2 => Dispensor로 자재를 Pick할 수 있는 위치로 이동
			nRet_1 = Robot_AutoMove_Safety_Zone( 1, 0 );
			if( nRet_1 == RET_GOOD )
			{
				mn_MoveMeasureStep = 1100;
			}
			break;

		case 1100:
			if( COMI.Get_MotCurrentPos(m_nRobot_X) <= ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[m_nRobot_X].mn_allow ) )
			{

				m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_INSPECT_POS];
				m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_INSPECT_POS];
				
				m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
				m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
				m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속 
				
				nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_MoveMeasureStep = 1200;
					
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_MoveMeasureStep = 1100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
					mn_MoveMeasureStep = 1100;
				}
			}
			else
			{
				mn_MoveMeasureStep = 1000;
			}
			break;

		case 1200:
			if( COMI.Get_MotCurrentPos(m_nRobot_X) <= ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[m_nRobot_X].mn_allow ) )
			{
				nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INSPECT_POS], COMI.mn_runspeed_rate);
				if (nRet_1 == BD_GOOD) //좌측으로 이동
				{
					mn_MoveMeasureStep = 2000;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_MoveMeasureStep = 1200;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
					CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
					mn_MoveMeasureStep = 1200;
				}
			}
			else
			{
				mn_MoveMeasureStep = 1000;
			}
			break;

		case 2000:
			if( st_basic.n_mode_7387 == CTL_YES )
			{
				st_vision.b7387MeasureCmp = false;
				st_vision.n7387MeasureResultFlag = -1;
				
				Func.Send7387MeasureStart();
				m_dwMCameraResWaitTime[0] = GetCurrentTime();
				mn_MoveMeasureStep = 2100;
			}
			else
			{
				mn_MoveMeasureStep = 2200;
			}
			break;

		case 2100:
			m_dwMCameraResWaitTime[1] = GetCurrentTime();
			m_dwMCameraResWaitTime[2] = m_dwMCameraResWaitTime[1] - m_dwMCameraResWaitTime[0];
			if( m_dwMCameraResWaitTime[2] <= 0 ) m_dwMCameraResWaitTime[0] = GetCurrentTime();

			if( st_vision.b7387MeasureCmp == TRUE )
			{
				mn_MoveMeasureStep = 2200;
			}
			else
			{
				if( m_dwMCameraResWaitTime[2] > 10000 )
				{//941000 1 A "MEASURE_VISION_IS_NOT_RESPONSE."
					CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, "941000");
				}
			}
			break;

		case 2200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_MoveMeasureStep = 3000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveMeasureStep = 2200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
				mn_MoveMeasureStep = 2200;
			}
			break;

		case 3000:
			if( st_basic.n_mode_7387 == CTL_YES )
			{

				if(st_vision.n7387MeasureResultFlag == CTL_NO || st_basic.n_7387InspectAlarmSkipMode == CTL_YES )
				{
					st_vision.n7387MeasureAlarmCnt = 0;
					mn_MoveMeasureStep = 4000;
				}
				else if(st_vision.n7387MeasureResultFlag > 0 && st_basic.n_7387InspectAlarmSkipMode == 0)
				{	
					st_vision.n7387MeasureAlarmCnt++;
					st_basic.n7387AlarmCount++;
					if(m_nContinusVError > st_basic.n7387AlarmCount)
					{
						m_nContinusVError = 0;
						//941001 1 A "VISION_MEASURE_7387_ERROR_HEATSINK_TRANSFER.";						
						CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, "941001");

					}
					else if(m_nTotalVError > st_vision.n7387MeasureAlarmCnt)
					{
						m_nTotalVError = 0;
						sprintf(Jamcode, "110037");//20130825
						
						//941001 1 A "VISION_MEASURE_7387_ERROR_HEATSINK_TRANSFER.";						
						CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, "941001");
					}
					else
					{
						//HeatSink 버리러 가기
						mn_MoveMeasureStep = 3100;
						
					}
				}
			}
			else
			{
				mn_MoveMeasureStep = 4000;
			}
			break;

		case 3010:
			//nMode == 2 => Dispensor로 자재를 Pick할 수 있는 위치로 이동
			nRet_1 = Robot_AutoMove_Safety_Zone( 1, 0 );
			if( nRet_1 == RET_GOOD )
			{
				mn_MoveMeasureStep = 3100;
			}
			break;

		case 3100:
			if( COMI.Get_MotCurrentPos(m_nRobot_X) <= ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[m_nRobot_X].mn_allow ) )
			{
				
				m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_INSPECT_GABAGE_POS];
				m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_INSPECT_GABAGE_POS];
				
				m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
				m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
				m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속 
				
				nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio);
				if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
				{
					mn_MoveMeasureStep = 3200;
					
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_MoveMeasureStep = 3100;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{
					CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
					mn_MoveMeasureStep = 3100;
				}
			}
			else
			{
				mn_MoveMeasureStep = 3010;
			}
			break;

		case 3200:
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_INSPECT_GABAGE_POS], st_motor[m_nRobot_X].mn_allow);
			nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_INSPECT_GABAGE_POS], st_motor[m_nRobot_Y].mn_allow);
			if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INSPECT_GABAGE_POS], COMI.mn_runspeed_rate);
				if (nRet_1 == BD_GOOD) //좌측으로 이동
				{
					mn_MoveMeasureStep = 3300;
				}
				else if (nRet_1 == BD_RETRY)
				{
					mn_MoveMeasureStep = 3200;
				}
				else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
				{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
					CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
					mn_MoveMeasureStep = 3200;
				}
			}
			else
			{
				mn_MoveMeasureStep = 3010;
			}
			break;

		case 3300:
			//COMM
			//피치를 벌린다
			mn_MoveMeasureStep = 3400;
			break;

		case 3400:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_MoveMeasureStep = 3500;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_MoveMeasureStep = 3400;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
				mn_MoveMeasureStep = 3400;
			}
			break;

		case 3500://check GABAGE sensor
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_HeatSink_Garbage_Full_Chk_1, IO_ON);
			nRet_2 = g_ioMgr.get_in_bit(st_io.i_HeatSink_Garbage_Full_Chk_2, IO_ON);
			if( nRet_1 == IO_OFF || nRet_2 == IO_OFF )
			{
				strAlarm.Empty();
				if( nRet_1 == IO_OFF ) strAlarm.Format("8%d%04d",IO_OFF,st_io.i_HeatSink_Garbage_Full_Chk_1);
				else				   strAlarm.Format("8%d%04d",IO_OFF,st_io.i_HeatSink_Garbage_Full_Chk_2);
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, strAlarm);
			}
			else
			{
				mn_MoveMeasureStep = 3600;
			}
			break;

		case 3600:
			nFuncRet = RET_ERROR;
			mn_MoveMeasureStep = 0;
			break;

		case 4000:
			nFuncRet = RET_GOOD;
			mn_MoveMeasureStep = 0;
			break;
	}
	return nFuncRet;
}

void CRun_HeatSinkVision_Transfer_Robot::RunMoveHeatSink()
{
	int nRet_1,nRet_2;
	long lMotionDone=0;
	bool bSkip = false;

	Func.ThreadFunctionStepTrace(30, mn_RunHsStep);
	switch(mn_RunHsStep)
	{
		case -1:
			mn_RunHsStep = 0;
			break;

		case 0:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_LOADER_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunHsStep = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunHsStep = 0;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunHsStep = 0;
			}
			break;

		case 100:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_P, st_motor[m_nRobot_P].md_pos[P_HEATSINK_PICKER_PITCH_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunHsStep = 200;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunHsStep = 100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunHsStep = 100;
			}
			break;

		case 200:
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_INIT_POS], st_motor[m_nRobot_X].mn_allow);
			nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_INIT_POS], st_motor[m_nRobot_Y].mn_allow);
			if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				mn_RunHsStep = 300;
			}
			else
			{
				mn_RunHsStep = 1000;
			}
			break;

		case 300:
			nRet_1 = Robot_AutoMove_Safety_Zone( 0, 0 );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunHsStep = 100;
			}
			break;

		case 1000:
			m_nFindLotNo_Flag = -1;
			m_nlast_pick = CTL_NO;
			if( g_lotMgr.GetLotCount() > 0 )
			{
				if( g_lotMgr.GetLotAt(0).GetInputHeatsinkCnt(PRIME) < g_lotMgr.GetLotAt(0).GetTotLotCount() )
				{
					m_nFindLotNo_Flag = 0;
					m_strLotNo[m_nFindLotNo_Flag] = g_lotMgr.GetLotAt(0).GetLotID();
					m_strPartNo[m_nFindLotNo_Flag]  = g_lotMgr.GetLotAt(0).GetPartID();

					if( ( g_lotMgr.GetLotAt(0).GetInputHeatsinkCnt(PRIME)-1) == g_lotMgr.GetLotAt(0).GetTotLotCount())
					{
						if( st_buffer_info[THD_PLACE_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES || st_buffer_info[THD_PICK_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES )
						{
							m_nlast_pick = CTL_YES;
						}
						else
						{
							m_nlast_pick = CTL_READY;
						}
					}
					//load plate에 자재 요청
					mn_RunHsStep = 1100;
				}
				else if( g_lotMgr.GetLotAt(0).GetPassCnt(PRIME) == g_lotMgr.GetLotAt(0).GetTotLotCount() )
				{

				}
				else if( g_lotMgr.GetLotCount() >= 2 )
				{
					if( g_lotMgr.GetLotAt(1).GetPassCnt(PRIME) < g_lotMgr.GetLotAt(1).GetTotLotCount() )
					{
						m_nFindLotNo_Flag = 1;
						m_strLotNo[m_nFindLotNo_Flag] = g_lotMgr.GetLotAt(1).GetLotID();
						m_strPartNo[m_nFindLotNo_Flag] = g_lotMgr.GetLotAt(1).GetPartID();
					}
					else
					{
						return;
					}
					mn_RunHsStep = 1100;
				}
				else
				{
					if( g_lotMgr.GetLotCount() > 0 )
					{

					}

				}
			}
			break;

			//미리 준비하기 위한 기본 준비는 무엇인가?

			//1. HEATSINK PICKUP
			//2. PLACE DISPENSOY slot  -> Rotator
			//3. Place dispensor slot one more.
			//4. Pickup heatsink that is on liquid
			//5. Inspection at UV Lamp vision
			//6. Carrier 대기
		case 1100:
			//nMode == 0 => Tray Pickup위치로 바로 갈수 있는 위치로 이동
			nRet_1 = Robot_AutoMove_Safety_Zone( 0, 0 );
			if( nRet_1 == RET_GOOD )
			{
				if( m_nlast_pick == CTL_YES ) mn_RunHsStep = 2000;
				else						  mn_RunHsStep = 1200;
			}
			break;

		case 1200:
			nRet_1 = Process_Dvc_Pickup( 0, THD_LD_HEATSINK_BUFF );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunHsStep = 2000;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				mn_RunHsStep = 1200;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				mn_RunHsStep = 1200;
			}
			break;

		case 2000:
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_NO )
			{
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_REQ;
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
				mn_RunHsStep = 2100;
			}
			break;

		case 2100:
			//nMode == 1 => Dispensor로 자재를 Place할 수 있는 위치로 이동
			nRet_1 = Robot_AutoMove_Safety_Zone( 1, 0 );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunHsStep = 2200;
			}
			break;

		case 2200:
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_READY && st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE)
			{
				 mn_RunHsStep = 2300;
			}
			break;

		case 2300:
			if( g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_0_Check, IO_ON) == IO_ON && g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_180_Check, IO_OFF) == IO_OFF )
			{
				if( m_nlast_pick == CTL_YES ) 
				{
					st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_RETEST;
					st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
					mn_RunHsStep = 3100;
				}
				else
				{
					nRet_1 = Process_Dvc_Place( 0, THD_PLACE_HEATSINK_DVC, 0 );
					if( nRet_1 == RET_GOOD )
					{
						mn_RunHsStep = 2400;
					}
					else if( nRet_1 == RET_PICKER_NOT_FIND)
					{
						mn_RunHsStep = 2300;
					}
					else if( nRet_1 == RET_TRAY_NOT_FIND)
					{
						mn_RunHsStep = 2300;
					}
				}			
			}
			else
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_HeatSink_Reverse_0_Check);
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
			}
			break;

		case 2400:
			//nMode == 1 => Dispensor로 자재를 Place할 수 있는 위치로 이동
			nRet_1 = Robot_AutoMove_Safety_Zone( 1, 0 );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunHsStep = 2500;
			}
			break;

		case 2500:
			st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_CHANGE;
			st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
			mn_RunHsStep = 2510;
			break;

		case 2510:
			if( m_nlast_pick == CTL_READY )
			{
				if( st_buffer_info[THD_PLACE_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES || st_buffer_info[THD_PICK_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES )
				{
					mn_RunHsStep = 3000;
				}
				else
				{
					mn_RunHsStep = 1000;
				}
			}
			else if( st_buffer_info[THD_PLACE_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES && st_buffer_info[THD_PICK_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES )
			{
				mn_RunHsStep = 3000;
			}
			else
			{
				mn_RunHsStep = 1000;
			}
			break;

		case 3000:
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_SORT && st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE )
			{
				mn_RunHsStep = 3100;
			}
			break;

		case 3100:
			nRet_1 = Process_Dvc_Pickup( 0, THD_PICK_REVERSE_DVC );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunHsStep = 3200;
			}
			else if( nRet_1 == RET_PICKER_NOT_FIND)
			{
				mn_RunHsStep = 3100;
			}
			else if( nRet_1 == RET_TRAY_NOT_FIND)
			{
				mn_RunHsStep = 3100;
			}
			break;

		case 3200:
			nRet_1 = Process_Measure_Vision();
			if( nRet_1 == RET_GOOD )
			{
				g_lotMgr.GetLotAt(m_nFindLotNo_Flag).AddHeatSinkInputCnt(PRIME);//히트싱크 픽업증가
				mn_RunHsStep = 4000;
			}
			else if(nRet_1 == RET_ERROR)//hestsink를 버렸다 다시 집는다.
			{
				mn_RunHsStep = 1000;
			}
			break;

		case 4000://move carrier safety
			nRet_1 = Robot_AutoMove_Safety_Zone( 3, 0 );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunHsStep = 4100;
			}
			break;
			
		case 4100:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][0] == CTL_REQ && 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_HEATSINK_PRBT][1] == WORK_PLACE)
			{
				m_nCarriorPos = 0;
				st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][0] = CTL_READY;
				mn_RunHsStep = 4200;
			}
			else if( m_nCarriorPos> 0 && st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][0] == CTL_CHANGE && 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_HEATSINK_PRBT][1] == WORK_PLACE)
			{
				mn_RunHsStep = 4210;
			}
			break;

		case 4110:
			nRet_1 = Robot_AutoMove_Safety_Zone( 3, 0 );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunHsStep = 4200;
			}
			break;

		case 4200:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][0] == CTL_CHANGE && 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_HEATSINK_PRBT][1] == WORK_PLACE)
			{
				mn_RunHsStep = 4210;
			}
			break;

		case 4210:
			if( ( COMI.Get_MotCurrentPos(m_nRobot_X) <= ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[m_nRobot_X].mn_allow ) ) &&
				( COMI.Get_MotCurrentPos(m_nRobot_Y) > ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] - st_motor[m_nRobot_X].mn_allow ) ) )
			{
				nRet_1 = Process_Dvc_Place(0, THD_PLACE_CARRIER_DVC, m_nCarriorPos);
				if( nRet_1 == RET_GOOD )
				{
					mn_RunHsStep = 4300;
				}
				else if( nRet_1 == RET_PICKER_NOT_FIND)
				{
					mn_RunHsStep = 4200;
				}
				else if( nRet_1 == RET_TRAY_NOT_FIND)
				{
					mn_RunHsStep = 4200;
				}
			}
			else
			{
				mn_RunHsStep = 4000;
			}
			break;

		case 4300:
			m_nCarriorPos++;
			if( m_nCarriorPos >= 3 )
			{
				mn_RunHsStep = 4900;
			}
			else
			{
				mn_RunHsStep = 1000;
			}
			break;

		case 4900:
			nRet_1 = Robot_AutoMove_Safety_Zone( 0, 0 );
			if( nRet_1 == RET_GOOD )
			{
				mn_RunHsStep = 5000;
			}
			break;

		case 5000:
			if( COMI.Get_MotCurrentPos(m_nRobot_Y) < ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] + st_motor[m_nRobot_X].mn_allow ) )
			{
				st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][0] = CTL_NO;
				st_sync.nCarrierRbt_Dvc_Req[THD_HEATSINK_PRBT][1] = CTL_NO;
				
				st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][0] = CTL_REQ;
				st_sync.nCarrierRbt_Dvc_Req[THD_VISION_RBT][1] = WORK_PLACE;
				mn_RunHsStep = 1000;
			}
			else
			{
				mn_RunHsStep = 4900;
			}
			break;

	}
}

void CRun_HeatSinkVision_Transfer_Robot::RunMoveDispensor()
{
	int nRet_1,nRet_2,nRet_3;
	long lMotionDone=0;
	bool bSkip = false;
	double dCurrDisY;
	CString strAlarm;

	double dCurrHeatSinkX, dCurrHeatSinkY;

	dCurrHeatSinkX = COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_X );
	dCurrHeatSinkY = COMI.Get_MotCurrentPos( M_HEATSINK_TRANSFER_Y );


	Func.ThreadFunctionStepTrace(37, mn_RunDispensorStep);
	switch(mn_RunDispensorStep)
	{
		case -1:
			mn_RunDispensorStep = 0;
			break;

		case 0:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunDispensorStep = 100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunDispensorStep = 0;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunDispensorStep = 0;
			}
			break;

		case 10:
			dCurrDisY = COMI.Get_MotCurrentPos(M_PRESS_Y);
			if( dCurrDisY > st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS] + st_motor[m_nRobot_DisY].mn_allow )
			{
				mn_RunDispensorStep = 0;
			}
			else
			{
				mn_RunDispensorStep = 100;
			}
			break;			

		case 100:
			if( g_lotMgr.GetLotCount() > 0 )
			{
				mn_RunDispensorStep = 1000;
			}
			break;

		case 1000://Picker ask disponsor if picker could place device
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_REQ && st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE )
			{				
				if( COMI.Get_MotCurrentPos(M_PRESS_Y) > st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS] + st_motor[m_nRobot_DisY].mn_allow )
				{
					mn_RunDispensorStep = 10;
				}
				else
				{
					//check sensor
					//if( /*st_buffer_info[THD_PLACE_HEATSINK_DVC].st_pcb_info[0].nYesNo == CTL_YES && */ g_ioMgr. .Get_In_Bit(st_io.i_Dispenser_Device_Check);
					nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Device_Check, IO_ON);
					nRet_2 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_0_Check, IO_ON);
					nRet_3 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_180_Check, IO_OFF);
					if( nRet_1 != IO_OFF ||  nRet_2 != IO_ON || nRet_3 != IO_OFF)
					{
						if( nRet_1 != IO_OFF) strAlarm.Format("8%d%04d", IO_OFF, st_io.i_Dispenser_Device_Check);
						else if( nRet_2 != IO_ON ) strAlarm.Format("8%d%04d", IO_ON, st_io.i_HeatSink_Reverse_0_Check);
						else								strAlarm.Format("8%d%04d", IO_OFF, st_io.i_HeatSink_Reverse_180_Check);						
						CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
					}
					else
					{
						st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_READY;
						st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
						mn_RunDispensorStep = 1100;
					}
				}
			}
			break;

		case 1100:
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_CHANGE && st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE )
			{
				mn_RunDispensorStep = 1110;
			}
			else if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_RETEST && st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE )//last pick
			{
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_FREE;
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PICK;
				mn_RunDispensorStep = 4500;
			}
			break;

		case 1110:
			if( ( dCurrHeatSinkX > (st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[M_HEATSINK_TRANSFER_X].mn_allow) ) &&
				( dCurrHeatSinkY < (st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRASNFER_Y_TURN_READY_POS] - st_motor[M_HEATSINK_TRANSFER_Y].mn_allow) ) )
			{
				strAlarm.Format("%02d0008", m_nRobot_DisY );
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
				break;
			}



			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Color_Check, IO_ON);
			if( st_basic.n_mode_7387 == CTL_NO) nRet_1 = IO_ON;
			if( nRet_1 == IO_ON)
			{
				mn_RunDispensorStep = 1200;
			}
			else
			{
				strAlarm.Format("8%d%04d", IO_ON, st_io.i_Dispenser_Color_Check);
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
			}
			break;

		case 1200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_GABAGE_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunDispensorStep = 1300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunDispensorStep = 1200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunDispensorStep = 1200;
			}
			break;

		case 1300:
			Set_Dispenser_Air_Blow(IO_ON);
			m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
			mn_RunDispensorStep = 1310;
			break;

		case 1310:
			m_dwWaitDispenserAirBlow[1] = GetCurrentTime();
			m_dwWaitDispenserAirBlow[2] = m_dwWaitDispenserAirBlow[1] - m_dwWaitDispenserAirBlow[0];
			if( m_dwWaitDispenserAirBlow[2] <= 0 ) m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
			if( m_dwWaitDispenserAirBlow[2] > st_wait.nOnWaitTime[WAIT_DISPENSOR_AIRBLOW] ) 
			{
				Set_Dispenser_Air_Blow(IO_OFF);
				mn_RunDispensorStep = 2000;
			}
			break;

		case 2000:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_HEATSINK_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				Set_Dispenser_Air_Blow(IO_ON);
				m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
				mn_RunDispensorStep = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunDispensorStep = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunDispensorStep = 2000;
			}
			break;

		case 2100:
			m_dwWaitDispenserAirBlow[1] = GetCurrentTime();
			m_dwWaitDispenserAirBlow[2] = m_dwWaitDispenserAirBlow[1] - m_dwWaitDispenserAirBlow[0];
			if( m_dwWaitDispenserAirBlow[2] <= 0 ) m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
			if( m_dwWaitDispenserAirBlow[2] > st_wait.nOnWaitTime[WAIT_DISPENSOR_AIRBLOW] ) 
			{
				Set_Dispenser_Air_Blow(IO_OFF);
				mn_RunDispensorStep = 2200;
			}
			break;

		case 2200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_DISPENSING_END_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				Set_Dispenser_Air_Blow(IO_ON);
				m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
				mn_RunDispensorStep = 2300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunDispensorStep = 2200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunDispensorStep = 2200;
			}
			break;

		case 2300:
			m_dwWaitDispenserAirBlow[1] = GetCurrentTime();
			m_dwWaitDispenserAirBlow[2] = m_dwWaitDispenserAirBlow[1] - m_dwWaitDispenserAirBlow[0];
			if( m_dwWaitDispenserAirBlow[2] <= 0 ) m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
			if( m_dwWaitDispenserAirBlow[2] > st_wait.nOnWaitTime[WAIT_DISPENSOR_AIRBLOW] ) 
			{
				Set_Dispenser_Air_Blow(IO_OFF);
				mn_RunDispensorStep = 3000;
			}
			break;

		case 3000:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_DISPENSING_END_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				Set_Dispenser_Air_Blow(IO_ON);
				m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
				mn_RunDispensorStep = 3100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunDispensorStep = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunDispensorStep = 3000;
			}
			break;

		case 3100:
			m_dwWaitDispenserAirBlow[1] = GetCurrentTime();
			m_dwWaitDispenserAirBlow[2] = m_dwWaitDispenserAirBlow[1] - m_dwWaitDispenserAirBlow[0];
			if( m_dwWaitDispenserAirBlow[2] <= 0 ) m_dwWaitDispenserAirBlow[0] = GetCurrentTime();
			if( m_dwWaitDispenserAirBlow[2] > st_wait.nOnWaitTime[WAIT_DISPENSOR_AIRBLOW] ) 
			{
				Set_Dispenser_Air_Blow(IO_OFF);
				mn_RunDispensorStep = 3200;
			}
			break;

		case 3200:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_LIQUID_CHECK_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunDispensorStep = 3300;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunDispensorStep = 3200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunDispensorStep = 3200;
			}
			break;

		case 3300:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_DisY, st_motor[m_nRobot_DisY].md_pos[P_DISPENSOR_Y_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunDispensorStep = 3400;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunDispensorStep = 3300;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, alarm.mstr_code);
				mn_RunDispensorStep = 3300;
			}
			break;

		case 3400:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Color_Check, IO_ON);
			if( st_basic.n_mode_7387 == CTL_NO) nRet_1 = IO_ON;
			if( nRet_1 == IO_ON)
			{
				mn_RunDispensorStep = 4000;
			}
			else
			{
				strAlarm.Format("8%d%04d", IO_ON, st_io.i_Dispenser_Color_Check);
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
			}
			break;

		case 3500:
			Set_HeatSink_Reverse_UpDown(OFF);
			Set_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
			mn_RunDispensorStep = 3600;
			break;

		case 3600:
			nRet_1 = Chk_HeatSink_Reverse_UpDown(OFF);
			nRet_2 = Chk_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				mn_RunDispensorStep = 4000;
			}
			break;

		case 4000:
			if(	g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Up_Check, IO_OFF) == IO_OFF && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Clamp_On_Check, IO_OFF) == IO_OFF )
			{
				mn_RunDispensorStep = 4100;
			}
			else
			{
				mn_RunDispensorStep = 3500;
			}
			break;

		case 4100:
			Set_HeatSink_Reverse_UpDown(ON);
			Set_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
			mn_RunDispensorStep = 4110;
			break;

		case 4110:
			nRet_1 = Chk_HeatSink_Reverse_UpDown(ON);
			nRet_2 = Chk_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD )
			{
				mn_RunDispensorStep = 4120;
			}
			break;

		case 4120:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Device_Check, IO_ON);
			if( nRet_1 == IO_ON )
			{
				if( ( dCurrHeatSinkX > (st_motor[M_HEATSINK_TRANSFER_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[M_HEATSINK_TRANSFER_X].mn_allow) ) &&
					( dCurrHeatSinkY < (st_motor[M_HEATSINK_TRANSFER_Y].md_pos[P_HEATSINK_TRASNFER_Y_TURN_READY_POS] - st_motor[M_HEATSINK_TRANSFER_Y].mn_allow) ) )
				{
					strAlarm.Format("%02d0008", m_nRobot_DisY );
					CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
					break;
				}

				Set_HeatSink_Reverse_UpDown(ON);
				Set_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
				Set_HeatSink_Reverse_Turn(ON);
				mn_RunDispensorStep = 4130;
			}
			else
			{
				strAlarm.Format("8%d%04d", IO_ON, st_io.i_Dispenser_Device_Check);
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
			}
			break;

		case 4130:
			nRet_1 = Chk_HeatSink_Reverse_UpDown(ON);
			nRet_2 = Chk_HeatSink_Reverse_Clamp_ForwardBackward(OFF);
			nRet_3 = Chk_HeatSink_Reverse_Turn(ON);
			if( nRet_1 == RET_GOOD && nRet_2 == RET_GOOD  && nRet_3 == RET_GOOD )
			{
				mn_RunDispensorStep = 4200;
			}
			break;

		case 4200:
			Set_HeatSink_Reverse_UpDown(OFF);
			mn_RunDispensorStep = 4210;
			break;

		case 4210:
			nRet_1 = Chk_HeatSink_Reverse_UpDown(OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_RunDispensorStep = 4220;
			}
			break;

		case 4220:
			Set_HeatSink_Reverse_Clamp_ForwardBackward(ON);
			mn_RunDispensorStep = 4230;
			break;

		case 4230:
			nRet_1 = Chk_HeatSink_Reverse_Clamp_ForwardBackward(ON);
			if( nRet_1 == RET_GOOD )
			{
				mn_RunDispensorStep = 4240;
			}
			break;

		case 4240:
			Set_HeatSink_Reverse_UpDown(ON);
			mn_RunDispensorStep = 4250;
			break;

		case 4250:
			nRet_1 = Chk_HeatSink_Reverse_UpDown(ON);
			if( nRet_1 == RET_GOOD )
			{
				mn_RunDispensorStep = 4260;
			}
			break;

		case 4260:
			nRet_1 = g_ioMgr.get_in_bit(st_io.i_Dispenser_Device_Check, IO_OFF);
			if( nRet_1 != IO_ON )
			{
				m_npSet_WorkPosYXCPB[0] = 0;
				m_npSet_WorkPosYXCPB[1] = 0;
				m_npSet_WorkPosYXCPB[3] = 0;

				Func.Data_Exchange_PickPlace(PICKER_PICK_MODE, 1, THD_PICK_REVERSE_DVC, THD_PLACE_HEATSINK_DVC, m_npSet_WorkPosYXCPB);
				Set_HeatSink_Reverse_Turn(OFF);
				mn_RunDispensorStep = 4300;
			}
			else
			{
				strAlarm.Format("8%d%04d", IO_OFF, st_io.i_Dispenser_Device_Check);
				CTL_Lib.Alarm_Error_Occurrence(1102, dWARNING, strAlarm);
			}
			break;

		case 4300:
			nRet_1 = Chk_HeatSink_Reverse_Turn(OFF);
			if( nRet_1 ==  RET_GOOD )
			{
				mn_RunDispensorStep = 4310;
			}
			break;

			// down to help heatsink robot picks up DVC
		case 4310:
			Set_HeatSink_Reverse_UpDown(OFF);
			mn_RunDispensorStep = 4320;
			break;

		case 4320:
			nRet_1 = Chk_HeatSink_Reverse_UpDown(OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_RunDispensorStep = 4400;
			}
			break;

		case 4400:			
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_CHANGE && 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE )
			{
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_SORT;
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
				mn_RunDispensorStep = 4500;
			}
			break;

		case 4500://HEATSINK ROBOT pick dvc in reverse site
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_FREE && 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE )
			{
// 				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_NO;
// 				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = CTL_NO;
				mn_RunDispensorStep = 4600;
			}
			break;

		case 4600:
			Set_HeatSink_Reverse_Clamp_ForwardBackward(IO_OFF);
			mn_RunDispensorStep = 4610;
			break;
			
		case 4610:
			nRet_1 = Chk_HeatSink_Reverse_Clamp_ForwardBackward(IO_OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_RunDispensorStep = 4700;
			}
			break;

		case 4700:
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_FREE && 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == WORK_PLACE )
			{
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] = CTL_FINAL;
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] = WORK_PLACE;
				mn_RunDispensorStep = 4800;
			}
			break;

		case 4800:
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][0] == CTL_NO && 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_RBT][1] == CTL_NO )
			{
				mn_RunDispensorStep = 5000;
			}
			break;


		case 5000:
			mn_RunDispensorStep = 10;
			break;
	}
}

//Heatsink pickup
int CRun_HeatSinkVision_Transfer_Robot::Process_Dvc_Place(int nMode, int nWork_Site, int nPos)
{
	int nFuncRet = RET_PROCEED;
	int i, nRet_1, nRet_2, nRet_3, nFlag=0;
	double dCurrentPosX =0.0, dCurrentPosY=0.0, dCurrInspectY = 0.0;

	dCurrentPosX = COMI.Get_MotCurrentPos( m_nRobot_X );
	dCurrentPosY = COMI.Get_MotCurrentPos( m_nRobot_Y );

	int m_nSitePicker=-1;
	if	   ( nWork_Site == THD_PLACE_HEATSINK_DVC ) m_nSitePicker = THD_HEATSINK_RBT;
	else if( nWork_Site == THD_PLACE_CARRIER_DVC )  m_nSitePicker = THD_HEATSINK_PRBT;
	else
	{
		return nFuncRet;
	}

	Func.ThreadFunctionStepTrace(32, mn_PlaceStep);
	switch(mn_PlaceStep)
	{
	case 0:
		m_bDvcWaitChk_Falg =  false;
		m_nPickRetry_Cnt = 0;
		mn_PlaceStep = 100;
		break;

	case 10:
		Set_HeatSink_Transfer_Pad_UpDown(IO_OFF);
		mn_PlaceStep = 20;
		break;

	case 20:
		nRet_1 = Chk_HeatSink_Transfer_Pad_UpDown(  IO_OFF);
		if( nRet_1 == RET_GOOD )
		{
			mn_PlaceStep = 100;
		}
		break;

	case 100:
		nRet_1 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Transfer_Pad_Up_Check,	 IO_ON);
		nRet_2 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Transfer_Pad_Down_Check,	 IO_OFF); 
		if( nRet_1 != IO_ON || nRet_2 != IO_OFF ) 
		{
			mn_PlaceStep = 10;
		}
		else
		{
			mn_PlaceStep = 200;
		}
		break;

	case 200:
		m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
		if(m_dCurrentPos[m_nRobot_Z] > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //안전 위치를 벗어나 있으면 저위치 시킨다
		{
			mn_PlaceStep = 900;
		}
		else
		{
			mn_PlaceStep = 1000;
		}
		break;

	case 900:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_bDvcWaitChk_Falg =  false;
			mn_PlaceStep = 1000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 900;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1230, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 900;
		}
		break;

	case 1000:
		m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z); 
		if(m_dCurrentPos[m_nRobot_Z] > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] + COMI.md_allow_value[m_nRobot_Z])
		{
			mn_PlaceStep = 900;
			break;
		}

		nRet_1 = Func.Check_PickerStatus(0, m_nSitePicker, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);

		if(nRet_1 == RET_GOOD)
		{
			m_bDvcWaitChk_Falg = false;
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
				CTL_Lib.Alarm_Error_Occurrence(1231, dWARNING, Func.m_strAlarmCode);
				break;
			}
		}

		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
		if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //안전 위치를 벅어나 있으면 저위치 시킨다
		{
			mn_PlaceStep = 900;
			break;
		}		 

		//작업 가능 위치를 찾는다
		m_n_TrayNum = -1; m_n_FirstTray_Y_Num = -1; m_n_FirstTray_Y_Num = -1; m_n_FirstPicker_Num = -1; //flag clear
		for(i = 0; i < m_nPickerPara; i++)
		{
			if(st_picker[m_nSitePicker].st_pcb_info[i].nYesNo == CTL_YES)//피커에 자재가 들고 있다는 정보가 유
			{
				m_n_FirstPicker_Num = i;
				i = m_nPickerPara;
			}			
		}

		if(m_n_FirstPicker_Num == -1)
		{
			//피커에 더이상 작업할 것이 없다
			mn_PlaceStep = 0;
			nFuncRet = RET_PICKER_NOT_FIND;
			break;
		}

		nRet_1 = Func.Find_TrayWork_Pos(WORK_PLACE, nWork_Site, CTL_NO, m_npFindWorkPosYXCPB, m_nSitePicker, _T("0")); //놓을때버퍼에서으 ㅣ작업순서는 뒤부터 놓는다 (4 -> 1 순서) 
		if(nRet_1 == RET_GOOD) //작업 가능한 자재가 있다
		{
			 m_n_FirstTray_X_Num = m_npFindWorkPosYXCPB[0]; m_n_FirstTray_Y_Num = m_npFindWorkPosYXCPB[1];
		}
		else if(nRet_1 == RET_SKIP) //더이상 자재가 없다
		{	//계속 진행할 수 없다
			mn_PlaceStep = 0;
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

				if(st_picker[m_nSitePicker].st_pcb_info[i].nYesNo == CTL_YES)// && st_picker[m_nRobotSite].st_pcb_info[i].nEnable == CTL_YES)
				{
					if(i >= 0 && m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num) < st_recipe.nLdBuffer_Num)
					{
						if(st_buffer_info[nWork_Site].st_pcb_info[m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)].nYesNo == CTL_NO)
						{
							m_npPicker_YesNo[i] = CTL_YES;
							nFlag++;
						}
					}
				}				
			}
			if(nFlag == 0) //작업할 피커가 하나도 준비되 않았다.
			{
				mn_PlaceStep = 0;
				nFuncRet = RET_PICKER_NOT_FIND;//피커에 더이상 작업할 것이 없다
				break;
			}

			//실제 작업 위치를 셋팅
			m_npFindWorkPosYXCPB[0] = m_n_FirstTray_Y_Num; m_npFindWorkPosYXCPB[1] = m_n_FirstTray_X_Num; m_npFindWorkPosYXCPB[3] = m_n_FirstPicker_Num;
			m_nMove_Flag[0] = CTL_NO; m_nMove_Flag[1] = CTL_NO;
			mn_PlaceStep = 2000;
		}
		break; 

	////////////////////////////////////////////////////////
	// 작업 할 위치로 X,Y robot 및 rotator 실린더 동작한다
	////////////////////////////////////////////////////////
	case 2000:
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
		if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //안전 위치를 벗어나 있으면 정위치 시킨다
		{
			mn_PlaceStep = 900;
			break;
		}

		dCurrInspectY = COMI.Get_MotCurrentPos(M_HEATSINK_INSPECT_Y);
		if( dCurrInspectY > ( st_motor[M_HEATSINK_INSPECT_Y].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS] + st_motor[M_HEATSINK_INSPECT_Y].mn_allow ) )
		{
			m_strAlarmCode.Format( _T("%02d0008"), M_HEATSINK_INSPECT_Y ); 
			CTL_Lib.Alarm_Error_Occurrence(1240, dWARNING, m_strAlarmCode);
			break;
		}



		nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_X, nWork_Site, nPos, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_X);             //Calculate_MovePos_Find(int nMotNum, int nSite, int nWorkPart, int *npFirstYXP_Pos, double *dpGetTargetPos)
		nRet_2 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWork_Site, nPos, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);

		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{//ok
			m_dpTargetPosList[0] = m_dTemp_CalTargetPos_X;
			m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1233, dWARNING, Func.m_strAlarmCode);
			break;
		}

		m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
		m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
		m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속 


		nRet_1 = CTL_Lib.Linear_Move(m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio); 
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_bDvcWaitChk_Falg =  false;
			//Z축을 내리기전에 Pitch를 오므린다.
			mn_PlaceStep = 3000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 2000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1234, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 2000;
		}
		break; 

	case 2900:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_bDvcWaitChk_Falg =  false;
			mn_PlaceStep = 3000;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 2900;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 2900;
		}
		break;

// 	case 2900:
// 		Set_HeatSink_Transfer_Pad_UpDown(IO_OFF);
// 		mn_PickStep = 2910;
// 		break;
// 
// 	case 2910:
// 		nRet_1 = Chk_HeatSink_Transfer_Pad_UpDown(IO_OFF);
// 		if( nRet_1 == RET_GOOD )
// 		{
// 			mn_PickStep = 3000;
// 		}
// 		break;
// 
// 	case 2950:
// 		if( nWork_Site == THD_PLACE_HEATSINK_DVC )//picker
// 		{//serial comm
// 			mn_PickStep = 2960;
// 		}
// 		else//THD_PLACE_CARRIER_DVC//glipper
// 		{
// 			mn_PickStep = 3000;
// 		}
// 		break;
// 
// 	case 2960:
// 		mn_PickStep = 3000;
// 		break;
// 
// 	case 3000:

	////////////////////////////////////////////////////////
	// 작업 할 자재를 놓는다 
	////////////////////////////////////////////////////////
	case 3000: 		
		m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);
		if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow)
		{
			mn_PlaceStep = 2900;
			break;
		}
		nRet_1 = Func.Check_PickerStatus(0, m_nSitePicker, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);

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
				CTL_Lib.Alarm_Error_Occurrence(1237, dWARNING, Func.m_strAlarmCode);
				break;
			}
		}

		nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, m_dpTargetPosList[0], COMI.md_allow_value[m_nRobot_X]);
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], COMI.md_allow_value[m_nRobot_Y]);
		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
		{
			mn_PlaceStep = 2000;
			break;
		}

		if(nWork_Site == THD_PLACE_HEATSINK_DVC)
		{
			nRet_1 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_0_Check, IO_ON);
			nRet_2 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_180_Check, IO_OFF);
			nRet_3 = FAS_IO.Chk_IO_OnOff(st_io.i_HeatSink_Reverse_0_Check, IO_ON, IO_STABLE_WAIT, IO_STABLE_LIMIT);
			if(nRet_3 == RET_PROCEED)
			{
				break; 
			} 
			if(nRet_1 == IO_ON && nRet_2 == IO_OFF && st_basic.n_mode_device != WITHOUT_DVC)
			{
				break; 
			} 
			else if(nRet_1 == RET_ERROR && st_basic.n_mode_device != WITHOUT_DVC)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_HeatSink_Reverse_0_Check);
				CTL_Lib.Alarm_Error_Occurrence(1239, dWARNING, m_strAlarmCode);
				break;
			}  

			m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_TURN_PLACE_POS];
		} 
		else if(nWork_Site == THD_PLACE_CARRIER_DVC)
		{
			dCurrInspectY = COMI.Get_MotCurrentPos(M_HEATSINK_INSPECT_Y);
			if( dCurrInspectY > ( st_motor[M_HEATSINK_INSPECT_Y].md_pos[P_HEATSINK_INSPECT_Y_INIT_POS] + st_motor[M_HEATSINK_INSPECT_Y].mn_allow ) )
			{
				m_strAlarmCode.Format( _T("%02d0008"), M_HEATSINK_INSPECT_Y ); 
				CTL_Lib.Alarm_Error_Occurrence(1240, dWARNING, m_strAlarmCode);
				break;
			}
			if		( nPos == TOP)		m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_CARRIER_TOP_DOWN_POS];
			else if( nPos == MIDDLE)	m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_CARRIER_MID_DOWN_POS];
			else/* if( nPos == BTM)*/	m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_CARRIER_BOT_DOWN_POS];

			if(st_basic.n_mode_7387 != CTL_YES)//not use
			{
				m_dpTargetPosList[2] = m_dpTargetPosList[2];// / 2;
			}
		}
		else 
		{//what happen error
			m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS];
			break;
		}

		
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			if( nWork_Site == THD_PLACE_HEATSINK_DVC ) Set_HeatSink_Transfer_Pad_UpDown(IO_ON);			
			mn_PlaceStep = 3100;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1245, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3000;
		}		
		break;

	case 3100:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{ 
			if( nWork_Site == THD_PLACE_HEATSINK_DVC )
			{
				mn_PlaceStep = 3110;
			}
			else //carrier
			{
				mn_PlaceStep = 3200;
			}
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3000;
		}		
		break;

	case 3110:
		if( nWork_Site == THD_PLACE_HEATSINK_DVC ) nRet_1 = Chk_HeatSink_Transfer_Pad_UpDown(IO_ON);
		if(nRet_1 == RET_GOOD)
		{
			Set_HeatSink_Vacuum_OnOff(IO_OFF);
			mn_PlaceStep = 3120;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1247, dWARNING, m_strAlarmCode);
		}
		break;

	case 3120:
		nRet_1 = Chk_HeatSink_Vacuum_OnOff( IO_OFF );
		if( nRet_1 == RET_GOOD )
		{
			mn_PlaceStep = 3200;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1247, dWARNING, m_strAlarmCode);
		}
		break;

	case 3200:
		if(nWork_Site == THD_PLACE_HEATSINK_DVC)
		{
			if( st_basic.n_mode_device == WITHOUT_DVC)
				nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.o_HeatSink_Vacuum_On_Sol, IO_OFF, IO_STABLE_WAIT, 1000);
			else
				nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.i_Dispenser_Device_Check, IO_OFF, IO_STABLE_WAIT, 1000);
			if( nRet_1 == RET_PROCEED)
				break;

			mn_PlaceStep = 4000;
		}
		else// if( nWork_Site == THD_PLACE_CARRIER_DVC )
		{
			m_nRubbRetry_Cnt = 0;
			mn_PlaceStep = 3300;
		} 
		break;

	case 3202:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{ 
			mn_PlaceStep = 3300;
			
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3202;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3202;
		}		
		break;
		
	case 3204:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{ 
			mn_PlaceStep = 3205;			
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3204;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3204;
		}		
		break;

	case 3205:
		if( COMI.Get_MotCurrentPos(m_nRobot_Z) > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] +  st_motor[m_nRobot_X].mn_allow)
		{
			mn_PlaceStep = 3204;
			break;
		}
		nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_X, nWork_Site, nPos, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_X);             //Calculate_MovePos_Find(int nMotNum, int nSite, int nWorkPart, int *npFirstYXP_Pos, double *dpGetTargetPos)
		nRet_2 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWork_Site, nPos, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);
		
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{//ok
			m_dpTargetPosList[0] = m_dTemp_CalTargetPos_X;
			m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1233, dWARNING, Func.m_strAlarmCode);
			break;
		}
		
		m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
		m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
		m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속 
		
		
		nRet_1 = CTL_Lib.Linear_Move(m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio); 
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_PlaceStep = 3202;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3205;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1234, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3205;
		}
		break; 

	case 3300://rubs on each heatsink
		//carrier 사이트 위치별로 받아온다
		nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, m_dpTargetPosList[0], COMI.md_allow_value[m_nRobot_X] + st_basic.dHSCarrierSpreadMoveOffset);
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], COMI.md_allow_value[m_nRobot_Y]);
		nRet_3 = COMI.Check_MotPosRange(m_nRobot_Z, m_dpTargetPosList[2], COMI.md_allow_value[m_nRobot_Z]);
		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD || nRet_3 != BD_GOOD)
		{
			if( nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				mn_PlaceStep = 3204;

			}
			else//if( nRet_3 != BD_GOOD )
			{
				mn_PlaceStep = 3202;
			}
			break;
		}
		if( st_basic.n_rubb_count <= m_nRubbRetry_Cnt )
		{
			m_nRubbRetry_Cnt = 0;
			mn_PlaceStep = 3500;
			break;
		}
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, m_dpTargetPosList[0]+ st_basic.dHSCarrierSpreadMoveOffset , st_recipe.nRubHSRunSpeed);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			m_nRubbRetry_Cnt++;
			mn_PlaceStep = 3400;		
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3300;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3300;
		}		
		break;


	case 3302:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{ 
			mn_PlaceStep = 3400;
			
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3302;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3302;
		}		
		break;
		
	case 3304:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{ 
			mn_PlaceStep = 3305;			
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3304;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3304;
		}		
		break;
		
	case 3305:
		if( COMI.Get_MotCurrentPos(m_nRobot_Z) > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] +  st_motor[m_nRobot_X].mn_allow)
		{
			mn_PlaceStep = 3304;
			break;
		}
		nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_X, nWork_Site, nPos, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_X);             //Calculate_MovePos_Find(int nMotNum, int nSite, int nWorkPart, int *npFirstYXP_Pos, double *dpGetTargetPos)
		nRet_2 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWork_Site, nPos, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);
		
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{//ok
			m_dpTargetPosList[0] = m_dTemp_CalTargetPos_X;
			m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1233, dWARNING, Func.m_strAlarmCode);
			break;
		}
		
		m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
		m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
		m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속 
		
		
		nRet_1 = CTL_Lib.Linear_Move(m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio); 
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_PlaceStep = 3302;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3305;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1234, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3305;
		}
		break; 


	case 3400:		
		nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, m_dpTargetPosList[0], COMI.md_allow_value[m_nRobot_X] + st_basic.dHSCarrierSpreadMoveOffset);
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], COMI.md_allow_value[m_nRobot_Y]);
		nRet_3 = COMI.Check_MotPosRange(m_nRobot_Z, m_dpTargetPosList[2], COMI.md_allow_value[m_nRobot_Z]);
		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD || nRet_3 != BD_GOOD)
		{
			if( nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				mn_PlaceStep = 3304;
				
			}
			else//if( nRet_3 != BD_GOOD )
			{
				mn_PlaceStep = 3302;
			}
			break;
		}
		//약간 뒤로 빠진뒤에 올린다.
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, m_dpTargetPosList[0] - st_basic.dHSCarrierSpreadMoveOffset, st_recipe.nRubHSRunSpeed);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_PlaceStep = 3300;		
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3400;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3400;
		}		
		break;

	case 3402:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{ 
			mn_PlaceStep = 3500;
			
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3402;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3402;
		}		
		break;
		
	case 3404:
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{ 
			mn_PlaceStep = 3405;			
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3404;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3404;
		}		
		break;
		
	case 3405:
		if( COMI.Get_MotCurrentPos(m_nRobot_Z) > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] +  st_motor[m_nRobot_X].mn_allow)
		{
			mn_PlaceStep = 3404;
			break;
		}
		nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_X, nWork_Site, nPos, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_X);             //Calculate_MovePos_Find(int nMotNum, int nSite, int nWorkPart, int *npFirstYXP_Pos, double *dpGetTargetPos)
		nRet_2 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWork_Site, nPos, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);
		
		if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
		{//ok
			m_dpTargetPosList[0] = m_dTemp_CalTargetPos_X;
			m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
		}
		else
		{
			CTL_Lib.Alarm_Error_Occurrence(1233, dWARNING, Func.m_strAlarmCode);
			break;
		}
		
		m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
		m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
		m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속 
		
		
		nRet_1 = CTL_Lib.Linear_Move(m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio); 
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_PlaceStep = 3402;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3405;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1234, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3405;
		}
		break; 


	case 3500:
		nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, m_dpTargetPosList[0], st_motor[m_nRobot_X].mn_allow + st_basic.dHSCarrierSpreadMoveOffset);
		nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], st_motor[m_nRobot_Y].mn_allow);
		nRet_3 = COMI.Check_MotPosRange(m_nRobot_Z, m_dpTargetPosList[2], st_motor[m_nRobot_Z].mn_allow);
		if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD || nRet_3 != BD_GOOD)
		{
			if( nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				mn_PlaceStep = 3404;
				
			}
			else//if( nRet_3 != BD_GOOD )
			{
				mn_PlaceStep = 3402;
			}
			break;
		}
		nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, m_dpTargetPosList[0] , st_recipe.nRubHSRunSpeed);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_PlaceStep = 3600;		
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3500;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3500;
		}		
		break;

	case 3600:
		nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nRobot_Z,  st_motor[m_nRobot_Z].md_pos[P_UNLOADER_TRANSFER_X_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_PlaceStep = 4000;		
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 3600;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1246, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 3600;
		}
		break;

	////////////////////////////////////////////////////////////
	//로보트를 안전 위치로 올린다 
	////////////////////////////////////////////////////////////
	case 4000: 		
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			Set_HeatSink_Transfer_Pad_UpDown(IO_OFF);
			m_bDvcWaitChk_Falg =  false;
			mn_PlaceStep = 4010;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 4000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1251, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 4000;	
		}
		break;

	case 4010:
		nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
		if (nRet_1 == BD_GOOD) //좌측으로 이동
		{
			mn_PlaceStep = 4020;
		}
		else if (nRet_1 == BD_RETRY)
		{
			mn_PlaceStep = 4000;
		}
		else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
		{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
			CTL_Lib.Alarm_Error_Occurrence(1253, dWARNING, alarm.mstr_code);
			mn_PlaceStep = 4000;
		}
		break;
		
	case 4020:
		nRet_1 = Chk_HeatSink_Transfer_Pad_UpDown(IO_OFF);  
		if(nRet_1 == RET_GOOD)
		{
			m_bDvcWaitChk_Falg = false;
			mn_PlaceStep = 4100;
		}
		else if(nRet_1 == RET_ERROR)
		{
			CTL_Lib.Alarm_Error_Occurrence(1252, dWARNING, m_strAlarmCode);
			mn_PlaceStep = 4000;
		} 
		break;


	case 4100:
		mn_PlaceStep = 5000; 
		break;

	////////////////////////////////////////////////////////
	// 작업을 완료한 상태로 피커 상태를 확인한다
	// 피커 데이터를 버퍼/트레이에 data exchange 한다
	////////////////////////////////////////////////////////
	case 5000: //picker 정보와 상태를 확인한다
		nRet_1 = Func.Check_PickerStatus(1, nWork_Site, CTL_NO, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
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
				if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;

				CTL_Lib.Alarm_Error_Occurrence(1255, dWARNING, Func.m_strAlarmCode);//2017.0313
				break;
			}
		}

		for (i = m_n_FirstPicker_Num; i < m_nPickerPara; i++)
		{
			if(st_picker[m_nSitePicker].st_pcb_info[i].nYesNo == CTL_YES && m_npPicker_YesNo[i] == CTL_YES )
			{
				if( nWork_Site == THD_PLACE_HEATSINK_DVC )
				{
					if( m_npPicker_Vacuum_Status[i] == IO_OFF /*B접점*/ && m_npPicker_OutputStatus[i] == IO_OFF )
					{
						if (m_strLotNo[1] == _T(""))
						{
							m_strLotNo[1]  = st_picker[m_nSitePicker].st_pcb_info[i].strLotNo;
							m_strPartNo[1] = st_picker[m_nSitePicker].st_pcb_info[i].strPartNo;
						}
						
						if(i >= 0 && m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num) < st_recipe.nLdBuffer_Num)
						{
							m_npPicker_YesNo[i] = CTL_NO;	
							m_npSet_WorkPosYXCPB[0] = m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num);
							m_npSet_WorkPosYXCPB[1] = m_npFindWorkPosYXCPB[1];
							m_npSet_WorkPosYXCPB[3]  = i; //picker 위치 정보
							
							Func.Data_Exchange_PickPlace(PICKER_PLACE_MODE, 1, m_nSitePicker, nWork_Site, m_npSet_WorkPosYXCPB);
						}
					}
				}
				else if( nWork_Site == THD_PLACE_CARRIER_DVC )
				{
					if( m_npPicker_Vacuum_Status[i] == IO_OFF && m_npPicker_OutputStatus[i] == IO_OFF )
					{
						if (m_strLotNo[1] == _T(""))
						{
							m_strLotNo[1]  = st_picker[m_nSitePicker].st_pcb_info[i].strLotNo;
							m_strPartNo[1] = st_picker[m_nSitePicker].st_pcb_info[i].strPartNo;
						}
						
						if(i >= 0 && m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num) < st_recipe.nLdBuffer_Num)
						{
							m_npPicker_YesNo[i] = CTL_NO;	
							m_npSet_WorkPosYXCPB[0] = m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num);
							m_npSet_WorkPosYXCPB[1] = m_npFindWorkPosYXCPB[1];
							m_npSet_WorkPosYXCPB[3]  = i; //picker 위치 정보
							
							Func.Data_Exchange_PickPlace(PICKER_PLACE_MODE, 1, m_nSitePicker, nWork_Site, m_npSet_WorkPosYXCPB);
						}
					}

				}
			}
		}

		if (nRet_1 == RET_GOOD) // 1000 step에서 셋팅한 피커 정보에 맞게 모두 정상적 집었을 경우 이
		{
			//화면 정보 갱신
			m_bDvcWaitChk_Falg = false;

			// jtkim 20150303 conveyor/loader tray 화면 표시
			if (st_handler.cwnd_list != NULL)
			{
// 				st_handler_info.cWndMain->PostMessage(WM_WORK_DISPLAY, MAIN_LD_PICKER_DISPLAY, 0);
// 				st_handler_info.cWndMain->PostMessage(WM_WORK_DISPLAY, MAIN_LD_BUF_PICK_DISPLAY, 0);
// 				st_handler_info.cWndMain->PostMessage(WM_WORK_DISPLAY, MAIN_COUNT_DISPLAY, 0);
			}
			mn_PlaceStep = 6000;
		}
		else if (nRet_1 == RET_ERROR)
		{
			 CTL_Lib.Alarm_Error_Occurrence(1255, dWARNING, Func.m_strAlarmCode);
		}
		break;

	case 6000:
		nRet_1 = Func.Check_PickerStatus(0, nWork_Site, CTL_YES, m_npPicker_YesNo, m_npPicker_Data_Status, m_npPicker_OutputStatus);
		
		if(nRet_1 == RET_GOOD)
		{
			nFuncRet = RET_GOOD;
			mn_PlaceStep = 0;
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
			CTL_Lib.Alarm_Error_Occurrence(1256, dWARNING, Func.m_strAlarmCode);
		}
		break; 

	default :
		break;
	}

	return nFuncRet;
}

//Heatsink pickup
int CRun_HeatSinkVision_Transfer_Robot::Process_Dvc_Pickup(int nMode, int nWork_Site)
{
	int nFuncRet = RET_PROCEED;
	int i, nRet_1, nRet_2, nRet_3, nFlag=0;
	double dCurrentPosX =0, dCurrentPosY=0;
	CString strAlarm;

	//THD_HEATSINK_TRAY , THD_PICK_HEATSINK

	dCurrentPosX = COMI.Get_MotCurrentPos( m_nRobot_X );
	dCurrentPosY = COMI.Get_MotCurrentPos( m_nRobot_Y );

	int m_nSitePicker=-1;
	if	   ( nWork_Site == THD_LD_HEATSINK_BUFF ) m_nSitePicker = THD_HEATSINK_RBT;
	else if( nWork_Site == THD_DISPENSOR_PRBT )	  m_nSitePicker = THD_HEATSINK_PRBT;
	else
	{
		return nFuncRet;
	}

	Func.ThreadFunctionStepTrace(33, mn_PickStep);
	switch(mn_PickStep)
	{
		case 0:
			m_bDvcWaitChk_Falg =  false;
			m_nPickRetry_Cnt = 0;
			mn_PickStep = 100;
			break;

		case 10:
			Set_HeatSink_Transfer_Pad_UpDown(IO_OFF);
			mn_PickStep = 20;
			break;

		case 20:
			nRet_1 = Chk_HeatSink_Transfer_Pad_UpDown(  IO_OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_PickStep = 100;
			}
			break;

		case 100:
			nRet_1 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Transfer_Pad_Up_Check,	 IO_ON);
			nRet_2 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Transfer_Pad_Down_Check,	 IO_OFF); 
			if( nRet_1 != IO_ON || nRet_2 != IO_OFF ) 
			{
				mn_PickStep = 10;
			}
			else
			{
				mn_PickStep = 200;
			}
			break;

		case 200:
			m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[m_nRobot_Z] > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //안전 위치를 벗어나 있으면 저위치 시킨다
			{
				mn_PickStep = 900;
			}
			else
			{
				mn_PickStep = 1000;
			}
			break;

		case 900:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_bDvcWaitChk_Falg =  false;
				mn_PickStep = 1000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_PickStep = 900;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1108, dWARNING, alarm.mstr_code);
				mn_PickStep = 900;
			}
			break;

		case 1000:
			m_dCurrentPos[m_nRobot_Z] = COMI.Get_MotCurrentPos(m_nRobot_Z);
			if(m_dCurrentPos[m_nRobot_Z] > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow)
			{
				mn_PickStep = 900;
				break;
			}
			//THD_LD_HEATSINK_BUFF
			nRet_1 = Func.Check_PickerStatus(0, m_nSitePicker, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
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
					CTL_Lib.Alarm_Error_Occurrence(1200, dWARNING, Func.m_strAlarmCode);
					m_bDvcWaitChk_Falg = false;
					break;
				}
			}

			//작업 가능 위치를 찾는다
			nFlag = 0;
			m_n_TrayNum = -1; m_n_FirstTray_X_Num = -1; m_n_FirstTray_Y_Num = -1; m_n_FirstPicker_Num = -1; //flag clear
			for(i = 0; i < m_nPickerPara; i++)
			{
				if(st_picker[m_nSitePicker].st_pcb_info[i].nYesNo == CTL_NO)//피커에 자재가 들고 있다는 정보가 무
				{
					m_n_FirstPicker_Num = i;
					i = m_nPickerPara;
				}
			}

			if(m_n_FirstPicker_Num == -1)
			{
				//피커에 더이상 작업할 것이 없다
				mn_PickStep = 0;
				nFuncRet = RET_PICKER_NOT_FIND;
				break;
			}

			nRet_1 = Func.Find_TrayWork_Pos(WORK_PICK, nWork_Site, CTL_YES, m_npFindWorkPosYXCPB, m_nSitePicker, _T("0"));
			if(nRet_1 == RET_GOOD) //작업 가능한 자재가 있다
			{
				m_n_FirstTray_X_Num = m_npFindWorkPosYXCPB[0]; m_n_FirstTray_Y_Num = m_npFindWorkPosYXCPB[1];
				m_n_TrayNum =  m_npFindWorkPosYXCPB[4];
			}
			else if(nRet_1 == RET_SKIP) //더이상 자재가 없다
			{	//계속 진행할 수 없다
				mn_PickStep = 0;
				nFuncRet = RET_TRAY_NOT_FIND;
				break;
			}

			nFlag = 0;
			if(m_n_TrayNum != -1 && m_n_FirstPicker_Num != -1 && m_n_FirstTray_Y_Num != -1 && m_n_FirstTray_X_Num != -1) //작업할 공간이 다 있을때 다음 작업을 진행한다
			{
				for(i = 0; i < m_nPickerPara; i++)  //m_nPickerPara == 1
				{
					m_npPicker_YesNo[i] = CTL_NO; //작업할 피커정보 clear
					if(i < m_n_FirstPicker_Num) continue;

					if(st_picker[m_nSitePicker].st_pcb_info[i].nYesNo == CTL_NO)
					{
						//st_recipe.nHsTrayY는 m_nSitePicker에 따라 달라진다. check!!!
						if(i >= 0 && m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num) < st_recipe.nHsTrayY)
						{
							if( nWork_Site == THD_LD_HEATSINK_BUFF )
							{
								if(st_tray_info[nWork_Site].st_dvc_info[m_n_TrayNum][ m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)][m_n_FirstTray_X_Num].nYesNo == CTL_YES) //트레이에 자재가  있으면 집기 가능하게 셋
								{
									m_npPicker_YesNo[i] = CTL_YES;
									nFlag++;
									st_picker[m_nSitePicker].st_pcb_info[i].nStatusChang = NO;
								}
							}
							else if( nWork_Site == THD_DISPENSOR_PRBT )
							{
								if(st_buffer_info[nWork_Site].st_pcb_info[m_n_FirstTray_Y_Num + (i - m_n_FirstPicker_Num)].nYesNo == CTL_YES) //트레이에 자재가  있으면 집기 가능하게 셋
								{
									m_npPicker_YesNo[i] = CTL_YES;
									nFlag++;
									st_picker[m_nSitePicker].st_pcb_info[i].nStatusChang = NO;
								}
							}
						}
					}					
				}

				if(nFlag == 0) //작업할 피커가 하나도 준비되 않았다.
				{ //이부분이 오면 안됨 
					mn_PickStep = 0;
					nFuncRet = RET_PICKER_NOT_FIND;//피커에 더이상 작업할 것이 없다
					break;
				}

				m_npFindWorkPosYXCPB[0] = m_n_FirstTray_X_Num; m_npFindWorkPosYXCPB[1] = m_n_FirstTray_Y_Num; m_npFindWorkPosYXCPB[3] = m_n_FirstPicker_Num;
				m_nMove_Flag[0] = CTL_NO; m_nMove_Flag[1] = CTL_NO;
				mn_PickStep = 1500;
			}
			break;		

		case 1500:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_PickStep = 2000;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_PickStep = 1500;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1201, dWARNING, alarm.mstr_code);
				mn_PickStep = 1500;
			}
			break;

		case 2000:
			m_dCurrentPos[2] = COMI.Get_MotCurrentPos(m_nRobot_Z);  // 모터 특정 축의 현재 위치 리턴 함수
			if(m_dCurrentPos[2] > st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS] + st_motor[m_nRobot_Z].mn_allow) //안전 위치를 벅어나 있으면 저위치 시킨다
			{
				mn_PickStep = 900;
				break;
			}
			
			nRet_1 = Func.Calculate_MovePos_Find(0, m_nRobot_X, nWork_Site, 0, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_X);
			nRet_2 = Func.Calculate_MovePos_Find(0, m_nRobot_Y, nWork_Site, 0, m_npFindWorkPosYXCPB, &m_dTemp_CalTargetPos_Y);             

			if(nRet_1 == RET_GOOD && nRet_2 == RET_GOOD)
			{//ok
				m_dpTargetPosList[0] = m_dTemp_CalTargetPos_X;
				m_dpTargetPosList[1] = m_dTemp_CalTargetPos_Y;
			}
			else
			{
				CTL_Lib.Alarm_Error_Occurrence(1203, dWARNING, Func.m_strAlarmCode);
				break;
			}

			m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
			m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
			m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속
			
			nRet_1 = CTL_Lib.Linear_Move(m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio); 
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				m_bDvcWaitChk_Falg =  false;
				mn_PickStep = 2950;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_PickStep = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1204, dWARNING, alarm.mstr_code);
				mn_PickStep = 2000;
			}
			break;

		///////////////////////////////////////////////
		//  디바이스를 실제 집는 위치 
		///////////////////////////////////////////////
		case 2900:
			if( g_ioMgr.get_out_bit( st_io.o_HeatSink_Trasnfer_Pad_Up_Sol, IO_ON) == IO_ON &&
				g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Pad_Up_Check, IO_ON) == IO_ON &&
				g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Pad_Down_Check, IO_OFF) == IO_OFF )
			{
				mn_PickStep = 3000;
			}
			else
			{
				Set_HeatSink_Transfer_Pad_UpDown(IO_OFF);
				mn_PickStep = 2910;
			}
			break;

		case 2910:
			nRet_1 = Chk_HeatSink_Transfer_Pad_UpDown(IO_OFF);
			if( nRet_1 == RET_GOOD )
			{
				mn_PickStep = 3000;
			}
			break;

		case 2950:
			if( nWork_Site == THD_LD_HEATSINK_BUFF )
			{//serial comm
				mn_PickStep = 2960;
			}
			else//THD_PICK_REVERSE_DVC
			{
				mn_PickStep = 3000;
			}
			break;

		case 2960:
			//
			mn_PickStep = 3000;
			break;

		case 3000: 
			nRet_1 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Transfer_Pad_Up_Check,	 IO_ON);
			nRet_2 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Transfer_Pad_Down_Check,	 IO_OFF); 
			nRet_3 = g_ioMgr.get_out_bit( st_io.o_HeatSink_Trasnfer_Pad_Up_Sol, IO_OFF);
			if( nRet_1 != IO_ON || nRet_2 != IO_OFF || nRet_3 != IO_ON) 
			{
				mn_PickStep = 2900;
				break;
			}

			nRet_1 = COMI.Check_MotPosRange(m_nRobot_X, m_dpTargetPosList[0], COMI.md_allow_value[m_nRobot_X]);
			nRet_2 = COMI.Check_MotPosRange(m_nRobot_Y, m_dpTargetPosList[1], COMI.md_allow_value[m_nRobot_Y]);
			if(nRet_1 != BD_GOOD || nRet_2 != BD_GOOD)
			{
				mn_PickStep = 2000;
				break;
			}

			if(nWork_Site == THD_LD_HEATSINK_BUFF)
			{
				m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_BOX_DOWN_POS];
			}
			else if(nWork_Site == THD_DISPENSOR_PRBT)
			{
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_0_Check, IO_ON);
				nRet_2 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Reverse_180_Check, IO_OFF);
				if(nRet_1 != IO_ON || nRet_2 != IO_OFF )
				{
					if(nRet_1 != IO_ON) strAlarm.Format("8%d%04d",st_io.i_HeatSink_Reverse_0_Check);
					else						strAlarm.Format("8%d%04d",st_io.i_HeatSink_Reverse_180_Check);
					CTL_Lib.Alarm_Error_Occurrence(1108, dWARNING, strAlarm);
					break;
				}
				m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_TURN_PICK_POS];
			}
			else
			{
				break;
			}

			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				if(nWork_Site == THD_LD_HEATSINK_BUFF)
				{
					Set_HeatSink_Vacuum_OnOff(IO_ON); 
					Set_HeatSink_Transfer_Pad_UpDown(IO_ON); 	
				}
				else// if( nWork_Site == THD_PICK_REVERSE_DVC)
				{

				}
				mn_PickStep = 3100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_PickStep = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1210, dWARNING, alarm.mstr_code);
				mn_PickStep = 3000;
			}		
			break;

		case 3100:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				if( nWork_Site == THD_LD_HEATSINK_BUFF)
				{
					mn_PickStep = 3110;
				}
				else//	if( nWork_Site == THD_PICK_REVERSE_DVC )
				{//serial comm
					mn_PickStep = 3300;
				}
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_PickStep = 3000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1211, dWARNING, alarm.mstr_code);
				mn_PickStep = 3000;
			}		
			break;

		case 3110: //picker down check 
			nRet_1 = Chk_HeatSink_Transfer_Pad_UpDown(IO_ON); 
			if(nRet_1 == RET_GOOD)
			{
				mn_PickStep = 3200;
			}
			else if(nRet_1 == RET_ERROR)
			{
				CTL_Lib.Alarm_Error_Occurrence(1212, dWARNING, m_strAlarmCode);
				mn_PickStep = 3000;
			}
			break;

		///////////////////////////////////////////
		//자재 클램프 
		///////////////////////////////////////////
		case 3200: //자재를 집는다 
			Set_HeatSink_Vacuum_OnOff(IO_ON);  
			mn_PickStep = 3210;
			break;

		case 3210:
			nRet_1 = Chk_HeatSink_Vacuum_OnOff(IO_ON); 
			if( st_basic.n_mode_device == WITHOUT_DVC ) nRet_1 = RET_GOOD;
			if(nRet_1 == RET_GOOD)
			{
				m_bDvcWaitChk_Falg = false;
				mn_PickStep = 3300;
			}
			else if(nRet_1 == RET_ERROR)
			{
				//Set_Glipper_OnOff(IO_ON);  //벌린다  
				CTL_Lib.Alarm_Error_Occurrence(1213, dWARNING, m_strAlarmCode);
				mn_PickStep = 3200;
			}
			break;

		case 3300:	
			if(nWork_Site == THD_LD_HEATSINK_BUFF)
			{
				nRet_1 = FAS_IO.Chk_IO_OnOff(st_io.o_HeatSink_Vacuum_On_Sol, IO_ON, IO_STABLE_WAIT, 1000);
				if( nRet_1 == RET_PROCEED)
					break;

				mn_PickStep = 4000;
			}
			else// THD_DISPENSOR_PRBT
			{
				mn_PickStep = 3400;// 
			} 
			break;

		//////////////////////////////////////////////////////////////////////////////
		// uld buffer의 그리퍼의 자재를 놓아 워킹로봇에서 자재를 집는것을 제어한다 
		//////////////////////////////////////////////////////////////////////////////
		case 3400: 
			//SERIAL COMM...............
			//..................................................
			//..................................................
			mn_PickStep = 3500;
			break;

		case 3500:
			nRet_1 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Transfer_CellIn_Check, IO_ON);
			if( st_basic.n_mode_device == WITHOUT_DVC ) nRet_1 = IO_ON;
			if( nRet_1 == IO_OFF )
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_HeatSink_Transfer_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(1219, dWARNING, m_strAlarmCode);
			}
			mn_PickStep = 3600;
			break;

		case 3600:
			st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_SORT;
			st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = WORK_PLACE;
			mn_PickStep = 3700;
			break;

		case 3700:
			if( st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] == CTL_FREE && 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] == WORK_PLACE)//wait until cylinder is opened
			{
				mn_PickStep = 3800;
			}
			break;

		case 3800:
			m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][0] = CTL_FINAL;
				st_sync.nHeatSinkRbt_Dvc_Req[THD_DISPENSOR_PRBT][1] = WORK_PLACE;
				mn_PickStep = 3900; //시간 단추을 위헤 체크는 다음으로 
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_PickStep = 3800;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1217, dWARNING, alarm.mstr_code);
				mn_PickStep = 3800;
			}		
			break;

		case 3900:
			if( st_basic.n_mode_device == WITHOUT_DVC )
				nRet_1 = IO_ON;
			else
				nRet_1 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Transfer_CellIn_Check, IO_ON);
			if(nRet_1 == IO_ON)
			{
				m_bDvcWaitChk_Falg = false;
				mn_PickStep = 5000;
			}
			else if(nRet_1 != IO_ON)
			{
				m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_HeatSink_Transfer_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(1219, dWARNING, m_strAlarmCode);
				mn_PickStep = 4000;
			}
			break;



		///////////////////////////////////////////
		//로보트 up 
		///////////////////////////////////////////
		case 4000:
			m_dpTargetPosList[2] = st_motor[m_nRobot_Z].md_pos[P_HEATSINK_TRANSFER_Z_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 
				Set_HeatSink_Transfer_Pad_UpDown(IO_OFF); //동작한 피커를 올린다  
				mn_PickStep = 4100 ; //시간 단추을 위헤 체크는 다음으로 
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_PickStep = 4000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1217, dWARNING, alarm.mstr_code);
				mn_PickStep = 4000;
			}		
			break;

		case 4100:
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK,m_nRobot_Z, m_dpTargetPosList[2], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{ 			 
				mn_PickStep = 4110;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_PickStep = 4000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1218, dWARNING, alarm.mstr_code);
				mn_PickStep = 4000;
			}		
			break;

		case 4110:
			nRet_1 = Chk_HeatSink_Transfer_Pad_UpDown(IO_OFF); //동작할 피커를 미리 내리면서 이동한다
			if( nWork_Site == THD_DISPENSOR_PRBT )
			{
				if( st_basic.n_mode_device == WITHOUT_DVC )
					nRet_2 = IO_ON;
				else
					nRet_2 = g_ioMgr.get_in_bit( st_io.i_HeatSink_Transfer_CellIn_Check, IO_ON);
			}
			else										nRet_2 = IO_ON;
			if(nRet_1 == RET_GOOD && nRet_2 == IO_ON )
			{
				m_bDvcWaitChk_Falg = false;
				mn_PickStep = 5000;
			}
			else if(nRet_1 == RET_ERROR || nRet_2 != IO_ON)
			{
				if( nRet_1 == RET_ERROR) CTL_Lib.Alarm_Error_Occurrence(1219, dWARNING, m_strAlarmCode);
				else
				{
					m_strAlarmCode.Format(_T("8%d%04d"), IO_ON, st_io.i_HeatSink_Transfer_CellIn_Check);
					CTL_Lib.Alarm_Error_Occurrence(1219, dWARNING, m_strAlarmCode);
				}
				mn_PickStep = 4000;
			}
			break;

		case 5000: //picker 정보와 상태를 확인한다
			//THD_LD_HEATSINK_BUFF THD_PICK_REVERSE_DVC
			nRet_1 = Func.Check_PickerStatus(1, nWork_Site, CTL_YES, m_npPicker_YesNo, m_npPicker_Vacuum_Status, m_npPicker_OutputStatus);
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
					if(m_dwWaitUntil[2] < IO_STABLE_WAIT) break;

					CTL_Lib.Alarm_Error_Occurrence(7927, dWARNING, Func.m_strAlarmCode);
					break;
				}
			}

			for (i = m_n_FirstPicker_Num; i < m_nPickerPara; i++)
			{		
				if( st_picker[m_nSitePicker].st_pcb_info[i].nYesNo == CTL_NO && m_npPicker_YesNo[i] == CTL_YES &&
					m_npPicker_Vacuum_Status[i] == IO_ON && m_npPicker_OutputStatus[i] == IO_ON/*IO_OFF*/ )
				{
					//아래 st_recipe.nHsTrayX 체크 할것
// 					if(nWork_Site == THD_LD_HEATSINK_BUFF) st_recipe.nHsTrayX;
// 					else								   st_recipe.nLdBuffer_Num
					if(i >= 0 && m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num) < st_recipe.nHsTrayX)
					{
						m_npPicker_YesNo[i] = CTL_NO; 

						m_npSet_WorkPosYXCPB[0]  = m_npFindWorkPosYXCPB[0] + (i - m_n_FirstPicker_Num); //tray X Pos 위치
						m_npSet_WorkPosYXCPB[1]  = m_npFindWorkPosYXCPB[1]; //tray Y Pos 위치
						m_npSet_WorkPosYXCPB[3]  = i; //picker 위치 정보

						Func.Data_Exchange_PickPlace(PICKER_PICK_MODE, 1, m_nSitePicker, nWork_Site, m_npSet_WorkPosYXCPB);		
						if (m_strLotNo[1] == _T(""))
						{
							m_strLotNo[1]	= st_picker[m_nSitePicker].st_pcb_info[i].strLotNo;
							m_strPartNo[1] = st_picker[m_nSitePicker].st_pcb_info[i].strPartNo;
						}
					}					
				}
			}

			if (nRet_1 == RET_GOOD) // 1000 step에서 셋팅한 피커 정보에 맞게 모두 정상적 집었을 경우 이
			{
				m_bDvcWaitChk_Falg = false;
				if (st_handler.cwnd_main != NULL)
				{
					//st_handler.cwnd_main->PostMessage(WM_WORK_DISPLAY, MAIN_LD_PICKER_DISPLAY, 0);
					//st_handler.cwnd_main->PostMessage(WM_WORK_DISPLAY, MAIN_LD_TRAY_DISPLAY, 0);
					//st_handler.cwnd_main->PostMessage(WM_WORK_DISPLAY, MAIN_RETEST_BUF_DISPLAY, 0);
				}
				mn_PickStep = 6000;
			}
			else if (nRet_1 == RET_ERROR) //셋팅한 피커중 한개라도 제대로 집지 못했으면 이곳으로 이동
			{
				CTL_Lib.Alarm_Error_Occurrence(1220, dWARNING, Func.m_strAlarmCode);
			}
			break;

		case 6000:
			nRet_1 = Func.Check_PickerStatus(0, m_nSitePicker, CTL_YES, m_npPicker_YesNo, m_npPicker_Data_Status, m_npPicker_OutputStatus);
			if (nRet_1 == RET_GOOD) //
			{
				nFuncRet = RET_GOOD;
				mn_PickStep = 0;
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
					CTL_Lib.Alarm_Error_Occurrence(1221, dWARNING, Func.m_strAlarmCode);
				}
			}
			break;

		default :
			break;
 
	}
	return nFuncRet;
}


int CRun_HeatSinkVision_Transfer_Robot::Robot_AutoMove_Safety_Zone( int nMode, int n_pos)
{
	int nRet_1=0, nRet_2=0, FuncRet = RET_PROCEED, i=0;
	double dCurrentPosX =0, dCurrentPosY=0;

	//nMode == 0 => Tray Pickup위치로 바로 갈수 있는 위치로 이동
	//nMode == 1 => Dispensor로 자재를 Place, Pick 할 수 있는 위치로 이동  대기로 이동
	//nMode == 2 => dispensoe에서 나오는 위치  대기로 이동
	//nMode == 3 => Carrier위치로 들어가는 위치

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
			nRet_1 = COMI.Check_MotPosRange(m_nRobot_Z, st_motor[m_nRobot_Z].md_pos[P_EPOXY_TRANSFER_Z_INIT_POS], st_motor[m_nRobot_Z].mn_allow);
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
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
				mn_SafetyStep = 200;
			}
			break;

			//nMode == 0 => Tray Pickup위치로 바로 갈수 있는 위치로 이동
			//nMode == 1 => Dispensor로 자재를 Place, Pick 할 수 있는 위치로 이동  대기로 이동
			//nMode == 2 => dispensoe에서 나오는 위치  대기로 이동
			//nMode == 3 => Carrier위치로 들어가는 위치

			//n_site ->   Turn -> ready , UV lmap  Carrier 1,2,3
			//n_site = 0 turn pos ==0 place  pos 1 pick
			//n_site = 1 uv lamp
			//n_site = 2 carrier pos == 0 carrier1, pos == 1 carrier2, pos == 2 carrier3

			//P_HEATSINK_TRANSFER_X_PLACE_POS  이상 P_HEATSINK_TRASNFER_Y_TURN_PLACE_POS 아래 dangerous
		
			//safety1 P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS,
			//safety2 P_HEATSINK_TRASNFER_X_TURN_READY_POS
			//safety3 P_HEATSINK_TRANSFER_Y_DISPENSOR_SAFETY_POS,
		case 1000:			
			if( dCurrentPosX >= ( st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[m_nRobot_X].mn_allow ) )
			{
				if( dCurrentPosY >=  ( st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] - st_motor[m_nRobot_Y].mn_allow ))
				{
					if( nMode == 3 )//이미 들어와 있다
					{
						FuncRet = RET_GOOD;
					}
					else
					{
						mn_SafetyStep = 1100;
					}
				}
				else if( ( dCurrentPosY <  ( st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS] - st_motor[m_nRobot_Y].mn_allow ) ) &&
					( dCurrentPosY >=  ( st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_DISPENSOR_SAFETY_POS] - st_motor[m_nRobot_Y].mn_allow ) ) )
				{//Carrier영역  이영역에서는 안전영역이므로 linear로 이동이 가능하다
					//nMode == 0 => Tray Pickup위치로 바로 갈수 있는 위치로 이동
					//nMode == 1 => Dispensor로 자재를 Place할 수 있는 위치로 이동
					//nMode == 2 => Dispensor로 자재를 Pick할 수 있는 위치로 이동
					if(nMode == 0 || nMode == 1 || nMode == 2)  
					{//after it moves turn safety pos first, and return good
						mn_SafetyStep = 2000;	//이미Carrier 안으로 들어가 있는 상태로 어느 위치에 있는지 확인한다 
					}
					else// if(nMode == 4)
					{
						mn_SafetyStep = 1100;
					}
				}
				else// if( dCurrentPosY <=  ( st_motor[dCurrentPosY].md_pos[P_HEATSINK_TRANSFER_Y_DISPENSOR_SAFETY_POS] - st_motor[m_nRobot_X].mn_allow ) )
				{
					if(nMode == 0 || nMode == 1 || nMode == 2 || nMode == 3)  //nMode == 0 => x축만 우선 safety로 움직인뒤에 turn ready로 움직인다.
					{
						mn_SafetyStep = 5000;
					}
					else// if(nMode == 4)
					{
						mn_SafetyStep = 5000;
					}
				}
			}
			else //if( dCurrentPosX < ( st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS] + st_motor[m_nRobot_Y].mn_allow ) )
			{
				//nMode == 0 => Tray Pickup위치로 바로 갈수 있는 위치로 이동
				//nMode == 1 => Dispensor로 자재를 Place할 수 있는 위치로 이동
				//nMode == 2 => Dispensor로 자재를 Pick할 수 있는 위치로 이동
				//nMode == 4 => Carrier위치로 이동가능한 위치로 이동
				if( dCurrentPosY >=  ( st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_DISPENSOR_SAFETY_POS] - st_motor[m_nRobot_Y].mn_allow ) )
				{
					if(nMode == 0 )
					{
						mn_SafetyStep = 1100;
					}
					else if(nMode == 3)
					{
						FuncRet = RET_GOOD;
						mn_SafetyStep = 0;
					}
					else if( nMode == 1 || nMode == 2 )//move to turn ready
					{
						mn_SafetyStep = 2000;
					}
					else// if( nMode == 4 )
					{
						mn_SafetyStep = 1100;
					}
				}
				else// if( dCurrentPosY < ( st_motor[dCurrentPosY].md_pos[P_HEATSINK_TRANSFER_Y_DISPENSOR_SAFETY_POS] - st_motor[m_nRobot_X].mn_allow ) )
				{
					if(nMode == 0 ||nMode == 3)
					{
						FuncRet = RET_GOOD;
						mn_SafetyStep = 0;
					}
					else if( nMode == 1 || nMode == 2 )//move to turn ready
					{
						mn_SafetyStep = 2000;
					}
					else// if( nMode == 4 )
					{
						mn_SafetyStep = 1100;
					}
				}
			}
			break;		

		case 1100:
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRANSFER_X_CARRIER_SAFETY_POS];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_Y_CARRIER_SAFETY_POS];

			m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
			m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
			m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				FuncRet = RET_GOOD;
				mn_SafetyStep = 0;

			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 1100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
				mn_SafetyStep = 1100;
			}
			break;

		case 1200://n_pos carrier 사이트로 이동한 후 대기
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRANSFER_X_PLACE_TOPPOS+n_pos], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				FuncRet = RET_GOOD;
				mn_SafetyStep = 0;					
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 1200;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
				mn_SafetyStep = 1200;
			}
			break;


		case 2000://after it moves turn safety pos first, and return good
			m_dpTargetPosList[0] = st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS];
			m_dpTargetPosList[1] = st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRASNFER_Y_TURN_READY_POS];

			m_dpSpdRatio[0] = (double)COMI.mn_runspeed_rate;	//work 속도 
			m_dpSpdRatio[1] = (double)COMI.mn_runspeed_rate;	// 가속 
			m_dpSpdRatio[2] = (double)COMI.mn_runspeed_rate;	// 감속 

			nRet_1 = CTL_Lib.Linear_Move( m_nLinearMove_Index, m_lAxisCnt, m_lpAxisNum, m_dpTargetPosList, m_dpSpdRatio);
			if(nRet_1 == BD_GOOD) //정상적으로 완료된 상태
			{
				FuncRet = RET_GOOD;
				mn_SafetyStep = 0;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{
				mn_SafetyStep = 2000;
				CTL_Lib.Alarm_Error_Occurrence(3620, dWARNING, alarm.mstr_code);
			}
			break;

		case 5000://nMode == 0 , 1=> x축만 우선 safety로 움직인뒤에 turn ready로 움직인다.
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_X, st_motor[m_nRobot_X].md_pos[P_HEATSINK_TRASNFER_X_TURN_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_SafetyStep = 5100;
				if( nMode == 1  || nMode == 2 )
				{
					mn_SafetyStep = 5100;
				}
				else if(nMode == 3 )
				{
					FuncRet = RET_GOOD;
					mn_SafetyStep = 0;					
				}
				else if(nMode == 4 )
				{
					mn_SafetyStep = 1100;
				}
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 5000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
				mn_SafetyStep = 5000;
			}
			break;

		case 5100:
			nRet_1 = CTL_Lib.Single_Move(BOTH_MOVE_FINISH, m_nRobot_Y, st_motor[m_nRobot_Y].md_pos[P_HEATSINK_TRASNFER_Y_TURN_READY_POS], COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				FuncRet = RET_GOOD;
				mn_SafetyStep = 0;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_SafetyStep = 5100;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1104, dWARNING, alarm.mstr_code);
				mn_SafetyStep = 5100;
			}
			break;
	}

	return FuncRet;
}

void CRun_HeatSinkVision_Transfer_Robot::Set_Vision_Y_Clamp_ForeardBackward(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];
	
	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);
	
	m_bVisionFwdBwdFlag	= false;
	m_dwVisionwdFwdBwd[0] = GetCurrentTime();
	
	g_ioMgr.set_out_bit(st_io.o_camera_y_clamp_sol, OnOff);
	g_ioMgr.set_out_bit(st_io.o_camera_y_unclamp_sol, !OnOff);
	
	if (OnOff == IO_OFF)
	{
		clsLog.LogFunction(_T("VISION_ROBOT"),_T("BACKWARD"),0,_T("CLAMP"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("VISION_ROBOT"),_T("FORWARD"),0,_T("CLAMP"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

int CRun_HeatSinkVision_Transfer_Robot::Chk_Vision_Y_Clamp_ForeardBackward(int OnOff)
{

	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_CAMERA_CLAMP_FWDBWD;

	if (OnOff == IO_OFF)
	{
		if (m_bVisionFwdBwdFlag == false && g_ioMgr.get_in_bit(st_io.i_camera_unclamp_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_camera_clamp_chk, IO_OFF) == IO_OFF )
		{
			m_bVisionFwdBwdFlag		= true;
			m_dwVisionwdFwdBwd[0]	= GetCurrentTime();
		}
		else if (m_bVisionFwdBwdFlag == true && g_ioMgr.get_in_bit(st_io.i_camera_unclamp_chk, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_camera_clamp_chk, IO_OFF) == IO_OFF )
		{
			m_dwVisionwdFwdBwd[1] = GetCurrentTime();
			m_dwVisionwdFwdBwd[2] = m_dwVisionwdFwdBwd[1] - m_dwVisionwdFwdBwd[0];

			if (m_dwVisionwdFwdBwd[2] <= 0)
			{
				m_dwVisionwdFwdBwd[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwVisionwdFwdBwd[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("VISION_ROBOT"),_T("BACKWARD"),1,_T("CLAMP"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwVisionwdFwdBwd[1] = GetCurrentTime();
			m_dwVisionwdFwdBwd[2] = m_dwVisionwdFwdBwd[1] - m_dwVisionwdFwdBwd[0];

			if (m_dwVisionwdFwdBwd[2] <= 0)
			{
				m_dwVisionwdFwdBwd[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwVisionwdFwdBwd[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_camera_unclamp_chk); 
		clsLog.LogFunction(_T("VISION_ROBOT"),_T("BACKWARD"),0,_T("CLAMP"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bVisionFwdBwdFlag == false && g_ioMgr.get_in_bit(st_io.i_camera_unclamp_chk, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_camera_clamp_chk, IO_ON) == IO_ON )
		{
			m_bVisionFwdBwdFlag			= true;
			m_dwVisionwdFwdBwd[0]	= GetCurrentTime();
		}
		else if (m_bVisionFwdBwdFlag == true && g_ioMgr.get_in_bit(st_io.i_camera_unclamp_chk, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_camera_clamp_chk, IO_ON) == IO_ON )
		{
			m_dwVisionwdFwdBwd[1]	= GetCurrentTime();
			m_dwVisionwdFwdBwd[2]	= m_dwVisionwdFwdBwd[1] - m_dwVisionwdFwdBwd[0];

			if (m_dwVisionwdFwdBwd[2] <= 0)
			{
				m_dwVisionwdFwdBwd[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwVisionwdFwdBwd[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("VISION_ROBOT"),_T("FORWARD"),1,_T("CLAMP"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwVisionwdFwdBwd[1]	= GetCurrentTime();
			m_dwVisionwdFwdBwd[2]	= m_dwVisionwdFwdBwd[1] - m_dwVisionwdFwdBwd[0];

			if (m_dwVisionwdFwdBwd[2] <= 0)
			{
				m_dwVisionwdFwdBwd[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwVisionwdFwdBwd[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_camera_unclamp_chk); 
				clsLog.LogFunction(_T("VISION_ROBOT"),_T("FORWARD"),1,_T("CLAMP"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}

void CRun_HeatSinkVision_Transfer_Robot::Set_HeatSink_Reverse_Turn(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bReverseFlag	= false;
	m_dwReverse[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit(st_io.o_HeatSink_Reverse_0_Sol, OnOff);
	g_ioMgr.set_out_bit(st_io.o_HeatSink_Reverse_180_Sol, !OnOff);

	if (OnOff == IO_OFF)
	{
		clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("REVERSE_0"),0,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("REVERSE_180"),0,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

int CRun_HeatSinkVision_Transfer_Robot::Chk_HeatSink_Reverse_Turn(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_DISPENSOR_REVERSE;

	if (OnOff == IO_OFF)
	{
		if (m_bReverseFlag == false && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_0_Check, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_180_Check, IO_OFF) == IO_OFF )
		{
			m_bReverseFlag		= true;
			m_dwReverse[0]	= GetCurrentTime();
		}
		else if (m_bReverseFlag == true && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_0_Check, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_180_Check, IO_OFF) == IO_OFF )
		{
			m_dwReverse[1] = GetCurrentTime();
			m_dwReverse[2] = m_dwReverse[1] - m_dwReverse[0];

			if (m_dwReverse[2] <= 0)
			{
				m_dwReverse[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwReverse[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("REVERSE_0"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwReverse[1] = GetCurrentTime();
			m_dwReverse[2] = m_dwReverse[1] - m_dwReverse[0];

			if (m_dwReverse[2] <= 0)
			{
				m_dwReverse[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwReverse[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_HeatSink_Reverse_0_Check); 
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("REVERSE_0"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bReverseFlag == false && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_0_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_180_Check, IO_ON) == IO_ON )
		{
			m_bReverseFlag			= true;
			m_dwReverse[0]	= GetCurrentTime();
		}
		else if (m_bReverseFlag == true && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_0_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_180_Check, IO_ON) == IO_ON )
		{
			m_dwReverse[1]	= GetCurrentTime();
			m_dwReverse[2]	= m_dwReverse[1] - m_dwReverse[0];

			if (m_dwReverse[2] <= 0)
			{
				m_dwReverse[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwReverse[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("REVERSE_180"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwReverse[1]	= GetCurrentTime();
			m_dwReverse[2]	= m_dwReverse[1] - m_dwReverse[0];

			if (m_dwReverse[2] <= 0)
			{
				m_dwReverse[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwReverse[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_HeatSink_Reverse_0_Check); 
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("REVERSE_180"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}

void CRun_HeatSinkVision_Transfer_Robot::Set_HeatSink_Reverse_UpDown(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bReverseUpDnFlag	= false;
	m_dwReverseUpDn[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit(st_io.o_HeatSink_Reverse_Up_Sol, OnOff);
	g_ioMgr.set_out_bit(st_io.o_HeatSink_Reverse_Down_Sol, !OnOff);

	if (OnOff == IO_OFF)
	{
		clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("DOWN"),0,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("UP"),0,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}

int CRun_HeatSinkVision_Transfer_Robot::Chk_HeatSink_Reverse_UpDown( int OnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_DISPENSOR_REVERSE_UPDN;

	if (OnOff == IO_OFF)
	{
		if (m_bReverseUpDnFlag == false && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Up_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Down_Check, IO_ON) == IO_ON )
		{
			m_bReverseUpDnFlag		= true;
			m_dwReverseUpDn[0]	= GetCurrentTime();
		}
		else if (m_bReverseUpDnFlag == true && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Up_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Down_Check, IO_ON) == IO_ON )
		{
			m_dwReverseUpDn[1] = GetCurrentTime();
			m_dwReverseUpDn[2] = m_dwReverseUpDn[1] - m_dwReverseUpDn[0];

			if (m_dwReverseUpDn[2] <= 0)
			{
				m_dwReverseUpDn[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwReverseUpDn[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("DOWN"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwReverseUpDn[1] = GetCurrentTime();
			m_dwReverseUpDn[2] = m_dwReverseUpDn[1] - m_dwReverseUpDn[0];

			if (m_dwReverseUpDn[2] <= 0)
			{
				m_dwReverseUpDn[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwReverseUpDn[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_HeatSink_Reverse_Up_Check); 
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("DOWN"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bReverseUpDnFlag == false && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Up_Check, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Down_Check, IO_OFF) == IO_OFF )
		{
			m_bReverseUpDnFlag			= true;
			m_dwReverseUpDn[0]	= GetCurrentTime();
		}
		else if (m_bReverseUpDnFlag == true &&g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Up_Check, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Down_Check, IO_OFF) == IO_OFF )
		{
			m_dwReverseUpDn[1]	= GetCurrentTime();
			m_dwReverseUpDn[2]	= m_dwReverseUpDn[1] - m_dwReverseUpDn[0];

			if (m_dwReverseUpDn[2] <= 0)
			{
				m_dwReverseUpDn[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwReverseUpDn[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("UP"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwReverseUpDn[1]	= GetCurrentTime();
			m_dwReverseUpDn[2]	= m_dwReverseUpDn[1] - m_dwReverseUpDn[0];

			if (m_dwReverseUpDn[2] <= 0)
			{
				m_dwReverseUpDn[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwReverseUpDn[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_HeatSink_Reverse_Up_Check); 
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("UP"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}

void CRun_HeatSinkVision_Transfer_Robot::Set_HeatSink_Reverse_Clamp_ForwardBackward(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bReverseFwdBwdFlag	= false;
	m_dwReverseFwdBwd[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit(st_io.o_HeatSink_Reverse_Clamp_On_Sol, OnOff);
	g_ioMgr.set_out_bit(st_io.o_HeatSink_Reverse_Clamp_Off_Sol, !OnOff);

	if (OnOff == IO_OFF)
	{
		clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("BACKWARD"),0,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("FORWARD"),0,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}


int CRun_HeatSinkVision_Transfer_Robot::Chk_HeatSink_Reverse_Clamp_ForwardBackward( int OnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_DISPENSOR_REVERSE_FWDBWD;

	if (OnOff == IO_OFF)
	{
		if (m_bReverseFwdBwdFlag == false && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Clamp_On_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Clamp_Off_Check, IO_ON) == IO_ON )
		{
			m_bReverseFwdBwdFlag		= true;
			m_dwReverseFwdBwd[0]	= GetCurrentTime();
		}
		else if (m_bReverseFwdBwdFlag == true && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Clamp_On_Check, IO_OFF) == IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Clamp_Off_Check, IO_ON) == IO_ON )
		{
			m_dwReverseFwdBwd[1] = GetCurrentTime();
			m_dwReverseFwdBwd[2] = m_dwReverseFwdBwd[1] - m_dwReverseFwdBwd[0];

			if (m_dwReverseFwdBwd[2] <= 0)
			{
				m_dwReverseFwdBwd[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwReverseFwdBwd[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("BACKWARD"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwReverseFwdBwd[1] = GetCurrentTime();
			m_dwReverseFwdBwd[2] = m_dwReverseFwdBwd[1] - m_dwReverseFwdBwd[0];

			if (m_dwReverseFwdBwd[2] <= 0)
			{
				m_dwReverseFwdBwd[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwReverseFwdBwd[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_HeatSink_Reverse_Clamp_On_Check); 
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("BACKWARD"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bReverseFwdBwdFlag == false && g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Clamp_On_Check, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Clamp_Off_Check, IO_OFF) == IO_OFF )
		{
			m_bReverseFwdBwdFlag			= true;
			m_dwReverseFwdBwd[0]	= GetCurrentTime();
		}
		else if (m_bReverseFwdBwdFlag == true &&g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Clamp_On_Check, IO_ON) == IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Reverse_Clamp_Off_Check, IO_OFF) == IO_OFF )
		{
			m_dwReverseFwdBwd[1]	= GetCurrentTime();
			m_dwReverseFwdBwd[2]	= m_dwReverseFwdBwd[1] - m_dwReverseFwdBwd[0];

			if (m_dwReverseFwdBwd[2] <= 0)
			{
				m_dwReverseFwdBwd[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwReverseFwdBwd[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("FORWARD"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwReverseFwdBwd[1]	= GetCurrentTime();
			m_dwReverseFwdBwd[2]	= m_dwReverseFwdBwd[1] - m_dwReverseFwdBwd[0];

			if (m_dwReverseFwdBwd[2] <= 0)
			{
				m_dwReverseFwdBwd[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwReverseFwdBwd[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_HeatSink_Reverse_Clamp_On_Check); 
				clsLog.LogFunction(_T("DISPENSER_ROBOT"),_T("FORWARD"),1,_T("REVERSE"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}

void CRun_HeatSinkVision_Transfer_Robot::Set_Dispenser_Air_Blow(int OnOff)
{

	if (OnOff == IO_OFF)
	{
		if(st_basic.n_mode_7387 == CTL_YES)
		{
			g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_On_Sol, IO_OFF);	
			g_ioMgr.set_out_bit(st_io.o_DisPenser_1_AirBlow_On_Sol, IO_OFF);	

			g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_On_Sol, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_Off_Sol, IO_ON);
			g_ioMgr.set_out_bit(st_io.o_Dispenser_Air_Timer_Start, IO_OFF);	
			g_ioMgr.set_out_bit(st_io.o_Dispenser_Air_Timer_Reset, IO_ON);

			g_ioMgr.set_out_bit(st_io.o_Dispenser_Air_Timer_Reset, IO_OFF);
			st_work.n7387BlowCheck = 0;
		}
	}
	else
	{
		if(st_basic.n_mode_7387 == CTL_YES)
		{
			g_ioMgr.set_out_bit(st_io.o_Dispenser_Air_Timer_Reset, IO_ON);
			Sleep(1);
			g_ioMgr.set_out_bit(st_io.o_Dispenser_Air_Timer_Reset, IO_OFF);
			g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_On_Sol, IO_ON);
			g_ioMgr.set_out_bit(st_io.o_DisPenser_1_AirBlow_On_Sol, IO_ON);		
			g_ioMgr.set_out_bit(st_io.o_Dispenser_1_Liquide_Off_Sol, IO_OFF);	
			g_ioMgr.set_out_bit(st_io.o_Dispenser_Air_Timer_Start, IO_ON);
			st_work.n7387BlowCheck = 1;
		}
	}

}

void CRun_HeatSinkVision_Transfer_Robot::Set_HeatSink_Vacuum_OnOff(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bPadVacuumFlag	= false;
	m_dwPadVacuum[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_HeatSink_Vacuum_On_Sol, OnOff);

	if (OnOff == IO_OFF)
	{
		clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("OFF"),0,_T("PICKER"),_T("VACUUM"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("ON"),0,_T("PICKER"),_T("VACUUM"),1,strLogKey,strLogData);
	}
}


int CRun_HeatSinkVision_Transfer_Robot::Chk_HeatSink_Vacuum_OnOff( int OnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_HEATSINK_PICKER_VACUUM;

	if (OnOff == IO_OFF)
	{
		if (m_bPadVacuumFlag == false &&	 g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Vacuum_Check, IO_OFF)	== IO_OFF)
		{
			m_bPadVacuumFlag		= true;
			m_dwPadVacuum[0]	= GetCurrentTime();
		}
		else if (m_bPadVacuumFlag == true && g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Vacuum_Check, IO_OFF)	== IO_OFF)
		{
			m_dwPadVacuum[1] = GetCurrentTime();
			m_dwPadVacuum[2] = m_dwPadVacuum[1] - m_dwPadVacuum[0];

			if (m_dwPadVacuum[2] <= 0)
			{
				m_dwPadVacuum[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwPadVacuum[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("OFF"),1,_T("PICKER"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwPadVacuum[1] = GetCurrentTime();
			m_dwPadVacuum[2] = m_dwPadVacuum[1] - m_dwPadVacuum[0];

			if (m_dwPadVacuum[2] <= 0)
			{
				m_dwPadVacuum[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwPadVacuum[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_HeatSink_Transfer_Vacuum_Check); 
				clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("OFF"),1,_T("PICKER"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bPadVacuumFlag == false && g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Vacuum_Check, IO_ON) == IO_ON )
		{
			m_bPadVacuumFlag			= true;
			m_dwPadVacuum[0]	= GetCurrentTime();
		}
		else if (m_bPadVacuumFlag == true && g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Vacuum_Check, IO_ON)	== IO_ON )
		{
			m_dwPadVacuum[1]	= GetCurrentTime();
			m_dwPadVacuum[2]	= m_dwPadVacuum[1] - m_dwPadVacuum[0];

			if (m_dwPadVacuum[2] <= 0)
			{
				m_dwPadVacuum[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwPadVacuum[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("ON"),1,_T("PICKER"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwPadVacuum[1]	= GetCurrentTime();
			m_dwPadVacuum[2]	= m_dwPadVacuum[1] - m_dwPadVacuum[0];

			if (m_dwPadVacuum[2] <= 0)
			{
				m_dwPadVacuum[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwPadVacuum[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_HeatSink_Transfer_Vacuum_Check); 
				clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("ON"),1,_T("PICKER"),_T("VACUUM"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}

void CRun_HeatSinkVision_Transfer_Robot::Set_HeatSink_Transfer_Pad_UpDown(int OnOff)
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bPadUpDnFlag	= false;
	m_dwPadUpDn[0]	= GetCurrentTime();

	//ON Down OFF Up
	g_ioMgr.set_out_bit( st_io.o_HeatSink_Trasnfer_Pad_Up_Sol, !OnOff);
	g_ioMgr.set_out_bit( st_io.o_HeatSink_Trasnfer_Pad_Down_Sol, OnOff);

	if (OnOff == IO_OFF)
	{
		clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("UP"),0,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("DOWN"),0,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}


int CRun_HeatSinkVision_Transfer_Robot::Chk_HeatSink_Transfer_Pad_UpDown( int OnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_HEATSINK_PICKER_UPDN;

	if (OnOff == IO_OFF)//UP
	{
		if (m_bPadUpDnFlag == false &&	g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Pad_Up_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Pad_Down_Check, IO_OFF)	== IO_OFF)
		{
			m_bPadUpDnFlag		= true;
			m_dwPadUpDn[0]	= GetCurrentTime();
		}
		else if (m_bPadUpDnFlag == true && g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Pad_Up_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Pad_Down_Check, IO_OFF)	== IO_OFF)
		{
			m_dwPadUpDn[1] = GetCurrentTime();
			m_dwPadUpDn[2] = m_dwPadUpDn[1] - m_dwPadUpDn[0];

			if (m_dwPadUpDn[2] <= 0)
			{
				m_dwPadUpDn[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwPadUpDn[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("UP"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwPadUpDn[1] = GetCurrentTime();
			m_dwPadUpDn[2] = m_dwPadUpDn[1] - m_dwPadUpDn[0];

			if (m_dwPadUpDn[2] <= 0)
			{
				m_dwPadUpDn[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwPadUpDn[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_HeatSink_Transfer_Pad_Up_Check); 
				clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("UP"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bPadUpDnFlag == false && g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Pad_Up_Check, IO_OFF)	== IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Pad_Down_Check, IO_ON)	== IO_ON)
		{
			m_bPadUpDnFlag			= true;
			m_dwPadUpDn[0]	= GetCurrentTime();
		}
		else if (m_bPadUpDnFlag == true && g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Pad_Up_Check, IO_OFF)	== IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_HeatSink_Transfer_Pad_Down_Check, IO_ON)	== IO_ON)
		{
			m_dwPadUpDn[1]	= GetCurrentTime();
			m_dwPadUpDn[2]	= m_dwPadUpDn[1] - m_dwPadUpDn[0];

			if (m_dwPadUpDn[2] <= 0)
			{
				m_dwPadUpDn[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwPadUpDn[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("DOWN"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwPadUpDn[1]	= GetCurrentTime();
			m_dwPadUpDn[2]	= m_dwPadUpDn[1] - m_dwPadUpDn[0];

			if (m_dwPadUpDn[2] <= 0)
			{
				m_dwPadUpDn[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwPadUpDn[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), OnOff, st_io.i_HeatSink_Transfer_Pad_Up_Check); 
				clsLog.LogFunction(_T("DVC_HEATSINK_ROBOT"),_T("DOWN"),1,_T("PICKER"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}


void CRun_HeatSinkVision_Transfer_Robot::Set_Device_Carrier_Camera_Y_Press_UpDown(int nOnOff)	
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bCameraUpDnFlag	= false;
	m_dwCameraUpDn[0]	= GetCurrentTime();
	
	//ON Down OFF Up
	g_ioMgr.set_out_bit( st_io.o_Camera_Y_Press_Up_Sol, !nOnOff);
	g_ioMgr.set_out_bit( st_io.o_Camera_Y_Press_Down_Sol, nOnOff);

	if (nOnOff == IO_OFF)
	{
		clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("UP"),0,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("DOWN"),0,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}


int CRun_HeatSinkVision_Transfer_Robot::Chk_Device_Carrier_Camera_Y_Press_UpDown( int nOnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_CAMERA_CLAMP_UPDN;

	if (nOnOff == IO_OFF)//UP
	{
		if (m_bCameraUpDnFlag == false &&	g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Up_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Down_Check, IO_OFF)	== IO_OFF)
		{
			m_bCameraUpDnFlag		= true;
			m_dwCameraUpDn[0]	= GetCurrentTime();
		}
		else if (m_bCameraUpDnFlag == true && g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Up_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Down_Check, IO_OFF)	== IO_OFF)
		{
			m_dwCameraUpDn[1] = GetCurrentTime();
			m_dwCameraUpDn[2] = m_dwCameraUpDn[1] - m_dwCameraUpDn[0];

			if (m_dwCameraUpDn[2] <= 0)
			{
				m_dwCameraUpDn[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCameraUpDn[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("UP"),1,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCameraUpDn[1] = GetCurrentTime();
			m_dwCameraUpDn[2] = m_dwCameraUpDn[1] - m_dwCameraUpDn[0];

			if (m_dwCameraUpDn[2] <= 0)
			{
				m_dwCameraUpDn[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCameraUpDn[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Camera_Y_Press_Up_Check); 
				clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("UP"),1,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bCameraUpDnFlag == false && g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Up_Check, IO_OFF)	== IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Down_Check, IO_ON)	== IO_ON)
		{
			m_bCameraUpDnFlag			= true;
			m_dwCameraUpDn[0]	= GetCurrentTime();
		}
		else if (m_bCameraUpDnFlag == true && g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Up_Check, IO_OFF)	== IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Camera_Y_Press_Down_Check, IO_ON)	== IO_ON)
		{
			m_dwCameraUpDn[1]	= GetCurrentTime();
			m_dwCameraUpDn[2]	= m_dwCameraUpDn[1] - m_dwCameraUpDn[0];

			if (m_dwCameraUpDn[2] <= 0)
			{
				m_dwCameraUpDn[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwCameraUpDn[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("DOWN"),1,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCameraUpDn[1]	= GetCurrentTime();
			m_dwCameraUpDn[2]	= m_dwCameraUpDn[1] - m_dwCameraUpDn[0];

			if (m_dwCameraUpDn[2] <= 0)
			{
				m_dwCameraUpDn[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCameraUpDn[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Camera_Y_Press_Up_Check); 
				clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("DOWN"),1,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}

void CRun_HeatSinkVision_Transfer_Robot::Set_Device_CameraY_Jig_Press_ForwardBackward(int nOnOff)
{
	///////////Jig 옆에서 밀어주는넘 
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);

	m_bCameraFwdBwdFlag	= false;
	m_dwCameraFwdBwd[0]	= GetCurrentTime();

	g_ioMgr.set_out_bit( st_io.o_Camera_Y_Jig_Press_Forward_Sol, nOnOff);
	g_ioMgr.set_out_bit( st_io.o_Camera_Y_Jig_Press_Backward_Sol, !nOnOff);

	if (nOnOff == IO_ON)
	{
		clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("FORWARD"),0,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("BACKWARD"),0,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
	}
}


int CRun_HeatSinkVision_Transfer_Robot::Chk_Device_CameraY_Jig_Press_ForwardBackward( int nOnOff )
{
	CString strLogKey[10];
	CString	strLogData[10];

	strLogKey[0] = _T("Mode End");
	strLogData[0].Format(_T("%d"),0);

	int nWaitTime = WAIT_CAMERA_CLAMP_FWDBWD;

	if (nOnOff == IO_OFF)//Backward
	{
		if (m_bCameraFwdBwdFlag == false &&	g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_OFF)	== IO_OFF)
		{
			m_bCameraFwdBwdFlag		= true;
			m_dwCameraFwdBwd[0]	= GetCurrentTime();
		}
		else if (m_bCameraFwdBwdFlag == true && g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_ON)	== IO_ON &&
			g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_OFF)	== IO_OFF)
		{
			m_dwCameraFwdBwd[1] = GetCurrentTime();
			m_dwCameraFwdBwd[2] = m_dwCameraFwdBwd[1] - m_dwCameraFwdBwd[0];

			if (m_dwCameraFwdBwd[2] <= 0)
			{
				m_dwCameraFwdBwd[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCameraFwdBwd[2] > (DWORD)st_wait.nOffWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("BACKWARD"),1,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCameraFwdBwd[1] = GetCurrentTime();
			m_dwCameraFwdBwd[2] = m_dwCameraFwdBwd[1] - m_dwCameraFwdBwd[0];

			if (m_dwCameraFwdBwd[2] <= 0)
			{
				m_dwCameraFwdBwd[0] = GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCameraFwdBwd[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Camera_Y_Jig_Press_Forward_Check); 
				clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("BACKWARD"),1,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}
	else
	{
		if (m_bCameraFwdBwdFlag == false && g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_OFF)	== IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_ON)	== IO_ON)
		{
			m_bCameraFwdBwdFlag			= true;
			m_dwCameraFwdBwd[0]	= GetCurrentTime();
		}
		else if (m_bCameraFwdBwdFlag == true && g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Backward_Check, IO_OFF)	== IO_OFF &&
			g_ioMgr.get_in_bit(st_io.i_Camera_Y_Jig_Press_Forward_Check, IO_ON)	== IO_ON)
		{
			m_dwCameraFwdBwd[1]	= GetCurrentTime();
			m_dwCameraFwdBwd[2]	= m_dwCameraUpDn[1] - m_dwCameraUpDn[0];

			if (m_dwCameraFwdBwd[2] <= 0)
			{
				m_dwCameraFwdBwd[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if(m_dwCameraFwdBwd[2] > (DWORD)st_wait.nOnWaitTime[nWaitTime])
			{
				clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("FORWARD"),1,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_GOOD;
			}
		}
		else
		{
			m_dwCameraFwdBwd[1]	= GetCurrentTime();
			m_dwCameraFwdBwd[2]	= m_dwCameraFwdBwd[1] - m_dwCameraFwdBwd[0];

			if (m_dwCameraFwdBwd[2] <= 0)
			{
				m_dwCameraFwdBwd[0]	= GetCurrentTime();
				return RET_PROCEED;
			}

			if (m_dwCameraFwdBwd[2] > (DWORD)st_wait.nLimitWaitTime[nWaitTime])
			{
				m_strAlarmCode.Format(_T("8%d%04d"), nOnOff, st_io.i_Camera_Y_Jig_Press_Forward_Check); 
				clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("FORWARD"),1,_T("CAMERA_ZIG"),_T("CYLINDER"),1,strLogKey,strLogData);
				return RET_ERROR;
			}
		}
	}

	return RET_PROCEED;
}

void CRun_HeatSinkVision_Transfer_Robot::Set_Device_Carrier_Camera_UV_LAMP_OnOff(int OnOff)	
{
	CString strLogKey[10];
	CString	strLogData[10];
	
	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);
	
	
	g_ioMgr.set_out_bit( st_io.o_Camera_UV_Lamp_HW_On_RY1, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Camera_UV_Lamp_HW_On_RY2, OnOff);
	
	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("DVC_HEASTSINK_ROBOT"),_T("ON"),0,_T("MEASURE_LAMP"),_T("LAMP"),1,strLogKey,strLogData);
		clsLog.LogFunction(_T("DVC_HEASTSINK_ROBOT"),_T("ON"),1,_T("MEASURE_LAMP"),_T("LAMP"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DVC_HEASTSINK_ROBOT"),_T("OFF"),0,_T("MEASURE_LAMP"),_T("LAMP"),1,strLogKey,strLogData);
		clsLog.LogFunction(_T("DVC_HEASTSINK_ROBOT"),_T("OFF"),1,_T("MEASURE_LAMP"),_T("LAMP"),1,strLogKey,strLogData);
	}
}


void CRun_HeatSinkVision_Transfer_Robot::Set_Device_Carrier_Camera_LED_LAMP_OnOff(int OnOff)	
{
	CString strLogKey[10];
	CString	strLogData[10];
	
	strLogKey[0] = _T("Mode Start");
	strLogData[0].Format(_T("%d"),0);
	
	
	g_ioMgr.set_out_bit( st_io.o_Camera_LED_Lamp_HW_On_RY1, OnOff);
	g_ioMgr.set_out_bit( st_io.o_Camera_LED_Lamp_HW_On_RY2, OnOff);
	
	if (OnOff == IO_ON)
	{
		clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("ON"),0,_T("INSPECT_LAMP"),_T("LAMP"),1,strLogKey,strLogData);
		clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("ON"),1,_T("INSPECT_LAMP"),_T("LAMP"),1,strLogKey,strLogData);
	}
	else
	{
		clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("OFF"),0,_T("INSPECT_LAMP"),_T("LAMP"),1,strLogKey,strLogData);
		clsLog.LogFunction(_T("DVC_VISION_ROBOT"),_T("OFF"),1,_T("INSPECT_LAMP"),_T("LAMP"),1,strLogKey,strLogData);
	}
}


