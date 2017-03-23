// FPSDatePickerCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "handler.h"
#include "FPSDatePickerCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "OleDateTimeEx.h"

/////////////////////////////////////////////////////////////////////////////
// CFPSDatePickerCtrl

CFPSDatePickerCtrl::CFPSDatePickerCtrl()
{
}

CFPSDatePickerCtrl::~CFPSDatePickerCtrl()
{
}


BEGIN_MESSAGE_MAP(CFPSDatePickerCtrl, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CFPSDatePickerCtrl)
	ON_NOTIFY_REFLECT(DTN_USERSTRING, OnUserstring)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// subclass a control on the parent
BOOL CFPSDatePickerCtrl::AttachEdit(CWnd *pwndParent, UINT uiID)
{
	BOOL bReturn = TRUE;

	if (pwndParent && ::IsWindow(pwndParent->GetSafeHwnd()))
	{
		CWnd* pwndChild = pwndParent->GetDlgItem(uiID);
		
		if (pwndChild && ::IsWindow(pwndChild->GetSafeHwnd()))
		{
			CDateTimeCtrl::SubclassDlgItem(uiID, pwndParent);
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
// CFPSDatePickerCtrl message handlers

void CFPSDatePickerCtrl::OnUserstring(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMDATETIMESTRING* pDateInfo = (NMDATETIMESTRING*)pNMHDR;

	CString strInput = pDateInfo->pszUserString;
	
	COleDateTimeEx dtTemp;

	if (dtTemp.ParseDateTime(strInput))
	{
		pDateInfo->dwFlags = GDT_VALID;
		dtTemp.GetAsSystemTime(pDateInfo->st);
	}
	else
	{
		pDateInfo->dwFlags = GDT_NONE;
	}

	*pResult = 0;
}

