// MySqlAlg.cpp: implementation of the MySqlAlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "handler.h"
#include "Alg_Mysql.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAlg_Mysql Mysql;

MYSQL_RES *pRes;
MYSQL_ROW pRow;
MYSQL_FIELD *pFields;

CAlg_Mysql::CAlg_Mysql()
{
	// Mysql DB 정의
	int i;
	m_n_jamcode_table_cnt		= 6;
	m_str_jamcode_table_name[0]	= "no";
	m_str_jamcode_table_name[1]	= "START_DATE";
	m_str_jamcode_table_name[2]	= "END_DATE";
	m_str_jamcode_table_name[3]	= "JAMCODE";
	m_str_jamcode_table_name[4]	= "JAMCODE_DESCRIP";
	m_str_jamcode_table_name[5]	= "PROCESS_TIME";
	
	
	m_n_jamcode_table_num[0]	= 5;
	m_n_jamcode_table_num[1]	= 25;
	m_n_jamcode_table_num[2]	= 25;
	m_n_jamcode_table_num[3]	= 10;
	m_n_jamcode_table_num[4]	= 200;
	m_n_jamcode_table_num[5]	= 10;
	
	m_n_jamcode_table_type[0]	= ID_TYPE;
	m_n_jamcode_table_type[1]	= DATE_TYPE;
	m_n_jamcode_table_type[2]	= DATE_TYPE;
	m_n_jamcode_table_type[3]	= TEXT_TYPE;
	m_n_jamcode_table_type[4]	= TEXT_TYPE;
	m_n_jamcode_table_type[5]	= INT_TYPE;
	
	m_n_lot_table_cnt		= 7;
	m_str_lot_table_name[0]	= "no";
	m_str_lot_table_name[1]	= "DATE";
	m_str_lot_table_name[2]	= "LOT_ID";
	m_str_lot_table_name[3]	= "WORKER";
	m_str_lot_table_name[4]	= "CREM";
	m_str_lot_table_name[5]	= "TRAY_NO";
	m_str_lot_table_name[6]	= "COUNT";
	
	m_n_lot_table_num[0]	= 5;
	m_n_lot_table_num[1]	= 25;
	m_n_lot_table_num[2]	= 25;
	m_n_lot_table_num[3]	= 20;
	m_n_lot_table_num[4]	= 10;
	m_n_lot_table_num[5]	= 10;
	m_n_lot_table_num[6]	= 10;
	
	m_n_lot_table_type[0]	= ID_TYPE;
	m_n_lot_table_type[1]	= DATE_TYPE;
	m_n_lot_table_type[2]	= TEXT_TYPE;
	m_n_lot_table_type[3]	= TEXT_TYPE;
	m_n_lot_table_type[4]	= TEXT_TYPE;
	m_n_lot_table_type[5]	= INT_TYPE;
	m_n_lot_table_type[6]	= INT_TYPE;
	
	m_n_dvc_table_cnt		= 5;
	m_str_dvc_table_name[0]	= "no";
	m_str_dvc_table_name[1]	= "DATE";
	m_str_dvc_table_name[2]	= "LOT_ID";
	m_str_dvc_table_name[3]	= "TRAY_NO";
	m_str_dvc_table_name[4]	= "SERIAL";
	
	
	m_n_dvc_table_num[0]	= 5;
	m_n_dvc_table_num[1]	= 25;
	m_n_dvc_table_num[2]	= 25;
	m_n_dvc_table_num[3]	= 10;
	m_n_dvc_table_num[4]	= 25;
	
	m_n_dvc_table_type[0]	= ID_TYPE;
	m_n_dvc_table_type[1]	= DATE_TYPE;
	m_n_dvc_table_type[2]	= TEXT_TYPE;
	m_n_dvc_table_type[3]	= TEXT_TYPE;
	m_n_dvc_table_type[4]	= INT_TYPE;
	
	m_n_gms_table_cnt		= 7;
	m_str_gms_table_name[0]	= "no";
	m_str_gms_table_name[1]	= "DATE";
	m_str_gms_table_name[2]	= "Channel_1";
	m_str_gms_table_name[3]	= "Channel_2";
	m_str_gms_table_name[4]	= "Channel_3";
	m_str_gms_table_name[5]	= "Channel_4";
	m_str_gms_table_name[6]	= "Channel_5";
	
	m_n_gms_table_num[0]	= 5;
	m_n_gms_table_num[1]	= 25;
	m_n_gms_table_num[2]	= 10;
	m_n_gms_table_num[3]	= 10;
	m_n_gms_table_num[4]	= 10;
	m_n_gms_table_num[5]	= 10;
	m_n_gms_table_num[6]	= 10;
	
	m_n_gms_table_type[0]	= ID_TYPE;
	m_n_gms_table_type[1]	= DATE_TYPE;
	m_n_gms_table_type[2]	= DOUBLE_TYPE;
	m_n_gms_table_type[3]	= DOUBLE_TYPE;
	m_n_gms_table_type[4]	= DOUBLE_TYPE;
	m_n_gms_table_type[5]	= DOUBLE_TYPE;
	m_n_gms_table_type[6]	= DOUBLE_TYPE;

	m_n_worker_table_cnt		= 4;
	m_str_worker_table_name[0]	= "no";
	m_str_worker_table_name[1]	= "DATE";
	m_str_worker_table_name[2]	= "WORKER_NAME";
	m_str_worker_table_name[3]	= "WORKER_CREM";
	m_str_worker_table_name[4]	= "WORKER_NO";
	
	m_n_worker_table_num[0]	= 5;
	m_n_worker_table_num[1]	= 25;
	m_n_worker_table_num[2]	= 25;
	m_n_worker_table_num[3]	= 25;
	m_n_worker_table_num[4]	= 25;
	
	m_n_worker_table_type[0]	= ID_TYPE;
	m_n_worker_table_type[1]	= DATE_TYPE;
	m_n_worker_table_type[2]	= TEXT_TYPE;
	m_n_worker_table_type[3]	= TEXT_TYPE;
	m_n_worker_table_type[4]	= TEXT_TYPE;

	m_n_tray_table_cnt			= 105;
	m_str_tray_table_name[0]	= "TRAY_POS";
	m_str_tray_table_name[1]	= "DATE";
	m_str_tray_table_name[2]	= "LOT_ID";
	m_str_tray_table_name[3]	= "PART_NO";
	m_str_tray_table_name[4]	= "COUNT";
	
	m_n_tray_table_num[0]	= 25;
	m_n_tray_table_num[1]	= 25;
	m_n_tray_table_num[2]	= 25;
	m_n_tray_table_num[3]	= 25;
	m_n_tray_table_num[4]	= 25;

	m_n_tray_table_type[0]	= TEXT_TYPE;
	m_n_tray_table_type[1]	= DATE_TYPE;
	m_n_tray_table_type[2]	= TEXT_TYPE;
	m_n_tray_table_type[3]	= TEXT_TYPE;
	m_n_tray_table_type[4]	= TEXT_TYPE;

	for(i=0; i<100; i++)
	{
		m_str_tray_table_name[i+5].Format("DEVICE_%03d", i+1);

		m_n_tray_table_num[i+5]	= 25;

		m_n_tray_table_type[i+5]	= TEXT_TYPE;
	}
	
// 	if(MySql_Open("localhost", "root", "1111", 3306))	
// 	{
// 		WaitTime(0,1000);
// 		WaitTime(1,300);	
// 	}
	
	MySql_Close();
	
	m_pRes = NULL;
}

