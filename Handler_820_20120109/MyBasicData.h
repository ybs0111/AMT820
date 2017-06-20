// MyBasicData.h: interface for the CMyBasicData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_)
#define AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Variable.h"  // 전역 변수 정의 클래스 추가 
#include "Public_Function.h"	// 모터 보드 및 I/O 보드 관련 클래스

class CMyBasicData  
{
public:
	CMyBasicData();
	virtual ~CMyBasicData();

// Attributes
public:

// Operations
public:
	CString GetWaitTimeName(int n_mode);
//	void On_Teach_Data_Save_As(CString str_device);
	void OnMaintenance_Data_Save_As(CString str_device);
	void OnInterface_Data_Save_As(CString str_device);
	void OnBasic_Data_Save_As(CString str_device);
	void OnInterface_Data_Save();
	void OnInterface_Data_Load();
	void OnDevice_Folder_Save();
	void OnDeviec_Folder_Load();
	CString OnGet_File_Name();
	void OnBasic_Data_Save();
	//void OnBasic_Data_Load();
	//kwlee 2017.0617
	void OnBasic_Data_Load(int nMode);
	void OnDaily_Count_Save();

	void On_Teach_Data_Save();
	void On_Teach_Data_Load();
	void OnMaintenance_Data_Save();
	void OnMaintenance_Data_Load();
	void OnWaitTime_Data_Save();
	void OnWaitTime_Data_Load();
	void OnMotorSpeed_Set_Data_Save();
	void OnMotorSpeed_Set_Data_Load();
	void OnStep_File_Name_Save(CString str_file);
	CString OnStep_File_Name_Load();
	void OnStep_File_Index_Save(CString str_index);
	CString OnStep_File_Index_Load();
	void OnPassWord_Save(CString str_save_level, CString str_pass_data);
	void OnPassWord_Load(CString str_load_level);
	CString OnGet_Teach_File_Name( CString strName, int iOpt = 0 );
};

#endif // !defined(AFX_MYBASICDATA_H__F10E96B3_4961_4A37_B43F_FDAD481A0494__INCLUDED_)
