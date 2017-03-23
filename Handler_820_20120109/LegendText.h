/************************************************************************************************
// $Header: /Surveyor/GuiLib/LegendText.h 3     10/30/02 10:23a Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : LegendText.h                                                                       */
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
#if !defined(AFX_LEGENDTEXT_H__D0D914B2_D2EC_11D5_8037_BD680817101A__INCLUDED_)
#define AFX_LEGENDTEXT_H__D0D914B2_D2EC_11D5_8037_BD680817101A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CLegendText window

#include "ChartWndBase.h"
#include "Shape.h"
#include "MemDC.h"

typedef enum {
    LT_NORMAL,
    LT_BOLD,
    LT_ITALIC,
    LT_UNDERLINE,
    };

#define ITEM_SPACING    5

class AFX_EXT_CLASS CLegendText : public CChartWndBase {
    // Construction
    public:
	    CLegendText();

    // Attributes
    public:
    protected:
        CString     m_LegendText;
        CShape      m_wndShape;
        CRect       m_rcClientRect;
        COLORREF    m_crItemColor;
        COLORREF    m_crBorderColor;
        COLORREF    m_crLineColor;
        COLORREF    m_crTextColor;
        COLORREF    m_crBackgroundColor;
        CFont*      m_pFont;

        BYTE        m_byTextStyle;
        BYTE        m_byItemStyle;
        BYTE        m_byLineStyle;
    private:
        BOOL        m_bCreate;

    // Operations
    public:
        void        Draw(CMemDC* pMemDC);
        void        RecalculateSize(CMemDC* m_pPrintDC=NULL);
        void        SetPrinting(CMemDC* pPrintDC=NULL);
    
    private:
        void        _CalculateClientRect(void);
        
    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CLegendText)
	    public:
	    protected:
	    public:
        virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext=NULL) {ASSERT(FALSE); return FALSE;};
        virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	    //}}AFX_VIRTUAL

    // Implementation
    public:
        void        GetClientRect(LPRECT lpRect) {*lpRect = m_rcClientRect;};
        COLORREF    SetTextColor(COLORREF crColor);
	    COLORREF    SetBackgroundColor(COLORREF crColor);
	    BOOL        SetFont(BYTE byPointSize, char* psFontName, BYTE byStyle=0);
        void        SetWindowText(char* pText);
        void        SetWindowText(CString* pText) {SetWindowText(pText->GetBuffer(0));};
        void        SetWindowText(CString Text) {SetWindowText(Text.GetBuffer(0));};
        
        COLORREF    SetItemColor(COLORREF crColor);
	    COLORREF    GetItemColor(void) {return m_wndShape.GetItemColor();};
	    COLORREF    SetBorderColor(COLORREF crColor);
	    COLORREF    SetLineColor(COLORREF crColor);
	    void        SetItemStyle(BYTE byStyle) {m_byItemStyle = byStyle; m_wndShape.SetItemStyle(byStyle);};
        void        SetLineStyle(BYTE byStyle) {m_byLineStyle = byStyle;  m_wndShape.SetLineStyle(byStyle);};
        BYTE        GetItemStyle(void) {return m_byItemStyle;};
	    BYTE        GetLineStyle(void) {return m_byLineStyle;};
	    
        virtual ~CLegendText();

	    // Generated message map functions
    protected:
	    //{{AFX_MSG(CLegendText)
	    afx_msg void OnPaint();
	    //}}AFX_MSG

	    DECLARE_MESSAGE_MAP()
    };

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEGENDTEXT_H__D0D914B2_D2EC_11D5_8037_BD680817101A__INCLUDED_)

//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/LegendText.h $
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
