/************************************************************************************************
// $Header: /Surveyor/GuiLib/DataSeries.cpp 1     8/30/02 3:06p Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : DataSeries.cpp                                                                     */
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
#include "stdafx.h"
#include "DataSeries.h"
#include <float.h>
#include "2DScale.h"
#include "MemDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern COLORREF SubdueColor(COLORREF Color);
extern COLORREF CreateBorderColor(COLORREF Color);

CDataSeries::CDataSeries(DATA_TYPE eDataType/*=TYPE_DBL_DATA*/) :
    m_eDataType(TYPE_NONE_DATA),
    m_wMaxInList(0),
    m_crColor(0),
    m_dXMin(DBL_MAX),
    m_dXMax(-DBL_MAX),
    m_dYMin(DBL_MAX),
    m_dYMax(-DBL_MAX),
    m_dwLastID(-1) {
    
    m_DataArray.SetSize(0, 1);
    
    return;
    }

CDataSeries::~CDataSeries() {Purge();}

/************************************************************************************************/
/*                                                                                              */
/* Function: AddDataPoint()                                                                     */
/*                                                                                              */
/* Purpose : adds a data point to the serise                                                    */
/*                                                                                              */
/* Inputs  : CDataPoint* pDataPoint -> the data point to add                                    */
/*                                                                                              */
/* Outputs : BOOL <- TRUE if a data point has been removed from the series                      */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
BOOL CDataSeries::AddDataPoint(CDataPoint* pDataPoint) {
    
    BOOL bRemoved(FALSE);

    if (m_wMaxInList&&m_wMaxInList<GetNumOfDataPoints())
        bRemoved = !RemoveDataPoint(0);

    CDataPoint* pDP = new CDataPoint;
    *pDP = pDataPoint;
    
    //now get the stats
    _CalculateStats(pDataPoint);
    
    pDP->m_dwID = ++m_dwLastID;

    m_DataArray.Add(pDP);

    return bRemoved;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: _CalculateStats()                                                                  */
/*                                                                                              */
/* Purpose : calculates statistics for a data series                                            */
/*                                                                                              */
/* Inputs  : CDataPoint* pDataPoint -> the data point to compare the current stats with         */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeries::_CalculateStats(CDataPoint* pDataPoint) {

    m_dXMin = m_dXMin+DBL_EPSILON>pDataPoint->m_dX?pDataPoint->m_dX:m_dXMin;
    m_dXMax = m_dXMax-DBL_EPSILON<pDataPoint->m_dX?pDataPoint->m_dX:m_dXMax;
    m_dYMin = m_dYMin+DBL_EPSILON>pDataPoint->m_dY?pDataPoint->m_dY:m_dYMin;
    m_dYMax = m_dYMax-DBL_EPSILON<pDataPoint->m_dY?pDataPoint->m_dY:m_dYMax;

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: RemoveDataPoint()                                                                  */
/*                                                                                              */
/* Purpose : removes a data point at the specified location                                     */
/*                                                                                              */
/* Inputs  : int i -> location to remove the data point from                                    */
/*                                                                                              */
/* Outputs : BOOL <- TRUE if a data point at the specified location does not exist              */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/* Scott Pelger          10MAR00     decrement the indicies of all remaining points from the point of removal */
/*                                                                                              */
/************************************************************************************************/
BOOL CDataSeries::RemoveDataPoint(int i) {
    
    int nSize(m_DataArray.GetSize());

    if (nSize-1<i) {
        ASSERT(FALSE);
        return TRUE;
        }

    delete (CDataPoint*)m_DataArray.GetAt(i);
    m_DataArray.RemoveAt(i);

    //now we need to go through all the remaining data points and decrement their indicies by one
    CDataPoint* pDataPoint;
    for (;i<m_DataArray.GetSize();i++) {
        pDataPoint = (CDataPoint*)m_DataArray.GetAt(i);
        pDataPoint->m_dX -= 1;
        }
        
    /****need to recalculate the stats since we have removed an item****/
    m_dXMin = m_dYMin = DBL_MAX;
    m_dXMax = m_dYMax = DBL_MIN;
    for (i=0;i<m_DataArray.GetSize();i++)
        _CalculateStats((CDataPoint*)m_DataArray.GetAt(i));
    /****need to recalculate the stats since we have removed an item****/

    return FALSE;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeries::Purge()                                                               */
/*                                                                                              */
/* Purpose : removes all data points from the series                                            */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeries::Purge() {
    
    for (int i(0);i<m_DataArray.GetSize();i++)
        delete (CDataPoint*)m_DataArray.GetAt(i);
    
    m_DataArray.RemoveAll();
    
    m_dXMin = m_dYMin = DBL_MAX;
    m_dXMax = m_dYMax = DBL_MIN;

    m_dwLastID = -1;

    return;
    }

/***********************/
/***********************/
/***********************/
/***********************/
/***********************/
CDataSeriesArray::CDataSeriesArray(DATA_TYPE eDataType/*=TYPE_DBL_DATA*/) :
    m_dXMin(DBL_MAX),
    m_dXMax(-DBL_MAX),
    m_dYMin(DBL_MAX),
    m_dYMax(-DBL_MAX),
    m_eChartType(TYPE_LINE),
    m_dHeight(100),
    m_dElevation(5),
    m_wMaxInList(0),
    m_bRemoved(FALSE),
    m_pbScrolling(FALSE),
    m_wStyle(0),
    m_dXOffset(0),
    m_dYOffset(0) {

    m_DataSeriesArray.SetSize(0, 1);

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::SetElevation()                                                   */
/*                                                                                              */
/* Purpose : the amount of height or thickness for the 3D pie chart                             */
/*                                                                                              */
/* Inputs  : double dHeight -> amount of height to set [0,100]                                  */
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
void CDataSeriesArray::SetHeight(double dHeight) {

    //restrict this between 0 and 100
    m_dHeight = dHeight+DBL_EPSILON>100?100:dHeight+DBL_EPSILON<0?0:dHeight;

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::SetElevation()                                                   */
/*                                                                                              */
/* Purpose : the elevation of ones perspective above the 3D pie chart                           */
/*                                                                                              */
/* Inputs  : double dElevation -> amount of elevation to set [5.0,49.5]                         */
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
void CDataSeriesArray::SetElevation(double dElevation) {
    
    //restrict this between 5.0 and 49.5
    m_dElevation = dElevation+DBL_EPSILON>49.5?49.5:dElevation+DBL_EPSILON<5.0?5.0:dElevation;

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::AddDataSeries()                                                  */
/*                                                                                              */
/* Purpose : adds a new data series to the data series array                                    */
/*                                                                                              */
/* Inputs  : CString Caption -> caption for this new data series                                */
/*           COLORREF Color -> color for this new data series                                   */
/*                                                                                              */
/* Outputs : int <- location of this new data series                                            */
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
int CDataSeriesArray::AddDataSeries(CString Caption/*=""*/, COLORREF Color/*=RGB(255, 0, 0)*/) {
    
    CDataSeries* pDataSeries = new CDataSeries;

    pDataSeries->m_sCaption = Caption;
    pDataSeries->m_crColor = Color;

    if (TYPE_STRIP_CHART        ==m_eChartType||
        TYPE_STRIP_CHART_FILLED ==m_eChartType) {
        pDataSeries->SetMaxNumDataPoints(m_wMaxInList);

        if (pDataSeries) {
            CTime TTime = CTime::GetCurrentTime();
            int nDataPoints(m_wMaxInList);
            if (nDataPoints>0) {
                CDataPoint DataPoint;
                DataPoint.m_dY = 0;
                DataPoint.m_dX = TTime.GetTime()-m_wMaxInList;
                for (int nDataPoint(0);nDataPoint<m_wMaxInList;nDataPoint++) {
                    DataPoint.m_dX = DataPoint.m_dX + 1;
                    //TRACE("(%f, %f)\n", DataPoint.m_dX, DataPoint.m_dY);
                    pDataSeries->AddDataPoint(&DataPoint);
                    }
                _CalculateStats();
                }
            }
        }
    
    return m_DataSeriesArray.Add(pDataSeries);
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::GetDataSeries()                                                  */
/*                                                                                              */
/* Purpose : returns the data series at the specified location                                  */
/*                                                                                              */
/* Inputs  : int nDataSeries -> location of the data series to retrieve                         */
/*                                                                                              */
/* Outputs : CDataSeries* <- the retrieved data series                                          */
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
CDataSeries* CDataSeriesArray::GetDataSeries(int nDataSeries) {
    
    return (CDataSeries*)m_DataSeriesArray.GetAt(nDataSeries);
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: AddDataPoint()                                                                     */
/*                                                                                              */
/* Purpose : adds a data point to a specified data series                                       */
/*                                                                                              */
/* Inputs  : CDataPoint* pDataPoint -> data point to add                                        */
/*           int nDataSeries -> the series to add the data point to                             */
/*                                                                                              */
/* Outputs : BOOL <- TRUE if a data point has been removed from the series                      */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
BOOL CDataSeriesArray::AddDataPoint(CDataPoint* pDataPoint, int nDataSeries/*=0*/) {
    
    ASSERT(nDataSeries>=0);

    if (nDataSeries<0)
        return FALSE;

    pDataPoint->m_dwDataSeries = nDataSeries;

    m_bRemoved = _GetDataSeries(nDataSeries)->AddDataPoint(pDataPoint);
    //TRACE("(%f, %f)\n", pDataPoint->m_dX, pDataPoint->m_dY);
    _CalculateStats();

    return m_bRemoved;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: RemoveDataPoint()                                                                  */
/*                                                                                              */
/* Purpose : removes a data point at the specified location from a specified data series        */
/*                                                                                              */
/* Inputs  : int nIndex -> location to remove the data point from                               */
/*           int nDataSeries -> the series to remove the data point from                        */
/*                                                                                              */
/* Outputs : BOOL <- TRUE if a data point at the specified location does not exist              */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
BOOL CDataSeriesArray::RemoveDataPoint(int nIndex, int nDataSeries/*=0*/) {
    
    ASSERT(nDataSeries>=0);

    if (nDataSeries<0)
        return TRUE;

    BOOL bResult(_GetDataSeries(nDataSeries)->RemoveDataPoint(nIndex));

    _CalculateStats();

    return bResult;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::GetRangeInDataSeries()                                           */
/*                                                                                              */
/* Purpose : returns the min and max from the specified series and axis                         */
/*                                                                                              */
/* Inputs  : int nDataSeries -> location of data series                                         */
/*           double& dMin -> storage location for min                                           */
/*           double& dMax -> storage location for max                                           */
/*           enum AXIS eAxis -> axis to retrive from                                            */
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
void CDataSeriesArray::GetRangeInDataSeries(int nDataSeries, double& dMin, double& dMax, enum AXIS eAxis/*=X_AXIS*/) {
    
    ASSERT(nDataSeries>=0);

    if (nDataSeries<0)
        return;

    if (X_AXIS==eAxis) {
        dMin = _GetDataSeries(nDataSeries)->m_dXMin;
        dMin = _GetDataSeries(nDataSeries)->m_dXMin;
        }
    else {
        dMax = _GetDataSeries(nDataSeries)->m_dYMax;
        dMax = _GetDataSeries(nDataSeries)->m_dYMax;
        }
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::GetNumOfDataPoints()                                             */
/*                                                                                              */
/* Purpose : returns the number of data points in a specified series                            */
/*                                                                                              */
/* Inputs  : int nDataSeries -> location of data series                                         */
/*                                                                                              */
/* Outputs : int <- number of data points in the data series                                    */
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
int CDataSeriesArray::GetNumOfDataPoints(int nDataSeries/*=0*/) {

    ASSERT(nDataSeries>=0);

    if (nDataSeries<0)
        return 0;

    return _GetDataSeries(nDataSeries)->GetNumOfDataPoints();
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::_GetDataSeries()                                                 */
/*                                                                                              */
/* Purpose : returns the data series at the specified location                                  */
/*                                                                                              */
/* Inputs  : int nDataSeries -> location of data series                                         */
/*                                                                                              */
/* Outputs : CDataSeries* <- retrieved data series                                              */
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
CDataSeries* CDataSeriesArray::_GetDataSeries(int nDataSeries) {
    
    if (nDataSeries<=m_DataSeriesArray.GetUpperBound()) {
        CDataSeries* pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(nDataSeries);
        if (pDataSeries)
            return pDataSeries;
        }

    ASSERT(FALSE);

    return NULL;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: Purge()                                                                            */
/*                                                                                              */
/* Purpose : removes all data points from all series                                            */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeriesArray::Purge(void) {
    
    for (int i(0);i<m_DataSeriesArray.GetSize();i++)
        delete (CDataSeries*)m_DataSeriesArray.GetAt(i);

    m_DataSeriesArray.RemoveAll();

    m_dXMin = m_dYMin = DBL_MAX;
    m_dXMax = m_dYMax = DBL_MIN;

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::SetColor()                                                       */
/*                                                                                              */
/* Purpose : sets the color of a specified data series                                          */
/*                                                                                              */
/* Inputs  : COLORREF Color -> color to set                                                     */
/*           int nDataSeries -> location of data series                                         */
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
void CDataSeriesArray::SetColor(COLORREF Color, int nDataSeries/*=0*/) {
    
    ASSERT(nDataSeries>=0);

    if (nDataSeries<0)
        return;

    CDataSeries* pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(nDataSeries);
    pDataSeries->m_crColor = Color;

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::GetColor()                                                       */
/*                                                                                              */
/* Purpose : returns the color from the specified data series                                   */
/*                                                                                              */
/* Inputs  : int nDataSeries -> location of data series                                         */
/*                                                                                              */
/* Outputs : COLORREF <- color retrieved                                                        */
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
COLORREF CDataSeriesArray::GetColor(int nDataSeries/*=0*/) {
    
    ASSERT(nDataSeries>=0);

    if (nDataSeries<0)
        return 0;

    CDataSeries* pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(nDataSeries);
    
    return pDataSeries->m_crColor;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::SetCaption()                                                     */
/*                                                                                              */
/* Purpose : sets the caption for the specified series                                          */
/*                                                                                              */
/* Inputs  : CString Caption -> caption to set                                                  */
/*           int nDataSeries -> location of data series                                         */
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
void CDataSeriesArray::SetCaption(CString Caption, int nDataSeries/*=0*/) {

    ASSERT(nDataSeries>=0);

    if (nDataSeries<0)
        return;

    CDataSeries* pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(nDataSeries);
    pDataSeries->m_sCaption = Caption;

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: CDataSeriesArray::GetCaption()                                                     */
/*                                                                                              */
/* Purpose : returns the caption from the specified data series                                 */
/*                                                                                              */
/* Inputs  : int nDataSeries -> location of data series                                         */
/*                                                                                              */
/* Outputs : CString <- caption retrieved                                                       */
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
CString CDataSeriesArray::GetCaption(int nDataSeries/*=0*/) {

    ASSERT(nDataSeries>=0);

    if (nDataSeries<0)
        return "";

    CDataSeries* pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(nDataSeries);
    
    return pDataSeries->m_sCaption;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: _CalculateStats()                                                                  */
/*                                                                                              */
/* Purpose : calculates statistics for all data series on a collective level                    */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeriesArray::_CalculateStats() {
    
    m_dXMin = DBL_MAX;
    m_dXMax = DBL_MIN;
    m_dYMin = DBL_MAX;
    m_dYMax = DBL_MIN;

    for (int i(0);i<m_DataSeriesArray.GetSize();i++) {
        CDataSeries* pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);

        m_dXMin = m_dXMin+DBL_EPSILON>pDataSeries->m_dXMin?pDataSeries->m_dXMin:m_dXMin;
        m_dXMax = m_dXMax-DBL_EPSILON<pDataSeries->m_dXMax?pDataSeries->m_dXMax:m_dXMax;
        m_dYMin = m_dYMin+DBL_EPSILON>pDataSeries->m_dYMin?pDataSeries->m_dYMin:m_dYMin;
        m_dYMax = m_dYMax-DBL_EPSILON<pDataSeries->m_dYMax?pDataSeries->m_dYMax:m_dYMax;
        }

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: GetMaxInAllSeries()                                                                */
/*                                                                                              */
/* Purpose : returns the maximum number of data points of all series'                           */
/*                                                                                              */
/* Inputs  : NONE                                                                               */
/*                                                                                              */
/* Outputs : int <- max number of data points                                                   */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
int CDataSeriesArray::GetMaxInAllSeries() {

    int nNumberOfSeries(m_DataSeriesArray.GetSize());
    int nNumberInSeries(0);
    
    for (int i(0);i<nNumberOfSeries;i++) {
        CDataSeries* pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);
        nNumberInSeries = nNumberInSeries>pDataSeries->GetCount()?nNumberInSeries:pDataSeries->GetCount();
        }
    
    return nNumberInSeries;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: SetChartType()                                                                     */
/*                                                                                              */
/* Purpose : sets the chart type. if the type is a strip chart then the number of data points   */
/*           is set also                                                                        */
/*                                                                                              */
/* Inputs  : enum CHART_TYPE eChartType -> the chart type                                       */
/*                                                                                              */
/* Outputs : enum CHART_TYPE <- the chart type passed in. in the case of a strip chart the type */
/*           returned is the base chart type                                                    */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
enum CHART_TYPE CDataSeriesArray::SetChartType(enum CHART_TYPE eChartType) {
    
    switch (eChartType) {
        case TYPE_STRIP_CHART_15:
        case TYPE_STRIP_CHART_30:
        case TYPE_STRIP_CHART_60:
        case TYPE_STRIP_CHART_120:
        case TYPE_STRIP_CHART_300:
        case TYPE_STRIP_CHART_600: {
            m_eChartType = TYPE_STRIP_CHART;
            int nNumberOfSeries(m_DataSeriesArray.GetSize());
            m_wMaxInList = eChartType;
            for (int i(0);i<nNumberOfSeries;i++) {
                CDataSeries* pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);
                pDataSeries->SetMaxNumDataPoints(eChartType);
                }
            }
            break;
        case TYPE_STRIP_CHART_15_FILLED:
        case TYPE_STRIP_CHART_30_FILLED:
        case TYPE_STRIP_CHART_60_FILLED:
        case TYPE_STRIP_CHART_120_FILLED:
        case TYPE_STRIP_CHART_300_FILLED:
        case TYPE_STRIP_CHART_600_FILLED: {
            m_eChartType = TYPE_STRIP_CHART_FILLED;
            int nNumberOfSeries(m_DataSeriesArray.GetSize());
            m_wMaxInList = eChartType-1;
            for (int i(0);i<nNumberOfSeries;i++) {
                CDataSeries* pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);
                pDataSeries->SetMaxNumDataPoints(eChartType);
                }
            }
            break;
        default:
            m_eChartType = eChartType;
            m_wMaxInList = 0;
        }

    return m_eChartType;
    }

/*BOOL Intersect(double dX11, double dY11, double dX12, double dY12, double dX21, double dY21, double dX22, double dY22, double& dXIntersect, double& dYIntersect) {

    double dM1((dY11-dY12)/(dX11-dX12));
    double dB1(dY11-dM1*dX11);
    double dM2((dY21-dY22)/(dX21-dX22));
    double dB2(dY21-dM2*dX21);

    dXIntersect = (dB2-dB1)/(dM1-dM2);
    dYIntersect = dM1*dXIntersect+dB1;

    if (dXIntersect+DBL_EPSILON>=dX11&&dXIntersect+DBL_EPSILON<=dX12)
        return TRUE;

    return FALSE;
    }*/

/************************************************************************************************/
/*                                                                                              */
/* Function: DrawData()                                                                         */
/*                                                                                              */
/* Purpose : draws a chart                                                                      */
/*                                                                                              */
/* Inputs  : CDC* pDC -> the device context to draw with                                        */
/*           C2DScale* pScale -> pointer to the scale class for access to the private methods   */
/*           and members of which this class has been granted friendship                        */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeriesArray::DrawData(CMemDC* pDC, C2DScale* pScale) {

    m_pDC = pDC;
    m_pScale = pScale;

    int nY = (int)(m_pScale->m_yDepth*sin(D2R(m_pScale->m_dVertRotation)));
    int nX = (int)(m_pScale->m_yDepth*sin(D2R(m_pScale->m_dHorzRotation)));

    switch (m_eChartType) {
        case TYPE_LINE :
        case TYPE_STRIP_CHART :
            _DrawLineChart(nX, nY);
            break;
        case TYPE_AREA :
        case TYPE_STRIP_CHART_FILLED :
            _DrawAreaChart(nX, nY);
            break;
        case TYPE_XY :
        case TYPE_XY_CONNECTED :
            _DrawXYChart(nX, nY);
            break;
        case TYPE_BAR :
            _DrawBarChart(nX, nY);
            break;
        case TYPE_PIE :
            _DrawPieChart(nX, nY);
            break;
        case TYPE_ROTATED_PIE :
            _DrawRotatedPieChart(nX, nY);
            break;
        }

    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: DrawIcons()                                                                        */
/*                                                                                              */
/* Purpose : draws any icons                                                                    */
/*                                                                                              */
/* Inputs  : CDC* pDC -> the device context to draw with                                        */
/*           C2DScale* pScale -> pointer to the scale class for access to the private methods   */
/*           and members of which this class has been granted friendship                        */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeriesArray::DrawIcons(CMemDC* pDC, C2DScale* pScale) {

    m_pDC = pDC;
    m_pScale = pScale;

    CDataPoint* pDataPoint = NULL;
    CDataSeries* pDataSeries = NULL;
    CPoint* pPointArray = NULL;
    
    for (int i(0);i<m_DataSeriesArray.GetSize();i++) {
        pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);
        if (!pDataSeries->GetCount())
            continue;

        int j((m_pbScrolling&&*m_pbScrolling)?pDataSeries->GetCount()-2:0);
        j = TYPE_STRIP_CHART_FILLED==m_eChartType?j:0;
        for (;j<pDataSeries->GetCount();j++) {
            pDataPoint = pDataSeries->GetAt(j);
            if (pDataPoint->m_hIcon)
                m_pDC->DrawIcon(m_pScale->_XValueToPixel(m_dXOffset+TYPE_STRIP_CHART_FILLED==m_eChartType||TYPE_STRIP_CHART==m_eChartType?pDataPoint->m_dX:j), 
                                m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset), pDataPoint->m_hIcon);
            }
        }
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: ChartCompare()                                                                     */
/*                                                                                              */
/* Purpose : this function compares the m_dY member of a CDataPoint array                       */
/*                                                                                              */
/* Inputs  : const void* v1 -> pointer to an item to compare                                    */
/*           const void* v2 -> pointer to an item to compare                                    */
/*                                                                                              */
/* Outputs : int <- -1 if v1 < v2, 0 if v1 == v2, 1 if v1 > v2                                  */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 10MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          10MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
BYTE ySortAxis;
int	ChartCompare(const void *v1, const void *v2) {
	
    CDataPoint* n1;
	CDataPoint* n2;
	
    int equal = 0;

	n1 = ((CDataPoint *)v1);
	n2 = ((CDataPoint *)v2);

    if(n1 == NULL || n2 == NULL) {
		ASSERT(FALSE);
        return 0;
        }

	if ((BYTE)(X_AXIS>>24)==ySortAxis)
        return n1->m_dX>n2->m_dX?1:n2->m_dX>n1->m_dX?-1:0;
    else if ((BYTE)(Y_AXIS>>24)==ySortAxis)
        return fabs(n1->m_dY)>fabs(n2->m_dY)?1:fabs(n2->m_dY)>fabs(n1->m_dY)?-1:0;

    return 0;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: _DrawLineChart()                                                                   */
/*                                                                                              */
/* Purpose : draws a line chart                                                                 */
/*                                                                                              */
/* Inputs  : int nX -> x offset for depth due to rotation                                       */
/*           int nY -> y offset for depth due to rotation                                       */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeriesArray::_DrawLineChart(int nX, int nY) {

    CDataPoint* pDataPoint = NULL;
    CDataSeries* pDataSeries = NULL;
    CPoint* pPointArray = NULL;
    
    CPen Pen;

    for (int i(0);i<m_DataSeriesArray.GetSize();i++) {
        pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);
        if (!pDataSeries->GetCount())
            continue;

        pPointArray = new CPoint[pDataSeries->GetCount()];

        //set the series' color
        Pen.CreatePen(PS_SOLID, 1, pDataSeries->m_crColor);
        m_pDC->SelectObject(&Pen);

        pDataPoint = pDataSeries->GetAt(0);
        
        for (int j(0);j<pDataSeries->GetCount();j++) {
            pDataPoint = pDataSeries->GetAt(j);
            pPointArray[j].x = m_pScale->_XValueToPixel(TYPE_STRIP_CHART==m_eChartType?pDataPoint->m_dX+m_dXOffset:j);
            pPointArray[j].y = m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset);

            if (m_eChartType==TYPE_LINE) {
                pDataPoint->m_DetectRegion.DeleteObject();
                pDataPoint->m_PopupRegion.DeleteObject();
                pDataPoint->m_DetectRegion.CreateRectRgn(pPointArray[j].x-5, pPointArray[j].y-5,
                                                         pPointArray[j].x+5, pPointArray[j].y+5);
                pDataPoint->m_PopupRegion.CreateRectRgn(pPointArray[j].x-5, pPointArray[j].y-5,
                                                        pPointArray[j].x+5, pPointArray[j].y+5);
                }
            }
        m_pDC->Polyline(pPointArray, pDataSeries->GetCount());
        Pen.DeleteObject();
        
        delete [] pPointArray;
        }
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: _DrawRotatedLineChart()                                                            */
/*                                                                                              */
/* Purpose : draws a rotated line chart                                                         */
/*                                                                                              */
/* Inputs  : int nX -> x offset for depth due to rotation                                       */
/*           int nY -> y offset for depth due to rotation                                       */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeriesArray::_DrawRotatedLineChart(int nX, int nY) {

    //TODO
    ASSERT(FALSE);
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: _DrawAreaChart()                                                                   */
/*                                                                                              */
/* Purpose : draws an area chart                                                                */
/*                                                                                              */
/* Inputs  : int nX -> x offset for depth due to rotation                                       */
/*           int nY -> y offset for depth due to rotation                                       */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
#if 1
#define SKIP 2
static UINT m_uSkip(SKIP);
void CDataSeriesArray::_DrawAreaChart(int nX, int nY) {

    CDataPoint* pDataPoint = NULL;
    CDataSeries* pDataSeries = NULL;
    CPoint* pPointArray = NULL;

    int nArraySize(m_DataSeriesArray.GetSize());
    int nSeriesSize(0);
    
    CPen Pen;
    
    if (m_pbScrolling&&*m_pbScrolling) {
        //TRACE("Scrolling\n");
        pPointArray = new CPoint[4];
        
        CMemDC memDC(m_pDC);
        memDC.SelectObject(m_pGraphBitmap);

        pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(0);
        nSeriesSize = pDataSeries->GetCount();
        
        pDataPoint = pDataSeries->GetAt(nSeriesSize-1);
        int nDelta(m_pScale->_XValueToPixel(pDataPoint->m_dX));
        
        pDataPoint = pDataSeries->GetAt(nSeriesSize-2);
        nDelta -= m_pScale->_XValueToPixel(pDataPoint->m_dX);

        m_pDC->BitBlt(m_pScale->m_pRect->left+1, m_pScale->m_pRect->top, m_pScale->m_pRect->Width()-nDelta-1, m_pScale->m_pRect->Height(), &memDC, nDelta+1, 0, SRCCOPY);

        for (int i(0);i<nArraySize;i++) {
            pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);
            
            pDataPoint = pDataSeries->GetAt(nSeriesSize-2);
            pPointArray[1].x = m_pScale->_XValueToPixel(pDataPoint->m_dX+m_dXOffset)-1;
            pPointArray[1].y = m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset);
            pDataPoint = pDataSeries->GetAt(nSeriesSize-1);
            pPointArray[2].x = pPointArray[1].x+nDelta;
            pPointArray[2].y = m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset);
            
            pPointArray[0].x = pPointArray[1].x;
            pPointArray[0].y = m_pScale->_YValueToPixel(0) - 1;
            pPointArray[3].x = pPointArray[2].x;
            pPointArray[3].y = pPointArray[0].y;
            
            //set the series' color
            Pen.DeleteObject();
            Pen.CreatePen(PS_SOLID, 1, pDataSeries->m_crColor);
            m_pDC->SelectObject(&Pen);
            CBrush Brush(pDataSeries->m_crColor);
            m_pDC->SelectObject(&Brush);
            
            m_pDC->Polygon(pPointArray, 4);
            
            Pen.DeleteObject();
            Pen.CreatePen(PS_SOLID, 1, CreateBorderColor(pDataSeries->m_crColor));
            m_pDC->SelectObject(&Pen);
            m_pDC->MoveTo(pPointArray[1]);
            m_pDC->LineTo(pPointArray[2]);
            }
        
        delete [] pPointArray;
        }
    
    for (int i(0);i<nArraySize;i++) {
        pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);
        nSeriesSize = pDataSeries->GetCount();
        if (!nSeriesSize)
            continue;
        
        pPointArray = new CPoint[nSeriesSize+(nSeriesSize>1?2:3)];
        
        //set the series' color
        Pen.DeleteObject();
        Pen.CreatePen(PS_SOLID, 1, pDataSeries->m_crColor);
        m_pDC->SelectObject(&Pen);
        CBrush Brush(pDataSeries->m_crColor);
        m_pDC->SelectObject(&Brush);
        
        pDataPoint = pDataSeries->GetAt(0);
        
        for (int j(0);j<nSeriesSize;j++) {
            pDataPoint = pDataSeries->GetAt(j);
            pPointArray[j+1].x = m_pScale->_XValueToPixel(TYPE_STRIP_CHART_FILLED==m_eChartType?pDataPoint->m_dX+m_dXOffset:j);
            pPointArray[j+1].y = m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset);
            }
        
        pPointArray[0].x = pPointArray[1].x + 1;
        pPointArray[0].y = m_pScale->_YValueToPixel(0/*-m_dYOffset*/) - 1; //the bottom

        if (1==nSeriesSize) {
            pPointArray[2].x = m_pScale->m_pRect->right - 1;
            pPointArray[2].y = m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset);
            pPointArray[3].x = m_pScale->m_pRect->right - 1;
            pPointArray[3].y = m_pScale->m_pRect->bottom;
            }
        else {
            pPointArray[j+1].x = pPointArray[j].x - 1;
            pPointArray[j+1].y = pPointArray[0].y;
            pPointArray[1].x++;
            pPointArray[j].x--;
            }
        
        if (TYPE_AREA==m_eChartType) {
            pDataPoint = pDataSeries->GetAt(0);
            pDataPoint->m_DetectRegion.DeleteObject();
            pDataPoint->m_PopupRegion.DeleteObject();
            pDataPoint->m_DetectRegion.CreatePolygonRgn(pPointArray, nSeriesSize+(nSeriesSize>1?2:3), ALTERNATE/*NOT USING*/);
            pDataPoint->m_PopupRegion.CreatePolygonRgn(pPointArray, nSeriesSize+(nSeriesSize>1?2:3), ALTERNATE/*NOT USING*/);
            }
        
    
        if (TYPE_AREA==m_eChartType||(!(m_pbScrolling&&*m_pbScrolling)&&(m_uSkip/SKIP))||(m_pbRefreshDisplayCompletely&&*m_pbRefreshDisplayCompletely)) {
            //TRACE("Not Skipped\n");
            m_pDC->Polygon(pPointArray, nSeriesSize+(nSeriesSize>1?2:3));
            if (TYPE_STRIP_CHART_FILLED==m_eChartType&&i==nArraySize-1) {
                m_uSkip = 1;
                *m_pbRefreshDisplayCompletely = FALSE;
                }
            }
        else if (!(m_pbScrolling&&*m_pbScrolling)) {
            //TRACE("Skipped\n");
            if (i==nArraySize-1)
                m_uSkip++;
            CPoint* pPointArray = new CPoint[4];
            CMemDC memDC(m_pDC);
            memDC.SelectObject(m_pGraphBitmap);

            pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(0);
            nSeriesSize = pDataSeries->GetCount();
    
            pDataPoint = pDataSeries->GetAt(nSeriesSize-1);
            int nDelta(m_pScale->_XValueToPixel(pDataPoint->m_dX+m_dXOffset));
    
            pDataPoint = pDataSeries->GetAt(nSeriesSize-2);
            nDelta -= m_pScale->_XValueToPixel(pDataPoint->m_dX+m_dXOffset);

            m_pDC->StretchBlt(m_pScale->m_pRect->left, m_pScale->m_pRect->top, m_pScale->m_pRect->Width()-nDelta-1, m_pScale->m_pRect->Height(), &memDC, 0, 0, m_pScale->m_pRect->Width(), m_pScale->m_pRect->Height(), SRCCOPY);
            
            for (int ii(0);ii<nArraySize;ii++) {
                pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(ii);
            
                pDataPoint = pDataSeries->GetAt(nSeriesSize-2);
                pPointArray[1].x = m_pScale->_XValueToPixel(pDataPoint->m_dX+m_dXOffset)-1;
                pPointArray[1].y = m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset);
                pDataPoint = pDataSeries->GetAt(nSeriesSize-1);
                pPointArray[2].x = pPointArray[1].x+nDelta;
                pPointArray[2].y = m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset);
            
                pPointArray[0].x = pPointArray[1].x;
                pPointArray[0].y = m_pScale->_YValueToPixel(0) - 1;
                pPointArray[3].x = pPointArray[2].x;
                pPointArray[3].y = pPointArray[0].y;
            
                //set the series' color
                Pen.DeleteObject();
                Pen.CreatePen(PS_SOLID, 1, pDataSeries->m_crColor);
                m_pDC->SelectObject(&Pen);
                CBrush Brush(pDataSeries->m_crColor);
                m_pDC->SelectObject(&Brush);
            
                m_pDC->Polygon(pPointArray, 4);
            
                Pen.DeleteObject();
                Pen.CreatePen(PS_SOLID, 1, CreateBorderColor(pDataSeries->m_crColor));
                m_pDC->SelectObject(&Pen);
                m_pDC->MoveTo(pPointArray[1]);
                m_pDC->LineTo(pPointArray[2]);
                }
            
            delete [] pPointArray;
            }
        
        Pen.DeleteObject();
        Pen.CreatePen(PS_SOLID, 1, CreateBorderColor(pDataSeries->m_crColor));
        m_pDC->SelectObject(&Pen);
        
        if (!(m_pbScrolling&&*m_pbScrolling)&&!(m_uSkip/SKIP))
            m_pDC->Polyline(pPointArray+1, nSeriesSize+(nSeriesSize>1?0:1));
        
        delete [] pPointArray;
        }

    return;
    }
#else
//this version is graphically superior but is a CPU hog
//the previous version of this was even better in that it did not
//even deal with scrolling, it just redrew everything
//each and every time but really ate the processor.
void CDataSeriesArray::_DrawAreaChart(int nX, int nY) {

    CDataPoint* pDataPoint = NULL;
    CDataSeries* pDataSeries = NULL;
    CPoint* pPointArray = NULL;

    int nArraySize(m_DataSeriesArray.GetSize());
    int nSeriesSize(0);

    if ((m_pbScrolling&&*m_pbScrolling)) {
        pPointArray = new CPoint[4];
        
        CMemDC memDC(m_pDC);
        memDC.SelectObject(m_pGraphBitmap);

        pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(0);
        nSeriesSize = pDataSeries->GetCount();
        
        pDataPoint = pDataSeries->GetAt(nSeriesSize-1);
        int nDelta(m_pScale->_XValueToPixel(pDataPoint->m_dX));
        
        pDataPoint = pDataSeries->GetAt(nSeriesSize-2);
        nDelta -= m_pScale->_XValueToPixel(pDataPoint->m_dX);

        m_pDC->BitBlt(m_pScale->m_pRect->left+1, m_pScale->m_pRect->top, m_pScale->m_pRect->Width()-nDelta-1, m_pScale->m_pRect->Height(), &memDC, nDelta+1, 0, SRCCOPY);

        for (int i(0);i<nArraySize;i++) {
            pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);
            
            pDataPoint = pDataSeries->GetAt(nSeriesSize-2);
            pPointArray[1].x = m_pScale->_XValueToPixel(pDataPoint->m_dX)-1;
            pPointArray[1].y = m_pScale->_YValueToPixel(pDataPoint->m_dY);
            pDataPoint = pDataSeries->GetAt(nSeriesSize-1);
            pPointArray[2].x = pPointArray[1].x+nDelta;
            pPointArray[2].y = m_pScale->_YValueToPixel(pDataPoint->m_dY);
            
            pPointArray[0].x = pPointArray[1].x;
            pPointArray[0].y = m_pScale->_YValueToPixel(0) - 1;
            pPointArray[3].x = pPointArray[2].x;
            pPointArray[3].y = pPointArray[0].y;
            
            //set the series' color
            CPen Pen(PS_SOLID, 1, pDataSeries->m_crColor);
            m_pDC->SelectObject(&Pen);
            CBrush Brush(pDataSeries->m_crColor);
            m_pDC->SelectObject(&Brush);
            
            m_pDC->Polygon(pPointArray, 4);
            
            Pen.DeleteObject();
            Pen.CreatePen(PS_SOLID, 1, CreateBorderColor(pDataSeries->m_crColor));
            m_pDC->SelectObject(&Pen);
            m_pDC->MoveTo(pPointArray[1]);
            m_pDC->LineTo(pPointArray[2]);
            }
        
        delete [] pPointArray;
        }
    
    for (int i(0);i<nArraySize;i++) {
        pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);
        nSeriesSize = pDataSeries->GetCount();
        if (!nSeriesSize)
            continue;
        
        pPointArray = new CPoint[nSeriesSize+(nSeriesSize>1?2:3)];
        
        //set the series' color
        CPen Pen(PS_SOLID, 1, pDataSeries->m_crColor);
        m_pDC->SelectObject(&Pen);
        CBrush Brush(pDataSeries->m_crColor);
        m_pDC->SelectObject(&Brush);
        
        pDataPoint = pDataSeries->GetAt(0);
        
        for (int j(0);j<nSeriesSize;j++) {
            pDataPoint = pDataSeries->GetAt(j);
            pPointArray[j+1].x = m_pScale->_XValueToPixel(TYPE_STRIP_CHART_FILLED==m_eChartType?pDataPoint->m_dX:j);
            pPointArray[j+1].y = m_pScale->_YValueToPixel(pDataPoint->m_dY);
            }
        
        pPointArray[0].x = pPointArray[1].x + 1;
        pPointArray[0].y = m_pScale->_YValueToPixel(0) - 1;

        if (1==nSeriesSize) {
            pPointArray[2].x = m_pScale->m_pRect->right - 1;
            pPointArray[2].y = m_pScale->_YValueToPixel(pDataPoint->m_dY);
            pPointArray[3].x = m_pScale->m_pRect->right - 1;
            pPointArray[3].y = m_pScale->m_pRect->bottom;
            }
        else {
            pPointArray[j+1].x = pPointArray[j].x - 1;
            pPointArray[j+1].y = pPointArray[0].y;
            pPointArray[1].x++;
            pPointArray[j].x--;
            }
        
        if (!(m_pbScrolling&&*m_pbScrolling))
            m_pDC->Polygon(pPointArray, nSeriesSize+(nSeriesSize>1?2:3));
        
        Pen.DeleteObject();
        Pen.CreatePen(PS_SOLID, 1, CreateBorderColor(pDataSeries->m_crColor));
        m_pDC->SelectObject(&Pen);
        if (!(m_pbScrolling&&*m_pbScrolling))
            m_pDC->Polyline(pPointArray+1, nSeriesSize+(nSeriesSize>1?0:1));
        
        for (j=0;j<-1*nSeriesSize;j++) {
            pDataPoint = pDataSeries->GetAt(j);
            pDataPoint->m_DetectRegion.DeleteObject();
            pDataPoint->m_PopupRegion.DeleteObject();
            pDataPoint->m_DetectRegion.CreatePolygonRgn(pPointArray, nSeriesSize+(nSeriesSize>1?2:3), ALTERNATE/*NOT USING*/);
            pDataPoint->m_PopupRegion.CreatePolygonRgn(pPointArray, nSeriesSize+(nSeriesSize>1?2:3), ALTERNATE/*NOT USING*/);
            }
        
        delete [] pPointArray;
        }
    
    return;
    }
#endif

/************************************************************************************************/
/*                                                                                              */
/* Function: _DrawXYChart()                                                                     */
/*                                                                                              */
/* Purpose : draws a XY chart                                                                   */
/*                                                                                              */
/* Inputs  : int nX -> x offset for depth due to rotation                                       */
/*           int nY -> y offset for depth due to rotation                                       */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeriesArray::_DrawXYChart(int nX, int nY) {

    CDataPoint* pDataPoint = NULL;
    CDataSeries* pDataSeries = NULL;
    
    for (int i(0);i<m_DataSeriesArray.GetSize();i++) {
        pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(i);
        if (!pDataSeries->GetCount())
            continue;

        //set the series' color
        CPen Pen(PS_SOLID, 1, pDataSeries->m_crColor);
        m_pDC->SelectObject(&Pen);
        CBrush Brush(pDataSeries->m_crColor);
        m_pDC->SelectObject(&Brush);

        pDataPoint = pDataSeries->GetAt(0);

        if (m_eChartType==TYPE_XY_CONNECTED)
            m_pDC->MoveTo(m_pScale->_XValueToPixel(pDataPoint->m_dX), m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset));
        
        CRect Rect;
        for (int j(0);j<pDataSeries->GetCount();j++) {
            pDataPoint = pDataSeries->GetAt(j);
            Rect.top = Rect.bottom = m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset) - 2;
            Rect.left = Rect.right = m_pScale->_XValueToPixel(pDataPoint->m_dX+m_dXOffset) - 2;
            Rect.top += 5;
            Rect.left += 5;
            m_pDC->Ellipse(Rect);
            if (m_eChartType==TYPE_XY_CONNECTED)
                m_pDC->LineTo(m_pScale->_XValueToPixel(pDataPoint->m_dX), m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset));
            pDataPoint->m_DetectRegion.DeleteObject();
            pDataPoint->m_PopupRegion.DeleteObject();
            pDataPoint->m_DetectRegion.CreateRectRgn(Rect.left-5, Rect.top-5,
                                                     Rect.right+5, Rect.bottom+5);
            pDataPoint->m_PopupRegion.CreateRectRgn(Rect.left-5, Rect.top-5,
                                                    Rect.right+5, Rect.bottom+5);
            }
        }
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: _DrawRotatedXYChart()                                                              */
/*                                                                                              */
/* Purpose : draws a rotated XY chart                                                           */
/*                                                                                              */
/* Inputs  : int nX -> x offset for depth due to rotation                                       */
/*           int nY -> y offset for depth due to rotation                                       */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeriesArray::_DrawRotatedXYChart(int nX, int nY) {

    //TODO
    ASSERT(FALSE);
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: _DrawBarChart()                                                                    */
/*                                                                                              */
/* Purpose : draws a bar chart                                                                  */
/*                                                                                              */
/* Inputs  : int nX -> x offset for depth due to rotation                                       */
/*           int nY -> y offset for depth due to rotation                                       */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/* Scott Pelger          06AUG02     added bar chart labeling                                   */
/*                                                                                              */
/************************************************************************************************/
#define BAR_SPACING 0.5
typedef struct _TEXT_LOCATION_ {
    int m_nX;
    int m_nY;
    int m_nLength;
    CString Caption;
    } TextLocation;

void CDataSeriesArray::_DrawBarChart(int nX, int nY) {

    int nNumberOfSeries(m_DataSeriesArray.GetSize());
    int nNumberInSeries(GetMaxInAllSeries());
    
    int nBarWidth;
    if (!nNumberOfSeries||!nNumberInSeries)
        nBarWidth = 0;
    else
        nBarWidth = (int)((m_pScale->m_pRect->Width()/nNumberOfSeries/(nNumberInSeries)+1)*BAR_SPACING);

    if (0==nBarWidth)
        return;

    TextLocation* pTextLocation = new TextLocation[nNumberInSeries];

    int nOffset(0);
    
    int nOffset2(m_pScale->_XValueToPixel(1)-m_pScale->_XValueToPixel(0)-nBarWidth*nNumberOfSeries/2);
    
    CRect Rect;
    Rect.bottom = m_pScale->_YValueToPixel(0);
    
    CDataSeries* pDataSeries = NULL;
    CDataPoint* pDataPoint = NULL;
    CPoint pts[4];
    CPoint Temp;

    CPoint RegionPoints[6];
    
/*  the following is a diagram of the order and points used to draw the bar
    first the face is drawn, followed by the side, and finally, the top
    during this process all points are transferred to the RegionPoints array
    for creation of the region object
    
           0----------3
          /          /|
         /          / |
        3----------2  |
        |          |  |
        |          |  |
        |          |  0
        |          | /
        |          |/
        0----------1                        */
    
    for (int i(0);i<nNumberInSeries;i++) {
        nOffset = nOffset2;
        for (int j(0);j<nNumberOfSeries;j++) {
            pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(j);
            if (!pDataSeries)
                continue;
            pDataPoint = pDataSeries->GetAt(i);
            if (!pDataPoint)
                continue;

            COLORREF crColor = nNumberOfSeries>1?pDataSeries->m_crColor:pDataPoint->m_crColor;
            COLORREF Color = CreateBorderColor(crColor);
            
            CPen Pen(PS_SOLID, 1, Color);
            m_pDC->SelectObject(&Pen);
            CBrush BrushFront(SubdueColor(crColor));
            CBrush BrushSide(SubdueColor(SubdueColor(crColor)));
            CBrush BrushTop(crColor);
            m_pDC->SelectObject(&BrushFront);
            
            //first, the face...
            if (pDataPoint->m_dY<=0) {
                pts[0].y = pts[1].y = m_pScale->_YValueToPixel(m_pScale->m_dYOffset+pDataPoint->m_dY);
                pts[2].y = pts[3].y = m_pScale->_YValueToPixel(m_pScale->m_dYOffset);
                }
            else {
                pts[0].y = pts[1].y = m_pScale->_YValueToPixel(m_pScale->m_dYOffset+m_pScale->m_dYMin);
                pts[2].y = pts[3].y = m_pScale->_YValueToPixel(m_pScale->m_dYOffset+pDataPoint->m_dY);
                }
            
            pts[0].x = pts[3].x = m_pScale->_XValueToPixel(i) + nOffset;
            pts[1].x = pts[2].x = m_pScale->_XValueToPixel(i) + nOffset + nBarWidth;
            m_pDC->Polygon(pts, 4);

            //prepare the label position for this item, only if series 1 though
            if (!j&&m_wStyle&CUSTOM_X_AXIS_LABEL_TEXT) {
                CSize TextWidth(m_pDC->GetTextExtent(pDataPoint->m_sCaption));
                int nX = pts[0].x - (TextWidth.cx-(pts[1].x-pts[0].x))/2;
                if (nNumberOfSeries>1)
                    nX += (int)((nNumberOfSeries-1)/2.*(pts[1].x-pts[0].x));
                pTextLocation[i].m_nX = nX;
                pTextLocation[i].m_nY = m_pScale->_YValueToPixel(0) + 10;
                pTextLocation[i].m_nLength = TextWidth.cx;
                pTextLocation[i].Caption = pDataPoint->m_sCaption;
                }
            
            //now collect the face points for the region...
            RegionPoints[0] = pts[3];
            RegionPoints[1] = pts[0];
            RegionPoints[2] = pts[1];
            RegionPoints[3] = pts[2]; //only for the m_PopupRegion
            pDataPoint->m_PopupRegion.DeleteObject();
            pDataPoint->m_PopupRegion.CreatePolygonRgn(RegionPoints, 4, ALTERNATE/*NOT USING*/);

            /****special case --> pDataPoint->m_dY small enough for m_PopupRegion==0 <--****/
            CRect Rect;
            pDataPoint->m_PopupRegion.GetRgnBox(&Rect);
            if (pts[0].y==pts[3].y) {
                CPoint pts[4] = {RegionPoints[0], RegionPoints[1], RegionPoints[2], RegionPoints[3]};
                pts[3].x = pts[0].x;
                pts[1].x = pts[2].x = pts[0].x + nBarWidth;
                while (Rect.IsRectEmpty()) {
                    pts[2].y = pts[3].y = pts[3].y - 1;
                    pDataPoint->m_PopupRegion.DeleteObject();
                    pDataPoint->m_PopupRegion.CreatePolygonRgn(pts, 4, ALTERNATE/*NOT USING*/);
                    pDataPoint->m_PopupRegion.GetRgnBox(&Rect);
                    }
                }
            /****special case --> pDataPoint->m_dY small enough for m_PopupRegion==0 <--****/
            
            nOffset += pts[1].x-pts[0].x + 1;
            Temp = pts[3];

            m_pDC->SelectObject(&BrushSide);
            
            //now, the side...
            if (pDataPoint->m_dY<=0) {
                pts[3].y = m_pScale->_YValueToPixel(m_pScale->m_dYOffset) - nY;
                pts[0].y = m_pScale->_YValueToPixel(m_pScale->m_dYOffset+pDataPoint->m_dY) - nY;
                }
            else {
                pts[3].y = m_pScale->_YValueToPixel(pDataPoint->m_dY+m_dYOffset) - nY;
                pts[0].y = m_pScale->_YValueToPixel(m_pScale->m_dYMin+m_pScale->m_dYOffset) - nY;
                }
            
            pts[0].x = pts[3].x = pts[1].x + nX;
            m_pDC->Polygon(pts, 4);
            
            //now collect the new side points for the region...
            RegionPoints[3] = pts[0];
            RegionPoints[4] = pts[3];
            
            m_pDC->SelectObject(&BrushTop);
            
            //finally, the top...
            pts[0] = pts[1] = Temp;
            pts[0].x += nX;
            pts[0].y -= nY;
            
            m_pDC->Polygon(pts, 4);

            //now collect the new top points for the region...
            RegionPoints[5] = pts[0];

            pDataPoint->m_DetectRegion.DeleteObject();
            pDataPoint->m_DetectRegion.CreatePolygonRgn(RegionPoints, 6, ALTERNATE/*NOT USING*/);
            }
        }

    BOOL bDrawText(m_wStyle&CUSTOM_X_AXIS_LABEL_TEXT);
    for (i=1;i<nNumberInSeries&&bDrawText;i++) {
        if (pTextLocation[i-1].m_nX + pTextLocation[i-1].m_nLength + 5 > pTextLocation[i].m_nX)
            bDrawText = FALSE;
        }
    
    for (i=0;i<nNumberInSeries&&bDrawText;i++) {
        m_pDC->TextOut(pTextLocation[i].m_nX, pTextLocation[i].m_nY, pTextLocation[i].Caption);
        }

    delete [] pTextLocation;
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: GetPoint()                                                                         */
/*                                                                                              */
/* Purpose : converts an angle to a point on the edge of the pie                                */
/*                                                                                              */
/* Inputs  : CRect* pRect -> the confining rect for this pie chart                              */
/*           double dAngle -> the angle to convert                                              */
/*                                                                                              */
/* Outputs : CPoint <- the point on the pie chart which corresponds to this angle               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 10MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          10MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
CPoint GetPoint(CRect* pRect, double dAngle) {

    //assuming a unit circle and using 12 o'clock as a reference
    //point rather than 3 o'clock, the point in question 
    //has polar coordinates as follows [1, D2R(90-dAngle)]
    //converting this into cartesian coordinates gives...
    //double dx = cos(D2R(90-dAngle));
    //double dy = sin(D2R(90-dAngle));

    //now, converting this into WIN GUI coordinates gives...
    double dx =  cos(D2R(90-dAngle));
    double dy = -sin(D2R(90-dAngle));

    //scale them up to the correct radius
    dx *= pRect->Width()/2.;
    dy *= pRect->Height()/2.;
    
    int x = pRect->Width();
    int y = pRect->Height();
    
    CPoint Point((int)dx, (int)dy);
    Point.Offset(pRect->CenterPoint());
    
    return Point;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: _DrawPieChart()                                                                    */
/*                                                                                              */
/* Purpose : draws a pie chart                                                                  */
/*                                                                                              */
/* Inputs  : int nX -> x offset for depth due to rotation                                       */
/*           int nY -> y offset for depth due to rotation                                       */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
void CDataSeriesArray::_DrawPieChart(int nX, int nY) {

    CDataPoint* pDataPoint = NULL;
    CDataSeries* pDataSeries = NULL;
    CDataPoint* pSortedDataPoints = NULL;
    
    if (!m_DataSeriesArray.GetSize())
        return;
    
    pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(0);
    if (!pDataSeries->GetCount())
        return;

    pSortedDataPoints = new CDataPoint[pDataSeries->GetCount()];

    //get the sum total of all points, we need to
    //render the chart in terms of percent...
    double dTotal(0);
    for (int j(0);j<pDataSeries->GetCount();j++) {
        pDataPoint = pDataSeries->GetAt(j);
        dTotal += fabs(pDataPoint->m_dY);
        pSortedDataPoints[j] = pDataPoint;
        }

    //now sort the items
    ySortAxis = (BYTE)(Y_AXIS>>24);
    qsort(pSortedDataPoints, pDataSeries->GetCount(), sizeof(CDataPoint), ChartCompare);
    
    // Get the rect to draw in and ensure it is square...
    CRect rectClient(m_pScale->m_pRect);
    if (rectClient.Width()>rectClient.Height())
        rectClient.DeflateRect((rectClient.Width()-rectClient.Height())/2, 0);
    else if (rectClient.Height()>rectClient.Width())
        rectClient.DeflateRect(0, (rectClient.Height()-rectClient.Width())/2);

    //the CDC::Pie(CRect* r, CPoint a, CPoint b) function goes from point A 
    //to point B in a ccw direction. there is a problem when trying to draw
    //adjacent sectors, it does not completely fill in the sectors.
    //there should be no bits in between sectors if their starting and 
    //stopping points are the same for consecutive sectors. for example: 
    //drawing two sectors from 90 to 60 degrees and from 60 to 20 degrees. 
    //in this case, there should be no bits in between the 60 degree point. 
    //i have noticed that there is, this must be a flaw in the CDC class
    //among other things i have noticed this same problem with Excels version
    //of a pie chart too. here i have attempted to compensate by increasing 
    //the sector by 10 degrees and drawing over this extra bit with the next 
    //sector. only the last sector is drawn correctly.
    
    CPoint PointA;
    CPoint PointB(GetPoint(&rectClient, 350));

    //set the series' color
    double dPercent(0);
    double dAngle(0);
    int nAngle(0);
    int nPrevAngle(0);
    CRect Rect;
    CPoint* pPointArray;
    CDataPoint* pActualDataPoint;
    
    for (j=0;j<pDataSeries->GetCount();j++) {
        pDataPoint = &pSortedDataPoints[j];
        dPercent = fabs(pDataPoint->m_dY)/dTotal;
        dAngle += dPercent*360;
        nAngle = (int)dAngle;
        PointA = GetPoint(&rectClient, dAngle+(dAngle+DBL_EPSILON<359.9?10:0));

        CPen Pen(PS_SOLID, 1, CreateBorderColor(pDataPoint->m_crColor));
        CBrush Brush(pDataPoint->m_crColor);
        m_pDC->SelectObject(&Pen);
        m_pDC->SelectObject(&Brush);
        
        m_pDC->Pie(rectClient, PointA, PointB);
        
        /****generate the region****/
        if (j==0)
            PointB = GetPoint(&rectClient, 360);
        pPointArray = new CPoint[nAngle-nPrevAngle+2];
        for (int d(nPrevAngle);d<(nAngle+1);d++) {
            PointB = GetPoint(&rectClient, d);
            pPointArray[d-nPrevAngle] = PointB;
            }
        pPointArray[d-nPrevAngle] = rectClient.CenterPoint();
        //find the actual datum point in the series
        for (int j(0);j<pDataSeries->GetCount();j++) {
            pActualDataPoint = pDataSeries->GetAt(j);
            if (pActualDataPoint->m_dwID==pDataPoint->m_dwID)
                break;
            }

        pActualDataPoint->m_DetectRegion.DeleteObject();
        pActualDataPoint->m_PopupRegion.DeleteObject();
        pActualDataPoint->m_DetectRegion.CreatePolygonRgn(pPointArray, nAngle-nPrevAngle+2, ALTERNATE/*NOT USING*/);
        BOOL bPointRight(pPointArray[(nAngle-nPrevAngle+2)/2].x>rectClient.CenterPoint().x);
        BOOL bPointBelow(pPointArray[(nAngle-nPrevAngle+2)/2].y>rectClient.CenterPoint().y);
        pActualDataPoint->m_PopupRegion.CreateRectRgn(pPointArray[(nAngle-nPrevAngle+2)/2].x * (bPointRight?1:-1),
                                                      pPointArray[(nAngle-nPrevAngle+2)/2].y * (bPointBelow?1:-1),
                                                      pPointArray[(nAngle-nPrevAngle+2)/2].x+1,
                                                      pPointArray[(nAngle-nPrevAngle+2)/2].y+1);
        delete [] pPointArray;
        /****generate the region****/
        
        PointA = GetPoint(&rectClient, dAngle);
        PointB = PointA;
        nPrevAngle = nAngle;
        }

    delete [] pSortedDataPoints;
    
    return;
    }

/************************************************************************************************/
/*                                                                                              */
/* Function: _DrawRotatedPieChart()                                                             */
/*                                                                                              */
/* Purpose : draws a rotated pie chart                                                          */
/*                                                                                              */
/* Inputs  : int nX -> x offset for depth due to rotation                                       */
/*           int nY -> y offset for depth due to rotation                                       */
/*                                                                                              */
/* Outputs : NONE                                                                               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 12MAR02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          12MAR02     initial version                                            */
/* Scott Pelger          07AUG02     fixed a rendering error for low count pie charts           */
/*                                                                                              */
/************************************************************************************************/
void CDataSeriesArray::_DrawRotatedPieChart(int nX, int nY) {

    CDataPoint* pDataPoint = NULL;
    CDataSeries* pDataSeries = NULL;
    CDataPoint* pSortedDataPoints = NULL;
    
    if (!m_DataSeriesArray.GetSize())
        return;
    
    pDataSeries = (CDataSeries*)m_DataSeriesArray.GetAt(0);
    if (!pDataSeries->GetCount())
        return;

    pSortedDataPoints = new CDataPoint[pDataSeries->GetCount()];

    //get the sum total of all points, we need to
    //render the chart in terms of percent...
    double dTotal(0);
    for (int j(0);j<pDataSeries->GetCount();j++) {
        pDataPoint = pDataSeries->GetAt(j);
        dTotal += fabs(pDataPoint->m_dY);
        pSortedDataPoints[j] = pDataPoint;
        }

    //now sort the items
    ySortAxis = (BYTE)(Y_AXIS>>24);
    qsort(pSortedDataPoints, pDataSeries->GetCount(), sizeof(CDataPoint), ChartCompare);
    
    // Get the rect to draw in and make it square...
    CRect rectClient(m_pScale->m_pRect);
    if (rectClient.Width()>rectClient.Height())
        rectClient.DeflateRect((rectClient.Width()-rectClient.Height())/2, 0);
    else if (rectClient.Height()>rectClient.Width())
        rectClient.DeflateRect(0, (rectClient.Height()-rectClient.Width())/2);
    
    rectClient.DeflateRect(10, 10);

    int nHeight = (int)(rectClient.Width()/8.*m_dHeight/100);
    rectClient.DeflateRect(0, (int)(rectClient.Height()*m_dElevation/100));
    rectClient.OffsetRect(0, nHeight/2);
    CPoint CenterPoint = rectClient.CenterPoint();
    CPoint PointArray[4];

    CPoint** pPtrPointArray = new CPoint*[pDataSeries->GetCount()];
    CPoint* pPopupPointArray = new CPoint[pDataSeries->GetCount()];
    
    for (int i(0);i<3;i++) {
        PointArray[0] = CPoint(rectClient.left+rectClient.Width()/2, rectClient.top);
        PointArray[1] = CPoint(GetPoint(&rectClient, 350));
        
        //set the series' color
        CPen* pPen;
        CBrush* pBrush;
        double dPercent(0);
        double dAngle(0);
        double dPrevAngle(0);
        int nAngle(0);
        int nPrevAngle(0);
        
        for (j=0;j<pDataSeries->GetCount();j++) {
            pDataPoint = &pSortedDataPoints[j];
            dPercent = fabs(pDataPoint->m_dY)/dTotal;
            dAngle += dPercent*360;
            nAngle = (int)dAngle;
            PointArray[0] = GetPoint(&rectClient, dAngle+(dAngle+DBL_EPSILON<359.9?10:0));

            CPen Pen(PS_SOLID, 1, i!=1?CreateBorderColor(pDataPoint->m_crColor):pDataPoint->m_crColor);
            CBrush Brush(pDataPoint->m_crColor);
            pPen = m_pDC->SelectObject(&Pen);
            pBrush = m_pDC->SelectObject(&Brush);
        
            if (1!=i)
                m_pDC->Pie(rectClient, PointArray[0], PointArray[1]);
            else {
                if ((PointArray[0].y>CenterPoint.y||PointArray[1].y>CenterPoint.y)&&PointArray[0].x<PointArray[1].x) {
                    PointArray[0] = GetPoint(&rectClient, dAngle);
                    PointArray[2] = PointArray[1];
                    PointArray[2].y -= nHeight;
                    PointArray[3] = PointArray[0];
                    PointArray[3].y -= nHeight;
                    if (dAngle+DBL_EPSILON>=270&&dPrevAngle+DBL_EPSILON<180)
                        PointArray[0].y = PointArray[1].y;
                    m_pDC->Polygon(PointArray, 4);
                    }
                //special case, the 90 degree point
                if ((dPrevAngle+DBL_EPSILON<90)&&(dAngle+DBL_EPSILON>90)) {
                    PointArray[1] = PointArray[0] = GetPoint(&rectClient, 90);
                    PointArray[1].y -= nHeight;
                    PointArray[2] = PointArray[1];
                    PointArray[2].x -= nHeight;
                    PointArray[3] = PointArray[0];
                    PointArray[3].x -= nHeight;
                    m_pDC->Polygon(PointArray, 4);
                    }
                //special case, the 270 degree point
                else if ((dPrevAngle+DBL_EPSILON<270)&&(dAngle+DBL_EPSILON>270)) {
                    CPoint LimitPoint = GetPoint(&rectClient, dPrevAngle);
                    PointArray[1] = PointArray[0] = GetPoint(&rectClient, 270);
                    PointArray[1].y -= nHeight;
                    PointArray[2] = PointArray[1];
                    PointArray[2].x = LimitPoint.x;
                    PointArray[3] = PointArray[0];
                    PointArray[3].x = LimitPoint.x;
                    m_pDC->Polygon(PointArray, 4);
                    }
                }

            if (2==i&&(dPrevAngle+DBL_EPSILON>90)&&(dPrevAngle+DBL_EPSILON<270)) {
                m_pDC->MoveTo(PointArray[1]);
                PointArray[1].y += nHeight+1;
                m_pDC->LineTo(PointArray[1]);
                }
            
            _GenerateRegion(i, j, nPrevAngle, nAngle, nHeight, &rectClient, pPopupPointArray, pPtrPointArray, pDataSeries, pDataPoint);
            
            m_pDC->SelectObject(pPen);
            m_pDC->SelectObject(pBrush);
            
            PointArray[1] = GetPoint(&rectClient, dAngle);
            dPrevAngle = dAngle;
            nPrevAngle = nAngle;
            }

        if (1==i)
            rectClient.OffsetRect(0, -nHeight);
        }

    delete [] pPopupPointArray;
    delete [] pPtrPointArray;
    delete [] pSortedDataPoints;
    
    return;
    }

void CDataSeriesArray::_GenerateRegion(int i, int j, int nPrevAngle, int nAngle, int nHeight, CRect* RectClient, CPoint* pPopupPointArray, CPoint** pPtrPointArray, CDataSeries* pDataSeries, CDataPoint* pDataPoint) {

    CPoint* pPointArray;
    CDataPoint* pActualDataPoint;

    if (i==0) {
        if (nPrevAngle>=90&&nAngle>90&&nPrevAngle<270&&nAngle<=270) {
            //all sectors between 90 and 270 degrees
            pPointArray = new CPoint[nAngle-nPrevAngle+2+2];
            pPointArray[0] = GetPoint(RectClient, nPrevAngle);
            pPointArray[0].y -= nHeight+1;
            
            for (int d(nPrevAngle);d<(nAngle+1);d++)
                pPointArray[d-nPrevAngle+1] = GetPoint(RectClient, d);
            pPointArray[d-nPrevAngle+1] = GetPoint(RectClient, nAngle+1);
            pPointArray[d-nPrevAngle+1].y -= nHeight+1;
            pPointArray[d-nPrevAngle+2] = RectClient->CenterPoint();
            pPointArray[d-nPrevAngle+2].y -= nHeight+1;
            }
        else if (nPrevAngle<90&&nAngle>90) {
            //the one sector that spans 90 degrees
            pPointArray = new CPoint[nAngle-nPrevAngle+2+2];
            for (int d(nPrevAngle);d<91;d++) {
                pPointArray[d-nPrevAngle] = GetPoint(RectClient, d);
                pPointArray[d-nPrevAngle].y -= nHeight+1;
                }
            for (d=90;d<(nAngle+1);d++)
                pPointArray[d-nPrevAngle+1] = GetPoint(RectClient, d);
            pPointArray[d-nPrevAngle+1] = GetPoint(RectClient, nAngle+1);
            pPointArray[d-nPrevAngle+1].y -= nHeight+1;
            pPointArray[d-nPrevAngle+2] = RectClient->CenterPoint();
            pPointArray[d-nPrevAngle+2].y -= nHeight+1;
            }
        else if (nPrevAngle<270&&nAngle>270) {
            CRect Rect;
            //the one sector that spans 270 degrees
            pPointArray = new CPoint[nAngle-nPrevAngle+2+2];
            pPointArray[0] = GetPoint(RectClient, nPrevAngle);
            pPointArray[0].y -= nHeight+1;
            for (int d(nPrevAngle);d<271;d++)
                pPointArray[d-nPrevAngle+1] = GetPoint(RectClient, d);
            for (d=270;d<(nAngle+1);d++) {
                pPointArray[d-nPrevAngle+1+1] = GetPoint(RectClient, d);
                pPointArray[d-nPrevAngle+1+1].y -= nHeight+1;
                }
            pPointArray[d-nPrevAngle+2] = RectClient->CenterPoint();
            pPointArray[d-nPrevAngle+2].y -= nHeight+1;
            }
        else
            pPointArray = new CPoint[nAngle-nPrevAngle+2];
        pPtrPointArray[j] = pPointArray;
        
        pPopupPointArray[j] = GetPoint(RectClient, nPrevAngle+(nAngle-nPrevAngle)/2);
        pPopupPointArray[j].y -= nHeight+1;
        }
    if (i==2) {
        int nNumInArray(nAngle-nPrevAngle+2);
        pPointArray = pPtrPointArray[j];
        if ((nPrevAngle>=90&&nAngle>90&&nPrevAngle<270&&nAngle<=270)||
            (nPrevAngle<90&&nAngle>90)||
            (nPrevAngle<270&&nAngle>270)) {
            nNumInArray += 2;
            }
        else {
            for (int d(nPrevAngle);d<(nAngle+1);d++)
                pPointArray[d-nPrevAngle] = GetPoint(RectClient, d);
            pPointArray[d-nPrevAngle] = RectClient->CenterPoint();
            pPopupPointArray[j] = GetPoint(RectClient, nPrevAngle+(nAngle-nPrevAngle)/2);
            }
        
        //find the actual datum point in the series
        for (int jj(0);jj<pDataSeries->GetCount();jj++) {
            pActualDataPoint = pDataSeries->GetAt(jj);
            if (pActualDataPoint->m_dwID==pDataPoint->m_dwID)
                break;
            }

        pActualDataPoint->m_DetectRegion.DeleteObject();
        pActualDataPoint->m_PopupRegion.DeleteObject();
        pActualDataPoint->m_DetectRegion.CreatePolygonRgn(pPointArray, nNumInArray, ALTERNATE/*NOT USING*/);
        BOOL bPointRight(pPopupPointArray[j].x>RectClient->CenterPoint().x);
        BOOL bPointBelow(pPopupPointArray[j].y>RectClient->CenterPoint().y);
        pActualDataPoint->m_PopupRegion.CreateRectRgn(pPopupPointArray[j].x * (bPointRight?1:-1),
                                                      pPopupPointArray[j].y * (bPointBelow?1:-1),
                                                      pPopupPointArray[j].x+1,
                                                      pPopupPointArray[j].y+1);
        delete [] pPointArray;
        }

    }

//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/DataSeries.cpp $
// 
// 1     8/30/02 3:06p Scott.pelger
// initial version
//***********************************************************************************************
