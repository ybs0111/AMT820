/************************************************************************************************
// $Header: /Surveyor/GuiLib/LegendWnd.cpp 3     9/27/02 4:06p Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : LegendWnd.cpp                                                                      */
/*                                                                                              */
/* Purpose : interface for the legend window                                                    */
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
// LegendWnd.cpp : implementation file
//

#include "stdafx.h"
#include "LegendWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLegendWnd

CLegendWnd::CLegendWnd() : m_bShowPointLegend(TRUE), m_yAutoHideWidth(100), m_yAutoHideHeight(100) {}
CLegendWnd::~CLegendWnd() {Purge();}

BEGIN_MESSAGE_MAP(CLegendWnd, CChartWndBase)
	//{{AFX_MSG_MAP(CLegendWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLegendWnd message handlers

void CLegendWnd::OnPaint() {
	
    if (m_pPrintDC)
        return;

    CPaintDC dc(this);
    
	CBitmap MemBitMap;
    GetClientRect(&m_rcClientRect);

    CMemDC MemDC(&dc, &m_rcClientRect, &MemBitMap);
    m_pMemDC = &MemDC;

    Draw(m_pMemDC);

    dc.BitBlt(m_rcClientRect.left, m_rcClientRect.top, m_rcClientRect.Width(), m_rcClientRect.Height(), m_pMemDC, 0, 0, SRCCOPY);
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::Draw()                                                                 */
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
void CLegendWnd::Draw(CMemDC* pMemDC) {

    m_pMemDC = pMemDC;
    
    GetClientRect(&m_rcClientRect);
    
    pMemDC->FillSolidRect(&m_rcClientRect, m_crWindowColor);

    switch (m_eLegendPosition) {
        case eLEFT :
        case eRIGHT :
            _ArrangeVertical(pMemDC);
            break;
        default :
            _ArrangeHorizontal(pMemDC);
        }

    return;
    }

void CLegendWnd::SetAutoHide(BYTE yWidth, BYTE yHeight) {
    
    m_yAutoHideWidth = yWidth>100?100:yWidth;
    m_yAutoHideHeight = yHeight>100?100:yHeight;
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::_ArrangeVertical()                                                     */
/*                                                                                              */
/* Purpose : arranges the legend when this window is taller than it is wider                    */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 17MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          17MAY02     initial version                                            */
/* Scott Pelger          24SEP02     added code to force the legend text to draw to our mem DC  */
/*                                                                                              */
/************************************************************************************************/
void CLegendWnd::_ArrangeVertical(CMemDC* pMemDC) {

    int nWidth(_GetWindowMaxWidth());
    int nXDelta(nWidth/2);
    int nHeight(_GetWindowMaxHeight());
    int nYDelta(nHeight/2);
    
    CPtrArray* pLegendList = _GetLegendList();
    
    int nCnt(pLegendList->GetSize());

    if (!nCnt||m_rcClientRect.IsRectEmpty())
        return;
    
    int nTotalHeight(0);
    int nNumInCol(0);
        
    for (int nCols(1);;nCols++) {
        nNumInCol = nCnt/nCols + nCnt%nCols;
        nTotalHeight = nHeight*nNumInCol + nYDelta*(nNumInCol-1);
        if (nTotalHeight<m_rcClientRect.Height())
            break;
        }
    
    CLegendText* pLegendText;
    CRect Rect;
    
    int nNumInRow(nCnt/nNumInCol + (nCnt%nNumInCol>0));
    int nOffsetX((m_rcClientRect.Width()-nWidth*nNumInRow-nXDelta*(nNumInRow-1))/2);
    int nX(nOffsetX);
    int nOffsetY((m_rcClientRect.Height()-(nHeight*nNumInCol+nYDelta*(nNumInCol-1)))/2);
    int nY(nOffsetY);
    
    for (int l(0),i(0);i<nNumInRow;i++) {
        for (int j(0);j<nNumInCol&&l<nCnt;j++,l++) {
            pLegendText = (CLegendText*)pLegendList->GetAt(l);
        
            if (!pLegendText)
                continue;
        
            pLegendText->GetClientRect(&Rect);
            pLegendText->MoveWindow(nX, nY, Rect.Width(), Rect.Height());
            CBitmap ItemBitmap;
            CRect ItemRect(nX, nY, nX+Rect.Width(), nY+Rect.Height());
            CMemDC MemDC(m_pMemDC, &ItemRect, &ItemBitmap);
            pLegendText->Draw(&MemDC);
            m_pMemDC->BitBlt(ItemRect.left, ItemRect.top, ItemRect.Width(), ItemRect.Height(), &MemDC, 0, 0, SRCCOPY);
            nY += nHeight + nYDelta;
            }
        nX += nWidth + nXDelta;
        nY = nOffsetY;
        }
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::_ArrangeHorizontal()                                                   */
/*                                                                                              */
/* Purpose : arranges the legend when this window is wider than it is taller                    */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 17MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          17MAY02     initial version                                            */
/* Scott Pelger          24SEP02     added code to force the legend text to draw to our mem DC  */
/*                                                                                              */
/************************************************************************************************/
void CLegendWnd::_ArrangeHorizontal(CMemDC* pMemDC) {

    int nWidth(_GetWindowMaxWidth());
    int nXDelta(nWidth/2);
    int nHeight(_GetWindowMaxHeight());
    int nYDelta(nHeight/2);
    
    CPtrArray* pLegendList = _GetLegendList();
    
    int nCnt(pLegendList->GetSize());

    if (!nCnt||m_rcClientRect.IsRectEmpty())
        return;
    
    int nTotalWidth(0);
    int nNumInRow(0);
        
    for (int nRows(1);;nRows++) {
        nNumInRow = nCnt/nRows + nCnt%nRows;
        nTotalWidth = nWidth*nNumInRow + nXDelta*(nNumInRow-1);
        if (nTotalWidth<m_rcClientRect.Width())
            break;
        }
    
    CLegendText* pLegendText;
    CRect Rect;
    
    int nNumInCol(nCnt/nNumInRow + (nCnt%nNumInRow>0));
    int nOffsetX((m_rcClientRect.Width()-nWidth*nNumInRow-nXDelta*(nNumInRow-1))/2);
    int nX(nOffsetX);
    int nOffsetY((m_rcClientRect.Height()-(nHeight*nNumInCol+nYDelta*(nNumInCol-1)))/2);
    int nY(nOffsetY);
    
    for (int l(0),i(0);i<nNumInCol;i++) {
        for (int j(0);j<nNumInRow&&l<nCnt;j++,l++) {
            pLegendText = (CLegendText*)pLegendList->GetAt(l);
        
            if (!pLegendText)
                continue;
        
            pLegendText->GetClientRect(&Rect);
            pLegendText->MoveWindow(nX, nY, Rect.Width(), Rect.Height());
            CBitmap ItemBitmap;
            CRect ItemRect(nX, nY, nX+Rect.Width(), nY+Rect.Height());
            CMemDC MemDC(m_pMemDC, &ItemRect, &ItemBitmap);
            pLegendText->Draw(&MemDC);
            m_pMemDC->BitBlt(ItemRect.left, ItemRect.top, ItemRect.Width(), ItemRect.Height(), &MemDC, 0, 0, SRCCOPY);
            nX += nWidth + nXDelta;
            }
        nY += nHeight + nYDelta;
        nX = nOffsetX;
        }
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::_GetWindowMaxWidth()                                                   */
/*                                                                                              */
/* Purpose : Returns the maximum window width                                                   */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : int <- width of the window                                                         */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 17MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          17MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
int CLegendWnd::_GetWindowMaxWidth() {
    
    CPtrArray* pLegendList = _GetLegendList();
    
    CLegendText* pLegendText;
    CRect Rect;
    
    int nCnt(pLegendList->GetSize());
    int nWidest(0);
    int nWidth;
    
    for (int i(0);i<nCnt;i++) {
        pLegendText = (CLegendText*)pLegendList->GetAt(i);
        
        if (!pLegendText)
            continue;
        
        pLegendText->RecalculateSize(m_pPrintDC);
        
        pLegendText->GetClientRect(&Rect);
        nWidth = Rect.Width();
        nWidest = nWidest>nWidth?nWidest:nWidth;
        }

    return nWidest;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::_GetWindowMaxHeight()                                                  */
/*                                                                                              */
/* Purpose : Returns the maximum window height                                                  */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : int <- height of the window                                                        */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 17MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          17MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
int CLegendWnd::_GetWindowMaxHeight() {
    
    CPtrArray* pLegendList = _GetLegendList();
    
    CLegendText* pLegendText;
    CRect Rect;
    
    if (!pLegendList->GetSize())
        return 0;
    
    pLegendText = (CLegendText*)pLegendList->GetAt(0);
    
    if (!pLegendText)
        return 0;
    
    pLegendText->RecalculateSize(m_pPrintDC);
    
    pLegendText->GetClientRect(&Rect);
    
    return Rect.Height();
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::GetMinimumWindowRect()                                                 */
/*                                                                                              */
/* Purpose : Returns the minimum window dimesnions to show the entire legend                    */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : CRect* <- minimunm rect                                                            */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 17MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          17MAY02     initial version                                            */
/* Scott Pelger          24JUL02     added condition to bail out of the for loops               */
/*                                                                                              */
/************************************************************************************************/
void CLegendWnd::GetMinimumWindowRect(CRect* pRect) {

    int nWidth(_GetWindowMaxWidth());
    int nXDelta(nWidth/2);
    int nHeight(_GetWindowMaxHeight());
    int nYDelta(nHeight/2);
    
    CPtrArray* pLegendList = _GetLegendList();

    int nCnt(pLegendList->GetSize());

    if (!nCnt||m_rcClientRect.IsRectEmpty())
        return;
    
    int nTotalWidth(0);
    int nTotalHeight(0);

    switch (m_eLegendPosition) {
        case eLEFT :
        case eRIGHT : {
            int nNumInCol(0);

            for (int nCols(1);nCols<=nCnt;nCols++) {
                nNumInCol = nCnt/nCols + nCnt%nCols;
                nTotalHeight = nHeight*nNumInCol + nYDelta*(nNumInCol-1);
                if (nTotalHeight<m_rcClientRect.Height()) {
                    int nNumInRow(nCnt/nNumInCol + (nCnt%nNumInCol>0));
                    nTotalWidth = nWidth*nNumInRow + nXDelta*(nNumInRow-1);
                    break;
                    }
                }
            if (nTotalWidth>m_rcClientRect.Width()*m_yAutoHideWidth/100)
                nTotalHeight = nTotalWidth = 0;
            }
            break;
        default : {
            int nNumInRow(0);
            for (int nRows(1);nRows<=nCnt;nRows++) {
                nNumInRow = nCnt/nRows + nCnt%nRows;
                nTotalWidth = nWidth*nNumInRow + nXDelta*(nNumInRow-1);
                if (nTotalWidth<m_rcClientRect.Width()) {
                    int nNumInCol(nCnt/nNumInRow + (nCnt%nNumInRow>0));
                    nTotalHeight = nHeight*nNumInCol + nYDelta*(nNumInCol-1);
                    break;
                    }
                }
            if (nTotalHeight>m_rcClientRect.Height()*m_yAutoHideHeight/100)
                nTotalHeight = nTotalWidth = 0;
            }
        }

    pRect->SetRect(0, 0, nTotalWidth, nTotalHeight);
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::SetFont()                                                              */
/*                                                                                              */
/* Purpose : sets the font for this window                                                      */
/*                                                                                              */
/* Inputs  : BYTE byPointSize -> size of the font                                               */
/*           char* psFontName -> name of the font                                               */
/*           BYTE byStyle -> styles to be applied to the font                                   */
/*                                                                                              */
/* Outputs : see CChartWndBase::SetFont()                                                       */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 15MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          15MAY02     initial version                                            */
/* Scott Pelger          02JAN03     telling the legend text windows of the new font            */
/*                                                                                              */
/************************************************************************************************/
BOOL CLegendWnd::SetFont(BYTE yPointSize, char* psFontName, BYTE byStyle/*=0*/) {

    m_yPointSize = yPointSize;
    m_sFontName = *psFontName;

    int nCnt(m_PointArray.GetSize());

    CLegendText* pWnd;
    for (int i(0);i<nCnt;i++) {
        pWnd = (CLegendText*)m_PointArray.GetAt(i);
        pWnd->SetFont(yPointSize, psFontName, byStyle);
        }

    nCnt = m_SeriesArray.GetSize();
    
    for (i=0;i<nCnt;i++) {
        pWnd = (CLegendText*)m_SeriesArray.GetAt(i);
        pWnd->SetFont(yPointSize, psFontName, byStyle);
        }

    return CChartWndBase::SetFont(yPointSize, psFontName, byStyle);
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::SetWindowColor()                                                       */
/*                                                                                              */
/* Purpose : sets the window color for the legend window                                        */
/*                                                                                              */
/* Inputs  : COLORREF Color -> sets the color of the window as well as the background color for */
/*           all legend items                                                                   */
/*                                                                                              */
/* Outputs : see CChartWndBase::SetWindowColor()                                                */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 15MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          15MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
COLORREF CLegendWnd::SetWindowColor(COLORREF Color) {

    m_crWindowColor = Color;
    
    int nCnt(m_PointArray.GetSize());

    CLegendText* pWnd;
    for (int i(0);i<nCnt;i++) {
        pWnd = (CLegendText*)m_PointArray.GetAt(i);
        pWnd->SetBackgroundColor(Color);
        }

    nCnt = m_SeriesArray.GetSize();
    
    for (i=0;i<nCnt;i++) {
        pWnd = (CLegendText*)m_SeriesArray.GetAt(i);
        pWnd->SetBackgroundColor(Color);
        }

    return CChartWndBase::SetWindowColor(Color);
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::SetLegendTextColor()                                                   */
/*                                                                                              */
/* Purpose : sets the legend text color                                                         */
/*                                                                                              */
/* Inputs  : COLORREF Color -> sets the color of the legend text                                */
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
void CLegendWnd::SetLegendTextColor(COLORREF Color) {

    m_crTextColor = Color;

    int nCnt(m_PointArray.GetSize());

    CLegendText* pWnd;
    for (int i(0);i<nCnt;i++) {
        pWnd = (CLegendText*)m_PointArray.GetAt(i);
        pWnd->SetTextColor(Color);
        }

    nCnt = m_SeriesArray.GetSize();
    
    for (i=0;i<nCnt;i++) {
        pWnd = (CLegendText*)m_SeriesArray.GetAt(i);
        pWnd->SetTextColor(Color);
        }

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::SetLegendSeriesItemColor()                                             */
/*                                                                                              */
/* Purpose : sets the legend series item color                                                  */
/*                                                                                              */
/* Inputs  : COLORREF Color -> sets the color of the legend item                                */
/*           int nDataSeries -> the data series whose color is to be set                        */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 28JUN02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          28JUN02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CLegendWnd::SetLegendSeriesItemColor(COLORREF Color, int nDataSeries/*=0*/) {

    int nCnt(m_SeriesArray.GetSize());

    if (nDataSeries+1>nCnt)
        return;

    CLegendText* pWnd = (CLegendText*)m_SeriesArray.GetAt(nDataSeries);
    pWnd->SetItemColor(Color);

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::GetLegendSeriesItemColor()                                             */
/*                                                                                              */
/* Purpose : gets the legend series item color                                                  */
/*                                                                                              */
/* Inputs  : int nDataSeries -> the data series whose color is to be retrieved                  */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 28JUN02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          28JUN02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
COLORREF CLegendWnd::GetLegendSeriesItemColor(int nDataSeries/*=0*/) {

    int nCnt(m_SeriesArray.GetSize());

    if (nDataSeries+1>nCnt)
        return 0;

    CLegendText* pWnd = (CLegendText*)m_SeriesArray.GetAt(nDataSeries);
    
    return pWnd->GetItemColor();
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::SetLegendPointItemColor()                                              */
/*                                                                                              */
/* Purpose : sets the legend point item color                                                   */
/*                                                                                              */
/* Inputs  : COLORREF Color -> sets the color of the legend item                                */
/*           int nDataPoint -> the data series whose color is to be set                         */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 28JUN02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          28JUN02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CLegendWnd::SetLegendPointItemColor(COLORREF Color, int nDataPoint/*=0*/) {

    int nCnt(m_PointArray.GetSize());

    if (nDataPoint+1>nCnt)
        return;

    CLegendText* pWnd = (CLegendText*)m_PointArray.GetAt(nDataPoint);
    pWnd->SetItemColor(Color);

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::GetLegendPointItemColor()                                              */
/*                                                                                              */
/* Purpose : gets the legend point item color                                                   */
/*                                                                                              */
/* Inputs  : int nDataPoint -> the data point whose color is to be retrieved                    */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 28JUN02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          28JUN02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
COLORREF CLegendWnd::GetLegendPointItemColor(int nDataPoint/*=0*/) {

    int nCnt(m_PointArray.GetSize());

    if (nDataPoint+1>nCnt)
        return 0;

    CLegendText* pWnd = (CLegendText*)m_PointArray.GetAt(nDataPoint);
    
    return pWnd->GetItemColor();
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::_CreateNewLegendItem()                                                 */
/*                                                                                              */
/* Purpose : creates a new legend item                                                          */
/*                                                                                              */
/* Inputs  : CString& Caption -> the caption for this item                                      */
/*           COLORREF Color -> the color for this item                                          */
/*                                                                                              */
/* Outputs : CLegendText* <- pointer to the new legend item                                     */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 15MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          15MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
CLegendText* CLegendWnd::_CreateNewLegendItem(CString& Caption, COLORREF Color) {

    CLegendText* m_wndLegendText = new CLegendText;
    
    m_wndLegendText->Create(Caption, WS_CHILD|WS_VISIBLE|SS_CENTER, CRect(0, 0, 0, 0), this);
    m_wndLegendText->SetFont(m_yPointSize, m_sFontName.GetBuffer(0));
    m_wndLegendText->SetBackgroundColor(m_crWindowColor);
    m_wndLegendText->SetBorderColor(m_crTextColor);
    m_wndLegendText->SetTextColor(m_crTextColor);
    m_wndLegendText->SetItemColor(Color);
    m_wndLegendText->SetItemStyle(SHP_SQUARE);

    return m_wndLegendText;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::AddPointItem()                                                         */
/*                                                                                              */
/* Purpose : adds a legend item to the m_PointArray pointer list                                */
/*                                                                                              */
/* Inputs  : CString& Caption -> the caption for this item                                      */
/*           COLORREF Color -> the color for this item                                          */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 15MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          15MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CLegendWnd::AddPointItem(CString& Caption, COLORREF Color) {
    
    m_PointArray.Add(_CreateNewLegendItem(Caption, Color));
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::RemovePointItem()                                                      */
/*                                                                                              */
/* Purpose : removes a legend item to the m_PointArray pointer list                             */
/*                                                                                              */
/* Inputs  : int nItem -> the item to remove                                                    */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 15MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          15MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CLegendWnd::RemovePointItem(int nItem) {
    
    CLegendText* pWnd = (CLegendText*)m_PointArray.GetAt(nItem);
    pWnd->DestroyWindow();
    delete pWnd;
    m_PointArray.RemoveAt(nItem);
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::AddSeriesItem()                                                        */
/*                                                                                              */
/* Purpose : adds a legend item to the m_SeriesArray pointer list                               */
/*                                                                                              */
/* Inputs  : CString& Caption -> the caption for this item                                      */
/*           COLORREF Color -> the color for this item                                          */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 15MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          15MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CLegendWnd::AddSeriesItem(CString& Caption, COLORREF Color) {
    
    m_SeriesArray.Add(_CreateNewLegendItem(Caption, Color));
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::AddSeriesItem()                                                        */
/*                                                                                              */
/* Purpose : removes a legend item to the m_SeriesArray pointer list                            */
/*                                                                                              */
/* Inputs  : int nItem -> the item to remove                                                    */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 15MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          15MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CLegendWnd::RemoveSeriesItem(int nItem) {
    
    CLegendText* pWnd = (CLegendText*)m_SeriesArray.GetAt(nItem);
    pWnd->DestroyWindow();
    delete pWnd;
    m_SeriesArray.RemoveAt(nItem);
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::Purge()                                                                */
/*                                                                                              */
/* Purpose : removes all legend items from both legend arrays                                   */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 15MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          15MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CLegendWnd::Purge() {
    
    int nCnt(m_PointArray.GetSize());

    CLegendText* pWnd;
    for (int i(0);i<nCnt;i++) {
        pWnd = (CLegendText*)m_PointArray.GetAt(i);
        pWnd->DestroyWindow();
        delete pWnd;
        }

    m_PointArray.RemoveAll();
    
    nCnt = m_SeriesArray.GetSize();
    
    for (i=0;i<nCnt;i++) {
        pWnd = (CLegendText*)m_SeriesArray.GetAt(i);
        pWnd->DestroyWindow();
        delete pWnd;
        }

    m_SeriesArray.RemoveAll();

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CLegendWnd::_GetLegendList()                                                       */
/*                                                                                              */
/* Purpose : returns the specified legend list                                                  */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : CPtrArray* <- the desired legend list                                              */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 17MAY02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          17MAY02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
CPtrArray* CLegendWnd::_GetLegendList() {
    
    if (m_bShowPointLegend) 
        return &m_PointArray; 
    else 
        return &m_SeriesArray;
    
    return NULL;
    }

void CLegendWnd::SetPrinting(CMemDC* pPrintDC/*=NULL*/) {

    CChartWndBase::SetPrinting(pPrintDC);
    
    int nCnt(m_PointArray.GetSize());

    CLegendText* pWnd;
    for (int i(0);i<nCnt;i++) {
        pWnd = (CLegendText*)m_PointArray.GetAt(i);
        pWnd->SetPrinting(pPrintDC);
        }

    nCnt = m_SeriesArray.GetSize();
    
    for (i=0;i<nCnt;i++) {
        pWnd = (CLegendText*)m_SeriesArray.GetAt(i);
        pWnd->SetPrinting(pPrintDC);
        }

    return;
    }

//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/LegendWnd.cpp $
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
