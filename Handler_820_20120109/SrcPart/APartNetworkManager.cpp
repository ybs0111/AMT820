// APartNetworkManager.cpp: implementation of the APartNetworkManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "handler.h"
#include "APartNetworkManager.h"
//#include "../Run_Network.h"
//#include "../CXGemManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
APartNetworkManager g_Net;

APartNetworkManager::APartNetworkManager()
{
	int i=0;

	for( i = 0; i < 8; i++ )
	{
//		g_client_bp[i].m_iBPPos = i;
	}
}

APartNetworkManager::~APartNetworkManager()
{

}

void APartNetworkManager::OnServerReceived( CServerItem* psItem )
{
	if( psItem == NULL )	return;
	if( psItem->m_ServerSocket == NULL )	return;

//	if( (CSocket*)g_server_bpc.GetPointer_Server() == psItem->m_ServerSocket )
//		g_server_bpc.OnServerReceived( psItem );
}

void APartNetworkManager::OnServerClosed( CServerItem* psItem )
{
	if( psItem == NULL )	return;
}

void APartNetworkManager::OnServerAccept( CServerSocket* pServer )
{
	// ·Î±×
}

void APartNetworkManager::OnClientReceived( CClientSocket* pClient )
{
	if( pClient == NULL )	return;
/*
	if( g_client_bp[0].GetPointer_Client() == pClient )
		g_client_bp[0].OnClientReceived();
	
	if( g_client_bp[1].GetPointer_Client() == pClient )
		g_client_bp[1].OnClientReceived();

	if( g_client_bp[2].GetPointer_Client() == pClient )
		g_client_bp[2].OnClientReceived();

	if( g_client_bp[3].GetPointer_Client() == pClient )
		g_client_bp[3].OnClientReceived();

	if( g_client_bp[4].GetPointer_Client() == pClient )
		g_client_bp[4].OnClientReceived();

	if( g_client_bp[5].GetPointer_Client() == pClient )
		g_client_bp[5].OnClientReceived();

	if( g_client_bp[6].GetPointer_Client() == pClient )
		g_client_bp[6].OnClientReceived();

	if( g_client_bp[7].GetPointer_Client() == pClient )
		g_client_bp[7].OnClientReceived();

	if( g_client_tc.GetPointer_Client() == pClient )
		g_client_tc.OnClientReceived();

	if( g_client_cim.GetPointer_Client() == pClient )
		g_client_cim.OnClientReceived();

	if( g_client_his.GetPointer_Client() == pClient )
		g_client_his.OnClientReceived();

	if( g_client_bpc.GetPointer_Client() == pClient )
		g_client_bpc.OnClientReceived();
	*/
}

void APartNetworkManager::OnClientClosed(CClientSocket* pClient )
{
	// close...
	if( pClient == NULL )	return;
/*
	if( g_client_bp[0].GetPointer_Client() == pClient )
		g_client_bp[0].OnClientClose();
	
	if( g_client_bp[1].GetPointer_Client() == pClient )
		g_client_bp[1].OnClientClose();

	if( g_client_bp[2].GetPointer_Client() == pClient )
		g_client_bp[2].OnClientClose();
	
	if( g_client_bp[3].GetPointer_Client() == pClient )
		g_client_bp[3].OnClientClose();

	if( g_client_bp[4].GetPointer_Client() == pClient )
		g_client_bp[4].OnClientClose();
	
	if( g_client_bp[5].GetPointer_Client() == pClient )
		g_client_bp[5].OnClientClose();

	if( g_client_bp[6].GetPointer_Client() == pClient )
		g_client_bp[6].OnClientClose();
	
	if( g_client_bp[7].GetPointer_Client() == pClient )
		g_client_bp[7].OnClientClose();

	if( g_client_tc.GetPointer_Client() == pClient )
		g_client_tc.OnClientClose();

	if( g_client_cim.GetPointer_Client() == pClient )
		g_client_cim.OnClientClose();

	if( g_client_his.GetPointer_Client() == pClient )
		g_client_his.OnClientClose();

	if( g_client_bpc.GetPointer_Client() == pClient )
		g_client_bpc.OnClientClose();
	*/
}

