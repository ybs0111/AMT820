/************************************************************************************************
// $Header: /Surveyor/GuiLib/DataSeries.h 1     8/30/02 3:06p Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : DataSeries.h                                                                       */
/*                                                                                              */
/* Purpose : interface for the data series within the CChartCtrl                                */
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
// DataSeries.h: interface for the CDataSeries class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASERIES_H__635CE59F_1253_4101_834D_A091171A855D__INCLUDED_)
#define AFX_DATASERIES_H__635CE59F_1253_4101_834D_A091171A855D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartConstants.h"
#include "MemDC.h"

class C2DScale;

class AFX_EXT_CLASS CDataPoint {
    public:
        double      m_dX;
        double      m_dY;
        COLORREF    m_crColor;
        CString     m_sCaption;
        DWORD       m_dwID;
        DWORD       m_dwDataSeries;
        HICON       m_hIcon;
        //for determining which chart item the mouse if hoovering over
        CRgn        m_DetectRegion;
        CRgn        m_PopupRegion;
        
        CDataPoint() : m_dwID(-1), m_hIcon(NULL) {};
        CDataPoint(double dX, double dY, COLORREF crColor, CString sCaption) : m_dX(dX), m_dY(dY), m_crColor(crColor), m_sCaption(sCaption), m_dwID(-1), m_hIcon(NULL) {};
        virtual ~CDataPoint() {};
        CDataPoint& CDataPoint::operator= (CDataPoint* DataPoint) {
            m_dwID = DataPoint->m_dwID;
            m_dwDataSeries = DataPoint->m_dwDataSeries;
            m_dX = DataPoint->m_dX;
            m_dY = DataPoint->m_dY;
            m_crColor = DataPoint->m_crColor;
            m_sCaption = DataPoint->m_sCaption;
            m_hIcon = DataPoint->m_hIcon;
            return *this;
            };
        CDataPoint& CDataPoint::operator= (CDataPoint& DataPoint) {
            m_dwID = DataPoint.m_dwID;
            m_dwDataSeries = DataPoint.m_dwDataSeries;
            m_dX = DataPoint.m_dX;
            m_dY = DataPoint.m_dY;
            m_crColor = DataPoint.m_crColor;
            m_sCaption = DataPoint.m_sCaption;
            m_hIcon = DataPoint.m_hIcon;
            return *this;
            };
    };

class AFX_EXT_CLASS CDataSeries {
    public:
	    CDataSeries(DATA_TYPE eDataType=TYPE_DBL_DATA);
	    virtual ~CDataSeries();

        BOOL            AddDataPoint(CDataPoint* pDataPoint);
        BOOL            RemoveDataPoint(int i);
        CDataPoint*     GetAt(int i) const {return (CDataPoint*)m_DataArray.GetAt(i);};
        
        int             GetNumOfDataPoints(void) {return m_DataArray.GetSize();};
        void            Purge(void);
        void            SetColor(COLORREF Color) {m_crColor = Color;};
        COLORREF        GetColor(void) {return m_crColor;};
        void            SetMaxNumDataPoints(WORD wMaxNumber) {m_wMaxInList = wMaxNumber;};
        WORD            GetMaxNumDataPoints(void) {return m_wMaxInList;};

        int             GetCount(void) {return m_DataArray.GetSize();};

    private:
        void            _CalculateStats(CDataPoint* pDataPoint);
    
    private:
        CPtrArray       m_DataArray;
        
    private:
        DATA_TYPE       m_eDataType;
        WORD            m_wMaxInList;
    public:
        COLORREF        m_crColor;
        CString         m_sCaption;
        DWORD           m_dwLastID;
        double          m_dXMin;
        double          m_dXMax;
        double          m_dYMin;
        double          m_dYMax;
    };

class AFX_EXT_CLASS CDataSeriesArray {
    public:
	    CDataSeriesArray(DATA_TYPE eDataType=TYPE_DBL_DATA);
        virtual ~CDataSeriesArray() {Purge();};

        int             AddDataSeries(CString Caption="", COLORREF Color=RGB(255, 0, 0));
        CDataSeries*    GetDataSeries(int nDataSeries);
        BOOL            AddDataPoint(CDataPoint* pDataPoint, int nDataSeries=0);
        BOOL            RemoveDataPoint(int nIndex, int nDataSeries=0);
        void            GetRangeInDataSeries(int nDataSeries, double& dMin, double& dMax, enum AXIS eAxis=X_AXIS);
        int             GetNumOfDataPoints(int nDataSeries=0);

