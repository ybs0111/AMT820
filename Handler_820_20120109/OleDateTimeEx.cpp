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
// This code is suitable for US and english speaking users only.  There
// is a need to enahance this algorithm to deal with multiple languages.  
// This enhancement is beyond the scope of the current projects requirements.
//
// Also, the methodology used here is quite simple and is basically a
// hard-coded set of common date/time entry situations.  There are no
// doubt numerous situations which are not covered.  The functions
// "EvaluateInputTextDate" and "EvaluateInputTextTime" are responsbile for
// the "parsing".  I belive it would be possible to come up with a rules
// based engine for dealing with this stuff, but I did not have time to
// create this engine.
//
// Also, this code makes heavy use of CString and as a result (in addition
// to the code being unoptimized), this code is not a high performance 
// algorithm.  This was perfectly OK for my uses since the parses are only
// used on rare occasions and even on slow PCs one hit is faster than the
// user would notice.  However, if this were used in an inner-loop situation
// where it was being called repeatedly, this algorithm would slow down
// the overall loop.
//
// Finally, at the time I released this code on CodeProject, the product
// which uses this class was in "late alpha" testing.  So far no problems
// have been identified in this class, but if some are, I will post them
// as soon as I can.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OleDateTimeEx.h"

#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleDateTimeEx::COleDateTimeEx()
{
	if (!m_bDefaultShortcutsLoaded)
	{
		LoadDefaultShortcuts();
		m_bDefaultShortcutsLoaded = TRUE;
	}

	m_lHourOptions = 0;
	m_lHourCutoff = OLEDATEEX_DEFAULT_HOUR_CUTOFF;
}

COleDateTimeEx::COleDateTimeEx(COleDateTimeEx& CopyFrom)
	: COleDateTime(CopyFrom)
{
	if (!m_bDefaultShortcutsLoaded)
	{
		LoadDefaultShortcuts();
		m_bDefaultShortcutsLoaded = TRUE;
	}

	m_lHourOptions = 0;
	m_lHourCutoff = OLEDATEEX_DEFAULT_HOUR_CUTOFF;
}

COleDateTimeEx::COleDateTimeEx(COleDateTime& CopyFrom)
	: COleDateTime(CopyFrom)
{
	if (!m_bDefaultShortcutsLoaded)
	{
		LoadDefaultShortcuts();
		m_bDefaultShortcutsLoaded = TRUE;
	}

	m_lHourOptions = 0;
	m_lHourCutoff = OLEDATEEX_DEFAULT_HOUR_CUTOFF;
}

COleDateTimeEx::~COleDateTimeEx()
{

}

void COleDateTimeEx::operator=(COleDateTimeEx& CopyFrom)
{
	m_dt = CopyFrom.m_dt;
	m_status = CopyFrom.m_status;
	m_lHourCutoff = CopyFrom.m_lHourCutoff;
	m_lHourOptions = CopyFrom.m_lHourOptions;
}

void COleDateTimeEx::operator=(COleDateTime& CopyFrom)
{
	m_dt = CopyFrom.m_dt;
	m_status = CopyFrom.m_status;
}


CMapStringToString		COleDateTimeEx::m_mapShortcuts;
BOOL					COleDateTimeEx::m_bDefaultShortcutsLoaded = FALSE;

// overide of the ParseDateTime function found in COleDateTime.  I call the base
// class first to see if it is a "normal" date situation.  If it is not, I then call
// EvaluateInputText which in-turn calls the EvaluateInputTextDate and 
// EvaluateInputTextTime functions attempting to parse the text.
BOOL COleDateTimeEx::ParseDateTime(LPCTSTR lpszDate, DWORD dwFlags, LCID lcid)
{
	BOOL bReturn = TRUE;

	COleDateTime dtTemp;
	CString strDate = lpszDate;

	// if we are performing time parsing, check for A.M. and P.M. situations
	if (dwFlags == VAR_TIMEVALUEONLY || dwFlags == 0)
		strDate = DealWithAMPMIssues(strDate);

	dtTemp.ParseDateTime(strDate, dwFlags, lcid);

	// try the default parser and check for success
	if (dtTemp.GetStatus() != COleDateTime::valid || (dwFlags == VAR_DATEVALUEONLY && IsOleDefaultDate(dtTemp)))
	{
		bReturn = FALSE; 

		// remove extra white space characters from input text.  This should improve
		// the parsing process to some (unknown) degree 
		CString strDateText = ExpandShortcuts(RemoveExtraWhiteSpace(FPSTrim(strDate, TRUE)));

		// perform only the requested parser operation
		if (dwFlags == VAR_DATEVALUEONLY)
		{
			if (EvaluateInputTextDate(strDateText, dtTemp))
				bReturn = TRUE;
		} 
		else if (dwFlags == VAR_TIMEVALUEONLY)
		{
			if (EvaluateInputTextTime(strDateText, dtTemp))
				bReturn = TRUE;
		}
		else 
		{
			if (EvaluateInputText(strDateText, dtTemp))
				bReturn = TRUE;
		}
	}

	if (bReturn)
	{
		m_dt = dtTemp.m_dt;
		m_status = dtTemp.m_status;
	}
	else
	{
		SetStatus(COleDateTime::invalid);
	}

	return bReturn;
}

// base implemnentation just calls date and time specific functions
BOOL COleDateTimeEx::EvaluateInputText(LPCTSTR lpszInput, COleDateTime &dt)
{
	if (EvaluateInputTextDate(lpszInput, dt))
		return TRUE;

	if (EvaluateInputTextTime(lpszInput, dt))
		return TRUE;

	return FALSE;
}

// This function determines when easter will occur for a specified year
// Special thanks to Oskar Wieland for his Easter calculation formula posted
// on CodeProject.
void COleDateTimeEx::CalcEasterSunday(WORD wYear, WORD &wMonth, WORD &wDay)
{
    // calculate easter sunday
    // [in]  wYear  - 4-digit year to calculate (but not before 1583)
    // [out] wMonth - month of easter sunday
    // [out] wDay   - day of easter sunday

    WORD wCorrection = 0;   
    
    if( wYear < 1700 )      wCorrection = 4;
    else if( wYear < 1800 ) wCorrection = 5;
    else if( wYear < 1900 ) wCorrection = 6;
    else if( wYear < 2100 ) wCorrection = 0;
    else if( wYear < 2200 ) wCorrection = 1;
    else if( wYear < 2300 ) wCorrection = 2;
    else if( wYear < 2500 ) wCorrection = 3;

    wDay = (WORD)((19 * (wYear % 19) + 24) % 30);        
    wDay = (WORD)(22 + wDay + ((2 * (wYear % 4) + 4 * (wYear % 7) + 6 * wDay + 5 + wCorrection) % 7));
    
    // jump to next month
    if( wDay > 31 )
    {
        wMonth = 4;
        wDay -= 31;
    }
    else
    {
        wMonth = 3;
    }
}

// see if strTest begins with a specified test string
BOOL COleDateTimeEx::BeginsWith(CString &strTest, LPCTSTR lpszTestFor)
{
	int iLen = lstrlen(lpszTestFor);

	return FPSCompareString(strTest.Left(iLen), lpszTestFor);
}

// see if strTest ends with a specified test string
BOOL COleDateTimeEx::EndsWith(CString &strTest, LPCTSTR lpszTestFor)
{
	int iLen = lstrlen(lpszTestFor);

	return FPSCompareString(strTest.Right(iLen), lpszTestFor);
}

// determine a month number from a string representation of the month
// This is soooo bad, but I don't have time to do this right at this time
int COleDateTimeEx::GetMonthNumFromName(LPCTSTR lpszInput)
{
	if (FPSCompareString(lpszInput, "JANUARY") || FPSCompareString(lpszInput, "JAN"))
		return 1;
	if (FPSCompareString(lpszInput, "FEBRUARY") || FPSCompareString(lpszInput, "FEB"))
		return 2;
	if (FPSCompareString(lpszInput, "MARCH") || FPSCompareString(lpszInput, "MAR"))
		return 3;
	if (FPSCompareString(lpszInput, "APRIL") || FPSCompareString(lpszInput, "APR"))
		return 4;
	if (FPSCompareString(lpszInput, "MAY"))
		return 5;
	if (FPSCompareString(lpszInput, "JUNE") || FPSCompareString(lpszInput, "JUN"))
		return 6;
	if (FPSCompareString(lpszInput, "JULY") || FPSCompareString(lpszInput, "JUL"))
		return 7;
	if (FPSCompareString(lpszInput, "AUGUST") || FPSCompareString(lpszInput, "AUG"))
		return 8;
	if (FPSCompareString(lpszInput, "SEPTEMBER") || FPSCompareString(lpszInput, "SEP"))
		return 9;
	if (FPSCompareString(lpszInput, "OCTOBER") || FPSCompareString(lpszInput, "OCT"))
		return 10;
	if (FPSCompareString(lpszInput, "NOVEMBER") || FPSCompareString(lpszInput, "NOV"))
		return 11;
	if (FPSCompareString(lpszInput, "DECEMBER") || FPSCompareString(lpszInput, "DEC"))
		return 12;

	return 0;
}

// computes the X weekday of a given month (ie. 2nd sunday in December)
COleDateTime COleDateTimeEx::ComputeDayInMonth(int iCount, int iDayOfWeek, int iMonth)
{
	COleDateTime dtNow = COleDateTime::GetCurrentTime();
	COleDateTime dtReturn;
	int iTries = 0;
	int iFoundTimes = 0;
	CString strStartDate;
	
	strStartDate = CStr(iMonth);
	strStartDate += "/01/";
	strStartDate += CStr(COleDateTime::GetCurrentTime().GetYear());

	dtReturn.SetDate(COleDateTime::GetCurrentTime().GetYear(), iMonth, 1);

	if (dtReturn.m_status == COleDateTime::valid)
	{
		CString strTemp = dtReturn.Format("%m/%d/%Y");

		while (iFoundTimes != iCount && iTries < 32)
		{
			if (dtReturn.GetDayOfWeek() == iDayOfWeek)
				iFoundTimes++;

			if (iFoundTimes < iCount)
				dtReturn += 1;

			iTries++;
		}

		if (iFoundTimes != iCount)
			dtReturn.m_status = COleDateTime::invalid;
	}

	// return should be in the future (or today)
	if (dtReturn.m_status == COleDateTime::valid && dtNow > dtReturn)
	{
		iTries = 0;
		iFoundTimes = 0;

		strStartDate = CStr(iMonth);
		strStartDate += "/01/";
		strStartDate += CStr(COleDateTime::GetCurrentTime().GetYear()+1);

		dtReturn.SetDate(COleDateTime::GetCurrentTime().GetYear()+1, iMonth, 1);

		if (dtReturn.m_status == COleDateTime::valid)
		{
			while (iFoundTimes != iCount && iTries < 32)
			{
				if (dtReturn.GetDayOfWeek() == iDayOfWeek)
					iFoundTimes++;

				if (iFoundTimes < iCount)
					dtReturn += 1;

				iTries++;
			}

			if (iFoundTimes != iCount)
				dtReturn.m_status = COleDateTime::invalid;
		}
	}

	return dtReturn;
}

// determines if the first tag in an input string is a numeric value
BOOL COleDateTimeEx::IsFirstTagNumeric(CString &strInput)
{
	BOOL bReturn = FALSE;
	int iPos = strInput.Find(" ");

	if (iPos != -1)
	{
		CString strTemp = FPSTrim(strInput.Left(iPos));
		if (IsInteger(strTemp))
			bReturn = TRUE;
	}

	return bReturn;
}

