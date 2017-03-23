// APartTray.cpp: implementation of the APartTray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "handler.h"
#include "APartTray.h"
#include "APartHandler.h"
#include "../AMTLotManager.h"
//#include "APartTestSite.h"
#include "APartNetworkManager.h"
#include "../IO_Manager.h"

#include <map>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
ATray::ATray()
{
	m_trayPos = TPOS_NONE;
	m_trayTargetPos = TPOS_NONE;

	m_nBufferNo = -1;
	m_bLastMdl = false;
	m_strLotID = "";
	m_bEmptyTray = true;
	m_bBypass = false;
}

ATray::~ATray()
{
	
}

void ATray::SetTrayInfo( CString strLotID, int nBufferNo, bool bLastMdl, bool bEmptyTray, bool bBypass )
{
	m_strLotID = strLotID;
	m_nBufferNo = nBufferNo;
	m_bLastMdl = bLastMdl;
	m_bEmptyTray = bEmptyTray;

	m_bBypass = bBypass;
}

void ATray::Clear()
{
	m_trayPos = TPOS_NONE;
	m_trayTargetPos = TPOS_NONE;

	m_nBufferNo = -1;
	m_bLastMdl = false;
	m_strLotID = "";
	m_bEmptyTray = true;
	m_bBypass = false;

	for( int i=0; i< MAX_MODULE; i++ )
	{
		m_mdl[i].EraseData();
	}
}

bool ATray::IsAllGood( bool bLastModule /*= false */ )
{
	int nTotalCnt = st_basic.n_tray_x * st_basic.n_tray_y;
	for( int i=0; i< nTotalCnt/*MAX_MODULE*/; i++ )
	{
		if( !bLastModule && m_mdl[i].IsExist() == false )
			return false;

		if( m_mdl[i].IsExist() && m_mdl[i].GetModuleState() != EMS_GOOD )
			return false;
	}	

	return true;
}

bool ATray::IsAllEmpty( bool bLastModule /*= false */ )
{
	int nTotalCnt = st_basic.n_tray_x * st_basic.n_tray_y;
	for( int i=0; i< nTotalCnt/*MAX_MODULE*/; i++ )
	{
		if( !bLastModule && m_mdl[i].IsExist() == false )
			return false;

		if( m_mdl[i].IsExist() && m_mdl[i].GetModuleState() != EMS_NONE )
			return false;
	}	

	return true;
}

int ATray::GetModuleCnt()
{
	int nTotalCnt = st_basic.n_tray_x * st_basic.n_tray_y;

	int nRtn = 0;
	for( int i=0; i<nTotalCnt; i++ )
	{
		if( GetModule(i).IsExist() )
			nRtn++;
	}

	return nRtn;
}

bool ATray::IsAllState( EMODULE_STATE ems )
{
	int nTotalCnt = st_basic.n_tray_x * st_basic.n_tray_y;
	for( int i=0; i< nTotalCnt/*MAX_MODULE*/; i++ )
	{
		if( m_mdl[i].IsExist() == false )
			continue;

		if( m_mdl[i].GetModuleState() != ems )
			return false;
	}	

	return true;
}

void ATray::SetPos( ETRAY_POS pos )
{
	 m_trayPos = pos;

//	 if( g_handler.GetMachinePos() == EMPOS_REAR )
//		 g_client_front.SendTrayInfo();
}

void ATray::SetTargetPos( ETRAY_POS pos )
{
	if( m_trayPos != m_trayTargetPos &&
		(m_trayTargetPos == TPOS_REJECT ||
		m_trayTargetPos == TPOS_NEXT ) )
		return;
	
	m_trayTargetPos = pos;

//	if( g_handler.GetMachinePos() == EMPOS_REAR )
//		 g_client_front.SendTrayInfo();
}


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
APartTray g_Tray;
APartTray::APartTray()
{
	m_vecRej.clear();
	m_nRejSelected = -1;
	m_nSelectedTray = -1;

	m_bSendNext_fromRej_Force = false;
}

