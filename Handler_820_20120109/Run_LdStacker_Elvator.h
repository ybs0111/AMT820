#if !defined(AFX_RUN_LDSTACKER_ELVATOR_H__81BB22BF_305F_4EEE_84AE_8944814F52EA__INCLUDED_)
#define AFX_RUN_LDSTACKER_ELVATOR_H__81BB22BF_305F_4EEE_84AE_8944814F52EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_LdStacker_Elvator.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_LdStacker_Elvator command target

class CRun_LdStacker_Elvator : public CObject
{
public:
	DECLARE_SERIAL(CRun_LdStacker_Elvator);

	CRun_LdStacker_Elvator();           // protected constructor used by dynamic creation
	virtual ~CRun_LdStacker_Elvator();

// Attributes
public:

// Operations
public:
	int mn_RunStep;
	int mn_reinstate_step;

	int m_nAxisNum;
	
	int mn_retry_cnt;
	long ml_retry_wait[3];
	
	long ml_wait_time[3];
	long ml_stop_wait[3];

	long ml_clamp_wait_time[3];
	int mn_clamp_retry_cnt;
	
	int mn_InitStep;
	
	char cJamcode[10];

	bool m_bInitVarBackward;

	double m_dcurr_pos[3];
	int	mn_reinstate_pos[3];

	long m_dwWaitTime[3];

	int m_nFindLotNo_Flag;	
	CString m_strLotNo;
	CString m_strPartNo;
	CString m_strAlarmCode;

	bool m_bClampOnOffFlag;
	DWORD m_dwClampOnOff[3];
	
public:

	void Thread_Run();
	void RunMove();
	void RunInit();

	int Ready_Stacker_Move_Check( int mode );
	void Set_Tray_Guide_Clamp_ForBackward(int OnOff);
	int Chk_Tray_Guide_Clamp_ForBackward(int OnOff);



// Implementation
protected:


};
extern CRun_LdStacker_Elvator Run_LdStacker_Elvator;

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RUN_LDSTACKER_ELVATOR_H__81BB22BF_305F_4EEE_84AE_8944814F52EA__INCLUDED_)