BOOL COleDateTimeEx::IsLastTagNumeric(CString &strInput)
{
	BOOL bReturn = FALSE;
	int iPos = strInput.ReverseFind(' ');

	if (iPos != -1)
	{
		CString strTemp = FPSTrim(strInput.Mid(iPos+1));
		if (IsInteger(strTemp))
			bReturn = TRUE;
	}

	return bReturn;
}

// calculates X # of weekday in the past (ie. 2 sundays ago)
BOOL COleDateTimeEx::CalcXDaysOfWeekAgo(int iDesiredCount, int iDayOfWeek, COleDateTime& dt)
{
	BOOL bReturn = FALSE;
	COleDateTime dtTemp = COleDateTime::GetCurrentTime();
	dtTemp -= 1;

	int iCount = 0;
	int iTries = 0;

	while (iCount != iDesiredCount && iTries < 200)
	{
		if (dtTemp.GetDayOfWeek() == iDayOfWeek)
			iCount++;				

		if (iCount != iDesiredCount)
			dtTemp -= 1;

		iTries++;
	}

	if (iCount == iDesiredCount)
	{
		dt = dtTemp;
		bReturn = TRUE;
	}

	return bReturn;
}

// calculates X # of weekday in the future (ie. 2 sundays from now)
BOOL COleDateTimeEx::CalcXDaysOfWeekFromNow(int iDesiredCount, int iDayOfWeek, COleDateTime& dt)
{
	BOOL bReturn = FALSE;
	COleDateTime dtTemp = COleDateTime::GetCurrentTime();
	dtTemp += 1;

	int iCount = 0;
	int iTries = 0;

	while (iCount != iDesiredCount && iTries < 200)
	{
		if (dtTemp.GetDayOfWeek() == iDayOfWeek)
			iCount++;				

		if (iCount != iDesiredCount)
			dtTemp += 1;

		iTries++;
	}

	if (iCount == iDesiredCount)
	{
		dt = dtTemp;
		bReturn = TRUE;
	}

	return bReturn;
}

// used to compute the "X months ago" situation and the "X months before easter"
// situations
COleDateTime COleDateTimeEx::ComputeMonthsAgo(int iMonths, COleDateTime& dtStart)
{
	COleDateTime dtReturn = dtStart;
	
	int iDay = dtReturn.GetDay();
	int iMonth = dtReturn.GetMonth();
	int iYear = dtReturn.GetYear();

	for (int iX = 0; iX < iMonths; iX++)
	{
		iMonth--;
		if (iMonth < 1)
		{
			iMonth = 12;
			iYear--;
		}

		// if the day of month from last iteration is not in the new month,
		// reverse the calculation
		int iDaysInMonth = GetMaxDaysInMonth(iMonth, iYear);
		if (iDay > iDaysInMonth)
		{
			iMonth--;
			if (iMonth < 1)
			{
				iMonth = 12;
				iYear--;
			}

			iDaysInMonth = GetMaxDaysInMonth(iMonth, iYear);
			iDay = iDaysInMonth - (iDaysInMonth - iDay);
		}
	}

	CString strDate = CStr(iMonth);
	strDate += "/";
	strDate += CStr(iDay);
	strDate += "/";
	strDate += CStr(iYear);

	dtReturn.SetDate(iYear, iMonth, iDay);

	return dtReturn;
}

// used to compute the "X months from now" situation and the "X months after easter"
// situations
COleDateTime COleDateTimeEx::ComputeMonthsFrom(int iMonths, COleDateTime& dtStart)
{
	COleDateTime dtReturn = dtStart;
	
	int iDay = dtReturn.GetDay();
	int iMonth = dtReturn.GetMonth();
	int iYear = dtReturn.GetYear();

	for (int iX = 0; iX < iMonths; iX++)
	{
		iMonth++;
		if (iMonth > 12)
		{
			iMonth = 1;
			iYear++;
		}

		// if the day of month from last iteration is not in the new month,
		// reverse the calculation
		int iDaysInMonth = GetMaxDaysInMonth(iMonth, iYear);
		if (iDay > iDaysInMonth)
		{
			iMonth++;
			if (iMonth > 12)
			{
				iMonth = 1;
				iYear++;
			}

			iDaysInMonth = GetMaxDaysInMonth(iMonth, iYear);
			iDay = (iDaysInMonth - iDay);
		}
	}

	CString strDate = CStr(iMonth);
	strDate += "/";
	strDate += CStr(iDay);
	strDate += "/";
	strDate += CStr(iYear);

	dtReturn.SetDate(iYear, iMonth, iDay);

	return dtReturn;
}

// determines the # of days in the month.  Only thing special
// about this function is the february situation and leap years
int COleDateTimeEx::GetMaxDaysInMonth(int iMonth, int iYear)
{
	int iReturn = 0;

	switch (iMonth)
	{
	case 1: {iReturn = 31; break;}
	case 2:
		{
			if (IsLeapYear(iYear))
				iReturn = 29;
			else
				iReturn = 28;
			break;
		}
	case 3: {iReturn = 31; break;}
	case 4: {iReturn = 30; break;}
	case 5: {iReturn = 31; break;}
	case 6: {iReturn = 30; break;}
	case 7: {iReturn = 31; break;}
	case 8: {iReturn = 31; break;}
	case 9: {iReturn = 30; break;}
	case 10: {iReturn = 31; break;}
	case 11: {iReturn = 30; break;}
	case 12: {iReturn = 31; break;}
	}

	return iReturn;
}

