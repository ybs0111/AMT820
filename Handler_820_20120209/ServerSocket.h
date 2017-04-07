#if !defined(AFX_SERVERSOCKET_H__2A49E7C7_67E3_11D3_8401_006097663D30__INCLUDED_)
#define AFX_SERVERSOCKET_H__2A49E7C7_67E3_11D3_8401_006097663D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSocket.h : header file
//
#include "ServerItem.h"


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//	ON_MESSAGE(SCM_ACCEPT,  OnClientAccept)		// Client 소켓 연결
//	ON_MESSAGE(SCM_CLOSE,   OnClientClose)		// Client 소켓 해제
//	ON_MESSAGE(SCM_RECEIVE, OnClientReceive)	// Client로 부터 수신되는 데이터
//	afx_msg LRESULT OnClientAccept(WPARAM wParam, LPARAM lClient);	// Client 소켓 연결
//	afx_msg LRESULT OnClientClose(WPARAM wParam, LPARAM lClient);	// Client 소켓 해제
//	afx_msg LRESULT OnClientReceive(WPARAM wParam, LPARAM lClient);	// Client로 부터 수신되는 데이터

/////////////////////////////////////////////////////////////////////////////
// CServerSocket command target

#define MAX_RECEIVE_LEN	131072
#include <afxmt.h>

class CServerSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CCriticalSection cs;
	CServerSocket();
	virtual ~CServerSocket();

	// 서버 소켓 생성
	BOOL Create(int n_address, int n_port);

// Overrides
public:
	// 접속된 Client 개수
	int GetClientCount();
	// Client Object
	CSocket* GetClient(int nIndex);

	// 모든 Client에 데이터 전송
	void SendClientAll(const void* lpBuf, int nBufLen, int nFlags=0);
	// 특정 Client에 데이터 전송
	int  SendClientAt(CSocket* ServerItem, const void* lpBuf, int nBufLen, int nFlags=0);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CServerSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	// Client 소켓 해제
	void CloseClient(CSocket* ServerItem);
	
	int			m_n_address;
	CPtrList	m_listClient;		// Client 소켓 list
	int			m_nReceiveDataLen;	// 최대 수신 데이터 길이


	friend CServerItem;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSOCKET_H__2A49E7C7_67E3_11D3_8401_006097663D30__INCLUDED_)
