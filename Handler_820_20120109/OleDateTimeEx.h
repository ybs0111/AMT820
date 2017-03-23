// COleDateTimeEx
// Author:	Matt Gullett
//			gullettm@yahoo.com
// Copyright (c) 2001
//
// This is a utility class used to enhance the date/time parsing abilities of
// the MFC provided class COleDateTime
//
// You may freely use this source code in personal, freeware, shareware
// or commercial applications provided that 1) my name is recognized in the
// code and if this code represents a substantial portion of the application
// that my name be included in the credits for the application (about box, etc)
//
// Use this code at your own risk.  This code is provided AS-IS.  No warranty
// is granted as to the correctness, usefulness or value of this code.
//
// Special thanks to Oskar Wieland for his Easter calculation formula posted
// on CodeProject.
// http://www.codeproject.com/datetime/easter.asp
//
//
// *******************************************************************
// TECHNICAL NOTES:
// 
// See .cpp file for tech notes
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_OLEDATETIMEEX_H__A5F3B26D_ACFB_4838_958B_DF0202518BAD__INCLUDED_)
#define AFX_OLEDATETIMEEX_H__A5F3B26D_ACFB_4838_958B_DF0202518BAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// defines for the hour cutoff options
#define OLEDATEEX_HOUR_DETECT_DEFAULT				0
#define OLEDATEEX_HOUR_DETECT_CURRENT_HOUR			1
#define OLEDATEEX_HOUR_DETECT_USE_CUTOFF			2
#define OLEDATEEX_HOUR_DETECT_LEAVE					3
#define OLEDATEEX_DEFAULT_HOUR_CUTOFF				7

// used for matching type of day methods (in recurrence patterns)
#define OLEDATEEX_DAY								0
#define OLEDATEEX_WEEK_DAY							1
#define OLEDATEEX_WEEKEND_DAY						2
#define OLEDATEEX_SUNDAY							3
#define OLEDATEEX_MONDAY							4
#define OLEDATEEX_TUESDAY							5
#define OLEDATEEX_WEDNESDAY							6	
#define OLEDATEEX_THURSDAY							7
#define OLEDATEEX_FRIDAY							8
#define OLEDATEEX_SATURDAY							9

// month defines
#define OLEDATEEX_MONTH_JANUARY						1
#define OLEDATEEX_MONTH_FEBRUARY					2
#define OLEDATEEX_MONTH_MARCH						3
#define OLEDATEEX_MONTH_APRIL						4
#define OLEDATEEX_MONTH_MAY							5
#define OLEDATEEX_MONTH_JUNE						6
#define OLEDATEEX_MONTH_JULY						7
#define OLEDATEEX_MONTH_AUGUST						8
#define OLEDATEEX_MONTH_SEPTEMBER					9
#define OLEDATEEX_MONTH_OCTOBER						10
#define OLEDATEEX_MONTH_NOVEMBER					11
#define OLEDATEEX_MONTH_DECEMBER					12

// day of week defines
#define OLEDATEEX_DAYOFWEEK_SUNDAY					1
#define OLEDATEEX_DAYOFWEEK_MONDAY					2
#define OLEDATEEX_DAYOFWEEK_TUESDAY					3
#define OLEDATEEX_DAYOFWEEK_WEDNESDAY				4
#define OLEDATEEX_DAYOFWEEK_THURSDAY				5
#define OLEDATEEX_DAYOFWEEK_FRIDAY					6
#define OLEDATEEX_DAYOFWEEK_SATURDAY				7

class COleDateTimeEx : public COleDateTime  
{
public:
	COleDateTimeEx();
	COleDateTimeEx(COleDateTimeEx& CopyFrom);
	COleDateTimeEx(COleDateTime& CopyFrom);
	virtual ~COleDateTimeEx();

	virtual void operator=(COleDateTimeEx& CopyFrom);
	virtual void operator=(COleDateTime& CopyFrom);
	virtual BOOL IsDayType(int iType);

	virtual BOOL ParseDateTime( LPCTSTR lpszDate, DWORD dwFlags = 0, LCID lcid = LANG_USER_DEFAULT );

	static void ClearShortcuts() {m_mapShortcuts.RemoveAll();}
	static void AddShortcut(LPCTSTR lpszShortcut, LPCTSTR lpszExpanded) {m_mapShortcuts.SetAt(lpszShortcut, lpszExpanded);}
	static void LoadDefaultShortcuts();

	virtual int GetHourThreshold();
	virtual void SetHourThreshold(int lHourCutoff);
	virtual long GetHourOption();
	virtual void SetHourOption(long lHourOptions);

	// exposed support methods
	virtual BOOL IsWeekendDay() {return (IsSaturday() || IsSunday());}
	virtual BOOL IsWeekDay() {return !IsWeekendDay();}