// this function checks a text input for various date-specific data entry
// situations such as today, tomorrow, yesterday, 2 weeks from now, etc
BOOL COleDateTimeEx::EvaluateInputTextDate(LPCTSTR lpszInput, COleDateTime &dt)
{
	// setup for various parsers
	BOOL bReturn = FALSE;
	CString strInput = FPSTrim(lpszInput, TRUE);

	// try various known formats of user input
	// these are basic entry methods commonly entered by my users
	if (IsInteger(strInput) && strInput.GetLength() == 6)		// MMDDYY or YYMMDD
	{
		CString strPart1 = strInput.Left(2);
		CString strPart2 = strInput.Mid(2,2);
		CString strPart3 = strInput.Right(2);
		CString strTemp;

		strTemp = strPart1;
		strTemp += "/";
		strTemp += strPart2;
		strTemp += "/";
		strTemp += strPart3;

		dt.ParseDateTime(strTemp, VAR_DATEVALUEONLY);

		if (dt.m_status == COleDateTime::valid && !IsOleDefaultDate(dt))
			return TRUE;
	}
	else if (IsInteger(strInput) && strInput.GetLength() == 8)		// MMDDYYYY
	{
		CString strPart1 = strInput.Left(2);
		CString strPart2 = strInput.Mid(2,2);
		CString strPart3 = strInput.Right(4);
		CString strTemp;

		strTemp = strPart1;
		strTemp += "/";
		strTemp += strPart2;
		strTemp += "/";
		strTemp += strPart3;

		dt.ParseDateTime(strTemp, VAR_DATEVALUEONLY);

		if (dt.m_status == COleDateTime::valid && !IsOleDefaultDate(dt))
			return TRUE;
	}

	// now, run the text through our little parser
	// most of the routines here are based on the current date
	dt = COleDateTime::GetCurrentTime();

	if (stricmp(strInput, "today") == 0 || stricmp(strInput, "now") == 0)
	{
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "yesterday") == 0)
	{
		dt -= 1;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "day before yesterday") == 0)
	{
		dt -= 2;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "tomorrow") == 0)
	{
		dt += 1;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "day after tomorrow") == 0)
	{
		dt += 2;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "next week") == 0)
	{
		dt += 7;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "last week") == 0)
	{
		dt -= 7;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "christmas") == 0 || stricmp(strInput, "xmas") == 0 || stricmp(strInput, "x-mas") == 0 || stricmp(strInput, "x-mass") == 0 || stricmp(strInput, "xmass") == 0 || stricmp(strInput, "x mass") == 0)
	{
		dt.SetDate(dt.GetYear(), 12, 25);
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "christmas eve") == 0 || stricmp(strInput, "xmas eve") == 0 || stricmp(strInput, "x-mas eve") == 0 || stricmp(strInput, "x-mass eve") == 0 || stricmp(strInput, "xmass eve") == 0 || stricmp(strInput, "x mass eve") == 0)
	{
		dt.SetDate(dt.GetYear(), 12, 24);
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "new years") == 0 || stricmp(strInput, "new years day") == 0)
	{
		dt.SetDate(dt.GetYear()+1, 1, 1);
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "new years eve") == 0 || stricmp(strInput, "new years eve day") == 0)
	{
		dt.SetDate(dt.GetYear(), 12, 31);
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "halloween") == 0)
	{
		dt.SetDate(dt.GetYear(), 10, 31);
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "July 4th") == 0 || stricmp(strInput, "July fourth") == 0 || stricmp(strInput, "July forth") == 0 || stricmp(strInput, "4th of July") == 0 || stricmp(strInput, "fourth of July") == 0 || stricmp(strInput, "forth of July") == 0 || stricmp(strInput, "independence day") == 0)
	{
		// set to 7/4 this year
		dt.SetDate(dt.GetYear(), 7, 4);

		// if 4th of july is already past, set to next year
		if (dt < COleDateTime::GetCurrentTime())
			dt.SetDate(dt.GetYear()+1, 7, 4);
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "easter") == 0 || stricmp(strInput, "easter sunday") == 0)
	{
		WORD wMonth = 0;
		WORD wDay = 0;

		CalcEasterSunday((WORD)dt.GetYear(), wMonth, wDay);

		dt.SetDate(dt.GetYear(), wMonth, wDay);

		// if easter has alreaddy occured, move to next year
		if (dt < COleDateTime::GetCurrentTime())
		{
			CalcEasterSunday((WORD)(dt.GetYear()+1), wMonth, wDay);

			dt.SetDate(dt.GetYear()+1, wMonth, wDay);
		}

		bReturn = TRUE;
	}
	else if (stricmp(strInput, "good friday") == 0)
	{
		WORD wMonth = 0;
		WORD wDay = 0;

		CalcEasterSunday((WORD)dt.GetYear(), wMonth, wDay);

		dt.SetDate(dt.GetYear(), wMonth, wDay);

		// if easter has alreaddy occured, move to next year
		if (dt < COleDateTime::GetCurrentTime())
		{
			CalcEasterSunday((WORD)(dt.GetYear()+1), wMonth, wDay);

			dt.SetDate(dt.GetYear()+1, wMonth, wDay);
		}
		
		// good friday occurs 2 days before easter sunday
		dt -= 2;

		bReturn = TRUE;
	}
	else if (stricmp(strInput, "ash wednesday") == 0)
	{
		WORD wMonth = 0;
		WORD wDay = 0;

		CalcEasterSunday((WORD)dt.GetYear(), wMonth, wDay);

		dt.SetDate(dt.GetYear(), wMonth, wDay);

		// if easter has alreaddy occured, move to next year
		if (dt < COleDateTime::GetCurrentTime())
		{
			CalcEasterSunday((WORD)(dt.GetYear()+1), wMonth, wDay);

			dt.SetDate(dt.GetYear()+1, wMonth, wDay);
		}
		
		// ash wednesday occurs 46 days before easte
		dt -= 46;

		bReturn = TRUE;
	}
	else if (stricmp(strInput, "veterans day") == 0 || stricmp(strInput, "veteran day") == 0)
	{
		dt.SetDate(dt.GetYear(), 11, 11);

		if (dt < COleDateTime::GetCurrentTime())
			dt.SetDate(dt.GetYear()+1, 11, 11);

		bReturn = TRUE;
	}
	else if (stricmp(strInput, "memorial day") == 0)
	{
		// last monday in may

		// try the 6th, 5th, 4th and 3rd mondays
		dt = ComputeDayInMonth(6, 2, 5);
		if (dt.m_status != COleDateTime::valid)
			dt = ComputeDayInMonth(5, 2, 5);
		if (dt.m_status != COleDateTime::valid)
			dt = ComputeDayInMonth(4, 2, 5);
		if (dt.m_status != COleDateTime::valid)
			dt = ComputeDayInMonth(3, 2, 5);

		if (dt.m_status == COleDateTime::valid)
			bReturn = TRUE;
	}
	else if (stricmp(strInput, "labor day") == 0)
	{
		// first monday in in september
		dt = ComputeDayInMonth(1, 2, 9);

		if (dt.m_status == COleDateTime::valid)
			bReturn = TRUE;
	}
	else if (stricmp(strInput, "columbus day") == 0)
	{
		// second monday in october
		dt = ComputeDayInMonth(2, 2, 10);

		if (dt.m_status == COleDateTime::valid)
			bReturn = TRUE;
	}
	else if (stricmp(strInput, "thanksgiving") == 0 || stricmp(strInput, "thanksgiving day") == 0)
	{
		// 4th thursday in november
		dt = ComputeDayInMonth(4, 5, 11);

		if (dt.m_status == COleDateTime::valid)
			bReturn = TRUE;
	}
	else if (stricmp(strInput, "Washington's Birthday") == 0 || stricmp(strInput, "Washingtons Birthday") == 0 || stricmp(strInput, "Washington Birthday") == 0 || stricmp(strInput, "presidents day") == 0 || stricmp(strInput, "president's day") == 0 || stricmp(strInput, "president day") == 0)
	{
		// third monday in february
		dt = ComputeDayInMonth(3, 2, 2);

		if (dt.m_status == COleDateTime::valid)
			bReturn = TRUE;
	}
	else if (stricmp(strInput, "NEXT YEAR") == 0 || stricmp(strInput, "THIS NEXT YEAR") == 0)
	{
		dt = COleDateTime::GetCurrentTime();

		dt.SetDate(dt.GetYear()+1, dt.GetMonth(), dt.GetDay());
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "PAST YEAR") == 0 || stricmp(strInput, "THIS PAST YEAR") == 0 || stricmp(strInput, "LAST YEAR") == 0 || stricmp(strInput, "THIS LAST YEAR") == 0)
	{
		dt = COleDateTime::GetCurrentTime();

		dt.SetDate(dt.GetYear()-1, dt.GetMonth(), dt.GetDay());
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "last sunday") == 0 || stricmp(strInput, "this past sunday") == 0 || stricmp(strInput, "past sunday") == 0)
	{
		int iSubDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iSubDays = 7; break;}		// sunday
		case 2: {iSubDays = 1; break;}		// monday
		case 3: {iSubDays = 2; break;}		// tuesday
		case 4: {iSubDays = 3; break;}		// wednesday
		case 5: {iSubDays = 4; break;}		// thursday
		case 6: {iSubDays = 5; break;}		// friday
		case 7: {iSubDays = 6; break;}		// saturday
		}

		dt -= iSubDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "last monday") == 0 || stricmp(strInput, "this past monday") == 0 || stricmp(strInput, "past monday") == 0)
	{
		int iSubDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iSubDays = 6; break;}		// sunday
		case 2: {iSubDays = 7; break;}		// monday
		case 3: {iSubDays = 1; break;}		// tuesday
		case 4: {iSubDays = 2; break;}		// wednesday
		case 5: {iSubDays = 3; break;}		// thursday
		case 6: {iSubDays = 4; break;}		// friday
		case 7: {iSubDays = 5; break;}		// saturday
		}

		dt -= iSubDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "last tuesday") == 0 || stricmp(strInput, "this past tuesday") == 0 || stricmp(strInput, "past tuesday") == 0)
	{
		int iSubDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iSubDays = 5; break;}		// sunday
		case 2: {iSubDays = 6; break;}		// monday
		case 3: {iSubDays = 7; break;}		// tuesday
		case 4: {iSubDays = 1; break;}		// wednesday
		case 5: {iSubDays = 2; break;}		// thursday
		case 6: {iSubDays = 3; break;}		// friday
		case 7: {iSubDays = 4; break;}		// saturday
		}

		dt -= iSubDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "last wednesday") == 0 || stricmp(strInput, "this past wednesday") == 0 || stricmp(strInput, "past wednesday") == 0)
	{
		int iSubDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iSubDays = 4; break;}		// sunday
		case 2: {iSubDays = 5; break;}		// monday
		case 3: {iSubDays = 6; break;}		// tuesday
		case 4: {iSubDays = 7; break;}		// wednesday
		case 5: {iSubDays = 1; break;}		// thursday
		case 6: {iSubDays = 2; break;}		// friday
		case 7: {iSubDays = 3; break;}		// saturday
		}

		dt -= iSubDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "last thursday") == 0 || stricmp(strInput, "this past thursday") == 0 || stricmp(strInput, "past thursday") == 0)
	{
		int iSubDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iSubDays = 3; break;}		// sunday
		case 2: {iSubDays = 4; break;}		// monday
		case 3: {iSubDays = 5; break;}		// tuesday
		case 4: {iSubDays = 6; break;}		// wednesday
		case 5: {iSubDays = 7; break;}		// thursday
		case 6: {iSubDays = 1; break;}		// friday
		case 7: {iSubDays = 2; break;}		// saturday
		}

		dt -= iSubDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "last friday") == 0 || stricmp(strInput, "this past friday") == 0 || stricmp(strInput, "past friday") == 0)
	{
		int iSubDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iSubDays = 2; break;}		// sunday
		case 2: {iSubDays = 3; break;}		// monday
		case 3: {iSubDays = 4; break;}		// tuesday
		case 4: {iSubDays = 5; break;}		// wednesday
		case 5: {iSubDays = 6; break;}		// thursday
		case 6: {iSubDays = 7; break;}		// friday
		case 7: {iSubDays = 1; break;}		// saturday
		}

		dt -= iSubDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "last saturday") == 0 || stricmp(strInput, "this past saturday") == 0 || stricmp(strInput, "past saturday") == 0)
	{
		int iSubDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iSubDays = 1; break;}		// sunday
		case 2: {iSubDays = 2; break;}		// monday
		case 3: {iSubDays = 3; break;}		// tuesday
		case 4: {iSubDays = 4; break;}		// wednesday
		case 5: {iSubDays = 5; break;}		// thursday
		case 6: {iSubDays = 6; break;}		// friday
		case 7: {iSubDays = 7; break;}		// saturday
		}

		dt -= iSubDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "next sunday") == 0 || stricmp(strInput, "sunday") == 0)
	{
		int iAddDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iAddDays = 7; break;}		// sunday
		case 2: {iAddDays = 6; break;}		// monday
		case 3: {iAddDays = 5; break;}		// tuesday
		case 4: {iAddDays = 4; break;}		// wednesday
		case 5: {iAddDays = 3; break;}		// thursday
		case 6: {iAddDays = 2; break;}		// friday
		case 7: {iAddDays = 1; break;}		// saturday
		}

		dt += iAddDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "next monday") == 0 || stricmp(strInput, "monday") == 0)
	{
		int iAddDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iAddDays = 1; break;}		// sunday
		case 2: {iAddDays = 7; break;}		// monday
		case 3: {iAddDays = 6; break;}		// tuesday
		case 4: {iAddDays = 5; break;}		// wednesday
		case 5: {iAddDays = 4; break;}		// thursday
		case 6: {iAddDays = 3; break;}		// friday
		case 7: {iAddDays = 2; break;}		// saturday
		}

		dt += iAddDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "next tuesday") == 0 || stricmp(strInput, "tuesday") == 0)
	{
		int iAddDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iAddDays = 2; break;}		// sunday
		case 2: {iAddDays = 1; break;}		// monday
		case 3: {iAddDays = 7; break;}		// tuesday
		case 4: {iAddDays = 6; break;}		// wednesday
		case 5: {iAddDays = 5; break;}		// thursday
		case 6: {iAddDays = 4; break;}		// friday
		case 7: {iAddDays = 3; break;}		// saturday
		}

		dt += iAddDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "next wednesday") == 0 || stricmp(strInput, "wednesday") == 0)
	{
		int iAddDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iAddDays = 3; break;}		// sunday
		case 2: {iAddDays = 2; break;}		// monday
		case 3: {iAddDays = 1; break;}		// tuesday
		case 4: {iAddDays = 7; break;}		// wednesday
		case 5: {iAddDays = 6; break;}		// thursday
		case 6: {iAddDays = 5; break;}		// friday
		case 7: {iAddDays = 4; break;}		// saturday
		}

		dt += iAddDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "next thursday") == 0 || stricmp(strInput, "thursday") == 0)
	{
		int iAddDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iAddDays = 4; break;}		// sunday
		case 2: {iAddDays = 3; break;}		// monday
		case 3: {iAddDays = 2; break;}		// tuesday
		case 4: {iAddDays = 1; break;}		// wednesday
		case 5: {iAddDays = 7; break;}		// thursday
		case 6: {iAddDays = 6; break;}		// friday
		case 7: {iAddDays = 5; break;}		// saturday
		}

		dt += iAddDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "next friday") == 0 || stricmp(strInput, "friday") == 0)
	{
		int iAddDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iAddDays = 5; break;}		// sunday
		case 2: {iAddDays = 4; break;}		// monday
		case 3: {iAddDays = 3; break;}		// tuesday
		case 4: {iAddDays = 2; break;}		// wednesday
		case 5: {iAddDays = 1; break;}		// thursday
		case 6: {iAddDays = 7; break;}		// friday
		case 7: {iAddDays = 6; break;}		// saturday
		}

		dt += iAddDays;
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "next saturday") == 0 || stricmp(strInput, "saturday") == 0)
	{
		int iAddDays = 0;

		switch (dt.GetDayOfWeek())
		{
		case 1: {iAddDays = 6; break;}		// sunday
		case 2: {iAddDays = 5; break;}		// monday
		case 3: {iAddDays = 4; break;}		// tuesday
		case 4: {iAddDays = 3; break;}		// wednesday
		case 5: {iAddDays = 2; break;}		// thursday
		case 6: {iAddDays = 1; break;}		// friday
		case 7: {iAddDays = 7; break;}		// saturday
		}

		dt += iAddDays;
		bReturn = TRUE;
	}
	else if ((stricmp(strInput, "ONE DAY AGO") == 0 || stricmp(strInput, "1 DAY AGO") == 0 || stricmp(strInput, "A DAY AGO") == 0 || stricmp(strInput, "DAY AGO") == 0) ||
			(stricmp(strInput, "ONE DAYS AGO") == 0 || stricmp(strInput, "1 DAYS AGO") == 0 || stricmp(strInput, "A DAYS AGO") == 0 || stricmp(strInput, "DAYS AGO") == 0))
	{
		dt -= 1;
		bReturn = TRUE;
	}
	else if ((stricmp(strInput, "ONE DAY FROM NOW") == 0 || stricmp(strInput, "1 DAY FROM NOW") == 0 || stricmp(strInput, "A DAY FROM NOW") == 0 || stricmp(strInput, "DAY FROM NOW") == 0) ||
		(stricmp(strInput, "ONE DAYS FROM NOW") == 0 || stricmp(strInput, "1 DAYS FROM NOW") == 0 || stricmp(strInput, "A DAYS FROM NOW") == 0 || stricmp(strInput, "DAYS FROM NOW") == 0))
	{
		dt += 1;
		bReturn = TRUE;
	}
	else if (EndsWith(strInput, " DAYS AGO") || EndsWith(strInput, " DAY AGO"))
	{
		int iPos = strInput.Find(" ");
		int iSubDays = CInt(strInput.Left(iPos));

		dt -= iSubDays;
		bReturn = TRUE;
	}
	else if (EndsWith(strInput, " DAYS FROM NOW") || EndsWith(strInput, " DAY FROM NOW"))
	{
		int iPos = strInput.Find(" ");
		int iAddDays = CInt(strInput.Left(iPos));

		dt += iAddDays;
		bReturn = TRUE;
	}
	else if (EndsWith(strInput, "WEEKS AGO") || EndsWith(strInput, "WEEK AGO"))
	{
		int iPos = strInput.Find(" ");
		int iSubDays = CInt(strInput.Left(iPos)) * 7;

		dt -= iSubDays;
		bReturn = TRUE;
	}
	else if (EndsWith(strInput, "WEEKS FROM NOW") || EndsWith(strInput, "WEEK FROM NOW"))
	{
		int iPos = strInput.Find(" ");
		int iAddDays = CInt(strInput.Left(iPos)) * 7;

		dt += iAddDays;
		bReturn = TRUE;
	}
	else if ((stricmp(strInput, "ONE MONTH AGO") == 0 || stricmp(strInput, "1 MONTH AGO") == 0 || stricmp(strInput, "A MONTH AGO") == 0 || stricmp(strInput, "MONTH AGO") == 0) ||
			(stricmp(strInput, "ONE MONTHS AGO") == 0 || stricmp(strInput, "1 MONTHS AGO") == 0 || stricmp(strInput, "A MONTHS AGO") == 0 || stricmp(strInput, "MONTHS AGO") == 0))
	{
		COleDateTime dtNowTemp = COleDateTime::GetCurrentTime();
		dt = ComputeMonthsAgo(1, dtNowTemp);
		bReturn = TRUE;
	}
	else if ((stricmp(strInput, "ONE MONTH FROM NOW") == 0 || stricmp(strInput, "1 MONTH FROM NOW") == 0 || stricmp(strInput, "A MONTH FROM NOW") == 0 || stricmp(strInput, "MONTH FROM NOW") == 0) ||
		(stricmp(strInput, "ONE MONTHS FROM NOW") == 0 || stricmp(strInput, "1 MONTHS FROM NOW") == 0 || stricmp(strInput, "A MONTHS FROM NOW") == 0 || stricmp(strInput, "MONTHS FROM NOW") == 0))
	{
		COleDateTime dtNowTemp = COleDateTime::GetCurrentTime();
		dt = ComputeMonthsFrom(1, dtNowTemp);
		bReturn = TRUE;
	}
	else if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, " MONTHS AGO") || EndsWith(strInput, " MONTHS AGO")))
	{
		int iPos = strInput.Find(" ");
		int iSubMonths = CInt(strInput.Left(iPos));
		COleDateTime dtNowTemp = COleDateTime::GetCurrentTime();

		dt = ComputeMonthsAgo(iSubMonths, dtNowTemp);
		bReturn = TRUE;
	}
	else if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, " MONTHS FROM NOW") || EndsWith(strInput, " MONTHS FROM NOW")))
	{
		int iPos = strInput.Find(" ");
		int iAddMonths = CInt(strInput.Left(iPos));
		COleDateTime dtNowTemp = COleDateTime::GetCurrentTime();

		dt = ComputeMonthsFrom(iAddMonths, dtNowTemp);
		bReturn = TRUE;
	}
	else if ((stricmp(strInput, "ONE YEAR AGO") == 0 || stricmp(strInput, "1 YEAR AGO") == 0 || stricmp(strInput, "A YEAR AGO") == 0 || stricmp(strInput, "YEAR AGO") == 0) ||
			(stricmp(strInput, "ONE YEARS AGO") == 0 || stricmp(strInput, "1 YEARS AGO") == 0 || stricmp(strInput, "A YEARS AGO") == 0 || stricmp(strInput, "YEARS AGO") == 0))
	{
		COleDateTime dtNowTemp = COleDateTime::GetCurrentTime();
		dt = ComputeMonthsAgo(12, dtNowTemp);
		bReturn = TRUE;
	}
	else if ((stricmp(strInput, "ONE YEAR FROM NOW") == 0 || stricmp(strInput, "1 YEAR FROM NOW") == 0 || stricmp(strInput, "A YEAR FROM NOW") == 0 || stricmp(strInput, "YEAR FROM NOW") == 0) ||
		(stricmp(strInput, "ONE YEARS FROM NOW") == 0 || stricmp(strInput, "1 YEARS FROM NOW") == 0 || stricmp(strInput, "A YEARS FROM NOW") == 0 || stricmp(strInput, "YEARS FROM NOW") == 0))
	{
		COleDateTime dtNowTemp = COleDateTime::GetCurrentTime();
		dt = ComputeMonthsFrom(12, dtNowTemp);
		bReturn = TRUE;
	}
	else if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, " YEARS AGO") || EndsWith(strInput, " YEARS AGO")))
	{
		int iPos = strInput.Find(" ");
		int iSubMonths = CInt(strInput.Left(iPos));
		COleDateTime dtNowTemp = COleDateTime::GetCurrentTime();

		dt = ComputeMonthsAgo(iSubMonths*12, dtNowTemp);
		bReturn = TRUE;
	}
	else if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, " YEARS FROM NOW") || EndsWith(strInput, " YEARS FROM NOW")))
	{
		int iPos = strInput.Find(" ");
		int iAddMonths = CInt(strInput.Left(iPos));
		COleDateTime dtNowTemp = COleDateTime::GetCurrentTime();

		dt = ComputeMonthsFrom(iAddMonths*12, dtNowTemp);
		bReturn = TRUE;
	}
	else if (BeginsWith(strInput, "1ST SUNDAY IN ") || BeginsWith(strInput, "1 SUNDAY IN ") || BeginsWith(strInput, "FIRST SUNDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(1, 1, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "2ND SUNDAY IN ") || BeginsWith(strInput, "2 SUNDAY IN ") || BeginsWith(strInput, "SECOND SUNDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(2, 1, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "3RD SUNDAY IN ") || BeginsWith(strInput, "3 SUNDAY IN ") || BeginsWith(strInput, "THIRD SUNDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(3, 1, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "4TH SUNDAY IN ") || BeginsWith(strInput, "4 SUNDAY IN ") || BeginsWith(strInput, "FOURTH SUNDAY IN ") || BeginsWith(strInput, "FORTH SUNDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(4, 1, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "5TH SUNDAY IN ") || BeginsWith(strInput, "5 SUNDAY IN ") || BeginsWith(strInput, "FIFTH SUNDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(5, 1, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "1ST MONDAY IN ") || BeginsWith(strInput, "1 MONDAY IN ") || BeginsWith(strInput, "FIRST MONDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(1, 2, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "2ND MONDAY IN ") || BeginsWith(strInput, "2 MONDAY IN ") || BeginsWith(strInput, "SECOND MONDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(2,2, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "3RD MONDAY IN ") || BeginsWith(strInput, "3 MONDAY IN ") || BeginsWith(strInput, "THIRD MONDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(3, 2, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "4TH MONDAY IN ") || BeginsWith(strInput, "4 MONDAY IN ") || BeginsWith(strInput, "FOURTH MONDAY IN ") || BeginsWith(strInput, "FORTH MONDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(4, 2, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "5TH MONDAY IN ") || BeginsWith(strInput, "5 MONDAY IN ") || BeginsWith(strInput, "FIFTH MONDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(5, 2, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "1ST TUESDAY IN ") || BeginsWith(strInput, "1 TUESDAY IN ") || BeginsWith(strInput, "FIRST TUESDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(1, 3, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "2ND TUESDAY IN ") || BeginsWith(strInput, "2 TUESDAY IN ") || BeginsWith(strInput, "SECOND TUESDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(2, 3, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "3RD TUESDAY IN ") || BeginsWith(strInput, "3 TUESDAY IN ") || BeginsWith(strInput, "THIRD TUESDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(3, 3, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "4TH TUESDAY IN ") || BeginsWith(strInput, "4 TUESDAY IN ") || BeginsWith(strInput, "FOURTH TUESDAY IN ") || BeginsWith(strInput, "FORTH TUESDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(4, 3, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "5TH TUESDAY IN ") || BeginsWith(strInput, "5 TUESDAY IN ") || BeginsWith(strInput, "FIFTH TUESDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(5, 3, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "1ST WEDNESDAY IN ") || BeginsWith(strInput, "1 WEDNESDAY IN ") || BeginsWith(strInput, "FIRST WEDNESDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(1, 4, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "2ND WEDNESDAY IN ") || BeginsWith(strInput, "2 WEDNESDAY IN ") || BeginsWith(strInput, "SECOND WEDNESDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(2, 4, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "3RD WEDNESDAY IN ") || BeginsWith(strInput, "3 WEDNESDAY IN ") || BeginsWith(strInput, "THIRD WEDNESDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(3, 4, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "4TH WEDNESDAY IN ") || BeginsWith(strInput, "4 WEDNESDAY IN ") || BeginsWith(strInput, "FOURTH WEDNESDAY IN ") || BeginsWith(strInput, "FORTH WEDNESDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(4, 4, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "5TH WEDNESDAY IN ") || BeginsWith(strInput, "5 WEDNESDAY IN ") || BeginsWith(strInput, "FIFTH WEDNESDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(5, 4, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "1ST THURSDAY IN ") || BeginsWith(strInput, "1 THURSDAY IN ") || BeginsWith(strInput, "FIRST THURSDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(1, 5, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "2ND THURSDAY IN ") || BeginsWith(strInput, "2 THURSDAY IN ") || BeginsWith(strInput, "SECOND THURSDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(2, 5, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "3RD THURSDAY IN ") || BeginsWith(strInput, "3 THURSDAY IN ") || BeginsWith(strInput, "THIRD THURSDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(3, 5, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "4TH THURSDAY IN ") || BeginsWith(strInput, "4 THURSDAY IN ") || BeginsWith(strInput, "FOURTH THURSDAY IN ") || BeginsWith(strInput, "FORTH THURSDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(4, 5, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "5TH THURSDAY IN ") || BeginsWith(strInput, "5 THURSDAY IN ") || BeginsWith(strInput, "FIFTH THURSDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(5, 5, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "1ST FRIDAY IN ") || BeginsWith(strInput, "1 FRIDAY IN ") || BeginsWith(strInput, "FIRST FRIDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(1, 6, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "2ND FRIDAY IN ") || BeginsWith(strInput, "2 FRIDAY IN ") || BeginsWith(strInput, "SECOND FRIDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(2, 6, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "3RD FRIDAY IN ") || BeginsWith(strInput, "3 FRIDAY IN ") || BeginsWith(strInput, "THIRD FRIDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(3, 6, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "4TH FRIDAY IN ") || BeginsWith(strInput, "4 FRIDAY IN ") || BeginsWith(strInput, "FOURTH FRIDAY IN ") || BeginsWith(strInput, "FORTH FRIDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(4, 6, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "5TH FRIDAY IN ") || BeginsWith(strInput, "5 FRIDAY IN ") || BeginsWith(strInput, "FIFTH FRIDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(5, 6, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "1ST SATURDAY IN ") || BeginsWith(strInput, "1 SATURDAY IN ") || BeginsWith(strInput, "FIRST SATURDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(1, 7, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "2ND SATURDAY IN ") || BeginsWith(strInput, "2 SATURDAY IN ") || BeginsWith(strInput, "SECOND SATURDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(2, 7, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "3RD SATURDAY IN ") || BeginsWith(strInput, "3 SATURDAY IN ") || BeginsWith(strInput, "THIRD SATURDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(3, 7, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "4TH SATURDAY IN ") || BeginsWith(strInput, "4 SATURDAY IN ") || BeginsWith(strInput, "FOURTH SATURDAY IN ") || BeginsWith(strInput, "FORTH SATURDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(4, 7, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}
	else if (BeginsWith(strInput, "5TH SATURDAY IN ") || BeginsWith(strInput, "5 SATURDAY IN ") || BeginsWith(strInput, "FIFTH SATURDAY IN "))
	{
		int iPos = strInput.ReverseFind(' ');
		CString strMonth = FPSTrim(strInput.Mid(iPos));
		int iMonth = GetMonthNumFromName(strMonth);

		if (iMonth > 0)
		{
			dt = ComputeDayInMonth(5, 7, iMonth);
			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}

	// check for the scenarious like (X days from XXXXXX or X weeks from XXXXX)
	if (!bReturn && IsFirstTagNumeric(strInput))
	{	
		COleDateTime dtTemp;
		int iPos = strInput.Find(" ");
		int iNumber = CInt(strInput.Left(iPos));
		CString strTemp = FPSTrim(strInput.Mid(iPos+1));

		if (BeginsWith(strTemp, "DAYS FROM "))
		{
			if (EvaluateInputText(strTemp.Mid(10), dtTemp))
			{
				dt = dtTemp;
				dt += iNumber;
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "DAYS AFTER "))
		{
			if (EvaluateInputText(strTemp.Mid(11), dtTemp))
			{
				dt = dtTemp;
				dt += iNumber;
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "DAYS BEFORE "))
		{
			if (EvaluateInputText(strTemp.Mid(12), dtTemp))
			{
				dt = dtTemp;
				dt -= iNumber;
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "WEEKS FROM "))
		{
			if (EvaluateInputText(strTemp.Mid(11), dtTemp))
			{
				dt = dtTemp;
				dt += (iNumber * 7);
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "WEEKS AFTER "))
		{
			if (EvaluateInputText(strTemp.Mid(12), dtTemp))
			{
				dt = dtTemp;
				dt += (iNumber * 7);
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "DAYS BEFORE "))
		{
			if (EvaluateInputText(strTemp.Mid(11), dtTemp))
			{
				dt = dtTemp;
				dt -= (iNumber * 7);
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "MONTHS BEFORE "))
		{
			if (EvaluateInputText(strTemp.Mid(13), dtTemp))
			{
				dt = ComputeMonthsAgo(iNumber, dtTemp);
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "MONTHS FROM "))
		{
			if (EvaluateInputText(strTemp.Mid(11), dtTemp))
			{
				dt = ComputeMonthsFrom(iNumber, dtTemp);
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "MONTHS AFTER "))
		{
			if (EvaluateInputText(strTemp.Mid(12), dtTemp))
			{
				dt = ComputeMonthsFrom(iNumber, dtTemp);
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "YEARS BEFORE "))
		{
			if (EvaluateInputText(strTemp.Mid(12), dtTemp))
			{
				dt = ComputeMonthsAgo(iNumber*12, dtTemp);
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "YEARS FROM "))
		{
			if (EvaluateInputText(strTemp.Mid(10), dtTemp))
			{
				dt = ComputeMonthsFrom(iNumber*12, dtTemp);
				bReturn = TRUE;
			}
		}
		else if (BeginsWith(strTemp, "YEARS AFTER "))
		{
			if (EvaluateInputText(strTemp.Mid(11), dtTemp))
			{
				dt = ComputeMonthsFrom(iNumber*12, dtTemp);
				bReturn = TRUE;
			}
		}
	}

	// if still no match, look for the siutations like ("christmas - 2 days" or "christmas + 2 days")
	if (!bReturn)
	{
		BOOL bOK = TRUE;
		BOOL bAdd = TRUE;
		CString strPart1;
		CString strPart2;

		// find the first addition/subtration operator
		if (strInput.Find(" + ") != -1)
		{
			int iPos = strInput.Find(" + ");
			strPart1 = strInput.Left(iPos);
			strPart2 = strInput.Mid(iPos+3);
		}
		if (strInput.Find(" +") != -1)
		{
			int iPos = strInput.Find(" +");
			strPart1 = strInput.Left(iPos);
			strPart2 = strInput.Mid(iPos+2);
		}
		if (strInput.Find("+ ") != -1)
		{
			int iPos = strInput.Find("+ ");
			strPart1 = strInput.Left(iPos);
			strPart2 = strInput.Mid(iPos+2);
		}
		if (strInput.Find("+") != -1)
		{
			int iPos = strInput.Find("+");
			strPart1 = strInput.Left(iPos);
			strPart2 = strInput.Mid(iPos+1);
		}
		else if (strInput.Find(" PLUS ") != -1)
		{
			int iPos = strInput.Find(" PLUS ");
			strPart1 = strInput.Left(iPos);
			strPart2 = strInput.Mid(iPos+6);
		}
		else if (strInput.Find(" - ") != -1)
		{
			bAdd = FALSE;
			int iPos = strInput.Find(" - ");
			strPart1 = strInput.Left(iPos);
			strPart2 = strInput.Mid(iPos+3);
		}
		else if (strInput.Find("- ") != -1)
		{
			bAdd = FALSE;
			int iPos = strInput.Find("- ");
			strPart1 = strInput.Left(iPos);
			strPart2 = strInput.Mid(iPos+2);
		}
		else if (strInput.Find(" -") != -1)
		{
			bAdd = FALSE;
			int iPos = strInput.Find(" -");
			strPart1 = strInput.Left(iPos);
			strPart2 = strInput.Mid(iPos+2);
		}
		else if (strInput.Find("-") != -1)
		{
			bAdd = FALSE;
			int iPos = strInput.Find("-");
			strPart1 = strInput.Left(iPos);
			strPart2 = strInput.Mid(iPos+1);
		}
		else if (strInput.Find(" MINUS ") != -1)
		{
			bAdd = FALSE;
			int iPos = strInput.Find(" MINUS ");
			strPart1 = strInput.Left(iPos);
			strPart2 = strInput.Mid(iPos+7);
		}
		else
		{
			bOK = FALSE;
		}

		// only proceed if we found an operator indicator
		if (bOK)
		{
			bOK = FALSE;
			COleDateTime dtTemp;
			int iAddDays = 0;

			// look for various situations (ie. part1 = number, part 2= number)
			if (IsInteger(strPart1))
			{
				iAddDays = CInt(strPart1);

				if (EvaluateInputText(strPart2, dtTemp))
					bOK = TRUE;
			}
			else if (IsInteger(strPart2))
			{
				iAddDays = CInt(strPart2);

				if (EvaluateInputText(strPart1, dtTemp))
					bOK = TRUE;
			}
			else if (IsFirstTagNumeric(strPart1) && FPSCompareString(strPart1.Mid(strPart1.Find(" ")+1), "DAYS"))
			{
				iAddDays = CInt(strPart1.Left(strPart1.Find(" ")));

				if (EvaluateInputText(strPart2, dtTemp))
					bOK = TRUE;
			}
			else if (IsFirstTagNumeric(strPart1) && FPSCompareString(strPart1.Mid(strPart1.Find(" ")+1), "WEEKS"))
			{
				iAddDays = CInt(strPart1.Left(strPart1.Find(" "))) * 7;

				if (EvaluateInputText(strPart2, dtTemp))
					bOK = TRUE;
			}
			else if (IsFirstTagNumeric(strPart2) && FPSCompareString(strPart2.Mid(strPart2.Find(" ")+1), "DAYS"))
			{
				iAddDays = CInt(strPart2.Left(strPart2.Find(" ")));

				if (EvaluateInputText(strPart1, dtTemp))
					bOK = TRUE;
			}
			else if (IsFirstTagNumeric(strPart2) && FPSCompareString(strPart2.Mid(strPart2.Find(" ")+1), "WEEKS"))
			{
				iAddDays = CInt(strPart2.Left(strPart2.Find(" "))) * 7;

				if (EvaluateInputText(strPart1, dtTemp))
					bOK = TRUE;
			}

			// only continue if part 1 and part 2 could be evaluated
			if (bOK && bAdd)
			{
				dt = dtTemp;
				dt += iAddDays;
				bReturn = TRUE;
			}
			else if (bOK && !bAdd)
			{
				dt = dtTemp;
				dt -= iAddDays;
				bReturn = TRUE;
			}
		}
	}

	// if still no computed date, look for the "X SUNDAYS AGO" scenarios
	if (!bReturn)
	{
		if (IsFirstTagNumeric(strInput) && EndsWith(strInput, "SUNDAYS AGO"))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekAgo(iDesiredCount, 1, dt);
		}
		else if (IsFirstTagNumeric(strInput) && EndsWith(strInput, "MONDAYS AGO"))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekAgo(iDesiredCount, 2, dt);
		}
		else if (IsFirstTagNumeric(strInput) && EndsWith(strInput, "TUESDAYS AGO"))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekAgo(iDesiredCount, 3, dt);
		}
		else if (IsFirstTagNumeric(strInput) && EndsWith(strInput, "WEDNESDAYS AGO"))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekAgo(iDesiredCount, 4, dt);
		}
		else if (IsFirstTagNumeric(strInput) && EndsWith(strInput, "THURSDAYS AGO"))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekAgo(iDesiredCount, 5, dt);
		}
		else if (IsFirstTagNumeric(strInput) && EndsWith(strInput, "FRIDAYS AGO"))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekAgo(iDesiredCount, 6, dt);
		}
		else if (IsFirstTagNumeric(strInput) && EndsWith(strInput, "SATURDAYS AGO"))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekAgo(iDesiredCount, 7, dt);
		}
	}

	// if still no computed date, look for the "X SUNDAYS FROM NOW" scenarios
	if (!bReturn)
	{
		if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, "SUNDAYS FROM NOW") || EndsWith(strInput, "SUNDAYS FROM TODAY")))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekFromNow(iDesiredCount, 1, dt);
		}
		else if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, "MONDAYS FROM NOW") || EndsWith(strInput, "MONDAYS FROM TODAY")))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekFromNow(iDesiredCount, 2, dt);
		}
		else if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, "TUESDAYS FROM NOW") || EndsWith(strInput, "TUESDAYS FROM TODAY")))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekFromNow(iDesiredCount, 3, dt);
		}
		else if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, "WEDNESDAYS FROM NOW") || EndsWith(strInput, "WEDNESDAYS FROM TODAY")))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekFromNow(iDesiredCount, 4, dt);
		}
		else if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, "THURSDAYS FROM NOW") || EndsWith(strInput, "THURSDAYS FROM TODAY")))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekFromNow(iDesiredCount, 5, dt);
		}
		else if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, "FRIDAYS FROM NOW") || EndsWith(strInput, "FRIDAYS FROM TODAY")))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekFromNow(iDesiredCount, 6, dt);
		}
		else if (IsFirstTagNumeric(strInput) && (EndsWith(strInput, "SATURDAYS FROM NOW") || EndsWith(strInput, "SATURDAYS FROM TODAY")))
		{
			int iDesiredCount = CInt(strInput.Left(strInput.Find(" ")));
			bReturn = CalcXDaysOfWeekFromNow(iDesiredCount, 7, dt);
		}
	}

	// if still no success, check for the NEXT EASTER, LAST NEW YEAR type 
	// siturations as generics
	if (!bReturn)
	{
		CString strTest;
		BOOL bOK = FALSE;
		BOOL bNext = FALSE;
		COleDateTimeEx dtCompute;
		COleDateTime dtNow;
		
		dtNow = COleDateTime::GetCurrentTime();

		if (BeginsWith(strInput, "NEXT "))
		{
			strTest = FPSTrim(strInput.Mid(5));
			bOK = TRUE;
			bNext = TRUE;
		}
		else if (BeginsWith(strInput, "THIS NEXT "))
		{
			strTest = FPSTrim(strInput.Mid(10));
			bOK = TRUE;
			bNext = TRUE;
		}
		else if (BeginsWith(strInput, "LAST ") || BeginsWith(strInput, "PAST "))
		{
			strTest = FPSTrim(strInput.Mid(5));
			bOK = TRUE;
			bNext = FALSE;
		}
		else if (BeginsWith(strInput, "THIS LAST ") || BeginsWith(strInput, "THIS PAST "))
		{
			strTest = FPSTrim(strInput.Mid(10));
			bOK = TRUE;
			bNext = FALSE;
		}

		// if condition is TRUE, try and parse the input text into a date
		if (bOK)
		{
			// compute a date from the words().  This is used for the Easter, Christmas, situations
			if (ComputeDateFromWord(dtCompute, strTest, dtNow.GetYear()))
			{
				if (bNext)
				{
					if (dtCompute <= dtNow)
						ComputeDateFromWord(dtCompute, strTest, dtNow.GetYear()+1);

					dt = dtCompute;
					bReturn = TRUE;
				}
				else
				{
					if (dtCompute >= dtNow)
						ComputeDateFromWord(dtCompute, strTest, dtNow.GetYear()-1);

					dt = dtCompute;
					bReturn = TRUE;
				}
			}
		}
	}

	// if still no parsable conditions, check for the 'Easter 2005' like situations
	if (!bReturn && IsLastTagNumeric(strInput))
	{
		int iPos = strInput.ReverseFind(' ');
		int iYear = CInt(strInput.Mid(iPos+1));
		CString strTest = FPSTrim(strInput.Left(iPos));
		COleDateTimeEx dtCompute;

		if (ComputeDateFromWord(dtCompute, strTest, iYear))
		{
			dt = dtCompute;
			bReturn = TRUE;
		}
	}

	// if still no computed date, look for the "IN X DAYS", "IN X WEEKS" scenarios
	if (!bReturn)
	{
		if (strInput.Right(5) == " DAYS")
		{
			if (strInput.Left(3) == "IN ")
				strInput = strInput.Mid(3);

			int iPos = strInput.Find(" ");
			int iAddDays = CInt(strInput.Left(iPos));

			dt += iAddDays;
			bReturn = TRUE;
		}
		else if (strInput.Right(6) == " WEEKS")
		{
			if (strInput.Left(3) == "IN ")
				strInput = strInput.Mid(3);

			int iPos = strInput.Find(" ");
			int iAddDays = CInt(strInput.Left(iPos)) * 7;

			dt += iAddDays;
			bReturn = TRUE;
		}
		else if (strInput.Right(7) == " MONTHS")
		{
			if (strInput.Left(3) == "IN ")
				strInput = strInput.Mid(3);

			int iPos = strInput.Find(" ");
			int iAddMonths = CInt(strInput.Left(iPos));
			COleDateTime dtNowTemp = COleDateTime::GetCurrentTime();

			dt = ComputeMonthsFrom(iAddMonths, dtNowTemp);
			bReturn = TRUE;
		}
		else if (strInput.Right(6) == " YEARS")
		{
			if (strInput.Left(3) == "IN ")
				strInput = strInput.Mid(3);

			int iPos = strInput.Find(" ");
			int iAddMonths = CInt(strInput.Left(iPos));
			COleDateTime dtNowTemp = COleDateTime::GetCurrentTime();

			dt = ComputeMonthsFrom(iAddMonths*12, dtNowTemp);
			bReturn = TRUE;
		}
	}

	return bReturn;
}

