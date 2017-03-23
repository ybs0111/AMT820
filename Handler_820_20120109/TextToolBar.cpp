// TextToolBar.cpp : implementation file
//

#include "stdafx.h"
//#include "Inspection.h"
#include "Resource.h"
#include "TextToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextToolBar

CTextToolBar::CTextToolBar()
{
}

CTextToolBar::~CTextToolBar()
{
}


BEGIN_MESSAGE_MAP(CTextToolBar, CToolBar)
	//{{AFX_MSG_MAP(CTextToolBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextToolBar message handlers

BOOL CTextToolBar::LoadToolBar(UINT nIDResource)
{
    BOOL bReturn = CToolBar::LoadToolBar(nIDResource);

    // Check if we loaded the toolbar.
    if (bReturn == FALSE) return bReturn;

     // Make it flat.
    ModifyStyle(0, GetStyle()|TBSTYLE_FLAT);

     // Set the text for each button
    CToolBarCtrl& bar = GetToolBarCtrl();

    int nIndex = 0;
    TBBUTTON    tb;

    for (nIndex = bar.GetButtonCount(); nIndex >= 0; nIndex--)
    {
         ZeroMemory(&tb, sizeof(TBBUTTON));
         bar.GetButton(nIndex, &tb);

         // Do we have a separator?
         if ((tb.fsStyle & TBSTYLE_SEP) == TBSTYLE_SEP) continue;

         // Have we got a valid command id?
         if (tb.idCommand == 0) continue;

         // Get the resource string if there is one.
         CString strText((LPCSTR)tb.idCommand);
         LPCTSTR lpszButtonText = NULL;

         CString strButtonText(_T(""));
         _TCHAR seps[] = _T("\n");

         if (!strText.IsEmpty())
         {
              lpszButtonText = _tcstok((LPTSTR)(LPCTSTR)strText, seps);
              while(lpszButtonText)
              {
                    strButtonText = lpszButtonText;
                    lpszButtonText = _tcstok(NULL, seps);
              }
         }

         if (!strButtonText.IsEmpty()) SetButtonText(nIndex, strButtonText);
    }

   // Resize the buttons so that the text will fit.
    CRect rc(0, 0, 0, 0);
    CSize sizeMax(0, 0);

    for (nIndex = bar.GetButtonCount(); nIndex >= 0; nIndex--)
    {
        bar.GetItemRect(nIndex, rc);
        rc.NormalizeRect();
        sizeMax.cx = __max(rc.Size().cx, sizeMax.cx);
        sizeMax.cy = __max(rc.Size().cy, sizeMax.cy);
    }
    SetSizes(sizeMax, CSize(32,32));
    return bReturn;
}

