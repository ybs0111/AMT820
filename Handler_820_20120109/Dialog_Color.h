#if !defined(AFX_DIALOG_COLOR_H__41B23817_6E6A_4F73_8312_AE4C0389EBCC__INCLUDED_)
#define AFX_DIALOG_COLOR_H__41B23817_6E6A_4F73_8312_AE4C0389EBCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Color.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Color dialog
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

class CDialog_Color : public CDialog
{
// Construction
public:
	void Init_Group();
	CDialog_Color(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Color)
	enum { IDD = IDD_DIALOG_COLOR };
	CMacButton	m_btn_o_text1;
	CMacButton	m_btn_o_grid1;
	CMacButton	m_btn_i_text1;
	CMacButton	m_btn_i_grid1;
	CMacButton	m_btn_ok;
	CMacButton	m_btn_o_text;
	CMacButton	m_btn_o_grid;
	CMacButton	m_btn_i_text;
	CMacButton	m_btn_i_grid;
	CXPGroupBox	m_group_output;
	CXPGroupBox	m_group_input;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Color)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Color)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnIGrid();
	afx_msg void OnBtnIText();
	afx_msg void OnBtnOGrid();
	afx_msg void OnBtnOText();
	afx_msg void OnBtnIGrid2();
	afx_msg void OnBtnIText2();
	afx_msg void OnBtnOGrid2();
	afx_msg void OnBtnOText2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_COLOR_H__41B23817_6E6A_4F73_8312_AE4C0389EBCC__INCLUDED_)