BOOL COleDateTimeEx::EvaluateInputTextTime(LPCTSTR lpszInput, COleDateTime &dt)
{
	BOOL bReturn = FALSE;
	CString strInput = FPSTrim(lpszInput, TRUE);

	// most of the routines here are based on the current date
	dt = COleDateTime::GetCurrentTime();

	if (stricmp(strInput, "now") == 0)
	{
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "noon") == 0)
	{
		dt.ParseDateTime("12:00 PM");
		bReturn = TRUE;
	}
	else if (stricmp(strInput, "midnight") == 0)
	{
		dt.ParseDateTime("12:00 AM");
		bReturn = TRUE;
	}
	else if (IsInteger(strInput) && strInput.GetLength() == 3)
	{
		// ie: 430
		CString strTemp;
		CString strHour;
		CString strMinute;
		int iMinute = CInt(strInput.Right(2));
		int iHour = CInt(strInput.Left(1));

		switch(m_lHourOptions)
		{
		case OLEDATEEX_HOUR_DETECT_CURRENT_HOUR:
			{
				if (iHour < dt.GetHour())
					iHour += 12;
				break;
			}
		case OLEDATEEX_HOUR_DETECT_USE_CUTOFF:
			{
				if (iHour < m_lHourCutoff)
					iHour += 12;
				break;
			}
		case OLEDATEEX_HOUR_DETECT_LEAVE:
			{
				// don't be clever, just do what you are told!
				break;
			} 
		default:		 //default behavior, before 7 is PM, after is AM
			{
				if (iHour < 7)
					iHour += 12;
				break;
			}
		}

		if (iHour > 23)
			iHour = 23;

		strHour = "00"; strHour += CStr(iHour); strHour = strHour.Right(2);
		strMinute = "00"; strMinute += CStr(iMinute); strMinute = strMinute.Right(2);

		strTemp = strHour;
		strTemp += ":";
		strTemp += strMinute;

		dt.ParseDateTime(strTemp, VAR_TIMEVALUEONLY);
		bReturn = TRUE;
	}
	else if (IsInteger(strInput) && strInput.GetLength() == 4)
	{
		// ie: 1230
		CString strTemp;
		CString strHour;
		CString strMinute;
		int iMinute = CInt(strInput.Right(2));
		int iHour = CInt(strInput.Left(2));

		switch(m_lHourOptions)
		{
		case OLEDATEEX_HOUR_DETECT_CURRENT_HOUR:
			{
				if (iHour < 12 && iHour < dt.GetHour())
					iHour += 12;
				break;
			}
		case OLEDATEEX_HOUR_DETECT_USE_CUTOFF:
			{
				//two digit hour, but less than 10, must be military, leave as is
				break;
			}
		case OLEDATEEX_HOUR_DETECT_LEAVE:
			{
				//don't be clever, just do what you are told!
				break;
			}
		default:
			{
				if (iHour < 7)
					iHour += 12;
			}
			break;
		}

		if (iHour > 23)
			iHour = 23;

		strHour = "00"; strHour += CStr(iHour); strHour = strHour.Right(2);
		strMinute = "00"; strMinute += CStr(iMinute); strMinute = strMinute.Right(2);

		strTemp = strHour;
		strTemp += ":";
		strTemp += strMinute;

		dt.ParseDateTime(strTemp, VAR_TIMEVALUEONLY);
		bReturn = TRUE;
	}
	else if (IsInteger(strInput))
	{
		// this is used for the situation when the user just enters a number
		CString strTemp;
		CString strHour = strInput;
		CString strMinute;
		int iMinute = 0;
		int iHour = CInt(strInput);

		strHour.TrimLeft();
		strHour.TrimRight();

		switch(m_lHourOptions)
		{
		case OLEDATEEX_HOUR_DETECT_CURRENT_HOUR:
			{
				if (iHour < 12 && iHour < dt.GetHour())
					iHour += 12;
				break;
			}
		case OLEDATEEX_HOUR_DETECT_USE_CUTOFF:
			{
				if (iHour < 12 && iHour < m_lHourCutoff)
					iHour += 12;
				break;
			}
		case OLEDATEEX_HOUR_DETECT_LEAVE:
			{
				//don't be clever, just do what you are told!
				break;
			}
		default:
			{
				if (iHour < 7)
					iHour += 12;
				break;
			}
		}

		if (iHour > 23)
			iHour = 23;

		strHour = "00"; strHour += CStr(iHour); strHour = strHour.Right(2);
		strMinute = "00";

		strTemp = strHour;
		strTemp += ":";
		strTemp += strMinute;

		dt.ParseDateTime(strTemp, VAR_TIMEVALUEONLY);
		bReturn = TRUE;
	}
	else if (strInput.Find("OCLOCK") > 1 || strInput.Find("O'CLOCK") > 1 || strInput.Find("O CLOCK") > 1)
	{
		CString strTemp;
		CString strHour;
		CString strMinute;
		int iPos = strInput.Find(" ");
		int iHour = CInt(strInput.Left(iPos));

		if (iHour < 7)
			iHour += 12;
		if (iHour > 23)
			iHour = 23;

		strHour = "00"; strHour += CStr(iHour); strHour = strHour.Right(2);
		strMinute = "00";

		strTemp = strHour;
		strTemp += ":";
		strTemp += strMinute;

		dt.ParseDateTime(strTemp, VAR_TIMEVALUEONLY);
		bReturn = TRUE;
	}
	else if (EndsWith(strInput, "MINUTES FROM NOW"))
	{
		int iPos = strInput.Find(" ");
		int iMinutes = CInt(strInput.Left(iPos));
		COleDateTimeSpan span(0,0,iMinutes,0);

		dt += span;
		bReturn = TRUE;
	}
	else if (EndsWith(strInput,"HOURS FROM NOW"))
	{
		int iPos = strInput.Find(" ");
		int iHours = CInt(strInput.Left(iPos));
		COleDateTimeSpan span(0,iHours,0,0);

		dt += span;
		bReturn = TRUE;
	}
	else if (EndsWith(strInput,"MINUTES AGO"))
	{
		int iPos = strInput.Find(" ");
		int iMinutes = CInt(strInput.Left(iPos));
		COleDateTimeSpan span(0,0,iMinutes,0);

		dt -= span;
		bReturn = TRUE;
	}
	else if (EndsWith(strInput,"HOURS AGO"))
	{
		int iPos = strInput.Find(" ");
		int iHours = CInt(strInput.Left(iPos));
		COleDateTimeSpan span(0,iHours,0,0);

		dt -= span;
		bReturn = TRUE;
	}
	else if (IsFirstTagNumeric(strInput))		// situations like (15 till 6) or (15 after 6)
	{
		int iPos = strInput.Find(" ");
		int iNumber = CInt(strInput.Left(iPos));
		strInput = FPSTrim(strInput.Mid(iPos+1));

		if (iNumber < 59 && iNumber > 0)
		{
			if (BeginsWith(strInput, "MINUTES TILL "))
			{
				COleDateTime dtTemp = COleDateTime::GetCurrentTime();
				CString strTemp = strInput.Mid(12);
				int iMinutes = iNumber;
				COleDateTimeSpan span(0,0,iMinutes,0);

				if (EvaluateInputTextTime(strTemp, dtTemp))
				{
					dt = dtTemp;
					dt -= span;
					bReturn = TRUE;
				}
			}
			else if (BeginsWith(strInput, "TILL "))
			{
				COleDateTime dtTemp = COleDateTime::GetCurrentTime();
				CString strTemp = strInput.Mid(4);
				int iMinutes = iNumber;
				COleDateTimeSpan span(0,0,iMinutes,0);

				if (EvaluateInputTextTime(strTemp, dtTemp))
				{
					dt = dtTemp;
					dt -= span;
					bReturn = TRUE;
				}
			}
			else if (BeginsWith(strInput, "TIL "))
			{
				COleDateTime dtTemp = COleDateTime::GetCurrentTime();
				CString strTemp = strInput.Mid(3);
				int iMinutes = iNumber;
				COleDateTimeSpan span(0,0,iMinutes,0);

				if (EvaluateInputTextTime(strTemp, dtTemp))
				{
					dt = dtTemp;
					dt -= span;
					bReturn = TRUE;
				}
			}
			else if (BeginsWith(strInput, "MINUTES UNTIL "))
			{
				COleDateTime dtTemp = COleDateTime::GetCurrentTime();
				CString strTemp = strInput.Mid(13);
				int iMinutes = iNumber;
				COleDateTimeSpan span(0,0,iMinutes,0);

				if (EvaluateInputTextTime(strTemp, dtTemp))
				{
					dt = dtTemp;
					dt -= span;
					bReturn = TRUE;
				}
			}
			else if (BeginsWith(strInput, "UNTIL "))
			{
				COleDateTime dtTemp = COleDateTime::GetCurrentTime();
				CString strTemp = strInput.Mid(5);
				int iMinutes = iNumber;
				COleDateTimeSpan span(0,0,iMinutes,0);

				if (EvaluateInputTextTime(strTemp, dtTemp))
				{
					dt = dtTemp;
					dt -= span;
					bReturn = TRUE;
				}
			}
			else if (BeginsWith(strInput, "MINUTES PAST "))
			{
				COleDateTime dtTemp = COleDateTime::GetCurrentTime();
				CString strTemp = strInput.Mid(12);
				int iMinutes = iNumber;
				COleDateTimeSpan span(0,0,iMinutes,0);

				if (EvaluateInputTextTime(strTemp, dtTemp))
				{
					dt = dtTemp;
					dt += span;
					bReturn = TRUE;
				}
			}
			else if (BeginsWith(strInput, "PAST "))
			{
				COleDateTime dtTemp = COleDateTime::GetCurrentTime();
				CString strTemp = strInput.Mid(4);
				int iMinutes = iNumber;
				COleDateTimeSpan span(0,0,iMinutes,0);

				if (EvaluateInputTextTime(strTemp, dtTemp))
				{
					dt = dtTemp;
					dt += span;
					bReturn = TRUE;
				}
			}
			else if (BeginsWith(strInput, "MINUTES AFTER "))
			{
				COleDateTime dtTemp = COleDateTime::GetCurrentTime();
				CString strTemp = strInput.Mid(13);
				int iMinutes = iNumber;
				COleDateTimeSpan span(0,0,iMinutes,0);

				if (EvaluateInputTextTime(strTemp, dtTemp))
				{
					dt = dtTemp;
					dt += span;
					bReturn = TRUE;
				}
			}
			else if (BeginsWith(strInput, "AFTER "))
			{
				COleDateTime dtTemp = COleDateTime::GetCurrentTime();
				CString strTemp = strInput.Mid(5);
				int iMinutes = iNumber;
				COleDateTimeSpan span(0,0,iMinutes,0);

				if (EvaluateInputTextTime(strTemp, dtTemp))
				{
					dt = dtTemp;
					dt += span;
					bReturn = TRUE;
				}
			}
		}
	}
	else if (BeginsWith(strInput, "HALF PAST "))
	{
		CString strTemp = strInput.Mid(10);
		COleDateTime dtTemp;
		COleDateTimeSpan span(0,0,30,0);

		if (EvaluateInputTextTime(strTemp, dtTemp))
		{
			dt = dtTemp;
			dt += span;
			bReturn = TRUE;
		}
	}

	return bReturn;
}