CAlg_Mysql::~CAlg_Mysql()
{
	if(m_strquery != NULL)
	{
		delete [] m_strquery;
		m_strquery = NULL;
	}
}

BOOL CAlg_Mysql::MySql_Open(CString hostname, CString username,  CString password, int port)
{
	// 데이터 베이스 열기
	mysql_init(&m_pMySql);
	
	if(!(mysql_real_connect(&m_pMySql, hostname, username, password, NULL,port,NULL,NULL)))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}

	// 한글 지원하기 위해
	if(mysql_query(&m_pMySql, "SET NAMES euckr"))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CAlg_Mysql::Database_Create(CString databasename)
{
	// 데이터 베이스 새로 추가하기 
	CString strQuery;

	strQuery = "CREATE DATABASE " + databasename;

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}

	return TRUE;
}

BOOL CAlg_Mysql::Database_Delete(CString databasename)
{
	// 데이터 베이스 삭제하기
	CString strQuery;
	
	strQuery = "DROP DATABASE " + databasename;
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}
	
	return TRUE;
}

/*
TYNYINT_TYPE			0
SMALLINT_TYPE			1
MEDIUMINT_TYPE			2
INT_TYPE				3
BIGINT_TYPE				4
FLOAT_TYPE				5
DOUBLE_TYPE				6
DATE_TYPE				7
DATETIME_TYPE			8
TIMESTAMP_TYPE			9 
TIME_TYPE				10
YEAR_TYPE				11
CHAR_TYPE				12
VARCHAR_TYPE			13
TINYBLOB_TYPE			14
TINYTEXT_TYPE			15
BLOB_TYPE				16
TEXT_TYPE				17
MEDIUMBLOB_TYPE			18
MEDIUMTEXT_TYPE			19
LONGBLOB_TYPE			20
LONGTEXT_TYPE			21
NUMBER_TYEP				22
*/

