/************************************************************************************************
// $Header: /Surveyor/GuiLib/TitleWnd.cpp 3     9/27/02 4:06p Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : TitleWnd.cpp                                                                       */
/*                                                                                              */
/* Purpose : interface for the footer and header windows of the CChartCtrl                      */
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
#include "TitleWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleWnd

CTitleWnd::CTitleWnd() :
    m_dwChartWndExtentLo(0),
    m_dwChartWndExtentHi(0),
    //m_wRightEdge(0),
    m_dwStyle(eNONE) {}

CTitleWnd::~CTitleWnd() {}

BEGIN_MESSAGE_MAP(CTitleWnd, CChartWndBase)
	//{{AFX_MSG_MAP(CTitleWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleWnd message handlers

/************************************************************************************************/
/*                                                                                              */
/* Function: CTitleWnd::CalculateWndHeight()                                                    */
/*                                                                                              */
/* Purpose : calculates the height of the window based on the specified font.                   */
/*                                                                                              */
/* Inputs  : CDC* pDC -> dc to calculate with                                                   */
/*           DWORD* dwHeight -> height of window                                                */
/*                                                                                              */
/* Outputs : BOOL <- FALSE of success                                                           */
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
BOOL CTitleWnd::CalculateWndHeight(CDC* pDC, DWORD* dwHeight) {

    if (!m_pFont)
        ASSERT(FALSE);

    CFont* pOldFont = pDC->GetCurrentFont();

    CRect Rect(0, 0, 0, 0);
    pDC->SelectObject(m_pFont);
    
    pDC->DrawText("X", &Rect, DT_CALCRECT);

    *dwHeight = Rect.Height();
    *dwHeight += T_MARGIN + B_MARGIN;

    pDC->SelectObject(pOldFont);
    
    return FALSE;
    }

void CTitleWnd::OnPaint() {
	
    if (m_pPrintDC)
        return;

    CPaintDC dc(this);

    /****set up the memory DC****/
    CBitmap     memBitmap;
    GetClientRect(&m_rcClientRect);
    CMemDC      memDC(&dc, &m_rcClientRect, &memBitmap);
	/****set up the memory DC****/
    
    Draw(&memDC);

    /****restore the original DC****/
    dc.BitBlt(m_rcClientRect.left, m_rcClientRect.top, m_rcClientRect.Width(), m_rcClientRect.Height(), &memDC, 0, 0, SRCCOPY);
    /****restore the original DC****/

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CTitleWnd::Draw()                                                                  */
/*                                                                                              */
/* Purpose : draws directly to the CMemDC passed in                                             */
/*                                                                                              */
/* Inputs  : CMemDC* pMemDC -> DC to draw to                                                    */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 24SEP02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          24SEP02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CTitleWnd::Draw(CMemDC* pMemDC) {

    GetClientRect(&m_rcClientRect);
    
    pMemDC->FillSolidRect(&m_rcClientRect, m_crWindowColor);

    pMemDC->SelectObject(m_pFont);
    pMemDC->SetTextColor(m_crTextColor);

    CRect Rect(0, 0, 0, 0);

    if (m_sLText!="") {
        pMemDC->DrawText(m_sLText, &Rect, DT_CALCRECT);
        int nYOffset = ((m_rcClientRect.Height() - (T_MARGIN + B_MARGIN)) - Rect.Height())/2;
        Rect.OffsetRect(L_MARGIN, T_MARGIN + nYOffset);
        pMemDC->DrawText(m_sLText, Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        Rect.SetRectEmpty();
        }

    if (m_sMText!="") {
        pMemDC->DrawText(m_sMText, &Rect, DT_CALCRECT);
        int nYOffset = ((m_rcClientRect.Height() - (T_MARGIN + B_MARGIN)) - Rect.Height())/2;
        /*if (m_dwStyle&CENTER_OVER_CHART&&m_wRightEdge)
            Rect.OffsetRect((m_wRightEdge-Rect.Width())/2, T_MARGIN + nYOffset);
        else*/
            Rect.OffsetRect((m_rcClientRect.Width()-Rect.Width())/2, T_MARGIN + nYOffset);
        pMemDC->DrawText(m_sMText, Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        Rect.SetRectEmpty();
        }

    if (m_sRText!="") {
        pMemDC->DrawText(m_sRText, &Rect, DT_CALCRECT);
        int nYOffset = ((m_rcClientRect.Height() - (T_MARGIN + B_MARGIN)) - Rect.Height())/2;
        Rect.OffsetRect(m_rcClientRect.Width()-R_MARGIN-Rect.Width(), T_MARGIN + nYOffset);
        pMemDC->DrawText(m_sRText, Rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        Rect.SetRectEmpty();
        }

    return;
    }

//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/TitleWnd.cpp $
// 
// 3     9/27/02 4:06p Scott.pelger
// if printing we dont do any painting
// 
// 2     9/25/02 8:55a Scott.pelger
// updated chart code to provide image exportation as bmp file
// 
// 1     8/30/02 3:06p Scott.pelger
// initial version
//***********************************************************************************************
