#if !defined(AFX_DIALOG_SELECT_RETEST_H__944E0633_021D_42D4_853E_6DAE392A986B__INCLUDED_)
#define AFX_DIALOG_SELECT_RETEST_H__944E0633_021D_42D4_853E_6DAE392A986B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Select_Retest.h : header file
//

#include "MacButtons.h"
#include "NewLabel.h"
#include "GradientStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CDialog_Select_Retest dialog

class CDialog_Select_Retest : public StandardDialog
{
// Construction
public:
	CDialog_Select_Retest(CWnd* pParent = NULL);   // standard constructor
	void OnSM_Button_Set();
	int mn_dialog;
	CFont* mp_sm_font;
	void OnSM_Label_Set();

// Dialog Data
	//{{AFX_DATA(CDialog_Select_Retest)
	enum { IDD = IDD_DIALOG_SELECT_REJECT };
	CMacButton	mp_btn_sm_lotcontinue;
	CMacButton	mp_btn_sm_buzzer_off;
	CMacButton	mp_btn_sm_reject_test;
	CNewLabel	mp_msg_sm_tray_clear;
	CGradientStatic	mp_msg_sm_info;
	CMacButton	mp_btn_sm_lotend;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Select_Retest)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Select_Retest)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSmLotend();
	afx_msg void OnBtnSmBuzzerOff();
	afx_msg void OnBtnSmRejectTest();
	afx_msg void OnBtnSmLotContinue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_SELECT_RETEST_H__944E0633_021D_42D4_853E_6DAE392A986B__INCLUDED_)
