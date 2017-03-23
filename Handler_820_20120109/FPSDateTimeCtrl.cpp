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

#include "stdafx.h"
#include "FPSDateTimeCtrl.h"

#include <math.h>
#include "FPSMiniCalendarCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// message used internally when new text is set for the control
#define WM_FPSDATECTRL_NEWTEXT	WM_USER+1001

/////////////////////////////////////////////////////////////////////////////
// CFPSDateTimeCtrl

CFPSDateTimeCtrl::CFPSDateTimeCtrl()
{
	m_bFirst = TRUE;
	m_pwndPopup = NULL;
	m_pwndButton = NULL;

	m_lHourOptions = 0;
	m_lHourCutoff = 7;
	m_dwParserOption = 0;
	m_bProcessing = FALSE;
	m_strDisplayFormat = "%m/%d/%Y";
	m_bShowPickerButton = FALSE;
	m_dwCalendarStyles = FMC_TODAYBUTTON | FMC_NONEBUTTON;
	m_pfuncSpecialDate = NULL;
}

CFPSDateTimeCtrl::~CFPSDateTimeCtrl()
{
	if (m_pwndPopup)
		delete m_pwndPopup;
	if (m_pwndButton)
		delete m_pwndButton;
}


BEGIN_MESSAGE_MAP(CFPSDateTimeCtrl, CEdit)
	//{{AFX_MSG_MAP(CFPSDateTimeCtrl)
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_FPSDATECTRL_NEWTEXT, OnNewText)
	ON_MESSAGE(WM_FPSDATE_HIDEDROPDOWN, OnHideDropDown)
END_MESSAGE_MAP()

// subclass a control on the parent
BOOL CFPSDateTimeCtrl::AttachEdit(CWnd *pwndParent, UINT uiID)
{
	BOOL bReturn = TRUE;

	if (pwndParent && ::IsWindow(pwndParent->GetSafeHwnd()))
	{
		CWnd* pwndChild = pwndParent->GetDlgItem(uiID);
		
		if (pwndChild && ::IsWindow(pwndChild->GetSafeHwnd()))
		{
			CEdit::SubclassDlgItem(uiID, pwndParent);
		}
		else
		{
			ASSERT(FALSE);
			bReturn = FALSE;
		}
	}
	else
	{
		ASSERT(FALSE);
		bReturn = FALSE;
	}

	return bReturn;
}

/////////////////////////////////////////////////////////////////////////////
// CFPSDateTimeCtrl message handlers
void CFPSDateTimeCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	if (!::IsWindow(GetSafeHwnd()))
		return;

	CString strText;

	GetWindowText(strText);

	m_bProcessing = TRUE;
	SetWindowText(GetFormattedDate(strText));
	m_bProcessing = FALSE;

	CEdit::OnKillFocus(pNewWnd);
}

CString CFPSDateTimeCtrl::GetFormattedDate(LPCTSTR lpszValue)
{
	CString strReturn;
	COleDateTimeEx dtTemp;

	dtTemp.SetHourOption(GetHourOption());
	dtTemp.SetHourThreshold(GetHourThreshold());

	// parse the input date into a COleDateTime object
	if (dtTemp.ParseDateTime(lpszValue, m_dwParserOption) && dtTemp.m_status == COleDateTime::valid)
	{
		// if the window text equates to a parsable date/time
		// just return it in the method formatted according
		// to the m_strDisplayFormat
		strReturn = dtTemp.Format(m_strDisplayFormat);
	}
	else
	{
		// not a date value and not an evaluatable
		// text value, just return the input value
		strReturn = lpszValue;
	}

	return strReturn;
}

BOOL CFPSDateTimeCtrl::PreTranslateMessage(MSG* pMsg) 
{	
	return CEdit::PreTranslateMessage(pMsg);
}

void CFPSDateTimeCtrl::OnUpdate() 
{
	if (!m_bProcessing && ::GetFocus() != m_hWnd)
	{
		CString strText;

		GetWindowText(strText);

		CString* pNewText = new CString(GetFormattedDate(strText));

		PostMessage(WM_FPSDATECTRL_NEWTEXT, (WPARAM)pNewText);
	}
}

LRESULT CFPSDateTimeCtrl::OnNewText(WPARAM wParam, LPARAM)
{
	m_bProcessing = TRUE;

	CString* pString = (CString*)wParam;

	if (pString)
	{
		SetWindowText(*pString);
		delete pString;
	}

	m_bProcessing = FALSE;

	return 0;
}


