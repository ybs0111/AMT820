#if !defined(AFX_RUN_EPOXY_TRANSFER_ROBOT_H__F0253461_9B3B_4C1C_AF3B_5F9C4530008F__INCLUDED_)
#define AFX_RUN_EPOXY_TRANSFER_ROBOT_H__F0253461_9B3B_4C1C_AF3B_5F9C4530008F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_Epoxy_Transfer_Robot.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_Epoxy_Transfer_Robot command target

class CRun_Epoxy_Transfer_Robot : public CObject
{
public:
	DECLARE_SERIAL(CRun_Epoxy_Transfer_Robot)

	CRun_Epoxy_Transfer_Robot();           // protected constructor used by dynamic creation
	virtual ~CRun_Epoxy_Transfer_Robot();

// Attributes
public:
	int mn_RunStep;
	int mn_reinstate_step;
	int mn_InitStep;
	int m_nAxisNum;
	
	int mn_retry_cnt;
	long ml_retry_wait[3];
	

	bool m_bClampOnOffFlag[2];
	DWORD m_dwClampOnOff[2][3];
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

	double m_dpTargetPos;

public:

	void ThreadRun();
	void RunMove();
	void RunInit();


};

/////////////////////////////////////////////////////////////////////////////
extern CRun_Epoxy_Transfer_Robot Run_Epoxy_Transfer_Robot;
#endif // !defined(AFX_RUN_EPOXY_TRANSFER_ROBOT_H__F0253461_9B3B_4C1C_AF3B_5F9C4530008F__INCLUDED_)
