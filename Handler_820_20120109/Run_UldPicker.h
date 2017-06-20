#if !defined(AFX_RUN_ULDPICKER_H__EB3432E0_75EE_4A35_9017_202D52801E79__INCLUDED_)
#define AFX_RUN_ULDPICKER_H__EB3432E0_75EE_4A35_9017_202D52801E79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_UldPicker.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_UldPicker command target

#define POS_CHK_READY		0
#define POS_MOV_READY		1
#define POS_MOV_PICK		2
#define POS_MOV_PLACE		3

class CRun_UldPicker : public CObject
{
public:
	DECLARE_SERIAL(CRun_UldPicker)

	CRun_UldPicker();           // protected constructor used by dynamic creation
	virtual ~CRun_UldPicker();

// Attributes
public:
	int mn_RunStep;
	int mn_RunSafetyStep;
	int mn_InitStep;
	int mn_Pick_Step;
	int mn_Place_Step;
	int mn_reinstate_step;
	int mn_MoveStep;

	int m_nRobot_X;
	int m_nRobot_Y;
	int m_nRobot_Z;

	int		m_nLinearMove_Index; 
	long	m_lAxisCnt;
	long	m_lpAxisNum[4];
	double	m_dpTargetPosList[4];
	double	m_dpSpdRatio[3];


	int m_nPickerPara;
	int m_nRobotSite;
	int m_nRobot_Z_Stop_Flag;

	int m_nMove_Flag[2];
	CString m_strLotNo[2];
	CString m_strPartNo[2];

	DWORD m_dwCycleTime[2][3];

	bool m_bDvcWaitChk_Falg;
	bool m_bClampOnOffFlag;
	DWORD m_dwClampOnOff[3];

	long m_motFlag[3];
	DWORD m_dwWaitUntil[3];

	int m_nPickRetry_Cnt;
	long m_motPickFlag[3];
	long m_motPlaceFlag[3];
	double m_dTemp_CalTargetPos_X;
	double m_dTemp_CalTargetPos_Y;


	int	m_npPicker_OutputStatus[4];// output ���¸� üũ 
	
	int	m_npSet_WorkPosYXCPB[5]; //�۾� �Ϸ��� �۾� ���� ��ġ ����,  [0]first tray Y, [1]first tray X, [2]:�۾����� ���� �Ǵ� ���� ����, [3]first picker num, [4]first buff num
	
	int	m_npTemp_Picker_YesNo[4];  //�ӽ� �۾�������Ŀ ���� 
	int	m_npPicker_YesNo[4];		//�۾� ���� ��Ŀ ���� 
	int	m_npPicker_Vacuum_Status[4];//���� ��Ũ ���� ���� 
	int	m_npPicker_Data_Status[4]; //����̽� ������ ���� 
	int	m_npPicker_Error_Status[4];//��Ŀ down�� Ʈ���̿��� ������ ��Ũ ���� ���� 
	
	int	m_npFindWorkPosYXCPB[5]; //[0]first tray Y, [1]first tray X, [2]:�۾����� ���� �Ǵ� ���� ����, [3]first picker num, [4]first buff num

	int m_n_FirstTray_X_Num;
	int m_n_FirstTray_Y_Num;
	int m_n_FirstPicker_Num;

	double m_dCurrentPos[100];
	CString m_strAlarmCode;
	DWORD m_dwWaitTime[3];
	BOOL m_bConnectAssemblyTimeOut;

// Operations
public:
	void Thread_Run();
	void RunMove();
	void RunInit();


	void Set_UnLoader_Transfer_Clamp_OnOff(int OnOff);
	int Chk_UnLoader_Transfer_Clamp_OnOff( int nOnOff );

	int Process_DVC_Pickup( int nMode, int nWorkSite, CString strLotNo);
	int Process_DVC_Place( int nMode, int nWorkSite, CString strLotNo);

	int Move_ReadySafeyXY();
	int Move_MakeSafetyPosBeforeWork( int nMode );

};

/////////////////////////////////////////////////////////////////////////////
extern CRun_UldPicker Run_UldPicker;

#endif // !defined(AFX_RUN_ULDPICKER_H__EB3432E0_75EE_4A35_9017_202D52801E79__INCLUDED_)
