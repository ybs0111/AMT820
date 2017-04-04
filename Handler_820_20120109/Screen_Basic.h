#if !defined(AFX_SCREEN_BASIC_H__7C0ED389_12B1_43C5_987C_C3D6162BF9D7__INCLUDED_)
#define AFX_SCREEN_BASIC_H__7C0ED389_12B1_43C5_987C_C3D6162BF9D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Basic.h : header file
//
#include "Variable.h"		// ���� ���� ���� Ŭ���� �߰� 
#include "Public_Function.h"
#include "NewLabel.h"		// Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "editex.h"			// Į�� ����Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "BtnST.h"          // Į�� ��ư ���� Ŭ���� �߰� 
#include "XPGroupBox.h"		// Į�� �׷� �ڽ� ���� Ŭ���� �߰� 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "ListBoxST.h"      // Į�� ������ ����Ʈ �ڽ� Ŭ����
#include "xpbutton.h"		// XP Į�� ��ư Ŭ����
#include "EXDigitST.h"		// ������ ī���� Ŭ����
#include "GradientStatic.h" // �׶��̼� Į�� �ؽ�Ʈ �ڽ� ���� Ŭ���� �߰� 
#include "FloatST.h"

/* ****************************************************************************** */
/* ���� ���� �ε� �� ���� Ŭ����                                                  */
/* ****************************************************************************** */
#include "MyBasicData.h"
/* ****************************************************************************** */
#include "GridControlAlg.h"
#include "Digit.h"			// ������ ī���� Ŭ����
/////////////////////////////////////////////////////////////////////////////
// CScreen_Basic form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_Basic : public CFormView
{
public:
	CScreen_Basic();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_Basic)

// Form Data
public:
	//{{AFX_DATA(CScreen_Basic)
	enum { IDD = IDD_SCREEN_BASIC };
	CButtonST	m_btn_delete;
	CButtonST	m_btn_create;
	CXPGroupBox	m_group_operate_method;
	CXPGroupBox	m_group_model;
	CMacButton	m_btn_cancel;
	CMacButton	m_btn_apply;
	//}}AFX_DATA

// Attributes
public:
	int				m_n_filename_pos;
	int				m_n_operate_cnt[3];
	int				m_n_operate_used[3];
	int				m_n_mode_interface[2];
	int				m_n_mode_device[2];
	int				m_n_mode_retest[2];
	int				m_n_count_retry[2];
	int				mn_device_name;

	TSpread			*m_grid_operate;
	TSpread			*m_grid_file;
	CFont*			mp_basic_font;			// ��Ʈ ���� ���� ����
	GridControlAlg	m_p_grid;
	CMyBasicData    mcls_basic;		// �⺻ ���� ���� �ε� �� ���� Ŭ����
	// **************************************************************************
	// ����Ʈ �ڽ� ��Ʈ�� ���� ���� Ŭ���� ���� ����                             
	// **************************************************************************
	CString mstr_device_name[2];		// ���õ� ����̽� ���� ���� ����
	CString mstr_temp_device;			// ���õ� ����̽� ���� �ӽ� ���� ����
	CString mstr_load_file;				// �ε��� ���Ͽ� ���� [����]+[���ϸ�] ���� ����

//	int mn_device_name;					// ���õ� ����̽��� ����Ʈ ��ġ ���� ����


	CString mstr_new_device;			// ���� ������ ����̽��� �̸�.

	CMyBasicData     mcls_m_basic;		// �⺻ ���� ���� �ε� �� ���� Ŭ����
	/* ************************************************************************** */
	/* Į�� ����Ʈ �ڽ� Ŭ���� ���� ����                                          */
	/* ************************************************************************** */

	CEditEx	 m_edit_device_type;

// Operations
public:
	void Init_Grid_Operate();
	void Display_File(int n_mode, int n_count, CString str_filename);
	void Init_Grid_File();
	void OnBasic_Data_Display();
	void Init_Reject();
	void Init_Pass();
	void OnBasic_Data_HisoryLog();
	int OnBasic_Input_Device_Check(CString str_device);
	int OnBasic_New_Device_Check(CString str_device);
	BOOL AddItem(int nIndex, WIN32_FIND_DATA *pfd);
	int OnBasic_Init_List(LPCTSTR pszPath);
	void OnBasic_Device_Display(CString str_dvc);
	void Data_Recovery();
	int Data_Comparison();
	void OnBasic_Data_Apply();
	void OnBasic_Data_Backup();
	void OnBasic_Data_Label();
    void OnBasic_Device_Focus_Set(); //kwlee 2017.0403
	void OnBasic_Data_Set();
	void OnBasic_Digital();
	void OnBasic_Data_Group();
	void OnBasic_Data_Button();
	void OnBasic_Data_EditBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_Basic)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_Basic();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_Basic)
	afx_msg void OnDestroy();
	afx_msg void OnBtnBasicCancel();
	afx_msg void OnBtnBasicApply();
	afx_msg void OnBtnBasicCreate();
	afx_msg void OnBtnDelete();
	//}}AFX_MSG
	afx_msg void OnCell_L_Click(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCell_R_Click(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_BASIC_H__7C0ED389_12B1_43C5_987C_C3D6162BF9D7__INCLUDED_)
