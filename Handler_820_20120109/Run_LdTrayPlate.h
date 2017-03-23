#if !defined(AFX_RUN_LDTRAYPLATE_H__F2FD03DC_A11E_495A_961A_D0D5BDBF31D3__INCLUDED_)
#define AFX_RUN_LDTRAYPLATE_H__F2FD03DC_A11E_495A_961A_D0D5BDBF31D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Run_LdTrayPlate.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRun_LdTrayPlate command target

class CRun_LdTrayPlate : public CObject
{
public:
	DECLARE_SERIAL(CRun_LdTrayPlate)

	CRun_LdTrayPlate();           // protected constructor used by dynamic creation
	virtual ~CRun_LdTrayPlate();

// Attributes
public:
	void ThreadRun();
	void RunMove();
	void RunInit();

	int mn_RunStep;
	int mn_InitStep;
	int mn_reinstate_step;

	CString m_strAlarmCode;

	int m_nFindLotNo_Flag;
	CString m_strLotNo;
	CString m_strPartNo;

	int m_bClampOnOffFlag;
	DWORD m_dwClampOnOff[3];

// Operations
public:

	void Set_Tray_Guide_Clamp_ForBackward(int OnOff);
	int Chk_Tray_Guide_Clamp_ForBackward(int OnOff);
};

/////////////////////////////////////////////////////////////////////////////
extern CRun_LdTrayPlate Run_LdTrayPlate;
#endif // !defined(AFX_RUN_LDTRAYPLATE_H__F2FD03DC_A11E_495A_961A_D0D5BDBF31D3__INCLUDED_)
