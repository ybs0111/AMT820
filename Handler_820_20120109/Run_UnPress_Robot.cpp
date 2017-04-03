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
}

void CRun_UnPress_Robot::RunInit()
{
}

void CRun_UnPress_Robot::RunMove()
{
	int nRet_1;
	int nRet[9] = {0,};
	long lMotionDone=0;
	CString strTemp;

	double mCurrPosY = COMI.Get_MotCurrentPos( M_PRESS_Y );
	double mCurrPosX = COMI.Get_MotCurrentPos( M_CARRIER_X );

	Func.ThreadFunctionStepTrace(11, mn_RunStep);
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
			if( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) )
			{
				mn_RunStep = 210;
				break;
			}
			else
			{
				if( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_INIT_POS] + st_motor[m_nPressAxisX].mn_allow ) )
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
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, alarm.mstr_code);
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
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, alarm.mstr_code);
				mn_RunStep = 210;
			}
			break;


		case 500:
			if( g_lotMgr.GetLotCount() > 0 )
			{
				mn_RunStep = 1000;
			}
			break;


		//////////////////////////////////////////////////////////////////////////
		//1. 작업영역이라서 UNPRESS 한다.
		//2. 한칸 미는 동작을 수행한다.
		//////////////////////////////////////////////////////////////////////////
		case 1000:
			//작업영역이라서 UNPRESS 한다.
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_REQ && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK )
			{
				if( Func.Check_RunAllSafety() == RET_GOOD )
				{
					if( Chk_PressClamp_Safety(0) == RET_GOOD )
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
					if( Chk_PressClamp_Safety(0) == RET_GOOD )
					{
						st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] = CTL_READY;
						mn_RunStep = 3000;
					}
				}
			}
			break;

		case 1100:
			if( st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][0] == CTL_CHANGE && st_sync.nCarrierRbt_Dvc_Req[THD_UNPRESS_RBT][1] == WORK_PICK )
			{
				st_sync.nCarrierSateyflag[UNPRESS_Y] = RET_ERROR;
				m_nTrayDeviceGap = 0;
				mn_RunStep = 2000;
			}
			break;

		case 2000:
			//nMode = 0 전부 업하여 모터가 움직일때
			if( m_nTrayDeviceGap == 0 && Chk_PressClamp_Safety(0) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				break;
			}
			else
			{//nMode = 3 Holder down상태로 움직일때
				if( Chk_PressClamp_Safety(3) == RET_ERROR )
				{
					COMI.Set_MotStop( 0 , m_nPressAxisX);
					COMI.Set_MotStop( 0 , m_nPressAxisY);
					break;
				}
			}


			if( m_nTrayDeviceGap > 0 && ( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nPressAxisX].mn_allow ) ||
				mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nPressAxisX].mn_allow ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			if( m_nTrayDeviceGap == 0 && ( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			if( m_nTrayDeviceGap > 0 )
			{
				if( ( mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nPressAxisX].mn_allow ) ) &&
					( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nPressAxisX].mn_allow ) ) )
				{
					mn_RunStep = 2100;
					break;
				}
			}		

			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS];
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_START, m_nPressAxisX, m_dTargetPosX, COMI.mn_runspeed_rate);
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
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				mn_RunStep = 2000;
			}
			break;

		case 2010:
			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS];
			nRet_1 = CTL_Lib.Single_Move(ONLY_MOVE_CHECK, m_nPressAxisX, m_dTargetPosX, COMI.mn_runspeed_rate);
			if (nRet_1 == BD_GOOD) //좌측으로 이동
			{
				mn_RunStep = 2100;
			}
			else if (nRet_1 == BD_RETRY)
			{
				mn_RunStep = 2000;
			}
			else if (nRet_1 == BD_ERROR || nRet_1 == BD_SAFETY)
			{//모터 알람은 이미 처리했으니 이곳에서는 런 상태만 바꾸면 된다
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				mn_RunStep = 2000;
			}
			break;

		//UNPRESS 실린더 다운시켜서 움직인다.
		case 2100:
			if( Chk_PressClamp_Safety(0) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			if( m_nTrayDeviceGap == 0 && ( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			Set_Device_Carrier_Holder(ON);
			Set_UnPress_PushClamp_OnOff(0, OFF);
			Set_UnPress_PushClamp_OnOff(1, OFF);
			mn_RunStep = 2200;
			break;

		case 2200:
			nRet[0] = g_ioMgr.get_in_bit( st_io.i_Press_Up_Check, IO_ON );
			nRet[1] = g_ioMgr.get_in_bit( st_io.i_Press_Down_Check, IO_OFF );
			nRet[2] = g_ioMgr.get_out_bit( st_io.o_Press_Up_Sol, IO_ON );
			nRet[3] = g_ioMgr.get_out_bit( st_io.o_Press_Down_Sol, IO_OFF );
			if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_OFF)
			{
				nRet_1 = Chk_Device_Carrier_Holder(ON);
				if(nRet_1 == RET_GOOD)
				{
					mn_RunStep = 2300;
				}
				else if(nRet_1 == RET_ERROR)
				{
					CTL_Lib.Alarm_Error_Occurrence(1101, dWARNING, m_strAlarmCode);
					mn_RunStep = 2200;
				}
			}
			else
			{
				if( nRet[0] != IO_ON || nRet[2] != IO_ON)
				{
					strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
					CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
				}
				else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
				{
					strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
					CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
				}
			}
			break;
			
		case 2300:
			//UNPRESS 실린더가 다운상태에서 움직여야 한다.
			if( Chk_PressClamp_Safety(3) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			if( m_nTrayDeviceGap >= 0 && ( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nPressAxisX].mn_allow ) ||
				mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nPressAxisX].mn_allow ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
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
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
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

			if( m_nTrayDeviceGap >= 0 && ( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nPressAxisX].mn_allow ) ||
				mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nPressAxisX].mn_allow ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
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
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				mn_RunStep = 2300;
			}
			break;

			//Site 하나씩 UNPRESS 한다.
		case 2320:
			Set_Device_Carrier_Holder(OFF);
			Set_UnPress_PushClamp_OnOff(0, OFF);
			Set_UnPress_PushClamp_OnOff(1, OFF);
			mn_RunStep = 2330;
			break;

		case 2330:
			if( Chk_Device_Carrier_Holder(OFF) == RET_GOOD)
			{
				mn_RunStep = 2400;
			}	
			break;

			//nMode = 1 Holder 상태에서  Press2로 움직인다.
		case 2400:
			if( Chk_PressClamp_Safety(1) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				break;
			}
			if( ( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nPressAxisX].mn_allow ) ) ||
				( mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nPressAxisX].mn_allow ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
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
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				mn_RunStep = 2400;
			}
			break;	

			//Press 이동 후 Holder 실린더를 내린다.
		case 2410:
			Set_Device_Carrier_Holder(ON);
			Set_UnPress_PushClamp_OnOff(0, OFF);
			Set_UnPress_PushClamp_OnOff(1, OFF);
			mn_RunStep = 2420;
			break;

		case 2420:
			nRet[0] = g_ioMgr.get_in_bit( st_io.i_Press_Up_Check, IO_ON );
			nRet[1] = g_ioMgr.get_in_bit( st_io.i_Press_Down_Check, IO_OFF );
			nRet[2] = g_ioMgr.get_out_bit( st_io.o_Press_Up_Sol, IO_ON );
			nRet[3] = g_ioMgr.get_out_bit( st_io.o_Press_Down_Sol, IO_OFF );
			if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_OFF)
			{
				nRet_1 = Chk_Device_Carrier_Holder(ON);
				if(nRet_1 == RET_GOOD)
				{
					mn_RunStep = 2500;
				}
				else if(nRet_1 == RET_ERROR)
				{
					CTL_Lib.Alarm_Error_Occurrence(1101, dWARNING, m_strAlarmCode);
					mn_RunStep = 2420;
				}
			}
			else
			{
				if( nRet[0] != IO_ON || nRet[2] != IO_ON)
				{
					strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
					CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
				}
				else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
				{
					strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
					CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
				}
			}
			break;

		case 2500:
			m_nTrayDeviceGap++;
			if( m_nTrayDeviceGap >= 3 )
			{// 전부 UNPRESS를 했으므로 원래 상태로돌린다
				m_nTrayDeviceGap = 0;
				mn_RunStep = 2600;
			}
			else
			{
				mn_RunStep = 2000;
			}
			break;

		case 2600:
			if( Chk_PressClamp_Safety(3) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			if( ( mCurrPosX > ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] + st_motor[m_nPressAxisX].mn_allow ) ) ||
				( mCurrPosX < ( st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS] - st_motor[m_nPressAxisX].mn_allow ) ) )
			{
				//070008 1 0 "M_CARRIER_X_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "070008");
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
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				mn_RunStep = 2600;
			}
			break;
		
		case 2610:
			Set_Device_Carrier_Holder(OFF);
			Set_UnPress_PushClamp_OnOff(0, OFF);
			Set_UnPress_PushClamp_OnOff(1, OFF);
			mn_RunStep = 2620;
			break;

		case 2620:
			if( Chk_Device_Carrier_Holder(OFF) == RET_GOOD && Chk_UnPress_PushClamp_OnOff(0, OFF ) == RET_GOOD )
			{
				mn_RunStep = 2700;
			}	
			break;

		case 2700:
			if( mCurrPosY > ( st_motor[m_nPressAxisX].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisX].mn_allow )  )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}
			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_INIT_POS];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nPressAxisX, m_dTargetPosX, COMI.mn_runspeed_rate);
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
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
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
			if( Chk_PressClamp_Safety(0) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}	

			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_UNPRESS_POS];
			nRet_1 = CTL_Lib.Single_Move( BOTH_MOVE_FINISH, m_nPressAxisX, m_dTargetPosX, COMI.mn_runspeed_rate);
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
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				mn_RunStep = 3100;
			}
			break;

		case 3110:
			if( Chk_PressClamp_Safety(0) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			if( m_nTrayDeviceGap == 0 && ( mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS".
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			Run_Device_Carrier_Robot.Set_Device_Carrier_HolderPin_Fix(0, OFF);
			Set_Device_Carrier_Holder( OFF);//UNPRESS
			Set_UnPress_PushClamp_OnOff(0, ON);
			Set_UnPress_PushClamp_OnOff(1, OFF);
			mn_RunStep = 3120;
			break;


		case 3120:
			if( Chk_Device_Carrier_Holder(OFF) == RET_GOOD &&  
				Chk_UnPress_PushClamp_OnOff( 0, ON ) == RET_GOOD && Chk_UnPress_PushClamp_OnOff( 1, OFF ) == RET_GOOD )
			{
				mn_RunStep = 3200;
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

			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
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
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				mn_RunStep = 3200;
			}
			break;

		case 3300:
			m_dwWaitUntil[1] = GetCurrentTime();
			m_dwWaitUntil[2] = m_dwWaitUntil[1] - m_dwWaitUntil[0];
			if( m_dwWaitUntil[2] <= 0 ) m_dwWaitUntil[0] = GetCurrentTime();
			if( m_dwWaitUntil[2] > 1000 )
			{
				mn_RunStep = 3310; //Carrier X를 조금 두;로 뺀다. 실린더를 업하기 위해
			}
			break;

		case 3310://실린더 고정
			Set_Device_Carrier_Holder(ON);
			mn_RunStep = 3320;
			break;

		case 3320:
			if( Chk_Device_Carrier_Holder(ON) == RET_GOOD)
			{
				mn_RunStep = 3400;
			}	
			break;

		case 3400:
			//PRESS 실린더가 다운상태에서 움직여야 한다.
			if( Chk_PressClamp_Safety(2) == RET_ERROR )
			{
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
				break;
			}

			m_dTargetPosX = st_motor[m_nPressAxisX].md_pos[P_CARRIER_X_PRESS_POS];
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
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
				mn_RunStep = 3400;
			}
			break;

		case 3410:
			Set_Device_Carrier_Holder(ON);
			Set_UnPress_PushClamp_OnOff( 0 ,OFF);
			Set_UnPress_PushClamp_OnOff( 1 ,OFF);
			mn_RunStep = 3420;
			break;

		case 3420:
			if( Chk_Device_Carrier_Holder(ON) == RET_GOOD &&  
				Chk_UnPress_PushClamp_OnOff( 0, OFF ) == RET_GOOD && Chk_UnPress_PushClamp_OnOff( 1, OFF ) == RET_GOOD )
			{
				mn_RunStep = 3500;
			}	
			break;

		case 3500:
			//PRESS 실린더가 다운상태에서 움직여야 한다.
			if(  mCurrPosY > ( st_motor[m_nPressAxisY].md_pos[P_PRESS_Y_INIT_POS] + st_motor[m_nPressAxisY].mn_allow ) )
			{
				//020008 1 0 "M_PRESS_Y_MOTOR_IS_NON_SAFETY_POS."
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, "020008");
				COMI.Set_MotStop( 0 , m_nPressAxisX);
				COMI.Set_MotStop( 0 , m_nPressAxisY);
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
				CTL_Lib.Alarm_Error_Occurrence(1103, dWARNING, alarm.mstr_code);
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
//nMode = 1 Holder 상태에서 움직일때
//nMode = 2 Press down상태로 밀때
//nMode = 3 Holder down상태로 움직일때

int CRun_UnPress_Robot::Chk_PressClamp_Safety( int nMode)
{
	int nRet[9] = {0,};
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
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			else if( nRet[4] != IO_ON || nRet[6] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);					
			}
			else if( nRet[8] != IO_OFF )
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_UpDown_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);					
			}
			else//nRet[6] != IO_ON  nRet[7] != IO_OFF
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			nFuncRet = RET_ERROR;
		}
	}//nMode = 1 Holder 상태에서 움직일때
	else if( nMode == 1)
	{
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_OFF && 
			nRet[4] == IO_ON && nRet[5] == IO_OFF && nRet[6] == IO_ON && nRet[7] == IO_OFF && nRet[8] == IO_ON )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			if( nRet[0] != IO_ON || nRet[2] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			else if( nRet[4] != IO_ON || nRet[6] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);					
			}
			else if( nRet[8] != IO_ON )
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);					
			}
			else//nRet[6] != IO_ON  nRet[7] != IO_OFF
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			nFuncRet = RET_ERROR;
		}
	}
	else if( nMode == 2 )// Press down상태로 밀때
	{
		if( nRet[0] == IO_OFF && nRet[1] == IO_ON && nRet[2] == IO_OFF && nRet[3] == IO_ON && 
			nRet[4] == IO_ON && nRet[5] == IO_OFF && nRet[6] == IO_ON && nRet[7] == IO_OFF && nRet[8] == IO_OFF )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			if( nRet[0] != IO_OFF || nRet[2] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			else if( nRet[1] != IO_ON || nRet[3] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			else if( nRet[4] != IO_ON || nRet[6] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);					
			}
			else if( nRet[8] != IO_OFF )
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);					
			}
			else//nRet[6] != IO_ON  nRet[7] != IO_OFF
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			nFuncRet = RET_ERROR;
		}
	}
	else if( nMode == 3)
	{
		if( nRet[0] == IO_ON && nRet[1] == IO_OFF && nRet[2] == IO_ON && nRet[3] == IO_OFF && 
			nRet[4] == IO_OFF && nRet[5] == IO_ON && nRet[6] == IO_OFF && nRet[7] == IO_ON && nRet[8] == IO_OFF )
		{
			nFuncRet = RET_GOOD;
		}
		else
		{
			if( nRet[0] != IO_ON || nRet[2] != IO_ON)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			else if( nRet[1] != IO_OFF || nRet[3] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			else if( nRet[4] != IO_OFF || nRet[6] != IO_OFF)
			{
				strTemp.Format("8%d%04d", IO_OFF, st_io.i_Press_Carrier_Holder_Up_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);					
			}
			else if( nRet[8] != IO_OFF )
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_UpDown_CellIn_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);					
			}
			else//nRet[6] != IO_ON  nRet[7] != IO_OFF
			{
				strTemp.Format("8%d%04d", IO_ON, st_io.i_Press_Carrier_Holder_Down_Check);
				CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, strTemp);
			}
			nFuncRet = RET_ERROR;
		}
	}
	else
	{
		//"901000 1 A "THERE_IS_NO_MODE_MOVING_PRESSRBT_IN_SAFETY_CLAMP."
		CTL_Lib.Alarm_Error_Occurrence(1236, dWARNING, "901000");
		nFuncRet = RET_ERROR;
	}

	return nFuncRet;
}


