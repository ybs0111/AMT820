/************************************************************************************************
// $Header: /Surveyor/GuiLib/PopupWindow.h 1     8/30/02 3:06p Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : PopupWindow.h                                                                      */
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
#if !defined(AFX_POPUPWINDOW_H__0BD2D843_7203_437D_897E_1EBC3B1AFB85__INCLUDED_)
#define AFX_POPUPWINDOW_H__0BD2D843_7203_437D_897E_1EBC3B1AFB85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PopupWindow.h : header file
//

#include "ChartWndBase.h"
/////////////////////////////////////////////////////////////////////////////
// CPopupWindow window

class AFX_EXT_CLASS CPopupWindow : public CChartWndBase
{
// Construction
public:
    CPopupWindow() {};

// Attributes
public:
private:
    CRect   m_ReferenceRect;

// Operations
public:
    void    SetWindowText(CString& WindowText);
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopupWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CPopupWindow() {};

	// Generated message map functions
protected:
	//{{AFX_MSG(CPopupWindow)
	afx_msg void OnPaint();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUPWINDOW_H__0BD2D843_7203_437D_897E_1EBC3B1AFB85__INCLUDED_)


//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/PopupWindow.h $
// 
// 1     8/30/02 3:06p Scott.pelger
// initial version
//***********************************************************************************************
