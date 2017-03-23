#if !defined(AFX_DIALOG_KEYBOARD_H__6A778609_5188_43AD_AF58_2B8B8F6822B0__INCLUDED_)
#define AFX_DIALOG_KEYBOARD_H__6A778609_5188_43AD_AF58_2B8B8F6822B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Keyboard.h : header file
//
#include "Variable.h"		// 전역 변수 정의 클래스 추가 
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "editex.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "sxlogfont.h"
#include "MacButtons.h"
/////////////////////////////////////////////////////////////////////////////
// CDialog_Keyboard dialog

class CDialog_Keyboard : public StandardDialog
{
// Construction
public:
	CDialog_Keyboard(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Keyboard)
	enum { IDD = IDD_DIALOG_KEYBOARD };
	CGradientStatic	m_msg_board;
	CXPGroupBox	m_group_board;
	CMacButton	m_btn_board_sharp;
	CMacButton	m_btn_board_rbrace_s;
	CMacButton	m_btn_board_rbrace_b;
	CMacButton	m_btn_board_line;
	CMacButton	m_btn_board_lbrace_s;
	CMacButton	m_btn_board_lbrace_b;
	CMacButton	m_btn_board_dot;
	CMacButton	m_btn_board_dollar;
	CMacButton	m_btn_board_div;
	CMacButton	m_btn_board_del;
	CMacButton	m_btn_board_back;
	CMacButton	m_btn_board_add;
	CMacButton	m_btn_board_z;
	CMacButton	m_btn_board_y;
	CMacButton	m_btn_board_x;
	CMacButton	m_btn_board_w;
	CMacButton	m_btn_board_v;
	CMacButton	m_btn_board_u;
	CMacButton	m_btn_board_t;
	CMacButton	m_btn_board_s;
	CMacButton	m_btn_board_r;
	CMacButton	m_btn_board_q;
	CMacButton	m_btn_board_p;
	CMacButton	m_btn_board_o;
	CMacButton	m_btn_board_n;
	CMacButton	m_btn_board_m;
	CMacButton	m_btn_board_l;
	CMacButton	m_btn_board_k;
	CMacButton	m_btn_board_j;
	CMacButton	m_btn_board_i;
	CMacButton	m_btn_board_h;
	CMacButton	m_btn_board_g;
	CMacButton	m_btn_board_f;
	CMacButton	m_btn_board_e;
	CMacButton	m_btn_board_d;
	CMacButton	m_btn_board_c;
	CMacButton	m_btn_board_b;
	CMacButton	m_btn_board_a;
	CMacButton	m_btn_board_9;
	CMacButton	m_btn_board_8;
	CMacButton	m_btn_board_7;
	CMacButton	m_btn_board_6;
	CMacButton	m_btn_board_5;
	CMacButton	m_btn_board_4;
	CMacButton	m_btn_board_3;
	CMacButton	m_btn_board_2;
	CMacButton	m_btn_board_1;
	CMacButton	m_btn_board_0;
	//}}AFX_DATA

// Attributes
private:
	CButton *mp_board_ok, *mp_board_cancel;		// 버튼에 대한 포인터 선언
	CFont* mp_board_font;						// 폰트 설정 변수 선언 

	/* ************************************************************************** */
	/* 타이틀 정보 임시 저장 변수 선언                                            */
	/* ************************************************************************** */
	CString mp_title_info;
	/* ************************************************************************** */

	/* ************************************************************************** */
	/* 칼라 에디트 박스 클래스 변수 선언                                          */
	/* ************************************************************************** */
	CEditEx m_edit_board_data;
	/* ************************************************************************** */

// Operations
public:
	BOOL OnKeyboard_Length_Check(CString strData);
	void OnKeyboard_Title_data_Transfer();
	void OnKeyboard_Title_data_Set();
	void OnKeyboard_Button_Set();
	void OnKeyboard_EditBoxSet();
	void OnKeyboard_Lable_Set();
	void OnKeyboard_GroupBox_Set();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Keyboard)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Keyboard)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnBoardBack();
	afx_msg void OnBtnBoardDel();
	virtual void OnOK();
	afx_msg void OnBtnBoard1();
	afx_msg void OnBtnBoard2();
	afx_msg void OnBtnBoard3();
	afx_msg void OnBtnBoard4();
	afx_msg void OnBtnBoard5();
	afx_msg void OnBtnBoard6();
	afx_msg void OnBtnBoard7();
	afx_msg void OnBtnBoard8();
	afx_msg void OnBtnBoard9();
	afx_msg void OnBtnBoard0();
	afx_msg void OnBtnBoardA();
	afx_msg void OnBtnBoardB();
	afx_msg void OnBtnBoardC();
	afx_msg void OnBtnBoardD();
	afx_msg void OnBtnBoardE();
	afx_msg void OnBtnBoardF();
	afx_msg void OnBtnBoardG();
	afx_msg void OnBtnBoardH();
	afx_msg void OnBtnBoardI();
	afx_msg void OnBtnBoardJ();
	afx_msg void OnBtnBoardK();
	afx_msg void OnBtnBoardL();
	afx_msg void OnBtnBoardM();
	afx_msg void OnBtnBoardN();
	afx_msg void OnBtnBoardO();
	afx_msg void OnBtnBoardP();
	afx_msg void OnBtnBoardQ();
	afx_msg void OnBtnBoardR();
	afx_msg void OnBtnBoardS();
	afx_msg void OnBtnBoardT();
	afx_msg void OnBtnBoardU();
	afx_msg void OnBtnBoardV();
	afx_msg void OnBtnBoardW();
	afx_msg void OnBtnBoardX();
	afx_msg void OnBtnBoardY();
	afx_msg void OnBtnBoardZ();
	afx_msg void OnBtnBoardLbraceS();
	afx_msg void OnBtnBoardRbraceS();
	afx_msg void OnBtnBoardDiv();
	afx_msg void OnBtnBoardLbraceB();
	afx_msg void OnBtnBoardRbraceB();
	afx_msg void OnBtnBoardDot();
	afx_msg void OnBtnBoardAdd();
	afx_msg void OnBtnBoardLine();
	afx_msg void OnBtnBoardDollar();
	afx_msg void OnBtnBoardSharp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_KEYBOARD_H__6A778609_5188_43AD_AF58_2B8B8F6822B0__INCLUDED_)
