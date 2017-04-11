// AMotor.cpp: implementation of the AMotor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "handler.h"
#include "AMotor.h"
#include "../ComizoaPublic.h"

#include "..\Cmmsdk.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AMotor::AMotor()
{
	m_step = MMSTEP_NONE;
	m_bStopReq = false;
	m_MotMoveMode = MMMODE_NONE;

	m_dMotGotoPos = 0;
	m_dMotGotoSpeed = 1;

	m_nMotorId = -1;

	mn_linearmove_index = 2;					//현재 map index 
	ml_AxisCnt = 2;								//현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개  
	mlp_AxisNum[0] = M_HEATSINK_TRANSFER_X;			//현재의 IndexNum에서 사용하는 모터 실제 번호를 가진다 
	mlp_AxisNum[1] = M_HEATSINK_TRANSFER_Y;  

	COMI.ml_axiscnt[mn_linearmove_index] = 2;
	COMI.mp_axisnum[mn_linearmove_index][0] = M_HEATSINK_TRANSFER_X;
	COMI.mp_axisnum[mn_linearmove_index][1] = M_HEATSINK_TRANSFER_Y;

/*
	mn_linearmove_index = 1;					//현재 map index 
	ml_AxisCnt = 2;								//현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개  
	mlp_AxisNum[0] = M_REAR_SHIFTER_Y;			//현재의 IndexNum에서 사용하는 모터 실제 번호를 가진다 
	mlp_AxisNum[1] = M_REAR_SHIFTER_X;  
	
	COMI.ml_axiscnt[mn_linearmove_index] = 2;
	COMI.mp_axisnum[mn_linearmove_index][0] = M_REAR_SHIFTER_Y;
	COMI.mp_axisnum[mn_linearmove_index][1] = M_REAR_SHIFTER_X;

*/



		
	mdp_PosList[0] = 0;				//최대 4 포인트를 한번에 이동가능하다 
	mdp_PosList[1] = 0;
	mdp_SpdRatio[0] = 100;        //속도비율 [0]:Vel, [1]:Acc, [2]:Dec
	mdp_SpdRatio[1] = 100;
	mdp_SpdRatio[2] = 100;

	mstr_error_msg = "";
}

AMotor::~AMotor()
{

}

EMOTRET AMotor::MotorMoving_repeat()
{
	EMOTRET ret = MOTRET_PROCEED;

	if( m_bStopReq )
	{
		if( m_step < MMSTEP_STOP )
		{
			m_step = MMSTEP_STOP;
		}
	}

	switch( m_step )
	{
	case MMSTEP_NONE:														break;

	case MMSTEP_MOVE_START:		MmStep_Move_Start();						break;
	case MMSTEP_MOVE_CHECK:		MmStep_Move_Check();						break;
	case MMSTEP_MOVE_FINISH:	MmStep_Move_Finish();	ret = MOTRET_GOOD;	break;

	case MMSTEP_HOME_HOMECHECK:	ret = MmStep_Home_HomeCheck();				break;
	case MMSTEP_STOP:			MmStep_Stop();								break;
	case MMSTEP_STOP_FINISH:	MmStep_Stop_Finish();	ret = MOTRET_GOOD;	break;
	case MMSTEP_MOVE_FAIL:		MmStep_Move_Fail();		ret = MOTRET_ERROR;	break;
	case MMSTEP_STOP_FAIL:		MmStep_Stop_Fail();		ret = MOTRET_ERROR;	break;
	}

	return ret;
}

EMOTRET AMotor::MotorLMoving_repeat()
{
	EMOTRET ret = MOTRET_PROCEED;
	
	if( m_bStopReq )
	{
		if( m_step < MMSTEP_STOP )
		{
			m_step = MMSTEP_STOP;
		}
	}
	
	switch( m_step )
	{
	case MMSTEP_NONE:														break;
		
	case MMSTEP_LMOVE_START:	ret = Linear_Move();						break;

	case MMSTEP_LMOVE_CHECK:	MmStep_LMove_Check();						break;
	case MMSTEP_LMOVE_FINISH:	MmStep_LMove_Finish();	ret = MOTRET_GOOD;	break;

	case MMSTEP_STOP:			MmStep_Stop();								break;
	case MMSTEP_STOP_FINISH:	MmStep_Stop_Finish();	ret = MOTRET_GOOD;	break;
	case MMSTEP_MOVE_FAIL:		MmStep_Move_Fail();		ret = MOTRET_ERROR;	break;
	case MMSTEP_STOP_FAIL:		MmStep_Stop_Fail();		ret = MOTRET_ERROR;	break;
	}
	
	return ret;
}