// ***************************************************************************
// Begin String Support functions
// ***************************************************************************
BOOL COleDateTimeEx::FPSCompareString(LPCSTR lpszTest1, LPCSTR lpszTest2)
{
	CString strComp1 = lpszTest1;
	CString strComp2 = lpszTest2;

	strComp1.TrimRight();
	strComp2.TrimRight();

	if (stricmp(strComp1, strComp2) == 0)
		return TRUE;

	return FALSE;
}

CString COleDateTimeEx::CStr(long lValue)
{
	char szValue[20];

	_ltoa(lValue, szValue, 10);

	return szValue;
}

CString COleDateTimeEx::FPSTrim(LPCSTR lpszInput, BOOL bForceUpper)
{
	CString strReturn;

	strReturn = lpszInput;
	strReturn.TrimRight();
	strReturn.TrimLeft();

	if (bForceUpper)
		strReturn.MakeUpper();

	return strReturn;
}

BOOL COleDateTimeEx::IsInteger(LPCSTR lpszVL)
{
	BOOL bReturn = TRUE;
	int iNumCount = 0;
	int iLen = lstrlen(lpszVL);
	int iPos = 0;

	while (iPos < iLen && bReturn)
	{
		if (::isdigit(lpszVL[iPos]) || lpszVL[iPos] == '-')
		{
			iNumCount++;
		}
		else
		{
			bReturn = FALSE;
		}

		iPos++;
	}

	if (iNumCount == 0)
		bReturn = FALSE;

	return bReturn;
}

