// APartDatabase.cpp: implementation of the APartDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "APartDatabase.h"
#include "..\SrcBase\ACriticalSection.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
APartDatabase g_db;

APartDatabase::APartDatabase()
{
	m_st = ST_NONE;
}

APartDatabase::~APartDatabase()
{

}

void APartDatabase::ChangeSection( SECTION_TYPE st )
{
	m_dwOldTime = GetCurrentTime();
	m_st = st;
}

void APartDatabase::DB_Write_Time(st_time_database st_timedb)
{
	return;
/*	g_cs.EnterCriticalSection();

	CString strTmp[7];
	
	DWORD dwCurrentTime = GetCurrentTime();
	DWORD dwTime_Section = dwCurrentTime - m_dwOldTime;

	m_dwOldTime = dwCurrentTime;

	st_timedb.mnTime_Time = (int)dwTime_Section;
	
	strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", st_timedb.mole_date.GetYear(), 
		st_timedb.mole_date.GetMonth(),
		st_timedb.mole_date.GetDay(),
		st_timedb.mole_date.GetHour(),
		st_timedb.mole_date.GetMinute(),
		st_timedb.mole_date.GetSecond());
	
	strTmp[2].Format("%-1s", st_timedb.mstrlot_no);
	strTmp[3].Format("%-1s", st_timedb.mstrpart_no);	
	
	strTmp[4].Format("%d", st_timedb.mnTime_Section);
	strTmp[5].Format("%d", st_timedb.mnTime_Time);
	strTmp[6].Format("%d", st_timedb.mnMode);

	if (Mysql.MySql_Open("localhost", "root", "1111", 3306))
	{
		if (Mysql.Database_Select("AMT8520"))
		{	
			if (Mysql.Table_Select("AMT8520", "TIME_DATA"))
			{
				Mysql.Record_Add("TIME_DATA", strTmp);
			}
			else
			{
				if (Mysql.Table_Create("AMT8520", "TIME_DATA", 7, Mysql.mstr_time_table_name, Mysql.mn_time_table_type, Mysql.mn_time_table_num))
				{
					Mysql.Record_Add("TIME_DATA", strTmp);
				}
			}
		}
		else
		{
			if (Mysql.Database_Create("AMT8520"))
			{
				if (Mysql.Table_Create("AMT8520", "TIME_DATA", 7, Mysql.mstr_time_table_name, Mysql.mn_time_table_type, Mysql.mn_time_table_num))
				{
					Mysql.Record_Add("TIME_DATA", strTmp);
				}
			}
		}
	}
	
	Mysql.MySql_Close();

	g_cs.LeaveCriticalSection();*/
}