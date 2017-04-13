/************************************************************************************************
// $Header: /Surveyor/GuiLib/Shape.h 3     10/30/02 10:23a Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : Shape.h                                                                            */
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
#if !defined(AFX_SHAPE_H__468F4ED4_D36F_11D5_ACA9_00609796948F__INCLUDED_)
#define AFX_SHAPE_H__468F4ED4_D36F_11D5_ACA9_00609796948F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemDC.h"

typedef enum {
    SHP_SQUARE,
    SHP_CIRCLE,
    SHP_TRIANGLE,
    SHP_DIAMOND,
    SHP_CROSS,
    
    SHP_NONE,
    SHP_SOLID,
    SHP_DASH,
    SHP_DOT,
    SHP_DASHDOT,
    SHP_DASHDOTDOT,
    };

#define ITEM_WIDTH      10
#define ITEM_HEIGHT     10

class AFX_EXT_CLASS CShape : public CWnd {
    // Construction
    public:
        CShape();

    // Attributes
    public:
    protected:
        COLORREF    m_crItemColor;
        COLORREF    m_crBorderColor;
        COLORREF    m_crLineColor;
        COLORREF    m_crBackgroundColor;
        
        BYTE        m_byItemStyle;
        BYTE        m_byLineStyle;

        CMemDC*     m_pPrintDC;

    // Operations
    public:
        void        Draw(CMemDC* pMemDC);
        void        SetPrinting(CMemDC* pPrintDC=NULL);

        void        SetItemColor(COLORREF crColor) {m_crItemColor = crColor;};
        COLORREF    GetItemColor(void) {return m_crItemColor;};
	    void        SetBorderColor(COLORREF crColor) {m_crBorderColor = crColor;};
	    void        SetLineColor(COLORREF crColor) {m_crLineColor = crColor;};
	    void        SetBackgroundColor(COLORREF crColor) {m_crBackgroundColor = crColor;};
	    void        SetItemStyle(BYTE byStyle) {m_byItemStyle = byStyle;};
        void        SetLineStyle(BYTE byStyle) {m_byLineStyle = byStyle;};
        BYTE        GetItemStyle(void) {return m_byItemStyle;};
	    BYTE        GetLineStyle(void) {return m_byLineStyle;};	    
    
    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CShape)
	    //}}AFX_VIRTUAL

    // Implementation
    public:
        virtual ~CShape() {};

	    // Generated message map functions
    protected:
	    //{{AFX_MSG(CShape)
	    afx_msg void OnPaint();
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
    };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAPE_H__468F4ED4_D36F_11D5_ACA9_00609796948F__INCLUDED_)