int COleDateTimeEx::CInt(LPCSTR lpszValue)
{
	return atoi(lpszValue);
}
// ***************************************************************************
// END String Support functions
// ***************************************************************************

BOOL COleDateTimeEx::IsLeapYear(int iYear)
{
	if (fmod(iYear, 4) == 0 && (fmod(iYear, 100) != 0 || fmod(iYear, 400) == 0))
		return TRUE;

	return FALSE;
}

BOOL COleDateTimeEx::IsOleDefaultDate(COleDateTime &dt)
{
	// if a ParseDateTime function is called and it cannot properly
	// convert the input text, sometimes (see MSDN article below) it
	// sets the date/time to December 30, 1899.  This is a problem which
	// we need to deal with
	// MSDN Article:
	// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/wcemfc/htm/coledati_15.asp
	if (dt.GetYear() == 1899 && dt.GetMonth() == 12 && dt.GetDay() == 30)
		return TRUE;
	return FALSE;
}

CString COleDateTimeEx::RemoveExtraWhiteSpace(LPCTSTR lpszInput)
{
	CString strReturn;
	LPCTSTR pszData = lpszInput;
	TCHAR cLastChar = 0;
	TCHAR cChar = 0;

	while (*pszData)
	{
		cChar = *pszData;

		if (cChar != ' ')
			strReturn += cChar;
		else if (cLastChar != ' ')
			strReturn += cChar;

		// record last character processed
		cLastChar = cChar;

		pszData++;
	}

	return strReturn;
}

