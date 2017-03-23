/************************************************************************************************
// $Header: /Surveyor/GuiLib/TitleWnd.h 2     9/25/02 8:55a Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : TitleWnd.h                                                                         */
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
#if !defined(AFX_TITLEWND_H__E4D51A35_CD70_11D5_8037_8C7A05B7325A__INCLUDED_)
#define AFX_TITLEWND_H__E4D51A35_CD70_11D5_8037_8C7A05B7325A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TitleWnd.h : header file
//

////////////////////////////////////////////////////////////////////////////
// CTitleWnd window

#include "ChartConstants.h"
#include "ChartWndBase.h"

class AFX_EXT_CLASS CTitleWnd : public CChartWndBase
{
// Construction
public:
	CTitleWnd();
    void    SetLText(CString& Text) {m_sLText = Text;};
    void    SetMText(CString& Text) {m_sMText = Text;};
    void    SetRText(CString& Text) {m_sRText = Text;};
    BOOL    CalculateWndHeight(CDC* pDC, DWORD* dwHeight);
    //void    SetRightEdge(WORD wRightEdge) {m_wRightEdge = wRightEdge;m_dwStyle|=CENTER_OVER_CHART;};
    //void    ClearRightEdge() {m_wRightEdge = 0;m_dwStyle&=~CENTER_OVER_CHART;};

    virtual void Draw(CMemDC* pMemDC);

// Attributes
public:
    CString m_sLText;
    CString m_sMText;
    CString m_sRText;
    DWORD   m_dwChartWndExtentLo;
    DWORD   m_dwChartWndExtentHi;
    DWORD   m_dwStyle;
    //WORD    m_wRightEdge;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTitleWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTitleWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLEWND_H__E4D51A35_CD70_11D5_8037_8C7A05B7325A__INCLUDED_)


//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/TitleWnd.h $
// 
// 2     9/25/02 8:55a Scott.pelger
// updated chart code to provide image exportation as bmp file
// 
// 1     8/30/02 3:06p Scott.pelger
// initial version
//***********************************************************************************************
