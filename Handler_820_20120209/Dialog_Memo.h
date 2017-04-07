#if !defined(AFX_DIALOG_MEMO_H__0D7B5879_C601_4062_8C27_4337A1615EF6__INCLUDED_)
#define AFX_DIALOG_MEMO_H__0D7B5879_C601_4062_8C27_4337A1615EF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog_Memo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog_Memo dialog
#include "BtnST.h"          // 칼라 버튼 생성 클래스 추가  
#include "NewLabel.h"		// 칼라 텍스트 박스 생성 클래스 추가 
#include "EditEx.h"			// 칼라 에디트 박스 생성 클래스 추가 
#include "EXDigitST.h"		// 디지털 카운터 클래스
#include "LedButton.h"      // LED 버튼 클래스
#include "XPGroupBox.h"		// 칼라 그룹 박스 생성 클래스 추가 
#include "SxLogFont.h"
#include "MacButtons.h"
#include "xShadeButton.h"	// 조그 버튼 생성 클래스 추가
#include "GradientStatic.h" // 그라데이션 칼라 텍스트 박스 생성 클래스 추가 
#include "XpButton.h"		// XP 칼라 버튼 클래스
#include "Dialog_Message.h"
#include "Variable.h"	// Added by ClassView

class CDialog_Memo : public CDialog
{
// Construction
public:
	CDialog_Memo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog_Memo)
	enum { IDD = IDD_DIALOG_MEMO };
	CMacButton	m_btn_apply;
	CMacButton	m_btn_cancel;
	CXPGroupBox	m_group_memo;
	CString	m_strmemo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog_Memo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog_Memo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_MEMO_H__0D7B5879_C601_4062_8C27_4337A1615EF6__INCLUDED_)
