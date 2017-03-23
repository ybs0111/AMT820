// AMTRegistry.h: interface for the AMTRegistry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMTREGISTRY_H__A6E1C5C0_C1BD_43A0_9D28_67FE1ECAC207__INCLUDED_)
#define AFX_AMTREGISTRY_H__A6E1C5C0_C1BD_43A0_9D28_67FE1ECAC207__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#define REG_KEY_TIME				"Software\\AMT\\AMT8520\\TIME\\"
#define REG_VAL_TIME				"DAILY"

#define REG_KEY_COK					"Software\\AMT\\AMT8520\\COK\\"
#define REG_VAL_ROBOT_COK			"ROBOT_COK"

#define REG_VAL_ITEM_CNT			"ITEM_CNT"
#define REG_VAL_SITE_COK			"SITE"
#define REG_VAL_HIFIX_POS			"HIFIX"

#define REG_KEY_COUNT				"Software\\AMT\\AMT8520\\COUNT\\"
#define REG_VAL_COUNT_INPUT_PRIME	"INPUT_PRIME"
#define REG_VAL_COUNT_PASS_PRIME	"PASS_PRIME"
#define REG_VAL_COUNT_FAIL_PRIME	"FAIL_PRIME"
#define REG_VAL_COUNT_INPUT_CUM		"INPUT_CUM"
#define REG_VAL_COUNT_PASS_CUM		"PASS_CUM"
#define REG_VAL_COUNT_FAIL_CUM		"FAIL_CUM"
#define REG_VAL_COUNT_BIN			"BIN"

#define REG_VAL_COUNT_TS			"TS"

#define REG_VAL_COUNT_ALARM			"ALARM"


#define REG_KEY_BIN					"Software\\AMT\\AMT8520\\BIN\\"
#define REG_VAL_TS_BIN_HISTORY		"TS_BIN_HISTORY"

#define REG_KEY_TEACH				"Software\\AMT\\AMT8520\\TEACH\\"

#define REG_KEY_USE_SOCKET			"Software\\AMT\\AMT8520\\USE_SOCKET\\"
#define REG_KEY_SOCKET_CAUSE		"Software\\AMT\\AMT8520\\SOCKET_CAUSE\\"
#define REG_VAL_SOCKET_TS			"CSOCKET"

#define REG_KEY_RECIPE				"Software\\AMT\\AMT8520\\RECIPE\\"
#define REG_VAL_ID					"ID"


class AMTRegistry  
{
public:
	AMTRegistry();
	virtual ~AMTRegistry();

	static UINT RegReadInt( LPCTSTR lpKey, LPCTSTR lpValue, UINT nDefault = 0 );	
	static BOOL RegWriteInt( LPCTSTR lpKey, LPCTSTR lpValue, UINT nData );
	static std::string RegReadString( LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault );
	static BOOL RegWriteString( LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData );
	static BOOL RegRemoveValue(LPCTSTR lpKey, LPCTSTR lpValue);

	static void RegWriteTime( SYSTEMTIME t );
	static SYSTEMTIME RegReadTime();
};

#endif // !defined(AFX_AMTREGISTRY_H__A6E1C5C0_C1BD_43A0_9D28_67FE1ECAC207__INCLUDED_)
