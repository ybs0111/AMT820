#if !defined(AFX_FPSDATETIMEBUTTONCTRL_H__E8C74E85_8A30_4CA8_819C_9310971D3AE9__INCLUDED_)
#define AFX_FPSDATETIMEBUTTONCTRL_H__E8C74E85_8A30_4CA8_819C_9310971D3AE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FPSDateTimeButtonCtrl.h : header file
//

class CFPSDateTimeCtrl;
/////////////////////////////////////////////////////////////////////////////
// CFPSDateTimeButtonCtrl window

class CFPSDateTimeButtonCtrl : public CButton
{
// Construction
public:
	CFPSDateTimeButtonCtrl();

// Attributes
public:
	void SetEditCtrl(CFPSDateTimeCtrl* pWnd) {m_pwndEdit = pWnd;}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFPSDateTimeButtonCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFPSDateTimeButtonCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFPSDateTimeButtonCtrl)
	afx_msg void OnClicked();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	CFPSDateTimeCtrl*	m_pwndEdit;
	CBitmap				m_Image;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FPSDATETIMEBUTTONCTRL_H__E8C74E85_8A30_4CA8_819C_9310971D3AE9__INCLUDED_)