BOOL CAlg_Mysql::Table_Create(CString database, CString tablename, int itemnum, CString *itemname, int *itemtype, int *itemhundred)
{
	CString strQuery;
	CString strItem;
	CString strTmp;
	int i;

	strQuery = "USE " + database;

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}


	for(i=0; i<itemnum; i++)
	{
		switch(itemtype[i])
		{
			case ID_TYPE:
				strTmp.Format("%s INT PRIMARY KEY NOT NULL AUTO_INCREMENT", itemname[i]);
				strItem += strTmp;
				break;
			case INT_TYPE:
				strTmp.Format("%s INT", itemname[i]);
				strItem += strTmp;
				break;
			case FLOAT_TYPE:
				strTmp.Format("%s FLOAT", itemname[i]);
				strItem += strTmp;
				break;
			case DOUBLE_TYPE:
				strTmp.Format("%s DOUBLE", itemname[i]);
				strItem += strTmp;
				break;
			case DATE_TYPE:
				strTmp.Format("%s DATETIME", itemname[i]);
				strItem += strTmp;
				break;
			case DATEAUTO_TYPE:
				strTmp.Format("%s IMESTAMP", itemname[i]);
				strItem += strTmp;
				break;
			case TEXT_TYPE:
				strTmp.Format("%s VARCHAR(%d)", itemname[i], itemhundred[i]);
				strItem += strTmp;
				break;	
		}

		if((itemnum - 1) > i)
		{
			strItem += ",";
		}
	}
	strQuery.Format("CREATE TABLE %s(%s)", tablename, strItem);

	if(mysql_query(&m_pMySql, strQuery))
	{
		strTmp = mysql_error(&m_pMySql);

		if(strTmp == "Table 'automation_machine' already exists")
		{
			return TRUE;
		}
		else{
			AfxMessageBox(mysql_error(&m_pMySql));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CAlg_Mysql::Table_Delete(CString tablename)
{
	CString strQuery;
	
	strQuery = "DROP TABLE " + tablename;
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}

	return TRUE;
}

BOOL CAlg_Mysql::Record_Add(CString tablename, CString *itemdata)
{
	// 레코드 추가 
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strValue;
	CString strItem;
	CString strTmp;
	CString strQuery;
	CString strType;

	int i;
	int pos;
	int num;

	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}
	
	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);

		if(itemdata[i] != "")
		{
			strItem += Row[0];
			strTmp = Row[1];
			pos = strTmp.Find("(", 0);
			if(pos < 0)
			{
				pos = strTmp.GetLength();
			}
			strType = strTmp.Mid(0, pos);
			strType.MakeUpper();

			strTmp = "";
			if(strType == "INT")
			{
				strTmp += itemdata[i];
			}
			else if(strType == "FLOAT")
			{
				strTmp += itemdata[i];
			}
			else if(strType == "DOUBLE")
			{
				strTmp += itemdata[i];
			}
			else if(strType == "DATETIME")
			{
				strTmp.Format("'%s'", itemdata[i]);
			}
			else if(strType == "TIMESTAMP")
			{
				
			}
			else if(strType == "VARCHAR")
			{
				strTmp.Format("'%s'", itemdata[i]);
			}
			else
			{
				strTmp += itemdata[i];
			}

			strValue += strTmp;
			
			if((num - 1) > i)
			{
				strItem += ",";
				strValue += ",";
			}
		}
	}

	strQuery.Format("INSERT INTO %s(%s) VALUES(%s)", tablename, strItem, strValue);

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}
	
	mysql_free_result(Res);
	return TRUE;
}

void CAlg_Mysql::MySql_Close()
{
	mysql_close(&m_pMySql);
}

