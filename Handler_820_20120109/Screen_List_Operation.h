#if !defined(AFX_SCREEN_LIST_OPERATION_H__F7709F14_7D5E_4FD2_B238_20A5C1D78821__INCLUDED_)
#define AFX_SCREEN_LIST_OPERATION_H__F7709F14_7D5E_4FD2_B238_20A5C1D78821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_List_Operation.h : header file
//
#include "Variable.h"			// 전역 변수 정의 클래스 추가
#include "Public_Function.h"	// 공용 함수 정의 클래스 추가
#include "EditEx.h"				// 칼라 에디트 박스 생성 클래스 추가 
#include "LedButton.h"			// LED 버튼 클래스
#include "XPGroupBox.h"			// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "ListBoxST.h"			// 칼라 아이콘 리스트 박스 클래스 
#include "MacButtons.h"
#include "GradientStatic.h"		// 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Operation form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_List_Operation : public CFormView
{
public:
	CScreen_List_Operation();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_List_Operation)

// Form Data
public:
	//{{AFX_DATA(CScreen_List_Operation)
	enum { IDD = IDD_SCREEN_LIST_OPERATION };
	CLedButton	m_radio_operation;
	CLedButton	m_radio_machine;
	CLedButton	m_radio_interface;
	CLedButton	m_radio_barcode;
	CGradientStatic	m_msg_operation_file_name;
	CGradientStatic	m_msg_operation_file;
	CMonthCalCtrl	m_monthcal_operation;
	CListBoxST	m_list_operation_file;
	CXPGroupBox	m_group_operation_type;
	CXPGroupBox	m_group_operation_list;
	CMacButton	m_btn_operation_file_refresh;
	CMacButton	m_btn_operation_file_del;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_operation_list_font;			// 폰트 설정 변수 선언 

	CImageList mp_operation_image_list;		// 이미지 리스트 변수 

	CString mstr_operation_file_name;		// 선택된 파일 이름 정보 저장 변수 

	/* ************************************************************************** */
	/* 칼라 에디트 박스 클래스 변수 선언                                          */
	/* ************************************************************************** */
	CEditEx  m_edit_operation_content;
	/* ************************************************************************** */
	
	/* ************************************************************************** */
	/* 알람 리스트 파일 경로 및 파일 종류 정보 저장 클래스 변수 선언              */
	/* ************************************************************************** */
	int mn_type_select;					// 파일을 찾을 경로 정보 저장 변수 

	CString mstr_default_path;			// 초기에 파일 로딩할 폴더 정보 저장 변수 
	CString mstr_full_path;				// 파일 로딩할 [폴더]+[파일명]+[확장자] 정보 저장 변수 

	CString mstr_selected_Date;			// 현재 선택한 날짜 정보 저장 변수 선언 (년+월+일/년+월/년+월+주)

	CString mstr_temp_file;				// 파일 이름 임시 저장 변수 
	/* ************************************************************************** */

// Operations
public:
	void OnOperation_List_Display_Selected_Day();
	void OnOperation_List_Reset_AllItem();
	int OnOperation_List_Selected_File_Name_Check();
	void OnOperation_List_Selected_File_Load(CString str_folder, CString str_file);
	BOOL AddItem(int n_index, WIN32_FIND_DATA *pfd);
	void OnOperation_List_Full_Path_Set(int n_selected);
	int OnOperation_List_Init_List(LPCTSTR pszPath);
	void OnOperation_List_Create_ImageList();
	void OnOperation_List_State_Change(int n_state);
	void OnOperation_List_RadioButton_Set();
	void OnOperation_List_EditBox_Set();
	void OnOperation_List_Lable_Set();
	void OnOperation_List_GroupBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_List_Operation)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_List_Operation();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_List_Operation)
	afx_msg void OnSelchangeListOperationFile();
	afx_msg void OnSelectMonthcalOperation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadioOperation();
	afx_msg void OnRadioInterface();
	afx_msg void OnRadioMachine();
	afx_msg void OnRadioBarcode();
	afx_msg void OnBtnOperationFileDel();
	afx_msg void OnBtnOperationFileRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_LIST_OPERATION_H__F7709F14_7D5E_4FD2_B238_20A5C1D78821__INCLUDED_)
