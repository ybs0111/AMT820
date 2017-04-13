// AMTRegistry.cpp: implementation of the AMTRegistry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "AMTRegistry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AMTRegistry::AMTRegistry()
{

}

AMTRegistry::~AMTRegistry()
{

}

UINT AMTRegistry::RegReadInt( LPCTSTR lpKey, LPCTSTR lpValue, UINT nDefault )
{
	HKEY hkey;
	DWORD dwDisp;
	UINT Result;
	DWORD dwSize;
	
	if(RegCreateKeyEx(HKEY_CURRENT_USER, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &hkey, &dwDisp) != ERROR_SUCCESS )
		return 0;
	
	dwSize = sizeof( LONG );
	if(RegQueryValueEx( hkey, lpValue, 0, NULL, (LPBYTE)&Result, &dwSize) != ERROR_SUCCESS )
		Result = nDefault;
	
	RegCloseKey( hkey );
	return Result;
}

BOOL AMTRegistry::RegWriteInt( LPCTSTR lpKey, LPCTSTR lpValue, UINT nData )
{
	HKEY hkey;
	DWORD dwDisp;
	if( RegCreateKeyEx( HKEY_CURRENT_USER, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL,	&hkey, &dwDisp ) != ERROR_SUCCESS )
		return FALSE;
	
	if( RegSetValueEx( hkey, lpValue, 0, REG_DWORD, (LPBYTE)&nData, (int)sizeof(UINT) ) != ERROR_SUCCESS )
		return FALSE;
	
	RegCloseKey( hkey );
	return TRUE;
}


std::string AMTRegistry::RegReadString( LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault )
{
	
	HKEY key;
	DWORD dwDisp;
	DWORD Size;
	CHAR lpRet[256];
	
	if( RegCreateKeyEx( HKEY_CURRENT_USER, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &key, &dwDisp ) != ERROR_SUCCESS )
		return "";
	
	Size = 128;
	
	if( RegQueryValueEx( key, lpValue, 0, NULL, (LPBYTE)lpRet, &Size ) != ERROR_SUCCESS )
	{
		return "";
	}
	
	RegCloseKey( key );
	return lpRet;
}

BOOL AMTRegistry::RegWriteString( LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData )
{
	HKEY key;
	DWORD dwDisp;
	
	if( RegCreateKeyEx( HKEY_CURRENT_USER, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, &dwDisp ) != ERROR_SUCCESS )
		return FALSE;
	
	if( RegSetValueEx( key, lpValue, 0, REG_SZ, (LPBYTE)lpData, (int)strlen( lpData ) + 1 ) != ERROR_SUCCESS )
		return FALSE;
	
	RegCloseKey( key );
	return TRUE;
}

void AMTRegistry::RegWriteTime( SYSTEMTIME t )
{
	CString strValue;
	strValue.Format("%04d%02d%02d", t.wYear, t.wMonth, t.wDay );
	AMTRegistry::RegWriteString( REG_KEY_TIME, REG_VAL_TIME, (LPCSTR)strValue );
}

SYSTEMTIME AMTRegistry::RegReadTime()
{
	CString strTime = RegReadString( REG_KEY_TIME, REG_VAL_TIME, "" ).c_str();

	SYSTEMTIME rtn;
	rtn.wYear = 0;
	rtn.wMonth = 1;
	rtn.wDay = 1;
	rtn.wHour = 0;
	rtn.wMinute = 0;
	rtn.wSecond = 0;
	rtn.wMilliseconds = 0;

	if( strTime.GetLength() < 6 )
		return rtn;

	rtn.wYear = atoi( strTime.Left(4) );
	rtn.wMonth = atoi( strTime.Mid(4, 2) );
	rtn.wDay = atoi( strTime.Mid(6, 2) );

	return rtn;
}

BOOL RegRemoveValue(LPCTSTR lpKey, LPCTSTR lpValue)
{
    HKEY key;
	
	if( RegOpenKeyEx( HKEY_CURRENT_USER, lpKey, 0, KEY_ALL_ACCESS,
		&key) != ERROR_SUCCESS )
		return FALSE;
	
	if(RegDeleteValue(key, lpValue) != ERROR_SUCCESS)
		return FALSE;
	
	RegCloseKey( key );
    return TRUE;
}
