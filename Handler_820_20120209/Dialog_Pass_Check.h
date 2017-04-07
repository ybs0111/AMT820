#if !defined(AFX_DIALOG_PASS_CHECK_H__3DD1B306_8FEB_48E3_8324_5A6A7294BF61__INCLUDED_)
#define AFX_DIALOG_PASS_CHECK_H__3DD1B306_8FEB_48E3_8324_5A6A7294BF61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Pass_Check.h : header file
//

#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"

/* ****************************************************************************** */
/* 암호 정보 로딩 및 저장 클래스 추가                                             */
/* ****************************************************************************** */
#include "MyBasicData.h"           
/* ****************************************************************************** */

/////////////////////////////////////////////////////////////////////////////
// CDialog_Pass_Check dialog

class CDialog_Pass_Check : public StandardDialog
{
// Construction
public:
	void OnPassChk_Level_Set();
	BOOL OnPassChk_Length_Check();
	void OnPassChk_Pass_Data_Load();
	void OnPassChk_EditBox_Set();
	void OnPassChk_Lable_Set();
	void OnPassChk_GroupBox_Set();
	void OnPassChk_Button_Set();
	CDialog_Pass_Check(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Pass_Check)
	enum { IDD = IDD_DIALOG_PASS_CHECK };
	CMacButton	m_btn_pass_chk_add;
	CGradientStatic	m_title_pass_chk;
	CGradientStatic	m_msg_pass_chk;
	CXPGroupBox	m_group_pass_chk;
	CMacButton	m_btn_pass_chk_sharp;
	CMacButton	m_btn_pass_chk_rbrace_s;
	CMacButton	m_btn_pass_chk_rbrace_b;
	CMacButton	m_btn_pass_chk_line;
	CMacButton	m_btn_pass_chk_lbrace_s;
	CMacButton	m_btn_pass_chk_lbrace_b;
	CMacButton	m_btn_pass_chk_dot;
	CMacButton	m_btn_pass_chk_dollar;
	CMacButton	m_btn_pass_chk_div;
	CMacButton	m_btn_pass_chk_del;
	CMacButton	m_btn_pass_chk_back;
	CMacButton	m_btn_pass_chk_z;
	CMacButton	m_btn_pass_chk_y;
	CMacButton	m_btn_pass_chk_x;
	CMacButton	m_btn_pass_chk_w;
	CMacButton	m_btn_pass_chk_v;
	CMacButton	m_btn_pass_chk_u;
	CMacButton	m_btn_pass_chk_t;
	CMacButton	m_btn_pass_chk_s;
	CMacButton	m_btn_pass_chk_r;
	CMacButton	m_btn_pass_chk_q;
	CMacButton	m_btn_pass_chk_p;
	CMacButton	m_btn_pass_chk_o;
	CMacButton	m_btn_pass_chk_n;
	CMacButton	m_btn_pass_chk_m;
	CMacButton	m_btn_pass_chk_l;
	CMacButton	m_btn_pass_chk_k;
	CMacButton	m_btn_pass_chk_j;
	CMacButton	m_btn_pass_chk_i;
	CMacButton	m_btn_pass_chk_h;
	CMacButton	m_btn_pass_chk_g;
	CMacButton	m_btn_pass_chk_f;
	CMacButton	m_btn_pass_chk_e;
	CMacButton	m_btn_pass_chk_d;
	CMacButton	m_btn_pass_chk_c;
	CMacButton	m_btn_pass_chk_b;
	CMacButton	m_btn_pass_chk_a;
	CMacButton	m_btn_pass_chk_9;
	CMacButton	m_btn_pass_chk_8;
	CMacButton	m_btn_pass_chk_7;
	CMacButton	m_btn_pass_chk_6;
	CMacButton	m_btn_pass_chk_5;
	CMacButton	m_btn_pass_chk_4;
	CMacButton	m_btn_pass_chk_3;
	CMacButton	m_btn_pass_chk_2;
	CMacButton	m_btn_pass_chk_1;
	CMacButton	m_btn_pass_chk_0;
	//}}AFX_DATA

// Attributes
private:
	CFont* mp_pass_font;							// 폰트 설정 변수 선언 
	CButton *mp_pass_chk_ok, *mp_pass_chk_cancel;	// 버튼에 대한 포인터 선언