int CRun_UnPress_Robot::Chk_PressMotor_Safety()
{
	int nFuncRet = RET_GOOD;
	double dInterval = 10.0;

	int nRet_1 = g_ioMgr.get_in_bit(st_io.i_Press_UpDown_CellIn_Check, IO_OFF);
	if( nRet_1 == IO_ON && ( COMI.Get_MotCurrentPos(M_PRESS_Y) >= st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS] - ( dInterval * st_motor[M_PRESS_Y].mn_allow ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS] + ( dInterval * st_motor[M_PRESS_Y].mn_allow ) ) &&
		( COMI.Get_MotCurrentPos(M_PRESS_Y) >= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS]+st_recipe.dLoaderTransferTrayDeviceGap) - ( dInterval * st_motor[M_PRESS_Y].mn_allow ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS]+st_recipe.dLoaderTransferTrayDeviceGap) + ( dInterval * st_motor[M_PRESS_Y].mn_allow ) ) &&
		( COMI.Get_MotCurrentPos(M_PRESS_Y) >= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS]+2*st_recipe.dLoaderTransferTrayDeviceGap) - ( dInterval * st_motor[M_PRESS_Y].mn_allow ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS1_POS]+2*st_recipe.dLoaderTransferTrayDeviceGap) + ( dInterval * st_motor[M_PRESS_Y].mn_allow ) ) )
	{
		nFuncRet = RET_ERROR;
	}

	if( nRet_1 == IO_ON && ( COMI.Get_MotCurrentPos(M_PRESS_Y) >= st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS] - ( dInterval * st_motor[M_PRESS_Y].mn_allow ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS] + ( dInterval * st_motor[M_PRESS_Y].mn_allow ) ) &&
		( COMI.Get_MotCurrentPos(M_PRESS_Y) >= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS]+st_recipe.dLoaderTransferTrayDeviceGap) - ( dInterval * st_motor[M_PRESS_Y].mn_allow ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS]+st_recipe.dLoaderTransferTrayDeviceGap) + ( dInterval * st_motor[M_PRESS_Y].mn_allow ) ) &&
		( COMI.Get_MotCurrentPos(M_PRESS_Y) >= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS]+2*st_recipe.dLoaderTransferTrayDeviceGap) - ( dInterval * st_motor[M_PRESS_Y].mn_allow ) &&
		COMI.Get_MotCurrentPos(M_PRESS_Y) <= (st_motor[M_PRESS_Y].md_pos[P_PRESS_Y_PRESS2_POS]+2*st_recipe.dLoaderTransferTrayDeviceGap) + ( dInterval * st_motor[M_PRESS_Y].mn_allow ) ) )
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
	g_ioMgr.Set_Out_Bit(st_io.o_Press_Carrier_Holder_Down_Sol, OnOff);
	g_ioMgr.Set_Out_Bit(st_io.o_Press_Carrier_Holder_Up_Sol, !OnOff);



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



	if (nOnOff == IO_OFF)//DOWN BWD
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
		if (m_bCarrierHolder == false && nRet_2 == IO_ON && nRet_1 == IO_OFF )
		{
			m_bCarrierHolder = true;
			m_dwCarrierHolder[0]	= GetCurrentTime();
		}
		else if (m_bCarrierHolder == true && nRet_2 == IO_ON && nRet_1 == IO_OFF )
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
		g_ioMgr.set_out_bit( st_io.o_Slide_Guide_X2_Forward_Sol, OnOff);
		g_ioMgr.set_out_bit( st_io.o_Slide_Guide_X2_Backward_Sol, !OnOff);
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
		nRet1 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Up_Check, OnOff);
		nRet2 = g_ioMgr.get_in_bit(st_io.i_Slide_Guide_X2_Down_Check, !OnOff);
	}
	else
	{
		nRet1 = g_ioMgr.get_in_bit(st_io.i_Press_Up_Check, OnOff);
		nRet2 = g_ioMgr.get_in_bit(st_io.i_Press_Down_Check, !OnOff);
	}

	if (OnOff == IO_OFF)
	{
		if (m_bPressUpDnFlag == false && nRet1 ==  IO_ON && nRet2 == IO_OFF )
		{
			m_bPressUpDnFlag[nMode]		= true;
			m_dwPressUpDn[nMode][0]	= GetCurrentTime();
		}
		else if (m_bPressUpDnFlag[nMode] == true && nRet1 ==  IO_ON && nRet2 == IO_OFF  )
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
		if (m_bPressUpDnFlag[nMode] == false && nRet1 ==  IO_OFF && nRet2 == IO_ON  )
		{
			m_bPressUpDnFlag[nMode]			= true;
			m_dwPressUpDn[nMode][0]	= GetCurrentTime();
		}
		else if (m_bPressUpDnFlag[nMode] == true && nRet1 ==  IO_OFF && nRet2 == IO_ON )
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