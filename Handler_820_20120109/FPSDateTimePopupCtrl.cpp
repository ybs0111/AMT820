// FPSDateTimePopupCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "FPSDateTimePopupCtrl.h"

#include "FPSDateTimeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WMID_CALENDAR 32999

/////////////////////////////////////////////////////////////////////////////
// CFPSDateTimePopupCtrl

CFPSDateTimePopupCtrl::CFPSDateTimePopupCtrl()
{
	m_pwndEdit = NULL;
	m_dt = COleDateTime::GetCurrentTime();
	m_dwStyles = FMC_TODAYBUTTON | FMC_NONEBUTTON;
	m_pfuncSpecialDate = NULL;
	m_bTracking = FALSE;
	m_uiTimerID = 0;
}

CFPSDateTimePopupCtrl::~CFPSDateTimePopupCtrl()
{
}


BEGIN_MESSAGE_MAP(CFPSDateTimePopupCtrl, CWnd)
	//{{AFX_MSG_MAP(CFPSDateTimePopupCtrl)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_NCRBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, WMID_CALENDAR, OnCalendarClick)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFPSDateTimePopupCtrl message handlers

BOOL CFPSDateTimePopupCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (m_pwndEdit && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		m_pwndEdit->PostMessage(WM_FPSDATE_HIDEDROPDOWN);
	
	return CWnd::PreTranslateMessage(pMsg);
}

int CFPSDateTimePopupCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndCalendar.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | FMC_AUTOSETTINGS | m_dwStyles, CRect(0,0,0,0), this, WMID_CALENDAR);
	m_wndCalendar.SetMaxSize(FMC_MAX_SIZE_NONE);
	m_wndCalendar.SetDate(m_dt);
	if (m_dt.GetStatus() == COleDateTime::valid)
		m_wndCalendar.SetCurrentMonthAndYear(m_dt.GetMonth(), m_dt.GetYear());
	else
		m_wndCalendar.SetCurrentMonthAndYear(COleDateTime::GetCurrentTime().GetMonth(), COleDateTime::GetCurrentTime().GetYear());
	m_wndCalendar.SetSpecialDaysCallback(m_pfuncSpecialDate);
	m_wndCalendar.SetRowsAndColumns(1, 1);
	
	CSize size = m_wndCalendar.ComputeTotalSize();

	SetWindowPos(NULL, 0, 0, size.cx, size.cy, SWP_NOMOVE);

	return 0;
}

BOOL CFPSDateTimePopupCtrl::OnEraseBkgnd(CDC*) 
{
	return FALSE;
}

void CFPSDateTimePopupCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);	
}

void CFPSDateTimePopupCtrl::OnCalendarClick(NMHDR * , LRESULT *  )
{
	m_dt = m_wndCalendar.GetDate();

	if (m_pwndEdit)
	{
		m_pwndEdit->SetDate(m_dt);
		m_pwndEdit->PostMessage(WM_FPSDATE_HIDEDROPDOWN);
	}
}

void CFPSDateTimePopupCtrl::BeginTracking()
{
	if (!m_bTracking)
	{
		m_bTracking = TRUE;
		m_uiTimerID = SetTimer(1001, 10, NULL);
	}
}

void CFPSDateTimePopupCtrl::EndTracking()
{
	KillTimer(m_uiTimerID);
	m_bTracking = FALSE;
}

void CFPSDateTimePopupCtrl::OnTimer(UINT nIDEvent) 
{
	POINT ptMouse;

	if (GetCursorPos(&ptMouse))
	{
		CRect ClientRect;
		GetClientRect(ClientRect);
		ClientToScreen(ClientRect);

		if (!ClientRect.PtInRect(ptMouse) && HIBYTE(GetAsyncKeyState(VK_LBUTTON)))
		{
			if (m_pwndEdit && ::GetCapture() != GetSafeHwnd() && ::GetCapture() != m_wndCalendar.GetSafeHwnd())
				m_pwndEdit->PostMessage(WM_FPSDATE_HIDEDROPDOWN);
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CFPSDateTimePopupCtrl::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	CWnd::OnNcRButtonDown(nHitTest, point);
}
