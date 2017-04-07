// ANetwork.cpp: implementation of the ANetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "handler.h"
#include "ANetwork.h"
#include "ACriticalSection.h"
#include "../SrcPart/APartNetworkManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMutex mutexNet;
CMutex mutexClientNet;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ANetwork::ANetwork()
{
	m_nPort = -1;
	m_iAddress = -1;

	m_vecRev.clear();
	m_vecSendMsg.clear();

	m_iSendID = 0;

	m_nHead = -1;

	m_bUsable = true;
}

ANetwork::~ANetwork()
{

}

void ANetwork::Run_Move()
{

}

CString ANetwork::GetPortStr()
{
	char szBuf[8];
	itoa( m_nPort, szBuf, 10 );

	return szBuf;
}

CString ANetwork::Parse_Received()
{
	int nLength;

	CString strBody;
	if( (m_vecRev.size() == 0 ) && m_strRecv_Buf == "" )
		return "";

//	m_strRecv_Buf = "";

	CString strVecRev;
	strVecRev.Empty();
	if( m_vecRev.size() > 0 )
	{
		strVecRev = m_vecRev.at(0);
		std::vector<CString>::iterator iter = m_vecRev.begin();
		m_vecRev.erase(iter);
	}

	m_strRecv_Buf += strVecRev;

	CString sPartLength, sPartBody;
	int nBodyLen = -1;
	nLength = m_strRecv_Buf.GetLength();
	
	int nPos[2] = {0,}, nHead;
	nPos[0]	= -1;
	nPos[0]	= m_strRecv_Buf.Find("HD=", 0);
	if(nPos[0] > -1)
	{
		nLength	= m_strRecv_Buf.GetLength() - 10;
		nHead = atoi(m_strRecv_Buf.Mid(nPos[0]+3, 6));
		if(nHead == nLength)
		{
			if(m_strOldRecv_Buf == m_strRecv_Buf)
			{
				m_strOldRecv_Buf = m_strRecv_Buf = "";
				return "";
			}
			m_strOldRecv_Buf = m_strRecv_Buf;
			return m_strRecv_Buf;
		}
		else
		{
			m_strRecv_Buf.Empty();
		}
	}
	else if(nLength > 16)
	{
		strBody = m_strRecv_Buf.Left(16);
		sPartBody = m_strRecv_Buf.Mid( 16 );
		m_nHead = atoi( (LPCSTR)strBody );
		nBodyLen	= sPartBody.GetLength();

		if(m_strRecv_Buf.GetAt(0) >= '0' && m_strRecv_Buf.GetAt(0) <= '9')
		{
			// 정상적인 데이터
		}
		else
		{
			if(nBodyLen >= m_nHead)
			{
				m_strRecv_Buf.Empty();
			}
			return "";
		}
	}
	else
	{
		// 수신 데이터를 처리할 수 없는 상태임
		return "";
	}
	if(m_nHead < 0)
	{
		m_strRecv_Buf.Empty();
		return "";
	}

	if(nBodyLen < m_nHead)
	{
		return "";
	}

	CString sData_Process;
	sData_Process = m_strRecv_Buf.Mid(0, m_nHead+16);
	nLength = sData_Process.GetLength();

	m_strRecv_Buf = m_strRecv_Buf.Mid(nLength);

	return sData_Process;

//	CString strRecv = m_strRecv_Buf + strVecRev;
//	nLength = strRecv.GetLength();
//	// HEAD 
//	if( strRecv.GetAt(0) >= '0' && strRecv.GetAt(0) <= '9' )
//	{
//		if( nLength < 16 )
//		{
//			m_strRecv_Buf = strRecv;
//			return "";
//		}
//		
//		strBody = strRecv.Left(16);
//		m_strRecv_Buf = strRecv.Mid( 16 );
//		m_nHead = atoi( (LPCSTR)strBody );
//		//return strBody;//2013,0910
//		return strRecv;
//	}
//
//	if( m_nHead < 0 )
//	{
//		m_strRecv_Buf = "";
//		return "";
//	}
//
//	if( nLength < m_nHead )
//	{
//		m_strRecv_Buf = strRecv;
//		return "";
//	}
//
//	strBody = strRecv.Left( m_nHead );
//	m_strRecv_Buf = strRecv.Mid( m_nHead );
//	m_nHead = -1;
//
//	return strBody;

}