APartTray::~APartTray()
{

}

void APartTray::AddTray( ATray tray )
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == TPOS_NONE )
		{
			m_tray[i] = tray;
			break;
		}
	}

//	if( g_handler.GetMachinePos() == EMPOS_REAR )
//		 g_client_front.SendTrayInfo();
}

void APartTray::TrayMinus()
{
	int i,j;
	i=j=0;
	for( j=MAX_TRAY-1; j>=0; j-- )
	{
		if( m_tray[j].GetPos() == TPOS_REJECT && m_tray[j].GetTargetPos() == TPOS_REJECT)
		{
			std::vector<int>::iterator it = g_Tray.m_vecRej.begin();
			for( i=0; i<m_vecRej.size(); i++ )
			{
				if( m_vecRej.at(i) == j )
				{
					it += i;
					break;
				}
			}
			m_vecRej.erase(it);
			m_tray[j].Clear();
			break;
		}
	}
	
	g_Tray.m_nRejSelected = g_Tray.m_vecRej.size() - 1;
	
	if( st_handler.cwnd_main )
	{
//		st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_REJ_FLOOR );
	}
}

////2015.0325
int APartTray::TrayPosCheck( ETRAY_POS pos )
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == pos )
		{
			return CTL_YES;
		}
	}
	return CTL_NO;
}
////