COleDateTimeEx CFPSDateTimeCtrl::GetDate()
{
	COleDateTimeEx dtReturn;

	CString strText;

	GetWindowText(strText);

	dtReturn.SetHourOption(GetHourOption());
	dtReturn.SetHourThreshold(GetHourThreshold());
	dtReturn.ParseDateTime(strText, m_dwParserOption);

	return dtReturn;
}

void CFPSDateTimeCtrl::SetDate(COleDateTime &dt)
{
	if (dt.GetStatus() == COleDateTime::valid)
		SetWindowText(dt.Format(m_strDisplayFormat));
	else
		SetWindowText("");
}

void CFPSDateTimeCtrl::HideDropDown()
{
	if (m_pwndPopup)
	{
		if (::IsWindow(m_pwndPopup->m_hWnd))
			m_pwndPopup->ShowWindow(FALSE);
		m_pwndPopup->EndTracking();
		delete m_pwndPopup;
		m_pwndPopup = NULL;
	}
}

void CFPSDateTimeCtrl::ShowDropDown()
{
	if (m_pwndPopup)
	{
		if (::IsWindow(m_pwndPopup->m_hWnd))
			m_pwndPopup->ShowWindow(FALSE);
		delete m_pwndPopup;
		m_pwndPopup = NULL;
	}

	// determine rectangle
	CRect ComboRect;
	CRect ListRect;

	GetClientRect(ComboRect);
	ClientToScreen(ComboRect);

	// box is too far to the left or right or when it is to low on the screen
	int iLeft = ComboRect.left;
	int iTop = ComboRect.bottom;

	ListRect.SetRect(iLeft, iTop, iLeft, iTop);

	m_pwndPopup = new CFPSDateTimePopupCtrl;
	m_pwndPopup->SetEditCtrl(this);
	m_pwndPopup->SetDate(GetDate());
	m_pwndPopup->SetCalendarStyles(m_dwCalendarStyles);
	m_pwndPopup->SetSpecialDateCallback(m_pfuncSpecialDate);

	// setup styles and class info
	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_POPUP | WS_EX_TOPMOST | WS_EX_WINDOWEDGE;
	LPCTSTR szWndCls = AfxRegisterWndClass(CS_BYTEALIGNCLIENT | CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
										   0,0,0);

	// create control
	m_pwndPopup->CreateEx(dwExStyle, 
							szWndCls, 
							NULL, 
							dwStyle, 
							iLeft-2, 
							iTop, 
							0, 
							0, 
							m_hWnd, 
							NULL);

	m_pwndPopup->ShowWindow(TRUE);
	m_pwndPopup->BeginTracking();

	RedrawWindow();
}

LRESULT CFPSDateTimeCtrl::OnHideDropDown(WPARAM, LPARAM)
{
	HideDropDown();

	return 0;
}
int CFPSDateTimeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	return 0;
}

void CFPSDateTimeCtrl::OnDestroy() 
{
	if (m_pwndButton)
	{
		delete m_pwndButton;
		m_pwndButton = NULL;
	}

	CEdit::OnDestroy();	
}

void CFPSDateTimeCtrl::OnMove(int x, int y) 
{
	CEdit::OnMove(x, y);
	
	if (m_bShowPickerButton)
		RepositionButton();
}

void CFPSDateTimeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CEdit::OnSize(nType, cx, cy);
	
	if (m_bShowPickerButton)
		RepositionButton();
}

void CFPSDateTimeCtrl::RepositionButton()
{
	if (m_pwndButton && GetParent())
	{
		CWnd* pwndParent = GetParent();
		CRect WndRect;
		CRect ButtonRect;

		GetWindowRect(WndRect);
		pwndParent->ScreenToClient(WndRect);

		ButtonRect.SetRect(WndRect.right+1, WndRect.top+1, WndRect.right+17, WndRect.bottom-1);
		m_pwndButton->SetWindowPos(NULL, ButtonRect.left, ButtonRect.top, ButtonRect.Width(), ButtonRect.Height(), 0);
	}
}

LRESULT CFPSDateTimeCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT lReturn = CEdit::DefWindowProc(message, wParam, lParam);

	if (m_bFirst)
	{
		if (m_bShowPickerButton)
		{
			ASSERT(!m_pwndButton);

			DWORD dwStyle = WS_VISIBLE | WS_CHILD | BS_BITMAP;
			m_pwndButton = new CFPSDateTimeButtonCtrl;
			m_pwndButton->SetEditCtrl(this);
			m_pwndButton->Create(NULL, dwStyle, CRect(0,0,0,0), GetParent(), 9191);

			RepositionButton();
		}

		m_bFirst = FALSE;
	}

	return lReturn;
}
