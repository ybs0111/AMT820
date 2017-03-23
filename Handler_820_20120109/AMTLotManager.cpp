// AMTLotManager.cpp: implementation of the AMTLotManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "AMTLotManager.h"
#include "AMTRegistry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AMTLotManager g_lotMgr;
ALot::ALot()
{
	m_bNewLotInPass = false;
	GetLocalTime(&m_timeStart);

	m_nCokType = -1;

	m_nCntInput[PRIME] = 0;
	m_nCntPass[PRIME] = 0;
	m_nCntFail[PRIME] = 0;

	m_nCntInput[CUM] = 0;
	m_nCntPass[CUM] = 0;
	m_nCntFail[CUM] = 0;

	for( int i=0; i<BIN_MAX; i++ )
	{
		m_nCntBin[i] = 0;
	}
}

ALot::ALot( CString strLotID, CString strPartID )
{
	m_strLotID = strLotID; 
	m_strPartID = strPartID;

	m_bNewLotInPass = false;
	GetLocalTime(&m_timeStart);

	m_strLastModule = "NO";
	m_strRProtyModule = "NO";
	m_strBypass = "NO";
	m_strAutoTimeout = "NO";
	m_nCntTimeout = 0;
	m_nTimeOut = 0;

	m_nCokType = -1;

	m_nCntInput[PRIME] = 0;
	m_nCntPass[PRIME] = 0;
	m_nCntFail[PRIME] = 0;

	m_nCntInput[CUM] = 0;
	m_nCntPass[CUM] = 0;
	m_nCntFail[CUM] = 0;

	for( int i=0; i<BIN_MAX; i++ )
	{
		m_nCntBin[i] = 0;
	}
}

ALot::~ALot()
{

}

CString ALot::GetTimeStart()
{
	CString strTimeStart;
	strTimeStart.Format( "%04d/%02d/%02d %02d:%02d:%02d.%03d", m_timeStart.wYear, m_timeStart.wMonth, m_timeStart.wDay,
		m_timeStart.wHour, m_timeStart.wMinute, m_timeStart.wSecond, m_timeStart.wMilliseconds );

	return strTimeStart;
}

void ALot::AddInputCnt( int nPrimeCum )
{
	m_nCntInput[nPrimeCum]++;

	g_lotMgr.AddInputCnt( nPrimeCum );
}

void ALot::AddPassCnt( int nPrimeCum )
{
	m_nCntPass[nPrimeCum]++;

	g_lotMgr.AddPassCnt( nPrimeCum );
}

void ALot::AddFailCnt( int nPrimeCum )
{
	m_nCntFail[nPrimeCum]++;

	g_lotMgr.AddFailCnt( nPrimeCum );
}

void ALot::AddBinCnt( int nBin )
{
	if( nBin < 1 || nBin > 9 )
		nBin = 1;
	m_nCntBin[nBin - 1]++;

	g_lotMgr.AddBinCnt( nBin );
}

void ALot::CalcCokType()
{
	CString strFirst = "";
	CString strLast = "";

	if( m_strPartID.GetLength() >= 18 )
	{
		strFirst = m_strPartID.Mid( 0, 3 );
		strLast = m_strPartID.Mid( 16, 2 );
	}

	/*
	if		( strFirst == "MZC" )		m_nCokType = st_handler.mn_cokIndex[COKINDEX_GS1];
	else if( strFirst == "MZD" )		m_nCokType = st_handler.mn_cokIndex[COKINDEX_GS2];
	else if( strFirst == "MZE" )		m_nCokType = st_handler.mn_cokIndex[COKINDEX_GS1];
	else if( strFirst == "MZM" )		m_nCokType = st_handler.mn_cokIndex[COKINDEX_MSATA];
	else if( strFirst == "MZ5" || strFirst == "MZ7" )
	{
		if( strLast == "A1" || strLast == "A2" || strLast == "A5" )
			m_nCokType = st_handler.mn_cokIndex[COKINDEX_25_REVERSE];
		else
			m_nCokType = st_handler.mn_cokIndex[COKINDEX_25_FRONT];
	}
	else
	{
		m_nCokType = -1;
	}
	*/
	
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =			
AMTLotManager::AMTLotManager()
{
	//ClearCountData();
	m_nCntInput[PRIME] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_INPUT_PRIME );
	m_nCntPass[PRIME] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_PASS_PRIME );
	m_nCntFail[PRIME] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_FAIL_PRIME );

	m_nCntInput[CUM] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_INPUT_CUM );
	m_nCntPass[CUM] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_PASS_CUM );
	m_nCntFail[CUM] = AMTRegistry::RegReadInt( REG_KEY_COUNT, REG_VAL_COUNT_FAIL_CUM );

	for( int i=0; i<BIN_MAX; i++ )
	{
		CString strVal;
		strVal.Format( "%s_%d", REG_VAL_COUNT_BIN, i + 1 );
		m_nCntBin[i] = AMTRegistry::RegReadInt( REG_KEY_COUNT, strVal );
	}
}

