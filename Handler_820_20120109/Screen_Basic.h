#if !defined(AFX_SCREEN_BASIC_H__7C0ED389_12B1_43C5_987C_C3D6162BF9D7__INCLUDED_)
#define AFX_SCREEN_BASIC_H__7C0ED389_12B1_43C5_987C_C3D6162BF9D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_Basic.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "Public_Function.h"
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
#include "ListBoxST.h"      // 칼라 아이콘 리스트 박스 클래스
#include "xpbutton.h"		// XP 칼라 버튼 클래스
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "FloatST.h"

/* ****************************************************************************** */
/* 설정 정보 로딩 및 저장 클래스                                                  */
/* ****************************************************************************** */
#include "MyBasicData.h"
/* ****************************************************************************** */
#include "GridControlAlg.h"
#include "Digit.h"			// 디지털 카운터 클래스
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
	CFont*			mp_basic_font;			// 폰트 정보 설정 변수
	GridControlAlg	m_p_grid;
	CMyBasicData    mcls_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	// **************************************************************************
	// 리스트 박스 컨트롤 정보 저장 클래스 변수 선언                             
	// **************************************************************************
	CString mstr_device_name[2];		// 선택된 디바이스 종류 저장 변수
	CString mstr_temp_device;			// 선택된 디바이스 종류 임시 저장 변수
	CString mstr_load_file;				// 로딩할 파일에 대한 [폴더]+[파일명] 저장 변수

//	int mn_device_name;					// 선택된 디바이스의 리스트 위치 저장 변수


	CString mstr_new_device;			// 새로 생성할 디바이스의 이름.

	CMyBasicData     mcls_m_basic;		// 기본 셋팅 정보 로딩 및 저장 클래스
	/* ************************************************************************** */
	/* 칼라 에디트 박스 클래스 변수 선언                                          */
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