CString COleDateTimeEx::ExpandShortcuts(LPCTSTR lpszInput)
{
	CString strReturn;
	LPCTSTR pszData = lpszInput;
	TCHAR cLastChar = 0;
	TCHAR cChar = 0;
	CString strTag;

	while (*pszData)
	{
		cChar = *pszData;

		if (!::isalnum(cChar))
		{
			strReturn += ExpandShortCut(strTag);
			strReturn += cChar;
			strTag = "";
		}
		else
		{
			strTag += cChar;
		}

		pszData++;
	}

	if (strTag != "")
		strReturn += ExpandShortCut(strTag);

	return strReturn;

}

CString COleDateTimeEx::ExpandShortCut(LPCTSTR lpszInput)
{
	CString strReturn;
	CString strExpanded;

	if (m_mapShortcuts.Lookup(lpszInput, strExpanded))
		strReturn = strExpanded;
	else
		strReturn = lpszInput;

	return strReturn;
}

// here I am hard-coding some very basic shortcuts.  Methods exist for
// clearing this list and creating a new one.  In the future I will provide
// a way (through UI) to customize these shortcuts
void COleDateTimeEx::LoadDefaultShortcuts()
{
	m_mapShortcuts.SetAt("T", "TODAY");
	m_mapShortcuts.SetAt("N", "NOW");
	m_mapShortcuts.SetAt("Y", "YESTERDAY");
	m_mapShortcuts.SetAt("TM", "TOMORROW");

	m_mapShortcuts.SetAt("NW", "NEXT WEEK");
	m_mapShortcuts.SetAt("NM", "NEXT MONTH");
	m_mapShortcuts.SetAt("N1", "NEXT SUNDAY");
	m_mapShortcuts.SetAt("N2", "NEXT MONDAY");
	m_mapShortcuts.SetAt("N3", "NEXT TUESDAY");
	m_mapShortcuts.SetAt("N4", "NEXT WEDNESDAY");
	m_mapShortcuts.SetAt("N5", "NEXT THURSDAY");
	m_mapShortcuts.SetAt("N6", "NEXT FRIDAY");
	m_mapShortcuts.SetAt("N7", "NEXT SATURDAY");

	m_mapShortcuts.SetAt("LW", "LAST WEEK");
	m_mapShortcuts.SetAt("LM", "LAST MONTH");
	m_mapShortcuts.SetAt("L1", "LAST SUNDAY");
	m_mapShortcuts.SetAt("L2", "LAST MONDAY");
	m_mapShortcuts.SetAt("L3", "LAST TUESDAY");
	m_mapShortcuts.SetAt("L4", "LAST WEDNESDAY");
	m_mapShortcuts.SetAt("L5", "LAST THURSDAY");
	m_mapShortcuts.SetAt("L6", "LAST FRIDAY");
	m_mapShortcuts.SetAt("L7", "LAST SATURDAY");

	m_mapShortcuts.SetAt("DY", "DAY BEFORE YESTERDAY");
	m_mapShortcuts.SetAt("DT", "DAY AFTER TOMORROW");

	m_mapShortcuts.SetAt("X", "CHRISTMAS");
	m_mapShortcuts.SetAt("XE", "CHRISTMAS EVE");
	m_mapShortcuts.SetAt("E", "EASTER");
	m_mapShortcuts.SetAt("GF", "GOOD FRIDAY");
	m_mapShortcuts.SetAt("AW", "ASH WEDNESDAY");
	m_mapShortcuts.SetAt("H", "HALLOWEEN");
	m_mapShortcuts.SetAt("NY", "NEW YEARS");
	m_mapShortcuts.SetAt("NYE", "NEW YEARS EVE");
	m_mapShortcuts.SetAt("J4", "JULY 4TH");
	m_mapShortcuts.SetAt("MD", "MEMORIAL DAY");
	m_mapShortcuts.SetAt("LD", "LABOR DAY");
	m_mapShortcuts.SetAt("VD", "VETERANS DAY");
	m_mapShortcuts.SetAt("CD", "COLUMBUS DAY");
	m_mapShortcuts.SetAt("TH", "THANKSGIVING");
	m_mapShortcuts.SetAt("PD", "PRESIDENTS DAY");
}

