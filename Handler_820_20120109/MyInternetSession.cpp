/****************************************************************/
/*																*/
/*  MyInternetSession.cpp										*/
/*																*/
/*  Implementation of the CMyInternetSession class.				*/
/*																*/
/*  Programmed by Pablo van der Meer							*/
/*  Copyright Pablo Software Solutions 2002						*/
/*	http://www.pablovandermeer.nl								*/
/*																*/
/*  Last updated: 15 may 2002									*/
/*																*/
/****************************************************************/


#include "stdafx.h"
#include "handler.h"
#include "MyInternetSession.h"
#include "Variable.h"			// ���� ���� ���� Ŭ���� �߰�

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CMyInternetSession::OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLen)
{
	CString strStatus;

	switch(dwInternetStatus){
		case INTERNET_STATUS_STATE_CHANGE:	
			strStatus.Format("[%d] Busy", AfxGetThread()->m_nThreadID);
			break;

		case INTERNET_STATUS_REQUEST_COMPLETE:
			strStatus.Format("[%d] request complete", AfxGetThread()->m_nThreadID);
			break;
		case INTERNET_STATUS_CONNECTING_TO_SERVER:
			strStatus.Format("[%d] Connecting to server...", AfxGetThread()->m_nThreadID);
			break;

		case INTERNET_STATUS_RESOLVING_NAME:
			strStatus.Format("[%d] resolving name for %s", AfxGetThread()->m_nThreadID, lpvStatusInformation);
			break;

		case INTERNET_STATUS_NAME_RESOLVED:	
			strStatus.Format("[%d] resolved name for %s!", AfxGetThread()->m_nThreadID, lpvStatusInformation);
			break;

		case INTERNET_STATUS_CONNECTED_TO_SERVER:
			strStatus.Format("[%d] Connected to server!", AfxGetThread()->m_nThreadID);
			break;

		case INTERNET_STATUS_SENDING_REQUEST:	
			strStatus.Format("[%d] Sending request...", AfxGetThread()->m_nThreadID);
			break;

		case INTERNET_STATUS_REQUEST_SENT:	
			strStatus.Format("[%d] Request sent", AfxGetThread()->m_nThreadID);
			break;
		case INTERNET_STATUS_RECEIVING_RESPONSE:
			strStatus.Format("[%d] Receiving response...", AfxGetThread()->m_nThreadID);
			break; 
		case INTERNET_STATUS_RESPONSE_RECEIVED:	
			strStatus.Format("[%d] Response received!", AfxGetThread()->m_nThreadID);
			break;
		case INTERNET_STATUS_CLOSING_CONNECTION:
			strStatus.Format("[%d] Closing connection...", AfxGetThread()->m_nThreadID);
			break;
		case INTERNET_STATUS_CONNECTION_CLOSED:	
			strStatus.Format("[%d] Connection close!", AfxGetThread()->m_nThreadID);
			break;
		case INTERNET_STATUS_HANDLE_CLOSING:
			strStatus.Format("[%d] Closing Connection...", AfxGetThread()->m_nThreadID);
			break;
		case INTERNET_STATUS_HANDLE_CREATED:
			strStatus.Format("[%d] Connection created!", AfxGetThread()->m_nThreadID);
			break;
		case INTERNET_STATUS_REDIRECT:
			strStatus.Format("[%d] Redirected to URL", AfxGetThread()->m_nThreadID);
			break;

		case INTERNET_STATUS_CTL_RESPONSE_RECEIVED:
		default:
			strStatus.Format("[%d] Unknown status: %d", AfxGetThread()->m_nThreadID, dwInternetStatus);
			break;
	}

	LPCSTR line = strStatus;

	if(st_handler.cwnd_list != NULL)
	{  // ����Ʈ �� ȭ�� ����
		st_other.str_normal_msg = strStatus;
		sprintf(st_other.c_normal_msg, st_other.str_normal_msg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
	}
}


CString CMyInternetSession::GetErrorString(DWORD dwErrorCode)
{
	CString strResult;
	
	switch(dwErrorCode)
	{
		case ERROR_INTERNET_OUT_OF_HANDLES:
			strResult = "No more handles could be generated at this time.";
			break;
		case ERROR_INTERNET_TIMEOUT:
            strResult = "The request has timed out.";
			break;
		case ERROR_INTERNET_EXTENDED_ERROR:
            // An extended error was returned from the server. This is
			//	typically a string or buffer containing a verbose error
            //   message. Call InternetGetLastResponseInfo to retrieve the error text.
		case ERROR_INTERNET_INTERNAL_ERROR:
			strResult = "An internal error has occurred.";
			break;
		case ERROR_INTERNET_INVALID_URL:
			strResult = "The URL is invalid.";
			break;
		case ERROR_INTERNET_UNRECOGNIZED_SCHEME:
			strResult = "The URL scheme could not be recognized or is not supported.";
			break;
		case ERROR_INTERNET_NAME_NOT_RESOLVED:
			strResult = "The server name could not be resolved.";
			break;
		case ERROR_INTERNET_PROTOCOL_NOT_FOUND:
			strResult = "The requested protocol could not be located.";
			break;
		case ERROR_INTERNET_INVALID_OPTION:
			strResult = "A request to InternetQueryOption or InternetSetOption specified an invalid option value.";
			break;
		case ERROR_INTERNET_BAD_OPTION_LENGTH:
			strResult = "The length of an option supplied to InternetQueryOption or "
						"InternetSetOption is incorrect for the type of option specified.";
			break;
		case ERROR_INTERNET_OPTION_NOT_SETTABLE:
			strResult = "The request option cannot be set, only queried.";
			break;
		case ERROR_INTERNET_SHUTDOWN:
			strResult = "The Win32 Internet function support is being shut down or unloaded.";
			break;
		case ERROR_INTERNET_INCORRECT_USER_NAME:
			strResult = "The request to connect and log on to an FTP server could "
						"not be completed because the supplied user name is incorrect.";
			break;
		case ERROR_INTERNET_INCORRECT_PASSWORD:
			strResult = "The request to connect and log on to an FTP server could "
						"not be completed because the supplied password is incorrect.";
			break;
		case ERROR_INTERNET_LOGIN_FAILURE:
			strResult = "The request to connect to and log on to an FTP server failed.";
			break;
		case ERROR_INTERNET_INVALID_OPERATION:
            strResult = "The requested operation is invalid.";
			break;
		case ERROR_INTERNET_OPERATION_CANCELLED:
            strResult = "The operation was canceled, usually because the handle on "
						"which the request was operating was closed before the operation completed.";
			break;
		case ERROR_INTERNET_INCORRECT_HANDLE_TYPE:
            strResult = "The type of handle supplied is incorrect for this operation.";
			break;
		case ERROR_INTERNET_INCORRECT_HANDLE_STATE:
            strResult = "The requested operation cannot be carried out because the "
						"handle supplied is not in the correct state. ";
			break;
		case ERROR_INTERNET_NOT_PROXY_REQUEST:
            strResult = "The request cannot be made via a proxy.";
			break;
		case ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND:
            strResult = "A required registry value could not be located.";
			break;
		case ERROR_INTERNET_BAD_REGISTRY_PARAMETER:
            strResult = "A required registry value was located but is an incorrect type or has an invalid value.";
			break;
		case ERROR_INTERNET_NO_DIRECT_ACCESS:
            strResult = "Direct network access cannot be made at this time.";
			break;
		case ERROR_INTERNET_NO_CONTEXT:
            strResult = "An asynchronous request could not be made because a zero context value was supplied.";
			break;
		case ERROR_INTERNET_NO_CALLBACK:
            strResult = "An asynchronous request could not be made because a callback function has not been set.";
			break;
		case ERROR_INTERNET_REQUEST_PENDING:
            strResult = "The required operation could not be completed because one or more requests are pending.";
			break;
		case ERROR_INTERNET_INCORRECT_FORMAT:
            strResult = "The format of the request is invalid.";
			break;
		case ERROR_INTERNET_ITEM_NOT_FOUND:
            strResult = "The requested item could not be located.";
			break;
		case ERROR_INTERNET_CANNOT_CONNECT:
            strResult = "The attempt to connect to the server failed.";
			break;
		case ERROR_INTERNET_CONNECTION_ABORTED:
            strResult = "The connection with the server has been terminated.";
			break;
		case ERROR_INTERNET_CONNECTION_RESET:
            strResult = "The connection with the server has been reset.";
			break;
		case ERROR_INTERNET_FORCE_RETRY:
            strResult = "Calls for the Win32 Internet function to redo the request.";
			break;
		case ERROR_INTERNET_INVALID_PROXY_REQUEST:
            strResult = "The request to the proxy was invalid.";
			break;
		case ERROR_INTERNET_HANDLE_EXISTS:
            strResult = "The request failed because the handle already exists.";
			break;
		case ERROR_FTP_TRANSFER_IN_PROGRESS:
            strResult = "The requested operation cannot be made on the FTP session "
						" handle because an operation is already in progress.";
			break;
		case ERROR_FTP_DROPPED:
            strResult = "The FTP operation was not completed because the session was aborted.";
			break;
	}

	if(st_handler.cwnd_list != NULL){  // ����Ʈ �� ȭ�� ����
		st_other.str_abnormal_msg = strResult;
		sprintf(st_other.c_abnormal_msg, st_other.str_abnormal_msg);
		st_handler.cwnd_list->PostMessage(WM_LIST_DATA, 0, NORMAL_MSG);  // ���� ���� ��� ��û
	}

	return strResult;
}
