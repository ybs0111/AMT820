#if !defined(AFX_SCREEN_LIST_ALARM_H__A2ACEC85_87A5_4EE6_A3F4_D765CACAADB5__INCLUDED_)
#define AFX_SCREEN_LIST_ALARM_H__A2ACEC85_87A5_4EE6_A3F4_D765CACAADB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_List_Alarm.h : header file
//
#include "Variable.h"			// ���� ���� ���� Ŭ���� �߰�
#include "Public_Function.h"	// ���� �Լ� ���� Ŭ���� �߰�
#include "EditEx.h"				// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "LedButton.h"			// LED ��ư Ŭ����
#include "XPGroupBox.h"			// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "SxLogFont.h"
#include "ListBoxST.h"			// Į�� ������ ����Ʈ �ڽ� Ŭ���� 
#include "MacButtons.h"
#include "GradientStatic.h"		// �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Alarm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_List_Alarm : public CFormView
{
public:
	CScreen_List_Alarm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_List_Alarm)

// Form Data
public:
	//{{AFX_DATA(CScreen_List_Alarm)
	enum { IDD = IDD_SCREEN_LIST_ALARM };
	CGradientStatic	m_msg_alarm_file_name;
	CLedButton	m_radio_alarm_month;
	CLedButton	m_radio_alarm_lot;
	CLedButton	m_radio_alarm_list;
	CLedButton	m_radio_alarm_day;
	CGradientStatic	m_msg_alarm_file;
	CMonthCalCtrl	m_monthcal_alarm;
	CListBoxST	m_list_alarm_file;
	CXPGroupBox	m_group_alarm_type;
	CXPGroupBox	m_group_alarm_list;
	CMacButton	m_btn_alarm_file_refresh;
	CMacButton	m_btn_alarm_file_del;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_alarm_list_font;			// ��Ʈ ���� ���� ���� 

	CImageList mp_alarm_image_list;		// �̹��� ����Ʈ ���� 

	CString mstr_alarm_file_name;		// ���õ� ���� �̸� ���� ���� ���� 

	/* ************************************************************************** */
	/* Į�� ����Ʈ �ڽ� Ŭ���� ���� ����                                          */
	/* ************************************************************************** */
	CEditEx  m_edit_alarm_content;
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* �˶� ����Ʈ ���� ��� �� ���� ���� ���� ���� Ŭ���� ���� ����              */
	/* ************************************************************************** */
	int mn_type_select;					// ������ ã�� ��� ���� ���� ���� 

	CString mstr_default_path;			// �ʱ⿡ ���� �ε��� ���� ���� ���� ���� 
	CString mstr_full_path;				// ���� �ε��� [����]+[���ϸ�]+[Ȯ����] ���� ���� ���� 

	CString mstr_selected_Date;			// ���� ������ ��¥ ���� ���� ���� ���� (��+��+��/��+��/��+��+��)

	CString mstr_temp_file;				// ���� �̸� �ӽ� ���� ���� 
	/* ************************************************************************** */

// Operations
public:
	void OnAlarm_List_Display_Selected_Day();
	void OnAlarm_List_Reset_AllItem();
	int OnAlarm_List_Selected_File_Name_Check();
	void OnAlarm_List_Selected_File_Load(CString str_folder, CString str_file);
	BOOL AddItem(int n_index, WIN32_FIND_DATA *pfd);
	void OnAlarm_List_Full_Path_Set(int n_selected);
	int OnAlarm_List_Init_List(LPCTSTR pszPath);
	void OnAlarm_List_Create_ImageList();
	void OnAlarm_List_State_Change(int n_state);
	void OnAlarm_List_RadioButton_Set();
	void OnAlarm_List_EditBox_Set();
	void OnAlarm_List_Lable_Set();
	void OnAlarm_List_GroupBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_List_Alarm)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_List_Alarm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_List_Alarm)
	afx_msg void OnRadioAlarmLot();
	afx_msg void OnRadioAlarmDay();
	afx_msg void OnRadioAlarmMonth();
	afx_msg void OnRadioAlarmList();
	afx_msg void OnBtnAlarmFileDel();
	afx_msg void OnBtnAlarmFileRefresh();
	afx_msg void OnSelectMonthcalAlarm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeListAlarmFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_LIST_ALARM_H__A2ACEC85_87A5_4EE6_A3F4_D765CACAADB5__INCLUDED_)
