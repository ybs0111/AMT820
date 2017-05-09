// COMI_Manager.h: interface for the COMI_Manager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMI_MANAGER_H__E5BA7DF5_D6AB_4E96_88E6_6DFCC4FD4B18__INCLUDED_)
#define AFX_COMI_MANAGER_H__E5BA7DF5_D6AB_4E96_88E6_6DFCC4FD4B18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COMI_Manager  
{
public:
	COMI_Manager();
	virtual ~COMI_Manager();

	int HomeCheck_Mot(int n_Axis, int n_HomeMode, int n_TimeOut);
	int Get_MotPower(int n_Axis);
	double Get_MotCurrentPos(int n_Axis);
	
	long _cmmSxIsDone(long Axis, long *IsDone);
	long _cmmCfgSetSpeedPattern_T(long Axis, long SpeedMode, double WorkSpeed, double Accel, double Decel);
	long _cmmSxSetSpeedRatio(long Axis, long SpeedMode, double VelRatio, double AccRatio, double DecRatio);	// check
	long _cmmSxMoveToStart(long Axis, double Position);
	long _cmmMsCheckSlaveState(long SlaveAxis, long* SlaveState);

	long _cmmIxIsDone( long MapIndex, long *IsDone );
	long _cmmIxLineToStart(long MapIndex, double* PosList);
	
	

public:
	void OnMain_Motor_Setting();
	
public:
	void SetMotBoardInit( int nVal ) { m_nMotBoardInit = nVal; }
	int GetMotBoardInit()	{ return m_nMotBoardInit; }
protected:
	int m_nMotBoardInit;	
};

extern COMI_Manager g_comiMgr;

enum MOTOR_NUM
{
	M_TRAY1_Z				= 0,
	M_TRAY2_Z,				
	M_PRESS_Y,				
	M_EPOXY_TRANSFER_X,		
	M_EPOXY_TRANSFER_Y,		
	M_EPOXY_TRANSFER_Z,		
	M_EPOXY_SCREW,
	M_CARRIER_X,
	
	M_LOADER_TRANSFER_Y,
	M_LOADER_TRANSFER_Z,
	M_HEATSINK_TRANSFER_X,
	M_HEATSINK_TRANSFER_Y,
	M_HEATSINK_TRANSFER_Z,
	M_UNLOADER_TRANSFER_X,
	M_UNLOADER_TRANSFER_Y,
	M_UNLOADER_TRANSFER_Z,
	
	M_DISPENSER_Y,	
	M_TRAY_REMOVE_X,			
	M_HEATSINK_INSPECT_Y,
	
	M_HEATSINK_INSPECT_Z,
	M_HEATSINK_PICKER_PITCH,
	
	M_MOTOR_COUNT,
};

#endif // !defined(AFX_COMI_MANAGER_H__E5BA7DF5_D6AB_4E96_88E6_6DFCC4FD4B18__INCLUDED_)