void APartTray::TrayMoved( ETRAY_POS posFrom, ETRAY_POS posTo )
{
	/*
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == posFrom )
		{
			if( posFrom == TPOS_REJECT && (m_vecRej.size() == 0 || m_vecRej.at( m_vecRej.size() - 1) != i) )
				continue;

			m_tray[i].SetPos( posTo );
			CheckAfterTrayMoved( i, posFrom, posTo );
			g_handler.CalcPriorityBin( posFrom, posTo );


			if( st_handler.cwnd_list )
			{
				sprintf(st_msg.c_normal_msg, "TRAY[%d] MOVE FROM: [%s] -----> TO:[%s]", i, (LPCSTR)GetStrPos(posFrom), (LPCSTR)GetStrPos(posTo) );
				//st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			int nCnt = st_basic.n_tray_x * st_basic.n_tray_y;
			CString strTrayDataIn;
			for( int j=1; j<=nCnt; j++ )
			{
				CString strTemp;
				AModule mdl = m_tray[i].GetModule(j - 1);
// 				strTemp.Format( " POCKET_%02d=(ARRAYSN=%s SERIAL=%s PPID=%s WWN=%s BIN=%s)", j, mdl.GetSN(), mdl.GetSerial(), mdl.GetPPID(),
// 					mdl.GetWWN(), mdl.GetBin() );
				strTemp.Format( " POCKET_%02d=(ARRAYSN=%s SERIAL=%s PPID=%s WWN=%s C_SERIAL=%s PSID=%s BIN=%s)", j, mdl.GetSN(), mdl.GetSerial(), mdl.GetPPID(),
					mdl.GetWWN(), mdl.GetCSN(), mdl.GetPSID(), mdl.GetBin() );
				strTrayDataIn += strTemp;
			}
 			Func.On_Log_Title_Msg(LOG_TOTAL, st_msg.c_normal_msg, strTrayDataIn);

			if( st_handler.cwnd_main )
			{
				st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_INIT, i );
			}

			if( g_handler.GetMachinePos() == EMPOS_REAR &&
				posFrom == TPOS_FRONT_TO_CONV1 && posTo == TPOS_CONV1 )
			{
				g_handler.SetReadySendNext( false );
			}
			else if( g_handler.GetMachinePos() == EMPOS_FRONT &&
				posTo == TPOS_CONV3 && m_tray[i].GetTargetPos() == TPOS_NEXT )
			{//2013,1222
// 				if( m_tray[i].GetEmptyTray() || m_tray[i].IsAllState( EMS_GOOD ) || m_tray[i].IsAllState( EMS_FAIL ) )
				if( m_tray[i].IsAllEmpty(true) ||  m_tray[i].GetEmptyTray() || m_tray[i].IsAllState( EMS_GOOD ) || m_tray[i].IsAllState( EMS_FAIL ) )
				{
					//ybs g_client_next.SetBufferReady( true );
					//2013,0807
// 					g_client_next.SetBufferReady( true, m_tray[i].GetEmptyTray(), m_tray[i].GetStrLotID());
					if(m_tray[i].GetEmptyTray())
					{
						if(st_basic.mn_empty_tray_out > 0)
						{
							if(g_lotMgr.GetLotAt(0).GetStrLastModule() == "YES")
							{
								if(g_ioMgr.get_in_bit( st_io.i_rear_shuttle_request_signal_chk ) == IO_ON)
								{
									g_client_next.SetBufferReady( true, true, m_tray[i].GetStrLotID());
								}
// 								else if(m_tray[i].IsAllState( EMS_GOOD ) || m_tray[i].IsAllState( EMS_FAIL ))
								else if(m_tray[i].GetModuleCnt() > 0)
								{
									if( st_handler.cwnd_list )
									{
										sprintf(st_msg.c_normal_msg, "emptyin_cnt:[%d],emptynext_cnt:[%d]", st_handler.mn_emptyin_cnt,st_handler.mn_emptynext_cnt);
										st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
									}
									g_client_next.SetBufferReady( true, true, m_tray[i].GetStrLotID());
								}
							}
							else
							{
								g_client_next.SetBufferReady( true, true, m_tray[i].GetStrLotID());
							}
						}
						else
						{
							g_client_next.SetBufferReady( true, true, m_tray[i].GetStrLotID());
						}
					}
					else
					{
						g_client_next.SetBufferReady( true, true, m_tray[i].GetStrLotID());
					}
				}
				else
				{//2013,1222
					if( m_tray[i].IsAllEmpty(true) )
					{
						//if( st_handler.cwnd_list )
						//{
						//	sprintf(st_msg.c_normal_msg, "[Buffer_Ready] AllEmpty [%d]", i);
						//	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
						//}
						
						g_client_next.SetBufferReady( true, true, m_tray[i].GetStrLotID());//체크 필요 중국 버저노가 차이 있음
					}
				}
			}
			else if( posTo == TPOS_NEXT )
			{
				m_tray[i].Clear();
				g_handler.CheckLotEnd();

				if( g_handler.GetMachinePos() == EMPOS_FRONT )
				{
					g_client_next.SetBufferReady( false );
					//if (st_handler.cwnd_list != NULL)
					//{
					//	sprintf(st_msg.c_normal_msg, "[SMEMA]BufferReady = %d", false);
					//	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // 동작 실패 출력 요청
					//}

				}
			}
			break;
		}
	}
	g_handler.CalcTrayMoveNext_fromRejFull();
	g_handler.CalcTrayMoveNext_fromBin3();
	g_handler.CalcTrayMoveNext_fromReject();
	
	g_handler.CalcTrayMoveNext_fromBin();*/
}

