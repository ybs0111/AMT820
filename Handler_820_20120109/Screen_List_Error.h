#if !defined(AFX_SCREEN_LIST_ERROR_H__AFEDC7FC_862D_4DF4_8136_2759E2BE97E5__INCLUDED_)
#define AFX_SCREEN_LIST_ERROR_H__AFEDC7FC_862D_4DF4_8136_2759E2BE97E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_List_Error.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Error form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "Variable.h"			// ���� ���� ���� Ŭ���� �߰�
#include "Public_Function.h"	// ���� �Լ� ���� Ŭ���� �߰�
#include "EditEx.h"				// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "LedButton.h"			// LED ��ư Ŭ����
#include "XPGroupBox.h"			// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "SxLogFont.h"
#include "ListBoxST.h"			// Į�� ������ ����Ʈ �ڽ� Ŭ���� 
#include "MacButtons.h"
#include "GradientStatic.h"		// �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 

class CScreen_List_Error : public CFormView
{
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
public:
	CScreen_List_Error();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_List_Error)

// Form Data
public:
	void OnWork_List_Display_Selected_Day();
	void OnWork_List_Reset_AllItem();
	int OnWork_List_Selected_File_Name_Check();
	void OnWork_List_Selected_File_Load(CString str_folder, CString str_file);
	BOOL AddItem(int n_index, WIN32_FIND_DATA *pfd);
	void OnWork_List_Full_Path_Set(int n_selected);
	int OnWork_List_Init_List(LPCTSTR pszPath, int nMode);
	void OnWork_List_Create_ImageList();
	void OnWork_List_State_Change(int n_state);
	void OnWork_List_RadioButton_Set();
	void OnWork_List_EditBox_Set();
	void OnWork_List_Lable_Set();
	void OnWork_List_GroupBox_Set();
	//{{AFX_DATA(CScreen_List_Error)
	enum { IDD = IDD_SCREEN_LIST_ERROR };
	CMacButton	m_btn_work_file_refresh;
	CMacButton	m_btn_work_file_del;
	CGradientStatic	m_msg_work_file_name;
	CGradientStatic	m_msg_work_file;
	CXPGroupBox	m_group_work_type;
	CXPGroupBox	m_group_work_list;
	CListBoxST	m_list_error_file;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_List_Error)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_List_Error();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_List_Error)
	afx_msg void OnSelchangeListWorkFile();
	afx_msg void OnBtnWorkFileDel();
	afx_msg void OnBtnWorkFileRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_LIST_ERROR_H__AFEDC7FC_862D_4DF4_8136_2759E2BE97E5__INCLUDED_)
