#if !defined(AFX_SCREEN_LIST_WORK_H__E716E070_F04E_4567_BBE0_97A31513982E__INCLUDED_)
#define AFX_SCREEN_LIST_WORK_H__E716E070_F04E_4567_BBE0_97A31513982E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_List_Work.h : header file
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
// CScreen_List_Work form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_List_Work : public CFormView
{
public:
	CScreen_List_Work();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_List_Work)

// Form Data
public:
	//{{AFX_DATA(CScreen_List_Work)
	enum { IDD = IDD_SCREEN_LIST_WORK };
	CLedButton	m_radio_work_month;
	CLedButton	m_radio_work_lot;
	CLedButton	m_radio_work_list;
	CLedButton	m_radio_work_day;
	CGradientStatic	m_msg_work_file_name;
	CGradientStatic	m_msg_work_file;
	CMonthCalCtrl	m_monthcal_work;
	CListBoxST	m_list_work_file;
	CXPGroupBox	m_group_work_type;
	CXPGroupBox	m_group_work_list;
	CMacButton	m_btn_work_file_refresh;
	CMacButton	m_btn_work_file_del;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_work_list_font;			// ��Ʈ ���� ���� ���� 

	CImageList mp_work_image_list;		// �̹��� ����Ʈ ���� 

	CString mstr_work_file_name;		// ���õ� ���� �̸� ���� ���� ���� 

	/* ************************************************************************** */
	/* Į�� ����Ʈ �ڽ� Ŭ���� ���� ����                                          */
	/* ************************************************************************** */
	CEditEx  m_edit_work_content;
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
	void OnWork_List_Display_Selected_Day();
	void OnWork_List_Reset_AllItem();
	int OnWork_List_Selected_File_Name_Check();
	void OnWork_List_Selected_File_Load(CString str_folder, CString str_file);
	BOOL AddItem(int n_index, WIN32_FIND_DATA *pfd);
	void OnWork_List_Full_Path_Set(int n_selected);
	int OnWork_List_Init_List(LPCTSTR pszPath);
	void OnWork_List_Create_ImageList();
	void OnWork_List_State_Change(int n_state);
	void OnWork_List_RadioButton_Set();
	void OnWork_List_EditBox_Set();
	void OnWork_List_Lable_Set();
	void OnWork_List_GroupBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_List_Work)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_List_Work();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_List_Work)
	afx_msg void OnRadioWorkLot();
	afx_msg void OnRadioWorkDay();
	afx_msg void OnRadioWorkMonth();
	afx_msg void OnRadioWorkList();
	afx_msg void OnBtnWorkFileDel();
	afx_msg void OnBtnWorkFileRefresh();
	afx_msg void OnSelchangeMonthcalWork(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeListWorkFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_LIST_WORK_H__E716E070_F04E_4567_BBE0_97A31513982E__INCLUDED_)
