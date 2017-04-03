#include "StdAfx.h"
//#include "LogFromat.h"
#include "CtlBd_Library.h"
#include "ComizoaPublic.h"
//#include "Variable.h"
//#include "io.h"			// 파일 존재 유무 검사 함수 호출하기 위해서는 반드시 필요
#include "AlgMemory.h"
#include "Public_Function.h"
#include "LogFromat.h"
#include "AMTLotManager.h"


#define LOG_NAME_W			"LOG_SHAREMEM_W"
// #define LOG_NAME_R			"LOG_SHAREMEM_R"

CLogFromat clsLog;

UINT OnMessageWrite(LPVOID lParam);

CLogFromat::CLogFromat(void)
{
	m_hMemMapLogW	= NULL;
	m_pQueueLogW	= NULL;

//	m_hMemMapLogR	= NULL;
//	m_pQueueLogR	= NULL;

	m_hMemMapLogW = CreateFileMapping(INVALID_HANDLE_VALUE,
									 NULL,
									 PAGE_READWRITE,
									 0,
									 sizeof(LOG_QUEUE_W),
									 (LPCTSTR)LOG_NAME_W);//(LPCWSTR)LOG_NAME_W);

	if (m_hMemMapLogW != NULL)
	{
		m_pQueueLogW = (P_LOG_QUEUE_W)MapViewOfFile(m_hMemMapLogW, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (m_pQueueLogW != NULL)
		{
			m_pQueueLogW->m_nFront	= 0;
			m_pQueueLogW->m_nRear		= 0;
			m_pQueueLogW->m_nCount	= 0;
		}
	}
	else
	{
		if (m_hMemMapLogW != NULL)
		{
			m_pQueueLogW = (P_LOG_QUEUE_W)MapViewOfFile(m_hMemMapLogW, FILE_MAP_READ, 0, 0, 0);
		}
	}
	int i;
	for (i=0; i<100; i++)
	{
		m_nMoveFlag[i] = NO;
	}	
}


CLogFromat::~CLogFromat(void)
{
	if (m_pQueueLogW != NULL)	UnmapViewOfFile(m_pQueueLogW);	
	if (m_hMemMapLogW != NULL)	CloseHandle(m_hMemMapLogW);
}

/*
UINT OnMessageWrite(LPVOID lParam)
{
	CLogFormat *pMsg = (CLogFormat *)lParam;

	while(pMsg->m_bThread)
	{
		if (pMsg->m_hMemMapLogR != NULL && pMsg->m_pQueueLogR != NULL)
		{
			if (pMsg->m_pQueueLogR->m_nCount != pMsg->m_nOldRearCount)
			{
				pMsg->m_nOldRearCount			= pMsg->m_pQueueLogR->m_nCount;
				pMsg->m_pQueueLogW->m_nCount	= pMsg->m_pQueueLogR->m_nCount;
			}
		}
	}

	return 0;
}
*/

CString	CLogFromat::OnCharToString(char chData[], int nLength)
{
	CString strTmp;
	BSTR	buf;
	int		nCnt;

	nCnt	= MultiByteToWideChar(CP_ACP, 0, chData, nLength, NULL, NULL);
	buf		= SysAllocStringLen(NULL, nCnt);
	MultiByteToWideChar(CP_ACP, 0, chData, nLength, buf, nCnt);
	strTmp.Format(_T("%s"), buf);

	SysFreeString(buf);

	return strTmp;
}


void CLogFromat::OnStringToChar(CString strData, char chData[])
{
// 	int nCnt;
// 	wchar_t *chBuf;
// 
// 	chBuf = strData.GetBuffer(strData.GetLength());
// 	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
// 	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}


int CLogFromat::OnFileSizeCheck(CString strFileName, long lSize, int nCheck)
{
	CFileFind finder;
	long Len=0, flag=0;
	BOOL bContinue;

	if(bContinue = finder.FindFile(strFileName))
	{	
		if (nCheck == YES)			// Size를 체크하는것이면...
		{
			finder.FindFile(strFileName);
			finder.FindNextFile();
			finder.GetFileName();
			Len = (long)finder.GetLength();
			if(Len > lSize)
			{
				flag = 1;
			}
			finder.Close();	
		}
		else						// Size를 확인하는것이면...
		{
			finder.FindFile(strFileName);
			finder.FindNextFile();
			finder.GetFileName();
			Len = (long)finder.GetLength();
			
			flag = Len;
			
			finder.Close();	
		}
	}
	
	return flag;
}

void CLogFromat::OnLogWrite(CString strMsg)
{
	CString strTemp;

	if (m_hMemMapLogW != NULL && m_pQueueLogW != NULL)
	{
		if (strMsg == _T(""))
		{
			return;
		}

		//OnStringToChar(strMsg, m_pQueueLogW->m_chMsg[m_pQueueLogW->m_nRear]);
		sprintf(m_pQueueLogW->m_chMsg[m_pQueueLogW->m_nRear], "%s", strMsg);

		m_pQueueLogW->m_nRear++;

		if (m_pQueueLogW->m_nRear >= 100) m_pQueueLogW->m_nRear = 0;
	
		m_pQueueLogW->m_nCount++;
	}
}

void CLogFromat::OnAddKeyAlarm(CString strDevice, CString strEvent, int nKey, CString *strKey, CString *strValue)
{
	int i;

	m_strDevice[ALARM_KEY]	= strDevice;
	m_strEvent[ALARM_KEY]	= strEvent;

	if (m_nKeyCount[ALARM_KEY] > 10) return;

	m_nKeyCount[ALARM_KEY] = nKey;
	for (i=0; i<nKey; i++)
	{
		m_strKey[ALARM_KEY][i]		= strKey[i];
		m_strValue[ALARM_KEY][i]	= strValue[i];
	}
}

void CLogFromat::OnLogMotorAlarm(CString strDevice, CString strEvent, int nMotor, int *nAxis, double *dPos)
{
	CString strKey[10];
	CString strValue[10];

	if (nMotor == 1)
	{
		strKey[0]	= _T("AXIS");
		strValue[0].Format(_T("%s"), GetDeviceID(nAxis[0]));
		strKey[1]	= _T("TARGET");
		strValue[1].Format(_T("%.3f"), dPos[0]);
		strKey[2]	= _T("CURRENT");
		strValue[2].Format(_T("%.3f"), COMI.Get_MotCurrentPos(nAxis[0]));

		OnAddKeyAlarm(strDevice, strEvent, 3, strKey, strValue);
	}
	else if (nMotor == 2)
	{
		strKey[0]	= _T("AXIS");
		strValue[0].Format(_T("%s"), GetDeviceID(nAxis[0]));
		strKey[1]	= _T("TARGET");
		strValue[1].Format(_T("%.3f"), dPos[0]);
		strKey[2]	= _T("CURRENT");
		strValue[2].Format(_T("%.3f"), COMI.Get_MotCurrentPos(nAxis[0]));

		strKey[3]	= _T("AXIS");
		strValue[3].Format(_T("%s"), GetDeviceID(nAxis[1]));
		strKey[4]	= _T("TARGET");
		strValue[4].Format(_T("%.3f"), dPos[1]);
		strKey[5]	= _T("CURRENT");
		strValue[5].Format(_T("%.3f"), COMI.Get_MotCurrentPos(nAxis[1]));

		OnAddKeyAlarm(strDevice, strEvent, 6, strKey, strValue);
	}
}
void CLogFromat::OnLogAlarm(CString strAlarmCode, int nStatus)
{
	LogAlarm(m_strDevice[ALARM_KEY], m_strEvent[ALARM_KEY], strAlarmCode, nStatus, m_nKeyCount[ALARM_KEY], m_strKey[ALARM_KEY], m_strValue[ALARM_KEY]);
}
CString CLogFromat::GetMotorName(int nAxis)
{
	CString strRet;

	switch (nAxis)
	{
		case M_TRAY1_Z:
			strRet = _T("TRAY1_Z");
			break;
		case M_TRAY2_Z:
			strRet = _T("TRAY2_Z");
			break;
		case M_PRESS_Y:
			strRet = _T("PRESS_Y");
			break;
		case M_EPOXY_TRANSFER_X:
			strRet = _T("EPOXY_TRANSFER_X");
			break;
		case M_EPOXY_TRANSFER_Y:
			strRet = _T("EPOXY_TRANSFER_Y");
			break;
		case M_EPOXY_TRANSFER_Z:
			strRet = _T("EPOXY_TRANSFER_Z");
			break;
		case M_EPOXY_SCREW:
			strRet = _T("EPOXY_SCREW");
			break;
		case M_CARRIER_X:
			strRet = _T("CARRIER_X");
			break;
		// 2번째 보드(8축)	
		
		case M_LOADER_TRANSFER_Y:
			strRet = _T("LOADER_TRANSFER_Y");
			break;
		case M_LOADER_TRANSFER_Z:
			strRet = _T("LOADER_TRANSFER_Z");
			break;
		case M_HEATSINK_TRANSFER_X:
			strRet = _T("HEATSINK_TRANSFER_X");
			break;
		case M_HEATSINK_TRANSFER_Y:
			strRet = _T("HEATSINK_TRANSFER_Y");
			break;
		case M_HEATSINK_TRANSFER_Z:
			strRet = _T("HEATSINK_TRANSFER_Z");
			break;
		case M_UNLOADER_TRANSFER_X:
			strRet = _T("UNLOADER_TRANSFER_X");
			break;
		case M_UNLOADER_TRANSFER_Y:
			strRet = _T("UNLOADER_TRANSFER_Y");
			break;
		case M_UNLOADER_TRANSFER_Z:
			strRet = _T("UNLOADER_TRANSFER_Z");
			break;
		case M_DISPENSER_Y:
			strRet = _T("DISPENSER_Y");
			break;		
		case M_TRAY_REMOVE_X:
			strRet = _T("TRAY_REMOVE_X");
			break;
		case M_HEATSINK_INSPECT_Y:
			strRet = _T("HEATSINK_INSPECT_Y");
			break;
		case M_HEATSINK_INSPECT_Z:
			strRet = _T("HEATSINK_INSPECT_Z");
			break;
		case M_HEATSINK_PICKER_PITCH:
			strRet = _T("HEATSINK_PICKER_PITCH");
			break;
	}

	return strRet;
}

CString CLogFromat::GetDeviceID(int nAxis)
{
	CString strRet;

	switch (nAxis)
	{
		case M_TRAY1_Z:
			strRet = _T("TRAY1_Z");
			break;
		case M_TRAY2_Z:
			strRet = _T("TRAY2_Z");
			break;
		case M_PRESS_Y:
			strRet = _T("PRESS_Y");
			break;
		case M_EPOXY_TRANSFER_X:
			strRet = _T("EPOXY_TRANSFER_X");
			break;
		case M_EPOXY_TRANSFER_Y:
			strRet = _T("EPOXY_TRANSFER_Y");
			break;
		case M_EPOXY_TRANSFER_Z:
			strRet = _T("EPOXY_TRANSFER_Z");
			break;
		case M_EPOXY_SCREW:
			strRet = _T("EPOXY_SCREW");
			break;
		case M_CARRIER_X:
			strRet = _T("CARRIER_X");
			break;
		// 2번째 보드(8축)	
		
		case M_LOADER_TRANSFER_Y:
			strRet = _T("LOADER_TRANSFER_Y");
			break;
		case M_LOADER_TRANSFER_Z:
			strRet = _T("LOADER_TRANSFER_Z");
			break;
		case M_HEATSINK_TRANSFER_X:
			strRet = _T("HEATSINK_TRANSFER_X");
			break;
		case M_HEATSINK_TRANSFER_Y:
			strRet = _T("HEATSINK_TRANSFER_Y");
			break;
		case M_HEATSINK_TRANSFER_Z:
			strRet = _T("HEATSINK_TRANSFER_Z");
			break;
		case M_UNLOADER_TRANSFER_X:
			strRet = _T("UNLOADER_TRANSFER_X");
			break;
		case M_UNLOADER_TRANSFER_Y:
			strRet = _T("UNLOADER_TRANSFER_Y");
			break;
		case M_UNLOADER_TRANSFER_Z:
			strRet = _T("UNLOADER_TRANSFER_Z");
			break;
		case M_DISPENSER_Y:
			strRet = _T("DISPENSER_Y");
			break;		
		case M_TRAY_REMOVE_X:
			strRet = _T("TRAY_REMOVE_X");
			break;
		case M_HEATSINK_INSPECT_Y:
			strRet = _T("HEATSINK_INSPECT_Y");
			break;
		case M_HEATSINK_INSPECT_Z:
			strRet = _T("HEATSINK_INSPECT_Z");
			break;
		case M_HEATSINK_PICKER_PITCH:
			strRet = _T("HEATSINK_PICKER_PITCH");
			break;
	}

	return strRet;
}

CString CLogFromat::GetSiteName(int nSite)
{
	CString strRet;

	switch (nSite)
	{
	case M_TRAY1_Z:
		strRet = _T("TRAY1_Z");
		break;
	case M_TRAY2_Z:
		strRet = _T("TRAY2_Z");
		break;
	case M_PRESS_Y:
		strRet = _T("PRESS_Y");
		break;
	case M_EPOXY_TRANSFER_X:
		strRet = _T("EPOXY_TRANSFER_X");
		break;
	case M_EPOXY_TRANSFER_Y:
		strRet = _T("EPOXY_TRANSFER_Y");
		break;
	case M_EPOXY_TRANSFER_Z:
		strRet = _T("EPOXY_TRANSFER_Z");
		break;
	case M_EPOXY_SCREW:
		strRet = _T("EPOXY_SCREW");
		break;
	case M_CARRIER_X:
		strRet = _T("CARRIER_X");
		break;
		// 2번째 보드(8축)	
		
	case M_LOADER_TRANSFER_Y:
		strRet = _T("LOADER_TRANSFER_Y");
		break;
	case M_LOADER_TRANSFER_Z:
		strRet = _T("LOADER_TRANSFER_Z");
		break;
	case M_HEATSINK_TRANSFER_X:
		strRet = _T("HEATSINK_TRANSFER_X");
		break;
	case M_HEATSINK_TRANSFER_Y:
		strRet = _T("HEATSINK_TRANSFER_Y");
		break;
	case M_HEATSINK_TRANSFER_Z:
		strRet = _T("HEATSINK_TRANSFER_Z");
		break;
	case M_UNLOADER_TRANSFER_X:
		strRet = _T("UNLOADER_TRANSFER_X");
		break;
	case M_UNLOADER_TRANSFER_Y:
		strRet = _T("UNLOADER_TRANSFER_Y");
		break;
	case M_UNLOADER_TRANSFER_Z:
		strRet = _T("UNLOADER_TRANSFER_Z");
		break;
	case M_DISPENSER_Y:
		strRet = _T("DISPENSER_Y");
		break;		
	case M_TRAY_REMOVE_X:
		strRet = _T("TRAY_REMOVE_X");
		break;
	case M_HEATSINK_INSPECT_Y:
		strRet = _T("HEATSINK_INSPECT_Y");
		break;
	case M_HEATSINK_INSPECT_Z:
		strRet = _T("HEATSINK_INSPECT_Z");
		break;
	case M_HEATSINK_PICKER_PITCH:
		strRet = _T("HEATSINK_PICKER_PITCH");
			break;
	}
		return strRet;
}

CString CLogFromat::GetRunStatus()
{
	CString strRet;

	switch (st_work.mn_run_status)
	{
		case dSTOP:
			strRet = _T("STOP");
			break;

		case dRUN:
			strRet = _T("RUN");
			break;

		case dJAM:
			strRet = _T("JAM");
			break;

		case dLOTEND:
			strRet = _T("LOTEND");
			break;

		case dINIT:
			strRet = _T("INIT");
			break;

		case dWARNING:
			strRet = _T("WARNING");
			break;

		case dLOCK:
			strRet = _T("LOCK");
			break;

		case dSELFCHECK:
			strRet = _T("SELFCHECK");
			break;

		case dMAINT:
			strRet = _T("MAINT");
			break;

		case dREINSTATE:
			strRet = _T("REINSTATE");
			break;

		case dRECOVERY:
			strRet = _T("RECOVERY");
			break;
	}
	return strRet;
}

void CLogFromat::OnLogMotorMove(int nAxis, int nStatus, double dPos)
{
	//CString strLogKey[10];
	//CString strLogData[10];
	CString strAxis;
	CString strStatus[2] = {_T("END"), _T("START")};
	CString strPos;
	CString strDvcID;
	CString strRunStatus;
	CString strMaterialType = _T("");
	CString strMaterialId	= _T("");

//	int i, j;
	strDvcID	= GetDeviceID(nAxis);
	strAxis		= GetMotorName(nAxis);
	strPos.Format(_T("%0.3f"), dPos);

	m_strLogKeyM[nAxis][0]	= _T("START");
	m_strLogKeyM[nAxis][1]	= _T("END");
	m_strLogKeyM[nAxis][2]	= _T("MOTION");
	m_strLogDataM[nAxis][0].Format(_T("%.3f"), COMI.Get_MotCurrentPos(nAxis));
	m_strLogDataM[nAxis][1].Format(_T("%.3f"), dPos);
	m_strLogDataM[nAxis][2]	= strStatus[nStatus];

	if (nStatus == 1)
	{
		if (m_nMoveFlag[nAxis] == YES)
		{
			LogFunction(strDvcID, _T("MOTOR MOVE") , 1, strMaterialId, strMaterialType, 2, m_strLogKeyM[nAxis], m_strLogDataM[nAxis]);
		}
		else
		{
			m_nMoveFlag[nAxis] = YES;
		}
	}
	else
	{
		m_nMoveFlag[nAxis] = NO;
	}

	switch(nAxis)
	{
		case M_HEATSINK_PICKER_PITCH:
			break;
	
	}
	//LogFunction(strDvcID, _T("MOTOR MOVE") , nStatus, st_recipe.strCokTypeName[st_recipe_info.nHifix - 1], _T("SSD"), 3, strLogKey, strLogData);
	if (nStatus == 1) LogFunction(strDvcID, _T("MOTOR MOVE") , 0, strMaterialId, strMaterialType, 2, m_strLogKeyM[nAxis], m_strLogDataM[nAxis]);
	else LogFunction(strDvcID, _T("MOTOR MOVE") , 1, strMaterialId, strMaterialType, 2, m_strLogKeyM[nAxis], m_strLogDataM[nAxis]);
}

void CLogFromat::OnTransferLog(int nSite, CString strEvent, CString strMatID, CString strMatType, CString strStart, CString strEnd)
{
//	int i;
	CString strDvcID;
	CString strLogKey[10];
	CString strLogData[10];

	strDvcID = GetSiteName(nSite);

	//LogTransfer(strDvcID, strEvent, strMatID, strMatType, strStart, strEnd, 0, strLogKey, strLogData);
}

//LogAlarm(CString strID, CString strEvent, CString strCode, int nStatus, int nDataCount, CString *strKey, CString *strData);
void CLogFromat::LogAlarm(CString strID, CString strEvent, CString strCode, int nStatus, int nDataCount, CString *strKey, CString *strData)
{
	CFileException e;
	CStdioFile file;

	SYSTEMTIME Time;	

	int i;
	int nAdd;

	CString strTime[3];
	CString strLog = _T("");
	CString strTemp;

	GetLocalTime(&Time);

	strTime[1].Format(_T("%04d/%02d/%02d"), Time.wYear, 
											Time.wMonth, 
											Time.wDay);
	strTime[0] += strTime[1];
	strTime[0] += _T("\t");

	strTime[2].Format(_T("%02d:%02d:%02d.%03d"), Time.wHour, 
												 Time.wMinute, 
												 Time.wSecond,
												 Time.wMilliseconds);
	strTime[0] += strTime[2];
	strTime[0] += _T("\t");

	strTemp.Format(_T("'%s'\t"), strID);
	strLog += strTemp;

	strTemp.Format(_T("'%s'\t"), _T("ALM"));
	strLog += strTemp;

	strTemp.Format(_T("'%s'\t"), strEvent);
	strLog += strTemp;

	strTemp.Format(_T("'%s'\t"), strCode);
	strLog += strTemp;

	if (nStatus == 0)
	{
		strTemp.Format(_T("'%s'\t"), _T("OCCURRED"));
		strLog += strTemp;
	}
	else
	{
		strTemp.Format(_T("'%s'\t"), _T("RELEASED"));
		strLog += strTemp;
	}

	nAdd = 0;
	for (i=0; i<nDataCount; i++)
	{
		nAdd++;
		if (nAdd == nDataCount)
		{
			strTemp.Format(_T("('%s', '%s')"), strKey[i], strData[i]);
		}
		else
		{
			strTemp.Format(_T("('%s', '%s')\t"), strKey[i], strData[i]);
		}
		strLog += strTemp;
	}

	if (m_strOldAlarm == strLog)
	{
		//2011.0504 추가 같은 메세지가 나오는 문제 처리 
		m_dwAlarmWaitTime[1] = GetCurrentTime();
		m_dwAlarmWaitTime[2] = m_dwAlarmWaitTime[1] - m_dwAlarmWaitTime[0];
		
		if(m_dwAlarmWaitTime[2] < 0)
		{
			m_dwAlarmWaitTime[0] = GetTickCount();
			return;
		}
		
		if (m_dwFuncWaitTime[2] < 60000) 
		{
			//OK 파일로 내용 저장 
			return;
		}
	}

	m_dwAlarmWaitTime[0]	= GetCurrentTime();
	m_strOldAlarm			= strLog;

	strTime[0]				+= strLog;
	OnLogWrite(strTime[0]);
}

//LogFunction(CString strID, CString strEvent, CString strMatID, CString strMatType, int nDataCount, CString *strKey, CString *strData);
void CLogFromat::LogFunction(CString strID, CString strEvent, int nStatus, CString strMatID, CString strMatType, int nDataCount, CString *strKey, CString *strData)
{
	CFileException e;
	CStdioFile file;

	SYSTEMTIME Time;	

	int i;
	int nAdd;

	CString strTime[3];
	CString strLog = _T("");
	CString strTemp;
	CString strVal[2];

	GetLocalTime(&Time);

	strTime[1].Format(_T("%04d/%02d/%02d"), Time.wYear, 
											Time.wMonth, 
											Time.wDay);
	strTime[0] += strTime[1];
	strTime[0] += _T("\t");

	strTime[2].Format(_T("%02d:%02d:%02d.%03d"), Time.wHour, 
												 Time.wMinute, 
												 Time.wSecond,
												 Time.wMilliseconds);
	strTime[0] += strTime[2];
	strTime[0] += _T("\t");

	strVal[0] = strID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strTemp.Format(_T("'%s'\t"), _T("FNC"));
	strLog += strTemp;

	strVal[0] = strEvent;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	if (nStatus == 0)
	{
		strTemp.Format(_T("'%s'\t"), _T("START"));
		strLog += strTemp;
	}
	else
	{
		strTemp.Format(_T("'%s'\t"), _T("END"));
		strLog += strTemp;
	}

	strVal[0] = strMatID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strVal[0] = strMatType;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strKey[nDataCount]		= _T("LOTID");
	strData[nDataCount]		= g_lotMgr.GetLotAt(0).GetLotID();
	strKey[nDataCount + 1]	= _T("RECIPE");
	strData[nDataCount + 1]	= st_basic.mstr_device_name;// _info.strDeviceName;

	nAdd = 0;
	for (i=0; i<nDataCount + 2; i++)
	{
		nAdd++;
		if (nAdd == (nDataCount + 2))
		{
			strVal[0] = strKey[i];
			if (strVal[0] == _T("")) strVal[0] = _T("$");
			strVal[1] = strData[i];
			if (strVal[1] == _T("")) strVal[1] = _T("$");

			strTemp.Format(_T("('%s', '%s')"), strVal[0], strVal[1]);
		}
		else
		{
			strVal[0] = strKey[i];
			if (strVal[0] == _T("")) strVal[0] = _T("$");
			strVal[1] = strData[i];
			if (strVal[1] == _T("")) strVal[1] = _T("$");

			strTemp.Format(_T("('%s', '%s')\t"), strVal[0], strVal[1]);
		}
		strLog += strTemp;
	}

	if (m_strOldFunc == strLog)
	{
		//2011.0504 추가 같은 메세지가 나오는 문제 처리 
		m_dwFuncWaitTime[1] = GetCurrentTime();
		m_dwFuncWaitTime[2] = m_dwFuncWaitTime[1] - m_dwFuncWaitTime[0];
		
		if(m_dwFuncWaitTime[2] < 0)
		{
			m_dwFuncWaitTime[0] = GetTickCount();
			return;
		}

		if (m_dwFuncWaitTime[2] < 60000) 
		{
			//OK 파일로 내용 저장 
			return;
		}
	}

	m_dwFuncWaitTime[0]	= GetCurrentTime();
	m_strOldFunc			= strLog;

	strTime[0]				+= strLog;
	OnLogWrite(strTime[0]);
}

//LogProcess(CString strID, CString strEvent, int nStatus, CString strMatID, CString strLotID, CString strRecipe, int nDataCount, CString *strKey, CString *strData);
void CLogFromat::LogProcess(CString strID, CString strEvent, int nStatus, CString strMatID, CString strLotID, CString strRecipe, int nDataCount, CString *strKey, CString *strData)
{
	CFileException e;
	CStdioFile file;

	SYSTEMTIME Time;	

	int i;
	int nAdd;

	CString strTime[3];
	CString strLog = _T("");
	CString strTemp;
	CString strVal[3];

	GetLocalTime(&Time);

	strTime[1].Format(_T("%04d/%02d/%02d"), Time.wYear, 
											Time.wMonth, 
											Time.wDay);
	strTime[0] += strTime[1];
	strTime[0] += _T("\t");

	strTime[2].Format(_T("%02d:%02d:%02d.%03d"), Time.wHour, 
												 Time.wMinute, 
												 Time.wSecond,
												 Time.wMilliseconds);
	strTime[0] += strTime[2];
	strTime[0] += _T("\t");

	strVal[0] = strID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strTemp.Format(_T("'%s'\t"), _T("PRC"));
	strLog += strTemp;

	strVal[0] = strEvent;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	if (nStatus == 0)
	{
		strTemp.Format(_T("'%s'\t"), _T("START"));
		strLog += strTemp;
	}
	else
	{
		strTemp.Format(_T("'%s'\t"), _T("END"));
		strLog += strTemp;
	}

	strVal[0] = strMatID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strVal[0] = strLotID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strVal[0] = strRecipe;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	nAdd = 0;
	for (i=0; i<nDataCount; i++)
	{
		nAdd++;
		if (nAdd == nDataCount)
		{
			strVal[0] = strKey[i];
			if (strVal[0] == _T("")) strVal[0] = _T("$");
			strVal[1] = strData[i];
			if (strVal[1] == _T("")) strData[1] = _T("$");

			strTemp.Format(_T("('%s', '%s')"), strVal[0], strVal[1]);
		}
		else
		{
			strVal[0] = strKey[i];
			if (strVal[0] == _T("")) strVal[0] = _T("$");
			strVal[1] = strData[i];
			if (strVal[1] == _T("")) strData[1] = _T("$");

			strTemp.Format(_T("('%s', '%s')\t"), strVal[0], strVal[1]);
		}
		strLog += strTemp;
	}

	if (m_strOldProcess == strLog)
	{
		//2011.0504 추가 같은 메세지가 나오는 문제 처리 
		m_dwProcessWaitTime[1] = GetCurrentTime();
		m_dwProcessWaitTime[2] = m_dwProcessWaitTime[1] - m_dwProcessWaitTime[0];
		
		if(m_dwProcessWaitTime[2] < 0)
		{
			m_dwProcessWaitTime[0] = GetTickCount();
			return;
		}
		
		if (m_dwFuncWaitTime[2] < 60000) 
		{
			//OK 파일로 내용 저장 
			return;
		}
	}

	m_dwProcessWaitTime[0]	= GetCurrentTime();
	m_strOldProcess			= strLog;

	strTime[0]				+= strLog;
	OnLogWrite(strTime[0]);
}

//LogTransfer(CString strID, CString strEvent, CString strMatID, CString strMatType, CString strStartPos, CString strEndPos, int nDataCount, CString *strKey, CString *strData);
void CLogFromat::LogTransfer(CString strID, CString strEvent, int nStatus, CString strMatID, CString strMatType, CString strStartPos, CString strEndPos, int nDataCount, CString *strKey, CString *strData)
{
	CFileException e;
	CStdioFile file;

	SYSTEMTIME Time;	

	int i;
	int nAdd;

	CString strTime[3];
	CString strFile;
	CString strLog = _T("");
	CString strTemp;
	CString strVal[2];

	GetLocalTime(&Time);

	strTime[1].Format(_T("%04d/%02d/%02d"), Time.wYear, 
											Time.wMonth, 
											Time.wDay);
	strTime[0] += strTime[1];
	strTime[0] += _T("\t");

	strTime[2].Format(_T("%02d:%02d:%02d.%03d"), Time.wHour, 
												 Time.wMinute, 
												 Time.wSecond,
												 Time.wMilliseconds);
	strTime[0] += strTime[2];
	strTime[0] += _T("\t");

	strVal[0] = strID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strTemp.Format(_T("'%s'\t"), _T("XFR"));
	strLog += strTemp;

	strVal[0] = strEvent;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	if (nStatus == 0)
	{
		strTemp.Format(_T("'%s'\t"), _T("START"));
		strLog += strTemp;
	}
	else
	{
		strTemp.Format(_T("'%s'\t"), _T("END"));
		strLog += strTemp;
	}

	strVal[0] = strMatID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strVal[0] = strMatType;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strVal[0] = strStartPos;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strVal[0] = strEndPos;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	nAdd = 0;
	for (i=0; i<nDataCount; i++)
	{
		nAdd++;
		if (nAdd == nDataCount)
		{
			strVal[0] = strKey[i];
			if (strVal[0] == _T("")) strVal[0] = _T("$");
			strVal[1] = strData[i];
			if (strVal[1] == _T("")) strData[1] = _T("$");

			strTemp.Format(_T("('%s', '%s')"), strVal[0], strVal[1]);
		}
		else
		{
			strVal[0] = strKey[i];
			if (strVal[0] == _T("")) strVal[0] = _T("$");
			strVal[1] = strData[i];
			if (strVal[1] == _T("")) strData[1] = _T("$");

			strTemp.Format(_T("('%s', '%s')\t"), strVal[0], strVal[1]);
		}
		strLog += strTemp;
	}

	if (m_strOldTrans == strLog)
	{
		//2011.0504 추가 같은 메세지가 나오는 문제 처리 
		m_dwTransWaitTime[1] = GetCurrentTime();
		m_dwTransWaitTime[2] = m_dwTransWaitTime[1] - m_dwTransWaitTime[0];
		
		if(m_dwTransWaitTime[2] < 0)
		{
			m_dwTransWaitTime[0] = GetTickCount();
			return;
		}
		
		if (m_dwFuncWaitTime[2] < 60000) 
		{
			//OK 파일로 내용 저장 
			return;
		}
	}

	m_dwTransWaitTime[0]	= GetCurrentTime();
	m_strOldTrans			= strLog;

	strTime[0]				+= strLog;
	OnLogWrite(strTime[0]);
}

//LogLotInform(CString strID, CString strEvent, CString strLotID, CString strRecipe, CString strCarrierID, int nDataCount, CString *strKey, CString *strData);
void CLogFromat::LogLotInform(CString strID, CString strEvent, CString strLotID, CString strRecipe, CString strCarrierID, int nDataCount, CString *strKey, CString *strData)
{
	CFileException e;
	CStdioFile file;

	SYSTEMTIME Time;	

	int i;
	int nAdd;

	CString strTime[2];
	CString strLog = _T("");
	CString strTemp;
	CString strVal[2];

	GetLocalTime(&Time);

	strTime[0].Format(_T("%04d/%02d/%02d"), Time.wYear, 
											Time.wMonth, 
											Time.wDay);
	strLog += strTime[0];
	strLog += _T("\t");

	strTime[1].Format(_T("%02d:%02d:%02d.%03d"), Time.wHour, 
												 Time.wMinute, 
												 Time.wSecond,
												 Time.wMilliseconds);
	strLog += strTime[1];
	strLog += _T("\t");

	strVal[0] = strID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strTemp.Format(_T("'%s'\t"), _T("LEH"));
	strLog += strTemp;

	strVal[0] = strEvent;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strVal[0] = strLotID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strVal[0] = strRecipe;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strVal[0] = strCarrierID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	nAdd = 0;
	for (i=0; i<nDataCount; i++)
	{
		nAdd++;
		if (nAdd == nDataCount)
		{
			strVal[0] = strKey[i];
			if (strVal[0] == _T("")) strVal[0] = _T("$");
			strVal[1] = strData[i];
			if (strVal[1] == _T("")) strData[1] = _T("$");

			strTemp.Format(_T("('%s', '%s')"), strVal[0], strVal[1]);
		}
		else
		{
			strVal[0] = strKey[i];
			if (strVal[0] == _T("")) strVal[0] = _T("$");
			strVal[1] = strData[i];
			if (strVal[1] == _T("")) strData[1] = _T("$");

			strTemp.Format(_T("('%s', '%s')\t"), strVal[0], strVal[1]);
		}
		strLog += strTemp;
	}

	OnLogWrite(strLog);
}

//LogConfig(CString strID, CString strEvent, CString strRecipe, int nDataCount, CString *strKey, CString *strData);
void CLogFromat::LogConfig(CString strID, CString strEvent, CString strRecipe, int nDataCount, CString *strKey, CString *strData)
{
	CFileException e;
	CStdioFile file;

	SYSTEMTIME Time;	

	int i;
	int nAdd;

	CString strTime[2];
	CString strLog = _T("");
	CString strTemp;
	CString strVal[2];

	GetLocalTime(&Time);

	strTime[0].Format(_T("%04d/%02d/%02d"), Time.wYear, 
											Time.wMonth, 
											Time.wDay);
	strLog += strTime[0];
	strLog += _T("\t");

	strTime[1].Format(_T("%02d:%02d:%02d.%03d"), Time.wHour, 
												 Time.wMinute, 
												 Time.wSecond,
												 Time.wMilliseconds);
	strLog += strTime[1];
	strLog += _T("\t");

	strVal[0] = strID;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	strTemp.Format(_T("'%s'\t"), _T("CFG"));
	strLog += strTemp;

	strVal[0] = strRecipe;
	if (strVal[0] == _T("")) strVal[0] = _T("$");
	strTemp.Format(_T("'%s'\t"), strVal[0]);
	strLog += strTemp;

	nAdd = 0;
	for (i=0; i<nDataCount; i++)
	{
		nAdd++;
		if (nAdd == nDataCount)
		{
			strVal[0] = strKey[i];
			if (strVal[0] == _T("")) strVal[0] = _T("$");
			strVal[1] = strData[i];
			if (strVal[1] == _T("")) strData[1] = _T("$");

			strTemp.Format(_T("('%s', '%s')"), strVal[0], strVal[1]);
		}
		else
		{
			strVal[0] = strKey[i];
			if (strVal[0] == _T("")) strVal[0] = _T("$");
			strVal[1] = strData[i];
			if (strVal[1] == _T("")) strData[1] = _T("$");

			strTemp.Format(_T("('%s', '%s')\t"), strVal[0], strVal[1]);
		}
		strLog += strTemp;
	}

	OnLogWrite(strLog);
}