void ANetwork::Run_Receive()
{
	g_cs.EnterCriticalSection();
	switch( m_iStep_Receive )
	{
	case RECV_STEP_WAIT:
		m_dwRecvWaitTime = GetCurrentTime();
		m_iStep_Receive = RECV_STEP_PARSE;
		break;
	case RECV_STEP_PARSE:
		{
			long lTimeGap = GetCurrentTime() - m_dwRecvWaitTime;
			if( lTimeGap > 50 )
			{
				OnParseComplete( (LPCSTR)Parse_Received() );
				m_iStep_Receive = RECV_STEP_WAIT;
			}
			else if(lTimeGap < 0)
			{
				m_dwRecvWaitTime = GetCurrentTime();
			}

		}
		break;
	}
	g_cs.LeaveCriticalSection();
}

int ANetwork::PushSendMsg( LPCTSTR szMsg, int iErrMax, DWORD dwType )
{
	stSendMsg msg;
	{
		msg.iID = m_iSendID;
		msg.dwType = dwType;
		msg.iErrMax = iErrMax;
		
		msg.iStep = SENDMSG_STEP_START;
		msg.iErrCount = 0;
		msg.bSuccess = false;
		msg.dwLastTime = GetCurrentTime();
		msg.bReceived = false;
		
		msg.strMsg = szMsg;
	}

	m_vecSendMsg.push_back( msg );

	m_iSendID++;
	if( m_iSendID > 1000 )
	{
		m_iSendID = 0;
	}

// 	Func.On_Log_Title_Msg( LOG_TCPIP, "+ ADD MSG", msg.strMsg.c_str() );

	return msg.iID;
}