	/* ************************************************************************** */
	/* 암호 정보 로딩/저장 클래스 변수 선언                                       */
	/* ************************************************************************** */
	CMyBasicData mcls_pass_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 칼라 에디트 박스 클래스 변수 선언                                          */
	/* ************************************************************************** */
	CEditEx   m_edit_pass_chk;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 에디트 박스에 입력된 암호 체크 내부 변수 선언                              */
	/* ************************************************************************** */
	CString mstr_pass_chk;		// 에디트 박스에 입력된 암호 정보 저장 변수 
	CString mstr_temp_chk;		// 설정 암호 임시 저장 변수 

	CString mstr_level1_chk;	// 설정 암호 Level 1 임시 저장 변수 
	CString mstr_level2_chk;	// 설정 암호 Level 2 임시 저장 변수 
	
	int mn_cnt;  // 에디트 박스에 입력된 암호 길이 검사 정보 저장 변수 
	/* ************************************************************************** */

	int mn_pass_level;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Pass_Check)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Pass_Check)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnPassChkBack();
	afx_msg void OnBtnPassChkDel();
	afx_msg void OnPassChkLbraceS();
	afx_msg void OnPassChkRbraceS();
	afx_msg void OnPassChkDiv();
	afx_msg void OnPassChkLbraceB();
	afx_msg void OnPassChkRbraceB();
	afx_msg void OnPassChkDot();
	afx_msg void OnPassChkAdd();
	afx_msg void OnPassChkLine();
	afx_msg void OnPassChkDollar();
	afx_msg void OnPassChkSharp();
	afx_msg void OnBtnPassChk1();
	afx_msg void OnBtnPassChk2();
	afx_msg void OnBtnPassChk3();
	afx_msg void OnBtnPassChk4();
	afx_msg void OnBtnPassChk5();
	afx_msg void OnBtnPassChk6();
	afx_msg void OnBtnPassChk7();
	afx_msg void OnBtnPassChk8();
	afx_msg void OnBtnPassChk9();
	afx_msg void OnBtnPassChk0();
	afx_msg void OnBtnPassChkA();
	afx_msg void OnBtnPassChkB();
	afx_msg void OnBtnPassChkC();
	afx_msg void OnBtnPassChkD();
	afx_msg void OnBtnPassChkE();
	afx_msg void OnBtnPassChkF();
	afx_msg void OnBtnPassChkG();
	afx_msg void OnBtnPassChkH();
	afx_msg void OnBtnPassChkI();
	afx_msg void OnBtnPassChkJ();
	afx_msg void OnBtnPassChkK();
	afx_msg void OnBtnPassChkL();
	afx_msg void OnBtnPassChkM();
	afx_msg void OnBtnPassChkN();
	afx_msg void OnBtnPassChkO();
	afx_msg void OnBtnPassChkP();
	afx_msg void OnBtnPassChkQ();
	afx_msg void OnBtnPassChkR();
	afx_msg void OnBtnPassChkS();
	afx_msg void OnBtnPassChkT();
	afx_msg void OnBtnPassChkU();
	afx_msg void OnBtnPassChkV();
	afx_msg void OnBtnPassChkW();
	afx_msg void OnBtnPassChkX();
	afx_msg void OnBtnPassChkY();
	afx_msg void OnBtnPassChkZ();
	afx_msg void OnBtnPassChkLbraceS();
	afx_msg void OnBtnPassChkRbraceS();
	afx_msg void OnBtnPassChkDiv();
	afx_msg void OnBtnPassChkLbraceB();
	afx_msg void OnBtnPassChkRbraceB();
	afx_msg void OnBtnPassChkDot();
	afx_msg void OnBtnPassChkAdd();
	afx_msg void OnBtnPassChkLine();
	afx_msg void OnBtnPassChkDollar();
	afx_msg void OnBtnPassChkSharp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_PASS_CHECK_H__3DD1B306_8FEB_48E3_8324_5A6A7294BF61__INCLUDED_)
