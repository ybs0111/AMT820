#ifndef PART_FUNCTION_H
#define PART_FUNCTION_H

#include <DbgHelp.h>
#include "Variable.h"

enum ENUM_MODULE_POSITION
{
	MPOS_NONE = -1,
	MPOS_INDEX_TESTSITE = 0,
	MPOS_INDEX_BIN1 = 100,
	MPOS_INDEX_BIN2 = 200,
	MPOS_INDEX_BIN3	= 300,
	MPOS_INDEX_REJ	= 400,
	MPOS_INDEX_COK_STACKER = 500,
//	MPOS_INDEX_REJ	= 300,
//	MPOS_INDEX_COK_STACKER = 400,
	MPOS_INDEX_JIG	= 600,
};
enum
{
	MPOS_INDEX_SITE1,
	MPOS_INDEX_SITE2,
	MPOS_INDEX_COK_SITE1_0,
	MPOS_INDEX_COK_SITE1_1,
	MPOS_INDEX_COK_SITE2_0,
	MPOS_INDEX_COK_SITE2_1,
};

enum EMODULE_STATE
{
	EMS_NONE,
	//EMS_BCR, //ybs
	EMS_LOAD,
	EMS_READY,
	EMS_READY_RECV,
	EMS_START,
	EMS_TEST,
	EMS_ABORT,
	EMS_ABORT_RECV,
	EMS_GOOD,
	EMS_FAIL,
	EMS_RELOAD,
	EMS_MAXCOUNT,
	EMS_COUNTINUE,
	EMS_OPS,
	EMS_REPAIR,
};

#define MPOS_DIVISION	100
	
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)( // Callback 함수의 원형
	HANDLE hProcess, 
	DWORD dwPid, 
	HANDLE hFile, 
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

void CaptureScreenShot();

HANDLE DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal);
bool WriteDIB(LPCSTR szFile, HANDLE hDIB);
LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS *exceptionInfo);

int DoModal_Msg( CString strMsg );
int DoModal_Select( CString strMsg );

COLORREF GetCokCol( int iIdx );

double GetPosCYAtCokStacker( int iIdx );
double GetPosCZByCnt( int iCnt );

void ReadBasicData_Int( int& nRead, CString strKey, int nInit, int nMin, int nMax, LPCSTR strFile );
void ReadBasicData_Double( double& dRead, CString strKey, double dInit, double dMin, double dMax , LPCSTR strFile );


int GetMdlIdx( int ix, int iy, int iw, int ih, bool bTurn );
//COLORREF CalcModuleColor( AModule mdl );
//COLORREF CalcModuleColor( EMODULE_STATE ems );

int GetMatrixXMdlIdx();
int GetMatrixYMdlIdx();


class AModule
{
public:
	AModule() { EraseData(); }
	virtual ~AModule() {}

	// Get
	CString GetSN() { return m_strSN; }
	CString GetSerial() { return m_strSerial; }
	CString GetPPID() { return m_strPPID; }
	CString GetWWN() { return m_strWWN; }
	CString GetCSN() { return m_strCSN; }
	CString GetPSID() { return m_strPSID; }
	CString GetBin() { return m_strBin; }
	CString GetLotNo() { return m_strLotNo; }
	CString GetCSN2() { return m_strCSN2; }
	EMODULE_STATE GetModuleState() { return m_ems; }
	int		GetTestCnt() { return m_nTestCnt; }
	int 	GetScrapeCnt() { return m_nScrap_retry; }

	CString GetScrapCode() { return m_strScrapCode; }

	bool IsExist() { return ((m_strSerial != "") && (m_strSerial != "NONE") && (m_strSerial != "NULL")); }
	bool IsTestEnd() { return (IsExist() == false || atoi(m_strBin) > 0 ); }
	bool IsTestEndState() { return (m_ems == EMS_GOOD || m_ems == EMS_FAIL); }
	bool IsTestLoadState() { return ( m_ems == EMS_LOAD || m_ems == EMS_RELOAD); }

	CString GetStrState()
	{
		switch( m_ems )
		{
		case EMS_NONE:				return "NONE";
		case EMS_LOAD:				return "LOAD";
		case EMS_READY:				return "READY";
		case EMS_READY_RECV:		return "READY_RECV";
		case EMS_START:				return "START";
		case EMS_TEST:				return "TEST";
		case EMS_ABORT:				return "ABORT";
		case EMS_ABORT_RECV:		return "ABORT_RECV";
		case EMS_GOOD:				return "GOOD";
		case EMS_FAIL:				return "FAIL";
		case EMS_RELOAD:			return "RELOAD";
		}
		return "NONE";
	}


