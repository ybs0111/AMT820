/************************************************************************************************
// $Header: /Surveyor/GuiLib/PopupWindow.cpp 1     8/30/02 3:06p Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : PopupWindow.cpp                                                                    */
/*                                                                                              */
/* Purpose : interface for the popup window which appears during mouse movements                */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 10JUN02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          10JUN02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
#include "stdafx.h"
#include "PopupWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern COLORREF SubdueColor(COLORREF Color);
#define INFLATE_X 4
#define INFLATE_Y 2

BEGIN_MESSAGE_MAP(CPopupWindow, CChartWndBase)
	//{{AFX_MSG_MAP(CPopupWindow)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/************************************************************************************************/
/*                                                                                              */
/* Function: CPopupWindow::SetWindowText()                                                      */
/*                                                                                              */
/* Purpose : sets the window text and readjusts teh window rect                                 */
/*                                                                                              */
/* Inputs  : CString& WindowText -> text to set                                                 */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 11JUN02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          11JUN02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CPopupWindow::SetWindowText(CString& WindowText) {

    //flush the window area
    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE);

    CWindowDC DC(this);
    CFont* pFont = DC.GetCurrentFont();
    CRect WindowRect(0, 0, 0, 0);

    DC.SelectObject(m_pFont);
    DC.DrawText(WindowText, &WindowRect, DT_CALCRECT);
    DC.SelectObject(pFont);

    WindowRect.InflateRect(INFLATE_X, INFLATE_Y);
    SetWindowPos(NULL, 0, 0, WindowRect.Width(), WindowRect.Height(), SWP_NOMOVE);

    CChartWndBase::SetWindowText(WindowText);

    return;
    }

void CPopupWindow::OnPaint() {
	
    CPaintDC DC(this);

    //first, save the original fore/back color, pen, and font
    CPen* pPen      = DC.GetCurrentPen();
    CFont* pFont    = DC.GetCurrentFont();
    COLORREF crBG   = DC.GetBkColor();
    COLORREF crTX   = DC.GetTextColor();

    CRect WindowRect;
    
    GetClientRect(&WindowRect);

    //fill the rect with the desired color...
    DC.FillSolidRect(&WindowRect, m_crWindowColor);
    DC.SetBkColor(m_crWindowColor);
    DC.SetTextColor(m_crTextColor);

    CString WindowText;
    CChartWndBase::GetWindowText(WindowText);
    
    CRect WindowRectCopy(WindowRect);
    WindowRect.OffsetRect(0, INFLATE_Y-1);
    
    DC.SelectObject(m_pFont);
    DC.DrawText(WindowText, &WindowRect, DT_CENTER);

    //draw the beveled edge...
    CPen Pen(PS_SOLID, 1, RGB(0, 0, 0));
    DC.SelectObject(&Pen);
    DC.MoveTo(WindowRectCopy.left, WindowRectCopy.bottom-1);
    DC.LineTo(WindowRectCopy.right-1, WindowRectCopy.bottom-1);
    DC.LineTo(WindowRectCopy.right-1, WindowRectCopy.top-1);
    Pen.DeleteObject();
    Pen.CreatePen(PS_SOLID, 1, SubdueColor(m_crWindowColor));
    DC.SelectObject(&Pen);
    DC.MoveTo(WindowRectCopy.right-2, WindowRectCopy.top+1);
    DC.LineTo(WindowRectCopy.right-2, WindowRectCopy.bottom-2);
    DC.LineTo(WindowRectCopy.left, WindowRectCopy.bottom-2);
    Pen.DeleteObject();
    Pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    DC.SelectObject(&Pen);
    DC.MoveTo(WindowRectCopy.left+1, WindowRectCopy.bottom-3);
    DC.LineTo(WindowRectCopy.left+1, WindowRectCopy.top+1);
    DC.LineTo(WindowRectCopy.right-2, WindowRectCopy.top+1);
    
    //lastly, restore the original fore/back color, pen, and font
    DC.SetBkColor(crBG);
    DC.SetTextColor(crTX);
    DC.SelectObject(pPen);
    DC.SelectObject(pFont);
    
    return;
    }

//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/PopupWindow.cpp $
// 
// 1     8/30/02 3:06p Scott.pelger
// initial version
//***********************************************************************************************