AMTLotManager::~AMTLotManager()
{

}

//2013,0912
void AMTLotManager::OnNewPLotin(CString strLotID, CString strPartID )
{
	int nLotCnt = m_vecPLot.size();
	for( int i=0; i<nLotCnt; i++ )
	{
		if( m_vecPLot.at(i).GetLotID() == strLotID )
		{
			return;
		}
	}
	AddPLot( strLotID, strPartID );
}

void AMTLotManager::AddPLot( CString strLotID, CString strPartID )
{
	ALot lot( strLotID, strPartID );
	lot.CalcCokType();

	m_vecPLot.push_back( lot );
}

int AMTLotManager::IsGetPLotID( CString strLotID)
{
	int nRet = -1;
	int nLotCnt = m_vecPLot.size();
	for (int i = 0; i < nLotCnt; i++)
	{
		if(m_vecPLot.at(i).GetLotID() == strLotID)
		{
			nRet = i;
			break;
		}
	}
	return nRet;
}

ALot& AMTLotManager::GetPLotAt( int iIdx )
{
	if( iIdx < 0 || iIdx >= m_vecPLot.size() ) return m_dummyPLot;

	return m_vecPLot.at( iIdx );
}

void AMTLotManager::DeletePLot(CString strLotID)
{
//	if (m_vecPLot.size() > 0)
//	{
//		std::vector<ALot>::iterator it = m_vecPLot.begin();
//		for( int i=0; i<m_vecPLot.size(); i++ )
//		{
//			it += i;
//			m_vecPLot.erase(it);
//		}
//	}
//	std::vector<ALot>::iterator it;
//	if( m_vecPLot.size() > 0 )
//	{
//		it = m_vecPLot.begin();
//		it.
//		while (it != m_vecPLot.end())
//		{
//			m_vecPLot.erase(it++);
//		}
//	}
//	for (int i = 0; i <= m_vecPLot.size(); ++i)
//	{
//		std::vector<ALot>::iterator it = m_vecPLot.begin();
//		m_vecPLot.erase(it);
//	}
	while (m_vecPLot.size() > 0)
	{
		std::vector<ALot>::iterator it = m_vecPLot.begin();
		m_vecPLot.erase(it);
	}

//	int i = 0;
//	if( m_vecPLot.size() > 0 )
//	{
//		do{
//			i = 0;
//			std::vector<ALot>::iterator it = m_vecPLot.begin();
//			if(m_vecPLot.at(i).GetLotID() == strLotID)
//			{
//				return;
//			}
//			else
//			{
//				m_vecPLot.erase(it);
//			}
//		}while(m_vecPLot.at(i).GetLotID() != strLotID);
//	}
}

void AMTLotManager::OnNewLotIn(CString strLotID, CString strPartID )
{

	int nLotCnt = m_vecLot.size();
	for( int i=0; i<nLotCnt; i++ )
	{
		if( m_vecLot.at(i).GetLotID() == strLotID )
		{
			return;
		}
	}

	AddLot( strLotID, strPartID );

	
	// 화면 갱신 (main)
	if( st_handler.cwnd_main )
		st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_LOTINFO, 0);
/*
	////2015.0604
	if(st_basic.mn_mode_ftp == 1)
	{
		if( st_handler.cwnd_main )
			st_handler.cwnd_main->PostMessage(WM_WORK_END, MAIN_FTPDOWN, 0);
	}
	////
	*/
}

