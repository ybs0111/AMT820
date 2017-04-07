// APartHandler.h: interface for the APartHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_)
#define AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ENUM_MACHINE_POSITION
{
	EMPOS_FRONT,
	EMPOS_REAR,
	EMPOS_ALONE,
};

enum ESTEP_CONV
{
	EC_NONE,

	EC_RECEIVE,
	EC_RECEIVE_CHECK,
	
	EC_UP,
	EC_UP_CHK,

	EC_DOWN,
	EC_DOWN_CHK,

	EC_SEND,
	EC_SEND_CHK,
};

class APartHandler  
{
public:
	APartHandler();
	virtual ~APartHandler();

	void Thread_Run();
	void Run_Move_Main();
	void Run_Move_SMEMA();
	void Run_Move_Conv(int nIdx);
	//////////////////////////////////////////////////////////////////////////
	//int mn_run_status;  // 장비 동작 상태 정보 저장 변수 (O:STOP   1:RUN    2:ALARM    3:MAINT    4:INIT)

	void SetMachinePos( ENUM_MACHINE_POSITION empos ) { m_empos = empos; }
	ENUM_MACHINE_POSITION GetMachinePos() { return m_empos; }



protected:
	void ControlConv( int iIdx, ESTEP_CONV ec );

protected:
	ENUM_MACHINE_POSITION m_empos;	

	ESTEP_CONV				m_step_conv[3];
};

extern APartHandler	g_handler;

#endif // !defined(AFX_APARTHANDLER_H__041B081B_17B1_4D42_AE2D_A5047A8C3B7E__INCLUDED_)
