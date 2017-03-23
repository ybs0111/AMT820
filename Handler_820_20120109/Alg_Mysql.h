// MySqlAlg.h: interface for the MySqlAlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSQLALG_H__E843957F_0AF5_4BB5_A596_7A50140E00C7__INCLUDED_)
#define AFX_MYSQLALG_H__E843957F_0AF5_4BB5_A596_7A50140E00C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define ID_TYPE					0
#define INT_TYPE				1
#define FLOAT_TYPE				2
#define DOUBLE_TYPE				3
#define TEXT_TYPE				4
#define DATE_TYPE				5
#define DATEAUTO_TYPE			6
/*
#define TYNYINT_TYPE			0
#define SMALLINT_TYPE			1
#define MEDIUMINT_TYPE			2
#define INT_TYPE				3
#define BIGINT_TYPE				4
#define FLOAT_TYPE				5
#define DOUBLE_TYPE				6
#define DATE_TYPE				7
#define DATETIME_TYPE			8
#define TIMESTAMP_TYPE			9 
#define TIME_TYPE				10
#define YEAR_TYPE				11
#define CHAR_TYPE				12
#define VARCHAR_TYPE			13
#define TINYBLOB_TYPE			14
#define TINYTEXT_TYPE			15
#define BLOB_TYPE				16
#define TEXT_TYPE				17
#define MEDIUMBLOB_TYPE			18
#define MEDIUMTEXT_TYPE			19
#define LONGBLOB_TYPE			20
#define LONGTEXT_TYPE			21
#define NUMBER_TYEP				22
*/
#include "mysql.h"
#include "Ctlbd_Variable.h"	// Added by ClassView
class CAlg_Mysql  
{
public:
	CString			*m_strquery;

	CString			m_str_jamcode_table_name[6];
	int				m_n_jamcode_table_cnt;
	int				m_n_jamcode_table_num[6];
	int				m_n_jamcode_table_type[6];
	
	CString			m_str_lot_table_name[7];
	int				m_n_lot_table_cnt;
	int				m_n_lot_table_num[7];
	int				m_n_lot_table_type[7];
	
	CString			m_str_dvc_table_name[5];
	int				m_n_dvc_table_cnt;
	int				m_n_dvc_table_num[5];
	int				m_n_dvc_table_type[5];
	
	CString			m_str_gms_table_name[7];
	int				m_n_gms_table_cnt;
	int				m_n_gms_table_num[7];
	int				m_n_gms_table_type[7];

	CString			m_str_worker_table_name[5];
	int				m_n_worker_table_cnt;
	int				m_n_worker_table_num[5];
	int				m_n_worker_table_type[5];

	CString			m_str_tray_table_name[105];
	int				m_n_tray_table_cnt;
	int				m_n_tray_table_num[105];
	int				m_n_tray_table_type[105];

	long			m_nread_cnt;
	long			m_nfind_cnt;

	MYSQL m_pMySql;
	MYSQL_RES *m_pRes;
	MYSQL_ROW m_pRow;
	MYSQL_FIELD *m_pFields;
	unsigned int m_nHeaderNum;
	LONG64 m_nRecordNum;
public:
	BOOL Record_Sort(int mode, CString tablename, CString itemsearch);
	BOOL WaitTime(int mode, int time);
	CString Record_Read();
	BOOL Table_Select(CString databasename, CString tablename);
	long Record_Date_Search(CString tablename, CString itemsearch, CString range, COleDateTime time[]);
	long Record_Text_Search(CString tablename, CString itemsearch, CString value);
	long Record_Double_Search(CString tablename, CString itemsearch, CString range, double value[]);
	long Record_Float_Search(CString tablename, CString itemsearch, CString range, float value[]);
	long Record_Int_Search(CString tablename, CString itemsearch, CString range, int value[]);
	BOOL Record_Date_Delete(CString tablename, CString itemname, CString range, COleDateTime time[]);
	BOOL Record_Text_Delete(CString tablename, CString itemname, CString value);
	BOOL Record_Double_Delete(CString tablename, CString itemname, CString range, double value[]);
	BOOL Record_Float_Delete(CString tablename, CString itemname, CString range, float value[]);
	BOOL Record_Int_Delete(CString tablename, CString itemname, CString range, int value[]);
	BOOL Record_Updata(CString tablename, CString search_item_1, CString search_data_1, CString search_item_2, CString search_data_2, CString change_item, CString data);
	BOOL Database_Select(CString databasename);
	void MySql_Close();
	BOOL Record_Add(CString tablename, CString *itemdata);
	BOOL Table_Delete(CString tablename);
	BOOL Table_Create(CString databasename, CString tablename, int itemcount, CString *itemname, int *itemtype, int *itemhundred);
	BOOL Database_Delete(CString databasename);
	BOOL Database_Create(CString databasename);
	BOOL MySql_Open(CString hostname = "localhost", CString username = "root", CString password = "1234", int port = 3306);
	CAlg_Mysql();
	virtual ~CAlg_Mysql();

};

extern CAlg_Mysql Mysql;
#endif // !defined(AFX_MYSQLALG_H__E843957F_0AF5_4BB5_A596_7A50140E00C7__INCLUDED_)
