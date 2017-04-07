#if !defined(AFX_FPSDATEPICKERCTRL_H__78D0D4C9_0C6E_4BFF_9920_4639CFAA9247__INCLUDED_)
#define AFX_FPSDATEPICKERCTRL_H__78D0D4C9_0C6E_4BFF_9920_4639CFAA9247__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FPSDatePickerCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFPSDatePickerCtrl window

class CFPSDatePickerCtrl : public CDateTimeCtrl
{
// Construction
public:
	CFPSDatePickerCtrl();

// Attributes
public:

// Operations
public:
	BOOL AttachEdit(CWnd *pwndParent, UINT uiID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFPSDatePickerCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFPSDatePickerCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFPSDatePickerCtrl)
	afx_msg void OnUserstring(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FPSDATEPICKERCTRL_H__78D0D4C9_0C6E_4BFF_9920_4639CFAA9247__INCLUDED_)
