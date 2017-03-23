// ServerItem.cpp : implementation file
//
// 이현철
// forcewin@popsmail.com

#include "stdafx.h"
#include "handler.h"
#include "ServerSocket.h"
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "SrcPart/APartNetworkManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerItem

CServerItem::CServerItem(CSocket* pServerSocket)
{
	m_ServerSocket = pServerSocket;
	m_nReceiveLen  = 0;

	m_byteReceive = new BYTE[((CServerSocket*)m_ServerSocket)->m_nReceiveDataLen];
}

CServerItem::~CServerItem()
{
	delete m_byteReceive;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerItem, CSocket)
	//{{AFX_MSG_MAP(CServerItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerItem member functions

// 소켓 닫기
void CServerItem::OnClose(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;

	// 소켓 해제
	g_Net.OnServerClosed( this );
	ServerSocket->CloseClient(this);
	
	CSocket::OnClose(nErrorCode);
}

// 수신 데이터 처리
void CServerItem::OnReceive(int nErrorCode) 
{
	CServerSocket* ServerSocket = (CServerSocket*)m_ServerSocket;

	// 데이터 읽기
	m_nReceiveLen = Receive(m_byteReceive, ServerSocket->m_nReceiveDataLen);

	//2016.0425
	CString strRecvLog;
	strRecvLog.Format( "C_RECV From ServerSocket : %d", m_nReceiveLen );
	sprintf(st_msg.c_msg, (LPCSTR)strRecvLog);
	Func.On_LogFile_Add(99, st_msg.c_msg);

	// 통지 : SCM_RECEIVE
	g_Net.OnServerReceived( this );

	CSocket::OnReceive(nErrorCode);
}
