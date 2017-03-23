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

	CString m_strLotNo[2];
	CString m_strPartNo[2];

	DWORD m_dwCycleTime[2][3];

	bool m_bDvcWaitChk_Falg;
	bool m_bClampOnOffFlag;
	DWORD m_dwClampOnOff[3];

	long m_motFlag[3];
	DWORD m_dwWaitUntil[3];

	long m_motPickFlag[3];
	long m_motPlaceFlag[3];
	double m_dpTargetPosList[3];
	double m_dTemp_CalTargetPos_X;
	double m_dTemp_CalTargetPos_Y

// Operations
public:
	void ThreadRun();
	void RunMove();
	void RunInit();


	void Set_UnLoader_Transfer_Clamp_OnOff(int OnOff);
	int Chk_UnLoader_Transfer_Clamp_OnOff( int nOnOff );

	int Process_DVC_Pickup( int nMode, int nWorkSite, CString strLotNo);
	int Process_DVC_Place( int nMode, int nWorkSite, CString strLotNo);

	int Move_MakeSafetyPosBeforeWork( int nMode, int nWorkSite );
	int CRun_UldPicker::Move_ReadySafeyXY();

};

/////////////////////////////////////////////////////////////////////////////
extern CRun_UldPicker Run_UldPicker;

#endif // !defined(AFX_RUN_ULDPICKER_H__EB3432E0_75EE_4A35_9017_202D52801E79__INCLUDED_)