void AMotor::MmStep_Move_Start()
{
	int nRet = COMI.Start_SingleMove( m_nMotorId, m_dMotGotoPos, (int)m_dMotGotoSpeed );
	if( nRet == MOTRET_GOOD )
	{
		m_step = MMSTEP_MOVE_CHECK;
	}
	else if( nRet == MOTRET_ERROR )
	{
		// "[" + m_str_cmd_motor + "] Motor failed to move."
		m_step = MMSTEP_MOVE_FAIL;
	}
	else if( nRet == MOTRET_SAFETY )
	{
		mstr_error_msg = "[SAFETY] Do not move being thought that is not safe at relevant motor difference.";
		m_step = MMSTEP_MOVE_FAIL;
	}
}

void AMotor::MmStep_Move_Check()
{
	int nRet = COMI.Check_SingleMove( m_nMotorId, m_dMotGotoPos );
	if( nRet == MOTRET_GOOD )
	{
		m_step = MMSTEP_MOVE_FINISH;
	}
	else if( nRet == MOTRET_ERROR )
	{
		// "[" + m_str_cmd_motor + "] Motor failed to move."
		m_step = MMSTEP_MOVE_FAIL;
	}
	else if( nRet == MOTRET_SAFETY )
	{
		mstr_error_msg = "[SAFETY] Do not move being thought that is not safe at relevant motor difference.";
		m_step = MMSTEP_MOVE_FAIL;
	}
}

void AMotor::MmStep_Move_Finish()
{
	// "[" + m_str_cmd_motor + "] Motor completed transfer."
	_repeat_Finish();
	_finish_manual();
}

EMOTRET AMotor::MmStep_Home_HomeCheck()
{
	EMOTRET ret = MOTRET_PROCEED;
	int nRet = g_comiMgr.HomeCheck_Mot( m_nMotorId, st_motor[m_nMotorId].mn_homecheck_method, MOT_TIMEOUT);

	if( nRet == MOTRET_GOOD )
	{
		_repeat_Finish();
		_finish_manual();
		ret = MOTRET_GOOD;
	}
	else if( nRet == MOTRET_ERROR )
	{
		_repeat_Finish();
		_finish_manual();
		ret = MOTRET_ERROR;
	}
	else if( nRet == MOTRET_SAFETY )
	{
		// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
		m_step = MMSTEP_NONE;
		_finish_manual();
		ret = MOTRET_ERROR;
	}

	return ret;
}
	
void AMotor::MmStep_Stop()
{
	int nRet = COMI.Set_MotStop( MOTSTOP_SLOWDOWN, m_nMotorId );
	if( nRet == MOTRET_GOOD )
	{
		m_step = MMSTEP_STOP_FINISH;
	}
	else
	{
		// "[" + m_str_cmd_motor + "] Though canceled transfer of motor, error happened."
		m_step = MMSTEP_STOP_FAIL;
	}
}

void AMotor::MmStep_Stop_Finish()
{
	// "[" + m_str_cmd_motor + "] Motor shut down during transfer."
	_repeat_Finish();
	_finish_manual();
}

void AMotor::MmStep_Move_Fail()
{
	// "[" + m_str_cmd_motor + "] Transfer command of motor failed."
	sprintf(st_msg.c_abnormal_msg, mstr_error_msg);
	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);  // 동작 실패 출력 요청

	_repeat_Finish();
	_finish_manual();
}
	
void AMotor::MmStep_Stop_Fail()
{
	// "[" + m_str_cmd_motor + "] Transfer command of motor failed."
	_repeat_Finish();
	_finish_manual();
}

void AMotor::_repeat_Finish()
{
	if( m_bStopReq )
	{
		m_bStopReq = false;
	}

	m_step = MMSTEP_NONE;
}

void AMotor::_finish_manual()
{
	if( m_MotMoveMode == MMMODE_MANUAL ||
		m_MotMoveMode == MMMODE_HOME )
	{
		if( st_handler.cwnd_motorAxis )
		{
			st_handler.cwnd_motorAxis->PostMessage( WM_MOTOR_COMPLETION, m_MotMoveMode, 0 );
		}

//		if( st_handler.cwnd_visionteaching )
//		{
//			st_handler.cwnd_visionteaching->PostMessage( WM_MOTOR_COMPLETION, m_MotMoveMode, 0 );
//		}

		m_MotMoveMode = MMMODE_NONE;
	}
}

