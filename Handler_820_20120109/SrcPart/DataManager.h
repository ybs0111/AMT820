// DataManager.h: interface for the DataManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAMANAGER_H__6645F632_7E6F_4138_9938_EE6EA3180EDD__INCLUDED_)
#define AFX_DATAMANAGER_H__6645F632_7E6F_4138_9938_EE6EA3180EDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

enum DATA_TYPE
{
	DT_BOOL,
	DT_INT,
	DT_DOUBLE,
	DT_STR,
	DT_FLOAT,
};

class AData
{
public:

	void InitData();
	void MappingData( void* pData, DATA_TYPE dt, CString strHead ) { m_pData = pData; m_dt = dt; m_strLogHead = strHead; }
	void UpdateData();
	void BackupData();
	void SetData( bool bVal )		{ m_bData = bVal; }
	void SetData( int iVal )		{ m_nData = iVal; }
	void SetData( double dVal )		{ m_dData = dVal; }
	void SetData( float fVal )		{ m_fData = fVal; }
	void SetData( CString strVal )	{ m_strData = strVal; }

	void* pGetData();
	void* GetOrgDataPointer() { return m_pData; }

	DATA_TYPE GetDataType() { return m_dt; }

	void CheckAndLog();

protected:
	void* m_pData;
	DATA_TYPE m_dt;
	
	bool m_bData;
	int m_nData;
	double m_dData;
	CString m_strData;
	float m_fData;

	CString m_strLogHead;

};

class DataManager  
{
public:
	DataManager();
	virtual ~DataManager();

	void UpdateDatas();
	void UpdateData( void* pData );

	void BackupDatas();
	void SetDataTemp( void* pData, bool bVal );
	void SetDataTemp( void* pData, int iVal );
	void SetDataTemp( void* pData, double dVal );
	void SetDataTemp( void* pData, CString strVal );

	bool GetBackupData( bool* pbOrg );
	int GetBackupData( int* piOrg );
	double GetBackupData( double* pdOrg );
	float GetBackupData( float* pfOrg );
	CString GetBackupData( CString* pstrOrg );

	void CheckAndLogAll();
	void CheckAndLogByPointer( void* pData );

protected:
	void MappingDatas();
	std::vector<AData>	m_vecData;
};

extern DataManager g_dataMgr;

#endif // !defined(AFX_DATAMANAGER_H__6645F632_7E6F_4138_9938_EE6EA3180EDD__INCLUDED_)
