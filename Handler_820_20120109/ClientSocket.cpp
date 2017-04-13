// ClientSocket.cpp : implementation file
//
// 이현철
// forcewin@popsmail.com

#include "stdafx.h"
#include "ClientSocket.h"
#include "Variable.h"			// 전역 변수 정의 클래스 추가 
#include "SrcPart/APartNetworkManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
	m_byteReceive = NULL;
}

CClientSocket::~CClientSocket()
{
	//2016.0112
	if (m_byteReceive != NULL)
	{
		delete m_byteReceive;
		m_byteReceive = NULL;
	}
}

BOOL CClientSocket::Create(int address, LPSTR szAddress, UINT nPortNo, int nReceiveSize)
{
	if (!CSocket::Create())
	{
		return FALSE;
	}

	CSocket::m_nTimeOut = 1;

	if (!CSocket::Connect(szAddress, nPortNo))
	{
		CSocket::Close();
		return FALSE;
	}
	m_nReceiveLen  = 0;
 
	m_nReceiveSize = nReceiveSize;
	m_byteReceive = new BYTE[m_nReceiveSize];
	
	m_naddress = address;

	mn_port = nPortNo;			// 추가.

	return TRUE;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnReceive(int nErrorCode) 
{
	// 데이터 읽기
	m_nReceiveLen = Receive(m_byteReceive, m_nReceiveSize);

	//2016.0425
	CString strRecvLog;
	strRecvLog.Format( "C_RECV From ClientSocket : %d", m_nReceiveLen );
	sprintf(st_msg.c_msg, (LPCSTR)strRecvLog);
	Func.On_LogFile_Add(99, st_msg.c_msg);

	// 통지 : SCM_RECEIVE
	g_Net.OnClientReceived( this );

	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode) 
{
	//2015.1008
	if (m_byteReceive != NULL)
	{
		delete m_byteReceive;
		m_byteReceive = NULL;
	}
	g_Net.OnClientClosed( this );
	CSocket::OnClose(nErrorCode);
}

void CClientSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnAccept(nErrorCode);
}

void CClientSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSocket::OnConnect(nErrorCode);
}

BOOL CClientSocket::IsConnect()
{
	return 0;
}

BOOL CClientSocket::ConnectHelper(const SOCKADDR *lpSockAddr, int nSockAddrLen)
{
	CTime  curt, st;
	CTimeSpan span(0, 0, 0, m_nTimeOut);

	if (m_pbBlocking != NULL)
	{
		WSASetLastError(WSAEINPROGRESS);
		return  FALSE;
	}

	m_nConnectError = -1;

	if(!CAsyncSocket::ConnectHelper(lpSockAddr, nSockAddrLen))
	{
		if(GetLastError() == WSAEWOULDBLOCK)
		{
			st = CTime().GetCurrentTime();

			while(PumpMessages(FD_CONNECT))
			{
				if(m_nConnectError != -1)
				{
					WSASetLastError(m_nConnectError);
					return (m_nConnectError == 0);
				}

				curt = CTime().GetCurrentTime();
				
				if(curt > (st + span))
				{
					return FALSE;
				}
			}
		}
		return FALSE;
	}
	return TRUE;
}
