#if !defined(AFX_RUN_EMPTYSTACKER_ELVATOR_H__B19C6D03_DBEF_49B4_94B5_65A158ADD2B5__INCLUDED_)
#define AFX_RUN_EMPTYSTACKER_ELVATOR_H__B19C6D03_DBEF_49B4_94B5_65A158ADD2B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_EmptyStacker_Elvator.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_EmptyStacker_Elvator command target

class CRun_EmptyStacker_Elvator : public CObject
{

public:
	DECLARE_SERIAL(CRun_EmptyStacker_Elvator)
	CRun_EmptyStacker_Elvator();           // protected constructor used by dynamic creation
	virtual ~CRun_EmptyStacker_Elvator();

// Attributes
public:
	int mn_RunStep;
	int mn_reinstate_step;
	int mn_InitStep;
	int m_nAxisNum;
	
	int mn_retry_cnt;
	long ml_retry_wait[3];
	

	bool m_bClampOnOffFlag;
	DWORD m_dwClampOnOff[3];
	int 	m_nTransferJobFlag[2];
	//work 트랜스퍼에 동작을 요청한다 , [0]:요청정보, [1]:1:트레이를 집기, 2:트레이를 놓기	
	char cJamcode[10];


	int m_nTransfer_WaitPosMove_Flag;

	double m_dcurr_pos[3];
	int	mn_reinstate_pos[3];

	long m_dwWaitTime[3];

	int m_nFindLotNo_Flag;
	CString m_strLotNo;
	CString m_strPartNo;
	CString m_strAlarmCode;

	int m_nRetry;

public:
	void Thread_Run();
	void RunMove();
	void RunInit();

	void Set_Tray_Fix_Clamp_ForBackward(int OnOff);
	int Chk_Tray_Fix_Clamp_ForBackward(int OnOff);

	int Ready_Stacker_Move_Check(int mode);

};
extern CRun_EmptyStacker_Elvator Run_EmptyStacker_Elvator;
/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_RUN_EMPTYSTACKER_ELVATOR_H__B19C6D03_DBEF_49B4_94B5_65A158ADD2B5__INCLUDED_)
