#if !defined(AFX_RUN_HEATSINKVISION_TRANSFER_ROBOT_H__7915B0A4_A413_4DFA_8BE4_0CF86B850AB5__INCLUDED_)
#define AFX_RUN_HEATSINKVISION_TRANSFER_ROBOT_H__7915B0A4_A413_4DFA_8BE4_0CF86B850AB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_HeatSinkVision_Transfer_Robot.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_HeatSinkVision_Transfer_Robot command target

class CRun_HeatSinkVision_Transfer_Robot : public CObject
{
public:
	DECLARE_SERIAL(CRun_HeatSinkVision_Transfer_Robot)

	CRun_HeatSinkVision_Transfer_Robot();           // protected constructor used by dynamic creation
	virtual ~CRun_HeatSinkVision_Transfer_Robot();

// Attributes
public:
	int mn_RunHsStep;
	int mn_RunVisStep;
	int mn_RunDispensorStep;
	int mn_RunBuffStep;
	int mn_PickStep;
	int mn_PlaceStep;
	int mn_MoveStep;
	int mn_SafetyStep;
	int mn_reinstate_step;
	int mn_InitStep;
	int mn_BufferPos;
	int mn_MoveMeasureStep;
	
	int m_nRobot_X;
	int m_nRobot_Y;
	int m_nRobot_Z;
	int m_nRobot_P;
	int m_nRobot_DisY;
	int m_nRobot_VisY;
	int m_nRobot_VisZ;
	int m_nCarriorPos;
	int m_nVisCarriorPos;
	int m_nVisTeachPos;

	int m_nlast_pick;

	int mn_retry_cnt;
	long ml_retry_wait[3];


	//////////////////////////////////////////////////////////////////////////
	//CAMERA
	bool m_bCameraFwdBwdFlag;
	DWORD m_dwCameraFwdBwd[3];

	bool m_bCameraUpDnFlag;
	DWORD m_dwCameraUpDn[3];
	//////////////////////////////////////////////////////////////////////////
	bool m_bWorkHeat;//Is there a device that working heatsink

	//////////////////////////////////////////////////////////////////////////
	//HEATSINK
	bool m_bPadVacuumFlag;
	DWORD m_dwPadVacuum[3];

	bool m_bPadUpDnFlag;
	DWORD m_dwPadUpDn[3];

	double m_dpTargetPos[3];

	int		m_nLinearMove_Index; 
	long	m_lAxisCnt;
	long	m_lpAxisNum[4];
	double	m_dpTargetPosList[4];
	double	m_dpSpdRatio[3];


	int		m_nMove_Flag[3];

	//////////////////////////////////////////////////////////////////////////
	//DVC pickup
	bool	m_bDvcWaitChk_Falg;
	int		m_nPickerPara;
	int		m_nPickRetry_Cnt;
	int		m_npPicker_YesNo[4];
	int		m_npPicker_Data_Status[4]; //디바이스 수량등 정보 
	int		m_npPicker_Vacuum_Status[4];
	int		m_npPicker_OutputStatus[4];
	double  m_dTemp_CalTargetPos_X;
	double  m_dTemp_CalTargetPos_Y;
	int		m_npFindWorkPosYXCPB[5]; //[0]first tray X, [1]first tray Y, [2]:작업가능 수량 또는 공간 정보, [3]first picker num, [4]first buff num
	int		m_npSet_WorkPosYXCPB[5]; //작업 완료후 작업 셋팅 위치 정보,  [0]first tray Y, [1]first tray X, [2]:작업가능 수량 또는 공간 정보, [3]first picker num, [4]first buff num

	DWORD	m_dwWaitUntil[3];

	int		m_n_TrayNum;
	int		m_n_FirstPicker_Num;
	int		m_n_FirstTray_X_Num;
	int		m_n_FirstTray_Y_Num;

	double  m_dCurrentPos[100];
	int     m_nRubbRetry_Cnt;

	//////////////////////////////////////////////////////////////////////////
	//Dispensor
	DWORD	m_dwWaitDispenserAirBlow[3];
	bool	m_bReverseUpDnFlag;
	DWORD	m_dwReverseUpDn[3];

	bool	m_bReverseFwdBwdFlag;
	DWORD	m_dwReverseFwdBwd[3];

	bool	m_bReverseFlag;
	DWORD	m_dwReverse[3];
	//////////////////////////////////////////////////////////////////////////
	
	int m_nFindLotNo_Flag;
	CString m_strLotNo[2];
	CString m_strPartNo[2];
	CString m_strAlarmCode;

	//////////////////////////////////////////////////////////////////////////
	//Measure Vision
	DWORD m_dwMCameraResWaitTime[3];
	int m_nContinusVError;
	int m_nTotalVError;
	bool	m_bVisionFwdBwdFlag;
	DWORD	m_dwVisionwdFwdBwd[3];


// Operations
public:

	void Thread_Run();
	void RunMoveHeatSink();
	void RunMoveVision();
	void RunMoveDispensor();
	void RunMoveBuffDispensor();
	void RunInit();
	
	int Process_Measure_Vision();
	int Process_Dvc_Pickup(int nMode, int nWork_Site);
	int Process_Dvc_Place(int nMode, int nWork_Site, int nPos);
	int Robot_AutoMove_Safety_Zone( int nMode, int n_pos);
	

	void Set_Vision_Y_Clamp_ForeardBackward(int OnOff);
	int Chk_Vision_Y_Clamp_ForeardBackward(int OnOff);

	void Set_HeatSink_Reverse_Turn(int OnOff);
	int Chk_HeatSink_Reverse_Turn(int OnOff);

	void Set_HeatSink_Reverse_UpDown(int OnOff);
	int Chk_HeatSink_Reverse_UpDown(int OnOff);

	void Set_HeatSink_Reverse_Clamp_ForwardBackward(int OnOff);
	int Chk_HeatSink_Reverse_Clamp_ForwardBackward(int OnOff);

	void Set_Dispenser_Air_Blow(int OnOff);

	void Set_HeatSink_Transfer_Pad_UpDown(int OnOff);
	int Chk_HeatSink_Transfer_Pad_UpDown(int OnOff);
	void Set_HeatSink_Vacuum_OnOff(int OnOff);
	int Chk_HeatSink_Vacuum_OnOff(int OnOff);
	void Set_Device_CameraY_Jig_Press_ForwardBackward( int OnOff );
	int Chk_Device_CameraY_Jig_Press_ForwardBackward( int nOnOff );

	void Set_Device_Carrier_Camera_UV_LAMP_OnOff(int OnOff);
	void Set_Device_Carrier_Camera_LED_LAMP_OnOff(int OnOff);
	void Set_Device_Carrier_Camera_Y_Press_UpDown(int OnOff);
	int Chk_Device_Carrier_Camera_Y_Press_UpDown(int OnOff);
};

/////////////////////////////////////////////////////////////////////////////
extern CRun_HeatSinkVision_Transfer_Robot Run_HeatSinkVision_Transfer_Robot;

#endif // !defined(AFX_RUN_HEATSINKVISION_TRANSFER_ROBOT_H__7915B0A4_A413_4DFA_8BE4_0CF86B850AB5__INCLUDED_)
