/************************************************************************************************
// $Header: /Surveyor/GuiLib/LegendWnd.h 2     9/25/02 8:55a Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : LegendWnd.h                                                                        */
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
#if !defined(AFX_LEGENDWND_H__E4D51A3A_CD70_11D5_8037_8C7A05B7325A__INCLUDED_)
#define AFX_LEGENDWND_H__E4D51A3A_CD70_11D5_8037_8C7A05B7325A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartWndBase.h"
#include "LegendText.h"

class AFX_EXT_CLASS CLegendWnd : public CChartWndBase
{
// Construction
public:
	CLegendWnd();

// Attributes
public:
private:
    CPtrArray               m_PointArray;
    CPtrArray               m_SeriesArray;

    BOOL                    m_bShowPointLegend;
    BYTE                    m_yPointSize;
    CString                 m_sFontName;

    CHART_WINDOW_POSITION   m_eLegendPosition;

    int                     m_yAutoHideWidth;
    int                     m_yAutoHideHeight;

// Operations
public:
    void                    Draw(CMemDC* pMemDC);
    void                    SetPrinting(CMemDC* pPrintDC/*=NULL*/);

    void                    SetAutoHide(BYTE yWidth, BYTE yHeight);
    void                    SetLegendPosition(CHART_WINDOW_POSITION eLegendPosition=eRIGHT) {m_eLegendPosition = eLegendPosition;};
    void                    SetInitialWindowSize(CRect* pRect) {m_rcClientRect = *pRect;};
    void                    ShowPointLegend(void) {m_bShowPointLegend = TRUE;};
    void                    ShowSeriesLegend(void) {m_bShowPointLegend = FALSE;};
    void                    AddPointItem(CString& Caption, COLORREF Color);
    void                    RemovePointItem(int nItem);
    void                    AddSeriesItem(CString& Caption, COLORREF Color);
    void                    RemoveSeriesItem(int nItem);
    void                    Purge(void);
    int                     _GetWindowMaxWidth();
    int                     _GetWindowMaxHeight();
    void                    GetMinimumWindowRect(CRect* pRect);
    void                    GetMinimumWindowRect(CRect& Rect) {GetMinimumWindowRect(&Rect);};
    virtual BOOL            SetFont(BYTE byPointSize, char* psFontName, BYTE byStyle=0);
    virtual COLORREF        SetWindowColor(COLORREF crColor);
    virtual void            SetLegendTextColor(COLORREF Color);
    virtual void            SetLegendSeriesItemColor(COLORREF Color, int nDataSeries=0);
    virtual COLORREF        GetLegendSeriesItemColor(int nDataSeries=0);
    virtual void            SetLegendPointItemColor(COLORREF Color, int nDataPoint=0);
    virtual COLORREF        GetLegendPointItemColor(int nDataPoint=0);

private:
    CLegendText*            _CreateNewLegendItem(CString& Caption, COLORREF Color);
    void                    _ArrangeVertical(CMemDC* pMemDC);
    void                    _ArrangeHorizontal(CMemDC* pMemDC);
    CPtrArray*              _GetLegendList(void);

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLegendWnd)
	//}}AFX_VIRTUAL
    
// Implementation
public:
	virtual                 ~CLegendWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLegendWnd)
	afx_msg void            OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEGENDWND_H__E4D51A3A_CD70_11D5_8037_8C7A05B7325A__INCLUDED_)


//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/LegendWnd.h $
// 
// 2     9/25/02 8:55a Scott.pelger
// updated chart code to provide image exportation as bmp file
// 
// 1     8/30/02 3:06p Scott.pelger
// initial version
//***********************************************************************************************