BOOL COleDateTimeEx::ComputeDateFromWord(COleDateTimeEx &dt, CString &strInput, int iYear)
{
	BOOL bReturn = FALSE;
	COleDateTime dtTemp;

	// deal with year windowing
	dtTemp.SetDate(iYear, 1, 1);

	if (dtTemp.m_status == COleDateTime::valid)
	{
		// reset year from dtTemp (this takes care of windowing situations like 96 = 1996
		iYear = dtTemp.GetYear();

		// we always start with the current date/time
		COleDateTime dtNow = COleDateTime::GetCurrentTime();
		dt.m_dt = dtNow.m_dt;
		dt.m_status = dtNow.m_status;

		if (stricmp(strInput, "christmas") == 0 || stricmp(strInput, "xmas") == 0 || stricmp(strInput, "x-mas") == 0 || stricmp(strInput, "x-mass") == 0 || stricmp(strInput, "xmass") == 0 || stricmp(strInput, "x mass") == 0)
		{
			dt.SetDate(iYear, 12, 25);
			bReturn = TRUE;
		}
		else if (stricmp(strInput, "christmas eve") == 0 || stricmp(strInput, "xmas eve") == 0 || stricmp(strInput, "x-mas eve") == 0 || stricmp(strInput, "x-mass eve") == 0 || stricmp(strInput, "xmass eve") == 0 || stricmp(strInput, "x mass eve") == 0)
		{
			dt.SetDate(iYear, 12, 24);
			bReturn = TRUE;
		}
		else if (stricmp(strInput, "new years") == 0 || stricmp(strInput, "new years day") == 0)
		{
			dt.SetDate(iYear, 1, 1);
			bReturn = TRUE;
		}
		else if (stricmp(strInput, "new years eve") == 0 || stricmp(strInput, "new years eve day") == 0)
		{
			dt.SetDate(iYear, 12, 31);
			bReturn = TRUE;
		}
		else if (stricmp(strInput, "halloween") == 0)
		{
			dt.SetDate(iYear, 10, 31);
			bReturn = TRUE;
		}
		else if (stricmp(strInput, "July 4th") == 0 || stricmp(strInput, "July fourth") == 0 || stricmp(strInput, "July forth") == 0 || stricmp(strInput, "4th of July") == 0 || stricmp(strInput, "fourth of July") == 0 || stricmp(strInput, "forth of July") == 0 || stricmp(strInput, "independence day") == 0)
		{
			// set to 7/4 this year
			dt.SetDate(iYear, 7, 4);
			bReturn = TRUE;
		}
		else if (stricmp(strInput, "easter") == 0 || stricmp(strInput, "easter sunday") == 0)
		{
			WORD wMonth = 0;
			WORD wDay = 0;

			CalcEasterSunday((WORD)iYear, wMonth, wDay);
			dt.SetDate(iYear, wMonth, wDay);
			bReturn = TRUE;
		}
		else if (stricmp(strInput, "good friday") == 0)
		{
			WORD wMonth = 0;
			WORD wDay = 0;

			CalcEasterSunday((WORD)iYear, wMonth, wDay);
			dt.SetDate(dt.GetYear(), wMonth, wDay);

			// good friday occurs 2 days before easter sunday
			dt -= 2;

			bReturn = TRUE;
		}
		else if (stricmp(strInput, "ash wednesday") == 0)
		{
			WORD wMonth = 0;
			WORD wDay = 0;

			CalcEasterSunday((WORD)iYear, wMonth, wDay);
			dt.SetDate(iYear, wMonth, wDay);
		
			// ash wednesday occurs 46 days before easte
			dt -= 46;

			bReturn = TRUE;
		}
		else if (stricmp(strInput, "veterans day") == 0 || stricmp(strInput, "veteran day") == 0)
		{
			dt.SetDate(iYear, 11, 11);

			bReturn = TRUE;
		}
		else if (stricmp(strInput, "memorial day") == 0)
		{
			// last monday in may
			// try the 6th, 5th, 4th and 3rd mondays
			dt.SetStatus(COleDateTime::invalid);
			dt.ComputeDayInMonth(6, 2, 5, iYear);
			if (dt.m_status != COleDateTime::valid)
				dt.ComputeDayInMonth(5, 2, 5, iYear);
			if (dt.m_status != COleDateTime::valid)
				dt.ComputeDayInMonth(4, 2, 5, iYear);
			if (dt.m_status != COleDateTime::valid)
				dt.ComputeDayInMonth(3, 2, 5, iYear);

			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
		else if (stricmp(strInput, "labor day") == 0)
		{
			// first monday in in september
			dt.ComputeDayInMonth(1, 2, 9, iYear);

			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
		else if (stricmp(strInput, "columbus day") == 0)
		{
			// second monday in october
			dt.ComputeDayInMonth(2, 2, 10, iYear);

			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
		else if (stricmp(strInput, "thanksgiving") == 0 || stricmp(strInput, "thanksgiving day") == 0)
		{
			// 4th thursday in november
			dt.ComputeDayInMonth(4, 5, 11, iYear);

			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
		else if (stricmp(strInput, "Washington's Birthday") == 0 || stricmp(strInput, "Washingtons Birthday") == 0 || stricmp(strInput, "Washington Birthday") == 0 || stricmp(strInput, "presidents day") == 0 || stricmp(strInput, "president's day") == 0 || stricmp(strInput, "president day") == 0)
		{
			// third monday in february
			dt.ComputeDayInMonth(3, 2, 2, iYear);

			if (dt.m_status == COleDateTime::valid)
				bReturn = TRUE;
		}
	}

	return bReturn;
}

BOOL COleDateTimeEx::ComputeDayInMonth(int iCount, int iDayOfWeek, int iMonth, int iYear)
{
	BOOL bReturn = TRUE;
	BOOL bContinue = TRUE;
	int iActualCount = 0;
	COleDateTimeEx dtTemp;
	CString strStart = CStr(iMonth); strStart += "/"; strStart += "01/"; strStart += CStr(iYear);

	dtTemp.SetDate(iYear, iMonth, 1);

	while (bContinue && dtTemp.GetStatus() == COleDateTime::valid)
	{
		if (dtTemp.GetDayOfWeek() == iDayOfWeek)
		{
			iActualCount++;

			if (iActualCount == iCount)
			{
				m_dt = dtTemp.m_dt;
				m_status = dtTemp.m_status;
				bContinue = FALSE;
			}
		}


		dtTemp += 1;
	}

	return bReturn;
}

void COleDateTimeEx::SetHourOption(long lHourOptions)
{
	if (lHourOptions == OLEDATEEX_HOUR_DETECT_DEFAULT ||
		lHourOptions == OLEDATEEX_HOUR_DETECT_CURRENT_HOUR ||
		lHourOptions == OLEDATEEX_HOUR_DETECT_USE_CUTOFF ||
		lHourOptions == OLEDATEEX_HOUR_DETECT_LEAVE)
	{
		m_lHourOptions = lHourOptions;
	}
	else
	{
		ASSERT(FALSE);		// invalid parsing option
	}
}

long COleDateTimeEx::GetHourOption()
{
	return m_lHourOptions;
}

void COleDateTimeEx::SetHourThreshold(int lHourCutoff)
{
	if (lHourCutoff >= 1 && lHourCutoff <= 11)
	{
		m_lHourCutoff = lHourCutoff;
	}
	else
	{
		ASSERT(FALSE);		// invalid cutoff
	}
}

int COleDateTimeEx::GetHourThreshold()
{
	return m_lHourCutoff;
}

BOOL COleDateTimeEx::IsDayType(int iType)
{
	BOOL bReturn = FALSE;

	switch (iType)
	{
	case OLEDATEEX_DAY: {bReturn = TRUE; break;}
	case OLEDATEEX_WEEK_DAY: {bReturn = IsWeekDay(); break;}
	case OLEDATEEX_WEEKEND_DAY: {bReturn = IsWeekendDay(); break;}
	case OLEDATEEX_SUNDAY: {bReturn = IsSunday(); break;}
	case OLEDATEEX_MONDAY: {bReturn = IsMonday(); break;}
	case OLEDATEEX_TUESDAY: {bReturn = IsTuesday(); break;}
	case OLEDATEEX_WEDNESDAY: {bReturn = IsWednesday(); break;}
	case OLEDATEEX_THURSDAY: {bReturn = IsThursday(); break;}
	case OLEDATEEX_FRIDAY: {bReturn = IsFriday(); break;}
	case OLEDATEEX_SATURDAY: {bReturn = IsSaturday(); break;}
	default:
		{
			ASSERT(FALSE);	// invalid type specified
			break;
		}
	}

	return bReturn;
}

// this function is responsible for converting "A.M.", "A M", "A.M", "A M." and "AM." into "AM"
//														AND
//											   "P.M.", "P M", "P.M", "P M." and "PM." into "PM"
CString COleDateTimeEx::DealWithAMPMIssues(CString &strInput)
{
	CString strReturn = strInput;
	int iPos = -1;
	BOOL bStop = FALSE;

	strInput.MakeUpper();

	// first, lets look for AM variants
	iPos = strInput.Find("A.M.");
	if (iPos > 0)
	{
		CString strPart1 = strInput.Left(iPos);
		CString strPart2 = strInput.Mid(iPos+4);
		strPart1.TrimRight();
		strPart2.TrimLeft();

		strReturn = strPart1;
		strReturn += " AM ";
		strReturn += strPart2;
		bStop = TRUE;
	}

	if (!bStop)
	{
		iPos = strInput.Find("A M");
		if (iPos > 0)
		{
			CString strPart1 = strInput.Left(iPos);
			CString strPart2 = strInput.Mid(iPos+3);
			strPart1.TrimRight();
			strPart2.TrimLeft();

			strReturn = strPart1;
			strReturn += " AM ";
			strReturn += strPart2;
			bStop = TRUE;
		}
	}

	if (!bStop)
	{
		iPos = strInput.Find("A.M");
		if (iPos > 0)
		{
			CString strPart1 = strInput.Left(iPos);
			CString strPart2 = strInput.Mid(iPos+3);
			strPart1.TrimRight();
			strPart2.TrimLeft();

			strReturn = strPart1;
			strReturn += " AM ";
			strReturn += strPart2;
			bStop = TRUE;
		}
	}

	if (!bStop)
	{
		iPos = strInput.Find("A M.");
		if (iPos > 0)
		{
			CString strPart1 = strInput.Left(iPos);
			CString strPart2 = strInput.Mid(iPos+4);
			strPart1.TrimRight();
			strPart2.TrimLeft();

			strReturn = strPart1;
			strReturn += " AM ";
			strReturn += strPart2;
			bStop = TRUE;
		}
	}

	if (!bStop)
	{
		iPos = strInput.Find("AM.");
		if (iPos > 0)
		{
			CString strPart1 = strInput.Left(iPos);
			CString strPart2 = strInput.Mid(iPos+3);
			strPart1.TrimRight();
			strPart2.TrimLeft();

			strReturn = strPart1;
			strReturn += " AM ";
			strReturn += strPart2;
			bStop = TRUE;
		}
	}

	// now, look for PM variants
	if (!bStop)
	{
		iPos = strInput.Find("P.M.");
		if (iPos > 0)
		{
			CString strPart1 = strInput.Left(iPos);
			CString strPart2 = strInput.Mid(iPos+4);
			strPart1.TrimRight();
			strPart2.TrimLeft();

			strReturn = strPart1;
			strReturn += " PM ";
			strReturn += strPart2;
			bStop = TRUE;
		}
	}

	if (!bStop)
	{
		iPos = strInput.Find("P M");
		if (iPos > 0)
		{
			CString strPart1 = strInput.Left(iPos);
			CString strPart2 = strInput.Mid(iPos+3);
			strPart1.TrimRight();
			strPart2.TrimLeft();

			strReturn = strPart1;
			strReturn += " PM ";
			strReturn += strPart2;
			bStop = TRUE;
		}
	}

	if (!bStop)
	{
		iPos = strInput.Find("P.M");
		if (iPos > 0)
		{
			CString strPart1 = strInput.Left(iPos);
			CString strPart2 = strInput.Mid(iPos+3);
			strPart1.TrimRight();
			strPart2.TrimLeft();

			strReturn = strPart1;
			strReturn += " PM ";
			strReturn += strPart2;
			bStop = TRUE;
		}
	}

	if (!bStop)
	{
		iPos = strInput.Find("P M.");
		if (iPos > 0)
		{
			CString strPart1 = strInput.Left(iPos);
			CString strPart2 = strInput.Mid(iPos+4);
			strPart1.TrimRight();
			strPart2.TrimLeft();

			strReturn = strPart1;
			strReturn += " PM ";
			strReturn += strPart2;
			bStop = TRUE;
		}
	}

	if (!bStop)
	{
		iPos = strInput.Find("PM.");
		if (iPos > 0)
		{
			CString strPart1 = strInput.Left(iPos);
			CString strPart2 = strInput.Mid(iPos+3);
			strPart1.TrimRight();
			strPart2.TrimLeft();

			strReturn = strPart1;
			strReturn += " PM ";
			strReturn += strPart2;
			bStop = TRUE;
		}
	}


	return strReturn;
}
