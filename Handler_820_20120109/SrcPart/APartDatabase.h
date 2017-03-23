// APartDatabase.h: interface for the APartDatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APARTDATABASE_H__88538F4D_5741_439F_8DA7_E24D37FE47C3__INCLUDED_)
#define AFX_APARTDATABASE_H__88538F4D_5741_439F_8DA7_E24D37FE47C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Variable.h"

enum SECTION_TYPE
{
	ST_NONE,
	ST_STOP,
};
// struct st_time_database
// {
// 	int				mndb;
// 	
// 	COleDateTime	mole_date;
// 	
// 	CString			mstrlot_no;
// 	CString			mstrpart_no;
// 	
// 	int				mnTime_Section;
// 	int				mnTime_Time;
// 	int				mnMode;
// 	
// 	int				mnSectionBuffer;
// };
// extern struct st_time_database st_timedb;

class APartDatabase  
{
public:
	APartDatabase();
	virtual ~APartDatabase();

public:
	void ChangeSection( SECTION_TYPE st );
	void DB_Write_Time(st_time_database st_timedb);


protected:
	
	DWORD			m_dwOldTime;
	SECTION_TYPE	m_st;
};

extern APartDatabase	g_db;
#endif // !defined(AFX_APARTDATABASE_H__88538F4D_5741_439F_8DA7_E24D37FE47C3__INCLUDED_)
