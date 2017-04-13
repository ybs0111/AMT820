// CFPSDateTimeCtrl
// Author:	Matt Gullett
//			gullettm@yahoo.com
// Copyright (c) 2001
//
// This is a utility class used for data entry of date and time values.
//
// You may freely use this source code in personal, freeware, shareware
// or commercial applications provided that 1) my name is recognized in the
// code and if this code represents a substantial portion of the application
// that my name be included in the credits for the application (about box, etc)
//
// Use this code at your own risk.  This code is provided AS-IS.  No warranty
// is granted as to the correctness, usefulness or value of this code.
//
// Special thanks to Oskar Wieland for his Easter calculation formula posted
// on CodeProject.
// http://www.codeproject.com/datetime/easter.asp
//
//
// *******************************************************************
// TECHNICAL NOTES:
//
// See .cpp file for tech notes
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_FPSDATETIMECTRL_H__055C7B66_AC8B_4FA3_A3F7_7D5A768DA99C__INCLUDED_)
#define AFX_FPSDATETIMECTRL_H__055C7B66_AC8B_4FA3_A3F7_7D5A768DA99C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FPSDateTimeCtrl.h : header file
//

#include "OleDateTimeEx.h"
#include "FPSDateTimePopupCtrl.h"
#include "FPSDateTimeButtonCtrl.h"
#include "FPSMiniCalendarCtrl.h"

#define WM_FPSDATE_HIDEDROPDOWN			WM_USER+102

/////////////////////////////////////////////////////////////////////////////
// CFPSDateTimeCtrl window

class CFPSDateTimeCtrl : public CEdit
{
// Construction
public:
	CFPSDateTimeCtrl();

// Attributes
public:
	virtual CString GetDisplayFormat() {return m_strDisplayFormat;}
	virtual void SetDisplayFormat(LPCTSTR lpszValue) {m_strDisplayFormat = lpszValue;}
	virtual DWORD GetParserOption() {return m_dwParserOption;}
	virtual void SetParserOption(DWORD dwValue) {m_dwParserOption = dwValue;}
	virtual COleDateTimeEx GetDate();
	virtual void SetDate(COleDateTime& dt);
	virtual void SetShowPickerButton(BOOL bValue) {m_bShowPickerButton = bValue;}
	virtual void SetCalendarStyles(DWORD dwStyle) {m_dwCalendarStyles = dwStyle;}
	virtual void SetSpecialDateCallback(funcSPECIALDATE pfunc) {m_pfuncSpecialDate = pfunc;}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFPSDateTimeCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ShowDropDown();
	void HideDropDown();
	int GetHourThreshold() {return m_lHourCutoff;}
	void SetHourThreshold(int lHourCutoff) {m_lHourCutoff = lHourCutoff;}
	long GetHourOption() {return m_lHourOptions;}
	void SetHourOption(long lHourOptions) {m_lHourOptions = lHourOptions;}

	virtual BOOL AttachEdit(CWnd* pwndParent, UINT uiID);
	virtual ~CFPSDateTimeCtrl();

	// Generated message map functions
protected:
	void RepositionButton();
	CString GetFormattedDate(LPCTSTR lpszValue);
	//{{AFX_MSG(CFPSDateTimeCtrl)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnNewText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHideDropDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
	CString	m_strDisplayFormat;
	BOOL	m_bProcessing;
	DWORD	m_dwParserOption;
	int		m_lHourOptions;
	int		m_lHourCutoff;

	DWORD					m_dwCalendarStyles;
	funcSPECIALDATE			m_pfuncSpecialDate;
	BOOL	m_bShowPickerButton;
	BOOL	m_bFirst;

	CFPSDateTimeButtonCtrl*	m_pwndButton;
	CFPSDateTimePopupCtrl*	m_pwndPopup;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FPSDATETIMECTRL_H__055C7B66_AC8B_4FA3_A3F7_7D5A768DA99C__INCLUDED_)
