/************************************************************************************************
// $Header: /Surveyor/GuiLib/Shape.cpp 3     10/30/02 10:23a Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : Shape.cpp                                                                          */
/*                                                                                              */
/* Purpose : interface for the shape that appears next to the legend text                       */
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
#include "Shape.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CShape::CShape() : m_crBackgroundColor(GetSysColor(COLOR_BTNFACE)) {};

BEGIN_MESSAGE_MAP(CShape, CWnd)
	//{{AFX_MSG_MAP(CShape)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShape message handlers

void CShape::OnPaint() {
	
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
/* Function: CShape::Draw()                                                                     */
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
/* Scott Pelger          25SEP02     needed to fill the background color of the entire client rect */
/*                                                                                              */
/************************************************************************************************/
void CShape::Draw(CMemDC* pMemDC) {

    CRect Rect;
    GetClientRect(&Rect);

    int nHeight = Rect.Height();
    int nWidth = Rect.Width();
    int nSquare = min(nHeight, nWidth);

    int nOffset(0);

    if (nSquare<nWidth)
        nOffset = (Rect.Width()-nSquare)/2;
    
    CPen Pen;
    switch (m_byLineStyle) {
        case SHP_NONE :
            break;
        case SHP_SOLID :
            Pen.CreatePen(PS_SOLID, 1, m_crBorderColor);
            break;
        case SHP_DASH :
            Pen.CreatePen(PS_DASH, 1, m_crBorderColor);
            break;
        case SHP_DOT :
            Pen.CreatePen(PS_DOT, 1, m_crBorderColor);
            break;
        case SHP_DASHDOT :
            Pen.CreatePen(PS_DASHDOT, 1, m_crBorderColor);
            break;
        case SHP_DASHDOTDOT :
            Pen.CreatePen(PS_DASHDOTDOT, 1, m_crBorderColor);
            break;
        default :
            ASSERT(FALSE);
        }

    CPen* pPen = pMemDC->SelectObject(&Pen);
    COLORREF crBackColor = pMemDC->SetBkColor(m_crBackgroundColor);
    pMemDC->FillSolidRect(&Rect, m_crBackgroundColor);
    
    if (m_byLineStyle!=SHP_NONE) {
        
        pMemDC->MoveTo(0, Rect.Height()/2);
        pMemDC->LineTo(Rect.Width()-1, Rect.Height()/2);
        pMemDC->SetBkColor(crBackColor);
        }
    
    Pen.DeleteObject();

    CBrush Brush(m_crItemColor);
    Pen.CreatePen(PS_SOLID, 1, m_crBorderColor);
    CBrush* pBrush = pMemDC->SelectObject(&Brush);
    pMemDC->SelectObject(&Pen);
    
    switch (m_byItemStyle) {
        case SHP_NONE :
            break;
        case SHP_SQUARE : {
            POINT Points[4];
            
            Points[0].x = nOffset;
            Points[0].y = 0;
            
            Points[1].x = nOffset;
            Points[1].y = nSquare-1;
            
            Points[2].x = nOffset+nSquare-1;
            Points[2].y = nSquare-1;
            
            Points[3].x = nOffset+nSquare-1;
            Points[3].y = 0;
            
            pMemDC->Polygon(Points, 4);
            
            break;
            }
        case SHP_CIRCLE :
            pMemDC->Ellipse(nOffset, 0, nOffset+nSquare, nSquare);
            break;
        case SHP_TRIANGLE : {
            POINT Points[3];
            
            Points[0].x = nOffset+nSquare/2-1;
            Points[0].y = 0;
            
            Points[1].x = nOffset;
            Points[1].y = nSquare-1;
            
            Points[2].x = nOffset+nSquare-2;
            Points[2].y = nSquare-1;
            
            pMemDC->Polygon(Points, 3);
            
            break;
            }
        case SHP_DIAMOND : {
            POINT Points[4];
            
            Points[0].x = nOffset+nSquare/2-1;
            Points[0].y = 1;
            
            Points[1].x = nOffset;
            Points[1].y = nSquare/2;
            
            Points[2].x = nOffset+nSquare/2-1;
            Points[2].y = nSquare-1;
            
            Points[3].x = nOffset+nSquare-2;
            Points[3].y = nSquare/2;
            
            pMemDC->Polygon(Points, 4);
            
            break;
            }
        case SHP_CROSS : {
            POINT Points[12];
            
            Points[0].x = nOffset+(nSquare-1)/3;
            Points[0].y = 0;
            
            Points[1].x = nOffset+(nSquare-1)/3;
            Points[1].y = (nSquare-1)/3;
            
            Points[2].x = nOffset;
            Points[2].y = (nSquare-1)/3;
            
            Points[3].x = nOffset;
            Points[3].y = (nSquare-1)*2/3;
            
            Points[4].x = nOffset+(nSquare-1)/3;
            Points[4].y = (nSquare-1)*2/3;
            
            Points[5].x = nOffset+(nSquare-1)/3;
            Points[5].y = nSquare-1;
            
            Points[6].x = nOffset+(nSquare-1)*2/3;
            Points[6].y = nSquare-1;
            
            Points[7].x = nOffset+(nSquare-1)*2/3;
            Points[7].y = (nSquare-1)*2/3;
            
            Points[8].x = nOffset+nSquare-1;
            Points[8].y = (nSquare-1)*2/3;
            
            Points[9].x = nOffset+nSquare-1;
            Points[9].y = (nSquare-1)/3;
            
            Points[10].x = nOffset+(nSquare-1)*2/3;
            Points[10].y = (nSquare-1)/3;
            
            Points[11].x = nOffset+(nSquare-1)*2/3;
            Points[11].y = 0;
            
            pMemDC->Polygon(Points, 12);
            
            break;
            }
        default:
            ASSERT(FALSE);
        }

    pMemDC->SelectObject(pBrush);
    pMemDC->SelectObject(pPen);

    return;
    }

void CShape::SetPrinting(CMemDC* pPrintDC/*=NULL*/) {

    m_pPrintDC = pPrintDC;

    return;
    }

//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/Shape.cpp $
// 
// 3     10/30/02 10:23a Scott.pelger
// added code that was developed during GTX development
// 
// 2     10/29/02 4:29p Scott.pelger
// copied over version from Surveyor's GuiLib, made one color init
// enhancement
// 
// 2     9/25/02 8:55a Scott.pelger
// updated chart code to provide image exportation as bmp file
// 
// 1     8/30/02 3:06p Scott.pelger
// initial version
//***********************************************************************************************
