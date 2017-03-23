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
	
};

extern COMI_Manager g_comiMgr;

#endif // !defined(AFX_COMI_MANAGER_H__E5BA7DF5_D6AB_4E96_88E6_6DFCC4FD4B18__INCLUDED_)
