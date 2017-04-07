// DataManager.cpp: implementation of the DataManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "handler.h"
#include "DataManager.h"

//#include "../Run_Network.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataManager g_dataMgr;

void AData::InitData()
{
	m_pData = NULL;
	m_dt = DT_BOOL;
	
	m_bData = false;
	m_nData = -1;
	m_strData = "";
}

void AData::UpdateData()
{
	switch( m_dt )
	{
	case DT_BOOL:
		{
			bool* pbTemp;
			pbTemp = (bool*)m_pData;
			*pbTemp = m_bData;
		}
		break;

	case DT_INT:
		{
			int* piTemp;
			piTemp = (int*)m_pData;
			*piTemp = m_nData;
		}
		break;

	case DT_DOUBLE:
		{
			double* pdTemp;
			pdTemp = (double*)m_pData;
			*pdTemp = m_dData;
		}
		break;

	case DT_STR:
		{
			CString* pstrTemp;
			pstrTemp = (CString*)m_pData;
			*pstrTemp = m_strData;
		}
		break;
	}
}

void AData::BackupData()
{
	switch( m_dt )
	{
	case DT_BOOL:
		{
			bool* pbTemp;
			pbTemp = (bool*)m_pData;

			m_bData = *pbTemp;
		}
		break;

	case DT_INT:
		{
			int* piTemp;
			piTemp = (int*)m_pData;
			m_nData = *piTemp;
		}
		break;

	case DT_DOUBLE:
		{
			double* pdTemp;
			pdTemp = (double*)m_pData;
			m_dData = *pdTemp;
		}
		break;

	case DT_STR:
		{
			CString* pstrTemp;
			pstrTemp = (CString*)m_pData;
			m_strData = *pstrTemp;
		}
		break;
	}
}

void* AData::pGetData()
{
	switch( m_dt )
	{
		case DT_BOOL:	return &m_bData;
		case DT_INT:	return &m_nData;
		case DT_DOUBLE:	return &m_dData;
		case DT_STR:	return &m_strData;
	}

	return &m_bData;
}

void AData::CheckAndLog()
{
	switch( m_dt )
	{
		case DT_BOOL:
			{
				bool* pbData = (bool*)m_pData;
				if( *pbData != m_bData )
				{
					CString strLog;
					CString strBackup = m_bData ? "TRUE" : "FALSE";
					CString strOrg = *pbData ? "TRUE" : "FALSE";
					strLog.Format( "{%s}	[%s] -----> [%s]", m_strLogHead, strOrg, strBackup );
					Func.On_LogFile_Add(LOG_OPERATION, strLog);
					Func.On_LogFile_Add(LOG_TOTAL, strLog);
				}
			}
			break;

		case DT_INT:
			{
				int* piData = (int*)m_pData;
				if( *piData != m_nData )
				{
					CString strLog;
					strLog.Format( "{%s}	[%d] -----> [%d]", m_strLogHead, *piData, m_nData );
					Func.On_LogFile_Add(LOG_OPERATION, strLog);
					Func.On_LogFile_Add(LOG_TOTAL, strLog);
				}
			}
			break;

		case DT_DOUBLE:
			{
				double* pdData = (double*)m_pData;
				if( *pdData != m_dData )
				{
					CString strLog;
					strLog.Format( "{%s}	[%.03f] -----> [%.03f]", m_strLogHead, *pdData, m_dData );
					Func.On_LogFile_Add(LOG_OPERATION, strLog);
					Func.On_LogFile_Add(LOG_TOTAL, strLog);
				}
			}
			break;

		case DT_FLOAT:
			{
				float* pfData = (float*)m_pData;
				if( *pfData != m_fData )
				{
					CString strLog;
					strLog.Format( "{%s}	[%.03f] -----> [%.03f]", m_strLogHead, *pfData, m_dData );
					Func.On_LogFile_Add(LOG_OPERATION, strLog);
					Func.On_LogFile_Add(LOG_TOTAL, strLog);
				}
			}
			break;

		case DT_STR:
			{
				CString* pstrData = (CString*)m_pData;
				if( *pstrData != m_strData )
				{
					CString strLog;
					strLog.Format( "{%s}	[%s] -----> [%s]", m_strLogHead, *pstrData, m_strData );
					Func.On_LogFile_Add(LOG_OPERATION, strLog);
					Func.On_LogFile_Add(LOG_TOTAL, strLog);
				}
			}
			break;
	}
}

DataManager::DataManager()
{
	MappingDatas();
}

DataManager::~DataManager()
{

}