CString APartTray::GetStrPos( ETRAY_POS epos )
{
	if(st_handler.mn_mirror_type == CTL_YES)
	{
		switch( epos )
		{
		case TPOS_NONE:					return "";
		case TPOS_FRONT:				return "FRONT";
		case TPOS_CONV1:				return "CONV1";
		case TPOS_CONV2:				return "CONV2";
		case TPOS_CONV3:				return "CONV3";
		case TPOS_NEXT:					return "NEXT";
		case TPOS_FRONT_TO_CONV1:		return "FRONT_TO_CONV1";
		case TPOS_CONV1_TO_CONV2:		return "CONV1_TO_CONV2";
		case TPOS_CONV2_TO_CONV3:		return "CONV2_TO_CONV3";
		case TPOS_CONV3_TO_NEXT:		return "CONV3_TO_NEXT";
		case TPOS_CONV_TO_BIN1:			return "CONV_TO_WORK1";
		case TPOS_CONV_TO_BIN2:			return "CONV_TO_WORK2";
		case TPOS_CONV_TO_BIN3:			return "CONV_TO_WORK3";
		case TPOS_BIN_TO_CONV1:			return "WORK_TO_CONV1";
		case TPOS_BIN_TO_CONV2:			return "WORK_TO_CONV2";
		case TPOS_BIN_TO_CONV3:			return "WORK_TO_CONV3";
		case TPOS_BIN1:					return "WORK1";
		case TPOS_BIN2:					return "WORK2";
		case TPOS_BIN3:					return "WORK3";
		case TPOS_BIN_TO_REJECT:		return "WORK1_TO_REJECT";
		case TPOS_REJECT:				return "REJECT";
		}
	}
	else
	{
		switch( epos )
		{
		case TPOS_NONE:					return "";
		case TPOS_FRONT:				return "FRONT";
		case TPOS_CONV1:				return "CONV1";
		case TPOS_CONV2:				return "CONV2";
		case TPOS_CONV3:				return "CONV3";
		case TPOS_NEXT:					return "NEXT";
		case TPOS_FRONT_TO_CONV1:		return "FRONT_TO_CONV1";
		case TPOS_CONV1_TO_CONV2:		return "CONV1_TO_CONV2";
		case TPOS_CONV2_TO_CONV3:		return "CONV2_TO_CONV3";
		case TPOS_CONV3_TO_NEXT:		return "CONV3_TO_NEXT";
		case TPOS_CONV_TO_BIN1:			return "CONV_TO_WORK1";
		case TPOS_CONV_TO_BIN2:			return "CONV_TO_WORK2";
		case TPOS_CONV_TO_BIN3:			return "CONV_TO_WORK3";
		case TPOS_BIN_TO_CONV1:			return "WORK_TO_CONV1";
		case TPOS_BIN_TO_CONV2:			return "WORK_TO_CONV2";
		case TPOS_BIN_TO_CONV3:			return "WORK_TO_CONV3";
		case TPOS_BIN1:					return "WORK1";
		case TPOS_BIN2:					return "WORK2";
		case TPOS_BIN3:					return "WORK3";
		case TPOS_BIN_TO_REJECT:		return "WORK3_TO_REJECT";
		case TPOS_REJECT:				return "REJECT";
		}
	}

	return "";
}

int APartTray::GetRejCnt()
{
	int iCnt = 0;
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == TPOS_REJECT || m_tray[i].GetPos() == TPOS_REJECT_TO_BIN )
		{
			iCnt++;
		}
	}

	return iCnt;
}

ETRAY_POS APartTray::CalcTargetPos( ATray tray )
{
	
	return TPOS_NEXT;

}

bool APartTray::IsEnablePos( ETRAY_POS epos)
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == epos || 
			m_tray[i].GetTargetPos() == epos )
		{
			return false;
		}
	}

	return true;
}

void APartTray::CheckAfterTrayMoved( int iIdx, ETRAY_POS posFrom, ETRAY_POS posTo )
{
	// Reject
	bool bChanged = false;
	if( posTo == TPOS_REJECT )
	{
		m_vecRej.push_back( iIdx );
		bChanged = true;
	}
	else if( posFrom == TPOS_REJECT )
	{
		std::vector<int>::iterator it = m_vecRej.begin();
		for( int i=0; i<m_vecRej.size(); i++ )
		{
			if( m_vecRej.at(i) == iIdx )
			{
				it += i;
				break;
			}
		}

		m_vecRej.erase(it);
		bChanged = true;
	}

	if( bChanged )
	{
		m_nRejSelected = m_vecRej.size() - 1;

		if( st_handler.cwnd_main )
		{
//			st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_REJ_FLOOR );
		}
	}

	if( iIdx == g_Tray.GetSelectedTray() )
	{
		if( st_handler.cwnd_main )
		{
//			st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_SEL_TRAY );
		}	
	}

	if( posFrom == TPOS_BIN1 || posTo == TPOS_BIN1 )
	{
//		if( st_handler.cwnd_main )	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_BINTRAY_0 );
	}

	if( posFrom == TPOS_BIN2 || posTo == TPOS_BIN2 )
	{
//		if( st_handler.cwnd_main )	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_BINTRAY_1 );
	}

	//ybs
	if( posFrom == TPOS_BIN3 || posTo == TPOS_BIN3 )
	{
//		if( st_handler.cwnd_main )	st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDT_BINTRAY_2 );
	}
}

