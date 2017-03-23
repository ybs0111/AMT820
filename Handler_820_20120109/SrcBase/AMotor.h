// AMotor.h: interface for the AMotor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMOTOR_H__D3DDFFAE_2163_4EDD_B5BE_DAD2BAFDBDA3__INCLUDED_)
#define AFX_AMOTOR_H__D3DDFFAE_2163_4EDD_B5BE_DAD2BAFDBDA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum EMOTSTOP					// enum_Motor_Stop
{
	MOTSTOP_SLOWDOWN,
	MOTSTOP_EMERGENCY,
};

enum EMOTMOVE_MODE				// enum_Motor_Move_Mode
{
	MMMODE_NONE,
	MMMODE_MANUAL,
	MMMODE_DISTANCE,
	MMMODE_HOME,
};

enum EMOTRET					// enum_Motor_Result
{
	MOTRET_ERROR = -1,
	MOTRET_PROCEED = 0,
	MOTRET_GOOD,
	MOTRET_RETRY,
	MOTRET_SAFETY,
	//MOTRET_ABORT,
	//MOT_RET_SKIP,
};

enum EMOTMOVE_STEP				//enum_Motor_Move_Step
{
	MMSTEP_NONE,

	// SINGLE MOVE
	MMSTEP_MOVE_START,
	MMSTEP_MOVE_CHECK,
	MMSTEP_MOVE_FINISH,

	// LINEAR MOVE
	MMSTEP_LMOVE_START,
	MMSTEP_LMOVE_CHECK,
	MMSTEP_LMOVE_FINISH,

	// HOME
	MMSTEP_HOME_HOMECHECK,

	// STOP
	MMSTEP_STOP,
	MMSTEP_STOP_FINISH,
	
	// FAIL
	MMSTEP_MOVE_FAIL,
	MMSTEP_STOP_FAIL,
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class AMotor  
{
public:
	AMotor();
	virtual ~AMotor();

protected:
	void MmStep_Move_Start();
	void MmStep_Move_Check();
	void MmStep_Move_Finish();
	EMOTRET MmStep_Home_HomeCheck();
	void MmStep_Stop();
	void MmStep_Stop_Finish();
	void MmStep_Move_Fail();
	void MmStep_Stop_Fail();

	void MmStep_LMove_Finish();
	void MmStep_LMove_Check();
	void MmStep_LMove_Start();

	void _repeat_Finish();
	void _finish_manual();

public:
	EMOTRET Linear_Move();
	void MotorLMoveStart(double dPos1, double dPos2, double dSpeed, EMOTMOVE_MODE motMoveMode = MMMODE_NONE );
	void SetMotorID( int iId )			{ m_nMotorId = iId; }
	void SetMotorMapID( int iMapId )	{ m_nMotorMapId = iMapId; }
	EMOTRET MotorMoving_repeat();
	EMOTRET MotorLMoving_repeat();

	void SetMotGotoPos( double dVal ) { m_dMotGotoPos = dVal; }
	void MotorStepClear();
	void MotorMoveStart( double dPos, double dSpeed, EMOTMOVE_MODE motMoveMode = MMMODE_NONE );
	void MotorHomeStart();

	void SetStopReq( bool bVal ) { m_bStopReq = bVal; }

	EMOTRET JogMoveStart( long lDir );

	int		 mn_linearmove_index;   //현재 map index 
	long     ml_AxisCnt;            //현재의 IndexNum에서 사용 가능한 모터 수량 최대 4개  
	long	 mlp_AxisNum[4];        //현재의 IndexNum에서 사용하는 모터 실재 번호를 가진다 
	double	 mdp_PosList[4];		  //최대 4 포인트를 한번에 이동가능하다 
 	double   mdp_SpdRatio[3];        //속도비율 [0]:Vel, [1]:Acc, [2]:Dec
	CString	 mstr_error_msg;

protected:
	EMOTMOVE_STEP	m_step;

	bool			m_bStopReq;

	int				m_nMotorId;
	int				m_nMotorMapId;

	double			m_dMotGotoPos;
	double			m_dMotGotoPosLinear[4];
	double			m_dMotGotoSpeed;
	double			m_dMotGotoSpeedRate[3];
	
	EMOTMOVE_MODE	m_MotMoveMode;
};

#endif // !defined(AFX_AMOTOR_H__D3DDFFAE_2163_4EDD_B5BE_DAD2BAFDBDA3__INCLUDED_)
