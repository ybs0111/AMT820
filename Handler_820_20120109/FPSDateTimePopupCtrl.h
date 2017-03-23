#if !defined(AFX_FPSDATETIMEPOPUPCTRL_H__22BD3592_7377_436C_9FF1_B4D801579E69__INCLUDED_)
#define AFX_FPSDATETIMEPOPUPCTRL_H__22BD3592_7377_436C_9FF1_B4D801579E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FPSDateTimePopupCtrl.h : header file
//

class CFPSDateTimeCtrl;
#include "FPSMiniCalendarCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CFPSDateTimePopupCtrl window

class CFPSDateTimePopupCtrl : public CWnd
{
// Construction
public:
	CFPSDateTimePopupCtrl();

// Attributes
public:

// Operations
public:
	void BeginTracking();
	void EndTracking();
	void SetEditCtrl(CFPSDateTimeCtrl* pwnd) {m_pwndEdit = pwnd;}
	void SetDate(COleDateTime dt) {m_dt = dt;}
	void SetCalendarStyles(DWORD dwStyle) {m_dwStyles = dwStyle;}
	void SetSpecialDateCallback(funcSPECIALDATE pfunc) {m_pfuncSpecialDate = pfunc;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFPSDateTimePopupCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFPSDateTimePopupCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFPSDateTimePopupCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	afx_msg void OnCalendarClick(NMHDR * , LRESULT *  );
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

	CFPSMiniCalendarCtrl	m_wndCalendar;
	COleDateTime			m_dt;
	DWORD					m_dwStyles;
	funcSPECIALDATE			m_pfuncSpecialDate;
	CFPSDateTimeCtrl*		m_pwndEdit;

	BOOL					m_bTracking;
	UINT					m_uiTimerID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FPSDATETIMEPOPUPCTRL_H__22BD3592_7377_436C_9FF1_B4D801579E69__INCLUDED_)