int APartTray::GetRejSelectedTray()
{
	if( m_nRejSelected < 0 || m_nRejSelected >= m_vecRej.size() )
		return -1;

	return m_vecRej[m_nRejSelected];
}

void APartTray::SetRejSelected( int iSel )
{
	if( iSel < 0 || iSel >= m_vecRej.size() )
		return;

	m_nRejSelected = iSel;
}

int APartTray::GetIdxByPos( ETRAY_POS epos )
{
	if( epos == TPOS_REJECT ) //ybs
	{
		int nRejCnt = m_vecRej.size();
		if( nRejCnt == 0 )
			return -1;

		return m_vecRej.at( nRejCnt - 1 );
	}
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == epos )
		{
			return i;
		}
	}

	return -1;
}

std::vector<int> APartTray::GetVecMustTestModule( int nLotIdx, int nExchange1, int nExchange2 )
{
	std::vector<int> vecRtn;
	vecRtn.clear();


	return vecRtn;
}

std::vector<int> APartTray::GetVecStacker3MustTestModule( int nLotIdx )
{
	std::vector<int> vecRtn;
	vecRtn.clear();



	return vecRtn;
}

std::vector<int> APartTray::GetVecEmptyCell( int nLotIdx, int nExchange1, int nExchange2 )
{
	std::vector<int> vecRtn;

	return vecRtn;
}

std::vector<int> APartTray::GetVecPutEmptyCell( int nLotIdx, int nExchange1, int nExchange2 )
{
	std::vector<int> vecRtn;



	return vecRtn;
}

std::vector<int> APartTray::GetVecStacker3EmptyCell( int nLotIdx, int nExchange1, int nExchange2 )
{
	std::vector<int> vecRtn;


	return vecRtn;
}

std::vector<int> APartTray::GetVecRejEmptyCell()
{

	std::vector<int> vecRtn;



	return vecRtn;
}

int APartTray::GetIdxByTargetPos( ETRAY_POS epos )
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetTargetPos() == epos )
		{
			return i;
		}
	}

	return -1;
}

std::vector<int> APartTray::GetVecFailinTray()
{
	std::vector<int> vecRtn;

	return vecRtn;
}

int APartTray::GetLoadModuleCnt()
{
	int nRtn = 0;
	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;

	for( int it=0; it<MAX_TRAY; it++ )
	{
		ATray tray = g_Tray.GetTray( it );
		for( int i=0; i<iMdlCnt; i++ )
		{
			if( tray.GetModule(i).IsExist() && tray.GetModule(i).IsTestLoadState() &&
				tray.GetStrLotID() == g_lotMgr.GetLotIDAt(0) )
				nRtn++;
		}
	}
	
	return nRtn;
}

bool APartTray::CalcLastTray( int iIdx, CString strLotID )
{
// 	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;


	return true;
}

int APartTray::GetTrayCnt_byLotID( CString strLotID )
{
	int nRtn = 0;
	/*for( int it=0; it<MAX_TRAY; it++ )
	{
		ATray tray = g_Tray.GetTray( it );
		if( tray.GetPos() == TPOS_NONE )		continue;
		
		if( tray.GetStrLotID() != strLotID )	continue;

		nRtn++;
	}*/

	return nRtn;
}

ATray& APartTray::GetRejTopTray()
{
	if( m_vecRej.size() == 0 )
		return m_tray[MAX_TRAY - 1];
	
	int nCnt = m_vecRej.size();
	return GetTray( m_vecRej[nCnt - 1] );
}

