// APartTray.h: interface for the APartTray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARTTRAY_H__3B578633_0F74_49F9_B988_2D5674B676F9__INCLUDED_)
#define AFX_APARTTRAY_H__3B578633_0F74_49F9_B988_2D5674B676F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "PartFunction.h"

enum ETRAY_POS
{
	TPOS_NONE,
	TPOS_FRONT,
	TPOS_CONV1,
	TPOS_CONV2,
	TPOS_CONV3,
	TPOS_NEXT,

	TPOS_FRONT_TO_CONV1,
	TPOS_CONV1_TO_CONV2,
	TPOS_CONV2_TO_CONV3,
	TPOS_CONV3_TO_NEXT,
	
	TPOS_CONV_TO_BIN1,
	TPOS_CONV_TO_BIN2,
	TPOS_CONV_TO_BIN3,
	TPOS_BIN_TO_CONV1,
	TPOS_BIN_TO_CONV2,
	TPOS_BIN_TO_CONV3,

	TPOS_BIN1,
	TPOS_BIN2,
	TPOS_BIN3,
	
	TPOS_BIN_TO_REJECT,
	TPOS_REJECT_TO_BIN,
	
	TPOS_REJECT,
	
	MAX_TPOS,
};

#define MAX_MODULE		30//2014,0310


class ATrayExtra
{
public:
	ATrayExtra(){}
	virtual ~ATrayExtra(){}

	// Get
	ETRAY_POS	GetPos() { return m_trayPos; }
	ETRAY_POS	GetTargetPos() { return m_trayTargetPos; }

	// Set		
	void		SetPos( ETRAY_POS pos ) { m_trayPos = pos; }
	void		SetTargetPos( ETRAY_POS pos ) {	m_trayTargetPos = pos; }	
	
protected:
	ETRAY_POS	m_trayPos;
	ETRAY_POS	m_trayTargetPos;
};


class ATray
{
public:
	ATray();
	virtual ~ATray();

	// Get
	ETRAY_POS	GetPos() { return m_trayPos; }
	ETRAY_POS	GetTargetPos() { return m_trayTargetPos; }
	int			GetBufferNo() { return m_nBufferNo; }
	bool		GetLastMdl() { return m_bLastMdl; }
	CString		GetStrLotID() { return m_strLotID; }
	bool		GetEmptyTray() { return m_bEmptyTray; }
	bool		GetBypass() { return m_bBypass; }
	int			GetModuleCnt();
	//2013,0715
	CString		GetProcessID() { return m_strProcessid; }
	CString		GetPropertyEnd() { return m_strPropertyEnd; }

	// Set
	void		SetPos( ETRAY_POS pos );
	void		SetTargetPos( ETRAY_POS pos );
				
	void		SetBufferNo( int iVal ) { m_nBufferNo = iVal; }
	void		SetLastMdl( bool bVal ) { m_bLastMdl = bVal; }
	void		SetLotID( CString strVal ) { m_strLotID = strVal; }
	void		SetEmptyTray( bool bVal ) { m_bEmptyTray = bVal; }
	void		SetBypass( bool bVal ) { m_bBypass = bVal; }
	//2013,0715
	void		SetProcessID( CString strid ) { m_strProcessid = strid; }
	void		SetPropertyEnd( CString strPEid ) { m_strPropertyEnd = strPEid; }


	void		SetTrayInfo( CString strLotID, int nBufferNo, bool bLastMdl, bool bEmptyTray, bool bBypass = false );

	void		SetModule( int nIdx, AModule mdl ) { m_mdl[nIdx] = mdl; }

	bool		IsExistModule( int iIdx ) { return m_mdl[iIdx].IsExist(); }

	AModule&	GetModule( int iIdx )	{ return m_mdl[iIdx]; }

	void		Clear();

	bool		IsAllGood( bool bLastModule = false );
	bool		IsAllEmpty( bool bLastModule = false );

