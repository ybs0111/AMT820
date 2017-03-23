#if !defined(AFX_SCREEN_LIST_STEP_H__801DB059_9152_4298_BDEC_105AD9743B11__INCLUDED_)
#define AFX_SCREEN_LIST_STEP_H__801DB059_9152_4298_BDEC_105AD9743B11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Screen_List_Step.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "xpbutton.h"		// XP 칼라 버튼 클래스 */
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 

/* ****************************************************************************** */
/* 쓰레드 스텝 정보 저장 및 로딩 클래스 추가                                      */
/* ****************************************************************************** */
#include "MyBasicData.h"
/* ****************************************************************************** */

/* ****************************************************************************** */
/* 스텝 정보 출력 타이머 선언                                                     */
/* ****************************************************************************** */
#define TMR_STEP	    862
/* ****************************************************************************** */
/////////////////////////////////////////////////////////////////////////////
// CScreen_List_Step form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen_List_Step : public CFormView
{
public:
	CScreen_List_Step();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScreen_List_Step)

// Form Data
public:
	//{{AFX_DATA(CScreen_List_Step)
	enum { IDD = IDD_SCREEN_LIST_STEP };
	CNewLabel	m_msg_thread_11;
	CGradientStatic	m_data_thread_11;
	CNewLabel	m_msg_thread_9;
	CNewLabel	m_msg_thread_8;
	CNewLabel	m_msg_thread_7;
	CNewLabel	m_msg_thread_10;
	CGradientStatic	m_data_thread_9;
	CGradientStatic	m_data_thread_8;
	CGradientStatic	m_data_thread_7;
	CGradientStatic	m_data_thread_10;
	CNewLabel	m_msg_thread_6;
	CNewLabel	m_msg_thread_5;
	CNewLabel	m_msg_thread_4;
	CNewLabel	m_msg_thread_3;
	CNewLabel	m_msg_thread_2;
	CNewLabel	m_msg_thread_1;
	CXPGroupBox	m_group_step_list;
	CGradientStatic	m_data_thread_6;
	CGradientStatic	m_data_thread_5;
	CGradientStatic	m_data_thread_4;
	CGradientStatic	m_data_thread_3;
	CGradientStatic	m_data_thread_2;
	CGradientStatic	m_data_thread_1;
	CXPButton	m_btn_step_save;
	//}}AFX_DATA

// Attributes
public:
	CFont* mp_step_list_font;  // 폰트 설정 변수 선언 

	/* ************************************************************************** */
	/* 생성할 스텝 저장 파일명 및 인덱스 로딩/저장 클래스 변수                    */
	/* ************************************************************************** */
	CMyBasicData mcls_file;   
	/* ************************************************************************** */

	int mn_timer_kill;  // 타이머 삭제 요청 플래그 

	/* ************************************************************************** */
	/* 쓰레드 스텝 정보 저장 클래스 변수 선언                                     */
	/* ************************************************************************** */
	CString mstr_thread_1;	// 로더 엘레베이터 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_2;  // 로더 트렌스퍼 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_3;	// 언로더 엘레베이터 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_4;	// 언로더 트렌스퍼 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_5;	// XYZ 로봇 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_6;	// 테스트 사이트 제어 쓰레드 스텝 정보 저장 변수 
	CString mstr_thread_7;
	CString mstr_thread_8;
	CString mstr_thread_9;
	CString mstr_thread_10;
	CString mstr_thread_11;
	/* ************************************************************************** */

// Operations
public:
	void TextAndLine(int dir, CString str_sdata, CString str_data, FILE *fp);
	CString FileTextLine(int dir, int length);
	void On_Step_List_File_Info_Save(CString str_step_file, CString str_file_index);
	int On_Step_List_File_Search(CString str_chk_file);
	void On_Step_List_File_Create(CString str_step_file, CString str_create_file, CString str_time);
	void On_Step_List_File_Compare(CString str_folder, CString str_file_name, CString str_time_info);
	void On_Step_List_File_Name_Init();
	void OnList_Step_Data_Display();
	void OnList_Step_Data_Set();
	void OnList_Step_Lable_Set();
	void OnList_Step_GroupBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreen_List_Step)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScreen_List_Step();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScreen_List_Step)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnStepSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREEN_LIST_STEP_H__801DB059_9152_4298_BDEC_105AD9743B11__INCLUDED_)