BOOL CAlg_Mysql::Database_Select(CString databasename)
{
	// 데이터 베이스 선택하기 
	if(mysql_select_db(&m_pMySql, databasename))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CAlg_Mysql::Record_Updata(CString tablename, CString search_item_1, CString search_data_1, CString search_item_2, CString search_data_2, CString change_item, CString data)
{
	CString strQuery;

	if(search_item_2 == "")
	{
		strQuery.Format("UPDATE %s SET %s='%s' where %s='%s'", tablename, change_item, data, search_item_1, search_data_1);
	}
	else
	{
		strQuery.Format("UPDATE %s SET %s='%s' where %s='%s' and %s='%s'", tablename, change_item, data, search_item_1, search_data_1, search_item_2, search_data_2);
	}

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CAlg_Mysql::Record_Int_Delete(CString tablename, CString itemname, CString range, int value[])
{
	CString strQuery;

	if(range == "<")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %d", tablename, itemname, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %d", tablename, itemname, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %d AND %s < %d", tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %d OR %s > %d", tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format("DELETE FROM %s WHERE %s = %d", tablename, itemname, value[0]);
	}
	

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Float_Delete(CString tablename, CString itemname, CString range, float value[])
{
	CString strQuery;
	
	if(range == "<")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %f", tablename, itemname, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %f", tablename, itemname, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %f AND %s < %f", tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %f OR %s > %f", tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format("DELETE FROM %s WHERE %s = %f", tablename, itemname, value[0]);
	}
	
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Double_Delete(CString tablename, CString itemname, CString range, double value[])
{
	CString strQuery;
	
	if(range == "<")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %f", tablename, itemname, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %f", tablename, itemname, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("DELETE FROM %s WHERE %s > %f AND %s < %f", tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("DELETE FROM %s WHERE %s < %f OR %s > %f", tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format("DELETE FROM %s WHERE %s = %f", tablename, itemname, value[0]);
	}
	
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Text_Delete(CString tablename, CString itemname, CString value)
{
	CString strQuery;

	strQuery.Format("DELETE FROM %s WHERE %s = '%s'", tablename, itemname, value);

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Date_Delete(CString tablename, CString itemname, CString range, COleDateTime time[])
{
	CString strQuery;
	CString strTmp[2];
	
	if(range == "<")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format("DELETE FROM %s WHERE %s < '%s'", tablename, itemname, strTmp[0]);
	}
	else if(range == ">")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format("DELETE FROM %s WHERE %s > '%s'", tablename, itemname, strTmp[0]);
	}
	else if(range == "<>")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format("DELETE FROM %s WHERE %s > '%s' AND %s < '%s'", tablename, itemname, strTmp[0], itemname, strTmp[1]);
	}
	else if(range == "><")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format("DELETE FROM %s WHERE %s < '%s' OR %s > '%s'", tablename, itemname, strTmp[0], itemname, strTmp[1]);
	}
	else
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format("DELETE FROM %s WHERE %s = '%s'", tablename, itemname, strTmp[0]);
	}

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}
	return TRUE;
}

long CAlg_Mysql::Record_Int_Search(CString tablename, CString itemsearch, CString range, int value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;

	stritem = "";

	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return 0;
	}
	
	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}

	if(range == "<")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %d", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %d", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %d AND %s < %d", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %d OR %s > %d", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s = %d", stritem, tablename, itemsearch, value[0]);
	}

	mysql_free_result(Res);

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}

	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery = new CString[m_nfind_cnt];

	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = "";
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}

		m_strquery[i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt = 0;

	return m_nfind_cnt;
}

long CAlg_Mysql::Record_Float_Search(CString tablename, CString itemsearch, CString range, float value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;

	stritem = "";
	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}
	
	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}
	
	if(range == "<")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %f", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %f", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %f AND %s < %f", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %f OR %s > %f", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s = %f", stritem, tablename, itemsearch, value[0]);
	}

	mysql_free_result(Res);

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}

	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery = new CString[m_nfind_cnt];

	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = "";
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}

		m_strquery[i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt = 0;

	return m_nfind_cnt;
}

long CAlg_Mysql::Record_Double_Search(CString tablename, CString itemsearch, CString range, double value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;

	stritem = "";
	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}

	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}

	//	SELECT name,age from guest where age <= 25 ;
	if(range == "<")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %f", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == ">")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %f", stritem, tablename, itemsearch, value[0]);
	}
	else if(range == "<>")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s > %f AND %s < %f", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else if(range == "><")
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s < %f OR %s > %f", stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format("SELECT %s FROM %s WHERE %s = %f", stritem, tablename, itemsearch, value[0]);
	}

	mysql_free_result(Res);

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}

	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery = new CString[m_nfind_cnt];

	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = "";
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}

		m_strquery[i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt = 0;

	return m_nfind_cnt;
}