void DataManager::MappingDatas()
{
	m_vecData.clear();
	AData dataTemp;
	
	/*
	// INT
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iRunMode, DT_INT, "RUN MODE" );								m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iDeviceMode, DT_INT, "DEVICE MODE" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iDeviceType, DT_INT, "DEVICE TYPE" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iPressMode, DT_INT, "PRESS MODE" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iWorkMode, DT_INT, "WORK MODE" );								m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iFxr_retest, DT_INT, "FLEXIBLE RETEST" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iAuto_retest_zone, DT_INT, "AUTO RETEST ZONE" );				m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mn_alarm_delay_time, DT_INT, "ALARM DELAY TIME" );				m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iNetworkWaitTime, DT_INT, "NETWORK WAIT TIME" );				m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_nNetworkRetryCnt, DT_INT, "NETWORK RETRY CNT" );				m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iNetworkRetryTime, DT_INT, "NETWORK RETRY TIME" );			m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iNtoTime, DT_INT, "NTO TIME" );								m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iBtoTime, DT_INT, "BTO TIME" );								m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iHtoTime, DT_INT, "HTO TIME" );								m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iHtoTimeMax, DT_INT, "HTO TIME MAX" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_dHtoTimeMaxRate, DT_DOUBLE, "HTO TIME RATE" );				m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_nPressCount, DT_INT, "PRESS CNT" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_nEjectCount, DT_INT, "EJECT CNT" );							m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_nRtCntHto, DT_INT, "RETEST COUNT HTO" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_nRtCntBto, DT_INT, "RETEST COUNT BTO" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_nRtCntPpr, DT_INT, "RETEST COUNT PPR" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_nRtCntNto, DT_INT, "RETEST COUNT NTO" );						m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_nRtGiveUpCntNto, DT_INT, "RETEST GIVE UP COUNT NTO" );		m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_nRtGiveUpCntOther, DT_INT, "RETEST GIVE UP COUNT OTHER" );	m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iRtEnbRz2ea, DT_INT, "RETEST ENABLE EZ2EA" );					m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iRtEnbNoRt, DT_INT, "RETEST ENABLE NO RETEST" );				m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iRetestMode, DT_INT, "RETEST MODE" );							m_vecData.push_back( dataTemp );

	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iUseTempControl, DT_INT, "TEMP CONTROL" );					m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iTempReadTime, DT_INT, "TEMP READ" );							m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iHeaterCondition1, DT_INT, "HEATER CONDITION1" );				m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iHeaterCondition3, DT_INT, "HEATER CONDITION3" );				m_vecData.push_back( dataTemp );
//	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iHISMode, DT_INT, "HIS MODE" );								m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iXGemMode, DT_INT, "XGEM MODE" );								m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iRcmdMode, DT_INT, "RCMD MODE" );								m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_nAutoSocketOffContiCount, DT_INT, "ASOCKETOFF_CONTI_CNT" );	m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.m_iAutoSocketOffMode, DT_INT, "ASOCKETOFF_MODE" );				m_vecData.push_back( dataTemp );

	// DOUBLE
//	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.md_electrostatic_value, DT_DOUBLE, "ELEC VALUE" );			m_vecData.push_back( dataTemp );

	// STRING
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_basic.mstr_device_name, DT_STR, "DEVICE NAME" );						m_vecData.push_back( dataTemp );
	dataTemp.InitData();	dataTemp.MappingData( (void*) &st_path.m_strStationLog, DT_STR, "STATION LOG PATH" );					m_vecData.push_back( dataTemp );

*/
}

// 디바이스네임은 업데이트 하지 않는다.
void DataManager::UpdateDatas()
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == &st_basic.mstr_device_name )
			continue;
		
		m_vecData.at(i).UpdateData();
	}
}

void DataManager::UpdateData( void* pData )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == pData )
			m_vecData.at(i).UpdateData();
	}
}

void DataManager::BackupDatas()
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		m_vecData.at(i).BackupData();
	}
}

void DataManager::SetDataTemp( void* pData, bool bVal )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == pData )
			m_vecData.at(i).SetData( bVal );
	}
}

void DataManager::SetDataTemp( void* pData, int iVal )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == pData )
			m_vecData.at(i).SetData( iVal );
	}
}

void DataManager::SetDataTemp( void* pData, double dVal )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == pData )
			m_vecData.at(i).SetData( dVal );
	}
}

void DataManager::SetDataTemp( void* pData, CString strVal )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == pData )
			m_vecData.at(i).SetData( strVal );
	}
}

bool DataManager::GetBackupData( bool* pbOrg )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == (void*)pbOrg )
		{
			bool* pRtn;
			pRtn = (bool*)m_vecData.at(i).pGetData();
			return *pRtn;
		}
	}

	return false;
}

int DataManager::GetBackupData( int* piOrg )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == (void*)piOrg )
		{
			int* pRtn;
			pRtn = (int*)m_vecData.at(i).pGetData();
			return *pRtn;
		}
	}

	return -1;
}

double DataManager::GetBackupData( double* pdOrg )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == (void*)pdOrg )
		{
			double* pRtn;
			pRtn = (double*)m_vecData.at(i).pGetData();
			return *pRtn;
		}
	}

	return -1;
}

float DataManager::GetBackupData( float* pfOrg )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == (void*)pfOrg )
		{
			float* pRtn;
			pRtn = (float*)m_vecData.at(i).pGetData();
			return *pRtn;
		}
	}
	
	return -1;
}
	
CString DataManager::GetBackupData( CString* pstrOrg )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == (void*)pstrOrg )
		{
			CString* pRtn;
			pRtn = (CString*)m_vecData.at(i).pGetData();
			return *pRtn;
		}
	}

	return "";
}

void DataManager::CheckAndLogAll()
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		m_vecData.at(i).CheckAndLog();
	}
}

void DataManager::CheckAndLogByPointer( void* pData )
{
	int iCnt = m_vecData.size();
	for( int i=0; i<iCnt; i++ )
	{
		if( m_vecData.at(i).GetOrgDataPointer() == pData )
		{
			m_vecData.at(i).CheckAndLog();
			return;
		}
	}
}