CString ANetwork::OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt)
{
	CString strFuncRet = _T("");//2016.0302
	int n_title, n_equal, n_end, n_length, n_qw = 0;   

	strFind += "=";
	n_title =	strBody.Find(strFind, 0);							// TITLE의 위치를 찾는다.
	n_equal =	strBody.Find("=", n_title + 1);						// 해당 item의 끝 위치를 찾는다.
	
	if (n_title == -1 || n_equal == -1)		return strFuncRet;
	
	if (nOpt == 0)
	{
		n_end =		strBody.Find(" ", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		if( n_end < 0 )	n_end = strBody.GetLength();

		n_length =	n_end - n_equal - 1;							// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if (nOpt == 7)
	{
		strFuncRet = strBody.Mid(n_title + 5);
	}
	else if (nOpt == 2)
	{
		n_end =		strBody.Find("=", n_title + 1);					// 해당 item의 끝 위치를 찾는다.						// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_end + 1);
	}
	else if (nOpt == 3)
	{
		n_end =		strBody.Find("=", n_title + 1);					// 해당 item의 끝 위치를 찾는다.						// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_end + 1, 1);
	}
	else if (nOpt == 4)
	{
		n_qw =		strBody.Find("((", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_end =		strBody.Find("))", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_qw - 1;								// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_qw + 2, n_length - 1);
	}
	else if (nOpt == 5)
	{
		n_qw =		strBody.Find("FAIL_REGNUMBER", n_title + 22);					// 해당 item의 끝 위치를 찾는다.
		n_equal =	strBody.Find("=", n_qw + 1);	
		n_end =		strBody.Find(" ", n_qw + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_equal - 1;							// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if (nOpt == 6)
	{
		n_qw =		strBody.Find("ID", n_title + 6);					// 해당 item의 끝 위치를 찾는다.
		n_equal =	strBody.Find("=", n_qw + 1);	
		n_end =		strBody.Find(" ", n_qw + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_equal - 1;							// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if ( nOpt == 8)
	{
		n_qw =		strBody.Find("\"", n_equal + 1 );
		n_end =		strBody.Find("\"", n_equal + 2 );
		n_length =	n_end - n_qw - 1;
		strFuncRet = strBody.Mid(n_qw + 1, n_length);
	}
	else
	{
		n_qw =		strBody.Find("(", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_end =		strBody.Find(")", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_qw - 1;								// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_qw + 1, n_length);
	}
	
	strFuncRet.MakeUpper();
	strFuncRet.TrimLeft(' ');
	strFuncRet.TrimRight(' ');

	return strFuncRet;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
ANetwork_Client::ANetwork_Client()
{
	m_bAccept = false;
	m_bReady = true;

	m_iStep_Receive = RECV_STEP_WAIT;
	m_strRecv_Buf = "";
}

ANetwork_Client::~ANetwork_Client()
{
	if( m_pClient != NULL )
	{
		m_pClient->Close();
		delete m_pClient;
		m_pClient = NULL;
	}
}

DWORD ANetwork_Client::GetDwIP()
{
	DWORD dwTmp[4];
	sscanf( (LPCSTR)m_strIP, "%d.%d.%d.%d", &dwTmp[0], &dwTmp[1], &dwTmp[2], &dwTmp[3] );
	DWORD dwRtn = (dwTmp[0] << 24) + (dwTmp[1] << 16) + (dwTmp[2] << 8) + dwTmp[3];

	return dwRtn;
}
void ANetwork_Client::OnClientAccept()
{
	if (m_pClient == NULL)
	{
		m_pClient = new CClientSocket;
		
		if ( !m_pClient->Create(m_iAddress, (LPTSTR)(LPCTSTR)m_strIP, m_nPort) )
		{
			delete m_pClient;
			m_pClient = NULL;
			
			if (st_handler.cwnd_list != NULL)
			{
				sprintf(st_msg.c_normal_msg, "Connect Fail To [%s]", g_Net.GetAddressName_C(m_iAddress));
				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			}
			
			m_bAccept = false;
			return;
		}
	}
	
	m_bAccept = true;
}

DWORD ANetwork_Client::OnClientSendToServer( CString strSend )
{
//	g_cs.EnterCriticalSection();

 	DWORD nRtn = SOCKET_ERROR;
 	std::string strLog;

 	if( m_pClient == NULL )
 	{
 		strLog = "Send Fail ( mp_Client == NULL ------->";
 		strLog.append( strSend );
//  		Func.On_LogFile_Add( LOG_TCPIP, strLog.c_str() );
 	}
 	
 	if (m_pClient != NULL)
 	{
 		nRtn = m_pClient->Send(LPCTSTR(strSend), strSend.GetLength());

// 	   nRtn = SOCKET_ERROR;

//		if( GetLastError() == WSAEWOULDBLOCK || nRtn == SOCKET_ERROR )
		if( nRtn == SOCKET_ERROR )
		{
			sprintf(st_msg.c_abnormal_msg, "C_Send to [%s] <FAIL>: %s", g_Net.GetAddressName_C(m_iAddress), strSend);
			//if (st_handler.cwnd_list != NULL) 
			//	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
 			Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);


			//OnClientClose();  //2013,0813 ybs
			//m_pClient->OnClose(nRtn);

// 			delete m_pClient;
// 			m_pClient = NULL;
// 			m_bAccept = false;


			sprintf(st_msg.c_abnormal_msg, "C_Send to [%s] <FAIL> OnClientClose: %s", g_Net.GetAddressName_C(m_iAddress), strSend);
			//if (st_handler.cwnd_list != NULL) 
			//	st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);
			nRtn = SOCKET_ERROR;
		}
 		else
 		{
 			sprintf(st_msg.c_abmsg, "C_Send to [%s] : <SUSPENDED> %s", g_Net.GetAddressName_C(m_iAddress), strSend);
//  			if (st_handler.cwnd_list != NULL) 
// 				st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			Func.On_LogFile_Add(99, st_msg.c_abmsg);
			for(int k = 0; k < 1000000; k++ )
			{
				int a = 0;
			}
			m_dwSendTime = GetCurrentTime();
 		}
	}

//	g_cs.LeaveCriticalSection();
	return nRtn;
}

void ANetwork_Client::OnClientClose()
{
	if (m_pClient != NULL)
	{
		//2015.1008
		if (m_pClient->m_byteReceive != NULL)
		{
			delete m_pClient->m_byteReceive;
			m_pClient->m_byteReceive = NULL;
		}

		delete m_pClient;
		m_pClient = NULL;

		//2016.0117
		//st_handler.mn_xgem_connect = CTL_NO;
		
		m_bAccept = false;
	}
	m_bReady = true;
}
void ANetwork_Client::Run_Move()
{
	Run_Send2Server();
	Run_Receive();
}

void ANetwork_Client::Run_Send2Server()
{
 	if( m_vecSendMsg.size() == 0 )
 		return;

	CSingleLock sing(&mutexNet);
	sing.Lock();

 	stSendMsg& msg = m_vecSendMsg.at(0);


 	switch (msg.iStep)
 	{
 	case SENDMSG_STEP_START:		C_Step_SendMsg_Start( msg );			break;
 	case SENDMSG_STEP_ACCEPT:		C_Step_SendMsg_Accept( msg );			break;
 	case SENDMSG_STEP_WAIT:			C_Step_SendMsg_Wait( msg );				break;
 	case SENDMSG_STEP_SEND:			C_Step_SendMsg_Send( msg );				break;
 	case SENDMSG_STEP_FINISH_CHECK:	C_Step_SendMsg_Finish_Chk( msg );		break;
 	case SENDMSG_STEP_RETRY:		C_Step_SendMsg_Retry( msg );			break;
 	case SENDMSG_STEP_FINISH:		C_Step_SendMsg_Finish( msg );			break;
	}

	sing.Unlock();

}

void ANetwork_Client::C_Step_SendMsg_Start( stSendMsg& msg )
{
	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	if ( m_bReady )
	{
		m_bReady = false;
		
		if ( m_bAccept )
		{
			msg.iStep  = SENDMSG_STEP_SEND;
// 			Func.On_Log_Title_Msg( LOG_TCPIP, "START ECSERVER SENDMSG -> SEND", strLog );

			//2015.0711
// 			if (m_pClient != NULL)
// 			{
// 				delete m_pClient;
// 				m_pClient = NULL;
// 				
// 				m_bAccept = false;
// 			}
// 			if( st_handler.cwnd_list )
// 				st_handler.cwnd_list->SendMessage(WM_SOCKET_ACCEPT, m_iAddress );
// 			
// 			m_dwTime_SendStart = GetCurrentTime();
// 			msg.iStep  = SENDMSG_STEP_ACCEPT;
		}
		else
		{
//			if( st_handler.cwnd_list )
//				st_handler.cwnd_list->SendMessage(WM_SOCKET_ACCEPT, m_iAddress );
			
			m_dwTime_SendStart = GetCurrentTime();
			msg.iStep  = SENDMSG_STEP_ACCEPT;
// 			Func.On_Log_Title_Msg( LOG_TCPIP, "START ECSERVER SENDMSG -> ACCEPT", strLog );
		}
	}
}

void ANetwork_Client::C_Step_SendMsg_Accept( stSendMsg& msg )
{
	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	if ( m_bAccept )
	{
		msg.iStep  = SENDMSG_STEP_SEND;
// 		Func.On_Log_Title_Msg( LOG_TCPIP, "ACCEPT ECSERVER SENDMSG -> SEND", strLog );
	}
	else
	{
		DWORD dwTimeCount = GetCurrentTime() - m_dwTime_SendStart;
		if (dwTimeCount > 3000)
		{
			msg.iErrCount++;
			
			if (msg.iErrCount > msg.iErrMax)
			{
				msg.iStep = SENDMSG_STEP_FINISH;
// 				Func.On_Log_Title_Msg( LOG_TCPIP, "ACCEPT ECSERVER SENDMSG -> FINISH", strLog );
				//응답이 없었습니다.
				m_bUsable = false; //2012,1225

				//2016.0117
//				if( st_handler.cwnd_main )
//					st_handler.cwnd_main->PostMessage( WM_DRAW_DATA_MAIN, EDDN_NETUSABLE);

			}
			else
			{
				m_bReady = CTL_YES;
				msg.iStep  = SENDMSG_STEP_START;
// 				Func.On_Log_Title_Msg( LOG_TCPIP, "ACCEPT ECSERVER SENDMSG -> START", strLog );
			}
		}
		else if(dwTimeCount < 0)
		{
			m_dwTime_SendStart = GetCurrentTime();
		}
	}

}

void ANetwork_Client::C_Step_SendMsg_Wait( stSendMsg& msg )
{
	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	DWORD dwTime = GetCurrentTime() - msg.dwLastTime;
	if( dwTime > 3000/*500*/ )
	{
		msg.iStep = SENDMSG_STEP_START;
		msg.iErrCount++;
		
		// 결과를 받으면 종료
		if( msg.bReceived )
		{
			msg.iStep = SENDMSG_STEP_FINISH;
			return;		
		}
		if( msg.iErrCount > msg.iErrMax )
		{
			msg.iStep = SENDMSG_STEP_FINISH;
// 			Func.On_Log_Title_Msg( LOG_TCPIP, "SEND ECSERVER WAIT -> FINISH (by Fail)", strLog );
			return;
		}
 	}
 	else
	{
		if( m_vecSendMsg.size() > 1 )
		{
			sprintf(st_msg.c_abnormal_msg, "C_Step_SendMsg_Wait [%s] <m_vecSendMsg.erase>", g_Net.GetAddressName_C(m_iAddress));
			Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);

			stSendMsg stTemp = msg;
			std::vector<stSendMsg>::iterator iter = m_vecSendMsg.begin();
			m_vecSendMsg.erase(iter);
			m_vecSendMsg.push_back( stTemp );
		}
	}
}

void ANetwork_Client::C_Step_SendMsg_Send( stSendMsg& msg )
{
	//2012,1220
	if( GetCurrentTime() - m_dwSendTime < 0 )
	{
		m_dwSendTime = GetCurrentTime();
	}
	if( GetCurrentTime() - m_dwSendTime < 100 )
		return;

	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	CString strHead;
	strHead.Format( "%016d", msg.strMsg.size() );
	DWORD nRtn = SOCKET_ERROR;
	if( OnClientSendToServer( strHead ) != SOCKET_ERROR )
		nRtn = OnClientSendToServer( msg.strMsg.c_str() );
	if( nRtn == SOCKET_ERROR )
	{
		msg.dwLastTime = GetCurrentTime();
		msg.iStep = SENDMSG_STEP_WAIT;
		m_bReady = CTL_YES;
		sprintf(st_msg.c_abnormal_msg, "C_Step_SendMsg_Send [%s] <SOCKET_ERROR>", g_Net.GetAddressName_C(m_iAddress));
		Func.On_LogFile_Add(99, st_msg.c_abnormal_msg);
		return;
	}	
	m_dwWaitTime = GetCurrentTime();
	msg.bSuccess = true;
	msg.iStep = SENDMSG_STEP_FINISH_CHECK;
}

void ANetwork_Client::C_Step_SendMsg_Finish_Chk( stSendMsg& msg )
{
	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	// 응답이 없는 것이면 끝내고, 
	// 응답을 받아야 하거나 실패한 것은 맨뒤로 보내고, 받는 처리를 한다.
	if( msg.bSuccess == false )
		msg.iStep = SENDMSG_STEP_RETRY;	
	else
		msg.iStep = SENDMSG_STEP_FINISH;
}

void ANetwork_Client::C_Step_SendMsg_Retry( stSendMsg& msg )
{
	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	msg.dwLastTime = GetCurrentTime();
 	msg.iStep = SENDMSG_STEP_WAIT;
	
	m_bReady = CTL_YES;
	
	if( m_vecSendMsg.size() > 1 )
	{
		stSendMsg stTemp = msg;
		std::vector<stSendMsg>::iterator iter = m_vecSendMsg.begin();
		m_vecSendMsg.erase(iter);
		m_vecSendMsg.push_back( stTemp );
	}
}

void ANetwork_Client::C_Step_SendMsg_Finish( stSendMsg& msg )
{
	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	std::vector<stSendMsg>::iterator iter = m_vecSendMsg.begin();
	m_vecSendMsg.erase(iter);
	m_bReady = true;
	
// 	Func.On_Log_Title_Msg( LOG_TCPIP, "* COMPLETE ECSERVER SENDMSG", strLog );
//	OnClientClose();
}

void ANetwork_Client::OnClientReceived()
{
	if( m_pClient == NULL )	return;
	if (m_pClient->m_nReceiveLen < 0 || m_pClient->m_nReceiveLen >= RECEIVE_MAX)	return;;
	
	g_cs.EnterCriticalSection();


	char szRecv[RECEIVE_MAX];
	memset(szRecv, 0, sizeof(m_pClient->m_nReceiveLen));
	memcpy(szRecv, m_pClient->m_byteReceive, m_pClient->m_nReceiveLen);
	szRecv[m_pClient->m_nReceiveLen] = 0;

	CString strRev = szRecv;
	m_vecRev.push_back( strRev );

	if( m_vecRev.size() > 100 )
	{
		m_vecRev.clear();
	}

	//2014,0311
	CString strRecvLog;
	strRecvLog.Format( "C_RECV From [%s]: %s", g_Net.GetAddressName_C( m_iAddress ), strRev );
	sprintf(st_msg.c_msg, (LPCSTR)strRecvLog);
	//if( st_handler.cwnd_list ) st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
	Func.On_LogFile_Add(99, st_msg.c_msg);
// 	Func.On_Log_Title_Msg(LOG_TCPIP, "ANetwork_Client::OnClientReceived", strRecvLog);


	g_cs.LeaveCriticalSection();
}

CString ANetwork_Client::OnNetworkBodyAnalysis(CString strBody, CString strFind, int nOpt)
{
	CString strFuncRet = _T("");//2016.0302
	int n_title, n_equal, n_end, n_length, n_qw = 0;    
	
	strFind += "=";
	n_title =	strBody.Find(strFind, 0);							// TITLE의 위치를 찾는다.
	n_equal =	strBody.Find("=", n_title + 1);						// 해당 item의 끝 위치를 찾는다.
	
	if (n_title == -1)		return strFuncRet;
	
	if (nOpt == 0)
	{
		n_end =		strBody.Find(" ", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		if( n_end < 0 ) 
			n_end = strBody.GetLength();
		
		n_length =	n_end - n_equal - 1;							// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if (nOpt == 7)
	{
		strFuncRet = strBody.Mid(n_title + 5);
	}
	else if (nOpt == 2)
	{
		n_end =		strBody.Find("=", n_title + 1);					// 해당 item의 끝 위치를 찾는다.						// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_end + 1);
	}
	else if (nOpt == 3)
	{
		n_end =		strBody.Find("=", n_title + 1);					// 해당 item의 끝 위치를 찾는다.						// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_end + 1, 1);
	}
	else if (nOpt == 4)
	{
		n_qw =		strBody.Find("((", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_end =		strBody.Find("))", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_qw - 1;								// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_qw + 2, n_length - 1);
	}
	else if (nOpt == 5)
	{
		n_qw =		strBody.Find("FAIL_REGNUMBER", n_title + 22);					// 해당 item의 끝 위치를 찾는다.
		n_equal =	strBody.Find("=", n_qw + 1);	
		n_end =		strBody.Find(" ", n_qw + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_equal - 1;							// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if (nOpt == 6)
	{
		n_qw =		strBody.Find("ID", n_title + 6);					// 해당 item의 끝 위치를 찾는다.
		n_equal =	strBody.Find("=", n_qw + 1);	
		n_end =		strBody.Find(" ", n_qw + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_equal - 1;							// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_equal + 1, n_length);
	}
	else if( nOpt == 8)
	{
		n_qw =		strBody.Find("\"", n_equal + 1 );
		n_end =		strBody.Find("\"", n_equal + 2 );
		n_length =	n_end - n_qw - 1;
		strFuncRet = strBody.Mid(n_qw + 1, n_length);
	}
	else
	{
		n_qw =		strBody.Find("(", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_end =		strBody.Find(")", n_title + 1);					// 해당 item의 끝 위치를 찾는다.
		n_length =	n_end - n_qw - 1;								// DATA의 길이 계산.
		strFuncRet = strBody.Mid(n_qw + 1, n_length);
	}
	
	strFuncRet.MakeUpper();
	strFuncRet.TrimLeft(' ');
	strFuncRet.TrimRight(' ');

	return strFuncRet;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
ANetwork_Server::ANetwork_Server()
{

}

ANetwork_Server::~ANetwork_Server()
{
	if( m_pServer != NULL )
	{
		m_pServer->Close();
		delete m_pServer;
		m_pServer = NULL;
	}
}

void ANetwork_Server::OnServerReceived( CServerItem* psItem )
{
	if (psItem->m_nReceiveLen < 0 || psItem->m_nReceiveLen >= RECEIVE_MAX) return;

	g_cs.EnterCriticalSection();
	
	char szRecv[RECEIVE_MAX];
	memset(szRecv, 0, sizeof(psItem->m_nReceiveLen));
	memcpy(szRecv, psItem->m_byteReceive, psItem->m_nReceiveLen);
	szRecv[psItem->m_nReceiveLen] = 0;

	CString strRev = szRecv;
	m_vecRev.push_back( strRev );

	if( m_vecRev.size() > 100 )
	{
		m_vecRev.clear();
	}

	//2014,0311
	CString strRecvLog;
	strRecvLog.Format( "S_RECV From [%s]: %s", g_Net.GetAddressName_S( m_iAddress ), strRev );
	sprintf(st_msg.c_msg, (LPCSTR)strRecvLog);
	//if( st_handler.cwnd_list != NULL) st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);
	Func.On_LogFile_Add(99, st_msg.c_msg);
// 	Func.On_Log_Title_Msg(LOG_TCPIP, "ANetwork_Client::OnServerReceived", strRecvLog);

	g_cs.LeaveCriticalSection();
}

bool ANetwork_Server::OnServerOpen( int iID )
{
	if (m_pServer == NULL)
	{
		m_pServer = new CServerSocket;
		UINT nPort = m_nPort;
		
		if (!m_pServer->Create(nPort, iID) )
		{
			delete m_pServer;
			m_pServer = NULL;
			
			//sprintf(st_msg.mc_abnormal_msg, "#%d Port : %d Server Fail", i, mn_port[i]);
			//OnListDataUpdate(0, ABNORMAL_MSG);
			sprintf(st_msg.c_msg, "#%d Port : %d Server Fail", iID, nPort);
			//if( st_handler.cwnd_list ) st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			Func.On_LogFile_Add(99, st_msg.c_msg);

			m_iAddress = -1;
			return false;
		}
		else
		{
			//				st_msg.mstr_normal_msg.Format("#%d Port : %d Server Create", i, mn_port[i]);
			//sprintf(st_msg.mc_normal_msg, "#%d Port : %d Server Create", i, mn_port[i]);
			//OnListDataUpdate(0, NORMAL_MSG);
			sprintf(st_msg.c_msg, "#%d Port : %d Server Create", iID, nPort);
			//if( st_handler.cwnd_list ) st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			Func.On_LogFile_Add(99, st_msg.c_msg);

		}
	}

	m_iAddress = iID;
	return true;
}

DWORD ANetwork_Server::OnServerSend(CString strSend )
{
//	g_cs.EnterCriticalSection();

 	DWORD nRtn = SOCKET_ERROR;
 	std::string strLog;

 	if( m_pServer == NULL )
 	{
 		strLog = "Send Fail ( mp_Server == NULL ------->";
 		strLog.append( strSend );
//  		Func.On_LogFile_Add( LOG_TCPIP, strLog.c_str() );

//		g_cs.LeaveCriticalSection();
		return nRtn;
 	}
 	
 	if (m_pServer != NULL)
 	{
 		// bpc server에 데이터 전송
 		int nCnt = m_pServer->GetClientCount();
		if ( nCnt > 0)
		{
			nRtn = m_pServer->SendClientAt( m_pServer->GetClient(nCnt - 1), (LPCSTR)strSend, strSend.GetLength() );
		}
		else
		{
			//"Send Fail : No Client";
		}

 		if( nRtn == SOCKET_ERROR )
 		{
 			sprintf(st_msg.c_msg, "S_SEND to [%s] <FAIL>: %s", g_Net.GetAddressName_S(m_iAddress), strSend);
//  			if (st_handler.cwnd_list != NULL) 
// 				st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, ABNORMAL_MSG);
			Func.On_LogFile_Add(99, st_msg.c_msg);
 		}
 		else
 		{
 			sprintf(st_msg.c_msg, "S_SEND to [%s] : %s", g_Net.GetAddressName_S(m_iAddress), strSend);
//  			if (st_handler.cwnd_list != NULL) 
// 				st_handler.cwnd_list->SendMessage(WM_LIST_DATA, 0, NORMAL_MSG);
			Func.On_LogFile_Add(99, st_msg.c_msg);
			m_dwSendTime = GetCurrentTime();
 		}
	}

//	g_cs.LeaveCriticalSection();
	return nRtn;
}

void ANetwork_Server::Run_Move()
{
	Run_Send2Client();
	Run_Receive();
}

void ANetwork_Server::Run_Send2Client()
{
	if( m_vecSendMsg.size() == 0 )
 		return;

	//2016.1031
	CSingleLock sing(&mutexClientNet);
	sing.Lock();


 	stSendMsg& msg = m_vecSendMsg.at(0);

 	switch (msg.iStep)
 	{
 	case SENDMSG_STEP_START:		msg.iStep = SENDMSG_STEP_SEND;			break;
 	case SENDMSG_STEP_WAIT:			S_Step_SendMsg_Wait( msg );				break;
 	case SENDMSG_STEP_SEND:			S_Step_SendMsg_Send( msg );				break;
 	case SENDMSG_STEP_FINISH_CHECK:	S_Step_SendMsg_Finish_Chk( msg );		break;
 	case SENDMSG_STEP_RETRY:		S_Step_SendMsg_Retry( msg );			break;
 	case SENDMSG_STEP_FINISH:		S_Step_SendMsg_Finish( msg );			break;
	}

	sing.Unlock();

}

void ANetwork_Server::S_Step_SendMsg_Wait( stSendMsg& msg )
{
	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	DWORD dwTime = GetCurrentTime() - msg.dwLastTime;
	if( dwTime > 500 )
	{
		msg.iStep = SENDMSG_STEP_START;
		msg.iErrCount++;
		
		// 결과를 받으면 종료
		if( msg.bReceived )
		{
			msg.iStep = SENDMSG_STEP_FINISH;
			return;		}
		if( msg.iErrCount > msg.iErrMax )
		{
			msg.iStep = SENDMSG_STEP_FINISH;
// 			Func.On_Log_Title_Msg( LOG_TCPIP, "SEND ECSERVER WAIT -> FINISH (by Fail)", strLog );
			return;
		}
 	}
 	else
	{
		if( m_vecSendMsg.size() > 1 )
		{
			stSendMsg stTemp = msg;
			std::vector<stSendMsg>::iterator iter = m_vecSendMsg.begin();
			m_vecSendMsg.erase(iter);
			m_vecSendMsg.push_back( stTemp );
		}
	}
}

void ANetwork_Server::S_Step_SendMsg_Send( stSendMsg& msg )
{
	//2012,1220
	if( GetCurrentTime() - m_dwSendTime < 0 )
	{
		m_dwSendTime = GetCurrentTime();
	}
	if( GetCurrentTime() - m_dwSendTime < 100 )
		return;

	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	CString strHead;
	strHead.Format("%016d", msg.strMsg.size() );
	DWORD nRtn = SOCKET_ERROR;
	if( OnServerSend( strHead ) != SOCKET_ERROR )
		nRtn = OnServerSend( msg.strMsg.c_str() );
	if( nRtn == SOCKET_ERROR )
	{
		msg.dwLastTime = GetCurrentTime();
		msg.iStep = SENDMSG_STEP_WAIT;
// 		Func.On_Log_Title_Msg( LOG_TCPIP, "SEND ECSERVER SENDMSG -> WAIT", strLog );
		return;
	}
	
	m_dwWaitTime = GetCurrentTime();
	msg.bSuccess = true;
	msg.iStep = SENDMSG_STEP_FINISH_CHECK;
// 	Func.On_Log_Title_Msg( LOG_TCPIP, "SEND ECSERVER SENDMSG -> FINISH", strLog );
}
	
void ANetwork_Server::S_Step_SendMsg_Finish_Chk( stSendMsg& msg )
{
	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	// 응답이 없는 것이면 끝내고, 
	// 응답을 받아야 하거나 실패한 것은 맨뒤로 보내고, 받는 처리를 한다.
	if( msg.bSuccess == false )
		msg.iStep = SENDMSG_STEP_RETRY;	
	else
		msg.iStep = SENDMSG_STEP_FINISH;
}
	
void ANetwork_Server::S_Step_SendMsg_Retry( stSendMsg& msg )
{
	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	msg.dwLastTime = GetCurrentTime();
 	msg.iStep = SENDMSG_STEP_WAIT;
	
	if( m_vecSendMsg.size() > 1 )
	{
		stSendMsg stTemp = msg;
		std::vector<stSendMsg>::iterator iter = m_vecSendMsg.begin();
		m_vecSendMsg.erase(iter);
		m_vecSendMsg.push_back( stTemp );
	}
}
	
void ANetwork_Server::S_Step_SendMsg_Finish( stSendMsg& msg )
{
	CString strLog;
	strLog.Format( "[ %d ] %s", msg.iID, msg.strMsg.c_str() );

	std::vector<stSendMsg>::iterator iter = m_vecSendMsg.begin();
	m_vecSendMsg.erase(iter);
	
// 	Func.On_Log_Title_Msg( LOG_TCPIP, "* COMPLETE ECSERVER SENDMSG", strLog );
}
