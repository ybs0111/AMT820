// ANetwork.h: interface for the ANetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANETWORK_H__16110310_634E_4426_ADF5_8A7ACCED6409__INCLUDED_)
#define AFX_ANETWORK_H__16110310_634E_4426_ADF5_8A7ACCED6409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum
{
	RECV_STEP_WAIT,
	RECV_STEP_PARSE,
};

#define RECEIVE_MAX		131072

#define SENDMSG_TYPE_NONE		0x00
#define SENDMSG_TYPE_RPY		0x01
#define SENDMSG_TYPE_HEAD_16	0x02

enum NET_RECEIVED
{
	NR_NONE,
	NR_WAITING,
	NR_RECEIVED,
};

enum ENUM_NET_OPT
{
	NET_OPT_NORMAL,
	NET_OPT_BRACKET,
	NET_OPT_2,
	NET_OPT_3,
	NET_OPT_4,
	NET_OPT_5,
	NET_OPT_6,
	NET_OPT_7,
	NET_OPT_QUOTES,
};
enum eSENDMSG_STEP
{
	SENDMSG_STEP_START,
	SENDMSG_STEP_ACCEPT,
	SENDMSG_STEP_WAIT,
	SENDMSG_STEP_SEND,
	SENDMSG_STEP_SEND2,
	SENDMSG_STEP_FINISH_CHECK,
	SENDMSG_STEP_RETRY,
	SENDMSG_STEP_FINISH,
};

enum eSENDED_RESULT
{
	SENDMSG_RES_FAIL = -1,
	SENDMSG_RES_WORKING = 0,
	SENDMSG_RES_SUCCESS = 1,
};

struct stSendMsg
{	
	int iID;
	std::string strMsg;
 	DWORD dwType;
 	int iStep;
 	int iErrMax;
 	int iErrCount;
 	bool bSuccess;
 	DWORD dwLastTime;
	bool bReceived;
};


class ANetwork
{
public:
	ANetwork();
	virtual ~ANetwork();

	virtual void Run_Move();

	void SetPort( int nPort )	{ m_nPort = nPort; }
	CString GetPortStr();
	int GetPort()				{ return m_nPort; }
	int* GetPointer_Port()		{ return &m_nPort; }

	void Run_Receive();
	CString Parse_Received();
	virtual void OnParseComplete( std::string strRecv ) = 0;

	int PushSendMsg( LPCTSTR szMsg, int iErrMax = 0, DWORD dwType = SENDMSG_TYPE_NONE );

	void SetAddress( int nAddr )	{m_iAddress = nAddr;}
	int GetAddress() { return m_iAddress; }

	bool GetUsable() { return m_bUsable; }
	void SetUsable( bool bVal ) { m_bUsable = bVal; }

//	void SetName( CString strName ) { m_strName = strName; }
//	CString GetName() { return m_strName; }

protected:
	CString OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt = 0);
	
protected:
	int						m_nPort;
	int						m_iAddress;

	std::vector<CString>	m_vecRev; 
	CString					m_strRecv_Buf;
	CString					m_strOldRecv_Buf;

	int						m_iStep_Receive;
	DWORD					m_dwWaitTime;
	DWORD					m_dwRecvWaitTime;

	std::vector<stSendMsg>	m_vecSendMsg;
	DWORD					m_dwTime_SendStart;

	int						m_iSendID;

	int						m_nHead;

	DWORD					m_dwSendTime;

	bool					m_bUsable;

	CString					m_strName;
};

//////////////////////////////////////////////////////////////////////////
class ANetwork_Client : public ANetwork
{
public:
	ANetwork_Client();
	virtual ~ANetwork_Client();

	// Server
	void OnClientAccept();
	DWORD OnClientSendToServer( CString strSend );
	void OnClientClose();

	virtual void Run_Move();
	void Run_Send2Server();
	void C_Step_SendMsg_Start( stSendMsg& msg );
	void C_Step_SendMsg_Accept( stSendMsg& msg );
	void C_Step_SendMsg_Wait( stSendMsg& msg );
	void C_Step_SendMsg_Send( stSendMsg& msg );
	void C_Step_SendMsg_Finish_Chk( stSendMsg& msg );
	void C_Step_SendMsg_Retry( stSendMsg& msg );
	void C_Step_SendMsg_Finish( stSendMsg& msg );

	virtual void OnParseComplete( std::string strRecv )=0;

	CString OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt = 0);

	void OnClientReceived();

public:

	CString GetIP()				{ return m_strIP; }
	DWORD GetDwIP();
	void SetIP( CString strIP )	{ m_strIP = strIP; }
	CString* GetPointer_IP()	{ return &m_strIP; }
	void ClearClient()			{ m_bAccept = false; m_bReady = true;}
	
	CClientSocket*	GetPointer_Client() { return m_pClient; }

	void SetName( CString strName ) { m_strName = strName; }
	CString GetName() { return m_strName; }

protected:	
	CClientSocket*	m_pClient;
	
	bool			m_bAccept;
	bool			m_bReady;

protected:
	CString			m_strIP;
	CString			m_strName;
};

//////////////////////////////////////////////////////////////////////////

class ANetwork_Server : public ANetwork
{
public:
	ANetwork_Server();
	virtual ~ANetwork_Server();

	bool OnServerOpen( int iID );
	void OnServerReceived( CServerItem* psItem );
	DWORD OnServerSend( CString strSend );

	void S_Step_SendMsg_Wait( stSendMsg& msg );
	void S_Step_SendMsg_Send( stSendMsg& msg );
	void S_Step_SendMsg_Finish_Chk( stSendMsg& msg );
	void S_Step_SendMsg_Retry( stSendMsg& msg );
	void S_Step_SendMsg_Finish( stSendMsg& msg );

	virtual void Run_Move();
	void Run_Send2Client();

	virtual void OnParseComplete( std::string strRecv )=0;

	CServerSocket*	GetPointer_Server() { return m_pServer; }

protected:
	CServerSocket*			m_pServer;
};

#endif // !defined(AFX_ANETWORK_H__16110310_634E_4426_ADF5_8A7ACCED6409__INCLUDED_)
