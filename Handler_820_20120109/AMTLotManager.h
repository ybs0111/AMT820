// AMTLotManager.h: interface for the AMTLotManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_)
#define AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum
{
	PRIME,
	CUM,
	CNT_TYPE_MAX,
};

#define BIN_MAX			9
#define TEST_MAX_CNT	8
// ALot
class ALot
{
public:
	ALot();
	ALot( CString strLotID, CString strPartID );
	virtual ~ALot();

	CString GetLotID() { return m_strLotID; }
	CString GetPartID() { return m_strPartID; }
	CString GetStrNewLotInPass() { if( m_bNewLotInPass ) return "YES"; else return "NO"; }
	bool GetNewLotInPass() { return m_bNewLotInPass; }
	CString GetTimeStart();
	CString GetStrLastModule() { return m_strLastModule; }
	CString GetStrLastUnloadModule() { return m_strLastUnloadModule; }
	//2013,0910
	CString GetStrRProtyModule() { return m_strRProtyModule; }
	CString GetStrProcessidModule() { return m_strProcessidModule; }
	CString GetStrBypass() { return m_strBypass; }

	CString GetAutoTimeOut() { return m_strAutoTimeout; }



	void SetNewLotInPass(bool bVal) { m_bNewLotInPass = bVal; }
	void SetLotID( CString strLotID ) {strLotID.MakeUpper(); m_strLotID = strLotID; }
	void SetPartID( CString strPartID ) { strPartID.MakeUpper(); m_strPartID = strPartID; }
	void SetLastModule( CString strLastModule )	{m_strLastModule = strLastModule; }
	void SetLastUnloadModule( CString strLastUnloadModule )	{ m_strLastUnloadModule = strLastUnloadModule; }
	void SetRProtyModule( CString strRProtyModule )	{m_strRProtyModule = strRProtyModule; }
	void SetProcessidModule( CString strProcessidModule )	{m_strProcessidModule = strProcessidModule; }
	void SetByPass( CString strBypass ) { m_strBypass = strBypass; }
	void SetAutoTimeOut( CString strAutoTimeOut ) { m_strAutoTimeout = strAutoTimeOut; }

	void SetTimeOut( int nTimeout) { m_nTimeOut = nTimeout; }
	void SetLotCount( int nLotDvcCnt) { m_nDvcLotCount = nLotDvcCnt; }


	void AddInputCnt( int nPrimeCum );
	void AddPassCnt( int nPrimeCum );
	void AddFailCnt( int nPrimeCum );
	void AddBinCnt( int nBin );


	void AddTimeOutCnt() { m_nCntTimeout++; }
	void SetTimeOutCnt(int nTime) { m_nCntTimeout = nTime; }

	void SetEachTestTimeOut(int nCnt, int nTime) { m_nEachTimeout[nCnt] = nTime; } 

	int GetInputCnt( int nPrimeCum ) { return m_nCntInput[nPrimeCum]; }
	int GetPassCnt( int nPrimeCum ) { return m_nCntPass[nPrimeCum]; }
	int GetFailCnt( int nPrimeCum ) { return m_nCntFail[nPrimeCum]; }
	
	int GetBinCnt( int nBin ) { return m_nCntBin[nBin]; }

	//2016.0112
	int GetTimeOutCnt() { return m_nCntTimeout; }
	int GetTimeOut() { return m_nTimeOut; }
	int GetEachTestTimeOut(int nCnt) {  return m_nEachTimeout[nCnt]; } 


	int GetCokType() { return m_nCokType; }
	void SetCokType( int iCok ) { m_nCokType = iCok; }
	void CalcCokType();

	int GetLotCount() { return m_nDvcLotCount; }


protected:
	CString m_strLotID;
	CString m_strPartID;
	bool m_bNewLotInPass;

	SYSTEMTIME	m_timeStart;
	CString m_strLastModule;
	CString m_strLastUnloadModule;
	CString m_strRProtyModule;//2013,0910
	CString m_strProcessidModule;//2013,0910
	CString m_strBypass;

	//2016.0112
	CString m_strAutoTimeout;
	int	m_nCntTimeout;
	int m_nTimeOut;

	int m_nDvcLotCount;


	int m_nCokType;

	// Yield Info
	int m_nCntInput[CNT_TYPE_MAX];
	int m_nCntPass[CNT_TYPE_MAX];
	int m_nCntFail[CNT_TYPE_MAX];

	int m_nCntBin[BIN_MAX];

	int m_nEachTimeout[20];
};


// AMTLotManager
class AMTLotManager  
{
public:
	AMTLotManager();
	virtual ~AMTLotManager();

	void OnNewLotIn( CString strLotID, CString strPartID );
	CString GetLotIDAt( int iIdx );
	ALot& GetLotAt( int iIdx );
	ALot& GetLotByLotID( CString strLotID );
	int GetLotCount() { return m_vecLot.size(); }


	void AddLot( CString strLotID, CString strPartID );
	void DeleteLot( int iIdx );

	void AddInputCnt( int nPrimeCum );
	void AddPassCnt( int nPrimeCum );
	void AddFailCnt( int nPrimeCum );
	void AddBinCnt( int nBin );

	int GetInputCnt( int nPrimeCum ) { return m_nCntInput[nPrimeCum]; }
	int GetPassCnt( int nPrimeCum ) { return m_nCntPass[nPrimeCum]; }
	int GetFailCnt( int nPrimeCum ) { return m_nCntFail[nPrimeCum]; }

	int GetBinCnt( int nBin ) { return m_nCntBin[nBin]; }

	void ClearCountData();

	//2013,0913
	void OnNewPLotin(CString strLotID, CString strPartID );
	void AddPLot( CString strLotID, CString strPartID );
	int IsGetPLotID( CString strLotID);
	int GetPLotCount() { return m_vecPLot.size(); }
	ALot& GetPLotAt( int iIdx );
	void DeletePLot(CString strLotID);

protected:
	std::vector< ALot > m_vecLot;
	std::vector< ALot > m_vecPLot;
	ALot	m_dummyLot;
	ALot	m_dummyPLot;

	// Yield Info
	int m_nCntInput[CNT_TYPE_MAX];
	int m_nCntPass[CNT_TYPE_MAX];
	int m_nCntFail[CNT_TYPE_MAX];

	int m_nCntBin[BIN_MAX];
};

extern AMTLotManager g_lotMgr;

#endif // !defined(AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_)
