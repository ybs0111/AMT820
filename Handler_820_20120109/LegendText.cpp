/************************************************************************************************
// $Header: /Surveyor/GuiLib/LegendText.cpp 3     10/30/02 10:23a Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : LegendText.cpp                                                                     */
/*                                                                                              */
/* Purpose : interface for the legend text                                                      */
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
// LegendText.cpp : implementation file
//

#include "stdafx.h"
#include "LegendText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLegendText

CLegendText::CLegendText() :
    m_rcClientRect(0, 0, 0, 0),
    m_pFont(NULL),
    m_byTextStyle(LT_NORMAL),
    m_byItemStyle(SHP_SQUARE),
    m_byLineStyle(SHP_NONE),
    m_bCreate(FALSE),
    m_crTextColor(0),
    m_crBackgroundColor(GetSysColor(COLOR_BTNFACE)) {
        
    m_wndShape.SetItemColor(RGB(255, 0, 0));
	m_wndShape.SetBorderColor(RGB(255, 255, 0));
	m_wndShape.SetLineColor(RGB(255, 255, 0));
    m_wndShape.SetItemStyle(SHP_SQUARE);
    m_wndShape.SetLineStyle(SHP_NONE);
    
    return;
    }

CLegendText::~CLegendText() {
    
    if (m_pFont)
        delete m_pFont;

    return;
    }

BEGIN_MESSAGE_MAP(CLegendText, CChartWndBase)
	//{{AFX_MSG_MAP(CLegendText)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLegendText message handlers

BOOL CLegendText::SetFont(BYTE byPointSize, char *psFontName, BYTE byStyle) {
    
    if (m_pFont)
        m_pFont->DeleteObject();
    
    if (!m_pFont) {
        m_pFont = new CFont;
        if (!m_pFont)
            return TRUE;
        }
    
    LOGFONT lf;
    
    memset(&lf, 0, sizeof(LOGFONT));
        
    strcpy(lf.lfFaceName, psFontName);

    byPointSize = byPointSize<10?10:byPointSize;

    lf.lfHeight     = byPointSize*10;
    lf.lfWeight     = byStyle&LT_BOLD?FW_BOLD:FW_NORMAL;
    lf.lfItalic     = byStyle&LT_ITALIC;
    lf.lfUnderline  = byStyle&LT_UNDERLINE;
  
    CClientDC dc(this);

    m_pFont->CreatePointFontIndirect(&lf, &dc);

    return FALSE;
    }

COLORREF CLegendText::SetItemColor(COLORREF crColor) {

    COLORREF crTempColor = m_crItemColor;
    m_crItemColor = crColor;

    m_wndShape.SetItemColor(crColor);
    
    return crTempColor;
    }

COLORREF CLegendText::SetBorderColor(COLORREF crColor) {

    COLORREF crTempColor = m_crBorderColor;
    m_crBorderColor = crColor;

    m_wndShape.SetBorderColor(crColor);
    
    return crTempColor;
    }

COLORREF CLegendText::SetLineColor(COLORREF crColor) {

    COLORREF crTempColor = m_crLineColor;
    m_crLineColor = crColor;

    m_wndShape.SetLineColor(crColor);
    
    return crTempColor;
    }

COLORREF CLegendText::SetTextColor(COLORREF crColor) {

    COLORREF crTempColor = m_crTextColor;
    m_crTextColor = crColor;

    m_wndShape.SetBorderColor(crColor);

    return crTempColor;
    }

COLORREF CLegendText::SetBackgroundColor(COLORREF crColor) {

    COLORREF crTempColor = m_crBackgroundColor;
    m_crBackgroundColor = crColor;
    
    m_wndShape.SetBackgroundColor(crColor);
    
    return crTempColor;
    }