CString AMTLotManager::GetLotIDAt(int iIdx )
{
	if( iIdx < 0 || iIdx >= m_vecLot.size() )	return "";
	if( m_vecLot.size() == 0 )					return "";

	return m_vecLot.at(iIdx).GetLotID();
}

ALot& AMTLotManager::GetLotAt( int iIdx )
{
	if( iIdx < 0 || iIdx >= m_vecLot.size() ) return m_dummyLot;

	return m_vecLot.at( iIdx );
}

ALot& AMTLotManager::GetLotByLotID( CString strLotID )
{
	int nCnt = m_vecLot.size();
	for( int i=0; i< nCnt; i++ )
	{
		if( m_vecLot.at( i ).GetLotID() == strLotID )
		{
			return m_vecLot.at( i );
		}
	}

	return m_dummyLot;
}

void AMTLotManager::AddLot( CString strLotID, CString strPartID )
{
	ALot lot( strLotID, strPartID );
	lot.CalcCokType();

	m_vecLot.push_back( lot );

//	if( st_handler.cwnd_data_lot )
//		st_handler.cwnd_data_lot->PostMessage( WM_DRAW_DATA_LOT );
}

void AMTLotManager::DeleteLot( int iIdx )
{
	if( iIdx < 0 || m_vecLot.size() <= iIdx )
		return;

	std::vector<ALot>::iterator it =m_vecLot.begin();
	it += iIdx;

	m_vecLot.erase(it);

//	if( st_handler.cwnd_data_lot )
//		st_handler.cwnd_data_lot->PostMessage( WM_DRAW_DATA_LOT );
}

void AMTLotManager::ClearCountData()
{
	m_nCntInput[PRIME] = 0;
	m_nCntPass[PRIME] = 0;
	m_nCntFail[PRIME] = 0;

	m_nCntInput[CUM] = 0;
	m_nCntPass[CUM] = 0;
	m_nCntFail[CUM] = 0;

	for( int i=0; i<BIN_MAX; i++ )
	{
		m_nCntBin[i] = 0;
	}

	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_INPUT_PRIME, 0 );
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_PASS_PRIME, 0 );
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_FAIL_PRIME, 0 );

	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_INPUT_CUM, 0 );
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_PASS_CUM, 0 );
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_FAIL_CUM, 0 );

	for( i=0; i<BIN_MAX; i++ )
	{
		CString strVal;
		strVal.Format( "%s_%d", REG_VAL_COUNT_BIN, i + 1 );
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, strVal, 0 );
	}
}

void AMTLotManager::AddInputCnt( int nPrimeCum )
{
	m_nCntInput[nPrimeCum]++;

	if( nPrimeCum == PRIME )
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_INPUT_PRIME, m_nCntInput[nPrimeCum] );
	else
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_INPUT_CUM, m_nCntInput[nPrimeCum] );
}

void AMTLotManager::AddPassCnt( int nPrimeCum )
{
	m_nCntPass[nPrimeCum]++;

	if( nPrimeCum == PRIME )
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_PASS_PRIME, m_nCntPass[nPrimeCum] );
	else
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_PASS_CUM, m_nCntPass[nPrimeCum] );
}

void AMTLotManager::AddFailCnt( int nPrimeCum )
{
	m_nCntFail[nPrimeCum]++;

	if( nPrimeCum == PRIME )
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_FAIL_PRIME, m_nCntFail[nPrimeCum] );
	else
		AMTRegistry::RegWriteInt( REG_KEY_COUNT, REG_VAL_COUNT_FAIL_CUM, m_nCntFail[nPrimeCum] );
}

void AMTLotManager::AddBinCnt( int nBin )
{
	if( nBin < 1 || nBin > 9 )
		nBin = 1;
	m_nCntBin[nBin - 1]++;

	CString strVal;
	strVal.Format( "%s_%d", REG_VAL_COUNT_BIN, nBin );
	AMTRegistry::RegWriteInt( REG_KEY_COUNT, (LPCSTR)strVal, m_nCntBin[nBin - 1] );
}