long CAlg_Mysql::Record_Text_Search(CString tablename, CString itemsearch, CString value)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;

	stritem = "";
	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}
	
	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}

	strQuery.Format("SELECT %s FROM %s WHERE %s = '%s'", stritem, tablename, itemsearch, value);

	mysql_free_result(Res);

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));

		mysql_free_result(m_pRes);
		return m_nfind_cnt;
	}

	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery = new CString[m_nfind_cnt];

	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = "";
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}

		m_strquery[i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt = 0;

	return m_nfind_cnt;
}

long CAlg_Mysql::Record_Date_Search(CString tablename, CString itemsearch, CString range, COleDateTime time[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	int i, j;
	int num;
	m_nfind_cnt = 0;
	
	stritem = "";
	strQuery.Format("DESCRIBE %s", tablename);
	
	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return m_nfind_cnt;
	}
	
	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}

	if(range == "<")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strQuery.Format("SELECT %s FROM %s WHERE %s > '%s'", stritem, tablename, itemsearch, strTmp[0]);
	}
	else if(range == ">")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strQuery.Format("SELECT %s FROM %s WHERE %s < '%s'", stritem, tablename, itemsearch, strTmp[0]);
	}
	else if(range == "<>")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
													      time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format("SELECT %s FROM %s WHERE %s > '%s' AND %s < '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else if(range == "><")
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format("%04d/%02d/%02d %02d:%02d:%02d", time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format("SELECT %s FROM %s WHERE %s < '%s' OR %s > '%s'", stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else
	{
		strTmp[0].Format("%04d/%02d/%02d %02d:%02d:%02d", time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format("SELECT %s FROM %s WHERE %s = '%s'", stritem, tablename, itemsearch, strTmp[0]);
	}

	mysql_free_result(Res);

	if(mysql_query(&m_pMySql, strQuery))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		mysql_free_result(m_pRes);
		return m_nfind_cnt;
	}

	Res = mysql_store_result(&m_pMySql);
	m_nfind_cnt = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery = new CString[m_nfind_cnt];

	for(i=0; i<m_nfind_cnt; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = "";
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += ",";
		}

		m_strquery[i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt = 0;

	return m_nfind_cnt;
}

BOOL CAlg_Mysql::Table_Select(CString databasename, CString tablename)
{
	CString strQuery;
	int num, i;
	CString strTmp;
	BOOL find;

	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	if(mysql_query(&m_pMySql, "SHOW TABLES"))
	{
		AfxMessageBox(mysql_error(&m_pMySql));
		return FALSE;
	}

	Res = mysql_store_result(&m_pMySql);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	find = FALSE;
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		strTmp = Row[0];

		strTmp.MakeUpper();
		
		if(strTmp == tablename)
		{
			find = TRUE;
			break;
		}
	}

	mysql_free_result(Res);

	if(find)
	{
		if(mysql_select_db(&m_pMySql, tablename))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	return FALSE;
}

CString CAlg_Mysql::Record_Read()
{
	CString strTmp;
	
	strTmp = "";
	m_nread_cnt++;

	strTmp = m_strquery[m_nread_cnt-1];
	if(m_nread_cnt == m_nfind_cnt)
	{
		delete [] m_strquery;
		m_strquery = NULL;
	}
	return strTmp;
}

BOOL CAlg_Mysql::WaitTime(int mode, int time)
{
	CString strQuery;
	
	if(mode == 0)	
	{	
		strQuery.Format("set global max_connections=%d", time);
	}
	else	
	{	
		strQuery.Format("set wait_timeout=%d", time);	
	}
	
	
	
	if(mysql_query(&m_pMySql, strQuery))		
	{
		return FALSE;	
	}
	
	return TRUE;    
}

BOOL CAlg_Mysql::Record_Sort(int mode, CString tablename, CString itemsearch)
{
	CString strQuery;

	if(mode == 0)
	{
		strQuery.Format("SELECT * FROM %s ORDER BY %s ASC", tablename, itemsearch);
	}
	else
	{
		strQuery.Format("SELECT * FROM %s ORDER BY %s DESC", tablename, itemsearch);
	}

	if(mysql_query(&m_pMySql, strQuery))		
	{
		return FALSE;	
	}
	
	return TRUE;
}
