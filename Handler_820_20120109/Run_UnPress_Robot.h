#if !defined(AFX_RUN_UNPRESS_ROBOT_H__615647AB_38B8_4BEB_A3CE_E388E1E113F2__INCLUDED_)
#define AFX_RUN_UNPRESS_ROBOT_H__615647AB_38B8_4BEB_A3CE_E388E1E113F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_UnPress_Robot.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_UnPress_Robot command target

class CRun_UnPress_Robot : public CObject
{
public:
	DECLARE_DYNCREATE(CRun_UnPress_Robot)
	CRun_UnPress_Robot();           // protected constructor used by dynamic creation
	virtual ~CRun_UnPress_Robot();


// Attributes
public:
	int mn_RunStep;
	int mn_Pick_Step;
	int mn_Place_Step;

	int m_nPressAxisX;
	int m_mPressAxisY;
	DWORD m_dwWaitUntil[3];
	int m_nTrayDeviceGap;
	double m_dTargetPosX;
	double m_dTargetPosY;

	bool m_bPressUpDnFlag[2];
	DWORD m_dwPressUpDn[2][3];

	bool m_bCarrierHolder[2];
	DWORD m_dwCarrierHolder[2][3];

	CString m_strAlarmCode;

// Operations
public:
	void ThreadRun();
	void RunMove();
	void RunInit();

	//nMode = 0 ���� ���Ͽ� ���Ͱ� �����϶�
	//nMode = 1 Holder ���¿��� �����϶�
	//nMode = 2 Press down���·� �ж�
	int Chk_PressClamp_Safety( int nMode);
	int Chk_PressMotor_Safety();
	void Set_UnPress_PushClamp_OnOff( int nMode, int OnOff);
	int Chk_UnPress_PushClamp_OnOff( int nMode, int nOnOff );

	void Set_Device_Carrier_Holder( int OnOff );
	int Chk_Device_Carrier_Holder( int nOnOff );

	int Process_Unpress_Pickup( int nMode, int nWorkSite, CString strLotNo);

};

/////////////////////////////////////////////////////////////////////////////
extern CRun_UnPress_Robot Run_UnPress_Robot;

#endif // !defined(AFX_RUN_UNPRESS_ROBOT_H__615647AB_38B8_4BEB_A3CE_E388E1E113F2__INCLUDED_)
