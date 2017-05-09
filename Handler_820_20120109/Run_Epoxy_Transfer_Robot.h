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
	int mn_MoveStep;
	int mn_SafetyStep;
	int mn_reinstate_step;
	int mn_InitStep;
	int mn_BufferPos;

	int m_nRobot_X;
	int m_nRobot_Y;
	int m_nRobot_Z;
	int m_nRobot_S;

	long m_dwInitWaitTime[3];

	
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

	double m_dpTargetPos[3];

	int	    m_nLinearMove_Index; 
	long	m_lAxisCnt;
	long	m_lpAxisNum[4];
	double	m_dpTargetPosList[4];
	double	m_dpSpdRatio[3];	




	double md_TargetAxisXValue[2][2];//double ldEpoxyTransferXDispenserFirstStartPos;[0][0] first start [0][1] first end [1][0] second first [1][1] second end
	double md_TargetAxisYValue[2][2];//double ldEpoxyTransferYDispenserFirstStartPos;
	//double ldEpoxyTransferXDispenserFirstEndPos;
	//double ldEpoxyTransferYDispenserFirstEndPos;
	//double ldEpoxyTransferXDispenserSecondStartPos;
	//double ldEpoxyTransferYDispenserSecondStartPos;
	//double ldEpoxyTransferXDispenserSecondEndPos;
	//double ldEpoxyTransferYDispenserSecondEndPos;
	double md_TargetDotXValue[4];//double ldEpoxyTransferXDispenserDotPos[4];
	double md_TargetDotYValue[4];//double ldEpoxyTransferYDispenserDotPos[4];
	double md_DvcGap;

	long ml_motflag[3];
	int m_nBillardWorking;//Billard is working or not
	int mn_FirstSecond;//first Billiard or second Billard
	int mn_StartEnd;//fist, start end or second, start end

	DWORD m_dwSatbleWaitTime[3];
	double dp_PosList[3];
	double dp_SpdRatio[3];
	int mn_dotPos;

public:
	void Thread_Run();
	void RunMove();
	void RunInit();

	int Move_Billiard_Epoxy( int nMode, int nSite);
	int Robot_Move_Safety_Zone( int nMode, int n_site, int n_flag);
	void Get_Billard_Pos( int nPos );
};

/////////////////////////////////////////////////////////////////////////////
extern CRun_Epoxy_Transfer_Robot Run_Epoxy_Transfer_Robot;
#endif // !defined(AFX_RUN_EPOXY_TRANSFER_ROBOT_H__F0253461_9B3B_4C1C_AF3B_5F9C4530008F__INCLUDED_)