bool APartTray::IsWorkingRejToNext()
{
/*	for( int it=0; it<MAX_TRAY; it++ )
	{
		ATray tray = g_Tray.GetTray( it );
		if( ( tray.GetPos() == TPOS_REJECT ||
			tray.GetPos() == TPOS_REJECT_TO_BIN ||
			tray.GetPos() == TPOS_BIN3 ||
			tray.GetPos() == TPOS_BIN_TO_CONV3 ||
			tray.GetPos() == TPOS_CONV3 ) &&
			tray.GetTargetPos() == TPOS_NEXT )
		{
			return true;
		}
	}*/

	return false;
}

ETRAY_POS APartTray::GetPosByStr( CString strPos )
{
	if(st_handler.mn_mirror_type == CTL_YES)
	{
		if( strPos == "FRONT" )						return TPOS_FRONT;
		else if( strPos == "CONV1" )				return TPOS_CONV1;
		else if( strPos == "CONV2" )				return TPOS_CONV2;
		else if( strPos == "CONV3" )				return TPOS_CONV3;
		else if( strPos == "NEXT" )					return TPOS_NEXT;
		else if( strPos == "FRONT_TO_CONV1" )		return TPOS_FRONT_TO_CONV1;
		else if( strPos == "CONV1_TO_CONV2" )		return TPOS_CONV1_TO_CONV2;
		else if( strPos == "CONV2_TO_CONV3" )		return TPOS_CONV2_TO_CONV3;
		else if( strPos == "CONV3_TO_NEXT" )		return TPOS_CONV3_TO_NEXT;
		else if( strPos == "CONV_TO_WORK1" )			return TPOS_CONV_TO_BIN1;
		else if( strPos == "CONV_TO_WORK2" )			return TPOS_CONV_TO_BIN2;
		else if( strPos == "CONV_TO_WORK3" )			return TPOS_CONV_TO_BIN3;
		else if( strPos == "WORK_TO_CONV1" )			return TPOS_BIN_TO_CONV1;
		else if( strPos == "WORK_TO_CONV2" )			return TPOS_BIN_TO_CONV2;
		else if( strPos == "WORK_TO_CONV3" )			return TPOS_BIN_TO_CONV3;
		else if( strPos == "WORK1" )					return TPOS_BIN1;
		else if( strPos == "WORK2" )					return TPOS_BIN2;
		else if( strPos == "WORK3" )					return TPOS_BIN3;
		else if( strPos == "WORK1_TO_REJECT" )		return TPOS_BIN_TO_REJECT;
		else if( strPos == "REJECT" )				return TPOS_REJECT;
	}
	else
	{
		if( strPos == "FRONT" )						return TPOS_FRONT;
		else if( strPos == "CONV1" )				return TPOS_CONV1;
		else if( strPos == "CONV2" )				return TPOS_CONV2;
		else if( strPos == "CONV3" )				return TPOS_CONV3;
		else if( strPos == "NEXT" )					return TPOS_NEXT;
		else if( strPos == "FRONT_TO_CONV1" )		return TPOS_FRONT_TO_CONV1;
		else if( strPos == "CONV1_TO_CONV2" )		return TPOS_CONV1_TO_CONV2;
		else if( strPos == "CONV2_TO_CONV3" )		return TPOS_CONV2_TO_CONV3;
		else if( strPos == "CONV3_TO_NEXT" )		return TPOS_CONV3_TO_NEXT;
		else if( strPos == "CONV_TO_WORK1" )			return TPOS_CONV_TO_BIN1;
		else if( strPos == "CONV_TO_WORK2" )			return TPOS_CONV_TO_BIN2;
		else if( strPos == "CONV_TO_WORK3" )			return TPOS_CONV_TO_BIN3;
		else if( strPos == "WORK_TO_CONV1" )			return TPOS_BIN_TO_CONV1;
		else if( strPos == "WORK_TO_CONV2" )			return TPOS_BIN_TO_CONV2;
		else if( strPos == "WORK_TO_CONV3" )			return TPOS_BIN_TO_CONV3;
		else if( strPos == "WORK1" )					return TPOS_BIN1;
		else if( strPos == "WORK2" )					return TPOS_BIN2;
		else if( strPos == "WORK3" )					return TPOS_BIN3;
		else if( strPos == "WORK3_TO_REJECT" )		return TPOS_BIN_TO_REJECT;
		else if( strPos == "REJECT" )				return TPOS_REJECT;
	}

	return TPOS_NONE;
}