	// Set
	void SetSN( CString strSN ) { m_strSN = strSN; }
	void SetSerial( CString strSerial ) { m_strSerial = strSerial; }
	void SetPPID( CString strPPID ) { m_strPPID = strPPID; }
	void SetWWN( CString strWWN ) { m_strWWN = strWWN; }
	void SetCSN( CString strCSN ) { m_strCSN = strCSN; }
	void SetPSID( CString strPSID ) { m_strPSID = strPSID; }
	void SetBin( CString strBin ) { m_strBin = strBin; }
	void SetModuleState( EMODULE_STATE ems ) { m_ems = ems; }
	void SetScrapCode( CString strCode ) { m_strScrapCode = strCode; }
	void SetLotNo( CString strLotNo ) { m_strLotNo = strLotNo; }
	void SetCSN2( CString strCSN2 ) { m_strCSN2 = strCSN2; }
	void CalcModuleState_TestEnd()
	{
		if( IsExist() )
		if( atoi(m_strBin) > 3 )
			m_ems = EMS_FAIL;
		else
			m_ems = EMS_GOOD;

	}

	void CalcModuleState_PlaceTray()
	{
////2015.0604
// 		if( m_ems == EMS_FAIL && m_nTestCnt <= st_handler.mn_retest_cnt )
// 		{
// 			m_ems = EMS_RELOAD;
// 			m_strBin = "";
// 		}

		/*if(st_basic.mn_mode_ftp == 0)//not use
		{
			if( m_ems == EMS_FAIL && m_nTestCnt <= st_handler.mn_retest_cnt )
			{
				m_ems = EMS_RELOAD;
				m_strBin = "";
			}
		}
		else if(st_basic.mn_mode_ftp == 1 || st_basic.mn_mode_ftp == 2)//FTP || //Local
		{
			if( m_ems == EMS_FAIL && m_nTestCnt <= m_nScrap_retry)//st_handler.mn_scrap_code_retry[0] )
			{
				m_ems = EMS_RELOAD;
				m_strBin = "";
			}
			else if( m_ems == EMS_FAIL && m_nTestCnt >= m_nScrap_retry)//st_handler.mn_scrap_code_retry[0] )//2016.0711
			{
				m_nTestCnt = st_handler.mn_retest_cnt + 1;
			}
			
		}*/	
////

	}

// 	void EraseData() { m_strSN = "";	m_strSerial = "";	m_strPPID = "";	m_strWWN = "";	m_strBin = ""; m_ems = EMS_NONE; m_nTestCnt = 0;
// 						m_strScrapCode = ""; m_strLotNo = ""; }
	void EraseData() { m_strSN = "";	m_strSerial = "";	m_strPPID = "";	m_strWWN = "";	m_strBin = ""; m_ems = EMS_NONE; m_nTestCnt = 0;
	m_strScrapCode = ""; m_strLotNo = ""; m_strCSN = ""; m_strPSID = ""; m_strCSN2 = ""; }

// 	void SetModuleInfo( CString strLotNo, CString strSN, CString strSerial, CString strPPID, CString strWWN, CString strBin , EMODULE_STATE ems = EMS_LOAD )
// 		{	m_strLotNo = strLotNo; m_strSN = strSN;	m_strSerial = strSerial;	m_strPPID = strPPID;	m_strWWN = strWWN;	m_strBin = strBin;	m_ems = ems; }
	void SetModuleInfo( CString strLotNo, CString strSN, CString strSerial, CString strPPID, CString strWWN, CString strCSN, CString strPSID, CString strBin , CString strCSN2 = "", EMODULE_STATE ems = EMS_LOAD )
	{	m_strLotNo = strLotNo; m_strSN = strSN;	m_strSerial = strSerial;	m_strPPID = strPPID;	m_strWWN = strWWN;	m_strCSN = strCSN; m_strPSID = strPSID; m_strBin = strBin;	m_strCSN2 = strCSN2; m_ems = ems; }

	void SetTestedPos( int nIdx ) { m_nTestedPos[m_nTestCnt] = nIdx; }
	void AddTestCnt() { m_nTestCnt += 1; }
	void SetTestCnt( int nCnt ) { m_nTestCnt = nCnt; }
	void SetScrapeCnt( int nCnt ) { m_nScrap_retry = nCnt; }

	int GetTestedPos( int nTestIdx ) { return m_nTestedPos[nTestIdx]; }


protected:
	CString m_strSN;
	CString m_strSerial;
	CString m_strPPID;
	CString m_strWWN;
	CString m_strCSN;
	CString m_strPSID;
	CString m_strBin;
	CString m_strScrapCode;
	CString	m_strCSN2;

	EMODULE_STATE m_ems;

	int		m_nTestCnt;
	int		m_nScrap_retry;

	CString m_strLotNo;

	int		m_nTestedPos[20];
};

EMODULE_STATE GetStateByStr( CString strState );


#endif