void AMotor::MotorStepClear()
{
	m_bStopReq = false;
	m_step = MMSTEP_NONE;
}

void AMotor::MotorMoveStart( double dPos, double dSpeed, EMOTMOVE_MODE motMoveMode )
{
	m_dMotGotoPos = dPos;
	m_dMotGotoSpeed = dSpeed;
	m_MotMoveMode = motMoveMode;

	m_step = MMSTEP_MOVE_START;
}

void AMotor::MotorHomeStart()
{
	m_MotMoveMode = MMMODE_HOME;
	COMI.mn_home_step[m_nMotorId] = 0;
	m_step = MMSTEP_HOME_HOMECHECK;
}

EMOTRET AMotor::JogMoveStart( long lDir )
{
	COMI.Set_MotSpeed(MOT_SPD_JOG, m_nMotorId, cmSMODE_KEEP, COMI.md_spd_jog[m_nMotorId], COMI.md_spd_vel[m_nMotorId][1], COMI.md_spd_vel[m_nMotorId][2]);		// 축의 속도를 변경한다.
			
	if (cmmSxVMoveStart(m_nMotorId, lDir) != cmERR_NONE)
	{ 
		return MOTRET_ERROR;
	}
	
	return MOTRET_GOOD;
}

void AMotor::MotorLMoveStart(double dPos1, double dPos2, double dSpeed, EMOTMOVE_MODE motMoveMode)
{
	m_dMotGotoPosLinear[0] = dPos1;
	m_dMotGotoPosLinear[1] = dPos2;
	m_dMotGotoSpeed = dSpeed;
	m_MotMoveMode = motMoveMode;
	
	m_step = MMSTEP_LMOVE_START;
}

void AMotor::MmStep_LMove_Start()
{
	m_dMotGotoSpeedRate[0] = m_dMotGotoSpeed;
	m_dMotGotoSpeedRate[1] = m_dMotGotoSpeed;
	m_dMotGotoSpeedRate[2] = m_dMotGotoSpeed;

	int nRet = COMI.Start_LinearMove(m_nMotorMapId, m_dMotGotoPosLinear, m_dMotGotoSpeedRate);

	if( nRet == MOTRET_GOOD )
	{
		m_step = MMSTEP_LMOVE_CHECK;
	}
	else if( nRet == MOTRET_ERROR )
	{
		// "[" + m_str_cmd_motor + "] Motor failed to move."
		m_step = MMSTEP_MOVE_FAIL;
	}
	else if( nRet == MOTRET_SAFETY )
	{
		// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
		m_step = MMSTEP_MOVE_FAIL;
	}
}

void AMotor::MmStep_LMove_Check()
{
	int nRet = COMI.Check_LinearMove( m_nMotorMapId, m_dMotGotoPosLinear );
	if( nRet == MOTRET_GOOD )
	{
		m_step = MMSTEP_LMOVE_FINISH;
	}
	else if( nRet == MOTRET_ERROR )
	{
		// "[" + m_str_cmd_motor + "] Motor failed to move."
		m_step = MMSTEP_MOVE_FAIL;
	}
	else if( nRet == MOTRET_SAFETY )
	{
		// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
		m_step = MMSTEP_MOVE_FAIL;
	}
}

void AMotor::MmStep_LMove_Finish()
{
	_repeat_Finish();
	_finish_manual();
}

EMOTRET AMotor::Linear_Move()
{
	m_dMotGotoSpeedRate[0] = m_dMotGotoSpeed;
	m_dMotGotoSpeedRate[1] = m_dMotGotoSpeed;
	m_dMotGotoSpeedRate[2] = m_dMotGotoSpeed;

	EMOTRET ret = MOTRET_PROCEED;
	int nRet = CTL_Lib.Linear_Move( m_nMotorMapId, 2, mlp_AxisNum, m_dMotGotoPosLinear, m_dMotGotoSpeedRate);
	
	if( nRet == MOTRET_GOOD )
	{
		_repeat_Finish();
		_finish_manual();
		ret = MOTRET_GOOD;
	}
	else if( nRet == MOTRET_ERROR )
	{
		_repeat_Finish();
		_finish_manual();
		ret = MOTRET_ERROR;
	}
	else if( nRet == MOTRET_SAFETY )
	{
		// "[SAFETY] Do not move being thought that is not safe at relevant motor difference."
		m_step = MMSTEP_NONE;
		_finish_manual();
		ret = MOTRET_ERROR;
	}
	
	return ret;
}