int APartTray::GetIdxByPos_Extra( ETRAY_POS epos )
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray_extra[i].GetPos() == epos )
		{
			return i;
		}
	}

	return -1;
}

int APartTray::GetIdxByTargetPos_Extra( ETRAY_POS epos )
{
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray_extra[i].GetTargetPos() == epos )
		{
			return i;
		}
	}

	return -1;
}

int APartTray::GetStopRejCnt()
{
	int iCnt = 0;
	for( int i=0; i<MAX_TRAY; i++ )
	{
		if( m_tray[i].GetPos() == TPOS_REJECT )
		{
			iCnt++;
		}
	}

	return iCnt;
}

std::vector<int> APartTray::GetVecGoodCell_3Stacker()
{
	std::vector<int> vecRtn;
	/*
	if( g_handler.GetPriorityBin() == TPOS_NONE )
		return vecRtn;
	
	int iBin1 = g_Tray.GetIdxByPos( TPOS_BIN1 );
	int iBin2 = g_Tray.GetIdxByPos( TPOS_BIN2 );
	int iBin3 = g_Tray.GetIdxByPos( TPOS_BIN3 );
	int ix=0, iy=0;
	ETRAY_POS ePosSub;
	if(st_handler.mn_mirror_type == CTL_YES)
	{
		ePosSub = TPOS_BIN1;
	}
	else
	{
		ePosSub = TPOS_BIN3;	
	}
	
	int iIndexBinSub = MPOS_INDEX_BIN3;	
	if( ePosSub == TPOS_BIN2 )	iIndexBinSub = MPOS_INDEX_BIN2;
	if( ePosSub == TPOS_BIN1 )	iIndexBinSub = MPOS_INDEX_BIN1;
	
	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;
	int iIdx = g_Tray.GetIdxByPos( ePosSub );
	if( iIdx < 0 )
		return vecRtn;
	
	ATray traySub = g_Tray.GetTray( iIdx );
	
	if( traySub.GetTargetPos() == ePosSub &&
		traySub.GetStrLotID() == g_lotMgr.GetLotIDAt(0) )
	{
		for( int i=0; i<iMdlCnt; i++ )
		{
			if( traySub.GetModule(i).IsExist() && traySub.GetModule(i).GetModuleState() == EMS_GOOD )
				vecRtn.push_back( iIndexBinSub + i );
		}
	}
	*/
	return vecRtn;
}


