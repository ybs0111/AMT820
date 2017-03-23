#if !defined(AFX_RUN_EMPTYTRAYTRANSFER_H__7A18250E_D64B_431D_B86A_3E03A6FF4167__INCLUDED_)
#define AFX_RUN_EMPTYTRAYTRANSFER_H__7A18250E_D64B_431D_B86A_3E03A6FF4167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_EmptyTrayTransfer.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_EmptyTrayTransfer command target

class CRun_EmptyTrayTransfer : public CObject
{
public:
	DECLARE_SERIAL(CRun_EmptyTrayTransfer)

	CRun_EmptyTrayTransfer();           // protected constructor used by dynamic creation
	virtual ~CRun_EmptyTrayTransfer();
// Attributes
public:

// Operations
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


	void Set_Tray_Grip_Vacuum_OnOff(int OnOff);
	int Chk_Tray_Grip_Vacuum_OnOff(int OnOff);
	void Set_Tray_Remover_Z_UpDown(int OnOff);
	int Chk_Tray_Remover_Z_UpDown(int OnOff);
};
extern CRun_EmptyTrayTransfer Run_EmptyTrayTransfer;
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_RUN_EMPTYTRAYTRANSFER_H__7A18250E_D64B_431D_B86A_3E03A6FF4167__INCLUDED_)