        void            Purge(void);
        int             GetCount(void) {return m_DataSeriesArray.GetUpperBound()+1;};
        
        void            GetXAxisRange(double& dMin, double& dMax) {dMin = m_dXMin; dMax = m_dXMax;};
        void            GetYAxisRange(double& dMin, double& dMax) {dMin = m_dYMin; dMax = m_dYMax;};
        
        void            SetColor(COLORREF Color, int nDataSeries=0);
        COLORREF        GetColor(int nDataSeries=0);
        void            SetCaption(CString Caption, int nDataSeries=0);
        CString         GetCaption(int nDataSeries=0);
        enum CHART_TYPE SetChartType(enum CHART_TYPE eChartType);
        enum CHART_TYPE GetChartType(void) {return m_eChartType;};

        int             GetMaxInAllSeries(void);
        void            DrawData(CMemDC* pDC, C2DScale* pScale);
        void            DrawIcons(CMemDC* pDC, C2DScale* pScale);

        //these are for 3d pie charts
        void            SetHeight(double dHeight);
        double          GetHeight(void) {return m_dHeight;};
        void            SetElevation(double dElevation);
        double          GetElevation(void) {return m_dElevation;};
        
        //these are for strip charts
        //void            SetMaxNumDataPoints(WORD wMaxNumber);
        //WORD            GetMaxNumDataPoints(void) {return m_wMaxInList;};

        BOOL            DataRemoved(void) {return m_bRemoved;};
        void            PrepareStripChartForScrolling(BOOL* pbScrolling, BOOL* pbRefreshDisplayCompletely, CBitmap* pGraphBitmap) {m_pbScrolling = pbScrolling; m_pbRefreshDisplayCompletely = pbRefreshDisplayCompletely; m_pGraphBitmap = pGraphBitmap;};

        void            SetCustomChartStyle(WORD wStyle) {m_wStyle = wStyle;};
        WORD            GetCustomChartStyle() {return m_wStyle;};

        void            SetAxisOffset(enum AXIS eAxis, double dOffset) {if (eAxis==X_AXIS) m_dXOffset = dOffset; if (eAxis==Y_AXIS) m_dYOffset = dOffset;};

    private:
        void            _CalculateStats();
        CDataSeries*    _GetDataSeries(int nDataSeries);
        void            _DrawLineChart(int nX, int nY);
        void            _DrawAreaChart(int nX, int nY);
        void            _DrawRotatedLineChart(int nX, int nY);
        void            _DrawXYChart(int nX, int nY);
        void            _DrawRotatedXYChart(int nX, int nY);
        void            _DrawBarChart(int nX, int nY);
        void            _DrawPieChart(int nX, int nY);
        void            _DrawRotatedPieChart(int nX, int nY);
        void            _GenerateRegion(int i, int j, int nPrevAngle, int nAngle, int nHeight, CRect* RectClient, CPoint* pPopupPointArray, CPoint** pPtrPointArray, CDataSeries* pDataSeries, CDataPoint* pDataPoint);
        
    private:
        CPtrArray       m_DataSeriesArray;
        DATA_TYPE       m_eDataType;
        WORD            m_wMaxInList;
        enum CHART_TYPE m_eChartType;
        double          m_dXMin;
        double          m_dXMax;
        double          m_dYMin;
        double          m_dYMax;
        CMemDC*         m_pDC;
        C2DScale*       m_pScale;
        double          m_dHeight;
        double          m_dElevation;

        WORD            m_wStyle;
        
        //the following are for negative values, when this class was
        //implemented negative data points were not taken into consideration
        //it is easier of provide an offset than to make all code work
        //for both negative and positive data points
        double      m_dXOffset; //offset for negative data values
        double      m_dYOffset; //offset for negative data values
        
        /****scrolling data members****/
        BOOL            m_bRemoved;
        BOOL*           m_pbScrolling;
        BOOL*           m_pbRefreshDisplayCompletely;
        CBitmap*        m_pGraphBitmap;
        /****scrolling data members****/
    };

#endif // !defined(AFX_DATASERIES_H__635CE59F_1253_4101_834D_A091171A855D__INCLUDED_)

//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/DataSeries.h $
// 
// 1     8/30/02 3:06p Scott.pelger
// initial version
//***********************************************************************************************
