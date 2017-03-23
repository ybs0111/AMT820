#if !defined(AFX_DIALOG_TEST_H__DB530450_2459_4E17_A7BB_1F8AEC4A8184__INCLUDED_)
#define AFX_DIALOG_TEST_H__DB530450_2459_4E17_A7BB_1F8AEC4A8184__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Test.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Test dialog
#include "Dialog_Setup_Tab.h"
class CDialog_Test : public CDialog
{
// Construction
public:
	CDialog_Test(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Test)
	enum { IDD = IDD_DIALOG4 };
	CDialog_Setup_Tab	m_test;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Test)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Test)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_TEST_H__DB530450_2459_4E17_A7BB_1F8AEC4A8184__INCLUDED_)