void APartNetworkManager::Run_Move()
{
	/*
	g_client_tc.Run_Move();
	g_client_cim.Run_Move();

	g_client_bp[0].Run_Move();
	g_client_bp[1].Run_Move();
	g_client_bp[2].Run_Move();
	g_client_bp[3].Run_Move();
	g_client_bp[4].Run_Move();
	g_client_bp[5].Run_Move();
	g_client_bp[6].Run_Move();
	g_client_bp[7].Run_Move();

	g_client_his.Run_Move();
	g_client_bpc.Run_Move();


	g_server_bpc.Run_Move();*/

	//g_XGemMgr.Update();
}

void APartNetworkManager::OnStartServer()
{
//	g_server_bpc.OnServerOpen( NID_S_BPC );
}

void APartNetworkManager::OnInitClientAddress()
{
	/*
	g_client_tc.SetAddress( NID_TC );
	g_client_cim.SetAddress( NID_CIM );

	g_client_bp[0].SetAddress( NID_ST1BP1 );
	g_client_bp[1].SetAddress( NID_ST1BP2 );
	g_client_bp[2].SetAddress( NID_ST2BP1 );
	g_client_bp[3].SetAddress( NID_ST2BP2 );
	g_client_bp[4].SetAddress( NID_ST3BP1 );
	g_client_bp[5].SetAddress( NID_ST3BP2 );
	g_client_bp[6].SetAddress( NID_ST4BP1 );
	g_client_bp[7].SetAddress( NID_ST4BP2 );

	g_client_his.SetAddress( NID_HIS );
	g_client_bpc.SetAddress( NID_BPC );*/
}

void APartNetworkManager::ClientAccept( int iAddr )
{/*
	switch( iAddr )
	{
	case NID_TC:		g_client_tc.OnClientAccept();			break;
	case NID_CIM:		g_client_cim.OnClientAccept();			break;
	case NID_ST1BP1:	g_client_bp[0].OnClientAccept();		break;
	case NID_ST1BP2:	g_client_bp[1].OnClientAccept();		break;
	case NID_ST2BP1:	g_client_bp[2].OnClientAccept();		break;
	case NID_ST2BP2:	g_client_bp[3].OnClientAccept();		break;
	case NID_ST3BP1:	g_client_bp[4].OnClientAccept();		break;
	case NID_ST3BP2:	g_client_bp[5].OnClientAccept();		break;
	case NID_ST4BP1:	g_client_bp[6].OnClientAccept();		break;
	case NID_ST4BP2:	g_client_bp[7].OnClientAccept();		break;
	case NID_HIS:		g_client_his.OnClientAccept();			break;
	case NID_BPC:		g_client_bpc.OnClientAccept();			break;
	
	}*/
}	

CString APartNetworkManager::GetAddressName_C( int iAddr )
{
	/*switch( iAddr )
	{
	case NID_TC:		return "TC SERVER";
	case NID_CIM:		return "CIM";
	case NID_BPC:		return "BPC";
	case NID_ST1BP1:	return "ST1 BP1";
	case NID_ST1BP2:	return "ST1 BP2";
	case NID_ST2BP1:	return "ST2 BP1";
	case NID_ST2BP2:	return "ST2 BP2";
	case NID_ST3BP1:	return "ST3 BP1";
	case NID_ST3BP2:	return "ST3 BP2";
	case NID_ST4BP1:	return "ST4 BP1";
	case NID_ST4BP2:	return "ST4 BP2";
	case NID_HIS:		return "HIS";
	}*/
	
	return "UNKNOWN";
}

CString APartNetworkManager::GetAddressName_S( int iAddr )
{
//	switch( iAddr )
//	{
//	case NID_S_BPC:		return "BPC";
//	}
	
	return "UNKNOWN";
}