void CLegendText::OnPaint() {
	
    CPaintDC dc(this);

    /****set up the memory DC****/
    CBitmap     memBitmap;
    CRect       ClientRect;
    GetClientRect(&ClientRect);
    CMemDC      memDC(&dc, &ClientRect, &memBitmap);
	/****set up the memory DC****/
    
    Draw(&memDC);

    /****restore the original DC****/
    dc.BitBlt(ClientRect.left, ClientRect.top, ClientRect.Width(), ClientRect.Height(), &memDC, 0, 0, SRCCOPY);
    /****restore the original DC****/

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendText::Draw()                                                                */
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
void CLegendText::Draw(CMemDC* pMemDC) {

    pMemDC->SetTextColor(m_crTextColor);
    pMemDC->SetBkColor(m_crBackgroundColor);
    if (m_pPrintDC) {
        CFont* pFont = m_pPrintDC->GetCurrentFont();
        pMemDC->SelectObject(pFont);
        }
    else
        pMemDC->SelectObject(m_pFont);

    pMemDC->DrawText(m_LegendText, -1, &m_rcClientRect, DT_CALCRECT);
    pMemDC->FillSolidRect(&m_rcClientRect, m_crBackgroundColor);

    int nSquare(m_rcClientRect.Height()/2);
    int nSpacing(m_pPrintDC?m_rcClientRect.Height()/4:ITEM_SPACING);
    
    SetWindowPos(NULL, 0, 0, m_rcClientRect.Width()+nSquare+nSpacing, m_rcClientRect.Height(), SWP_NOZORDER|SWP_NOMOVE);
    
    //need to set the position of the shape window
    m_wndShape.SetWindowPos(NULL, 0, (m_rcClientRect.Height()-nSquare)/2, nSquare, nSquare, SWP_NOZORDER);
    CBitmap ItemBitmap;
    CRect ItemRect(0, (m_rcClientRect.Height()-nSquare)/2, nSquare, (m_rcClientRect.Height()-nSquare)/2+nSquare);
    CMemDC MemDC(pMemDC, &ItemRect, &ItemBitmap);
    m_wndShape.Draw(&MemDC);
    pMemDC->BitBlt(ItemRect.left, ItemRect.top, ItemRect.Width(), ItemRect.Height(), &MemDC, 0, 0, SRCCOPY);

    m_rcClientRect.OffsetRect(nSquare+nSpacing, 0);
    
    pMemDC->DrawText(m_LegendText, -1, &m_rcClientRect, DT_LEFT|DT_VCENTER);

    m_rcClientRect.left -= nSquare+nSpacing;
    
    return;
    }

BOOL CLegendText::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {

    m_bCreate = TRUE;
    
    BOOL bReturn(CChartWndBase::Create("STATIC", "", dwStyle, rect, pParentWnd, nID));

    if (bReturn)
        m_wndShape.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 1010);    

    m_LegendText = "Afghjpqy"; //this text should cover the max height of normal chars in USA

    _CalculateClientRect();
    
    m_LegendText = lpszText;

    _CalculateClientRect();
    
    return bReturn;
    }

void CLegendText::SetPrinting(CMemDC* pPrintDC/*=NULL*/) {

    CChartWndBase::SetPrinting(pPrintDC);
    m_wndShape.SetPrinting(pPrintDC);

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendText::_CalculateClientRect()                                                */
/*                                                                                              */
/* Purpose : calculates the client rect based on the current text and font                      */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 29OCT02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          29OCT02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CLegendText::_CalculateClientRect() {

    CClientDC dc(this);
	
    CFont* pFont = dc.SelectObject(m_pFont);

    dc.DrawText(m_LegendText, -1, &m_rcClientRect, DT_CALCRECT);

    int nSquare(m_rcClientRect.Height()/2);
    int nSpacing(m_pPrintDC?m_rcClientRect.Height()/4:ITEM_SPACING);
    
    m_rcClientRect.right += nSquare+nSpacing;
    
    dc.SelectObject(pFont);

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendText::SetWindowText()                                                       */
/*                                                                                              */
/* Purpose : sets the window text and recalculates the client rect                              */
/*                                                                                              */
/* Inputs  : char* pText -> new window text                                                     */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 29OCT02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          29OCT02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CLegendText::SetWindowText(char* pText) {

    m_LegendText = pText;
    
    _CalculateClientRect();
    
    return;
    }

void CLegendText::RecalculateSize(CMemDC* pMemDC/*=NULL*/) {

    if (!pMemDC) {
        _CalculateClientRect();
        return;
        }

    pMemDC->DrawText(m_LegendText, -1, &m_rcClientRect, DT_CALCRECT);
    
    int nSquare(m_rcClientRect.Height()/2);
    int nSpacing(m_pPrintDC?m_rcClientRect.Height()/4:ITEM_SPACING);
    
    m_rcClientRect.right += nSquare+nSpacing;
    
    return;
    }

//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/LegendText.cpp $
// 
// 3     10/30/02 10:23a Scott.pelger
// added code that was developed during GTX development
// 
// 2     10/29/02 4:29p Scott.pelger
// copied over version from Surveyor's GuiLib, made color init
// enhancements, added SetWindowText()
// 
// 2     9/25/02 8:55a Scott.pelger
// updated chart code to provide image exportation as bmp file
// 
// 1     8/30/02 3:06p Scott.pelger
// initial version
//***********************************************************************************************