std::vector<int> APartTray::GetVecGoodCell_Sub()
{

	std::vector<int> vecRtn;
/*
	if( g_handler.GetPriorityBin() == TPOS_NONE )
		return vecRtn;

	int iBin1 = g_Tray.GetIdxByPos( TPOS_BIN1 );
	int iBin2 = g_Tray.GetIdxByPos( TPOS_BIN2 );
	int iBin3 = g_Tray.GetIdxByPos( TPOS_BIN3 );
	int ix=0, iy=0;
	ETRAY_POS ePosPri = g_handler.GetPriorityBin();
	ETRAY_POS ePosSub = TPOS_NONE;

	if(st_handler.mn_mirror_type == CTL_YES)
	{
		if(ePosPri == TPOS_BIN1)
		{
			if(iBin2 > -1)	ePosSub = TPOS_BIN2;
			else if(iBin3 > -1)	ePosSub = TPOS_BIN3;
			else			ePosSub = TPOS_NONE;
		}
		else
		{
			if(ePosPri == TPOS_BIN2) 
			{
				if(iBin3 > -1)	    ePosSub = TPOS_BIN3;
				else if(iBin1 > -1 && st_handler.mb_rej_3stacker == false) ePosSub = TPOS_BIN1;
				else                ePosSub = TPOS_NONE;
			}
			else
			{
				if(ePosPri == TPOS_BIN3)
				{
					if(iBin1 > -1 && st_handler.mb_rej_3stacker == false) ePosSub = TPOS_BIN1;
					else
					{
						if(iBin2 > -1) ePosSub = TPOS_BIN2;
						else           ePosSub = TPOS_NONE;
					}
				}
			}
		}
	}
	else
	{
		if(ePosPri == TPOS_BIN1)
		{
			if(iBin2 > -1)	ePosSub = TPOS_BIN2;
			else if(iBin3 > -1 && st_handler.mb_rej_3stacker == false)	ePosSub = TPOS_BIN3;
			else			ePosSub = TPOS_NONE;
		}
		else
		{
			if(ePosPri == TPOS_BIN2) 
			{
				if(iBin3 > -1 && st_handler.mb_rej_3stacker == false)	    ePosSub = TPOS_BIN3;
				else if(iBin1 > -1) ePosSub = TPOS_BIN1;
				else                ePosSub = TPOS_NONE;
			}
			else
			{
				if(ePosPri == TPOS_BIN3 && st_handler.mb_rej_3stacker == false)
				{
					if(iBin1 > -1) ePosSub = TPOS_BIN1;
					else
					{
						if(iBin2 > -1) ePosSub = TPOS_BIN2;
						else           ePosSub = TPOS_NONE;
					}
				}
			}
		}
	}
	int iIndexBinPri = MPOS_INDEX_BIN3;
	int iIndexBinSub = MPOS_INDEX_BIN3;

	if( ePosPri == TPOS_BIN2 )	iIndexBinPri = MPOS_INDEX_BIN2;
	if( ePosSub == TPOS_BIN2 )	iIndexBinSub = MPOS_INDEX_BIN2;
	if( ePosPri == TPOS_BIN1 )	iIndexBinPri = MPOS_INDEX_BIN1;
	if( ePosSub == TPOS_BIN1 )	iIndexBinSub = MPOS_INDEX_BIN1;

	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;
	int iIdx = g_Tray.GetIdxByPos( ePosSub );
	if( iIdx < 0 )
		return vecRtn;

	ATray traySub = g_Tray.GetTray( iIdx );

	if( traySub.GetTargetPos() == ePosSub &&
		traySub.GetStrLotID() == g_lotMgr.GetLotIDAt(0) )
	{
		for( int i=0; i<iMdlCnt; i++ )
		{
			if( traySub.GetModule(i).IsExist() && traySub.GetModule(i).GetModuleState() == EMS_GOOD )
				vecRtn.push_back( iIndexBinSub + i );
		}
	}
*/
	return vecRtn;
}

std::vector<int> APartTray::GetVecEmptyCell_Pri()
{
	std::vector<int> vecRtn;
/*
	if( g_handler.GetPriorityBin() == TPOS_NONE )
		return vecRtn;

	ETRAY_POS ePosPri = g_handler.GetPriorityBin();

	int iIndexBinPri = MPOS_INDEX_BIN3;

	if( ePosPri == TPOS_BIN2 )	iIndexBinPri = MPOS_INDEX_BIN2;
	if( ePosPri == TPOS_BIN1 )	iIndexBinPri = MPOS_INDEX_BIN1;

	int iMdlCnt = st_basic.n_tray_x * st_basic.n_tray_y;
	int iIdx = g_Tray.GetIdxByPos( ePosPri );
	if( iIdx < 0 )
		return vecRtn;

	ATray trayPri = g_Tray.GetTray( iIdx );

	if( trayPri.GetTargetPos() == ePosPri &&
		trayPri.GetStrLotID() == g_lotMgr.GetLotIDAt(0) )
	{
		for( int i=0; i<iMdlCnt; i++ )
		{
			if( trayPri.GetModule(i).IsExist() == false  )
				vecRtn.push_back( iIndexBinPri + i );
		}
	}*/

	return vecRtn;
}