	virtual BOOL IsSunday() {return (GetDayOfWeek() == OLEDATEEX_DAYOFWEEK_SUNDAY);}
	virtual BOOL IsMonday() {return (GetDayOfWeek() == OLEDATEEX_DAYOFWEEK_MONDAY);}
	virtual BOOL IsTuesday() {return (GetDayOfWeek() == OLEDATEEX_DAYOFWEEK_TUESDAY);}
	virtual BOOL IsWednesday() {return (GetDayOfWeek() == OLEDATEEX_DAYOFWEEK_WEDNESDAY);}
	virtual BOOL IsThursday() {return (GetDayOfWeek() == OLEDATEEX_DAYOFWEEK_THURSDAY);}
	virtual BOOL IsFriday() {return (GetDayOfWeek() == OLEDATEEX_DAYOFWEEK_FRIDAY);}
	virtual BOOL IsSaturday() {return (GetDayOfWeek() == OLEDATEEX_DAYOFWEEK_SATURDAY);}

	virtual BOOL IsJanuary() {return GetMonth() == OLEDATEEX_MONTH_JANUARY;}
	virtual BOOL IsFebruary() {return GetMonth() == OLEDATEEX_MONTH_FEBRUARY;}
	virtual BOOL IsMarch() {return GetMonth() == OLEDATEEX_MONTH_MARCH;}
	virtual BOOL IsApril() {return GetMonth() == OLEDATEEX_MONTH_APRIL;}
	virtual BOOL IsMay() {return GetMonth() == OLEDATEEX_MONTH_MAY;}
	virtual BOOL IsJune() {return GetMonth() == OLEDATEEX_MONTH_JUNE;}
	virtual BOOL IsJuly() {return GetMonth() == OLEDATEEX_MONTH_JULY;}
	virtual BOOL IsAugust() {return GetMonth() == OLEDATEEX_MONTH_AUGUST;}
	virtual BOOL IsSeptember() {return GetMonth() == OLEDATEEX_MONTH_SEPTEMBER;}
	virtual BOOL IsOctober() {return GetMonth() == OLEDATEEX_MONTH_OCTOBER;}
	virtual BOOL IsNovember() {return GetMonth() == OLEDATEEX_MONTH_NOVEMBER;}
	virtual BOOL IsDecember() {return GetMonth() == OLEDATEEX_MONTH_DECEMBER;}

	virtual BOOL IsLeapYear(int iYear);
	virtual BOOL IsLeapYear() {return IsLeapYear(GetYear());}

	virtual BOOL IsValid() {return (GetStatus() == COleDateTime::valid);}

protected:
	CString DealWithAMPMIssues(CString& strInput);
	BOOL ComputeDateFromWord(COleDateTimeEx& dt, CString& strTest, int iYear);
	CString ExpandShortCut(LPCTSTR lpszInput);
	CString ExpandShortcuts(LPCTSTR lpszInput);
	CString RemoveExtraWhiteSpace(LPCTSTR lpszInput);
	BOOL IsOleDefaultDate(COleDateTime& dt);

	BOOL ComputeDayInMonth(int iCount, int iDayOfWeek, int iMonth, int iYear);
	int GetMaxDaysInMonth(int iMonth, int iYear);
	COleDateTime ComputeMonthsAgo(int iMonths, COleDateTime& dtStart);
	COleDateTime ComputeMonthsFrom(int iMonths, COleDateTime& dtStart);
	BOOL CalcXDaysOfWeekFromNow(int iDesiredCount, int iDayOfWeek, COleDateTime& dt);
	BOOL CalcXDaysOfWeekAgo(int iDesiredCount, int iDayOfWeek, COleDateTime& dt);
	BOOL IsFirstTagNumeric(CString& strInput);
	BOOL IsLastTagNumeric(CString& strInput);
	COleDateTime ComputeDayInMonth(int iCount, int iDayOfWeek, int iMonth);
	int GetMonthNumFromName(LPCTSTR lpszInput);
	void CalcEasterSunday( WORD wYear, WORD& wMonth, WORD& wDay );
	BOOL EvaluateInputText(LPCTSTR lpszInput, COleDateTime& dt);
	BOOL EvaluateInputTextDate(LPCTSTR lpszInput, COleDateTime& dt);
	BOOL EvaluateInputTextTime(LPCTSTR lpszInput, COleDateTime& dt);
	CString GetFormattedDate(LPCTSTR lpszValue);

	// ***************************************************************
	// support functiosn copied from gnrlsprt.h
	int CInt(LPCSTR lpszValue);
	BOOL IsInteger(LPCSTR lpszValue);
	CString FPSTrim(LPCSTR lpszInput, BOOL bForceUpper = FALSE);
	CString CStr(long lValue);
	BOOL FPSCompareString(LPCSTR lpszTest1, LPCSTR lpszTest2);
	BOOL EndsWith(CString& strTest, LPCTSTR lpszTestFor);
	BOOL BeginsWith(CString& strTest, LPCTSTR lpszTestFor);
	// ***************************************************************

	static CMapStringToString	m_mapShortcuts;
	static BOOL					m_bDefaultShortcutsLoaded;

	int m_lHourOptions;
	int m_lHourCutoff;
};

#endif // !defined(AFX_OLEDATETIMEEX_H__A5F3B26D_ACFB_4838_958B_DF0202518BAD__INCLUDED_)
