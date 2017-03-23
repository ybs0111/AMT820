#if !defined(AFX_RUN_LDPICKER_H__D18CCDCF_2A28_48D6_B034_905920623548__INCLUDED_)
#define AFX_RUN_LDPICKER_H__D18CCDCF_2A28_48D6_B034_905920623548__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_LdPicker.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_LdPicker command target

class CRun_LdPicker : public CObject
{
public:
	DECLARE_SERIAL(CRun_LdPicker);

	CRun_LdPicker();           // protected constructor used by dynamic creation
	virtual ~CRun_LdPicker();

// Attributes
public:
	void ThreadRun();
	void RunMove();
	void RunInit();

	int mn_RunStep;
	int mn_InitStep;
	int mn_Pick_Step;
	int mn_Place_Step;
	int mn_reinstate_step;
	
	int mn_retry_cnt;
	long ml_retry_wait[3];
	
	long ml_wait_time[3];
	long ml_stop_wait[3];

	long ml_clamp_wait_time[3];
	int mn_clamp_retry_cnt;
	

	int m_nFindLotNo_Flag;
	CString m_strFindLotNo;
	
	char cJamcode[10];

	bool m_bInitVarBackward;

	double m_dcurr_pos[3];
	int	mn_reinstate_pos[3];

	long m_dwWaitTime[3];

	int m_nPickerPara;
	int	m_nRobot_Y;
	int m_nRobot_Z;
	int m_nRobot_Z_Stop_Flag;
	int m_nRobotSite;

	DWORD m_dwWaitTime[3];
	int m_bClampOnOffFlag;
	DWORD m_dwClampOnOff[3];

	int m_nPickRetry_Cnt;
	double			m_dCurrentPos[100];

	int				 m_npPicker_OutputStatus[4];// output 상태를 체크 

	int				m_npFindWorkPosYXCPB[5]; //처음 찾을때의 정보,  [0]first tray Y, [1]first tray X, [2]:작업가능 수량 또는 공간 정보, [3]first picker num, [4]first buff num
	int				m_npSet_WorkPosYXCPB[5]; //작업 완료후 작업 셋팅 위치 정보,  [0]first tray Y, [1]first tray X, [2]:작업가능 수량 또는 공간 정보, [3]first picker num, [4]first buff num

	int				m_npTemp_Picker_YesNo[4];  //임시 작업가능피커 정의 
	int				m_npPicker_YesNo[4];		//작업 가능 피커 정의 
	int				m_npPicker_Vacuum_Status[4];//현재 백큠 상태 정보 
	int				m_npPicker_Data_Status[4]; //디바이스 수량등 정보 
	int				m_npPicker_Error_Status[4];//피커 down후 트레이에서 집을때 백큠 에러 정보 

	int				m_n_FirstPicker_Num;
	int				m_n_FirstTray_X_Num;
	int				m_n_FirstTray_Y_Num;

	DWORD		m_dwWaitUntil[3];
	int				m_nMove_Flag[5]; //모터 이동 플레그	

	//Process_DVC_PICK
	bool m_bDvcWaitChk_Falg;// if DVC is exist, or not, It will check.

	//Process_DVC_PLACE
	int					m_nClash_ErrorInfoStatus[4];
	double          m_dTemp_CalTargetPos_Y;
	double			m_dpTargetPosList[4];		   //최대 4 포인트를 한번에 이동가능하다 

	//2016.1203
	CString			m_strLotNo[2];
	CString			m_strPartNo[2];	


	int mn_override_flag;
	long lp_OvrrideFlag;
public:
	
	int Process_DVC_Pickup( int nMode, int nWorkSite, CString strLotNo);
	int Process_DVC_Place( int nMode, int nWorkSite, CString strLotNo);

	//buffer align
	void Set_Loader_Buffer_Align_OnOff(int OnOff);
	int Chk_Loader_Buffer_Align_OnOff( int nOnOff );
	//picker clamp
	void Set_Loader_Transfer_Clamp_OnOff(int OnOff);
	int Chk_Loader_Transfer_Clamp_OnOff( int nOnOff );


// Implementation
protected:


};

/////////////////////////////////////////////////////////////////////////////
extern CRun_LdPicker Run_LdPicker;

#endif // !defined(AFX_RUN_LDPICKER_H__D18CCDCF_2A28_48D6_B034_905920623548__INCLUDED_)