	bool		IsAllState( EMODULE_STATE ems );

protected:
	ETRAY_POS	m_trayPos;
	ETRAY_POS	m_trayTargetPos;

	int			m_nBufferNo;
	bool		m_bLastMdl;
	CString		m_strLotID;
	bool		m_bEmptyTray;
	bool		m_bBypass;
	//2013,0715
	CString		m_strProcessid;
	CString		m_strPropertyEnd;

	AModule		m_mdl[MAX_MODULE];
};




#define MAX_TRAY		12

class APartTray  
{
public:
	APartTray();
	virtual ~APartTray();

	ATray& GetTray( int iIdx ) { return m_tray[iIdx]; }
	ATrayExtra& GetTrayExtra( int iIdx ) { return m_tray_extra[iIdx]; }
	int GetRejCnt();
	int GetStopRejCnt();
	ATray& GetRejTopTray();

	bool CalcLastTray( int iIdx, CString strLotID );
	int GetTrayCnt_byLotID( CString strLotID );

	void SetTrayExtra( int iIdx, ATrayExtra trayExtra ) { m_tray_extra[iIdx] = trayExtra; }


protected:
	ATray		m_tray[MAX_TRAY];
	ATrayExtra	m_tray_extra[MAX_TRAY];


public:
	void AddTray( ATray tray );
	int TrayPosCheck( ETRAY_POS pos );//2015.0325

	void TrayMoved( ETRAY_POS posFrom, ETRAY_POS posTo );
	void CheckAfterTrayMoved( int iIdx, ETRAY_POS posFrom, ETRAY_POS posTo );

	CString GetStrPos( ETRAY_POS epos );
	ETRAY_POS GetPosByStr( CString strPos ); 

	ETRAY_POS CalcTargetPos( ATray tray );
	bool IsEnablePos( ETRAY_POS epos);


	int GetRejSelectedTray();
	void SetRejSelected( int iSel );
	int GetRejSelected() { return m_nRejSelected; }

	void SetSelectedTray(int iSel) { m_nSelectedTray = iSel; }
	int GetSelectedTray() { return m_nSelectedTray; }

	int GetIdxByPos( ETRAY_POS  epos );
	int GetIdxByTargetPos( ETRAY_POS epos );

	int GetIdxByPos_Extra( ETRAY_POS  epos );
	int GetIdxByTargetPos_Extra( ETRAY_POS epos );

	std::vector<int> GetVecMustTestModule( int nLotIdx, int nExchange1 = -1, int nExchange2 = -1 );
	std::vector<int> GetVecStacker3MustTestModule( int nLotIdx );//2012,1225
	std::vector<int> GetVecEmptyCell( int nLotIdx, int nExchange1 = -1, int nExchange2 = -1 );
	std::vector<int> GetVecPutEmptyCell( int nLotIdx, int nExchange1 = -1, int nExchange2 = -1 );
	std::vector<int> GetVecStacker3EmptyCell( int nLotIdx, int nExchange1 = -1, int nExchange2 = -1 );//2012,1225
	std::vector<int> GetVecRejEmptyCell();
	std::vector<int> GetVecFailinTray();

	//2013,0321
	std::vector<int> GetVecGoodCell_3Stacker();
	std::vector<int> GetVecGoodCell_Sub();
	std::vector<int> GetVecEmptyCell_Pri();

	int GetLoadModuleCnt();

	void SetSendNextFromRejForce( bool bVal ) { m_bSendNext_fromRej_Force = bVal; }
	bool GetSendNextFromRejForce() { return m_bSendNext_fromRej_Force; }

	bool		IsWorkingRejToNext();
	void TrayMinus();

protected:
	std::vector<int>	m_vecRej;
	int					m_nRejSelected;
	int					m_nSelectedTray;

	bool				m_bSendNext_fromRej_Force;
};

extern APartTray g_Tray;

#endif // !defined(AFX_APARTTRAY_H__3B578633_0F74_49F9_B988_2D5674B676F9__INCLUDED_)
