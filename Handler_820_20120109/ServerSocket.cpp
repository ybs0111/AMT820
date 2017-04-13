// ServerSocket.cpp : implementation file
//
// 이현철
// forcewin@popsmail.com

#include "stdafx.h"
#include "ServerSocket.h"
#include "Variable.h"		// 전역 변수 정의 클래스 추가 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSocket

CServerSocket::CServerSocket()
{

}

CServerSocket::~CServerSocket()
{
	CServerItem *ServerItem;

	while(m_listClient.GetCount())
	{
		ServerItem = (CServerItem*)m_listClient.GetHead();
		ServerItem->Close();
		m_listClient.RemoveHead();
		delete ServerItem;
	}
}

// 서버 소켓 생성
BOOL CServerSocket::Create(int n_address, int n_port) 
{
	if(!CSocket::Create(n_port, SOCK_STREAM))
	{
		AfxMessageBox("Error : Create() in socket");
		return FALSE;
	}

	if(!CSocket::Listen())
	{
		AfxMessageBox("Error : Listen() in socket");
		return FALSE;
	}

	m_n_address = n_address;

	return TRUE;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServerSocket, CSocket)
	//{{AFX_MSG_MAP(CServerSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServerSocket member functions

// Client 소켓 연결
void CServerSocket::OnAccept(int nErrorCode) 
{
	CServerItem* ServerItem = new CServerItem(this);

	if (CSocket::Accept(*ServerItem))
	{
		m_listClient.AddTail(ServerItem);

//		m_wndParent->SendMessage(SCM_ACCEPT, WPARAM(m_nServer), LPARAM(ServerItem));
	}
	else
	{
		delete ServerItem;
//		AfxMessageBox("Error : Accept() in socket");
	}
	
	CSocket::OnAccept(nErrorCode);
}

// Client 소켓 닫기
void CServerSocket::CloseClient(CSocket* ServerItem)
{
	POSITION pos = m_listClient.Find(ServerItem);
	if (pos != NULL)
	{
		ServerItem->Close();
		m_listClient.RemoveAt(pos);
		delete ServerItem;
	}
}

// 모든 Client에 데이터 전송
void CServerSocket::SendClientAll(const void *lpBuf, int nBufLen, int nFlags)
{
	CServerItem *ServerItem;

	POSITION pos = m_listClient.GetHeadPosition();
	while (pos != NULL)
	{
		ServerItem = (CServerItem*)m_listClient.GetNext(pos);
		ServerItem->Send(lpBuf, nBufLen, nFlags);
	}
}

// 특정 Client에 데이터 전송
int CServerSocket::SendClientAt(CSocket* ServerItem, const void *lpBuf, int nBufLen, int nFlags)
{
	return ServerItem->Send(lpBuf, nBufLen, nFlags);
}

// 접속된 Client 개수
int CServerSocket::GetClientCount()
{
	return m_listClient.GetCount();
}

// Client Object
CSocket* CServerSocket::GetClient(int nIndex)
{
	return (CSocket*)m_listClient.GetAt(m_listClient.FindIndex(nIndex));
}

void CServerSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnReceive(nErrorCode);
}

void CServerSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnClose(nErrorCode);
}
