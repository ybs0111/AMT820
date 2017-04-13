// FPSDateTimeButtonCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "FPSDateTimeButtonCtrl.h"

#include "FPSDateTimeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFPSDateTimeButtonCtrl

CFPSDateTimeButtonCtrl::CFPSDateTimeButtonCtrl()
{
	m_pwndEdit = NULL;
}

CFPSDateTimeButtonCtrl::~CFPSDateTimeButtonCtrl()
{
}


BEGIN_MESSAGE_MAP(CFPSDateTimeButtonCtrl, CButton)
	//{{AFX_MSG_MAP(CFPSDateTimeButtonCtrl)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFPSDateTimeButtonCtrl message handlers

void CFPSDateTimeButtonCtrl::OnClicked() 
{
	if (m_pwndEdit)
		m_pwndEdit->ShowDropDown();	
}

int CFPSDateTimeButtonCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_Image.LoadBitmap(IDB_DATEPICKER_BUTTON);
	SetBitmap(m_Image);
	
	return 0;
}

void CFPSDateTimeButtonCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CButton::OnLButtonDown(nFlags, point);
}
